# Understanding # Understanding WebSocket Protocol: Client-Side Perspective

WebSocket is a protocol that provides full-duplex communication channels over a single TCP connection. Let me explain how WebSockets work from a client perspective, following the RFC 6455 standard, with a detailed look at the data flow.

## WebSocket Protocol Flow

The WebSocket protocol consists of two main phases:
1. Opening handshake
2. Data transfer

Let's examine each phase in detail with actual bytes being transmitted.

## 1. Opening Handshake

The handshake begins with the client sending an HTTP upgrade request to the server.

### Client Handshake Request

```
GET /chat HTTP/1.1
Host: server.example.com
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Origin: http://example.com
Sec-WebSocket-Protocol: chat
Sec-WebSocket-Version: 13
```

Let's break down what's happening in this request:

- `GET /chat HTTP/1.1`: Standard HTTP request line with the resource path
- `Host: server.example.com`: The server hostname
- `Upgrade: websocket`: Indicates a request to upgrade the connection
- `Connection: Upgrade`: Specifies that this is an upgrade request
- `Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==`: A randomly generated base64-encoded key
- `Origin: http://example.com`: The origin of the requesting client
- `Sec-WebSocket-Protocol: chat`: The subprotocol the client wants to use
- `Sec-WebSocket-Version: 13`: The WebSocket protocol version

The `Sec-WebSocket-Key` is particularly important. In our example code, we can see this being generated:

```c
// Generate random key
srand((unsigned int)time(NULL));
for (int i = 0; i < 16; i++) {
    key[i] = rand() % 256;
}
char* base64_key = base64_encode(key, 16);
strncpy_s(encoded_key, sizeof(encoded_key), base64_key, 24);
encoded_key[24] = '\0';
free(base64_key);
```

### Server Handshake Response

If the server accepts the connection, it responds with:

```
HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=
Sec-WebSocket-Protocol: chat
```

The key part here is the `Sec-WebSocket-Accept` header. The server calculates this value by:

1. Taking the client's `Sec-WebSocket-Key` value
2. Concatenating it with the GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
3. Computing the SHA-1 hash of this concatenated string
4. Base64-encoding the resulting hash

For example:
- Client key: `dGhlIHNhbXBsZSBub25jZQ==`
- Concatenated with GUID: `dGhlIHNhbXBsZSBub25jZQ==258EAFA5-E914-47DA-95CA-C5AB0DC85B11`
- SHA-1 hash (in hex): `b37a4f2cc0624f1690f64606cf385945b2bec4ea`
- Base64-encoded: `s3pPLMBiTxaQ9kYGzzhZRbK+xOo=`

The client must verify this value to ensure the server actually processed the WebSocket handshake.

## 2. Data Transfer

After the handshake, both client and server can send data frames to each other. Let's look at the frame structure:

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
|                     Payload Data continued ...                |
+---------------------------------------------------------------+
```

### Example: Client Sending a Text Message

Let's say the client wants to send the text message "Hello, WebSocket!":

1. **Frame Construction**:
   - FIN bit = 1 (final fragment)
   - RSV1-3 bits = 0 (no extensions)
   - Opcode = 0x1 (text frame)
   - MASK bit = 1 (client-to-server messages must be masked)
   - Payload length = 17 (length of "Hello, WebSocket!")
   - Masking key = 4 random bytes, e.g., [0x37, 0xFA, 0x21, 0x3D]
   - Payload data = "Hello, WebSocket!" (masked)

2. **Raw Bytes Sent on Wire**:
   ```
   0x81 0x91 0x37 0xFA 0x21 0x3D 0x7F 0x9A 0x40 0x58 0x7B 0x99 0x42 0x59 0x7E 0x9A 0x42 0x5C 0x7F 0x9A 0x40
   ```

   Breaking this down:
   - `0x81`: 10000001 in binary (FIN=1, RSV1-3=0, Opcode=1)
   - `0x91`: 10010001 in binary (MASK=1, Payload length=17)
   - `0x37 0xFA 0x21 0x3D`: The 4-byte masking key
   - Remaining bytes: The masked payload data

3. **Masking Process**:
   The original text "Hello, WebSocket!" is masked by XORing each byte with the corresponding byte of the masking key (cycling through the key):
   
   ```
   Original: H    e    l    l    o    ,         W    e    b    S    o    c    k    e    t    !
   ASCII:    72   101  108  108  111  44   32   87   101  98   83   111  99   107  101  116  33
   Key:      37   FA   21   3D   37   FA   21   3D   37   FA   21   3D   37   FA   21   3D   37
   XOR:      127  154  64   88   123  153  66   89   126  154  66   92   127  154  64   91   4
   ```

### Example: Server Sending a Text Message Response

When the server responds with "Welcome to the chat server!":

1. **Frame Construction**:
   - FIN bit = 1 (final fragment)
   - RSV1-3 bits = 0 (no extensions)
   - Opcode = 0x1 (text frame)
   - MASK bit = 0 (server-to-client messages are not masked)
   - Payload length = 27 (length of "Welcome to the chat server!")
   - Payload data = "Welcome to the chat server!" (unmasked)

2. **Raw Bytes Sent on Wire**:
   ```
   0x81 0x1B 0x57 0x65 0x6C 0x63 0x6F 0x6D 0x65 0x20 0x74 0x6F 0x20 0x74 0x68 0x65 0x20 0x63 0x68 0x61 0x74 0x20 0x73 0x65 0x72 0x76 0x65 0x72 0x21
   ```

   Breaking this down:
   - `0x81`: 10000001 in binary (FIN=1, RSV1-3=0, Opcode=1)
   - `0x1B`: 00011011 in binary (MASK=0, Payload length=27)
   - Remaining bytes: The ASCII values for "Welcome to the chat server!"

## Control Frames

WebSocket also defines control frames for protocol-level signaling:

### Ping Frame (from server to client)

```
0x89 0x05 0x48 0x65 0x6C 0x6C 0x6F
```

This is a ping with the payload "Hello":
- `0x89`: 10001001 in binary (FIN=1, RSV1-3=0, Opcode=9 for Ping)
- `0x05`: 00000101 in binary (MASK=0, Payload length=5)
- Remaining bytes: "Hello" in ASCII

### Pong Response (from client to server)

```
0x8A 0x85 0x37 0xFA 0x21 0x3D 0x7F 0x9A 0x40 0x58
```

This is a pong with the same payload, but masked:
- `0x8A`: 10001010 in binary (FIN=1, RSV1-3=0, Opcode=10 for Pong)
- `0x85`: 10000101 in binary (MASK=1, Payload length=5)
- `0x37 0xFA 0x21 0x3D`: The 4-byte masking key
- Remaining bytes: The masked "Hello" payload

### Close Frame (initiated by client)

```
0x88 0x82 0x37 0xFA 0x21 0x3D 0x03 0xE8
```

This is a close frame with status code 1000 (normal closure):
- `0x88`: 10001000 in binary (FIN=1, RSV1-3=0, Opcode=8 for Close)
- `0x82`: 10000010 in binary (MASK=1, Payload length=2)
- `0x37 0xFA 0x21 0x3D`: The 4-byte masking key
- `0x03 0xE8`: The masked status code 1000 (normal closure)

## Complete Communication Flow Visualization

Here's a visualization of a complete WebSocket communication:

```
CLIENT                                                SERVER
  |                                                     |
  |  HTTP Upgrade Request                               |
  | ------------------------------------------------->  |
  |  GET /chat HTTP/1.1                                 |
  |  Host: server.example.com                           |
  |  Upgrade: websocket                                 |
  |  Connection: Upgrade                                |
  |  Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==        |
  |  Origin: http://example.com                         |
  |  Sec-WebSocket-Protocol: chat                       |
  |  Sec-WebSocket-Version: 13                          |
  |                                                     |
  |                         HTTP Upgrade Response       |
  |  <-------------------------------------------------  |
  |                         HTTP/1.1 101 Switching      |
  |                         Protocols                   |
  |                         Upgrade: websocket          |
  |                         Connection: Upgrade         |
  |                         Sec-WebSocket-Accept:       |
  |                         s3pPLMBiTxaQ9kYGzzhZRbK+xOo=|
  |                         Sec-WebSocket-Protocol: chat|
  |                                                     |
  |  Text Frame (masked)                                |
  | ------------------------------------------------->  |
  |  0x81 0x91 0x37 0xFA 0x21 0x3D 0x7F 0x9A...         |
  |  ("Hello, WebSocket!")                              |
  |                                                     |
  |                         Text Frame (unmasked)       |
  |  <-------------------------------------------------  |
  |                         0x81 0x1B 0x57 0x65 0x6C... |
  |                         ("Welcome to the chat...")  |
  |                                                     |
  |                         Ping Frame                  |
  |  <-------------------------------------------------  |
  |                         0x89 0x05 0x48 0x65 0x6C... |
  |                         ("Hello")                   |
  |                                                     |
  |  Pong Frame (masked)                                |
  | ------------------------------------------------->  |
  |  0x8A 0x85 0x37 0xFA 0x21 0x3D 0x7F 0x9A...         |
  |  ("Hello")                                          |
  |                                                     |
  |  Close Frame (masked)                               |
  | ------------------------------------------------->  |
  |  0x88 0x82 0x37 0xFA 0x21 0x3D 0x03 0xE8            |
  |  (Status code: 1000 - Normal Closure)               |
  |                                                     |
  |                         Close Frame (unmasked)      |
  |  <-------------------------------------------------  |
  |                         0x88 0x02 0x03 0xE8         |
  |                         (Status code: 1000)         |
  |                                                     |
  |  TCP Connection Closed                              |
  | <------------------------------------------------>  |
  |                                                     |
```

## Key Points About WebSocket Client Implementation

1. **Security Considerations**:
   - All frames from client to server must be masked to prevent cache poisoning attacks
   - The masking key should be randomly generated for each frame
   - The client must verify the server's Sec-WebSocket-Accept value

2. **Frame Types**:
   - Text frames (opcode 0x1): UTF-8 encoded text data
   - Binary frames (opcode 0x2): Binary data
   - Control frames:
     - Close (opcode 0x8): Initiates the closing handshake
     - Ping (opcode 0x9): Used to check if connection is still alive
     - Pong (opcode 0xA): Response to a ping

3. **Fragmentation**:
   - Large messages can be split into multiple frames
   - First fragment has the appropriate opcode (0x1 for text, 0x2 for binary)
   - Continuation fragments use opcode 0x0
   - Only the final fragment has the FIN bit set to 1

4. **Connection Closure**:
   - Either side can initiate closure by sending a Close frame
   - The other side should respond with a Close frame
   - After sending and receiving a Close frame, the TCP connection is closed

The WebSocket protocol provides a clean, efficient way to establish bidirectional communication between client and server, with minimal overhead after the initial handshake. This makes it ideal for real-time applications like chat, gaming, and live data feeds.

## References

[RFC 6455: The WebSocket Protocol](https://datatracker.ietf.org/doc/html/rfc6455)
: Client-Side Perspective

WebSocket is a protocol that provides full-duplex communication channels over a single TCP connection. Let me explain how WebSockets work from a client perspective, following the RFC 6455 standard, with a detailed look at the data flow.

## WebSocket Protocol Flow

The WebSocket protocol consists of two main phases:
1. Opening handshake
2. Data transfer

Let's examine each phase in detail with actual bytes being transmitted.

## 1. Opening Handshake

The handshake begins with the client sending an HTTP upgrade request to the server.

### Client Handshake Request

```
GET /chat HTTP/1.1
Host: server.example.com
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Origin: http://example.com
Sec-WebSocket-Protocol: chat
Sec-WebSocket-Version: 13
```

Let's break down what's happening in this request:

- `GET /chat HTTP/1.1`: Standard HTTP request line with the resource path
- `Host: server.example.com`: The server hostname
- `Upgrade: websocket`: Indicates a request to upgrade the connection
- `Connection: Upgrade`: Specifies that this is an upgrade request
- `Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==`: A randomly generated base64-encoded key
- `Origin: http://example.com`: The origin of the requesting client
- `Sec-WebSocket-Protocol: chat`: The subprotocol the client wants to use
- `Sec-WebSocket-Version: 13`: The WebSocket protocol version

The `Sec-WebSocket-Key` is particularly important. In our example code, we can see this being generated:

```c
// Generate random key
srand((unsigned int)time(NULL));
for (int i = 0; i < 16; i++) {
    key[i] = rand() % 256;
}
char* base64_key = base64_encode(key, 16);
strncpy_s(encoded_key, sizeof(encoded_key), base64_key, 24);
encoded_key[24] = '\0';
free(base64_key);
```

### Server Handshake Response

If the server accepts the connection, it responds with:

```
HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=
Sec-WebSocket-Protocol: chat
```

The key part here is the `Sec-WebSocket-Accept` header. The server calculates this value by:

1. Taking the client's `Sec-WebSocket-Key` value
2. Concatenating it with the GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
3. Computing the SHA-1 hash of this concatenated string
4. Base64-encoding the resulting hash

For example:
- Client key: `dGhlIHNhbXBsZSBub25jZQ==`
- Concatenated with GUID: `dGhlIHNhbXBsZSBub25jZQ==258EAFA5-E914-47DA-95CA-C5AB0DC85B11`
- SHA-1 hash (in hex): `b37a4f2cc0624f1690f64606cf385945b2bec4ea`
- Base64-encoded: `s3pPLMBiTxaQ9kYGzzhZRbK+xOo=`

The client must verify this value to ensure the server actually processed the WebSocket handshake.

## 2. Data Transfer

After the handshake, both client and server can send data frames to each other. Let's look at the frame structure:

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
|                     Payload Data continued ...                |
+---------------------------------------------------------------+
```

### Example: Client Sending a Text Message

Let's say the client wants to send the text message "Hello, WebSocket!":

1. **Frame Construction**:
   - FIN bit = 1 (final fragment)
   - RSV1-3 bits = 0 (no extensions)
   - Opcode = 0x1 (text frame)
   - MASK bit = 1 (client-to-server messages must be masked)
   - Payload length = 17 (length of "Hello, WebSocket!")
   - Masking key = 4 random bytes, e.g., [0x37, 0xFA, 0x21, 0x3D]
   - Payload data = "Hello, WebSocket!" (masked)

2. **Raw Bytes Sent on Wire**:
   ```
   0x81 0x91 0x37 0xFA 0x21 0x3D 0x7F 0x9A 0x40 0x58 0x7B 0x99 0x42 0x59 0x7E 0x9A 0x42 0x5C 0x7F 0x9A 0x40
   ```

   Breaking this down:
   - `0x81`: 10000001 in binary (FIN=1, RSV1-3=0, Opcode=1)
   - `0x91`: 10010001 in binary (MASK=1, Payload length=17)
   - `0x37 0xFA 0x21 0x3D`: The 4-byte masking key
   - Remaining bytes: The masked payload data

3. **Masking Process**:
   The original text "Hello, WebSocket!" is masked by XORing each byte with the corresponding byte of the masking key (cycling through the key):
   
   ```
   Original: H    e    l    l    o    ,         W    e    b    S    o    c    k    e    t    !
   ASCII:    72   101  108  108  111  44   32   87   101  98   83   111  99   107  101  116  33
   Key:      37   FA   21   3D   37   FA   21   3D   37   FA   21   3D   37   FA   21   3D   37
   XOR:      127  154  64   88   123  153  66   89   126  154  66   92   127  154  64   91   4
   ```

### Example: Server Sending a Text Message Response

When the server responds with "Welcome to the chat server!":

1. **Frame Construction**:
   - FIN bit = 1 (final fragment)
   - RSV1-3 bits = 0 (no extensions)
   - Opcode = 0x1 (text frame)
   - MASK bit = 0 (server-to-client messages are not masked)
   - Payload length = 27 (length of "Welcome to the chat server!")
   - Payload data = "Welcome to the chat server!" (unmasked)

2. **Raw Bytes Sent on Wire**:
   ```
   0x81 0x1B 0x57 0x65 0x6C 0x63 0x6F 0x6D 0x65 0x20 0x74 0x6F 0x20 0x74 0x68 0x65 0x20 0x63 0x68 0x61 0x74 0x20 0x73 0x65 0x72 0x76 0x65 0x72 0x21
   ```

   Breaking this down:
   - `0x81`: 10000001 in binary (FIN=1, RSV1-3=0, Opcode=1)
   - `0x1B`: 00011011 in binary (MASK=0, Payload length=27)
   - Remaining bytes: The ASCII values for "Welcome to the chat server!"

## Control Frames

WebSocket also defines control frames for protocol-level signaling:

### Ping Frame (from server to client)

```
0x89 0x05 0x48 0x65 0x6C 0x6C 0x6F
```

This is a ping with the payload "Hello":
- `0x89`: 10001001 in binary (FIN=1, RSV1-3=0, Opcode=9 for Ping)
- `0x05`: 00000101 in binary (MASK=0, Payload length=5)
- Remaining bytes: "Hello" in ASCII

### Pong Response (from client to server)

```
0x8A 0x85 0x37 0xFA 0x21 0x3D 0x7F 0x9A 0x40 0x58
```

This is a pong with the same payload, but masked:
- `0x8A`: 10001010 in binary (FIN=1, RSV1-3=0, Opcode=10 for Pong)
- `0x85`: 10000101 in binary (MASK=1, Payload length=5)
- `0x37 0xFA 0x21 0x3D`: The 4-byte masking key
- Remaining bytes: The masked "Hello" payload

### Close Frame (initiated by client)

```
0x88 0x82 0x37 0xFA 0x21 0x3D 0x03 0xE8
```

This is a close frame with status code 1000 (normal closure):
- `0x88`: 10001000 in binary (FIN=1, RSV1-3=0, Opcode=8 for Close)
- `0x82`: 10000010 in binary (MASK=1, Payload length=2)
- `0x37 0xFA 0x21 0x3D`: The 4-byte masking key
- `0x03 0xE8`: The masked status code 1000 (normal closure)

## Complete Communication Flow Visualization

Here's a visualization of a complete WebSocket communication:

```
CLIENT                                                SERVER
  |                                                     |
  |  HTTP Upgrade Request                               |
  | ------------------------------------------------->  |
  |  GET /chat HTTP/1.1                                 |
  |  Host: server.example.com                           |
  |  Upgrade: websocket                                 |
  |  Connection: Upgrade                                |
  |  Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==        |
  |  Origin: http://example.com                         |
  |  Sec-WebSocket-Protocol: chat                       |
  |  Sec-WebSocket-Version: 13                          |
  |                                                     |
  |                         HTTP Upgrade Response       |
  |  <-------------------------------------------------  |
  |                         HTTP/1.1 101 Switching      |
  |                         Protocols                   |
  |                         Upgrade: websocket          |
  |                         Connection: Upgrade         |
  |                         Sec-WebSocket-Accept:       |
  |                         s3pPLMBiTxaQ9kYGzzhZRbK+xOo=|
  |                         Sec-WebSocket-Protocol: chat|
  |                                                     |
  |  Text Frame (masked)                                |
  | ------------------------------------------------->  |
  |  0x81 0x91 0x37 0xFA 0x21 0x3D 0x7F 0x9A...         |
  |  ("Hello, WebSocket!")                              |
  |                                                     |
  |                         Text Frame (unmasked)       |
  |  <-------------------------------------------------  |
  |                         0x81 0x1B 0x57 0x65 0x6C... |
  |                         ("Welcome to the chat...")  |
  |                                                     |
  |                         Ping Frame                  |
  |  <-------------------------------------------------  |
  |                         0x89 0x05 0x48 0x65 0x6C... |
  |                         ("Hello")                   |
  |                                                     |
  |  Pong Frame (masked)                                |
  | ------------------------------------------------->  |
  |  0x8A 0x85 0x37 0xFA 0x21 0x3D 0x7F 0x9A...         |
  |  ("Hello")                                          |
  |                                                     |
  |  Close Frame (masked)                               |
  | ------------------------------------------------->  |
  |  0x88 0x82 0x37 0xFA 0x21 0x3D 0x03 0xE8            |
  |  (Status code: 1000 - Normal Closure)               |
  |                                                     |
  |                         Close Frame (unmasked)      |
  |  <-------------------------------------------------  |
  |                         0x88 0x02 0x03 0xE8         |
  |                         (Status code: 1000)         |
  |                                                     |
  |  TCP Connection Closed                              |
  | <------------------------------------------------>  |
  |                                                     |
```

## Key Points About WebSocket Client Implementation

1. **Security Considerations**:
   - All frames from client to server must be masked to prevent cache poisoning attacks
   - The masking key should be randomly generated for each frame
   - The client must verify the server's Sec-WebSocket-Accept value

2. **Frame Types**:
   - Text frames (opcode 0x1): UTF-8 encoded text data
   - Binary frames (opcode 0x2): Binary data
   - Control frames:
     - Close (opcode 0x8): Initiates the closing handshake
     - Ping (opcode 0x9): Used to check if connection is still alive
     - Pong (opcode 0xA): Response to a ping

3. **Fragmentation**:
   - Large messages can be split into multiple frames
   - First fragment has the appropriate opcode (0x1 for text, 0x2 for binary)
   - Continuation fragments use opcode 0x0
   - Only the final fragment has the FIN bit set to 1

4. **Connection Closure**:
   - Either side can initiate closure by sending a Close frame
   - The other side should respond with a Close frame
   - After sending and receiving a Close frame, the TCP connection is closed

The WebSocket protocol provides a clean, efficient way to establish bidirectional communication between client and server, with minimal overhead after the initial handshake. This makes it ideal for real-time applications like chat, gaming, and live data feeds.

## References

[RFC 6455: The WebSocket Protocol](https://datatracker.ietf.org/doc/html/rfc6455)
