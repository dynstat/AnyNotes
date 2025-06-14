# WebSockets Implementation in Python, JavaScript (Node.js/Browser), and C

This guide provides simple WebSocket server and client programs in **Python**, **JavaScript** (both **Node.js** and **Browser** environments), and **C**. Each implementation exchanges three independent string messages between the client and server with their respective responses. The code includes detailed comments to facilitate understanding.

---

## Table of Contents

1. [Python Implementation](#python-implementation)
   - [WebSocket Server](#websocket-server)
   - [WebSocket Client](#websocket-client)
2. [JavaScript Implementation](#javascript-implementation)
   - [Node.js WebSocket Server](#nodejs-websocket-server)
   - [Browser WebSocket Client](#browser-websocket-client)
3. [C Implementation](#c-implementation)
   - [WebSocket Server](#c-websocket-server)
   - [WebSocket Client](#c-websocket-client)

---

## Python Implementation

### WebSocket Server

```python
import asyncio
import websockets

async def handle_client(websocket, path):
    """
    Handles incoming messages from the client and sends responses.

    Parameters:
    - websocket: The WebSocket connection instance.
    - path: The URL path of the WebSocket connection.
    """
    print(f"Client connected from {websocket.remote_address}")
    try:
        # Receive and respond to three messages
        for i in range(3):
            message = await websocket.recv()
            print(f"Received from client: {message}")

            response = f"Server response to message {i+1}"
            await websocket.send(response)
            print(f"Sent to client: {response}")

    except websockets.exceptions.ConnectionClosed as e:
        print(f"Connection closed: {e}")

async def main():
    """
    Starts the WebSocket server on localhost at port 8765.
    """
    async with websockets.serve(handle_client, "localhost", 8765):
        print("WebSocket server started on ws://localhost:8765")
        await asyncio.Future()  # Run forever

if __name__ == "__main__":
    asyncio.run(main())
```

### WebSocket Client

```python:python_client.py
import asyncio
import websockets

async def communicate():
    """
    Connects to the WebSocket server, sends three messages, and prints responses.
    """
    uri = "ws://localhost:8765"
    async with websockets.connect(uri) as websocket:
        # Send and receive three messages
        for i in range(3):
            message = f"Client message {i+1}"
            await websocket.send(message)
            print(f"Sent to server: {message}")

            response = await websocket.recv()
            print(f"Received from server: {response}")

if __name__ == "__main__":
    asyncio.run(communicate())
```

### Explanation

#### Python WebSocket Server (`python_server.py`)

- **Imports**:
  - `asyncio`: For asynchronous I/O operations.
  - `websockets`: A library for building WebSocket servers and clients.

- **`handle_client` Function**:
  - Accepts incoming WebSocket connections.
  - Receives three messages from the client.
  - Sends a corresponding response for each received message.
  - Handles connection closure gracefully.

- **`main` Function**:
  - Initializes and starts the WebSocket server on `localhost` at port `8765`.
  - Keeps the server running indefinitely using `asyncio.Future()`.

- **Execution**:
  - The server starts when the script is run directly.
  
#### Python WebSocket Client (`python_client.py`)

- **Imports**:
  - `asyncio`: For asynchronous operations.
  - `websockets`: To establish WebSocket connections.

- **`communicate` Function**:
  - Connects to the WebSocket server at `ws://localhost:8765`.
  - Sends three distinct messages to the server.
  - Receives and prints the server's responses.

- **Execution**:
  - The client initiates communication when the script is run directly.

#### Example Usage

1. **Start the Server**:
   ```bash
   python python_server.py
   ```
   Output:
   ```
   WebSocket server started on ws://localhost:8765
   ```

2. **Run the Client**:
   ```bash
   python python_client.py
   ```
   Output:
   ```
   Sent to server: Client message 1
   Received from server: Server response to message 1
   Sent to server: Client message 2
   Received from server: Server response to message 2
   Sent to server: Client message 3
   Received from server: Server response to message 3
   ```

3. **Server Output**:
   ```
   Client connected from ('127.0.0.1', 54321)
   Received from client: Client message 1
   Sent to client: Server response to message 1
   Received from client: Client message 2
   Sent to client: Server response to message 2
   Received from client: Client message 3
   Sent to client: Server response to message 3
   ```

---

## JavaScript Implementation

### Node.js WebSocket Server

```javascript
const WebSocket = require('ws');

// Create a new WebSocket server listening on port 8080
const wss = new WebSocket.Server({ port: 8080 }, () => {
    console.log('WebSocket server started on ws://localhost:8080');
});

// Listen for connection events
wss.on('connection', function connection(ws) {
    console.log('New client connected');

    // Listen for messages from the client
    ws.on('message', function incoming(message) {
        console.log(`Received from client: ${message}`);

        // Determine response based on received message
        const response = `Server response to "${message}"`;
        ws.send(response);
        console.log(`Sent to client: ${response}`);
    });

    // Handle connection closure
    ws.on('close', () => {
        console.log('Client disconnected');
    });
});
```

### Browser WebSocket Client

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>WebSocket Browser Client</title>
</head>
<body>
    <h1>WebSocket Browser Client</h1>
    <button id="start">Start Communication</button>
    <ul id="messages"></ul>

    <script>
        // Get references to DOM elements
        const startButton = document.getElementById('start');
        const messagesList = document.getElementById('messages');

        // Initialize WebSocket connection
        const socket = new WebSocket('ws://localhost:8080');

        // Connection opened
        socket.addEventListener('open', function (event) {
            console.log('Connected to WebSocket server');
        });

        // Listen for messages from the server
        socket.addEventListener('message', function (event) {
            console.log('Received from server:', event.data);
            const li = document.createElement('li');
            li.textContent = `Server: ${event.data}`;
            messagesList.appendChild(li);
        });

        // Handle button click to start communication
        startButton.addEventListener('click', function () {
            for (let i = 1; i <= 3; i++) {
                const message = `Client message ${i}`;
                socket.send(message);
                console.log(`Sent to server: ${message}`);

                const li = document.createElement('li');
                li.textContent = `Client: ${message}`;
                messagesList.appendChild(li);
            }
        });

        // Handle connection closure
        socket.addEventListener('close', function () {
            console.log('Disconnected from WebSocket server');
        });
    </script>
</body>
</html>
```

### Explanation

#### Node.js WebSocket Server (`node_server.js`)

- **Imports**:
  - `ws`: A WebSocket library for Node.js.

- **Server Initialization**:
  - Creates a WebSocket server listening on port `8080`.
  - Logs a confirmation message upon starting.

- **Connection Handling**:
  - Listens for new client connections.
  - For each connection:
    - Logs when a new client connects.
    - Listens for incoming messages from the client.
    - Sends a corresponding response for each received message.
    - Logs when a client disconnects.

- **Execution**:
  - Ensure that the `ws` library is installed:
    ```bash
    npm install ws
    ```
  - Run the server:
    ```bash
    node node_server.js
    ```

#### Browser WebSocket Client (`browser_client.html`)

- **HTML Structure**:
  - A button to initiate communication.
  - An unordered list to display messages.

- **JavaScript Logic**:
  - Establishes a WebSocket connection to `ws://localhost:8080`.
  - Logs connection status in the console.
  - Listens for messages from the server and appends them to the DOM.
  - When the "Start Communication" button is clicked:
    - Sends three distinct messages to the server.
    - Displays sent messages in the DOM.
  - Handles connection closure gracefully.

- **Usage**:
  1. **Start the Node.js Server**:
     ```bash
     node node_server.js
     ```
  2. **Open the Client in Browser**:
     - Open `browser_client.html` in a web browser.
     - Click the "Start Communication" button to begin sending messages.
  3. **Observe Interactions**:
     - Messages sent and received will appear in the browser's console and on the webpage.

#### Example Interaction

1. **Server Output**:
   ```
   WebSocket server started on ws://localhost:8080
   New client connected
   Received from client: Client message 1
   Sent to client: Server response to "Client message 1"
   Received from client: Client message 2
   Sent to client: Server response to "Client message 2"
   Received from client: Client message 3
   Sent to client: Server response to "Client message 3"
   Client disconnected
   ```

2. **Browser Console**:
   ```
   Connected to WebSocket server
   Sent to server: Client message 1
   Sent to server: Client message 2
   Sent to server: Client message 3
   Received from server: Server response to "Client message 1"
   Received from server: Server response to "Client message 2"
   Received from server: Server response to "Client message 3"
   Disconnected from WebSocket server
   ```

3. **Browser Display**:
   - **Client**: Displays the sent messages.
   - **Server**: Processes and responds to each message.

---

## C Implementation

Implementing WebSockets in C requires handling the WebSocket handshake, framing, masking, and communication over TCP sockets. For simplicity, we'll use the **libwebsockets** library, which abstracts much of the complexity involved in WebSocket communication.

### Prerequisites

- **libwebsockets** library installed.
- **C Compiler** (e.g., `gcc`).

#### Installing libwebsockets

```bash
# On Ubuntu/Debian
sudo apt-get update
sudo apt-get install libwebsockets-dev

# Alternatively, build from source
git clone https://libwebsockets.org/repo/libwebsockets
cd libwebsockets
mkdir build
cd build
cmake ..
make
sudo make install
```

### WebSocket Server

```c
#include <libwebsockets.h>
#include <string.h>
#include <stdio.h>

// Protocols supported by the server
static int callback_websocket(struct lws *wsi, enum lws_callback_reasons reason,
                              void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_RECEIVE:
            {
                // Received data from client
                char *received = (char *)in;
                printf("Received from client: %.*s\n", (int)len, received);

                // Prepare response
                char response[256];
                snprintf(response, sizeof(response), "Server response to \"%.*s\"", (int)len, received);

                // Send response back to client
                lws_write(wsi, (unsigned char *)response, strlen(response), LWS_WRITE_TEXT);
                printf("Sent to client: %s\n", response);
            }
            break;

        case LWS_CALLBACK_ESTABLISHED:
            printf("Client connected\n");
            break;

        case LWS_CALLBACK_CLOSED:
            printf("Client disconnected\n");
            break;

        default:
            break;
    }
    return 0;
}

// Define the protocols
static struct lws_protocols protocols[] = {
    {
        "websocket-protocol",
        callback_websocket,
        0,
        1024,
    },
    { NULL, NULL, 0, 0 } // Terminator
};

int main(void) {
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));

    // Server will listen on port 9000
    info.port = 9000;
    info.protocols = protocols;

    // Create the WebSocket context
    struct lws_context *context = lws_create_context(&info);
    if (context == NULL) {
        fprintf(stderr, "Failed to create WebSocket context\n");
        return -1;
    }

    printf("WebSocket server started on ws://localhost:9000\n");

    // Event loop
    while (1) {
        lws_service(context, 1000);
    }

    // Clean up
    lws_context_destroy(context);
    return 0;
}
```

### WebSocket Client

```c:c_client.c
#include <libwebsockets.h>
#include <string.h>
#include <stdio.h>

static int callback_websocket(struct lws *wsi, enum lws_callback_reasons reason,
                              void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            printf("Connected to server\n");
            // Send three messages to the server
            for (int i = 1; i <= 3; i++) {
                char message[256];
                snprintf(message, sizeof(message), "Client message %d", i);
                lws_write(wsi, (unsigned char *)message, strlen(message), LWS_WRITE_TEXT);
                printf("Sent to server: %s\n", message);
            }
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            {
                // Received data from server
                char *received = (char *)in;
                printf("Received from server: %.*s\n", (int)len, received);
            }
            break;

        case LWS_CALLBACK_CLOSED:
            printf("Disconnected from server\n");
            break;

        default:
            break;
    }
    return 0;
}

// Define the protocols
static struct lws_protocols protocols[] = {
    {
        "websocket-protocol",
        callback_websocket,
        0,
        1024,
    },
    { NULL, NULL, 0, 0 } // Terminator
};

int main(void) {
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));

    // Specify client mode
    info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    info.port = CONTEXT_PORT_NO_LISTEN; // No server

    info.protocols = protocols;

    // Create the WebSocket context
    struct lws_context *context = lws_create_context(&info);
    if (context == NULL) {
        fprintf(stderr, "Failed to create WebSocket context\n");
        return -1;
    }

    // Create a client connection
    struct lws_client_connect_info ccinfo = {0};
    ccinfo.context = context;
    ccinfo.address = "localhost";
    ccinfo.port = 9000;
    ccinfo.path = "/";
    ccinfo.host = lws_canonical_hostname(context);
    ccinfo.origin = "origin";
    ccinfo.protocol = protocols[0].name;

    struct lws *wsi = lws_client_connect_via_info(&ccinfo);
    if (wsi == NULL) {
        fprintf(stderr, "Failed to connect to server\n");
        lws_context_destroy(context);
        return -1;
    }

    // Event loop
    while (1) {
        lws_service(context, 1000);
    }

    // Clean up
    lws_context_destroy(context);
    return 0;
}
```

### Explanation

#### C WebSocket Server (`c_server.c`)

- **Includes**:
  - `libwebsockets.h`: Core libwebsockets functions.
  - `string.h`, `stdio.h`: For string manipulation and input/output.

- **`callback_websocket` Function**:
  - Handles different WebSocket events:
    - **`LWS_CALLBACK_ESTABLISHED`**: When a client connects.
    - **`LWS_CALLBACK_RECEIVE`**: When a message is received from the client.
      - Logs the received message.
      - Constructs a response message.
      - Sends the response back to the client.
    - **`LWS_CALLBACK_CLOSED`**: When a client disconnects.

- **Protocols Definition**:
  - Defines the WebSocket protocols the server supports. Here, it's a single protocol named `"websocket-protocol"`.

- **`main` Function**:
  - Sets up the WebSocket server to listen on port `9000`.
  - Creates the WebSocket context.
  - Enters an infinite loop to handle incoming connections and messages using `lws_service`.

- **Compilation**:
  ```bash
  gcc c_server.c -o c_server -lwebsockets
  ```

#### C WebSocket Client (`c_client.c`)

- **Includes**:
  - `libwebsockets.h`: Core libwebsockets functions.
  - `string.h`, `stdio.h`: For string manipulation and input/output.

- **`callback_websocket` Function**:
  - Handles different WebSocket events:
    - **`LWS_CALLBACK_CLIENT_ESTABLISHED`**: When the client connection is established.
      - Sends three distinct messages to the server.
    - **`LWS_CALLBACK_CLIENT_RECEIVE`**: When a message is received from the server.
      - Logs the received message.
    - **`LWS_CALLBACK_CLOSED`**: When the connection is closed.

- **Protocols Definition**:
  - Defines the WebSocket protocols the client supports, matching the server's protocol.

- **`main` Function**:
  - Sets up the WebSocket client.
  - Specifies the server address (`localhost`) and port (`9000`).
  - Initiates the connection to the server.
  - Enters an infinite loop to handle events using `lws_service`.

- **Compilation**:
  ```bash
  gcc c_client.c -o c_client -lwebsockets
  ```

#### Example Usage

1. **Start the C Server**:
   ```bash
   ./c_server
   ```
   Output:
   ```
   WebSocket server started on ws://localhost:9000
   Client connected
   ```

2. **Run the C Client**:
   ```bash
   ./c_client
   ```
   Output:
   ```
   Connected to server
   Sent to server: Client message 1
   Sent to server: Client message 2
   Sent to server: Client message 3
   Received from server: Server response to "Client message 1"
   Received from server: Server response to "Client message 2"
   Received from server: Server response to "Client message 3"
   Disconnected from server
   ```

3. **Server Output After Client Interaction**:
   ```
   Client connected
   Received from client: Client message 1
   Sent to client: Server response to "Client message 1"
   Received from client: Client message 2
   Sent to client: Server response to "Client message 2"
   Received from client: Client message 3
   Sent to client: Server response to "Client message 3"
   Client disconnected
   ```

---

## Conclusion

This guide demonstrated how to implement simple WebSocket server and client programs in **Python**, **JavaScript** (Node.js and Browser), and **C**. Each implementation facilitates the exchange of three independent string messages between the client and server, accompanied by detailed code comments for clarity.

**Key Highlights**:

- **Python** leverages the `websockets` library for straightforward asynchronous WebSocket communication.
- **JavaScript** utilizes the `ws` library for Node.js servers and the native `WebSocket` API for browser clients, showcasing both server-side and client-side perspectives.
- **C** implementations rely on the `libwebsockets` library to manage the complexities of WebSocket protocols, providing robust communication channels.

These examples serve as foundational templates for building more complex WebSocket-based applications across different programming environments.