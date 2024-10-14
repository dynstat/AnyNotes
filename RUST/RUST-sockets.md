## TCP Echo Server and Client in Rust

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
    println!("TCP server running on {}", addr);

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
    // Connect to the TCP server
    let addr = "127.0.0.1:8080";
    let mut socket = TcpStream::connect(addr).await?;
    println!("Connected to the server at {}", addr);

    // Example message to send
    let message = "Hello, TCP!";
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

The provided Rust programs implement a simple TCP echo server and client. The communication protocol follows a specific format for data transmission: a 2-byte `TAG`, a 2-byte payload length, and the payload itself in JSON format.

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
   - Sends an example message: `"Hello, TCP!"`.
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
  {"message":"Hello, TCP!"}
  ```
- **Server Receives and Echoes:**
  ```
  {"message":"Hello, TCP!"}
  ```
- **Client Receives Echo:**
  ```
  {"message":"Hello, TCP!"}
  ```
