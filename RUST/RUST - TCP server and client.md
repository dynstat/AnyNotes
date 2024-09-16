## Echo Server and Client in Rust

Below are the implementations of a simple server and client in Rust. The server echoes back any received messages, and both the server and client adhere to the specified data format: `TAG (2 bytes)`, `Length of Payload (2 bytes)`, followed by the `Payload` in JSON format.

### Server Implementation

```rust:src/server.rs
use std::net::SocketAddr;
use tokio::net::{TcpListener, TcpStream};
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use serde_json::Value;

const TAG: &str = "TS"; // Example TAG (2 bytes)

#[tokio::main]
async fn main() -> tokio::io::Result<()> {
    // Bind the server to the specified address and port
    let addr = "127.0.0.1:8080";
    let listener = TcpListener::bind(addr).await?;
    println!("WebSocket server running on {}", addr);

    loop {
        // Accept incoming connections
        let (socket, addr) = listener.accept().await?;
        println!("New connection from {}", addr);
        tokio::spawn(handle_client(socket, addr));
    }
}

async fn handle_client(mut socket: TcpStream, addr: SocketAddr) {
    loop {
        let mut header = [0u8; 4]; // 2 bytes for TAG and 2 bytes for length

        // Read the header (TAG and length)
        if let Err(e) = socket.read_exact(&mut header).await {
            println!("Failed to read header from {}: {}", addr, e);
            return;
        }

        // Extract TAG
        let received_tag = String::from_utf8_lossy(&header[0..2]);
        if received_tag != TAG {
            println!("Invalid TAG from {}: {}", addr, received_tag);
            return;
        }

        // Extract payload length
        let length = u16::from_be_bytes([header[2], header[3]]) as usize;

        // Read the payload based on the length
        let mut payload = vec![0u8; length];
        if let Err(e) = socket.read_exact(&mut payload).await {
            println!("Failed to read payload from {}: {}", addr, e);
            return;
        }

        // Convert payload to a UTF-8 string
        let message = match String::from_utf8(payload) {
            Ok(msg) => msg,
            Err(e) => {
                println!("Invalid UTF-8 sequence from {}: {}", addr, e);
                return;
            }
        };

        println!("Received from {}: {}", addr, message);

        // Echo the message back to the client
        if let Err(e) = send_message(&mut socket, &message).await {
            println!("Failed to send message to {}: {}", addr, e);
            return;
        }
    }
}

async fn send_message(socket: &mut TcpStream, message: &str) -> tokio::io::Result<()> {
    // Create a JSON payload
    let json_payload = serde_json::json!({ "message": message });
    let payload_str = json_payload.to_string();
    let payload_bytes = payload_str.as_bytes();
    let length = payload_bytes.len() as u16;

    // Prepare the header with TAG and length in big-endian
    let mut header = Vec::new();
    header.extend_from_slice(TAG.as_bytes()); // TAG (2 bytes)
    header.extend_from_slice(&length.to_be_bytes()); // Length (2 bytes)

    // Send the header and payload
    socket.write_all(&header).await?;
    socket.write_all(payload_bytes).await?;
    Ok(())
}
```

### Client Implementation

```rust:type:src/client.rs
use tokio::net::TcpStream;
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use serde_json::json;

const TAG: &str = "TS"; // Example TAG (2 bytes)

#[tokio::main]
async fn main() -> tokio::io::Result<()> {
    // Connect to the WebSocket server
    let addr = "127.0.0.1:8080";
    let mut socket = TcpStream::connect(addr).await?;
    println!("Connected to the server at {}", addr);

    // Example message to send
    let message = "Hello, WebSocket!";
    send_message(&mut socket, message).await?;
    println!("Sent: {}", message);

    // Receive the echoed message
    let echoed_message = receive_message(&mut socket).await?;
    println!("Received: {}", echoed_message);

    Ok(())
}

async fn send_message(socket: &mut TcpStream, message: &str) -> tokio::io::Result<()> {
    // Create a JSON payload
    let json_payload = json!({ "message": message });
    let payload_str = json_payload.to_string();
    let payload_bytes = payload_str.as_bytes();
    let length = payload_bytes.len() as u16;

    // Prepare the header with TAG and length in big-endian
    let mut header = Vec::new();
    header.extend_from_slice(TAG.as_bytes()); // TAG (2 bytes)
    header.extend_from_slice(&length.to_be_bytes()); // Length (2 bytes)

    // Send the header and payload
    socket.write_all(&header).await?;
    socket.write_all(payload_bytes).await?;
    Ok(())
}

async fn receive_message(socket: &mut TcpStream) -> tokio::io::Result<String> {
    let mut header = [0u8; 4]; // 2 bytes for TAG and 2 bytes for length

    // Read the header (TAG and length)
    socket.read_exact(&mut header).await?;

    // Extract TAG
    let received_tag = String::from_utf8_lossy(&header[0..2]);
    if received_tag != TAG {
        return Err(tokio::io::Error::new(tokio::io::ErrorKind::InvalidData, "Invalid TAG"));
    }

    // Extract payload length
    let length = u16::from_be_bytes([header[2], header[3]]) as usize;

    // Read the payload based on the length
    let mut payload = vec![0u8; length];
    socket.read_exact(&mut payload).await?;

    // Convert payload to a UTF-8 string
    let message = String::from_utf8(payload).map_err(|_| tokio::io::Error::new(tokio::io::ErrorKind::InvalidData, "Invalid UTF-8"))?;
    Ok(message)
}
```

## Explanation

### Overview

The provided Rust programs implement a simple WebSocket echo server and client. The communication protocol follows a specific format for data transmission: a 2-byte `TAG`, a 2-byte payload length, and the payload itself in JSON format.

### Server (`src/server.rs`)

1. **Imports and Constants:**
   - The server uses `tokio` for asynchronous networking and `serde_json` for JSON handling.
   - A constant `TAG` is defined as `"TS"` to identify the message type.

2. **Main Function:**
   - Binds the server to `127.0.0.1:8080`.
   - Listens for incoming TCP connections.
   - For each new connection, it spawns a new asynchronous task to handle the client.

3. **Handling Clients:**
   - Reads the first 4 bytes: 2 for `TAG` and 2 for the payload length.
   - Validates the `TAG`.
   - Reads the payload based on the specified length.
   - Converts the payload from bytes to a UTF-8 string.
   - Logs the received message.
   - Echoes the message back to the client using the same format.

4. **Sending Messages:**
   - Constructs a JSON object containing the message.
   - Prepares the header with `TAG` and payload length.
   - Sends the header followed by the payload.

### Client (`src/client.rs`)

1. **Imports and Constants:**
   - Similar to the server, it uses `tokio` and `serde_json`.
   - The same `TAG` is used to maintain consistency.

2. **Main Function:**
   - Connects to the server at `127.0.0.1:8080`.
   - Sends an example message: `"Hello, WebSocket!"`.
   - Waits to receive the echoed message from the server.
   - Prints both sent and received messages.

3. **Sending Messages:**
   - Constructs a JSON payload with the message.
   - Prepares the header with `TAG` and payload length.
   - Sends the header followed by the payload.

4. **Receiving Messages:**
   - Reads the first 4 bytes for `TAG` and payload length.
   - Validates the `TAG`.
   - Reads the payload based on the length.
   - Converts the payload from bytes to a UTF-8 string.

### Detailed Comments

Both the server and client codes are heavily commented to aid understanding:

- **Import Statements:** Explain the purpose of each imported module.
- **Constants:** Define fixed values like `TAG`.
- **Functions:**
  - Each function has comments explaining its purpose, parameters, and functionality.
  - Steps within functions are annotated to describe what each block of code does.
- **Error Handling:** Comments indicate where and why errors might occur, aiding in debugging.

### Running the Programs

1. **Prerequisites:**
   - Ensure Rust is installed. If not, install it from [rustup.rs](https://rustup.rs/).
   - Add the `tokio` and `serde_json` dependencies in your `Cargo.toml`:

```toml
[dependencies]
tokio = { version = "1", features = ["full"] }
serde_json = "1.0"
```

2. **Running the Server:**
   - Navigate to the server's directory.
   - Execute `cargo run --bin server`.

3. **Running the Client:**
   - In a separate terminal, navigate to the client's directory.
   - Execute `cargo run --bin client`.
   - Observe the sent and received messages in the client's output.
   - The server's terminal will log the received and echoed messages.

### Example Interaction

- **Client Sends:** 
  ```
  {"message":"Hello, WebSocket!"}
  ```
- **Server Receives and Echoes:**
  ```
  {"message":"Hello, WebSocket!"}
  ```
- **Client Receives Echo:**
  ```
  {"message":"Hello, WebSocket!"}
  ```



The provided Rust code implements a simple echo server and client using **TCP sockets** rather than **WebSockets**. While the terminology and file descriptions mention "WebSocket," the actual implementation leverages raw TCP communication without adhering to the WebSocket protocol.

## Understanding the Difference

### TCP Sockets vs. WebSockets

- **TCP Sockets:**
  - **Protocol:** TCP (Transmission Control Protocol) is a low-level network protocol that provides reliable, ordered, and error-checked delivery of data between applications.
  - **Use Case:** Suitable for applications that require direct communication without the overhead of additional protocols.
  - **Implementation:** In Rust, handled using `TcpListener` and `TcpStream` from the `tokio::net` module, as seen in your code.

- **WebSockets:**
  - **Protocol:** WebSockets operate over TCP but provide a full-duplex communication channel over a single, long-lived connection, following the WebSocket protocol as defined by RFC 6455.
  - **Use Case:** Ideal for web applications needing real-time communication between clients and servers, such as chat applications, live feeds, or online gaming.
  - **Implementation:** Requires handling of the WebSocket handshake, framing, and possibly using dedicated libraries like `tokio-tungstenite` in Rust.

### Your Current Implementation

Your current server and client use `TcpListener` and `TcpStream` for handling connections and data transmission. Here's an overview:

- **Server (`src/server.rs`):**
  - Listens on `127.0.0.1:8080` for incoming TCP connections.
  - For each connection, it reads a custom protocol consisting of a 2-byte `TAG`, a 2-byte payload length, and a JSON-formatted payload.
  - Echoes back the received message using the same protocol.

- **Client (`src/client.rs`):**
  - Connects to the server at `127.0.0.1:8080` using a TCP stream.
  - Sends a message following the custom protocol.
  - Receives and prints the echoed message from the server.

### Example Clarification

#### Current Communication Flow:

1. **Client:**
   - Sends:
     ```
     [TAG][Length][{"message":"Hello, WebSocket!"}]
     ```
   - Example bytes:
     ```
     54 53 00 1C 7B 22 6D 65 73 73 61 67 65 22 3A 22 48 65 6C 6C 6F 2C 20 57 65 62 53 6F 63 6B 65 74 21 22 7D
     ```
     - `54 53` corresponds to the ASCII characters `T` and `S` for the `TAG`.
     - `00 1C` represents the payload length in big-endian format (`28` bytes).
     - The following bytes are the JSON payload.

2. **Server:**
   - Receives the above bytes.
   - Parses and validates the `TAG` and payload length.
   - Echoes back the same message using the same protocol.

#### Absence of WebSocket Handshake:

WebSockets require an initial handshake over HTTP to upgrade the connection, which involves specific HTTP headers and a unique opening handshake process. Your current implementation does not perform this handshake, indicating it's purely using TCP sockets.

## Transitioning to WebSockets

If your intention is to implement actual WebSocket communication, you'll need to modify your approach to adhere to the WebSocket protocol. Here's a brief guide on how to achieve this in Rust.

### Using `tokio-tungstenite` for WebSockets

[`tokio-tungstenite`](https://docs.rs/tokio-tungstenite/latest/tokio_tungstenite/) is a popular library for handling WebSocket connections in Rust asynchronously.

#### Example Server Implementation with WebSockets

```rust:src/server.rs
use tokio::net::TcpListener;
use tokio_tungstenite::accept_async;
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use futures_util::{StreamExt, SinkExt};
use serde_json::json;

const TAG: &str = "TS"; // Example TAG (2 bytes)

#[tokio::main]
async fn main() -> tokio::io::Result<()> {
    let addr = "127.0.0.1:8080";
    let listener = TcpListener::bind(addr).await?;
    println!("WebSocket server running on {}", addr);

    while let Ok((stream, addr)) = listener.accept().await {
        println!("New connection from {}", addr);
        tokio::spawn(handle_connection(stream, addr));
    }

    Ok(())
}

async fn handle_connection(stream: tokio::net::TcpStream, addr: std::net::SocketAddr) {
    let ws_stream = match accept_async(stream).await {
        Ok(ws) => ws,
        Err(e) => {
            println!("WebSocket handshake failed with {}: {}", addr, e);
            return;
        }
    };

    println!("WebSocket connection established with {}", addr);

    let (mut write, mut read) = ws_stream.split();

    while let Some(message) = read.next().await {
        match message {
            Ok(msg) => {
                if msg.is_text() || msg.is_binary() {
                    let received_text = msg.to_text().unwrap_or("");
                    println!("Received from {}: {}", addr, received_text);

                    // Echo the message back
                    if let Err(e) = write.send(msg).await {
                        println!("Failed to send message to {}: {}", addr, e);
                        break;
                    }
                }
            }
            Err(e) => {
                println!("Error receiving message from {}: {}", addr, e);
                break;
            }
        }
    }

    println!("Connection with {} closed.", addr);
}
```

#### Example Client Implementation with WebSockets

```rust:type:src/client.rs
use tokio_tungstenite::connect_async;
use url::Url;
use futures_util::{StreamExt, SinkExt};
use serde_json::json;

const TAG: &str = "TS"; // Example TAG (2 bytes)

#[tokio::main]
async fn main() -> tokio::io::Result<()> {
    let url = Url::parse("ws://127.0.0.1:8080").expect("Invalid WebSocket URL");
    let (mut ws_stream, _) = connect_async(url).await.expect("Failed to connect");

    println!("Connected to the WebSocket server.");

    // Example message to send
    let message = json!({ "message": "Hello, WebSocket!" }).to_string();
    ws_stream.send(tokio_tungstenite::tungstenite::Message::Text(message.clone())).await?;
    println!("Sent: {}", message);

    // Receive the echoed message
    if let Some(msg) = ws_stream.next().await {
        match msg {
            Ok(tokio_tungstenite::tungstenite::Message::Text(text)) => {
                println!("Received: {}", text);
            }
            Ok(_) => {}
            Err(e) => println!("Error receiving message: {}", e),
        }
    }

    Ok(())
}
```

### Key Changes for WebSockets

1. **Handshake:**
   - WebSockets require an initial handshake to upgrade the HTTP connection. Libraries like `tokio-tungstenite` handle this automatically.

2. **Message Framing:**
   - WebSockets define a frame-based protocol for sending and receiving messages, abstracting the lower-level TCP details.

3. **Protocol Usage:**
   - Use the `ws://` or `wss://` schemes to denote WebSocket connections.

4. **Libraries:**
   - Utilize dedicated WebSocket libraries (`tokio-tungstenite`) to manage connections, which handle the intricacies of the WebSocket protocol.

### Dependencies

Add the following dependencies to your `Cargo.toml` for WebSocket support:

```toml
[dependencies]
tokio = { version = "1", features = ["full"] }
tokio-tungstenite = "0.20"
tungstenite = "0.20"
futures-util = "0.3"
serde_json = "1.0"
url = "2.2"
```

## Summary

- **Current Code:** Implements an echo server and client using raw **TCP sockets** with a custom protocol for message framing.
- **WebSockets:** If your goal is to implement genuine WebSocket communication, you'll need to adopt the WebSocket protocol, which involves an initial handshake and frame-based messaging. Libraries like `tokio-tungstenite` simplify this process in Rust.
- **Recommendation:** Clarify your project's requirements. If WebSocket functionality is essential, consider refactoring your current implementation to use a dedicated WebSocket library as demonstrated above.

---

## Example Interaction with TCP Sockets (Current Implementation)

1. **Client Sends:** 
   ```
   {"message":"Hello, WebSocket!"}
   ```
   - Framed with `TAG` and payload length.

2. **Server Receives and Echoes:**
   ```
   {"message":"Hello, WebSocket!"}
   ```
   - Using the same framing protocol.

3. **Client Receives Echo:**
   ```
   {"message":"Hello, WebSocket!"}
   ```

## Example Interaction with WebSockets (After Refactoring)

1. **Client Sends:** 
   ```
   {"message":"Hello, WebSocket!"}
   ```
   - Encapsulated within a WebSocket text frame.

2. **Server Receives and Echoes:**
   ```
   {"message":"Hello, WebSocket!"}
   ```
   - Sent back as a WebSocket text frame.

3. **Client Receives Echo:**
   ```
   {"message":"Hello, WebSocket!"}
   ```

---

By distinguishing between TCP sockets and WebSockets and providing guidance on transitioning to WebSockets, you can align your implementation with your project's intended communication protocol.

---
---
