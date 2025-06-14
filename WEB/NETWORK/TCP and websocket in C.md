Let's walk through the `getaddrinfo` function with sample data, illustrating how the input data is processed and how the related structures change at each step. We will use a hypothetical example where we want to resolve the hostname "example.com" on port 80 (HTTP).

### Sample Data

- **Hostname**: "example.com"
- **Service (Port)**: "80"
- **Hints**: We want to use IPv4 and TCP.

### Step-by-Step Breakdown

1. **Define the Hints Structure**:
   Before calling `getaddrinfo`, we need to set up the `addrinfo` hints structure.

   ```c
   struct addrinfo hints;
   memset(&hints, 0, sizeof(hints)); // Initialize to zero
   hints.ai_family = AF_INET;         // We want IPv4 addresses
   hints.ai_socktype = SOCK_STREAM;   // We want TCP sockets
   ```

   **State of `hints`**:
   ```c
   hints = {
       .ai_flags = 0,
       .ai_family = AF_INET,          // IPv4
       .ai_socktype = SOCK_STREAM,    // TCP
       .ai_protocol = 0,              // Default protocol
       .ai_addrlen = 0,
       .ai_addr = NULL,
       .ai_canonname = NULL,
       .ai_next = NULL
   };
   ```

2. **Call `getaddrinfo`**:
   Now we call `getaddrinfo` with the hostname, port, and hints.

   ```c
   struct addrinfo *result;
   int gai_result = getaddrinfo("example.com", "80", &hints, &result);
   ```

   **Input to `getaddrinfo`**:
   - `node`: "example.com"
   - `service`: "80"
   - `hints`: as defined above
   - `res`: a pointer that will be set to the first `addrinfo` structure

3. **Inside `getaddrinfo`**:
   The function performs DNS resolution and populates the linked list of `addrinfo` structures. Let's assume it resolves "example.com" to two IPv4 addresses: `93.184.216.34` and `93.184.216.35`.

   **State of `result` after `getaddrinfo`**:
   ```c
   result = {
       .ai_flags = 0,
       .ai_family = AF_INET,          // IPv4
       .ai_socktype = SOCK_STREAM,    // TCP
       .ai_protocol = IPPROTO_TCP,    // TCP protocol
       .ai_addrlen = sizeof(struct sockaddr_in),
       .ai_addr = (struct sockaddr *) &addr1, // Pointer to first address
       .ai_canonname = NULL,
       .ai_next = &addrinfo2          // Pointer to next addrinfo
   };

   // addr1 (first address)
   struct sockaddr_in addr1 = {
       .sin_family = AF_INET,
       .sin_port = htons(80),         // Port 80
       .sin_addr.s_addr = inet_addr("93.184.216.34") // First resolved IP
   };

   // addrinfo2 (second address)
   struct addrinfo addrinfo2 = {
       .ai_flags = 0,
       .ai_family = AF_INET,
       .ai_socktype = SOCK_STREAM,
       .ai_protocol = IPPROTO_TCP,
       .ai_addrlen = sizeof(struct sockaddr_in),
       .ai_addr = (struct sockaddr *) &addr2, // Pointer to second address
       .ai_canonname = NULL,
       .ai_next = NULL
   };

   // addr2 (second address)
   struct sockaddr_in addr2 = {
       .sin_family = AF_INET,
       .sin_port = htons(80),         // Port 80
       .sin_addr.s_addr = inet_addr("93.184.216.35") // Second resolved IP
   };
   ```

4. **Iterating Through the Linked List**:
   After `getaddrinfo` returns, we can iterate through the linked list of `addrinfo` structures to create sockets and attempt connections.

   ```c
   struct addrinfo *ptr;
   for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
       // Create a socket
       int sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
       if (sockfd == -1) {
           continue; // Try the next address
       }

       // Attempt to connect
       if (connect(sockfd, ptr->ai_addr, ptr->ai_addrlen) == 0) {
           // Successfully connected
           break; // Exit the loop
       }

       // Close the socket if connection failed
       closesocket(sockfd);
   }
   ```

   - **First Iteration**:
     - `ptr` points to the first `addrinfo` structure (address `93.184.216.34`).
     - A socket is created using `socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)`.
     - The `connect` function is called with the first address.
     - If successful, the loop breaks; otherwise, the socket is closed, and the loop continues.

   - **Second Iteration**:
     - If the first connection attempt fails, `ptr` moves to the next `addrinfo` structure (address `93.184.216.35`).
     - The same process is repeated: create a socket and attempt to connect.

5. **Freeing the Linked List**:
   After attempting to connect to all addresses, we need to free the linked list to avoid memory leaks.

   ```c
   freeaddrinfo(result);
   ```

   This function traverses the linked list and frees each `addrinfo` structure, ensuring that all allocated memory is properly released.

### Summary of Data Changes

- **Input Data**:
  - Hostname: "example.com"
  - Service: "80"
  - Hints: Specifies IPv4 and TCP.

- **Output Data**:
  - `result`: A linked list of `addrinfo` structures containing resolved addresses.
    - First `addrinfo`: Points to `93.184.216.34`.
    - Second `addrinfo`: Points to `93.184.216.35`.

- **Socket Creation and Connection**:
  - For each address in the linked list, a socket is created, and a connection attempt is made.

- **Memory Management**:
  - The linked list is freed after use to prevent memory leaks.

This example illustrates how `getaddrinfo` processes input data to produce a linked list of address structures, which can then be used to establish network connections. The linked list structure allows for multiple addresses to be returned, accommodating scenarios where a hostname resolves to several IP addresses.

---
---
The concept of masking in the WebSocket protocol is an important aspect of ensuring data integrity and security during communication. Masking is primarily used to obfuscate the payload data sent from the client to the server, preventing certain types of attacks, such as cross-site WebSocket hijacking.

### Key Concepts of Masking in WebSocket

1. **Purpose of Masking**:
   - Masking is used to prevent intermediaries from easily reading the data being sent over the WebSocket connection.
   - It ensures that the data is not sent in a straightforward manner, adding a layer of security.

2. **Masking Requirement**:
   - According to the WebSocket protocol, the client must mask the payload data when sending messages to the server.
   - The server does not mask the data when sending messages back to the client.

3. **Masking Key**:
   - A masking key is a 32-bit value (4 bytes) generated by the client. This key is used to obfuscate the payload data.
   - The masking key is applied to the payload data using the XOR (exclusive OR) operation.

4. **Masking Process**:
   - The client generates a random masking key.
   - The client applies the masking key to each byte of the payload data before sending it to the server.
   - The server receives the masked data and must unmask it using the same masking key.

### Step-by-Step Visualization of Masking

#### 1. Client Sends a Message

**Example Payload**: Let's say the client wants to send the message "Hello".

**Step 1: Generate a Masking Key**

- The client generates a random 32-bit masking key. For this example, let's assume the key is `0x1A2B3C4D`.

**Step 2: Convert the Payload to Bytes**

- The payload "Hello" is converted to its byte representation:
  - `H` = 0x48
  - `e` = 0x65
  - `l` = 0x6C
  - `l` = 0x6C
  - `o` = 0x6F

**Step 3: Apply the Masking Key**

- The client applies the masking key to each byte of the payload using the XOR operation:

| Byte | Value (Hex) | Masking Key (Hex) | Result (Hex) | Result (Decimal) |
|------|-------------|--------------------|---------------|-------------------|
| 1    | 0x48       | 0x1A2B3C4D         | 0x48 ^ 0x1A = 0x52 | 82                |
| 2    | 0x65       | 0x2B3C4D1A         | 0x65 ^ 0x2B = 0x4E | 78                |
| 3    | 0x6C       | 0x3C4D1A2B         | 0x6C ^ 0x3C = 0x50 | 80                |
| 4    | 0x6C       | 0x4D1A2B3C         | 0x6C ^ 0x4D = 0x21 | 33                |
| 5    | 0x6F       | 0x1A2B3C4D         | 0x6F ^ 0x1A = 0x75 | 117               |

- The masked payload is now: `0x52 0x4E 0x50 0x21 0x75`.

**Step 4: Construct the WebSocket Frame**

- The client constructs the WebSocket frame to send to the server. The frame includes the masked payload and the masking key.

**WebSocket Frame Structure**:
- FIN bit: 1 (indicating the final fragment)
- Opcode: 0x1 (text frame)
- Mask bit: 1 (indicating the payload is masked)
- Payload length: 5 (length of "Hello")
- Masking key: `0x1A2B3C4D`
- Masked payload: `0x52 0x4E 0x50 0x21 0x75`

#### 2. Server Receives the Message

**Step 5: Unmask the Payload**

- Upon receiving the masked payload, the server extracts the masking key and applies it to the masked data to retrieve the original payload.

| Byte | Masked Value (Hex) | Masking Key (Hex) | Result (Hex) | Result (Character) |
|------|---------------------|--------------------|---------------|---------------------|
| 1    | 0x52                | 0x1A2B3C4D         | 0x52 ^ 0x1A = 0x48 | H                   |
| 2    | 0x4E                | 0x2B3C4D1A         | 0x4E ^ 0x2B = 0x65 | e                   |
| 3    | 0x50                | 0x3C4D1A2B         | 0x50 ^ 0x3C = 0x6C | l                   |
| 4    | 0x21                | 0x4D1A2B3C         | 0x21 ^ 0x4D = 0x6C | l                   |
| 5    | 0x75                | 0x1A2B3C4D         | 0x75 ^ 0x1A = 0x6F | o                   |

- The server reconstructs the original message: "Hello".

### Summary of Masking in WebSocket

- **Masking is Mandatory**: The client must mask the payload data when sending messages to the server, while the server does not mask the data sent back to the client.
- **Masking Key**: A 32-bit masking key is generated by the client and applied to each byte of the payload using the XOR operation.
- **Unmasking**: The server uses the same masking key to unmask the data and retrieve the original payload.
- **Security**: Masking helps prevent certain types of attacks by obfuscating the data being sent over the WebSocket connection.

This process ensures that the data transmitted over WebSocket connections is not easily readable by intermediaries, adding a layer of security to the communication.

----
---

## WebSocket Frame Structure

A WebSocket frame consists of the following parts:

1. **FIN, RSV1, RSV2, RSV3, Opcode (1 byte)**
2. **Mask and Payload Length (1 or 2 bytes)**
3. **Extended Payload Length (if needed, 2 or 8 bytes)**
4. **Masking Key (if needed, 4 bytes)**
5. **Payload Data (variable length)**

### Breakdown of the Frame Structure

#### 1. FIN, RSV1, RSV2, RSV3, Opcode (1 byte)

- **FIN (1 bit)**: Indicates if this is the final fragment in a message. If set to `1`, it means this is the last frame of the message.
- **RSV1, RSV2, RSV3 (1 bit each)**: Reserved for future use. These bits should be set to `0` unless an extension defines their use.
- **Opcode (4 bits)**: Indicates the type of frame being sent. Common opcodes include:
  - `0x0`: Continuation frame
  - `0x1`: Text frame (UTF-8 encoded)
  - `0x2`: Binary frame
  - `0x8`: Connection close
  - `0x9`: Ping
  - `0xA`: Pong

**Example**: If the first byte is `0x81`, it means:
- FIN = 1 (final frame)
- RSV1 = 0, RSV2 = 0, RSV3 = 0 (not used)
- Opcode = 0x1 (text frame)

#### 2. Mask and Payload Length (1 or 2 bytes)

- **Mask (1 bit)**: Indicates whether the payload data is masked. If set to `1`, the payload is masked (only applicable for frames sent from the client to the server).
- **Payload Length (7 bits)**: Indicates the length of the payload data. The length can be:
  - 0-125: The payload length is the value itself.
  - 126: The next 2 bytes represent the payload length (16 bits).
  - 127: The next 8 bytes represent the payload length (64 bits).

**Example**: If the second byte is `0x85`, it means:
- Mask = 1 (payload is masked)
- Payload Length = 5 (the length of the payload is 5 bytes)

#### 3. Extended Payload Length (if needed, 2 or 8 bytes)

- If the payload length is `126`, the next 2 bytes represent the actual payload length.
- If the payload length is `127`, the next 8 bytes represent the actual payload length.

**Example**: If the payload length is `126`, the next two bytes might be `0x00 0x05`, indicating a payload length of 5 bytes.

#### 4. Masking Key (if needed, 4 bytes)

- If the Mask bit is set to `1`, the next 4 bytes represent the masking key used to obfuscate the payload data.

**Example**: If the masking key is `0x1A2B3C4D`, it will be included in the frame after the extended payload length.

#### 5. Payload Data (variable length)

- The actual data being sent, which can be text, binary, or any other type of data. The length of this data is determined by the Payload Length field.

**Example**: If the payload data is "Hello", it will be represented in bytes as:
- `0x48 0x65 0x6C 0x6C 0x6F` (the ASCII values for 'H', 'e', 'l', 'l', 'o').

### Complete Example of a WebSocket Frame

Let's construct a complete WebSocket frame for sending the message "Hello" from a client to a server.

1. **Message**: "Hello"
2. **Payload Length**: 5 bytes
3. **Masking Key**: `0x1A2B3C4D`
4. **Masked Payload**:
   - Original Payload: `0x48 0x65 0x6C 0x6C 0x6F`
   - Masked Payload:
     - `0x48 ^ 0x1A = 0x52`
     - `0x65 ^ 0x2B = 0x4E`
     - `0x6C ^ 0x3C = 0x50`
     - `0x6C ^ 0x4D = 0x21`
     - `0x6F ^ 0x1A = 0x75`
   - Resulting Masked Payload: `0x52 0x4E 0x50 0x21 0x75`

### Frame Construction

- **FIN, RSV1, RSV2, RSV3, Opcode**: `0x81` (FIN=1, Opcode=0x1)
- **Mask, Payload Length**: `0x85` (Mask=1, Payload Length=5)
- **Masking Key**: `0x1A 0x2B 0x3C 0x4D`
- **Masked Payload**: `0x52 0x4E 0x50 0x21 0x75`

### Final Frame Representation

Putting it all together, the complete WebSocket frame sent from the client to the server would look like this in hexadecimal:

```
+---------------------+---------------------+---------------------+---------------------+---------------------+
| FIN, RSV, Opcode    | Mask, Payload Length | Extended Payload    | Masking Key         | Masked Payload      |
| 0x81                | 0x85                 | (none)              | 0x1A 0x2B 0x3C 0x4D | 0x52 0x4E 0x50 0x21 0x75 |
+---------------------+---------------------+---------------------+---------------------+---------------------+
```

### Summary

The WebSocket frame structure is designed to efficiently transmit data over a WebSocket connection. It includes fields for indicating the type of frame, whether the payload is masked, the length of the payload, and the actual data being sent. Masking is a critical feature that adds a layer of security by obfuscating the payload data sent from the client to the server. Understanding this structure is essential for implementing WebSocket communication effectively.
