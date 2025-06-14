### **TCP 3-Way Handshake & Data Transmission in Detail**

TCP (Transmission Control Protocol) establishes a reliable connection using a **3-way handshake** before actual data transmission begins. This process ensures both sender and receiver are ready to exchange data.

---

## **Step 1: TCP 3-Way Handshake**
The handshake consists of three messages exchanged between the **client (initiator)** and the **server (responder)**:

### **1. SYN (Synchronize) - Client → Server**
   - The client initiates the connection by sending a **SYN** packet.
   - This packet includes:
     - **Sequence Number (Seq#)** → A randomly chosen initial sequence number.
     - **SYN flag** set to `1` (indicating a connection request).

#### **Example SYN Packet:**
| Field                | Value (Example) | Meaning                              |
| -------------------- | --------------- | ------------------------------------ |
| **Source Port**      | `54321`         | Random client port                   |
| **Destination Port** | `80`            | Server’s listening port (e.g., HTTP) |
| **Sequence Number**  | `1000`          | Initial random sequence number       |
| **ACK Number**       | `0`             | Not set (irrelevant for SYN)         |
| **Flags**            | `SYN`           | Connection request                   |
| **Window Size**      | `64240`         | Available buffer size                |
| **Checksum**         | `0x1234`        | Error checking                       |
|                      |                 |                                      |

---
### **2. SYN-ACK (Synchronize-Acknowledge) - Server → Client**
   - The server responds with:
     - **SYN flag** (to acknowledge connection request).
     - **ACK flag** (to acknowledge receipt of the client’s SYN).
     - A **new random sequence number**.
     - **ACK Number** = Client’s Seq# + 1 (Confirming receipt).

#### **Example SYN-ACK Packet:**
| Field | Value (Example) | Meaning |
|--------|--------------|----------|
| **Source Port** | `80` | Server port |
| **Destination Port** | `54321` | Client's chosen port |
| **Sequence Number** | `5000` | Server’s random sequence number |
| **ACK Number** | `1001` | Acknowledging client’s SYN (Seq# + 1) |
| **Flags** | `SYN, ACK` | Connection acknowledgment |
| **Window Size** | `64240` | Available buffer size |
| **Checksum** | `0x5678` | Error checking |

---
### **3. ACK (Acknowledge) - Client → Server**
   - The client sends an **ACK** back to the server.
   - This confirms that the client received the server's **SYN-ACK**.
   - Client’s **ACK Number** = Server’s Seq# + 1.

#### **Example ACK Packet:**
| Field | Value (Example) | Meaning |
|--------|--------------|----------|
| **Source Port** | `54321` | Client’s port |
| **Destination Port** | `80` | Server’s port |
| **Sequence Number** | `1001` | Same as before (no data yet) |
| **ACK Number** | `5001` | Acknowledging server’s SYN (Seq# + 1) |
| **Flags** | `ACK` | Finalizing handshake |
| **Window Size** | `64240` | Available buffer size |
| **Checksum** | `0x9abc` | Error checking |

---

## **📌 Visualization of TCP Handshake**

```
Client                           Server
--------------------------------------------------
[Step 1] SYN (Seq=1000)   → 
                         ←  [Step 2] SYN-ACK (Seq=5000, Ack=1001)
[Step 3] ACK (Ack=5001)   → 
--------------------------------------------------
```

---
## **📌 Actual Data Transmission**
Once the handshake is complete, data can flow between the client and server.

### **Example Data Packet (Client to Server)**
| Field | Value (Example) | Meaning |
|--------|--------------|----------|
| **Source Port** | `54321` | Client’s port |
| **Destination Port** | `80` | Server’s port |
| **Sequence Number** | `1001` | Next expected |
| **ACK Number** | `5001` | Acknowledging server’s SYN |
| **Flags** | `ACK, PSH` | Push data |
| **Data** | `"GET /index.html HTTP/1.1"` | HTTP request |

```
Client                           Server
--------------------------------------------------
GET /index.html     → 
                    ←  [ACK] (Ack=1031, Seq=5001)
--------------------------------------------------
```

### **Example Data Packet (Server to Client - HTTP Response)**
| Field | Value (Example) | Meaning |
|--------|--------------|----------|
| **Source Port** | `80` | Server’s port |
| **Destination Port** | `54321` | Client’s port |
| **Sequence Number** | `5001` | Server’s data sequence |
| **ACK Number** | `1031` | Acknowledging client’s request |
| **Flags** | `ACK, PSH` | Data transmission |
| **Data** | `"HTTP/1.1 200 OK ... <html>...</html>"` | Response |

```
Client                           Server
--------------------------------------------------
                     ←  HTTP Response (HTML Content)
--------------------------------------------------
```

---

## **📌 Full TCP Sequence Diagram**
```plaintext
Client                        Server
--------------------------------------------------
[Handshake]
SYN (Seq=1000)       →  
                      ←  SYN-ACK (Seq=5000, Ack=1001)
ACK (Ack=5001)       →  

[Data Transmission]
GET /index.html      →  
                      ←  ACK (Ack=1031)
                      ←  HTTP Response (200 OK)

[Data Exchange Continues...]
--------------------------------------------------
```

---

## **Conclusion**
- The **3-way handshake** ensures a reliable connection before data exchange.
- TCP assigns **sequence numbers** to track packets.
- **Acknowledgment numbers** confirm data reception.
- **Flags (SYN, ACK, PSH, etc.)** define the packet’s purpose.
- Data transfer follows the handshake, enabling communication between client and server.

---
---
## **📌 TCP Packet Structure with Field Sizes**

Each TCP packet consists of several fields, each with a fixed size (in bytes). Below, we break down each **packet type** (SYN, SYN-ACK, ACK, Data) and their respective sizes.

---

## **🟢 TCP Header Format (Common for all packets)**
A TCP header is **typically 20 bytes** (without options) and contains:

| Field | Size (Bytes) | Description |
|--------|------------|-------------|
| **Source Port** | 2 | Identifies sender’s port |
| **Destination Port** | 2 | Identifies recipient’s port |
| **Sequence Number** | 4 | Initial sequence number |
| **Acknowledgment Number** | 4 | Confirms received data |
| **Header Length** | 0.5 | Size of TCP header |
| **Flags (SYN, ACK, etc.)** | 0.5 | Control flags |
| **Window Size** | 2 | Buffer size for flow control |
| **Checksum** | 2 | Error checking |
| **Urgent Pointer** | 2 | Used with URG flag (rarely) |
| **Options** | 0-40 | Extra features (e.g., MSS) |
| **Padding** | Variable | Aligns header to 4-byte multiples |

**🔹 Minimum TCP header size: 20 bytes** (without options).

---

## **🟢 TCP SYN Packet Breakdown**
This is the first packet sent by the client.

| Field | Size (Bytes) | Example Value | Description |
|--------|------------|---------------|-------------|
| **Source Port** | 2 | `54321` | Client’s random port |
| **Destination Port** | 2 | `80` | Server’s port |
| **Sequence Number** | 4 | `1000` | Initial sequence number |
| **Acknowledgment Number** | 4 | `0` | Not set (SYN only) |
| **Header Length & Flags** | 1 | `SYN=1` | SYN flag set |
| **Window Size** | 2 | `64240` | Flow control buffer size |
| **Checksum** | 2 | `0x1234` | Error detection |
| **Urgent Pointer** | 2 | `0` | Not used |
| **Options (MSS, etc.)** | 12 | `1460` | Maximum segment size (MSS) |
| **Padding** | Variable | - | Alignment |
| **Total Size** | **32 bytes** | - | SYN packets often include options |

---

## **🟢 TCP SYN-ACK Packet Breakdown**
This is the response from the server.

| Field | Size (Bytes) | Example Value | Description |
|--------|------------|---------------|-------------|
| **Source Port** | 2 | `80` | Server’s port |
| **Destination Port** | 2 | `54321` | Client’s port |
| **Sequence Number** | 4 | `5000` | Server’s random sequence number |
| **Acknowledgment Number** | 4 | `1001` | Acknowledging client’s SYN |
| **Header Length & Flags** | 1 | `SYN=1, ACK=1` | SYN-ACK response |
| **Window Size** | 2 | `64240` | Flow control buffer size |
| **Checksum** | 2 | `0x5678` | Error detection |
| **Urgent Pointer** | 2 | `0` | Not used |
| **Options (MSS, etc.)** | 12 | `1460` | Maximum segment size |
| **Padding** | Variable | - | Alignment |
| **Total Size** | **32 bytes** | - | Similar to SYN |

---

## **🟢 TCP ACK Packet Breakdown**
This final handshake packet confirms the connection.

| Field | Size (Bytes) | Example Value | Description |
|--------|------------|---------------|-------------|
| **Source Port** | 2 | `54321` | Client’s port |
| **Destination Port** | 2 | `80` | Server’s port |
| **Sequence Number** | 4 | `1001` | No new data sent yet |
| **Acknowledgment Number** | 4 | `5001` | Acknowledging server’s SYN |
| **Header Length & Flags** | 1 | `ACK=1` | Acknowledgment flag set |
| **Window Size** | 2 | `64240` | Flow control buffer size |
| **Checksum** | 2 | `0x9abc` | Error detection |
| **Urgent Pointer** | 2 | `0` | Not used |
| **Options** | 0 | - | No extra options needed |
| **Padding** | 0 | - | No padding needed |
| **Total Size** | **20 bytes** | - | Minimum TCP header |

---

## **🟢 TCP Data Packet Breakdown**
This is the actual data transfer after the handshake.

| Field | Size (Bytes) | Example Value | Description |
|--------|------------|---------------|-------------|
| **Source Port** | 2 | `54321` | Client’s port |
| **Destination Port** | 2 | `80` | Server’s port |
| **Sequence Number** | 4 | `1001` | Tracking data order |
| **Acknowledgment Number** | 4 | `5001` | Acknowledging last received byte |
| **Header Length & Flags** | 1 | `ACK=1, PSH=1` | Pushing data immediately |
| **Window Size** | 2 | `64240` | Flow control buffer size |
| **Checksum** | 2 | `0xdef0` | Error detection |
| **Urgent Pointer** | 2 | `0` | Not used |
| **Options** | 0 | - | No extra options needed |
| **Padding** | 0 | - | No padding needed |
| **Data** | Variable | `"GET /index.html"` | Actual request (HTTP) |
| **Total Size** | **20 bytes + Data** | - | Depends on the data length |

📌 **Example for an HTTP request (`GET /index.html` ~ 30 bytes of data)**  
- **Total TCP Packet Size =** 20 (header) + 30 (data) = **50 bytes**

---

## **📌 Complete TCP Packet Size Comparison**
| Packet Type | Header Size (Bytes) | Data Size (Bytes) | Total Size (Bytes) |
|-------------|--------------------|------------------|------------------|
| **SYN** | 32 | 0 | **32** |
| **SYN-ACK** | 32 | 0 | **32** |
| **ACK** | 20 | 0 | **20** |
| **Data Packet** | 20 | Variable (e.g., 30) | **50+** |

---

## **📌 Final Visualization**
```plaintext
Client                              Server
---------------------------------------------------
[SYN]      Seq=1000, Win=64240  →   (32 bytes)
                                ←   [SYN-ACK] Seq=5000, Ack=1001  (32 bytes)
[ACK]      Ack=5001            →   (20 bytes)

[Data]     "GET /index.html"   →   (50 bytes)
                                ←   [ACK]  (20 bytes)
                                ←   "HTTP 200 OK..." (Data Packet, e.g., 500 bytes)
---------------------------------------------------
```

---

## **📌 Summary**
- **TCP header is typically 20 bytes** (can be up to 60 bytes with options).
- **SYN & SYN-ACK include options (~32 bytes)**.
- **ACK is the smallest (20 bytes)**.
- **Data packets add extra payload size**.

---
---
## Example for the mws-lib related test-nb.c client (4april25)

# TCP and WebSocket Connection Analysis

This packet capture shows two connection attempts to port 8765 from local clients on ports 63310 and 63311. Let me analyze each frame in detail to explain what's happening.

## Connection 1: Port 63310 to Port 8765

### Frame 645: SYN Packet (Connection Request)
```
Source: ::1 (localhost IPv6)
Destination: ::1 (localhost IPv6)
Source Port: 63310
Destination Port: 8765
Flags: [SYN]
Sequence Number: 0 (Initial sequence number)
Window Size: 65535 (Receiver buffer size in bytes)
Options:
  - MSS: 65475 (Maximum Segment Size)
  - WS: 256 (Window Scale factor)
  - SACK_PERM (Selective ACK permitted)
```
This is the first packet of the TCP three-way handshake. The client (63310) is initiating a connection to the server (8765) by sending a SYN (synchronize) packet with an initial sequence number of 0.

### Frame 646: SYN-ACK Packet (Connection Acknowledgment)
```
Source: ::1
Destination: ::1
Source Port: 8765
Destination Port: 63310
Flags: [SYN, ACK]
Sequence Number: 0 (Server's initial sequence number)
Acknowledgment Number: 1 (Acknowledging client's SYN)
Window Size: 65535
Options: Same as client (MSS, WS, SACK_PERM)
```
The server responds with a SYN-ACK packet, acknowledging the client's SYN and establishing its own initial sequence number. The ACK number is 1 because it's acknowledging the client's SYN packet (which increments the sequence number by 1 even though it carries no data).

### Frame 647: ACK Packet (Handshake Completion)
```
Source: ::1
Destination: ::1
Source Port: 63310
Destination Port: 8765
Flags: [ACK]
Sequence Number: 1
Acknowledgment Number: 1
Window Size: 327168 (Scaled window size)
```
The client acknowledges the server's SYN-ACK, completing the three-way handshake. The connection is now established. Note that the window size is now 327168, which is the result of applying the window scale factor.

### Frame 648: FIN-ACK Packet (Connection Termination Request)
```
Source: ::1
Destination: ::1
Source Port: 63310
Destination Port: 8765
Flags: [FIN, ACK]
Sequence Number: 1
Acknowledgment Number: 1
Window Size: 327168
```
Immediately after establishing the connection, the client sends a FIN-ACK packet, indicating it wants to close the connection. This is unusual behavior for a normal connection but might indicate a connection probe or a quick check of service availability.

### Frame 649: ACK Packet (Acknowledging FIN)
```
Source: ::1
Destination: ::1
Source Port: 8765
Destination Port: 63310
Flags: [ACK]
Sequence Number: 1
Acknowledgment Number: 2
Window Size: 2160640
```
The server acknowledges the client's FIN packet. The acknowledgment number is 2 because the FIN flag increments the sequence number by 1.

### Frame 655: FIN-ACK Packet (Server Termination)
```
Source: ::1
Destination: ::1
Source Port: 8765
Destination Port: 63310
Flags: [FIN, ACK]
Sequence Number: 1
Acknowledgment Number: 2
Window Size: 2160640
```
The server now sends its own FIN-ACK to complete the connection termination process.

### Frame 656: ACK Packet (Final Acknowledgment)
```
Source: ::1
Destination: ::1
Source Port: 63310
Destination Port: 8765
Flags: [ACK]
Sequence Number: 2
Acknowledgment Number: 2
Window Size: 327168
```
The client acknowledges the server's FIN, completing the four-way termination handshake. The connection is now fully closed.

## Connection 2: Port 63311 to Port 8765 (WebSocket Upgrade)

### Frame 650: SYN Packet
```
Source: ::1
Destination: ::1
Source Port: 63311
Destination Port: 8765
Flags: [SYN]
Sequence Number: 0
Window Size: 65535
Options: MSS, WS, SACK_PERM
```
Similar to the first connection, this is the start of a TCP three-way handshake from a new client port.

### Frame 651: SYN-ACK Packet
```
Source: ::1
Destination: ::1
Source Port: 8765
Destination Port: 63311
Flags: [SYN, ACK]
Sequence Number: 0
Acknowledgment Number: 1
Window Size: 65535
Options: MSS, WS, SACK_PERM
```
Server acknowledges the connection request.

### Frame 652: ACK Packet
```
Source: ::1
Destination: ::1
Source Port: 63311
Destination Port: 8765
Flags: [ACK]
Sequence Number: 1
Acknowledgment Number: 1
Window Size: 327168
```
Client completes the handshake. TCP connection is now established.

### Frame 653: HTTP GET Request (WebSocket Upgrade)
```
Source: ::1
Destination: ::1
Source Port: 63311
Destination Port: 8765
Protocol: HTTP
Method: GET
Path: /
HTTP Version: 1.1
Sequence Number: 1
Acknowledgment Number: 1
Length: 148 bytes
```
Unlike the first connection, this client sends an HTTP GET request after establishing the TCP connection. This is likely a WebSocket upgrade request (HTTP is used to establish WebSocket connections via the Upgrade header).

### Frame 654: ACK Packet
```
Source: ::1
Destination: ::1
Source Port: 8765
Destination Port: 63311
Flags: [ACK]
Sequence Number: 1
Acknowledgment Number: 149
Window Size: 2160384
```
Server acknowledges receipt of the HTTP request. The acknowledgment number is 149 (1 + 148) because the HTTP request was 148 bytes.

### Frame 657: HTTP Response (101 Switching Protocols)
```
Source: ::1
Destination: ::1
Source Port: 8765
Destination Port: 63311
Protocol: HTTP
Status: 101 Switching Protocols
Sequence Number: 1
Acknowledgment Number: 149
Length: 203 bytes
```
The server responds with HTTP status 101 (Switching Protocols), which confirms that the WebSocket upgrade request was successful. The server is agreeing to switch from HTTP to the WebSocket protocol for this connection.

### Frame 658: ACK Packet
```
Source: ::1
Destination: ::1
Source Port: 63311
Destination Port: 8765
Flags: [ACK]
Sequence Number: 149
Acknowledgment Number: 204
Window Size: 327168
```
Client acknowledges the HTTP response. The acknowledgment number is 204 (1 + 203) because the HTTP response was 203 bytes.

## Summary

1. **Connection 1 (Port 63310)**: This appears to be a quick connection probe. The client establishes a TCP connection and immediately closes it without sending any application data. This might be a service check or part of a scanning process.

2. **Connection 2 (Port 63311)**: This is a successful WebSocket connection establishment. The client:
   - Establishes a TCP connection
   - Sends an HTTP GET request with WebSocket upgrade headers
   - Receives a "101 Switching Protocols" response
   - The connection is now upgraded from HTTP to WebSocket protocol

The server on port 8765 appears to be a WebSocket server that accepts connections and handles the protocol upgrade process correctly. After frame 658, the WebSocket connection is established and ready for bidirectional communication.


----
---
