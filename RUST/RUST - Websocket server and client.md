## WebSocket Echo Server and Client in Rust

Below are the implementations of a WebSocket echo server and client in Rust. The server echoes back any received messages using the WebSocket protocol. Both the server and client adhere to best practices for WebSocket communication.

### Server Implementation

````rust:src/server.rs
use tokio::net::TcpListener;
use tokio_tungstenite::accept_async;
use futures_util::{StreamExt, SinkExt};
use std::env;
use std::net::SocketAddr;

/// The main asynchronous function that starts the WebSocket server.
#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // Bind the server to the specified address and port.
    let addr = "127.0.0.1:8080";
    let listener = TcpListener::bind(&addr).await?;
    println!("WebSocket server running on {}", addr);

    // Continuously accept incoming TCP connections.
    while let Ok((stream, addr)) = listener.accept().await {
        println!("New connection from {}", addr);

        // Spawn a new task to handle each client connection.
        tokio::spawn(handle_connection(stream, addr));
    }

    Ok(())
}

/// Handles an individual client connection.
async fn handle_connection(stream: tokio::net::TcpStream, addr: SocketAddr) {
    // Perform the WebSocket handshake.
    let ws_stream = match accept_async(stream).await {
        Ok(ws) => ws,
        Err(e) => {
            println!("WebSocket handshake failed with {}: {}", addr, e);
            return;
        }
    };

    println!("WebSocket connection established with {}", addr);

    // Split the WebSocket stream into separate sender and receiver.
    let (mut write, mut read) = ws_stream.split();

    // Continuously listen for incoming messages from the client.
    while let Some(message) = read.next().await {
        match message {
            Ok(msg) => {
                if msg.is_text() || msg.is_binary() {
                    let received_text = msg.to_text().unwrap_or("");
                    println!("Received from {}: {}", addr, received_text);

                    // Echo the received message back to the client.
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
````

### Client Implementation

````rust:src/client.rs
use tokio_tungstenite::connect_async;
use tokio_tungstenite::tungstenite::Message;
use futures_util::{StreamExt, SinkExt};
use serde_json::json;
use std::env;
use url::Url;

/// The main asynchronous function that starts the WebSocket client.
#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // Define the WebSocket server URL.
    let url = Url::parse("ws://127.0.0.1:8080").expect("Invalid WebSocket URL");

    // Connect to the WebSocket server.
    let (mut ws_stream, _) = connect_async(url).await.expect("Failed to connect");
    println!("Connected to the WebSocket server.");

    // Example message to send to the server.
    let message = json!({ "message": "Hello, WebSocket!" }).to_string();

    // Send the message as a text frame.
    ws_stream.send(Message::Text(message.clone())).await?;
    println!("Sent: {}", message);

    // Listen for the echoed message from the server.
    if let Some(msg) = ws_stream.next().await {
        match msg {
            Ok(Message::Text(text)) => {
                println!("Received: {}", text);
            }
            Ok(Message::Binary(bin)) => {
                println!("Received binary data: {:?}", bin);
            }
            Ok(_) => {}
            Err(e) => println!("Error receiving message: {}", e),
        }
    }

    Ok(())
}
````

## Explanation

### Overview

The provided Rust programs implement a WebSocket echo server and client using the `tokio` and `tokio-tungstenite` libraries. The server listens for WebSocket connections, receives messages from clients, and echoes them back. The client connects to the server, sends a message, and prints the echoed response.

### Dependencies

To run these programs, ensure you have the following dependencies in your `Cargo.toml`:

```toml
[dependencies]
tokio = { version = "1", features = ["full"] }
tokio-tungstenite = "0.20"
tungstenite = "0.20"
futures-util = "0.3"
serde_json = "1.0"
url = "2.2"
```

### Server (`src/server.rs`)

1. **Imports:**
   - `tokio::net::TcpListener`: For listening to incoming TCP connections.
   - `tokio_tungstenite::accept_async`: To upgrade TCP streams to WebSocket streams.
   - `futures_util::StreamExt` and `futures_util::SinkExt`: For handling asynchronous streams and sinks.
   - `std::net::SocketAddr`: To handle socket addresses.

2. **Main Function:**
   - Binds the server to `127.0.0.1:8080` using `TcpListener`.
   - Enters an infinite loop to accept incoming TCP connections.
   - For each connection, it spawns a new asynchronous task (`handle_connection`) to manage the client.

3. **Handling Connections:**
   - Performs the WebSocket handshake using `accept_async`.
   - Splits the WebSocket stream into a sender (`write`) and receiver (`read`) using `.split()`.
   - Listens for incoming messages from the client.
   - Upon receiving a text or binary message, it prints the message and echoes it back to the client.
   - Handles any errors during message reception or sending.

### Client (`src/client.rs`)

1. **Imports:**
   - `tokio_tungstenite::connect_async`: To establish a WebSocket connection.
   - `tokio_tungstenite::tungstenite::Message`: To handle WebSocket messages.
   - `futures_util::StreamExt` and `futures_util::SinkExt`: For handling asynchronous streams and sinks.
   - `serde_json::json`: For constructing JSON messages.
   - `url::Url`: To parse the WebSocket server URL.

2. **Main Function:**
   - Defines the WebSocket server URL (`ws://127.0.0.1:8080`).
   - Connects to the server using `connect_async`.
   - Constructs a JSON-formatted message: `{"message": "Hello, WebSocket!"}`.
   - Sends the message as a text frame to the server.
   - Waits to receive the echoed message from the server.
   - Prints both the sent and received messages.

### Running the Programs

1. **Prerequisites:**
   - Ensure Rust is installed. If not, install it from [rustup.rs](https://rustup.rs/).
   - Create a new Rust project or navigate to your existing project's directory.
   - Add the required dependencies to your `Cargo.toml` as shown above.

2. **Running the Server:**
   - Navigate to the server's directory.
   - Execute the server using:
     ```bash
     cargo run --bin server
     ```
   - The server will start and listen on `127.0.0.1:8080`.

3. **Running the Client:**
   - Open a new terminal window.
   - Navigate to the client's directory.
   - Execute the client using:
     ```bash
     cargo run --bin client
     ```
   - The client will connect to the server, send a message, and print the echoed response.

### Example Interaction

1. **Server Output:**
   ```
   WebSocket server running on 127.0.0.1:8080
   New connection from 127.0.0.1:54321
   WebSocket connection established with 127.0.0.1:54321
   Received from 127.0.0.1:54321: {"message":"Hello, WebSocket!"}
   Connection with 127.0.0.1:54321 closed.
   ```

2. **Client Output:**
   ```
   Connected to the WebSocket server.
   Sent: {"message":"Hello, WebSocket!"}
   Received: {"message":"Hello, WebSocket!"}
   ```

### Detailed Comments

Both the server and client codes are thoroughly commented to enhance understanding:

- **Server (`src/server.rs`):**
  - **Binding and Listening:** The server binds to a local address and listens for incoming connections.
  - **WebSocket Handshake:** Upgrades raw TCP connections to WebSocket streams.
  - **Message Handling:** Receives messages from clients, logs them, and echoes them back.
  - **Error Handling:** Logs any errors encountered during message reception or sending.

- **Client (`src/client.rs`):**
  - **Connecting to Server:** Establishes a WebSocket connection to the specified server URL.
  - **Sending Messages:** Constructs and sends a JSON-formatted message as a text frame.
  - **Receiving Messages:** Listens for the echoed message from the server and prints it.
  - **Error Handling:** Handles any errors encountered during message sending or reception.

### Transition from TCP to WebSockets

Unlike the initial TCP-based implementation, the WebSocket protocol provides a standardized way to handle real-time, bidirectional communication between clients and servers. By using libraries like `tokio-tungstenite`, the complexities of the WebSocket handshake and message framing are abstracted, allowing developers to focus on the core application logic.

### Summary

- **WebSocket Protocol:** Enables full-duplex communication channels over a single TCP connection, facilitating real-time data exchange.
- **Libraries Used:**
  - `tokio`: For asynchronous runtime.
  - `tokio-tungstenite`: For WebSocket support.
  - `futures-util`: For handling asynchronous streams.
  - `serde_json`: For JSON serialization/deserialization.
- **Functionality:**
  - **Server:** Listens for WebSocket connections, receives messages, and echoes them back.
  - **Client:** Connects to the server, sends a message, and listens for the echoed response.

By adhering to the WebSocket protocol and utilizing appropriate Rust libraries, the provided implementations offer a robust foundation for building real-time, interactive applications.