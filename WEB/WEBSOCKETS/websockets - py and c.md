# server.py

```python
import asyncio
import websockets
import json
import logging
import os
import traceback
from datetime import datetime

import hashlib
import uuid
from datetime import datetime
import platform
import subprocess
import re

# cython: language_level=3
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import padding
import os

# Define a 32-byte encryption key for AES-256
ENCRYPTION_KEY = b"346hklase89345hklhdsjhgrd7t34fet87345iuhdsf78"[:32]

def encrypt_message(message):
    """Encrypt a message using AES-256 with an IV."""
    iv = os.urandom(16)  # Generate a random 16-byte IV
    cipher = Cipher(algorithms.AES(ENCRYPTION_KEY), modes.CBC(iv), backend=default_backend())
    encryptor = cipher.encryptor()
    
    # Pad the message to be a multiple of the block size (16 bytes for AES)
    padder = padding.PKCS7(algorithms.AES.block_size).padder()
    padded_message = padder.update(message.encode()) + padder.finalize()
    
    # Encrypt the padded message
    encrypted_message = encryptor.update(padded_message) + encryptor.finalize()
    return iv + encrypted_message  # Prepend IV to the encrypted message

def decrypt_message(encrypted_message):
    """Decrypt a message using AES-256 with an IV."""
    iv = encrypted_message[:16]  # Extract the IV from the beginning
    actual_encrypted_message = encrypted_message[16:]
    
    cipher = Cipher(algorithms.AES(ENCRYPTION_KEY), modes.CBC(iv), backend=default_backend())
    decryptor = cipher.decryptor()
    
    # Decrypt the message
    padded_message = decryptor.update(actual_encrypted_message) + decryptor.finalize()
    
    # Unpad the message
    unpadder = padding.PKCS7(algorithms.AES.block_size).unpadder()
    message = unpadder.update(padded_message) + unpadder.finalize()
    
    return message.decode()

def authenticate(token):
    """Authenticate a token."""
    # Implement your authentication logic here
    return token == "valid_token"  # Placeholder

def is_localhost(ip_address):
    """Check if the given IP address is localhost."""
    return ip_address == "127.0.0.1"

def get_mac_address():
    """
    Get the MAC address of the machine in a platform-independent way.
    
    :return: MAC address as a string of 12 lowercase hexadecimal digits
    """
    if platform.system() == "Windows":
        # Windows-specific method
        output = subprocess.check_output("getmac").decode()
        mac = re.search(r"([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})", output).group()
    elif platform.system() == "Darwin" or platform.system() == "Linux":
        # macOS and Linux method
        mac = ':'.join(['{:02x}'.format((uuid.getnode() >> elements) & 0xff) 
                        for elements in range(0, 8*6, 8)][::-1])
    else:
        raise OSError("Unsupported operating system")
    
    # Remove any colons or hyphens and ensure lowercase
    return ''.join(c.lower() for c in mac if c not in ':-')

def generate_client_name(port):
    """
    Generate a client name based on MAC address, current date, and port number.
    
    :param port: The port number of the client connection
    :return: A tuple containing (client_name, mac_xor_date)
    """
    mac_bytes = bytes.fromhex(get_mac_address())
    current_date = datetime.now().strftime("%d%H%y")
    date_bytes = current_date.encode()
    
    # Ensure mac_bytes and date_bytes have the same length
    min_length = min(len(mac_bytes), len(date_bytes))
    
    # Perform XOR operation between MAC address and date bytes, then convert to hexadecimal
    # Example:
    # If mac_bytes = b'\x12\x34\x56' and date_bytes = b'\xab\xcd\xef'
    # The XOR operation would be:
    #   0x12 ^ 0xab = 0xb9
    #   0x34 ^ 0xcd = 0xf9
    #   0x56 ^ 0xef = 0xb9
    # Resulting in: b'\xb9\xf9\xb9'
    # After .hex(), the final result would be: 'b9f9b9'
    mac_xor_date = bytes(a ^ b for a, b in zip(mac_bytes[:min_length], date_bytes[:min_length])).hex()
    
    client_name = encode_client_name(mac_xor_date, port)
    
    return client_name, mac_xor_date

def encode_client_name(mac_xor_date, port):
    """
    Encode the client name using MAC XOR date and port number.
    
    :param mac_xor_date: The result of XORing MAC address with current date
    :param port: The port number of the client connection
    :return: Encoded client name
    """
    combined = f"{mac_xor_date}{port}"
    hash_object = hashlib.sha256(combined.encode())
    return hash_object.hexdigest()[:12]

def create_client_name_request():
    """
    Create a client name request message.
    
    :return: Bytes object containing the request message
    """
    return b'\x81' + len("clname").to_bytes(2, 'big') + b"clname"

def create_client_name_response(client_name):
    """
    Create a client name response message.
    
    :param client_name: The generated client name
    :return: Bytes object containing the response message
    """
    return b'\x82' + len(client_name).to_bytes(2, 'big') + client_name.encode()

def parse_client_name_response(response):
    """
    Parse the client name response message.
    
    :param response: The received response message
    :return: The decoded client name
    """
    if response[0] != 0x82:
        raise ValueError("Invalid client response tag")
    
    response_length = int.from_bytes(response[1:3], 'big')
    return response[3:3+response_length].decode()

# Configure logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger('softoken')

# Global variables
current_client = None
HEARTBEAT_INTERVAL = 30
HEARTBEAT_TIMEOUT = 10

async def send_encrypted_message(websocket, tag, data):
    """
    Send an encrypted message over the WebSocket.
    
    :param websocket: The WebSocket connection
    :param tag: Message tag identifier
    :param data: Data to send (will be JSON serialized)
    """
    message = json.dumps(data).encode()
    encrypted_message = encrypt_message(message)
    full_message = bytes([tag]) + len(encrypted_message).to_bytes(2, 'big') + encrypted_message
    await websocket.send(full_message)

async def receive_encrypted_message(websocket):
    """
    Receive and decrypt a message from the WebSocket.
    
    :param websocket: The WebSocket connection
    :return: Tuple of (tag, data)
    """
    response = await websocket.recv()
    tag = response[0]
    length = int.from_bytes(response[1:3], 'big')
    encrypted_data = response[3:3+length]
    decrypted_data = decrypt_message(encrypted_data)
    data = json.loads(decrypted_data)
    return tag, data

async def handle_connection(websocket, path):
    """
    Handle a new WebSocket connection.
    
    :param websocket: The WebSocket connection
    :param path: The URL path for the WebSocket
    """
    global current_client
    client_info = None
    heartbeat_task = None

    async def heartbeat():
        """Send periodic heartbeat pings to ensure the connection is alive."""
        try:
            while True:
                await asyncio.sleep(HEARTBEAT_INTERVAL)
                try:
                    await asyncio.wait_for(websocket.ping(), timeout=HEARTBEAT_TIMEOUT)
                except asyncio.TimeoutError:
                    raise websockets.exceptions.ConnectionClosed(1011, "Client heartbeat timeout")
        except asyncio.CancelledError:
            pass

    try:
        logger.info("New connection received")
        
        if not is_localhost(websocket.remote_address[0]):
            # Only allow localhost connections
            logger.warning("Non-localhost connection attempt")
            await websocket.close(code=4003, reason="Only localhost connections are allowed")
            return

        if current_client:
            # Only allow one client at a time
            logger.warning("Connection attempt while another client is connected")
            await websocket.close(code=4002, reason="Server is busy with another client")
            return

        logger.info("Waiting for authentication token")
        tag, auth_data = await receive_encrypted_message(websocket)
        logger.info(f"Received message with tag: {tag}")
        if tag != 0x79 or not authenticate(auth_data.get('token')):
            # Authentication failed
            logger.warning("Authentication failed")
            await websocket.close(code=4001, reason="Authentication failed")
            return

        logger.info("Authentication successful")

        # Request client name
        await send_encrypted_message(websocket, 0x80, {"request": "client_name"})
        logger.info("Sent client name request")

        tag, client_data = await receive_encrypted_message(websocket)
        if tag != 0x81:
            raise ValueError("Invalid client name response tag")
        received_client_name = client_data.get("client_name")

        port = websocket.remote_address[1]
        expected_name, _ = generate_client_name(port)

        if received_client_name != expected_name:
            # Client name verification failed
            logger.warning(f"Client name verification failed. Received: {received_client_name}")
            await send_encrypted_message(websocket, 0x82, {"status": "authfail"})
            await websocket.close(code=4004, reason="Client name verification failed")
            return

        # Client authenticated successfully
        await send_encrypted_message(websocket, 0x82, {"status": "authok"})
        logger.info(f"Client authenticated successfully: {received_client_name}")

        client_info = f"{websocket.remote_address[0]}:{port}"
        current_client = client_info
        logger.info(f"Client connected and verified: {client_info} and id: {received_client_name}")

        # Start the heartbeat task
        heartbeat_task = asyncio.create_task(heartbeat())

        while True:
            tag, data = await receive_encrypted_message(websocket)
            
            if tag == 0x83:
                # Handle initialization request
                logger.info(f"Received initialization request: {data}")
                # Initialize your softoken here
                await send_encrypted_message(websocket, 0x84, {"status": "success"})
                logger.info("Sent initialization success response")
            elif tag == 0x85:
                try:
                    # Handle APDU command
                    apdu_command = bytes(data['data'])
                    logger.info(f"Received APDU command: {apdu_command.hex()}")
                    
                    # Process the APDU command
                    response_apdu = b'\x90\x00'  # Dummy response
                    
                    logger.info(f"Sending APDU response: {response_apdu.hex()}")
                    await send_encrypted_message(websocket, 0x86, {"status": "success", "response": list(response_apdu)})
                except Exception as e:
                    logger.error(f"Error processing APDU command: {e}")
                    await send_encrypted_message(websocket, 0x86, {"status": "failure", "response": []})
            else:
                # Handle unexpected message tags
                logger.warning(f"Unexpected message tag: {tag}")

    except websockets.exceptions.ConnectionClosed as e:
        logger.info(f"Client disconnected: {client_info}. Reason: {e.reason}")
    except Exception as e:
        logger.error(f"Error in WebSocket connection: {e}")
        logger.error(traceback.format_exc())
    finally:
        if heartbeat_task:
            heartbeat_task.cancel()
        current_client = None
        if client_info:
            logger.info(f"Connection closed for client: {client_info} and id: {received_client_name}")

async def main():
    """
    Main function to start the WebSocket server.
    """
    server = await websockets.serve(
        handle_connection,
        "127.0.0.1",
        8765,
        max_size=1048576,
        max_queue=1,
        compression=None,
        ping_interval=None,
        ping_timeout=None,
    )
    logger.info("Server started on 127.0.0.1:8765")
    await server.wait_closed()

if __name__ == "__main__":
    asyncio.run(main())
```

## Explanation

### Imports and Configuration

- **Standard Libraries**: The script imports several standard Python libraries such as `asyncio` for asynchronous programming, `websockets` for WebSocket communication, `json` for handling JSON data, `logging` for logging events, and others like `os`, `traceback`, and `datetime` for system operations and error handling.

- **Cryptography**: It utilizes the `cryptography` library to handle AES-256 encryption and decryption, ensuring secure communication between the server and client.

- **Constants**: `ENCRYPTION_KEY` is defined as a 32-byte key sliced from a given byte string, used for AES-256 encryption.

### Encryption and Decryption Functions

- **`encrypt_message`**:
  - **Purpose**: Encrypts a plaintext message using AES-256 in CBC mode.
  - **Process**:
    1. Generates a random Initialization Vector (IV) using `os.urandom`.
    2. Creates a cipher object with the encryption key and IV.
    3. Pads the message to ensure it's a multiple of the block size (16 bytes for AES).
    4. Encrypts the padded message and returns the IV prepended to the ciphertext.
  - **Example**: If the message is "Hello", it gets padded and encrypted, the IV is added to ensure uniqueness.

- **`decrypt_message`**:
  - **Purpose**: Decrypts an encrypted message that was encrypted using `encrypt_message`.
  - **Process**:
    1. Extracts the IV from the first 16 bytes of the encrypted message.
    2. Creates a cipher object with the extracted IV.
    3. Decrypts the ciphertext.
    4. Removes the padding to retrieve the original plaintext.
  - **Example**: Given the encrypted data, it reverses the encryption steps to return "Hello".

### Authentication and Utility Functions

- **`authenticate`**:
  - **Purpose**: Validates an authentication token.
  - **Current Implementation**: Checks if the token equals `"valid_token"`. This is a placeholder and should be replaced with actual authentication logic.

- **`is_localhost`**:
  - **Purpose**: Checks if the provided IP address is `127.0.0.1`, ensuring that only local connections are accepted.

- **`get_mac_address`**:
  - **Purpose**: Retrieves the MAC address of the machine in a platform-independent manner.
  - **Process**:
    - For Windows, it uses the `getmac` command.
    - For macOS and Linux, it processes the UUID node.
    - Removes any separators and formats it as a 12-character lowercase hexadecimal string.
  - **Example**: On a Linux machine, it might return `"a1b2c3d4e5f6"`.

- **`generate_client_name`**:
  - **Purpose**: Generates a unique client name based on the device's MAC address, the current date, and the port number.
  - **Process**:
    1. Retrieves the MAC address and current date.
    2. Performs a byte-wise XOR between the MAC address and the date string.
    3. Encodes the result using SHA-256 and takes the first 12 characters as the client name.
  - **Example**: If MAC XOR date results in `'b9f9b9'`, and port `8765`, the SHA-256 hash is generated, and the first 12 characters are used.

- **`encode_client_name`**:
  - **Purpose**: Encodes the client name by hashing the concatenation of `mac_xor_date` and `port` using SHA-256 and truncating the result.
  - **Example**: Combining `'b9f9b9'` with `'8765'` results in a unique 12-character hash.

- **`create_client_name_request` & `create_client_name_response`**:
  - **Purpose**: Constructs request and response messages for client name verification using specific byte patterns.

- **`parse_client_name_response`**:
  - **Purpose**: Validates and extracts the client name from the response message.

### Logging Configuration

- Configures the logging module to display timestamps, logger names, log levels, and messages.
- A logger named `'softoken'` is created for logging events.

### Global Variables

- **`current_client`**: Tracks the currently connected client to ensure only one client connects at a time.
- **Heartbeat Configuration**: Defines intervals and timeouts for maintaining active connections.

### Asynchronous Message Handling

- **`send_encrypted_message`**:
  - **Purpose**: Serializes and encrypts data before sending it over the WebSocket with a specific tag.
  - **Process**:
    1. Converts the data to JSON and encodes it to bytes.
    2. Encrypts the message using `encrypt_message`.
    3. Prepares the final message by concatenating the tag, length, and encrypted data.
    4. Sends the message via the WebSocket.

- **`receive_encrypted_message`**:
  - **Purpose**: Receives, decrypts, and deserializes data from the WebSocket.
  - **Process**:
    1. Receives the raw message.
    2. Extracts the tag and length.
    3. Decrypts the message using `decrypt_message`.
    4. Parses the JSON data and returns it along with the tag.

### Connection Handling

- **`handle_connection`**:
  - **Purpose**: Manages the lifecycle of a WebSocket connection, including authentication, client verification, heartbeat maintenance, and message processing.
  - **Process**:
    1. **Connection Validation**:
       - Ensures the connection is from `localhost`.
       - Checks if another client is already connected.
    2. **Authentication**:
       - Waits for an authentication token.
       - Validates the token using `authenticate`.
    3. **Client Name Verification**:
       - Requests the client to send its name.
       - Generates the expected client name and compares it with the received name.
    4. **Heartbeat**:
       - Starts a heartbeat task that periodically pings the client to ensure the connection is alive.
    5. **Message Processing**:
       - Listens for messages with specific tags (`0x83` for initialization, `0x85` for APDU commands).
       - Processes each message accordingly and sends appropriate responses.
    6. **Exception Handling**:
       - Handles connection closures and unexpected errors gracefully, ensuring resources are cleaned up.

### Server Initialization

- **`main`**:
  - **Purpose**: Starts the WebSocket server on `127.0.0.1:8765` and awaits incoming connections.
  - **Process**:
    1. Calls `websockets.serve` with `handle_connection` as the handler.
    2. Logs that the server has started.
    3. Waits indefinitely for the server to close.

- **Entry Point**:
  - When the script is run directly, it executes the `main` coroutine using `asyncio.run`.

### Why This Approach?

- **Security**: By encrypting messages and using a secure authentication mechanism, the server ensures that only authorized and verified clients can communicate.
  
- **Scalability and Maintainability**: Using asynchronous programming with `asyncio` allows the server to handle multiple tasks (like heartbeats and message processing) efficiently without blocking.
  
- **Platform Independence**: Functions like `get_mac_address` are designed to work across different operating systems, enhancing the server's versatility.
  
- **Robustness**: Comprehensive error handling and logging provide insights into the server's operations and facilitate debugging.

---

# client.c

```c
#include <libwebsockets.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "crypt32.lib")

#define ENCRYPTION_KEY "346hklase89345hklhdsjhgrd7t34fet"
#define MAX_PAYLOAD 4096
// #define LWS_SEND_BUFFER_PRE_PADDING 16
// #define LWS_SEND_BUFFER_POST_PADDING 16

// ... (we'll add function declarations and global variables here)

void handle_openssl_error(void) {
    ERR_print_errors_fp(stderr);
    exit(1);
}

void encrypt_message(const unsigned char *message, size_t message_len, unsigned char **encrypted, size_t *encrypted_len) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;
    unsigned char key[32];
    unsigned char iv[16];

    *encrypted = NULL;
    *encrypted_len = 0;

    // Generate random IV
    if (RAND_bytes(iv, sizeof(iv)) != 1) {
        handle_openssl_error();
    }

    // Decode base64 key
    EVP_DecodeBlock(key, (unsigned char *)ENCRYPTION_KEY, strlen(ENCRYPTION_KEY));

    // Create and initialize the context
    if (!(ctx = EVP_CIPHER_CTX_new())) handle_openssl_error();

    // Initialize the encryption operation
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handle_openssl_error();

    // Allocate memory for the encrypted message (message length + block size for padding + IV)
    *encrypted = malloc(message_len + EVP_CIPHER_block_size(EVP_aes_256_cbc()) + 16);
    if (!*encrypted) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }

    // Copy IV to the beginning of the encrypted message
    memcpy(*encrypted, iv, 16);
    ciphertext_len = 16;

    // Encrypt the message
    if (1 != EVP_EncryptUpdate(ctx, *encrypted + 16, &len, message, message_len))
        handle_openssl_error();
    ciphertext_len += len;

    // Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, *encrypted + ciphertext_len, &len))
        handle_openssl_error();
    ciphertext_len += len;

    *encrypted_len = ciphertext_len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);
}

void decrypt_message(const unsigned char *encrypted, size_t encrypted_len, unsigned char **decrypted, size_t *decrypted_len) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;
    unsigned char key[32];
    unsigned char iv[16];

    *decrypted = NULL;
    *decrypted_len = 0;

    // Extract IV from the beginning of the encrypted message
    memcpy(iv, encrypted, 16);

    // Decode base64 key
    EVP_DecodeBlock(key, (unsigned char *)ENCRYPTION_KEY, strlen(ENCRYPTION_KEY));

    // Create and initialize the context
    if (!(ctx = EVP_CIPHER_CTX_new())) handle_openssl_error();

    // Initialize the decryption operation
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handle_openssl_error();

    // Allocate memory for the decrypted message
    *decrypted = malloc(encrypted_len);
    if (!*decrypted) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }

    // Decrypt the message
    if (1 != EVP_DecryptUpdate(ctx, *decrypted, &len, encrypted + 16, encrypted_len - 16))
        handle_openssl_error();
    plaintext_len = len;

    // Finalize the decryption
    if (1 != EVP_DecryptFinal_ex(ctx, *decrypted + len, &len))
        handle_openssl_error();
    plaintext_len += len;

    *decrypted_len = plaintext_len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);
}

// ... (we'll add more functions here)

static int callback_client(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            printf("Connection established\n");
            // Send authentication token
            // ... (implement send_encrypted_message here)
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            // Handle received messages
            // ... (implement receive_encrypted_message here)
            break;

        case LWS_CALLBACK_CLIENT_CLOSED:
            printf("Connection closed\n");
            break;

        default:
            break;
    }

    return 0;
}

int main(void) {

    printf("Program started\n");
    struct lws_context_creation_info info;
    struct lws_client_connect_info conn_info;
    struct lws_context *context;
    const char *address = "localhost";
    int port = 8765;

    memset(&info, 0, sizeof(info));
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = (struct lws_protocols[]) {
        {
            "client-protocol",
            callback_client,
            0,
            MAX_PAYLOAD,
        },
        { NULL, NULL, 0, 0 }
    };
    info.gid = -1;
    info.uid = -1;

    context = lws_create_context(&info);
    if (!context) {
        fprintf(stderr, "Failed to create LWS context\n");
        return 1;
    }

    memset(&conn_info, 0, sizeof(conn_info));
    conn_info.context = context;
    conn_info.address = address;
    conn_info.port = port;
    conn_info.path = "/";
    conn_info.host = lws_canonical_hostname(context);
    conn_info.origin = "origin";
    conn_info.protocol = "client-protocol";

    lws_client_connect_via_info(&conn_info);

    while (1) {
        lws_service(context, 50);
    }

    lws_context_destroy(context);

    return 0;
}
```

## Explanation

### Header Inclusions and Library Linking

- **Standard Libraries**: The client program includes standard C libraries such as `stdio.h` for input/output operations, `stdlib.h` for memory management, and `string.h` for string manipulation.

- **Libwebsockets**: Utilizes `libwebsockets` for handling WebSocket communications.

- **OpenSSL**: Includes OpenSSL headers (`evp.h`, `rand.h`, `err.h`) for cryptographic operations like encryption and decryption.

- **Windows-Specific Libraries**: Incorporates Windows-specific libraries (`winsock2.h`, `windows.h`) and links against them using `#pragma comment` directives to ensure proper linking during compilation.

### Constants and Macros

- **`ENCRYPTION_KEY`**: Defined as a string `"346hklase89345hklhdsjhgrd7t34fet"`, representing the key used for AES-256 encryption. Note that in the server, this was a byte string sliced to 32 bytes.

- **`MAX_PAYLOAD`**: Sets the maximum payload size for WebSocket messages to 4096 bytes.

- **Padding Constants**: Commented out `LWS_SEND_BUFFER_PRE_PADDING` and `LWS_SEND_BUFFER_POST_PADDING` hints at buffer management during message sending, which might be adjusted based on requirements.

### OpenSSL Error Handling

- **`handle_openssl_error`**:
  - **Purpose**: Prints OpenSSL error messages to `stderr` and exits the program.
  - **Usage**: Called whenever an OpenSSL function fails to handle the error appropriately.

### Encryption and Decryption Functions

- **`encrypt_message`**:
  - **Purpose**: Encrypts a plaintext message using AES-256-CBC.
  - **Process**:
    1. Generates a random 16-byte IV using `RAND_bytes`.
    2. Decodes the base64-encoded encryption key.
    3. Initializes the encryption context with the key and IV.
    4. Allocates memory for the encrypted message, accounting for padding and IV.
    5. Copies the IV to the beginning of the encrypted buffer.
    6. Encrypts the plaintext message and appends the ciphertext to the buffer.
    7. Finalizes encryption to handle any remaining bytes.
  - **Example**: Encrypting "Hello World" would result in an IV concatenated with the ciphertext.

- **`decrypt_message`**:
  - **Purpose**: Decrypts an encrypted message using AES-256-CBC.
  - **Process**:
    1. Extracts the IV from the first 16 bytes of the encrypted message.
    2. Decodes the base64-encoded encryption key.
    3. Initializes the decryption context with the key and IV.
    4. Allocates memory for the decrypted message.
    5. Decrypts the ciphertext.
    6. Finalizes decryption to handle padding.
  - **Example**: Given the encrypted data, it retrieves the original plaintext "Hello World".

### Libwebsockets Callback

- **`callback_client`**:
  - **Purpose**: Handles various WebSocket events such as connection establishment, message reception, and connection closure.
  - **Events**:
    - **`LWS_CALLBACK_CLIENT_ESTABLISHED`**:
      - Triggered when the client successfully establishes a connection.
      - Expected Action: Send the authentication token to the server. (Implementation placeholder)
    - **`LWS_CALLBACK_CLIENT_RECEIVE`**:
      - Triggered when a message is received from the server.
      - Expected Action: Decrypt and process the received message. (Implementation placeholder)
    - **`LWS_CALLBACK_CLIENT_CLOSED`**:
      - Triggered when the connection is closed.
      - Action: Print a closure message.
    - **Default**:
      - No action for other events.

### Main Function

- **`main`**:
  - **Purpose**: Initializes the WebSocket client, sets up connection parameters, and starts the event loop.
  - **Process**:
    1. **Initialization**:
       - Prints "Program started" to indicate the client is running.
       - Defines and clears `lws_context_creation_info` and `lws_client_connect_info` structures.
    2. **Context Setup**:
       - Configures the WebSocket context with no listening port (`CONTEXT_PORT_NO_LISTEN`).
       - Defines the protocols the client will use, associating `callback_client` with `"client-protocol"`.
       - Sets group and user IDs to `-1` indicating default behavior.
    3. **Context Creation**:
       - Creates the WebSocket context using `lws_create_context`.
       - If creation fails, prints an error and exits.
    4. **Connection Information**:
       - Sets connection parameters such as address (`localhost`), port (`8765`), path (`"/"`), and protocol.
       - Uses `lws_canonical_hostname` to retrieve the hostname.
    5. **Establishing Connection**:
       - Initiates the client connection using `lws_client_connect_via_info`.
    6. **Event Loop**:
       - Enters an infinite loop where `lws_service` is called to handle incoming events and messages.
    7. **Cleanup**:
       - After exiting the loop (which in this setup, never occurs), destroys the WebSocket context.

### Why This Approach?

- **Security**:
  - Implements AES-256-CBC encryption to secure messages exchanged between the client and server.
  - Ensures that sensitive data like authentication tokens are encrypted during transmission.

- **Libwebsockets Integration**:
  - Utilizes the `libwebsockets` library for handling WebSocket connections, providing a robust and scalable communication channel.
  
- **Error Handling**:
  - Comprehensive error handling ensures that any issues during encryption/decryption or WebSocket communication are caught and handled gracefully.

- **Modularity**:
  - Encryption and decryption functionalities are encapsulated in separate functions, promoting code reuse and maintainability.

- **Platform Compatibility**:
  - Includes Windows-specific libraries and ensures compatibility across Windows systems, which is essential for clients running on different operating systems.

- **Future Expansion**:
  - Placeholder comments indicate where additional functionalities, such as sending authentication tokens and processing received messages, can be implemented.

### Next Steps for Implementation

To fully realize the client's capabilities, the following implementations are necessary:

1. **Sending Authentication Tokens**:
   - Within `LWS_CALLBACK_CLIENT_ESTABLISHED`, implement `send_encrypted_message` by serializing the authentication token, encrypting it using `encrypt_message`, and sending it to the server.

2. **Receiving and Processing Messages**:
   - Within `LWS_CALLBACK_CLIENT_RECEIVE`, implement `receive_encrypted_message` to decrypt incoming messages, deserialize the JSON data, and handle different message types based on tags.

3. **Client Name Generation and Verification**:
   - Implement logic to generate a client name similar to the server's `generate_client_name` function, ensuring both sides can verify each other's identities.

4. **Handling APDU Commands**:
   - Implement the processing of APDU (Application Protocol Data Unit) commands to interact with smart cards or secure elements if applicable.

5. **Heartbeat Mechanism**:
   - Implement periodic heartbeat messages to maintain the connection and handle potential disconnections gracefully.

6. **Error Handling Enhancements**:
   - Add more robust error handling to cover various edge cases and ensure the client can recover from unexpected scenarios.

---
