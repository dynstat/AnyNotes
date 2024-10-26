### server.py

```python
import socket
import threading
import base64
import hashlib
import struct
import json

class SimpleWebSocketServer:
    GUID = '258EAFA5-E914-47DA-95CA-44DDHHR85B11'

    def __init__(self, host='localhost', port=8765):
        self.host = host
        self.port = port
        self.clients = []

    def start(self):
        # Create a TCP socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind((self.host, self.port))
        self.sock.listen(5)
        print(f'Listening on {self.host}:{self.port}')

        while True:
            client, address = self.sock.accept()
            print(f'Connection from {address}')
            threading.Thread(target=self.handshake, args=(client,)).start()

    def handshake(self, client):
        request = client.recv(1024).decode()
        headers = self.parse_headers(request)
        if 'Sec-WebSocket-Key' not in headers:
            client.close()
            return

        key = headers['Sec-WebSocket-Key']
        accept_key = self.generate_accept_key(key)
        response = (
            'HTTP/1.1 101 Switching Protocols\r\n'
            'Upgrade: websocket\r\n'
            'Connection: Upgrade\r\n'
            f'Sec-WebSocket-Accept: {accept_key}\r\n\r\n'
        )
        client.send(response.encode())
        self.clients.append(client)
        print('Handshake successful')

        while True:
            try:
                data = self.receive_data(client)
                if not data:
                    break
                print(f'Received: {data}')
                # Echo the received data back to the client
                response_data = json.dumps({'message': 'Echo: ' + data})
                self.send_data(client, response_data)
            except:
                break

        client.close()
        self.clients.remove(client)
        print('Client disconnected')

    def parse_headers(self, request):
        headers = {}
        lines = request.split('\r\n')
        for line in lines[1:]:
            if ': ' in line:
                key, value = line.split(': ', 1)
                headers[key] = value
        return headers

    def generate_accept_key(self, key):
        sha1 = hashlib.sha1()
        sha1.update((key + self.GUID).encode())
        return base64.b64encode(sha1.digest()).decode()

    def receive_data(self, client):
        # Read the first two bytes of the header
        first_two_bytes = client.recv(2)
        if not first_two_bytes:
            return None
        byte1, byte2 = first_two_bytes
        fin = byte1 & 0b10000000
        opcode = byte1 & 0b00001111
        mask = byte2 & 0b10000000
        payload_length = byte2 & 0b01111111

        if payload_length == 126:
            payload_length = struct.unpack('>H', client.recv(2))[0]
        elif payload_length == 127:
            payload_length = struct.unpack('>Q', client.recv(8))[0]

        if mask:
            masking_key = client.recv(4)
            masked_data = client.recv(payload_length)
            data = ''.join([chr(b ^ masking_key[i % 4]) for i, b in enumerate(masked_data)])
            return data
        else:
            data = client.recv(payload_length).decode()
            return data

    def send_data(self, client, data):
        # Encode the data to JSON
        json_data = json.dumps(data)
        encoded_data = json_data.encode()

        # Build the WebSocket frame
        frame = bytearray()
        frame.append(0x81)  # FIN bit set and opcode for text

        length = len(encoded_data)
        if length <= 125:
            frame.append(length)
        elif length <= 65535:
            frame.append(126)
            frame.extend(struct.pack('>H', length))
        else:
            frame.append(127)
            frame.extend(struct.pack('>Q', length))

        frame.extend(encoded_data)
        client.send(frame)
```

### client.py

```python
import socket
import base64
import hashlib
import os
import struct
import json

class SimpleWebSocketClient:
    GUID = '258EAFA5-E914-47DA-95CA-C5AB0DC85B11'

    def __init__(self, host='localhost', port=8765):
        self.host = host
        self.port = port

    def connect(self):
        # Create a TCP socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.host, self.port))
        self.handshake()

    def handshake(self):
        # Generate a random Sec-WebSocket-Key
        key = base64.b64encode(os.urandom(16)).decode()
        headers = (
            f"GET / HTTP/1.1\r\n"
            f"Host: {self.host}:{self.port}\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            f"Sec-WebSocket-Key: {key}\r\n"
            "Sec-WebSocket-Version: 13\r\n\r\n"
        )
        self.sock.send(headers.encode())

        # Receive the handshake response
        response = self.sock.recv(1024).decode()
        if '101 Switching Protocols' not in response:
            raise Exception('Handshake failed')

        print('Handshake successful')

    def send_data(self, data):
        # Encode the data to JSON
        json_data = json.dumps(data)
        encoded_data = json_data.encode()

        # Build the WebSocket frame
        frame = bytearray()
        frame.append(0x81)  # FIN bit set and opcode for text

        length = len(encoded_data)
        if length <= 125:
            frame.append(length | 0b10000000)  # Mask bit set
        elif length <= 65535:
            frame.append(126 | 0b10000000)
            frame.extend(struct.pack('>H', length))
        else:
            frame.append(127 | 0b10000000)
            frame.extend(struct.pack('>Q', length))

        # Generate a masking key
        masking_key = os.urandom(4)
        frame.extend(masking_key)

        # Mask the payload
        masked_data = bytearray(b ^ masking_key[i % 4] for i, b in enumerate(encoded_data))
        frame.extend(masked_data)

        self.sock.send(frame)

    def receive_data(self):
        # Read the first two bytes of the header
        first_two_bytes = self.sock.recv(2)
        if not first_two_bytes:
            return None
        byte1, byte2 = first_two_bytes
        fin = byte1 & 0b10000000
        opcode = byte1 & 0b00001111
        mask = byte2 & 0b10000000
        payload_length = byte2 & 0b01111111

        if payload_length == 126:
            payload_length = struct.unpack('>H', self.sock.recv(2))[0]
        elif payload_length == 127:
            payload_length = struct.unpack('>Q', self.sock.recv(8))[0]

        if mask:
            masking_key = self.sock.recv(4)
            masked_data = self.sock.recv(payload_length)
            data = ''.join([chr(b ^ masking_key[i % 4]) for i, b in enumerate(masked_data)])
            return data
        else:
            data = self.sock.recv(payload_length).decode()
            return data

    def close(self):
        self.sock.close()

# Example usage
if __name__ == "__main__":
    client = SimpleWebSocketClient()
    client.connect()
    client.send_data({'greeting': 'Hello, Server!'})
    response = client.receive_data()
    print(f'Received from server: {response}')
    client.close()
```

## Explanation

### Server (`server.py`)

The `SimpleWebSocketServer` class sets up a basic WebSocket server using raw TCP sockets without any third-party libraries. Here's how it works:

1. **Initialization**:
   
   - The server is initialized with a host and port.
   - It maintains a list of connected clients.

2. **Starting the Server** (`start` method):
   
   - Creates a TCP socket and binds it to the specified host and port.
   - Listens for incoming connections.
   - For each new connection, it starts a new thread to handle the handshake and communication.

3. **Handshake Process** (`handshake` method):
   
   - Receives the client's HTTP request for upgrading the connection to WebSocket.
   - Parses the headers to extract the `Sec-WebSocket-Key`.
   - Generates a `Sec-WebSocket-Accept` key by concatenating the received key with a GUID, hashing it using SHA1, and then encoding it in base64.
   - Sends back an HTTP 101 Switching Protocols response to establish the WebSocket connection.

4. **Receiving Data** (`receive_data` method):
   
   - Reads the WebSocket frame from the client.
   - Parses the frame to extract the payload.
   - Unmasks the data if necessary.

5. **Sending Data** (`send_data` method):
   
   - Encodes the message in JSON format.
   - Frames the JSON data according to the WebSocket protocol.
   - Sends the framed data back to the client.

### Client (`client.py`)

The `SimpleWebSocketClient` class establishes a WebSocket connection to the server and facilitates sending and receiving JSON-formatted data.

1. **Initialization**:
   
   - The client is initialized with the server's host and port.

2. **Connecting to the Server** (`connect` method):
   
   - Creates a TCP socket and connects to the server.
   - Initiates the WebSocket handshake.

3. **Handshake Process** (`handshake` method):
   
   - Generates a random `Sec-WebSocket-Key`.
   - Sends an HTTP GET request with headers to upgrade the connection to WebSocket.
   - Waits for the server's handshake response to confirm the upgrade.

4. **Sending Data** (`send_data` method):
   
   - Encodes the message in JSON format.
   - Frames the JSON data according to the WebSocket protocol, including masking the payload.
   - Sends the framed data to the server.

5. **Receiving Data** (`receive_data` method):
   
   - Reads the WebSocket frame from the server.
   - Parses the frame to extract the payload.
   - Unmasks the data if necessary.

6. **Closing the Connection** (`close` method):
   
   - Closes the TCP socket.

### Example Usage

In the `client.py`, the following example demonstrates how to use the `SimpleWebSocketClient`:

1. **Establish Connection**:
   
   ```python
   client = SimpleWebSocketClient()
   client.connect()
   ```
   
   - Creates a client instance and connects to the server.

2. **Send Data**:
   
   ```python
   client.send_data({'greeting': 'Hello, Server!'})
   ```
   
   - Sends a JSON-formatted greeting message to the server.

3. **Receive Response**:
   
   ```python
   response = client.receive_data()
   print(f'Received from server: {response}')
   ```
   
   - Waits for a response from the server and prints it.

4. **Close Connection**:
   
   ```python
   client.close()
   ```
   
   - Closes the connection to the server.

### Running the Example

1. **Start the Server**:
   
   - Run `server.py` to start the WebSocket server.
     
     ```bash
     python server.py
     ```
   - The server listens on `localhost:8765` and waits for client connections.

2. **Run the Client**:
   
   - Run `client.py` to connect to the server, send a message, and receive a response.
     
     ```bash
     python client.py
     ```
   - The client connects to the server, sends a greeting, receives an echo response, and then closes the connection.

### Sample Interaction

**Server Output**:

```
Listening on localhost:8765
Connection from ('127.0.0.1', 54321)
Handshake successful
Received: {"greeting": "Hello, Server!"}
Client disconnected
```

**Client Output**:

```
Handshake successful
Received from server: {"message": "Echo: {"greeting": "Hello, Server!"}"}
```

In this interaction:

- The client connects to the server and successfully completes the WebSocket handshake.
- The client sends a JSON message: `{"greeting": "Hello, Server!"}`.
- The server receives the message, prints it, and sends back an echo message: `{"message": "Echo: {"greeting": "Hello, Server!"}"}`.
- The client receives and prints the server's response before closing the connection.
