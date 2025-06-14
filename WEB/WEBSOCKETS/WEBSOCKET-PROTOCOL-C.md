

---

## Table of Contents

1. [Understanding WebSocket Frames](#understanding-websocket-frames)
2. [Masking and Its Importance](#masking-and-its-importance)
3. [Key WebSocket Terms and Concepts](#key-websocket-terms-and-concepts)
4. [WebSocket vs. Raw TCP Sockets](#websocket-vs-raw-tcp-sockets)
5. [Visualization of Data Transmission](#visualization-of-data-transmission)
6. [Detailed Code Explanation](#detailed-code-explanation)
   - [File: MINIMAL_CLIENT.c](#file-minimal_clientc)
   - [Function: send_websocket_frame](#function-send_websocket_frame)
   - [Function: receive_websocket_frame](#function-receive_websocket_frame)
   - [JSON Handling Functions](#json-handling-functions)
7. [Building Your WebSocket Library](#building-your-websocket-library)
8. [Conclusion](#conclusion)

---

## Understanding WebSocket Frames

### What is a WebSocket Frame?

A **WebSocket frame** is the fundamental unit of communication in the WebSocket protocol as defined by [RFC 6455](https://tools.ietf.org/html/rfc6455). Frames are structured packets that encapsulate data being sent between a client and server, enabling full-duplex (two-way) communication.

### Structure of a WebSocket Frame

A WebSocket frame consists of several components:

1. **FIN Bit (1 bit):**
   
   - Indicates if this is the final fragment in a message.
   - `1` means final frame; `0` indicates more frames follow.

2. **Opcode (4 bits):**
   
   - Defines the interpretation of the payload data.
   - Common opcodes:
     - `0x0`: Continuation Frame
     - `0x1`: Text Frame
     - `0x2`: Binary Frame
     - `0x8`: Connection Close
     - `0x9`: Ping
     - `0xA`: Pong

3. **MASK Bit (1 bit):**
   
   - Determines if the payload data is masked.
   - **Client-to-Server:** Always masked (`1`).
   - **Server-to-Client:** Not masked (`0`).

4. **Payload Length (7 bits or more):**
   
   - If ≤ 125: Represents the actual payload length.
   - `126`: Indicates that the following 2 bytes represent the payload length.
   - `127`: Indicates that the following 8 bytes represent the payload length.

5. **Masking Key (0 or 4 bytes):**
   
   - Present only if the MASK bit is set.
   - A 4-byte key used to unmask the payload data.

6. **Payload Data (Variable Length):**
   
   - Actual data being transmitted.

### Example Frame Breakdown

Consider sending the message "Hello":

```
+-----+-----+----------+----------+
| FIN | OPC | MASK LEN | PAYLOAD  |
+-----+-----+----------+----------+
| 1   | 1   | 1 0000101| ...      |
+-----+-----+----------+----------+
```

- **FIN:** `1` (Final frame)
- **Opcode:** `1` (Text frame)
- **MASK:** `1` (Masked)
- **Payload Length:** `5` (Length of "Hello")
- **Masking Key:** Random 4-byte key
- **Payload Data:** "Hello" XORed with the Masking Key

---

## Masking and Its Importance

### What is Masking?

**Masking** is a security mechanism in WebSockets where the client masks the payload data sent to the server. This involves XOR-ing each byte of the payload with a byte from the masking key. The primary goals are:

1. **Security:** Prevents malicious injection of scripts or data via intermediary proxies or caches.
2. **Protocol Compliance:** Ensures that WebSocket traffic is safely transmitted over existing HTTP infrastructure.

### How Masking Works

1. **Generate Masking Key:**
   
   - A random 4-byte key is generated for each frame.

2. **Apply Masking Operation:**
   
   - Each byte of the payload is XOR-ed with a corresponding byte from the masking key (repeating the key as necessary).

3. **Transmission:**
   
   - The masked payload and the masking key are sent to the server.

4. **Unmasking at Server:**
   
   - The server applies the same XOR operation using the masking key to retrieve the original payload.

### Example

Original Payload: `H` (`0x48`)
Masking Key Byte: `0x5D`

```
Masked Byte: 0x48 ^ 0x5D = 0x15
Unmasked Byte: 0x15 ^ 0x5D = 0x48 ('H')
```

### Important Notes

- **Client-to-Server:** Masking is **mandatory**.
- **Server-to-Client:** Masking is **not used**.

---

## Key WebSocket Terms and Concepts

1. **FIN Bit:**
   
   - Indicates the final fragment of a message.
   - Supports message fragmentation.

2. **Opcode:**
   
   - Specifies the frame type (e.g., text, binary, close).

3. **MASK Bit:**
   
   - Determines if masking is applied.
   - Ensures payload data is obscured in transit.

4. **Payload Length:**
   
   - Specifies the size of the payload.
   - Supports extended lengths for large messages.

5. **Masking Key:**
   
   - A 4-byte key for masking/unmasking payload data.

6. **Extended Payload Length:**
   
   - Used when payload size exceeds 125 bytes.

7. **Control Frames:**
   
   - Special frames for protocol-level communication (e.g., Ping, Pong, Close).

8. **Data Frames:**
   
   - Carry application-specific data (e.g., Text, Binary).

9. **Fragmentation:**
   
   - Splitting large messages into smaller frames.

10. **Handshake:**
    
    - The initial HTTP request-response to establish a WebSocket connection.

11. **Subprotocol:**
    
    - An application-level protocol layered over WebSockets (e.g., MQTT).

12. **Extension:**
    
    - Additional protocol features negotiated during the handshake (e.g., compression).

---

## WebSocket vs. Raw TCP Sockets

Understanding the differences between WebSockets and raw TCP sockets is crucial for implementing a WebSocket library.

### Raw TCP Sockets

- **Protocol:** TCP is a low-level protocol that facilitates the transmission of raw byte streams between client and server.
- **Usage:** Suitable for applications that require custom protocols or where developers manage framing and data parsing.
- **Features:**
  - Full control over data transmission.
  - No built-in message framing or masking.
  - Requires manual handling of connection management, reconnection, and data parsing.

### WebSockets

- **Protocol:** WebSockets operate on top of TCP, providing a standardized full-duplex communication channel.
- **Usage:** Ideal for web applications requiring real-time data exchange (e.g., chat applications, live feeds).
- **Features:**
  - **Framing:** Built-in message framing with metadata.
  - **Masking:** Automatic masking of client-to-server messages for security.
  - **Protocol Upgrade:** Initiates as an HTTP connection, then upgrades to WebSocket.
  - **Built-in Control Frames:** Supports Ping/Pong for keep-alive and connection management.
  - **Standard API:** Simplifies implementation with standardized mechanisms.

### Comparison Table

| Feature              | Raw TCP Sockets                       | WebSockets                                             |
| -------------------- | ------------------------------------- | ------------------------------------------------------ |
| **Protocol Layer**   | Transport Layer (TCP)                 | Application Layer (HTTP Upgrade)                       |
| **Framing**          | None (raw byte stream)                | Built-in message framing                               |
| **Masking**          | Optional (developer-implemented)      | Mandatory for client-to-server messages                |
| **Control Frames**   | None (requires custom implementation) | Ping/Pong, Close frames                                |
| **Connection Setup** | Simple TCP connection                 | HTTP handshake followed by protocol upgrade            |
| **Use Cases**        | Custom protocols, low-level control   | Real-time web applications, standardized communication |

---

## Visualization of Data Transmission

### Raw TCP Socket Data Transmission

When sending a message like "Hello, World!" over a raw TCP socket:

```
+----------------------------+
| H | e | l | l | o | , | ... |
+----------------------------+
```

- **Content:** Sent as-is without any additional metadata or framing.
- **Length Information:** Not inherently included; the application must manage data boundaries.

### WebSocket Frame Data Transmission

Sending the same message over a WebSocket connection encapsulates it within a structured frame:

```
+--------+--------+------------+--------------------+
| Header | Length | Masking    | Masked Payload     |
|  (2B)  |  (1B)  | Key (4B)   |     (13B)          |
+--------+--------+------------+--------------------+
|   81   |  8D    | 5D 38 A5 51| 61 21 B0  ...      |
+--------+--------+------------+--------------------+
```

- **Header (2 bytes):**
  
  - `0x81`: FIN=1, Opcode=0x1 (Text frame)
  - `0x8D`: MASK=1, Payload Length=13 (`0x0D`)

- **Masking Key (4 bytes):**
  
  - `5D 38 A5 51`: Randomly generated for each frame.

- **Masked Payload (13 bytes):**
  
  - Original "Hello, World!" XORed with the masking key.

### Diagram Representation

#### Raw TCP Socket:

```
+---------------------------+
| Hello, World!             |
+---------------------------+
```

#### WebSocket Frame:

```
+--------+--------+------------+---------------------+
| Header | Length | Masking    | Masked Payload      |
| 0x81   | 0x8D   | 5D 38 A5 51 | XORed data bytes    |
+--------+--------+------------+---------------------+
```

---

## Detailed Code Explanation

Let's analyze the relevant portions of your `MINIMAL_CLIENT.c` to understand how WebSocket frames are constructed and handled.

### File: `MINIMAL_CLIENT.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <time.h>
#include <cjson/cJSON.h>  // Reintroduce this line

#pragma comment(lib, "ws2_32.lib")

#define SERVER "127.0.0.1"
#define PORT 8765
#define BUFFER_SIZE 2048

// Function prototypes
int send_websocket_frame(SOCKET sock, const char* message);
int receive_websocket_frame(SOCKET sock, char* buffer, int buffer_size);
char* create_json_message(const char* message);
void parse_json_response(const char* json_string);

// ... (rest of the code)
```

#### Explanation

1. **Libraries Included:**
   
   - **Standard C Libraries:** `stdio.h`, `stdlib.h`, `string.h`, `time.h`
   - **Windows Networking Libraries:** `winsock2.h`, `ws2tcpip.h`, `windows.h`
   - **JSON Handling Library:** `cjson/cJSON.h`

2. **Pragma Comment:**
   
   - Links against the Winsock library (`ws2_32.lib`).

3. **Macros Defined:**
   
   - **SERVER:** IP address of the WebSocket server (`127.0.0.1`).
   - **PORT:** Port number (`8765`).
   - **BUFFER_SIZE:** Size of the receive buffer (`2048` bytes).

4. **Function Prototypes:**
   
   - `send_websocket_frame`: Sends a WebSocket frame.
   - `receive_websocket_frame`: Receives a WebSocket frame.
   - `create_json_message`: Creates a JSON-formatted message.
   - `parse_json_response`: Parses a JSON-formatted response.

---

### Function: `send_websocket_frame`

```c
int send_websocket_frame(SOCKET sock, const char* message) {
    int payload_length = strlen(message);
    int frame_size;
    int header_size;

    // Determine frame size and header size based on payload length
    if (payload_length <= 125) {
        frame_size = 2 + 4 + payload_length;
        header_size = 2;
    } else if (payload_length <= 65535) {
        frame_size = 4 + 4 + payload_length;
        header_size = 4;
    } else {
        frame_size = 10 + 4 + payload_length;
        header_size = 10;
    }

    unsigned char* frame = (unsigned char*)malloc(frame_size);
    if (!frame) {
        printf("Memory allocation failed\n");
        return -1;
    }

    // Set FIN and Opcode for Text Frame
    frame[0] = 0x81; // FIN=1, Opcode=1 (Text)

    // Set Mask bit and Payload length
    if (payload_length <= 125) {
        frame[1] = 0x80 | (unsigned char)payload_length;
    } else if (payload_length <= 65535) {
        frame[1] = 0xFE; // 126 + mask bit
        frame[2] = (payload_length >> 8) & 0xFF;
        frame[3] = payload_length & 0xFF;
    } else {
        frame[1] = 0xFF; // 127 + mask bit
        for (int i = 0; i < 8; i++) {
            frame[2 + i] = (payload_length >> ((7 - i) * 8)) & 0xFF;
        }
    }

    // Generate a random masking key
    srand((unsigned int)time(NULL));
    unsigned char mask[4];
    for (int i = 0; i < 4; i++) {
        mask[i] = rand() % 256;
        frame[header_size + i] = mask[i];
    }

    // Mask the payload
    for (int i = 0; i < payload_length; i++) {
        frame[header_size + 4 + i] = message[i] ^ mask[i % 4];
    }

    // Send the frame
    int total_sent = 0;
    while (total_sent < frame_size) {
        int sent = send(sock, (char*)frame + total_sent, frame_size - total_sent, 0);
        if (sent == SOCKET_ERROR) {
            printf("send failed: %ld\n", WSAGetLastError());
            free(frame);
            return -1;
        }
        total_sent += sent;
    }

    free(frame);
    return total_sent;
}
```

#### Step-by-Step Explanation

1. **Calculate Payload Length:**
   
   - Determines the size of the message to be sent.

2. **Determine Frame and Header Sizes:**
   
   - **Payload ≤ 125 bytes:**
     - **Frame Size:** `2 (header) + 4 (mask) + payload_length`.
     - **Header Size:** `2` bytes.
   - **126 ≤ Payload ≤ 65535 bytes:**
     - **Frame Size:** `4 (extended header) + 4 (mask) + payload_length`.
     - **Header Size:** `4` bytes.
   - **Payload > 65535 bytes:**
     - **Frame Size:** `10 (extended header) + 4 (mask) + payload_length`.
     - **Header Size:** `10` bytes.

3. **Allocate Memory for Frame:**
   
   - Dynamically allocates memory for the entire frame.

4. **Set FIN and Opcode:**
   
   - `0x81`: `FIN=1` (final frame), `Opcode=1` (text frame).

5. **Set MASK Bit and Payload Length:**
   
   - **MASK Bit:** Always `1` for client-to-server frames.
   - **Payload Length:**
     - If ≤ 125, set directly.
     - If between 126 and 65535, use two additional bytes.
     - If > 65535, use eight additional bytes.

6. **Generate Masking Key:**
   
   - Initializes a random mask using `rand()`.
   - **Note:** Using `srand()` with `time(NULL)` may lead to predictable masks; consider using a more secure random generator for production.

7. **Apply Masking to Payload:**
   
   - Each byte of the payload is XOR-ed with the corresponding byte of the masking key.

8. **Send the Frame:**
   
   - Sends the entire frame over the socket, handling partial sends.

9. **Cleanup:**
   
   - Frees the allocated memory after sending.

---

### Function: `receive_websocket_frame`

```c
int receive_websocket_frame(SOCKET sock, char* buffer, int buffer_size) {
    unsigned char header[2];
    int result = recv(sock, (char*)header, 2, 0);
    if (result != 2) {
        printf("Failed to read frame header\n");
        return -1;
    }

    int fin = (header[0] & 0x80) >> 7;
    int opcode = header[0] & 0x0F;
    int mask = (header[1] & 0x80) >> 7;
    int payload_length = header[1] & 0x7F;

    if (payload_length == 126) {
        unsigned char extended[2];
        result = recv(sock, (char*)extended, 2, 0);
        if (result != 2) {
            printf("Failed to read extended payload length\n");
            return -1;
        }
        payload_length = (extended[0] << 8) | extended[1];
    } else if (payload_length == 127) {
        unsigned char extended[8];
        result = recv(sock, (char*)extended, 8, 0);
        if (result != 8) {
            printf("Failed to read extended payload length\n");
            return -1;
        }
        payload_length = 0;
        for (int i = 0; i < 8; i++) {
            payload_length = (payload_length << 8) | extended[i];
        }
    }

    unsigned char mask_key[4];
    if (mask) {
        result = recv(sock, (char*)mask_key, 4, 0);
        if (result != 4) {
            printf("Failed to read mask key\n");
            return -1;
        }
    }

    if (payload_length > buffer_size - 1) {
        printf("Payload too large to handle\n");
        return -1;
    }

    int total_received = 0;
    while (total_received < payload_length) {
        result = recv(sock, buffer + total_received, (int)(payload_length - total_received), 0);
        if (result <= 0) {
            printf("Failed to read payload\n");
            return -1;
        }
        total_received += result;
    }

    if (mask) {
        for (int i = 0; i < payload_length; i++) {
            buffer[i] ^= mask_key[i % 4];
        }
    }

    buffer[payload_length] = '\0';
    return (int)payload_length;
}
```

#### Step-by-Step Explanation

1. **Receive Frame Header:**
   
   - Reads the first 2 bytes to get FIN, Opcode, MASK bit, and initial Payload Length.

2. **Extract Header Information:**
   
   - **FIN:** `(header[0] & 0x80) >> 7`
   - **Opcode:** `header[0] & 0x0F`
   - **Mask:** `(header[1] & 0x80) >> 7`
   - **Payload Length:** `header[1] & 0x7F`

3. **Handle Extended Payload Length:**
   
   - If Payload Length is `126`, read the next 2 bytes.
   - If Payload Length is `127`, read the next 8 bytes.
   - Concatenate bytes to form the full Payload Length.

4. **Receive Masking Key (if present):**
   
   - If the MASK bit is set, read the next 4 bytes as the masking key.

5. **Validate Payload Size:**
   
   - Ensures that the payload can fit into the provided buffer.

6. **Receive Payload Data:**
   
   - Continuously reads from the socket until the entire payload is received.

7. **Unmask Payload (if masked):**
   
   - Applies the masking key to retrieve the original payload.

8. **Null-Terminate and Return:**
   
   - Adds a null terminator to the buffer for string handling.
   - Returns the length of the received payload.

---

### JSON Handling Functions

#### Function: `create_json_message`

```c
char* create_json_message(const char* message) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "message", message);

    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);

    return json_str;
}
```

#### Function: `parse_json_response`

```c
void parse_json_response(const char* json_string) {
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "JSON parsing error: %s\n", error_ptr);
        }
        return;
    }

    cJSON *response = cJSON_GetObjectItemCaseSensitive(json, "response");
    if (cJSON_IsString(response) && (response->valuestring != NULL)) {
        printf("Parsed response: %s\n", response->valuestring);
    } else {
        printf("Unable to parse 'response' from JSON\n");
    }

    cJSON_Delete(json);
}
```

#### Explanation

- **create_json_message:**
  
  - Creates a JSON object with a single key-value pair (`"message": "Your Message"`).
  - Serializes the JSON object to a string using `cJSON_Print`.
  - Cleans up the JSON object to prevent memory leaks.

- **parse_json_response:**
  
  - Parses the received JSON string.
  - Extracts the value associated with the `"response"` key.
  - Prints the parsed response or an error message if parsing fails.

---

## Building Your WebSocket Library

To create a comprehensive WebSocket library in C, consider the following components:

1. **Connection Management:**
   
   - **Initialization:** Setting up Winsock.
   - **Handshake:** Implementing the HTTP upgrade request and response.
   - **Connection Closure:** Handling graceful shutdowns.

2. **Frame Handling:**
   
   - **Sending Frames:** Constructing frames with proper headers, masking, and payload.
   - **Receiving Frames:** Parsing frames, handling masking, and reconstructing messages.
   - **Fragmentation Support:** Managing messages split across multiple frames.

3. **Control Frame Management:**
   
   - **Ping/Pong:** Implementing keep-alive mechanisms.
   - **Close Frames:** Handling termination signals.

4. **Error Handling:**
   
   - Robust error checking and reporting at each stage of communication.

5. **Extensions and Subprotocols:**
   
   - Allowing for protocol extensions and custom subprotocols.

6. **Utility Functions:**
   
   - **Mask Generation:** Securely generating masking keys.
   - **Payload Encoding/Decoding:** Managing text and binary data.

### Recommended Structure

```
WebSocketLibrary/
├── include/
│   ├── websocket.h
│   └── cJSON.h
├── src/
│   ├── websocket.c
│   ├── frame.c
│   ├── handshake.c
│   └── utils.c
├── examples/
│   └── MINIMAL_CLIENT.c
├── lib/
│   └── cjson.lib
├── Makefile
└── README.md
```

### Steps to Develop

1. **Define Data Structures:**
   
   - Represent WebSocket frames, connections, and states.

2. **Implement Handshake:**
   
   - Craft the HTTP Upgrade request with a valid `Sec-WebSocket-Key`.
   - Validate the server's `Sec-WebSocket-Accept` response.

3. **Frame Construction and Parsing:**
   
   - Modularize `send_websocket_frame` and `receive_websocket_frame` for reusability.
   - Handle different opcodes and fragmentation.

4. **Masking Operations:**
   
   - Securely generate random masking keys.
   - Apply masking/unmasking consistently for all payloads.

5. **Error and Edge Case Handling:**
   
   - Manage unexpected frames, invalid opcodes, payload size limits.

6. **Testing:**
   
   - Create comprehensive test cases, including edge scenarios.
   - Use WebSocket testing tools and servers to validate functionality.

7. **Documentation:**
   
   - Provide clear API documentation and usage examples.

--- 

### LARGE DATA HANDLING

---

 `ws_send` and `ws_recv` functions with sample data, explaining each step of the process.

Let's start with `ws_send`:

Assume we want to send the message "Hello, WebSocket!" (17 bytes) with an opcode of 0x01 (text frame).

```c
int ws_send(ws_ctx* ctx, const char* data, size_t length, int opcode) {
    // data = "Hello, WebSocket!"
    // length = 17
    // opcode = 0x01

    if (ctx->state != WS_STATE_OPEN) {
        return -1;  // Connection not open, return error
    }

    uint8_t header[14];  // Maximum header size (2 + 8 + 4)
    size_t header_size = 2;  // Initial header size
    uint32_t mask = generate_mask();  // Let's say this generates 0x12345678

    // Set FIN bit and opcode
    header[0] = 0x80 | (opcode & 0x0F);  // 0x81 (10000001)

    // Set payload length and mask bit
    header[1] = 0x80 | length;  // 0x91 (10010001) - masked, 17 bytes payload

    // Add the mask to the header
    memcpy(header + header_size, &mask, 4);
    header_size += 4;  // header_size is now 6

    // Allocate memory for the entire frame
    size_t frame_size = header_size + length;  // 6 + 17 = 23 bytes
    uint8_t* frame = (uint8_t*)malloc(frame_size);
    if (!frame) return -1;

    // Copy the header to the frame
    memcpy(frame, header, header_size);
    // frame now contains: 81 91 12 34 56 78

    // Apply the mask to the data and copy it to the frame
    uint32_t* mask_ptr = (uint32_t*)(frame + header_size - 4);
    uint32_t* data_ptr = (uint32_t*)(frame + header_size);
    size_t words = length / 4;  // 17 / 4 = 4 words
    size_t remaining_bytes = length % 4;  // 17 % 4 = 1 byte

    for (size_t i = 0; i < words; i++) {
        data_ptr[i] = ((uint32_t*)data)[i] ^ *mask_ptr;
    }
    // This masks 16 bytes of the message

    // Mask remaining byte
    for (size_t i = 0; i < remaining_bytes; i++) {
        frame[header_size + words * 4 + i] = data[words * 4 + i] ^ ((uint8_t*)mask_ptr)[i];
    }
    // This masks the last byte of the message

    // At this point, frame contains the full masked message

    // Send the entire frame
    int result = send(ctx->socket, (char*)frame, frame_size, 0);
    free(frame);

    if (result != frame_size) {
        printf("Send failed. Sent %d bytes out of %zu\n", result, frame_size);
        return -1;
    }

    return 0;
}
```

Now, let's look at `ws_recv`:

Assume we're receiving the same message "Hello, WebSocket!" (17 bytes) that we sent above.

```c
int ws_recv(ws_ctx* ctx, char* buffer, size_t buffer_size) {
    // buffer_size = 1024 (for example)

    if (ctx->state != WS_STATE_OPEN) {
        printf("Error: WebSocket not in OPEN state\n");
        return -1;
    }

    size_t total_received = 0;
    bool final_fragment = false;

    while (!final_fragment && total_received < buffer_size) {
        // Receive frame header
        uint8_t header[2];
        int bytes_received = recv(ctx->socket, (char*)header, 2, 0);
        // header now contains: 81 91

        final_fragment = header[0] & 0x80;  // true (0x81 & 0x80 = 0x80)
        int opcode = header[0] & 0x0F;  // 1 (text frame)
        bool masked = header[1] & 0x80;  // true
        uint64_t payload_length = header[1] & 0x7F;  // 17

        // No extended payload length needed for this message

        // Handle masking
        uint32_t mask = 0;
        if (masked) {
            bytes_received = recv(ctx->socket, (char*)&mask, 4, 0);
            // mask now contains: 0x12345678
        }

        // Receive payload data
        char* payload_buffer = (char*)malloc(payload_length);
        if (!payload_buffer) {
            printf("Error allocating memory for payload\n");
            return -1;
        }

        size_t bytes_to_receive = payload_length;  // 17
        size_t payload_received = 0;

        while (bytes_to_receive > 0) {
            bytes_received = recv(ctx->socket, payload_buffer + payload_received, bytes_to_receive, 0);
            payload_received += bytes_received;
            bytes_to_receive -= bytes_received;
        }
        // payload_buffer now contains the masked message

        // Unmask data
        for (size_t i = 0; i < payload_length; i++) {
            payload_buffer[i] ^= ((uint8_t*)&mask)[i % 4];
        }
        // payload_buffer now contains "Hello, WebSocket!"

        // Copy data to the output buffer
        size_t bytes_to_copy = (payload_length <= buffer_size - total_received) ? payload_length : buffer_size - total_received;
        memcpy(buffer + total_received, payload_buffer, bytes_to_copy);
        total_received += bytes_to_copy;  // total_received is now 17

        // Clean up
        free(payload_buffer);

        // Loop ends here because final_fragment is true
    }

    return total_received;  // Returns 17
}
```

In both functions, the process for handling larger messages (>125 bytes or >65535 bytes) would be similar, but with additional steps to handle the extended payload length fields. The masking/unmasking process would remain the same, just operating on more data.

These implementations can handle messages of any size (up to memory limits), as they use 64-bit integers for payload lengths and dynamically allocate memory for the frame/payload.
