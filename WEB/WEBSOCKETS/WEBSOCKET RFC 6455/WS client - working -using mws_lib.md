This WebSocket client is implemented in C and is designed to connect to a WebSocket server running at `ws://localhost:8765/`. It checks for server availability in a separate thread and maintains a connection while periodically exchanging messages. Below is a **detailed line-by-line explanation** of the program.

---
```c
/*
 * ws_client.c
 *
 * A sample WebSocket client that uses the mws_lib library for client-side
 * WebSocket operations. This client uses a separate thread to check server
 * availability and connects using the URI "ws://localhost:8765/" when available.
 *
 * The communication loop does the following:
 *   - Calls ws_service() periodically to handle WebSocket control frames
 *     (ping/pong/close).
 *   - Receives any incoming data from the WebSocket.
 *   - Sends a test message every 10 seconds.
 *
 * The client stays connected until either the client or server closes the connection.
 *
 * This file is intended to be built using Visual Studio Code on Windows.
 */

// IMPORTANT: Define WIN32_LEAN_AND_MEAN before any Windows headers
#define WIN32_LEAN_AND_MEAN

// Include Windows headers first
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>  // For _beginthreadex

// Then include standard C headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// Finally include your custom headers
#include "mws_lib.h"
// Link with Ws2_32.lib
//#pragma comment(lib, "Ws2_32.lib")

// Thread synchronization objects
HANDLE serverCheckThread = NULL;
CRITICAL_SECTION stateLock;
CONDITION_VARIABLE serverAvailableCV;
CONDITION_VARIABLE clientConnectedCV;

// Shared state variables
volatile bool serverAvailable = false;
volatile bool clientConnected = false;
volatile bool terminateThread = false;

// Server check thread function
unsigned __stdcall serverCheckThreadFunc(void* arg) {
    const char* host = "localhost";
    int port = 8765;
    int retryDelay = 2000; // Start with 2 seconds

    while (!terminateThread) {
        // Check if client is connected - if so, wait until disconnected
        EnterCriticalSection(&stateLock);
        while (clientConnected && !terminateThread) {
            // Client is connected, wait for it to disconnect
            SleepConditionVariableCS(&clientConnectedCV, &stateLock, INFINITE);
        }
        
        // Reset server availability flag when starting to check
        serverAvailable = false;
        LeaveCriticalSection(&stateLock);
        
        if (terminateThread) break;
        
        // Check server availability with exponential backoff
        while (!terminateThread) {
            if (ws_check_server_available(host, port)) {
                // Server is available
                EnterCriticalSection(&stateLock);
                serverAvailable = true;
                LeaveCriticalSection(&stateLock);
                
                // Signal main thread that server is available
                WakeConditionVariable(&serverAvailableCV);
                
                printf("Server check thread: Server is available!\n");
                break;
            }
            
            printf("Server check thread: Server not available. Retrying in %d ms...\n", retryDelay);
            Sleep(retryDelay);
            retryDelay = min(retryDelay * 2, 30000); // Exponential backoff, capped at 30 seconds
        }
        
        // Wait for client to connect or for thread termination
        EnterCriticalSection(&stateLock);
        while (serverAvailable && !clientConnected && !terminateThread) {
            // Server is available but client not yet connected
            SleepConditionVariableCS(&serverAvailableCV, &stateLock, INFINITE);
        }
        LeaveCriticalSection(&stateLock);
    }
    
    printf("Server check thread: Terminating\n");
    return 0;
}

int main(void) {
    // Seed the random generator once
    srand((unsigned int)time(NULL));

    // Initialize synchronization objects
    InitializeCriticalSection(&stateLock);
    InitializeConditionVariable(&serverAvailableCV);
    InitializeConditionVariable(&clientConnectedCV);

    // Initialize Winsock. If this fails, exit.
    if (ws_init() != 0) {
        fprintf(stderr, "ws_client: Failed to initialize Winsock.\n");
        return 1;
    }
    printf("ws_client: Starting WebSocket client...\n");

    // Start server check thread
    serverCheckThread = (HANDLE)_beginthreadex(NULL, 0, serverCheckThreadFunc, NULL, 0, NULL);
    if (serverCheckThread == NULL) {
        fprintf(stderr, "ws_client: Failed to create server check thread.\n");
        ws_cleanup();
        return 1;
    }

    while (1) {
        // Wait for server to become available
        EnterCriticalSection(&stateLock);
        while (!serverAvailable && !terminateThread) {
            printf("ws_client: Waiting for server to become available...\n");
            SleepConditionVariableCS(&serverAvailableCV, &stateLock, INFINITE);
        }
        LeaveCriticalSection(&stateLock);
        
        if (terminateThread) break;
        
        printf("ws_client: Server is available! Attempting to connect...\n");

        // Create a new WebSocket context
        ws_ctx* ctx = ws_create_ctx();
        if (ctx == NULL) {
            printf("ws_client: Failed to allocate WebSocket context. Retrying...\n");
            Sleep(2000);
            continue;
        }

        // Implement exponential backoff for connection attempts
        int retryDelay = 2000; // Reset retry delay
        int connectAttempts = 0;
        int maxConnectAttempts = 5; // Maximum number of connection attempts before giving up
        bool connected = false;
        
        while (connectAttempts < maxConnectAttempts && !connected) {
            if (ws_connect(ctx, "ws://localhost:8765/") == 0) {
                connected = true;
                
                // Update connection state
                EnterCriticalSection(&stateLock);
                clientConnected = true;
                LeaveCriticalSection(&stateLock);
                
                // Signal server check thread that client is connected
                WakeConditionVariable(&clientConnectedCV);
                break;
            }
            
            printf("ws_client: Failed to connect to the server. Attempt %d of %d. Retrying in %d ms...\n", 
                   connectAttempts + 1, maxConnectAttempts, retryDelay);
            Sleep(retryDelay);
            retryDelay = min(retryDelay * 2, 30000); // Exponential backoff, capped at 30 seconds
            connectAttempts++;
        }
        
        if (!connected) {
            printf("ws_client: Failed to connect after %d attempts. Restarting connection process.\n", maxConnectAttempts);
            ws_destroy_ctx(ctx);
            
            // Reset server availability to trigger rechecking
            EnterCriticalSection(&stateLock);
            serverAvailable = false;
            LeaveCriticalSection(&stateLock);
            
            // Signal server check thread to resume checking
            WakeConditionVariable(&serverAvailableCV);
            continue;
        }
        
        printf("ws_client: Connected to WebSocket server at ws://localhost:8765/!\n");

        // Communication loop: as long as the connection remains open
        time_t lastMsgTime = time(NULL);
        while (ws_get_state(ctx) == WS_STATE_OPEN) {
            // Process control frames (like PING, PONG, and CLOSE)
            if (ws_service(ctx) != 0 || !ws_check_connection(ctx)) {
                printf("ws_client: Connection issue detected during service.\n");
                break;
            }

            // Check for any incoming application data
            char recvBuffer[1024];
            int bytesReceived = ws_recv(ctx, recvBuffer, sizeof(recvBuffer) - 1);
            if (bytesReceived > 0) {
                // Null-terminate and print the received data
                recvBuffer[bytesReceived] = '\0';
                printf("ws_client: Received: %s\n", recvBuffer);
            } else if (bytesReceived < 0 && ws_get_state(ctx) == WS_STATE_OPEN) {
                // Only log errors if we're still supposed to be connected
                printf("ws_client: Error receiving data.\n");
            }

            // Every 10 seconds, send a test message
            time_t currentTime = time(NULL);
            if (currentTime - lastMsgTime >= 10) {
                const char* testMsg = "Hello from WebSocket client!";
                if (ws_send(ctx, testMsg, strlen(testMsg), WS_OPCODE_TEXT) == 0) {
                    printf("ws_client: Sent: %s\n", testMsg);
                } else {
                    printf("ws_client: Failed to send test message.\n");
                    break; // Exit the loop if sending fails
                }
                lastMsgTime = currentTime;
            }

            // Sleep briefly (100 ms) so as to yield CPU time
            Sleep(100);
        }

        // Implement graceful closing
        printf("ws_client: Connection ending. Sending close frame...\n");
        ws_close(ctx); // Send a proper close frame
        Sleep(500);    // Give time for the close handshake to complete
        
        printf("ws_client: Disconnected from server. Cleaning up context.\n");
        ws_destroy_ctx(ctx);
        
        // Update connection state
        EnterCriticalSection(&stateLock);
        clientConnected = false;
        LeaveCriticalSection(&stateLock);
        
        // Signal server check thread that client is disconnected
        WakeConditionVariable(&clientConnectedCV);
        
        // Wait a moment before attempting to reconnect
        Sleep(1000);
    }

    // Signal thread to terminate and wait for it
    terminateThread = true;
    WakeConditionVariable(&serverAvailableCV);
    WakeConditionVariable(&clientConnectedCV);
    WaitForSingleObject(serverCheckThread, INFINITE);
    CloseHandle(serverCheckThread);
    
    // Cleanup synchronization objects
    DeleteCriticalSection(&stateLock);
    
    // Cleanup Winsock resources
    ws_cleanup();
    return 0;
}
```


----

## **1. Preprocessor Directives & Includes**

```c
// IMPORTANT: Define WIN32_LEAN_AND_MEAN before any Windows headers
#define WIN32_LEAN_AND_MEAN
```
- **Purpose**: Reduces the size of Windows headers by excluding unnecessary components.
- **Benefit**: Speeds up compilation and reduces potential conflicts.

```c
// Include Windows headers first
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>  // For _beginthreadex
```
- **Why?** These headers provide networking (`winsock2.h`), multithreading (`process.h`), and system utilities (`windows.h`).

```c
// Then include standard C headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
```
- **Purpose**: These are standard C libraries required for:
  - **I/O operations** (`stdio.h`)
  - **Memory management** (`stdlib.h`)
  - **String handling** (`string.h`)
  - **Time functions** (`time.h`)
  - **Boolean logic** (`stdbool.h`).

```c
// Finally include your custom headers
#include "mws_lib.h"
```
- **Custom WebSocket Library**: Provides functions for WebSocket operations, like checking server availability, creating WebSocket connections, sending/receiving messages.

```c
// Link with Ws2_32.lib
//#pragma comment(lib, "Ws2_32.lib")
```
- **Purpose**: Explicitly tells the linker to include Windows Sockets 2 (`Ws2_32.lib`), required for networking.

---

## **2. Global Variables & Synchronization Objects**
```c
// Thread synchronization objects
HANDLE serverCheckThread = NULL;
CRITICAL_SECTION stateLock;
CONDITION_VARIABLE serverAvailableCV;
CONDITION_VARIABLE clientConnectedCV;
```
- **Purpose**: These are synchronization primitives to manage shared state in multithreading.
  - `CRITICAL_SECTION stateLock`: Prevents race conditions.
  - `CONDITION_VARIABLE serverAvailableCV`: Notifies the main thread when the server is available.
  - `CONDITION_VARIABLE clientConnectedCV`: Notifies the checking thread when the client connects.

```c
// Shared state variables
volatile bool serverAvailable = false;
volatile bool clientConnected = false;
volatile bool terminateThread = false;
```
- **Why `volatile`?** It prevents compiler optimizations that might cause inconsistent updates in a multithreaded environment.

---

## **3. Server Check Thread**
```c
unsigned __stdcall serverCheckThreadFunc(void* arg) {
```
- **Thread Function**: Checks if the WebSocket server is online.

```c
const char* host = "localhost";
int port = 8765;
int retryDelay = 2000; // Start with 2 seconds
```
- **Hardcoded Server Address**: The WebSocket server runs on `ws://localhost:8765/`.

### **Checking Server Availability**
```c
while (!terminateThread) {
```
- **Loop runs indefinitely** unless `terminateThread` is set to `true`.

```c
    EnterCriticalSection(&stateLock);
    while (clientConnected && !terminateThread) {
        SleepConditionVariableCS(&clientConnectedCV, &stateLock, INFINITE);
    }
```
- **If the client is already connected, pause the checking thread** until the connection is lost.

```c
    serverAvailable = false;
    LeaveCriticalSection(&stateLock);
```
- **Resets server availability status before checking again**.

### **Exponential Backoff for Server Check**
```c
while (!terminateThread) {
    if (ws_check_server_available(host, port)) {
```
- **Uses `ws_check_server_available()` to check if the WebSocket server is reachable**.

```c
        EnterCriticalSection(&stateLock);
        serverAvailable = true;
        LeaveCriticalSection(&stateLock);
```
- **Marks the server as available**.

```c
        WakeConditionVariable(&serverAvailableCV);
```
- **Notifies the main thread that the server is up**.

```c
        break;
```
- **Stops the checking loop** once the server is confirmed online.

```c
    printf("Server check thread: Server not available. Retrying in %d ms...\n", retryDelay);
    Sleep(retryDelay);
    retryDelay = min(retryDelay * 2, 30000); // Exponential backoff, capped at 30 seconds
```
- **Implements exponential backoff** to prevent excessive retries.

---

## **4. Main Function**
```c
int main(void) {
```
- **Entry Point of the Program**.

```c
srand((unsigned int)time(NULL));
```
- **Seeds the random number generator** (used for potential future randomness).

### **Initializing Synchronization Primitives**
```c
InitializeCriticalSection(&stateLock);
InitializeConditionVariable(&serverAvailableCV);
InitializeConditionVariable(&clientConnectedCV);
```
- **Initializes synchronization primitives for thread safety**.

### **Initializing WebSocket Client**
```c
if (ws_init() != 0) {
    fprintf(stderr, "ws_client: Failed to initialize Winsock.\n");
    return 1;
}
```
- **Ensures Windows networking (Winsock) is initialized before proceeding**.

### **Starting the Server Check Thread**
```c
serverCheckThread = (HANDLE)_beginthreadex(NULL, 0, serverCheckThreadFunc, NULL, 0, NULL);
```
- **Creates the server-checking thread**.

---

## **5. Connecting to WebSocket Server**
```c
while (1) {
```
- **Main loop** that maintains WebSocket connectivity.

```c
    EnterCriticalSection(&stateLock);
    while (!serverAvailable && !terminateThread) {
        SleepConditionVariableCS(&serverAvailableCV, &stateLock, INFINITE);
    }
    LeaveCriticalSection(&stateLock);
```
- **Waits until the server is available before attempting a connection**.

### **Attempting Connection**
```c
ws_ctx* ctx = ws_create_ctx();
if (ctx == NULL) {
    printf("ws_client: Failed to allocate WebSocket context. Retrying...\n");
    Sleep(2000);
    continue;
}
```
- **Allocates WebSocket context for connection**.

```c
if (ws_connect(ctx, "ws://localhost:8765/") == 0) {
    connected = true;
```
- **Tries connecting to the WebSocket server**.

---

## **6. Communication Loop**
```c
time_t lastMsgTime = time(NULL);
while (ws_get_state(ctx) == WS_STATE_OPEN) {
```
- **Continues communication as long as WebSocket is open**.

```c
    if (ws_service(ctx) != 0 || !ws_check_connection(ctx)) {
```
- **Handles WebSocket control frames and checks connection health**.

```c
    char recvBuffer[1024];
    int bytesReceived = ws_recv(ctx, recvBuffer, sizeof(recvBuffer) - 1);
    if (bytesReceived > 0) {
        recvBuffer[bytesReceived] = '\0';
        printf("ws_client: Received: %s\n", recvBuffer);
    }
```
- **Receives and prints messages from the server**.

### **Sending Messages Periodically**
```c
if (currentTime - lastMsgTime >= 10) {
    const char* testMsg = "Hello from WebSocket client!";
    ws_send(ctx, testMsg, strlen(testMsg), WS_OPCODE_TEXT);
    lastMsgTime = currentTime;
}
```
- **Sends a message every 10 seconds**.

---

## **7. Graceful Disconnection**
```c
ws_close(ctx);
Sleep(500);
ws_destroy_ctx(ctx);
```
- **Closes the WebSocket connection properly**.

```c
terminateThread = true;
WakeConditionVariable(&serverAvailableCV);
WakeConditionVariable(&clientConnectedCV);
WaitForSingleObject(serverCheckThread, INFINITE);
CloseHandle(serverCheckThread);
```
- **Ensures the checking thread terminates properly**.

```c
ws_cleanup();
return 0;
```
- **Cleans up Winsock resources** and exits.

---

### **Flow Summary**
1. **Thread checks server availability**.
2. **Main loop waits for server to be available**.
3. **Client attempts connection** (with exponential backoff).
4. **Client maintains WebSocket connection**.
5. **Client exchanges messages with server**.
6. **If disconnected, retries connection**.
7. **Terminates gracefully on exit**.
---
---
