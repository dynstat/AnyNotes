

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