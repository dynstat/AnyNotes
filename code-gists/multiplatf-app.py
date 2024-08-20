import asyncio
import websockets
import ctypes
import ssl
import json
import os
import platform
import logging
from cryptography.fernet import Fernet
import traceback

# Configure logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

def load_library():
    system = platform.system()
    if system == "Windows":
        lib_name = 'softToken.dll'
    elif system == "Darwin":  # macOS
        lib_name = 'softToken.dylib'
    elif system == "Linux":
        lib_name = 'softToken.so'
    else:
        raise OSError(f"Unsupported operating system: {system}")
    
    # Get the directory of the current script
    current_dir = os.path.dirname(os.path.abspath(__file__))
    lib_path = os.path.join(current_dir, lib_name)
    
    if not os.path.exists(lib_path):
        raise FileNotFoundError(f"Library file not found: {lib_path}")
    
    try:
        return ctypes.CDLL(lib_path)
    except OSError as e:
        logger.error(f"Failed to load softToken library: {e}")
        raise

try:
    softToken = load_library()
except OSError as e:
    logger.error(f"Failed to load softToken library: {e}")
    raise

# Define the process_apdu function signature
softToken.process_apdu.argtypes = [ctypes.POINTER(ctypes.c_ubyte), ctypes.c_int, ctypes.POINTER(ctypes.c_ubyte), ctypes.POINTER(ctypes.c_int)]
softToken.process_apdu.restype = ctypes.c_int

# Function to handle APDU processing
def handle_apdu(apdu_command):
    command_len = len(apdu_command)
    response_len = ctypes.c_int(256)  # Assuming max response length
    response_apdu = (ctypes.c_ubyte * 256)()
    
    try:
        result = softToken.process_apdu(
            (ctypes.c_ubyte * command_len)(*apdu_command),
            command_len,
            response_apdu,
            ctypes.byref(response_len)
        )
        
        if result != 0:
            raise Exception(f"Error processing APDU: {result}")
        
        return bytes(response_apdu[:response_len.value])
    except Exception as e:
        logger.error(f"APDU processing error: {e}")
        raise

# Encryption key for secure communication
ENCRYPTION_KEY = Fernet.generate_key()
fernet = Fernet(ENCRYPTION_KEY)

# WebSocket handler
async def handle_connection(websocket, path):
    try:
        # Perform authentication here (e.g., JWT, client certificates)
        # For simplicity, we're using a basic token-based auth
        auth_token = await websocket.recv()
        if not authenticate(auth_token):
            await websocket.close(code=4001, reason="Authentication failed")
            return

        logger.info("Client authenticated successfully")

        async for encrypted_message in websocket:
            try:
                # Decrypt the incoming message
                message = fernet.decrypt(encrypted_message).decode()
                data = json.loads(message)
                apdu_command = data['apdu_command']
                
                logger.info(f"Received APDU command: {apdu_command}")
                
                # Process the APDU command
                response_apdu = handle_apdu(apdu_command)
                
                # Encrypt and send the response back to the client
                encrypted_response = fernet.encrypt(json.dumps({'response_apdu': list(response_apdu)}).encode())
                await websocket.send(encrypted_response)
                
                logger.info(f"Sent response: {list(response_apdu)}")
            except Exception as e:
                logger.error(f"Error processing message: {e}")
                logger.error(traceback.format_exc())
                await websocket.close(code=1011, reason=str(e))
                return

    except websockets.exceptions.ConnectionClosed:
        logger.info("Client disconnected")
    except Exception as e:
        logger.error(f"Error in WebSocket connection: {e}")
        logger.error(traceback.format_exc())
        await websocket.close(code=1011, reason=str(e))

def authenticate(token):
    # Implement your authentication logic here
    # For example, verify JWT token or check against a database
    return token == "valid_token"  # Placeholder

# Main function to start the WebSocket server
async def main():
    # ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    # ssl_context.load_cert_chain(certfile='path/to/cert.pem', keyfile='path/to/key.pem')
    
    server = await websockets.serve(
        handle_connection, 
        "0.0.0.0",  # Listen on all available interfaces
        8765, 
        # ssl=ssl_context,
        max_size=1048576,  # 1MB max message size
        max_queue=32,  # Limit pending connections
        compression=None,  # Disable compression for performance
        ping_interval=None, # Disable pinging
        ping_timeout=None # Disable ping timeout
    )
    
    logger.info(f"WebSocket server started with encryption key: {ENCRYPTION_KEY}")
    await server.wait_closed()

if __name__ == "__main__":
    asyncio.run(main())