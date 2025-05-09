
Okay, let's dive into how standard input/output (stdio) transport works in FastMCP.

To understand this, I need to examine the codebase, particularly focusing on how the client and server communicate when using this method. I'll start by searching for relevant files and code snippets.



Ran tool
The search results point to several relevant files:

1.  `src/fastmcp/client/transports.py`: Defines various client-side transport mechanisms, including `StdioTransport` and specific implementations like `PythonStdioTransport`, `NodeStdioTransport`, etc. This seems crucial for the client side.
2.  `src/fastmcp/server/server.py`: Contains the `FastMCP` server class, which has methods like `run_async`, `run_stdio_async`, and references `mcp.server.stdio.stdio_server`. This handles the server side.
3.  `src/fastmcp/cli/cli.py`: Shows how the `fastmcp run` command uses the transport options.
4.  `src/fastmcp/client/client.py`: Defines the main `Client` class which uses a `ClientTransport` instance.

Let's read these files to get the full picture. I'll start with the client transports, then the server implementation.



Read file: src/fastmcp/server/server.py

Read file: src/fastmcp/client/transports.py
Based on the code, here are detailed notes on how stdio transport works in FastMCP:

## FastMCP Stdio Transport Explained

This document explains the low-level workings of the Standard Input/Output (stdio) transport mechanism used for communication between a FastMCP client and a FastMCP server when they run as separate processes on the same machine.

**1. Core Concept:**

*   The fundamental idea is to use the standard input (`stdin`) and standard output (`stdout`) streams of a child process (the server) as the communication channel for the parent process (the client).
*   The client launches the server as a subprocess.
*   The client writes MCP requests (formatted messages) to the server's `stdin`.
*   The client reads MCP responses and notifications from the server's `stdout`.
*   This avoids the need for network sockets (like WebSockets or SSE) when the client and server are co-located.

**2. Client-Side Implementation (`src/fastmcp/client/transports.py`):**

*   **`StdioTransport` Class:** This is the base class for stdio-based transports.
    *   `__init__(self, command: str, args: list[str], env: dict[str, str] | None = None, cwd: str | None = None)`: Stores the command to execute (e.g., `python`, `node`, `fastmcp`), its arguments, optional environment variables (`env`), and the working directory (`cwd`) for the server subprocess.
    *   `connect_session(self, **session_kwargs)`: This is the core method that establishes the connection.
        *   It creates a `StdioServerParameters` object from the stored `command`, `args`, `env`, and `cwd`. This object encapsulates how to start the server process.
        *   It calls `mcp.client.stdio.stdio_client(server_params)` (from the underlying `mcp-python` library). This function:
            *   Uses `anyio.open_process` (or similar asynchronous process management) to start the specified `command` with its `args`, `env`, and `cwd`.
            *   Crucially, it configures the subprocess's `stdin` and `stdout` to be pipes that the client can read from and write to asynchronously. `stderr` is typically redirected separately, often to the client's `stderr` or a log file, to avoid polluting the communication channel.
            *   Returns an asynchronous context manager yielding a `(read_stream, write_stream)` tuple. `read_stream` is connected to the server's `stdout`, and `write_stream` is connected to the server's `stdin`.
        *   It then creates a standard `mcp.ClientSession` using these streams.
        *   The `ClientSession` handles the MCP protocol details (message serialization/deserialization, request/response matching, etc.) over the provided streams.
        *   The initialized `ClientSession` is yielded for use within an `async with` block. When the block exits, `stdio_client` ensures the subprocess is terminated gracefully.
*   **Specialized Subclasses:**
    *   `PythonStdioTransport`: Pre-configures `command` to `sys.executable` (the current Python interpreter) and takes a `script_path` argument.
    *   `FastMCPStdioTransport`: Pre-configures `command` to `"fastmcp"` and `args` to `["run", script_path]`, using the FastMCP CLI to launch the server.
    *   `NodeStdioTransport`: Similar to `PythonStdioTransport` but for Node.js scripts (`command="node"`).
    *   `UvxStdioTransport`, `NpxStdioTransport`: Handle launching servers via `uvx` or `npx` respectively, constructing the appropriate command and arguments.
*   **`infer_transport` Function:** Automatically creates the correct `StdioTransport` subclass if given a file path ending in `.py` or `.js`.

**3. Server-Side Implementation (`src/fastmcp/server/server.py`):**

*   **`FastMCP.run_async(self, transport: Literal["stdio", "sse"] | None = None, ...)`:** The main entry point for running the server. If `transport` is specified or defaults to `"stdio"`, it calls `run_stdio_async`.
*   **`FastMCP.run_stdio_async(self) -> None`:**
    *   Calls `mcp.server.stdio.stdio_server()`. This function (from the underlying `mcp-python` library):
        *   Provides an asynchronous context manager that yields a `(read_stream, write_stream)` tuple representing the process's *own* `stdin` and `stdout`, adapted for asynchronous reading and writing (using `anyio` streams).
    *   Inside the `async with stdio_server() ...` block:
        *   It calls `self._mcp_server.run(read_stream, write_stream, ...)`. `self._mcp_server` is an instance of `mcp.server.lowlevel.server.Server`.
        *   The `MCPServer.run` method starts the main server loop:
            *   It continuously listens for incoming messages on the `read_stream` (which is the process's `stdin`).
            *   It deserializes these messages according to the MCP JSON-RPC format.
            *   It dispatches the requests to the appropriate handlers registered with the server (e.g., `_mcp_call_tool`, `_mcp_list_resources`).
            *   It serializes the responses or notifications and writes them to the `write_stream` (which is the process's `stdout`).
*   **`fastmcp run` CLI Command (`src/fastmcp/cli/cli.py`):**
    *   This command parses the file path and optional server object name.
    *   It imports the specified Python file/object to get the `FastMCP` instance.
    *   It calls `server.run(transport="stdio", ...)` (or defaults to stdio if no transport is specified), triggering the server-side stdio logic described above.

**4. Data Flow Visualization:**

```
+-----------------+                  +-----------------+
| Client Process  |                  | Server Process  |
| (e.g., your app)|                  | (e.g., fastmcp |
|                 |                  |      run ...)   |
|-----------------|                  |-----------------|
| StdioTransport  |                  | FastMCP Server  |
| .connect_session| --- Spawns -->   | .run_stdio_async|
|                 |                  |                 |
|   stdio_client  |                  |   stdio_server  |
|                 |                  |                 |
|     anyio       |                  |     anyio       |
| .open_process() |                  | .stdin/stdout() |
+-------|---------+                  +--------|--------+
        |                                    |
+-------v---------+      PIPE      +---------v-------+
| write_stream    |--------------->| read_stream     |
| (Client ->      | (Server's stdin)| (Server ->     |
|  Server's stdin)|                  | MCP Server Loop)|
+-----------------+                  +-----------------+

+-----------------+      PIPE      +-----------------+
| read_stream     |<---------------| write_stream    |
| (Client <-      | (Server's stdout| (Server ->     |
|  Server's stdout)|                  | Client)         |
+-----------------+                  +-----------------+
```

**5. Message Format:**

*   Communication over stdio uses the standard MCP JSON-RPC 2.0 format. Messages (requests, responses, notifications) are JSON objects, typically sent one per line.
*   The `ClientSession` on the client side and the `MCPServer` on the server side handle the serialization (Python object -> JSON string) and deserialization (JSON string -> Python object) of these messages.

**In Summary:**

FastMCP's stdio transport leverages standard process management and I/O redirection. The client starts the server as a child process and uses its `stdin` and `stdout` as direct, asynchronous communication channels. The underlying `mcp-python` library provides the `stdio_client` and `stdio_server` utilities to manage the process lifecycle and adapt the standard streams for asynchronous communication, while `ClientSession` and `MCPServer` handle the MCP protocol layer on top of these streams. This provides an efficient local communication method without network overhead.


---
---


Yes, exactly.

1.  **Pipe Usage:** The stdio transport mechanism fundamentally relies on **anonymous pipes** provided by the operating system. When the client process starts the server process, it instructs the OS to connect the client's output stream to the server's standard input (`stdin`) via a pipe, and connect the server's standard output (`stdout`) to the client's input stream via another pipe.

2.  **Platform OS Compatibility:** Yes, this mechanism works across all major operating systems (Windows, macOS, Linux). Pipes for inter-process communication (IPC) using standard streams (`stdin`/`stdout`) are a standard feature supported by POSIX-compliant systems (Linux, macOS) and Windows. The underlying libraries used by FastMCP (like `anyio` and Python's built-in `subprocess` module) abstract away the OS-specific differences in creating and managing these pipes and processes.

3.  **How to Implement Your Own (Simplified Example):**

    You can implement a basic version of this using standard Python libraries like `subprocess` and `sys`. Here's a conceptual example:

    **a) Server Script (`my_simple_server.py`)**

    This script reads commands (one per line) from its `stdin` and writes responses to its `stdout`.

    ```python
    # my_simple_server.py
    import sys
    import json
    import time

    def process_command(command_data):
        """Simulates processing a command."""
        if command_data.get("action") == "echo":
            return {"status": "ok", "original": command_data, "echo_message": command_data.get("payload", "")}
        elif command_data.get("action") == "add":
            try:
                result = float(command_data.get("a", 0)) + float(command_data.get("b", 0))
                return {"status": "ok", "result": result}
            except (TypeError, ValueError):
                 return {"status": "error", "message": "Invalid numbers for addition"}
        else:
            return {"status": "error", "message": "Unknown action"}

    if __name__ == "__main__":
        # Basic logging to stderr to avoid interfering with stdout communication
        print("Simple Server: Starting and listening on stdin...", file=sys.stderr)

        try:
            # Continuously read lines from standard input
            for line in sys.stdin:
                line = line.strip() # Remove leading/trailing whitespace (like newline)
                print(f"Simple Server: Received line: '{line}'", file=sys.stderr)

                if not line: # Skip empty lines
                    continue

                try:
                    # Assume each line is a JSON command
                    command = json.loads(line)
                    response = process_command(command)
                except json.JSONDecodeError:
                    response = {"status": "error", "message": "Invalid JSON received"}
                except Exception as e:
                     response = {"status": "error", "message": f"Internal server error: {e}"}

                # Convert response back to JSON
                response_json = json.dumps(response)

                # Write the JSON response to standard output, followed by a newline
                print(response_json, file=sys.stdout)

                # --- CRITICAL ---
                # Flush stdout to ensure the client receives the message immediately,
                # especially when stdout is connected to a pipe.
                sys.stdout.flush()

                print(f"Simple Server: Sent response: {response_json}", file=sys.stderr)

        except EOFError:
             print("Simple Server: Client closed stdin (EOF). Exiting.", file=sys.stderr)
        except KeyboardInterrupt:
             print("Simple Server: Interrupted. Exiting.", file=sys.stderr)
        finally:
             print("Simple Server: Shutting down.", file=sys.stderr)

    ```

    **b) Client Script (`my_simple_client.py`)**

    This script starts the server, sends it commands via its `stdin` pipe, and reads responses from its `stdout` pipe.

    ```python
    # my_simple_client.py
    import subprocess
    import sys
    import json
    import time

    # --- Configuration ---
    # Use sys.executable to ensure we use the same Python interpreter
    # that's running the client to run the server.
    SERVER_COMMAND = [sys.executable, "my_simple_server.py"]

    if __name__ == "__main__":
        print("Simple Client: Starting server process...")

        # Start the server process
        # - stdin=subprocess.PIPE: Makes server's stdin available as process.stdin
        # - stdout=subprocess.PIPE: Makes server's stdout available as process.stdout
        # - stderr=subprocess.PIPE: Capture server's error output (optional)
        # - text=True: Work with strings directly (handles encoding/decoding)
        # - bufsize=1: Use line buffering (often redundant with text=True and flushing)
        process = subprocess.Popen(
            SERVER_COMMAND,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE, # Or subprocess.DEVNULL to ignore, or None to inherit
            text=True, # Use text mode (str) instead of bytes mode
            encoding='utf-8', # Explicitly set encoding
            bufsize=1 # Force line buffering (good practice with pipes)
        )

        print(f"Simple Client: Server process started (PID: {process.pid})")

        # Give the server a moment to start (optional, for demonstration)
        time.sleep(0.5)

        # Commands to send (as Python dicts)
        commands = [
            {"action": "echo", "payload": "Hello Server!"},
            {"action": "add", "a": 5, "b": 10},
            {"action": "unknown"},
            {"action": "add", "a": "foo", "b": 5}, # Intentionally bad command
        ]

        try:
            for cmd in commands:
                # Convert command to JSON string
                cmd_json = json.dumps(cmd)
                print(f"Simple Client: Sending command: {cmd_json}")

                # Write the command to the server's stdin, followed by a newline
                process.stdin.write(cmd_json + '\\n')

                # --- CRITICAL ---
                # Flush the client's end of the pipe to ensure data is sent
                process.stdin.flush()

                # Read the response line from the server's stdout
                response_line = process.stdout.readline()
                if not response_line:
                    print("Simple Client: Server closed stdout unexpectedly. Exiting loop.", file=sys.stderr)
                    break # Exit loop if server closes the pipe

                response_line = response_line.strip()
                print(f"Simple Client: Received response: {response_line}")

                try:
                    # Try to parse the response as JSON
                    response_data = json.loads(response_line)
                    # Process the response data...
                    if response_data.get("status") == "error":
                         print(f"  -> Server reported error: {response_data.get('message')}", file=sys.stderr)

                except json.JSONDecodeError:
                     print(f"  -> Received non-JSON response: {response_line}", file=sys.stderr)

                # Small delay between commands for demonstration
                time.sleep(0.2)

        finally:
            print("Simple Client: Closing server's stdin...")
            # Close the server's stdin pipe to signal no more input
            if process.stdin:
                process.stdin.close()

            print("Simple Client: Waiting for server process to terminate...")
            # Wait for the server process to exit and get return code
            return_code = process.wait(timeout=5) # Add a timeout
            print(f"Simple Client: Server process terminated with code: {return_code}")

            # Read any remaining stderr output from the server (optional)
            if process.stderr:
                 remaining_stderr = process.stderr.read()
                 if remaining_stderr:
                      print("--- Server Stderr Output ---", file=sys.stderr)
                      print(remaining_stderr, file=sys.stderr)
                      print("----------------------------", file=sys.stderr)
                 process.stderr.close()

            # Ensure stdout is also closed (often done by process.wait())
            if process.stdout:
                 process.stdout.close()

        print("Simple Client: Finished.")

    ```

    **To run this:**
    1.  Save the two code blocks as `my_simple_server.py` and `my_simple_client.py` in the same directory.
    2.  Open your terminal or command prompt in that directory.
    3.  Run the client: `python my_simple_client.py`

    The client will start the server, send commands, print the responses it receives, and then shut down the server. You'll see output from both the client and (on stderr) the server.

    **Key Takeaways for Implementation:**
    *   Use `subprocess.Popen` with `stdin=PIPE`, `stdout=PIPE`.
    *   Use `text=True` or manage encoding/decoding manually.
    *   **Flush streams** (`sys.stdout.flush()` in the server, `process.stdin.flush()` in the client) after writing to ensure data crosses the pipe boundary promptly. This is the most common source of errors when pipes seem "stuck".
    *   Define a clear message format (like JSON per line) for communication.
    *   Handle process termination gracefully (close stdin, wait for process).
    *   Read output line-by-line (`readline`) in the client if the server writes line-based responses.
    *   Redirect or capture the server's `stderr` so it doesn't interfere with `stdout` communication.
