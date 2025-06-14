

### Memory Layout Visualization (Byte by Byte)

```
Memory Address │ Byte Content    │ Bit Layout        │ Description
─────────────────────────────────────────────────────────────────────
Byte 0        │ 1000 0001       │ F─┬─RSV─┬─Opcode   First byte
              │                 │ 1 │ 000 │ 0001   │ FIN=1, RSV=000, Opcode=1 (text)
              │                 │   │     │        │
Byte 1        │ 1000 0110       │ M ┬────Length─── │ Second byte
              │                 │ 1 │  000110      │ MASK=1, Payload len=6
              │                 │   │              │
              │                 │   │              │
If len ≥ 126: │                 │                  │
Bytes 2-3     │ [16 bits]       │ Extended Length  │ Only if payload len = 126
   OR         │                 │                  │ (16-bit unsigned int)
Bytes 2-9     │ [64 bits]       │ Extended Length  │ Only if payload len = 127
              │                 │                │ (64-bit unsigned int)
              │                 │                │
If MASK=1:    │                 │                │
Bytes N..N+3  │ [32 bits]       │ Masking Key     │ 4 bytes for masking key
              │ e.g.:           │                │ (if MASK bit is set)
              │ A4 B5 C6 D7     │                │
              │                 │                │
Remaining     │ Payload data    │ Actual Data     │ Payload bytes
Bytes         │                 │                │ (masked if MASK=1)
```

### Real Example with 6-byte Text Message "Hello!"

```
Memory Layout (Hex View):
Address  │ 00 01 02 03 04 05 06 07 08 09 0A 0B
─────────┼────────────────────────────────────
0x000000 │ 81 86 A4 B5 C6 D7 2B 4C 5D 6E 7F 80

Breakdown byte by byte:

Byte 0 (0x81):
┌─────────┐
│1000 0001│
└─────────┘
 │││└┴┴┴┘
 ││└─RSV   → 000 (all reserved bits are 0)
 │└─FIN    → 1 (final fragment)
 └──Opcode → 0001 (text frame)

Byte 1 (0x86):
┌─────────┐
│1000 0110│
└─────────┘
 │└───┴┴┴┘
 └─MASK    → 1 (payload is masked)
   Length  → 000110 (6 bytes payload)

Bytes 2-5 (Masking Key):
┌─────────┬─────────┬─────────┬─────────┐
│A4       │B5       │C6       │D7       │
└─────────┴─────────┴─────────┴─────────┘

Bytes 6-11 (Masked Payload):
┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
│2B       │4C       │5D       │6E       │7F       │80       │
└─────────┴─────────┴─────────┴─────────┴─────────┴─────────┘
```

### Memory Layout for Different Payload Lengths

1. **Small Payload (0-125 bytes)**
```
┌────────┬────────┬────────────┬─────────┐
│Byte 0  │Byte 1  │Mask Key    │Payload  │
│Header  │Length  │(if masked) │Data     │
└────────┴────────┴────────────┴─────────┘
```

2. **Medium Payload (126-65535 bytes)**
```
┌────────┬────────┬────────┬────────────┬─────────┐
│Byte 0  │Byte 1  │2 Bytes │Mask Key    │Payload  │
│Header  │126     │Length  │(if masked) │Data     │
└────────┴────────┴────────┴────────────┴─────────┘
```

3. **Large Payload (65536+ bytes)**
```
┌────────┬────────┬────────┬────────────┬─────────┐
│Byte 0  │Byte 1  │8 Bytes │Mask Key    │Payload  │
│Header  │127     │Length  │(if masked) │Data     │
└────────┴────────┴────────┴────────────┴─────────┘
```

### Memory Alignment Note
- Each byte is aligned on a single byte boundary
- No padding is used between fields
- Multi-byte integers (extended length) are in network byte order (big-endian)
- The masking key and payload data follow immediately after the length fields
- Total frame size = Header + Length Fields + Masking Key (if present) + Payload Length

This layout is extremely efficient as it:
1. Uses minimal overhead for small messages (just 2 bytes of header)
2. Can scale up to very large messages (64-bit length field)
3. Maintains byte alignment throughout
4. Allows for easy parsing as all fields are at fixed offsets once the length is known

---
---


### WebSocket Frame Structure
```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)          |
|N|V|V|V|       |S|             |   (if payload len==126/127)  |
| |1|2|3|       |K|             |                              |
+-+-+-+-+-------+-+-------------+-------------------------------+
|     Extended payload length continued, if payload len == 127  |
+-------------------------------+-------------------------------+
|                               |
+     Masking-key, if MASK set |
|                               |
+-------------------------------+-------------------------------+
| Payload Data continued ...                                    |
+---------------------------------------------------------------+
```

### 1. Text Message Frame (Client to Server)
```
Example: Sending "Hello, WebSocket!"
Binary representation:

81 8B 7F 9F 4D 51 30 31 32 33 34 35 36 37 38 39

Breakdown:
81       -> Text frame (FIN=1, Opcode=1)
8B       -> Masked, payload length 11
7F9F4D51 -> Masking key
30...39  -> Masked payload ("Hello, WebSocket!")
```

### 2. Text Message Frame (Server to Client)
```
Example: Sending "Welcome!"
Binary representation:

81 08 57 65 6C 63 6F 6D 65 21

Breakdown:
81    -> Text frame (FIN=1, Opcode=1)
08    -> Unmasked, payload length 8
57... -> Raw payload ("Welcome!")
```

### 3. Binary Frame (Client to Server)
```
Example: Sending image data
Binary representation:

82 FE 01 00 A4 B5 C6 D7 [256 bytes of masked binary data]

Breakdown:
82          -> Binary frame (FIN=1, Opcode=2)
FE 01 00    -> Masked, payload length 256 (using 16-bit length)
A4B5C6D7    -> Masking key
[data]      -> Masked binary payload
```

### 4. Ping Frame (Server to Client)
```
Example: Ping with "ping" text
Binary representation:

89 04 70 69 6E 67

Breakdown:
89    -> Ping frame (FIN=1, Opcode=9)
04    -> Unmasked, payload length 4
70... -> Payload ("ping")
```

### 5. Pong Frame (Client to Server)
```
Example: Pong response with "ping" text
Binary representation:

8A 84 A1 B2 C3 D4 11 22 33 44

Breakdown:
8A       -> Pong frame (FIN=1, Opcode=A)
84       -> Masked, payload length 4
A1B2C3D4 -> Masking key
11223344 -> Masked payload ("ping")
```

### 6. Close Frame (Either Direction)
```
Example: Normal closure with reason "bye"
Binary representation:

88 85 03 E8 62 79 65

Breakdown:
88       -> Close frame (FIN=1, Opcode=8)
85       -> Payload length 5
03E8     -> Status code 1000 (normal closure)
627965   -> Reason ("bye")
```

### 7. Fragmented Message (Client to Server)
```
Example: Sending "Hello World" in fragments
First fragment:
01 85 A1 B2 C3 D4 48 65 6C 6C 6F  // "Hello"

Continuation fragment:
00 86 E1 F2 G3 H4 20 57 6F 72 6C   // " Worl"

Final fragment:
80 81 I1 J2 K3 L4 64               // "d"

Breakdown:
01    -> First fragment (FIN=0, Opcode=1 for text)
00    -> Continuation fragment (FIN=0, Opcode=0)
80    -> Final fragment (FIN=1, Opcode=0)
```

### 8. Large Binary Frame (Server to Client)
```
Example: Sending 65KB of data
Binary representation:

82 7F 00 00 00 00 00 01 00 00 [65536 bytes of data]

Breakdown:
82                -> Binary frame (FIN=1, Opcode=2)
7F                -> Unmasked, payload length indicator 127
0000000000010000  -> 64-bit payload length (65536)
[data]           -> Binary payload
```

These examples cover the most common WebSocket frame types and scenarios. Note that:
- Client-to-server messages are always masked
- Server-to-client messages are never masked
- The masking key is a 32-bit value
- Payload lengths can be 7 bits, 16 bits, or 64 bits
- Control frames (Ping, Pong, Close) must be ≤ 125 bytes and cannot be fragmented

---
---
---


### FIRST HEADER for WebSocket

Here's a detailed visualization of the WebSocket handshake HTTP request headers:

```
CLIENT REQUEST HEADERS:
─────────────────────────────────────────────────────────────────────────
GET /chat HTTP/1.1                    │ Request line (WebSocket endpoint)
Host: server.example.com              │ Server host
Upgrade: websocket                    │ Required - Indicates WebSocket upgrade
Connection: Upgrade                   │ Required - Connection must be upgraded
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==   │ Required - Random 16 bytes base64
Sec-WebSocket-Version: 13            │ Required - WebSocket protocol version
Origin: https://example.com          │ Optional - Client origin
                                    │
[Optional Headers]                   │
Sec-WebSocket-Protocol: chat, superchat  │ Optional - Subprotocols client supports
Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits  │ Optional - Extensions
User-Agent: Mozilla/5.0 ...          │ Optional - Client information
Cookie: session=abc123               │ Optional - Cookies if any
─────────────────────────────────────────────────────────────────────────

SERVER RESPONSE HEADERS:
─────────────────────────────────────────────────────────────────────────
HTTP/1.1 101 Switching Protocols     │ Response status for upgrade
Upgrade: websocket                   │ Required - Confirms WebSocket upgrade
Connection: Upgrade                  │ Required - Confirms connection upgrade
Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=  │ Required - Key verification
                                    │
[Optional Headers]                   │
Sec-WebSocket-Protocol: chat         │ Optional - Selected subprotocol
Sec-WebSocket-Extensions: permessage-deflate  │ Optional - Accepted extensions
─────────────────────────────────────────────────────────────────────────
```

### Detailed Header Explanations:

1. **Required Client Headers**:
```
GET /chat HTTP/1.1
│    │     └─ HTTP version must be 1.1+
│    └─ WebSocket endpoint path
└─ Must be GET method

Host: server.example.com
└─ Server hostname and optional port

Upgrade: websocket
└─ Case-insensitive value must be "websocket"

Connection: Upgrade
└─ Must include "Upgrade" (case-insensitive)

Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
└─ Random 16 bytes, base64 encoded
   (Server uses this to prove it's a WebSocket server)

Sec-WebSocket-Version: 13
└─ Must be 13 for current WebSocket protocol
```

2. **Optional Client Headers**:
```
Sec-WebSocket-Protocol: chat, superchat
└─ Comma-separated list of supported subprotocols

Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits
└─ Requested protocol extensions

Origin: https://example.com
└─ Client origin for security checks

Additional standard HTTP headers:
- User-Agent
- Cookie
- Authorization
- etc.
```

3. **Required Server Response Headers**:
```
HTTP/1.1 101 Switching Protocols
└─ Must be 101 status code

Upgrade: websocket
└─ Must match client header

Connection: Upgrade
└─ Must match client header

Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=
└─ Calculated value from client's Sec-WebSocket-Key:
   1. Append "258EAFA5-E914-47DA-95CA-C5AB0DC85B11" to key
   2. Take SHA-1 hash
   3. Base64 encode the hash
```

4. **Optional Server Response Headers**:
```
Sec-WebSocket-Protocol: chat
└─ Selected subprotocol from client's list

Sec-WebSocket-Extensions: permessage-deflate
└─ Accepted extensions with parameters
```

### Example Raw HTTP Message:
```
CLIENT REQUEST:
GET /chat HTTP/1.1
Host: server.example.com
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Sec-WebSocket-Version: 13
Origin: https://example.com
Sec-WebSocket-Protocol: chat, superchat
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)
[blank line]

SERVER RESPONSE:
HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=
Sec-WebSocket-Protocol: chat
[blank line]
```

After these headers are exchanged successfully, the WebSocket connection is established, and binary frame communication begins.
Let me break down the WebSocket key verification process step by step:

### 1. Client Side (Sec-WebSocket-Key)
```
Example Generation Process:
1. Generate 16 random bytes
   [Example bytes in hex]
   └─ 74 68 65 20 73 61 6D 70 6C 65 20 6E 6F 6E 63 65

2. Base64 encode these 16 bytes
   Result: "dGhlIHNhbXBsZSBub25jZQ=="
   └─ This becomes the Sec-WebSocket-Key
```

### 2. Server Side (Sec-WebSocket-Accept) Calculation
```
Step 1: Take client's Sec-WebSocket-Key
└─ "dGhlIHNhbXBsZSBub25jZQ=="

Step 2: Append Magic String (UUID defined in RFC 6455)
└─ Magic String: "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

Result of concatenation:
"dGhlIHNhbXBsZSBub25jZQ==258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

Step 3: Calculate SHA-1 hash
└─ SHA-1 hash (in hex):
   b3 7a 4f 2c c0 62 4f 16 90 f6 46 06 cf 38 59 45 b2 be c4 ea

Step 4: Base64 encode the SHA-1 hash
└─ Final result: "s3pPLMBiTxaQ9kYGzzhZRbK+xOo="
   This becomes the Sec-WebSocket-Accept value
```

### Purpose:
1. **Security Verification**:
```
┌──────────────┐         ┌──────────────┐
│    Client    │         │    Server    │
└──────────┬───┘         └──────┬───────┘
           │                    │
           │ Random Key         │
           │──────────────────►│
           │                    │
           │                    │ Transforms Key
           │                    │ using known algorithm
           │ Verify Response    │
           │◄──────────────────│
           │                    │
```

2. **Prevents Caching**:
```
Regular HTTP:          WebSocket:
┌─────────────┐       ┌─────────────┐
│ GET /path   │       │ Random Key   │
│ [can cache] │       │[unique always]│
└─────────────┘       └─────────────┘
```

3. **Confirms WebSocket Server**:
```
┌─────────────────────┐
│ Regular HTTP Server │ Cannot calculate correct response
└─────────────────────┘

┌─────────────────────┐
│ WebSocket Server    │ Knows the algorithm & magic string
└─────────────────────┘
```

### Real-World Example:
```
1. Client generates key:
Key: "x3JJHMbDL1EzLkh9GBhXDw=="

2. Server calculation:
Input = "x3JJHMbDL1EzLkh9GBhXDw==258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
SHA-1 = "1d 29 08 b0 8b 14 23 72 c2 6a 9d 6a 0c f3 d6 1a 9f 44 3c 3b"
Base64 = "HSkIuIsUI3LCap1qDPPWGp9EPDs="

3. Verification:
Client expects: "HSkIuIsUI3LCap1qDPPWGp9EPDs="
Server sends:   "HSkIuIsUI3LCap1qDPPWGp9EPDs="
└─ Connection proceeds if they match
```

### Benefits:
1. Prevents non-WebSocket servers from accidentally accepting connections
2. Ensures the server actually processed the WebSocket request
3. Prevents caching proxies from interfering
4. Provides a basic level of handshake verification
5. Makes it harder for non-WebSocket clients to fake connections

