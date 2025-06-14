
## Understanding the WebSocket Frame Structure

A WebSocket frame is a fundamental unit of communication in the WebSocket protocol. It encapsulates the data sent between the client and the server. To comprehend this structure, it's essential to understand both **bytes** and **bits**.

### Key Definitions

- **Byte**: A unit of digital information that consists of **8 bits**. Bytes are typically numbered starting from **0**.
- **Bit**: The smallest unit of data in computing, representing a **0** or **1**.

### Frame Diagram Explained

Below is a refined and detailed representation of a WebSocket frame:

```plaintext
0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload Len |    Extended Payload Length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)            |
|N|V|V|V|       |S|             |                               |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended Payload Length continued, if payload len == 126 or 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-Key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-Key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+---------------------------------------------------------------+
```

### Breakdown of the Frame Components

1. **Byte 0**
   - **FIN (1 bit)**: Indicates if this is the final fragment in a message.
     - `1`: Final fragment.
     - `0`: More fragments to follow.
   - **RSV1, RSV2, RSV3 (1 bit each)**: Reserved bits for extensions. Typically set to `0`.
   - **Opcode (4 bits)**: Defines the interpretation of the payload data.
     - `0x1`: Text frame.
     - `0x2`: Binary frame.
     - `0x8`: Connection close.
     - `0x9`: Ping.
     - `0xA`: Pong.

2. **Byte 1**
   - **Mask (1 bit)**: Indicates if the payload data is masked.
     - `1`: Payload is masked.
     - `0`: Payload is not masked.
   - **Payload Length (7 bits)**:
     - `0-125`: Represents the actual payload length.
     - `126`: Indicates that the actual payload length is represented by the next **2 bytes** (16-bit unsigned integer).
     - `127`: Indicates that the actual payload length is represented by the next **8 bytes** (64-bit unsigned integer).

3. **Bytes 2-3 or 2-9 (Extended Payload Length)**
   - Present only if the **Payload Length** in Byte 1 is `126` or `127`.
     - **126**: Next **2 bytes** represent the payload length.
     - **127**: Next **8 bytes** represent the payload length.

4. **Masking-Key (Bytes 4-7 if Mask is `1`)**
   - **4 bytes** used to mask/unmask the payload data.
   - Mandatory for client-to-server frames.
   - Not used for server-to-client frames.

5. **Payload Data**
   - The actual data being transmitted.
   - If masked, each byte of the payload is XOR'd with a byte from the masking key.

### Example Breakdown

Let's consider an example frame to illustrate each component:

#### Example Frame (Hex Representation)

```
81 85 37 FA 21 3D 7F E9 83 4C 86 A8 5F
```

#### Byte-by-Byte Analysis

1. **Byte 0 (`0x81`)**
   - **FIN**: `1` (Final fragment).
   - **RSV1, RSV2, RSV3**: `0, 0, 0` (No extensions).
   - **Opcode**: `0x1` (Text frame).

2. **Byte 1 (`0x85`)**
   - **Mask**: `1` (Payload is masked).
   - **Payload Length**: `5` (Actual payload length).

3. **Bytes 2-5 (`37 FA 21 3D`)**
   - **Masking-Key**: `37 FA 21 3D` (Used to unmask the payload).

4. **Bytes 6-10 (`7F E9 83 4C 86 A8 5F`)**
   - **Masked Payload Data**: Encrypted form of the actual data.
   - **Unmasked Payload**: To obtain the original data, each byte of the masked payload is XOR'd with the corresponding byte of the masking key.

### Visual Representation

```plaintext
0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload Len |    Extended Payload Length    |
+-+-+-+-+-------+-+-------------+-------------------------------+
|     Extended Payload Length continued, if payload len == 126 or 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-Key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-Key (continued)       |          Payload Data         |
+---------------------------------------------------------------+
|                     Payload Data continued ...                |
+---------------------------------------------------------------+
```

### Step-by-Step Explanation

1. **FIN, RSV1-3, Opcode (Byte 0)**
   - **FIN**: Indicates if this is the last frame for the message.
   - **RSV1, RSV2, RSV3**: Reserved for future use; typically `0`.
   - **Opcode**: Specifies the type of frame (e.g., text, binary).

2. **Mask, Payload Length (Byte 1)**
   - **Mask**: Tells us if the payload is masked. Client-to-server frames must mask data.
   - **Payload Length**:
     - `0-125`: Directly represents the payload size.
     - `126`: Payload size is in the next 2 bytes.
     - `127`: Payload size is in the next 8 bytes.

3. **Extended Payload Length**
   - **Present** only if the initial **Payload Length** is `126` or `127`.
   - **126**: The next **2 bytes** are interpreted as a 16-bit integer for payload length.
   - **127**: The next **8 bytes** are interpreted as a 64-bit integer for payload length.

4. **Masking-Key (Bytes 4-7)**
   - **Present** only if the **Mask** bit is set to `1`.
   - Consists of **4 bytes** used to mask/unmask the payload data.

5. **Payload Data**
   - The actual content being transmitted.
   - If masked, each byte is XOR'd with the corresponding byte from the **Masking-Key** to retrieve the original data.

### Unmasking Process

When the **Mask** bit is `1`, the payload data is masked for security reasons. To retrieve the original data:

1. **Retrieve the Masking-Key**: Located in the 4 bytes following the **Payload Length**.
2. **XOR Operation**: Each byte of the **Masked Payload** is XOR'd with a byte from the **Masking-Key**, cycling through the key as needed.

#### Example:

- **Masking-Key**: `37 FA 21 3D`
- **Masked Payload**: `7F E9 83 4C 86 A8 5F`
  
To unmask:

```
Unmasked Byte 1: 0x7F ^ 0x37 = 0x48 ('H')
Unmasked Byte 2: 0xE9 ^ 0xFA = 0x13
Unmasked Byte 3: 0x83 ^ 0x21 = 0xA2
Unmasked Byte 4: 0x4C ^ 0x3D = 0x71 ('q')
...
```

*Note: The actual unmasked payload should align with the intended message. Ensure that the masking process is correctly implemented according to the WebSocket specification.*

### Summary

- **Bytes**: Numbered starting from `0`, each consisting of `8 bits`.
- **Bits**: Numbered within each byte from `0` to `7`.
- **Frame Structure**: Organized first by bytes, then by bits within those bytes.
- **Extended Payload and Masking-Key**: Additional sections in the frame based on initial header values.
