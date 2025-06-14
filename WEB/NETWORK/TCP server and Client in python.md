### Server.py

```python
import socket
import threading
import time

class TCPServer:
    def __init__(self, host='127.0.0.1', port=9999):
        """Initialize the TCP server with host and port."""
        self.host = host
        self.port = port
        # Create a socket object using IPv4 (AF_INET) and TCP (SOCK_STREAM)
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # Set socket option to reuse address to avoid "Address already in use" errors
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        # Dictionary to store connected clients
        self.clients = {}
        # Flag to control server running state
        self.running = False

    def start(self):
        """Start the server and listen for incoming connections."""
        try:
            # Bind the socket to the specified host and port
            self.server_socket.bind((self.host, self.port))
            # Listen for incoming connections (5 is the backlog parameter)
            self.server_socket.listen(5)
            self.running = True
            print(f"[*] Server started on {self.host}:{self.port}")
            
            # Start accepting client connections
            self.accept_connections()
        except KeyboardInterrupt:
            print("\n[!] Server interrupted by user")
            self.stop()  # Call stop to clean up resources
        except Exception as e:
            print(f"[!] Error starting server: {e}")
            self.stop()

    def accept_connections(self):
        """Accept incoming client connections and handle them in separate threads."""
        # Set a timeout on the socket to allow checking for interrupts
        self.server_socket.settimeout(1.0)
        
        while self.running:
            try:
                # Accept a client connection
                client_socket, client_address = self.server_socket.accept()
                print(f"[+] New connection from {client_address[0]}:{client_address[1]}")
                
                # Create a new thread to handle the client
                client_thread = threading.Thread(
                    target=self.handle_client,
                    args=(client_socket, client_address)
                )
                client_thread.daemon = True
                client_thread.start()
                
                # Store the client information
                self.clients[client_address] = {
                    'socket': client_socket,
                    'thread': client_thread
                }
            except socket.timeout:
                # This is expected due to the timeout we set
                continue
            except Exception as e:
                if self.running:
                    print(f"[!] Error accepting connection: {e}")
                break

    def handle_client(self, client_socket, client_address):
        """Handle communication with a connected client."""
        try:
            # Send welcome message to client
            welcome_msg = "Welcome to the TCP Server! Type 'exit' to disconnect."
            client_socket.send(welcome_msg.encode('utf-8'))
            
            while self.running:
                # Receive data from client (1024 bytes at a time)
                data = client_socket.recv(1024)
                if not data:
                    # If no data is received, the client has disconnected
                    break
                
                # Decode the received data
                message = data.decode('utf-8').strip()
                print(f"[>] Received from {client_address[0]}:{client_address[1]}: {message}")
                
                # Check if client wants to exit
                if message.lower() == 'exit':
                    break
                
                # Process the message (echo it back with a timestamp)
                response = f"[{time.strftime('%H:%M:%S')}] Server received: {message}"
                client_socket.send(response.encode('utf-8'))
                
        except Exception as e:
            print(f"[!] Error handling client {client_address[0]}:{client_address[1]}: {e}")
        finally:
            # Clean up when client disconnects
            self.disconnect_client(client_address)

    def disconnect_client(self, client_address):
        """Disconnect a client and clean up resources."""
        if client_address in self.clients:
            try:
                # Close the client socket
                self.clients[client_address]['socket'].close()
                print(f"[-] Client {client_address[0]}:{client_address[1]} disconnected")
            except Exception as e:
                print(f"[!] Error disconnecting client: {e}")
            finally:
                # Remove client from the dictionary
                del self.clients[client_address]

    def stop(self):
        """Stop the server and clean up all resources."""
        self.running = False
        
        # Disconnect all clients
        for client_address in list(self.clients.keys()):
            self.disconnect_client(client_address)
        
        # Close the server socket
        try:
            self.server_socket.close()
            print("[*] Server stopped")
        except Exception as e:
            print(f"[!] Error stopping server: {e}")


if __name__ == "__main__":
    server = None
    try:
        # Create and start the server
        server = TCPServer()
        # Start the server in a separate thread to allow keyboard interrupts
        server_thread = threading.Thread(target=server.start)
        server_thread.daemon = True
        server_thread.start()
        
        # Keep the main thread running
        while True:
            try:
                cmd = input("Enter 'exit' to stop the server: ")
                if cmd.lower() == 'exit':
                    break
            except KeyboardInterrupt:
                print("\n[!] Server interrupted by user")
                break
    except Exception as e:
        print(f"\n[!] Error: {e}")
    finally:
        # Stop the server when exiting
        if server:
            server.stop()
```



### Clinet.py

```python
import socket
import threading
import time

class TCPClient:
    def __init__(self, host='127.0.0.1', port=9999):
        """Initialize the TCP client with server host and port."""
        self.host = host
        self.port = port
        # Create a socket object using IPv4 (AF_INET) and TCP (SOCK_STREAM)
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # Flag to control client running state
        self.running = False
        # Thread for receiving messages
        self.receive_thread = None
        # Lock for synchronizing input prompt with message reception
        self.input_lock = threading.Lock()
        # Event to signal when a message has been received
        self.message_received = threading.Event()
        # Flag to indicate if this is the first message
        self.first_message = True

    def connect(self):
        """Connect to the TCP server."""
        try:
            print(f"[*] Connecting to server at {self.host}:{self.port}...")
            # Connect to the server
            self.client_socket.connect((self.host, self.port))
            self.running = True
            print("[+] Connected to server")
            
            # Start a thread to receive messages from the server
            self.receive_thread = threading.Thread(target=self.receive_messages)
            self.receive_thread.daemon = True
            self.receive_thread.start()
            
            # Wait for the initial welcome message from the server
            self.message_received.wait()
            
            return True
        except Exception as e:
            print(f"[!] Connection error: {e}")
            return False

    def receive_messages(self):
        """Receive and display messages from the server."""
        while self.running:
            try:
                # Receive data from the server (1024 bytes at a time)
                data = self.client_socket.recv(1024)
                if not data:
                    # If no data is received, the server has disconnected
                    print("[!] Server disconnected")
                    self.disconnect()
                    break
                
                # Acquire the lock before printing the message
                with self.input_lock:
                    # Decode and display the received message
                    message = data.decode('utf-8')
                    print(f"\n[SERVER] {message}")
                
                # Signal that a message has been received
                self.message_received.set()
                
            except Exception as e:
                if self.running:
                    print(f"[!] Error receiving message: {e}")
                break

    def send_message(self, message):
        """Send a message to the server."""
        try:
            # Encode and send the message
            self.client_socket.send(message.encode('utf-8'))
            # Reset the message received event
            self.message_received.clear()
            return True
        except Exception as e:
            print(f"[!] Error sending message: {e}")
            self.disconnect()
            return False

    def disconnect(self):
        """Disconnect from the server and clean up resources."""
        self.running = False
        try:
            self.client_socket.close()
            print("[*] Disconnected from server")
        except Exception as e:
            print(f"[!] Error disconnecting: {e}")

    def start(self):
        """Start an interactive session with the server."""
        if not self.running:
            if not self.connect():
                return
        
        try:
            # Main loop for sending messages
            while self.running:
                # Acquire the lock before prompting for input
                with self.input_lock:
                    message = input("\nEnter message (or 'exit' to quit): ")
                
                # Check if user wants to exit
                if message.lower() == 'exit':
                    # Send exit message to server before disconnecting
                    self.send_message(message)
                    time.sleep(0.5)  # Give server time to process
                    break
                
                # Send the message
                if not self.send_message(message):
                    break
                    
                # Wait for a response from the server before prompting again
                # This ensures we only prompt after receiving a message
                self.message_received.wait()
                
        except KeyboardInterrupt:
            print("\n[!] Client interrupted")
        finally:
            # Disconnect when exiting
            self.disconnect()


if __name__ == "__main__":
    # Create a client instance
    client = TCPClient()
    
    # Start interactive session
    client.start()
```