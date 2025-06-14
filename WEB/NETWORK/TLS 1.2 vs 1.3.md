### **Step-by-Step Explanation of TLS 1.2 and TLS 1.3 with Sample Bytes and Visualizations**

---

#### **1. TLS 1.2 Handshake**
TLS 1.2 uses a **4-RTT handshake** (reduced to 2-RTT in practice). Below is the breakdown:

---

##### **Step 1: ClientHello**
- **Purpose**: Client initiates the handshake and lists supported parameters.
- **Structure**:
  - **Record Layer**: Content Type (1 byte) + Version (2 bytes) + Length (2 bytes).
  - **Handshake Header**: Type (1 byte) + Length (3 bytes).
  - **Body**: Client Version, Random, Session ID, Cipher Suites, Compression Methods, Extensions.

**Sample Bytes (Simplified)**:
```
16 03 03 00 45       // TLS Record Layer: Handshake (0x16), TLS 1.2 (0x0303), Length 0x0045
01 00 00 41          // Handshake Type: ClientHello (0x01), Length 0x000041
03 03                // Client Version: TLS 1.2 (0x0303)
[32-byte Random]     // Random data (GMT time + 28 bytes)
00                   // Session ID Length (0)
00 02 00 35          // Cipher Suites: TLS_RSA_WITH_AES_128_CBC_SHA (0x0035)
01 00                // Compression: Null (0x00)
00 17                // Extensions Length (23 bytes)
...
```

---

##### **Step 2: ServerHello, Certificate, ServerKeyExchange, ServerHelloDone**
- **ServerHello**: Server selects TLS version, cipher suite, and sends its random.
- **Certificate**: Server’s X.509 certificate.
- **ServerKeyExchange**: Parameters for key exchange (e.g., DH public key).
- **ServerHelloDone**: Signals end of server messages.

**Sample ServerHello**:
```
16 03 03 00 31       // TLS Record Layer
02 00 00 2D          // Handshake Type: ServerHello (0x02)
03 03                // Server Version: TLS 1.2
[32-byte Random]     // Server Random
00                   // Session ID Length (0)
00 35                // Selected Cipher Suite: TLS_RSA_WITH_AES_128_CBC_SHA
00                   // Compression: Null
...
```

---

##### **Step 3: ClientKeyExchange, ChangeCipherSpec, Finished**
- **ClientKeyExchange**: Pre-master secret encrypted with server’s public key.
- **ChangeCipherSpec**: Signals switch to encrypted communication.
- **Finished**: Hash of all handshake messages (validates integrity).

**ClientKeyExchange (RSA)**:
```
16 03 03 00 10       // Record Layer
10 00 00 0C          // Handshake Type: ClientKeyExchange (0x10)
[Encrypted Pre-Master Secret]
```

**ChangeCipherSpec**:
```
14 03 03 00 01       // Content Type: ChangeCipherSpec (0x14)
01                   // Payload
```

---

##### **Step 4: Server’s ChangeCipherSpec and Finished**
- Server confirms encryption and sends its `Finished` message.

---

#### **Visualization: TLS 1.2 Handshake**
```
Client                       Server
  |----ClientHello---------->|
  |<--ServerHello----------|
  |<--Certificate----------|
  |<--ServerKeyExchange----|
  |<--ServerHelloDone------|
  |----ClientKeyExchange--->|
  |----ChangeCipherSpec---->|
  |----Finished------------>|
  |<--ChangeCipherSpec-----|
  |<--Finished-------------|
  |----Application Data---->|
```

---

#### **2. TLS 1.3 Handshake**
TLS 1.3 uses a **1-RTT handshake** (or 0-RTT for resumption). Key differences:
- Ephemeral keys only (no static RSA).
- Merged messages (e.g., ServerHello includes key share).

---

##### **Step 1: ClientHello**
- Includes `supported_versions` and `key_share` extensions.

**Sample Bytes**:
```
16 03 01 00 A0       // Legacy Record Layer (TLS 1.0 for compatibility)
01 00 00 9C          // ClientHello
03 03                // Legacy Version (TLS 1.2)
[32-byte Random]
20                   // Session ID Length (32)
...                  // Legacy Session ID
00 04 13 01 13 03    // Cipher Suites: TLS 1.3 (e.g., TLS_AES_128_GCM_SHA256)
01 00                // Compression: Null
00 4E                // Extensions Length
000B 0004 0300 0102  // Supported Versions: TLS 1.3 (0x0304)
002D 0002 0100       // Supported Groups (e.g., x25519)
0033 0026            // Key Share Extension
  001D               // x25519
  0020               // Public Key Length (32 bytes)
  [Client Public Key]
```

---

##### **Step 2: ServerHello**
- Responds with `key_share`, selects TLS 1.3.

**Sample Bytes**:
```
16 03 03 00 5E       // Record Layer
02 00 00 5A          // ServerHello
0303                 // Legacy Version (TLS 1.2)
[32-byte Random]
...                  // Legacy Session ID
1301                 // Cipher Suite: TLS_AES_128_GCM_SHA256
0000                 // Legacy Compression
000B 0002 0304       // Supported Versions: TLS 1.3
0033 0026            // Key Share Extension
  001D               // x25519
  0020               // Public Key Length
  [Server Public Key]
```

---

##### **Step 3: Server Parameters (Encrypted)**
- **EncryptedExtensions**: Additional parameters (e.g., ALPN).
- **Certificate**: Server’s certificate.
- **CertificateVerify**: Proof of private key ownership.
- **Finished**: Integrity check.

---

##### **Step 4: Client Finished**
- Client computes shared secret, sends `Finished`.

---

#### **Visualization: TLS 1.3 Handshake**
```
Client                       Server
  |----ClientHello (KeyShare)-->|
  |<--ServerHello (KeyShare)---|
  |<--EncryptedExtensions-----|
  |<--Certificate-------------|
  |<--CertificateVerify-------|
  |<--Finished----------------|
  |----Finished--------------->|
  |----Application Data------->|
```

---

#### **Key Differences**
| **Feature**         | **TLS 1.2**                          | **TLS 1.3**                          |
|----------------------|--------------------------------------|--------------------------------------|
| **Handshake RTT**    | 2-RTT                                | 1-RTT                                |
| **Key Exchange**     | RSA/DHE (static keys allowed)        | Ephemeral DH only (no static RSA)    |
| **Cipher Suites**    | Supports legacy algorithms (e.g., CBC)| Removes insecure ciphers (e.g., CBC) |
| **Encrypted Data**   | After ChangeCipherSpec               | Immediately after ServerHello       |

---

#### **Example Key Exchange (x25519 in TLS 1.3)**
1. **Client Public Key**: `0xA0B1C2...` (32 bytes).
2. **Server Public Key**: `0xD3E4F5...` (32 bytes).
3. **Shared Secret**: `x25519(A0B1C2..., D3E4F5...)`.

---

#### **Visualization of Encrypted Handshake**
```
TLS 1.3 Record (Encrypted):
17 03 03 00 30       // Content Type: Application Data (0x17)
[Encrypted Handshake Messages]
```

---

#### **Summary**
- **TLS 1.2**: Explicit key exchange steps, 2-RTT, less secure.
- **TLS 1.3**: Streamlined, 1-RTT, forward-secure by design.