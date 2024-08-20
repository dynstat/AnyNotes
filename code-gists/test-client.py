import asyncio
import websockets
import json
from cryptography.fernet import Fernet

# Use the same encryption key as in the server
ENCRYPTION_KEY = b'1UR8e4q99t9PpwiKHrpB_hdjiKT1dTRdoNjJrw0vpQk='  # Use the key printed by the server
fernet = Fernet(ENCRYPTION_KEY)

async def send_apdu(websocket, apdu):
    # Encrypt and send the APDU command
    message = json.dumps({'apdu_command': apdu})
    encrypted_message = fernet.encrypt(message.encode())
    await websocket.send(encrypted_message)
    print(f"Sent encrypted APDU: {apdu}")

    # Receive and decrypt the response
    encrypted_response = await websocket.recv()
    decrypted_response = fernet.decrypt(encrypted_response).decode()
    response_data = json.loads(decrypted_response)
    print("Received response:", response_data['response_apdu'])

async def test_client():
    uri = "ws://localhost:8765"
    async with websockets.connect(uri, ping_interval=None) as websocket:
        # Send authentication token
        await websocket.send("valid_token")
        print("Sent authentication token")

        # Define the three APDU commands
        apdu_commands = [
            [0x00, 0xA4, 0x04, 0x00, 0x0A, 0xA0, 0x00, 0x00, 0x00, 0x62, 0x03, 0x01, 0x0C, 0x06, 0x01],
            [0x00, 0x20, 0x00, 0x80, 0x08, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38],
            [0x00, 0xC0, 0x00, 0x00, 0x08]
        ]

        for i, apdu in enumerate(apdu_commands, 1):
            input(f"Press Enter to send APDU {i}...")
            await send_apdu(websocket, apdu)
            print()

        print("All APDUs sent. Press Enter to exit...")
        input()

if __name__ == "__main__":
    asyncio.run(test_client())