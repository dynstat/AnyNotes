# Notes (by gemini 2.5 pro) : MCP agents file related mcp server and client

let's break down the provided code and the Model Context Protocol (MCP) interactions.
## Project Overview

This project demonstrates a simple agent system using the Model Context Protocol (MCP). It consists of two main parts:

1.  **`file_agent_server.py`**: A backend server that exposes file system operations (reading, writing, listing directories) as "tools" via MCP.
2.  **`file_agent_client.py`**: A client application that connects to the server, interacts with the Google Gemini API, and can invoke the server's tools either directly through user commands or potentially via the Gemini model (though the current client focuses on direct user commands).

The communication between the client and server happens over standard input/output (stdio) using JSON-RPC messages formatted according to the MCP specification.

---

## 1. `file_agent_server.py` Explained

This script acts as the "tool provider". It runs as a separate process and listens for requests from an MCP client.

```python
# file_agent_server.py
from mcp.server.fastmcp import FastMCP # Import the simplified MCP server class
import os # Import os for file system operations

# Create a FastMCP server instance, giving it a name that clients can use to identify it.
file_agent = FastMCP("File Operations Agent")

# Decorator to expose the 'read_file' function as an MCP tool.
@file_agent.tool()
def read_file(file_path: str) -> str:
    """Read the contents of a file."""
    try:
        # Open the specified file in read mode ('r') with UTF-8 encoding.
        with open(file_path, 'r', encoding='utf-8') as f:
            # Read the entire content and return it.
            return f.read()
    except Exception as e:
        # If any error occurs (e.g., file not found, permission denied),
        # return an error message string.
        return f"Error reading file: {str(e)}"

# Decorator to expose 'write_file' as an MCP tool.
@file_agent.tool()
def write_file(file_path: str, content: str) -> str:
    """Write content to a file."""
    try:
        # Ensure the directory for the target file exists.
        # os.path.abspath ensures we handle relative paths correctly.
        # os.path.dirname gets the directory part of the path.
        # exist_ok=True prevents an error if the directory already exists.
        os.makedirs(os.path.dirname(os.path.abspath(file_path)), exist_ok=True)

        # Open the file in write mode ('w') with UTF-8 encoding.
        # This will create the file if it doesn't exist, or overwrite it if it does.
        with open(file_path, 'w', encoding='utf-8') as f:
            # Write the provided content to the file.
            f.write(content)
        # Return a success message.
        return f"Successfully wrote to {file_path}"
    except Exception as e:
        # Return an error message if writing fails.
        return f"Error writing to file: {str(e)}"

# Decorator to expose 'list_directory' as an MCP tool.
@file_agent.tool()
def list_directory(directory_path: str) -> str:
    """List files in a directory."""
    try:
        # Get the absolute path of the requested directory.
        abs_path = os.path.abspath(directory_path)
        # Get the current working directory of the *server* process.
        current_path = os.getcwd()
        # List all files and directories within the specified path.
        files = os.listdir(directory_path)
        # Return a formatted string including the server's cwd, the target absolute path,
        # and the list of files/directories found.
        return f"Current path: {current_path}\nCurrent directory: {abs_path}\n\nFiles:\n" + "\n".join(files)
    except Exception as e:
        # Return an error message if listing fails (e.g., path doesn't exist).
        return f"Error listing directory: {str(e)}"

# Standard Python entry point.
if __name__ == "__main__":
    # Start the FastMCP server. This will typically start listening
    # for MCP messages on stdin and sending responses to stdout.
    file_agent.run()
```

**Key Concepts:**

*   **`FastMCP(name)`**: Initializes the server. The `name` ("File Operations Agent") is important for discovery, especially when using configuration files like `mcp.json`.
*   **`@file_agent.tool()`**: This decorator automatically registers the decorated function with the `FastMCP` instance. It inspects the function signature (parameter names and type hints) to generate the necessary schema (`inputSchema` in MCP terms) that tells clients what arguments the tool expects.
*   **Tool Functions (`read_file`, `write_file`, `list_directory`)**:
    *   They take simple parameters (strings).
    *   They perform standard Python file/OS operations.
    *   They return strings (either the result or an error message). MCP handles serializing these into the appropriate response format.
    *   **Parameters:**
        *   `file_path` (for `read_file`, `write_file`): The path to the file, which can be relative (to the server's working directory) or absolute.
        *   `content` (for `write_file`): The string data to write into the file.
        *   `directory_path` (for `list_directory`): The path to the directory to list, relative or absolute.
*   **`file_agent.run()`**: This function starts the server's main loop, handling incoming MCP requests over stdio and dispatching them to the appropriate tool functions.

---

## 2. `mcp.json` Explained

This file is typically used by an MCP *host* environment (like the Cursor editor) to know how to launch MCP servers.

```json
// path=c:\Users\xspc\.cursor\mcp.json
{
  "mcpServers": { // Top-level object containing server definitions
    "File Operations Agent": { // Key: The unique name of the server (must match FastMCP name)
      "command": "python", // The executable to run to start the server
      "args": ["D:/proj/agents-mcp-sample/file_agent_server.py"], // Arguments passed to the command
      "env": {} // Optional environment variables to set for the server process
    }
  }
}
```

**Purpose:**

*   When the host environment (e.g., Cursor) needs the capabilities provided by the "File Operations Agent", it looks up this configuration.
*   It uses the `command` (`python`) and `args` (`D:/proj/agents-mcp-sample/file_agent_server.py`) to spawn the server process.
*   The host then establishes an MCP connection with the spawned process, usually via its standard input and output streams.

---

## 3. `file_agent_client.py` Explained

This script acts as the client, connecting to the server, potentially interacting with an LLM, and invoking the server's tools.

```python
# file_agent_client.py
import asyncio # For asynchronous operations
from mcp import ClientSession, StdioServerParameters, types as mcp_types # MCP client components and types
from mcp.client.stdio import stdio_client # MCP transport for stdio
from google import genai # Google Generative AI SDK
from google.genai import types as genai_types # Gemini types
import os # For accessing environment variables
from dotenv import load_dotenv # To load .env files

# Load environment variables (like API keys) from a .env file in the same directory
load_dotenv()

# Initialize the Google GenAI Client using the API key from the environment
# Requires a GEMINI_API_KEY environment variable to be set (e.g., in the .env file)
client = genai.Client(api_key=os.getenv("GEMINI_API_KEY"))

# Define how to launch the server process
server_params = StdioServerParameters(
    command="python", # The command to run
    args=["file_agent_server.py"], # The script to run as an argument
    # Note: Assumes file_agent_server.py is in the current working directory or Python path
)

# --- Agent Callback (Gemini Interaction Logic) ---
# This function is designed to be called when the MCP session needs an LLM response.
# In this specific script's flow (direct tool calls), it's defined but not directly triggered by the main loop.
# It would typically be invoked by the MCP host/framework if the agent needed to generate a response.
async def agent_callback(
    request: mcp_types.CreateMessageRequestParams, # Input: MCP request containing conversation history, etc.
) -> mcp_types.CreateMessageResult: # Output: MCP result containing the assistant's response
    # Convert MCP message history to the format Gemini SDK expects
    conversation = []
    for msg in request.messages:
        if isinstance(msg.content, mcp_types.TextContent):
            role = "user" if msg.role == "user" else "model" # Map roles
            # Append message content in Gemini's format
            conversation.append(genai_types.Content(role=role, parts=[genai_types.Part.from_text(msg.content.text)]))

    # Define instructions for the Gemini model
    system_prompt = "You are a helpful file assistant. You can read files, write to files, and list directories."

    # Specify the Gemini model to use
    model_name = 'gemini-1.5-pro-latest'

    try:
        # Call the Gemini API asynchronously
        response = await client.chats.send_message_async(
            model=model_name,
            contents=conversation, # Pass the formatted history
            config=genai_types.GenerateContentConfig( # Pass configuration
                system_instruction=system_prompt,
                # temperature=0.7, # Optional sampling parameter
            )
        )

        # Extract the text response from the Gemini result object
        response_text = response.text

        # Format the response into the MCP CreateMessageResult structure
        return mcp_types.CreateMessageResult(
            role="assistant", # Role is 'assistant' for model responses
            content=mcp_types.TextContent(
                type="text",
                text=response_text,
            ),
            model=model_name, # Report the model used
            stopReason="endTurn", # Indicate the reason generation stopped
        )
    except Exception as e:
        # Handle errors during the Gemini API call
        print(f"Error in agent_callback calling Gemini API: {e}") # Log error
        # Return an error message within the MCP structure
        return mcp_types.CreateMessageResult(
            role="assistant",
            content=mcp_types.TextContent(
                type="text",
                text=f"Error calling Gemini API: {str(e)}",
            ),
            stopReason="error", # Indicate an error occurred
        )

# --- Main Client Execution Logic ---
async def run_client():
    try: # Outer try block for stdio_client connection errors
        # Establish connection to the server process via stdio
        # 'stdio_client' starts the server using 'server_params' and provides
        # async 'read' and 'write' streams for communication.
        async with stdio_client(server_params) as (read, write):
            print("stdio_client connected.")
            try: # Inner try block for ClientSession errors
                # Create an MCP client session using the read/write streams.
                # 'agent_callback' is passed as the sampling_callback, which would handle
                # 'sampling/createMessage' requests if the server sent them (not typical for this setup).
                async with ClientSession(
                    read, write, sampling_callback=agent_callback
                ) as session:
                    print("ClientSession created.")

                    # --- MCP Initialization and Tool Discovery ---
                    print("Initializing session...")
                    await session.initialize() # Perform MCP handshake with the server
                    print("Session initialized.")

                    print("Listing tools...")
                    # Request the list of tools from the connected server
                    list_tools_result = await session.list_tools()
                    print("Available tools:")
                    if list_tools_result and list_tools_result.tools:
                        # Iterate through the Tool objects in the result
                        for tool in list_tools_result.tools:
                            print(f"- {tool.name}: {tool.description}") # Print tool name and description
                    else:
                        print("No tools found or failed to retrieve tools.")

                    # --- Example Direct Tool Calls ---
                    print("Calling list_directory tool...")
                    # Call the 'list_directory' tool on the server with arguments
                    call_list_result = await session.call_tool("list_directory", {"directory_path": "."})
                    print(f"list_directory result received.")
                    # Extract the text content from the result
                    list_output = "Error retrieving directory listing."
                    if call_list_result and call_list_result.content and isinstance(call_list_result.content[0], mcp_types.TextContent):
                         list_output = call_list_result.content[0].text
                    print(f"\nFiles in current directory:\n{list_output}")

                    print("Calling read_file tool...")
                    file_to_read = "file_agent_server.py" # File to read (relative path)
                    # Call the 'read_file' tool
                    call_read_result = await session.call_tool("read_file", {"file_path": file_to_read})
                    print(f"read_file result received.")
                    # Extract the text content
                    read_output = "Error reading file content."
                    if call_read_result and call_read_result.content and isinstance(call_read_result.content[0], mcp_types.TextContent):
                         read_output = call_read_result.content[0].text
                    # Print the first 200 characters of the file content
                    print(f"\nContents of {file_to_read}:\n{read_output[:200]}...")

                    # --- Interactive User Command Loop ---
                    print("\nEnter messages (or type 'exit' to quit):")
                    while True:
                        user_input = input("> ") # Get input from the user
                        if user_input.lower() == 'exit':
                            break # Exit the loop if user types 'exit'

                        try: # Try block for errors during tool calls within the loop
                            result_text = "Error executing command." # Default error message
                            tool_name = None
                            tool_params = None

                            # Parse user input to determine the tool and parameters
                            if user_input.startswith("read "):
                                tool_name = "read_file"
                                tool_params = {"file_path": user_input[5:].strip()}
                            elif user_input.startswith("write "):
                                # Split "write file content" into ["file", "content"]
                                parts = user_input[6:].split(" ", 1)
                                if len(parts) == 2:
                                    tool_name = "write_file"
                                    tool_params = {"file_path": parts[0], "content": parts[1]}
                                else:
                                     print("Invalid write command format. Use: write <file> <content>")
                            elif user_input.startswith("list "):
                                 tool_name = "list_directory"
                                 tool_params = {"directory_path": user_input[5:].strip()}
                            else:
                                print("Unknown command. Use 'read <file>', 'write <file> <content>', or 'list <directory>'")
                                continue # Go to the next loop iteration

                            # If a valid tool command was parsed, call the tool
                            if tool_name and tool_params:
                                print(f"Calling tool '{tool_name}' with params: {tool_params}")
                                call_result = await session.call_tool(tool_name, tool_params)
                                # Extract the text result from the server's response
                                if call_result and call_result.content and isinstance(call_result.content[0], mcp_types.TextContent):
                                    result_text = call_result.content[0].text
                                else:
                                     result_text = f"Tool '{tool_name}' executed, but no text content received."
                                     # You might want to inspect call_result further here if needed
                                print(result_text) # Print the result

                        except Exception as loop_err:
                             print(f"Error during interactive command: {loop_err}")
                             # import traceback # Uncomment for detailed debugging
                             # traceback.print_exc()

            except Exception as session_err:
                # Handle errors during session creation or initial calls
                print(f"\n!!! Error during ClientSession operations: {session_err} !!!")
                import traceback
                traceback.print_exc()

    except Exception as outer_err:
        # Handle errors during the initial stdio client connection setup
        print(f"\n!!! Error during stdio_client setup or connection: {outer_err} !!!")
        import traceback
        traceback.print_exc()


if __name__ == "__main__":
    try:
        # Run the main asynchronous client function
        asyncio.run(run_client())
    except Exception as e:
        # Catch any final unhandled errors
        print(f"\n!!! Unhandled top-level error occurred: {e} !!!")
        # import traceback
        # traceback.print_exc()
```

**Key Concepts:**

*   **`asyncio`**: Used extensively because MCP communication and API calls are I/O-bound and handled asynchronously.
*   **`load_dotenv()`, `genai.Client()`**: Standard setup for using the Google Gemini API.
*   **`StdioServerParameters`**: Tells the client *how* to start the server process it wants to connect to.
*   **`stdio_client(server_params)`**: This is the MCP *transport* layer for stdio. It starts the server process defined in `server_params` and yields a pair of asynchronous streams (`read`, `write`) connected to the server's stdout and stdin, respectively.
*   **`ClientSession(read, write, sampling_callback)`**: This manages the MCP protocol logic over the transport streams.
    *   `read`, `write`: The streams obtained from `stdio_client`.
    *   `sampling_callback`: A function (`agent_callback` here) that the session *would* call if it received a `sampling/createMessage` request from the server. In this client-driven example, this callback isn't strictly necessary for the interactive loop but is included perhaps for future agent integration.
*   **`session.initialize()`**: Sends the `initialize` request to the server and processes the response, establishing the MCP connection parameters (protocol version, capabilities).
*   **`session.list_tools()`**: Sends a `tools/list` request to the server. The server (our `file_agent_server.py`) responds with a list of its registered tools (`read_file`, `write_file`, `list_directory`) including their names, descriptions, and input schemas (derived from the Python function signatures). The result is an `mcp_types.ListToolsResult` object.
*   **`session.call_tool(tool_name, arguments)`**: Sends a `tools/call` request to the server.
    *   `tool_name`: The name of the tool to call (e.g., "read\_file").
    *   `arguments`: A dictionary where keys are parameter names expected by the tool (e.g., "file\_path") and values are the arguments.
    *   The server executes the corresponding Python function (e.g., `read_file(file_path=...)`) and sends back the return value. The client receives this as an `mcp_types.CallToolResult` object. The actual content is usually within the `content` attribute, often as a `TextContent` object.
*   **Interactive Loop**: Provides a simple command-line interface to directly test the tool calls through the MCP connection. It parses user input and maps it to `session.call_tool` calls.
*   **Error Handling**: Uses `try...except` blocks to catch potential issues at different stages (connection, session management, individual tool calls).

---

## 4. Overall Flow and MCP Interaction

1.  **Server Startup**: When needed (either manually or by a host like Cursor using `mcp.json`), `python file_agent_server.py` is executed. The `FastMCP` server starts and listens on its stdin for MCP messages.
2.  **Client Startup**: The user runs `python file_agent_client.py`.
3.  **Connection via Stdio**:
    *   The client's `stdio_client` function executes the `server_params` command (`python file_agent_server.py`), launching the server process.
    *   It establishes pipes to the server process's stdin and stdout.
    *   It yields `read` and `write` async streams connected to these pipes.
4.  **MCP Handshake**:
    *   The `ClientSession` is created using the `read`/`write` streams.
    *   `session.initialize()` sends an `initialize` request over the `write` stream.
    *   The server receives this on its stdin, processes it using its `FastMCP` logic, and sends an `initialize` result back over its stdout.
    *   The client receives this result on its `read` stream, completing the handshake.
5.  **Tool Discovery**:
    *   The client calls `session.list_tools()`, sending a `tools/list` request.
    *   The server receives this, gathers details about its `@file_agent.tool()` decorated functions, and sends back a `tools/list` result containing the tool definitions.
    *   The client receives and prints this information.
6.  **Tool Execution (Direct Call / Interactive Loop)**:
    *   The client (either in the initial examples or via user input) calls `session.call_tool("tool_name", {...args...})`.
    *   This sends a `tools/call` request containing the tool name and arguments as JSON.
    *   The server receives this request, finds the matching Python function (`read_file`, `write_file`, or `list_directory`), and calls it with the provided arguments.
    *   The Python function executes (e.g., reads a file).
    *   The server takes the return value (e.g., the file content string) and packages it into a `tools/call` result message.
    *   The server sends this result message back to the client via its stdout.
    *   The client receives the result message on its `read` stream, parses the `CallToolResult`, extracts the content, and prints it.
7.  **Agent Interaction (Conceptual / `agent_callback`)**:
    *   In a more integrated scenario (like Cursor), the *host* would manage the LLM chat.
    *   If the LLM response indicated a tool call was needed (e.g., Gemini asks to read 'example.txt'), the *host* would use *its* MCP client session connected to the `file_agent_server.py` to invoke `session.call_tool("read_file", {"file_path": "example.txt"})`.
    *   The server would execute the tool and return the result to the host.
    *   The host would then format this result and feed it back into the LLM conversation.
    *   The `agent_callback` in *this client code* is set up to *respond* to `sampling/createMessage` requests (which typically come *from* the server/agent *to* the host/client that controls the LLM), making its placement slightly unusual for a script primarily focused on *making* tool calls. It demonstrates *how* to interact with Gemini *if* this client were acting as the LLM-calling part of an MCP system.
8.  **Termination**: When the client exits (user types 'exit' or an error occurs), the `async with` blocks clean up the session and the `stdio_client` likely terminates the server process.

---

## 5. Key MCP Functions/Parameters Summary

*   **`mcp.server.FastMCP(name: str)`**:
    *   `name`: Identifier for the server.
    *   `.tool()`: Decorator to register functions as callable tools.
    *   `.run()`: Starts the server loop (usually listening on stdio).
*   **`mcp.StdioServerParameters(command: str, args: list[str], env: dict | None = None)`**:
    *   `command`: Executable to run the server (e.g., "python").
    *   `args`: List of arguments for the command (e.g., ["my_server_script.py"]).
    *   `env`: Optional environment variables for the server process.
*   **`mcp.client.stdio.stdio_client(server_params: StdioServerParameters)`**:
    *   Context manager that starts the server defined by `server_params`.
    *   Yields `(read, write)`: Asynchronous streams for sending/receiving `JSONRPCMessage` objects to/from the server's stdio.
*   **`mcp.ClientSession(read, write, sampling_callback=None, ...)`**:
    *   `read`, `write`: Communication streams from a transport like `stdio_client`.
    *   `sampling_callback`: An `async` function (`async def callback(request: mcp.types.CreateMessageRequestParams) -> mcp.types.CreateMessageResult`) that the session calls if it receives a `sampling/createMessage` request. Used when the connected MCP entity needs this client to interact with an LLM.
    *   `initialize()`: Async method to perform the MCP handshake. Must be called after creating the session.
    *   `list_tools()`: Async method. Sends `tools/list` request. Returns `mcp.types.ListToolsResult`.
    *   `call_tool(tool_name: str, arguments: dict)`: Async method. Sends `tools/call` request. Returns `mcp.types.CallToolResult`.
*   **MCP Types (`mcp.types.*`)**:
    *   `Role`: "user" or "assistant".
    *   `TextContent`: Represents text data (`{ "type": "text", "text": "..." }`).
    *   `SamplingMessage`: A message in a conversation history (`{ "role": ..., "content": ... }`).
    *   `CreateMessageRequestParams`: Parameters for requesting an LLM completion (includes `messages`, `systemPrompt`, `modelPreferences`, etc.). Sent *to* the entity controlling the LLM.
    *   `CreateMessageResult`: The response from an LLM completion request (includes `role`, `content`, `model`, `stopReason`). Sent *from* the entity controlling the LLM.
    *   `Tool`: Represents a tool definition (`{ "name": ..., "description": ..., "inputSchema": ... }`). Found in `ListToolsResult.tools`.
    *   `CallToolResult`: The result of a tool call (`{ "content": [...], "isError": ... }`). The `content` is a list, often containing a single `TextContent`, `ImageContent`, or `EmbeddedResource`.
    *   `ListToolsResult`: Contains the list of available tools (`{ "tools": [...] }`).

This setup provides a robust way to separate concerns: the file operations logic resides entirely in the server, while the client handles user interaction, LLM communication (via `agent_callback`), and orchestrating calls to the server's tools using the standardized MCP protocol.

---



# Notes (by claude): File Operations Agent with MCP (Multi-Modal Chat Protocol)

## Complete Code

### file_agent_client.py
```python
# file_agent_client.py
import asyncio
from mcp import ClientSession, StdioServerParameters, types as mcp_types
from mcp.client.stdio import stdio_client
from google import genai
from google.genai import types as genai_types
import os
from dotenv import load_dotenv

load_dotenv()
client = genai.Client(api_key=os.getenv("GEMINI_API_KEY"))

server_params = StdioServerParameters(
    command="python",
    args=["file_agent_server.py"],
)

async def agent_callback(
    request: mcp_types.CreateMessageRequestParams,
) -> mcp_types.CreateMessageResult:
    conversation = []
    for msg in request.messages:
        if isinstance(msg.content, mcp_types.TextContent):
            role = "user" if msg.role == "user" else "model"
            conversation.append(genai_types.Content(role=role, parts=[genai_types.Part.from_text(msg.content.text)]))
    
    system_prompt = "You are a helpful file assistant. You can read files, write to files, and list directories."
    model_name = 'gemini-1.5-pro-latest'

    try:
        response = await client.chats.send_message_async(
            model=model_name,
            contents=conversation,
            config=genai_types.GenerateContentConfig(
                system_instruction=system_prompt,
            )
        )
        
        response_text = response.text
        
        return mcp_types.CreateMessageResult(
            role="assistant",
            content=mcp_types.TextContent(
                type="text",
                text=response_text,
            ),
            model=model_name,
            stopReason="endTurn",
        )
    except Exception as e:
        return mcp_types.CreateMessageResult(
            role="assistant",
            content=mcp_types.TextContent(
                type="text",
                text=f"Error calling Gemini API: {str(e)}",
            ),
            stopReason="error",
        )

async def run_client():
    try:
        async with stdio_client(server_params) as (read, write):
            print("stdio_client connected.")
            try:
                async with ClientSession(
                    read, write, sampling_callback=agent_callback
                ) as session:
                    print("ClientSession created.")
                    print("Initializing session...")
                    await session.initialize()
                    print("Session initialized.")
                    
                    print("Listing tools...")
                    list_tools_result = await session.list_tools()
                    
                    print("Available tools:")
                    if list_tools_result and list_tools_result.tools:
                        for tool in list_tools_result.tools:
                            print(f"- {tool.name}: {tool.description}")
                    else:
                        print("No tools found or failed to retrieve tools.")

                    print("Calling list_directory tool...")
                    call_list_result = await session.call_tool("list_directory", {"directory_path": "."})
                    print(f"list_directory result received.")
                    list_output = "Error retrieving directory listing."
                    if call_list_result and call_list_result.content and isinstance(call_list_result.content[0], mcp_types.TextContent):
                         list_output = call_list_result.content[0].text
                    print(f"\nFiles in current directory:\n{list_output}")
                    
                    print("Calling read_file tool...")
                    file_to_read = "file_agent_server.py"
                    call_read_result = await session.call_tool("read_file", {"file_path": file_to_read})
                    print(f"read_file result received.")
                    read_output = "Error reading file content."
                    if call_read_result and call_read_result.content and isinstance(call_read_result.content[0], mcp_types.TextContent):
                         read_output = call_read_result.content[0].text
                    print(f"\nContents of {file_to_read}:\n{read_output[:200]}...")
                    
                    print("\nEnter messages (or type 'exit' to quit):")
                    while True:
                        user_input = input("> ")
                        if user_input.lower() == 'exit':
                            break
                        
                        try:
                            result_text = "Error executing command."
                            tool_name = None
                            tool_params = None

                            if user_input.startswith("read "):
                                tool_name = "read_file"
                                tool_params = {"file_path": user_input[5:].strip()}
                            elif user_input.startswith("write "):
                                parts = user_input[6:].split(" ", 1)
                                if len(parts) == 2:
                                    tool_name = "write_file"
                                    tool_params = {"file_path": parts[0], "content": parts[1]}
                                else:
                                     print("Invalid write command format.")
                            elif user_input.startswith("list "):
                                 tool_name = "list_directory"
                                 tool_params = {"directory_path": user_input[5:].strip()}
                            else:
                                print("Unknown command. Use 'read <file>', 'write <file> <content>', or 'list <directory>'")
                                continue

                            if tool_name and tool_params:
                                call_result = await session.call_tool(tool_name, tool_params)
                                if call_result and call_result.content and isinstance(call_result.content[0], mcp_types.TextContent):
                                    result_text = call_result.content[0].text
                                print(result_text)

                        except Exception as loop_err:
                             print(f"Error during interactive command: {loop_err}")

            except Exception as session_err:
                print(f"\n!!! Error during ClientSession operations: {session_err} !!!")
                import traceback
                traceback.print_exc()

    except Exception as outer_err:
        print(f"\n!!! Error during stdio_client setup or connection: {outer_err} !!!")
        import traceback
        traceback.print_exc()


if __name__ == "__main__":
    try:
        asyncio.run(run_client())
    except Exception as e:
        print(f"\n!!! Unhandled top-level error occurred: {e} !!!")
```

### file_agent_server.py
```python
# file_agent_server.py
from mcp.server.fastmcp import FastMCP
import os

# Create a FastMCP server
file_agent = FastMCP("File Operations Agent")

@file_agent.tool()
def read_file(file_path: str) -> str:
    """Read the contents of a file."""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            return f.read()
    except Exception as e:
        return f"Error reading file: {str(e)}"

@file_agent.tool()
def write_file(file_path: str, content: str) -> str:
    """Write content to a file."""
    try:
        # Ensure the directory exists
        os.makedirs(os.path.dirname(os.path.abspath(file_path)), exist_ok=True)
        
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(content)
        return f"Successfully wrote to {file_path}"
    except Exception as e:
        return f"Error writing to file: {str(e)}"

@file_agent.tool()
def list_directory(directory_path: str) -> str:
    """List files in a directory."""
    try:
        # Get absolute path of the directory
        abs_path = os.path.abspath(directory_path)
        current_path = os.getcwd()
        files = os.listdir(directory_path)
        return f"Current path: {current_path}\nCurrent directory: {abs_path}\n\nFiles:\n" + "\n".join(files)
    except Exception as e:
        return f"Error listing directory: {str(e)}"

if __name__ == "__main__":
    file_agent.run()
```

### MCP Configuration (mcp.json)
```json
{
  "mcpServers": {
    "File Operations Agent": {
      "command": "python",
      "args": ["D:/proj/agents-mcp-sample/file_agent_server.py"],
      "env": {}
    }
  }
}
```

## System Overview

This project implements a file operations agent using the Model context Protocol (MCP) architecture. It consists of two main components:

1. **Server (file_agent_server.py)**: Provides file operation tools (read, write, list)
2. **Client (file_agent_client.py)**: Connects to the server and enables interaction with Gemini AI through tool calls

The system allows AI models (specifically Google's Gemini) to perform file system operations using tool functions. It demonstrates how to build AI assistants that can interact with the file system in a controlled way.

## Detailed Workflow

1. **Server Initialization**:
   - The `file_agent_server.py` creates a FastMCP server with three tools: read_file, write_file, and list_directory
   - Each tool is registered with the server using the `@file_agent.tool()` decorator

2. **Client Initialization**:
   - `file_agent_client.py` loads environment variables (Gemini API key)
   - Sets up server parameters using StdioServerParameters
   - Initializes the Google GenAI client

3. **Connection Establishment**:
   - Client connects to the server using stdio_client
   - Creates a ClientSession with the agent_callback function

4. **Session Initialization**:
   - Client initializes the session with the server
   - Lists available tools from the server
   - Performs example tool calls (list_directory, read_file)

5. **User Interaction**:
   - Client presents an interactive interface to issue commands
   - Commands are parsed and converted to tool calls
   - Results are displayed to the user

6. **Model Integration**:
   - The `agent_callback` function processes user messages
   - Creates a conversation history for the Gemini model
   - Sends requests to the Gemini API
   - Returns formatted responses

## Component Details

### Server Components (file_agent_server.py)

#### FastMCP Server
- Creates an MCP-compatible server named "File Operations Agent"
- Registers tool functions that can be called by clients

#### Tool: read_file
- **Purpose**: Reads and returns the contents of a specified file
- **Parameters**:
  - `file_path` (str): Path to the file to be read
- **Returns**: Content of the file as a string or error message
- **Error Handling**: Catches exceptions and returns formatted error messages

#### Tool: write_file
- **Purpose**: Writes content to a specified file
- **Parameters**:
  - `file_path` (str): Path to the file to write
  - `content` (str): Text content to write to the file
- **Returns**: Success message or error message
- **Features**: Creates parent directories if they don't exist
- **Error Handling**: Catches exceptions and returns formatted error messages

#### Tool: list_directory
- **Purpose**: Lists files in a specified directory
- **Parameters**:
  - `directory_path` (str): Path to the directory to list
- **Returns**: Formatted string containing the current path, directory path, and list of files
- **Error Handling**: Catches exceptions and returns formatted error messages

### Client Components (file_agent_client.py)

#### Environment Setup
- Uses `dotenv` to load API keys from `.env` file
- Initializes the Google GenAI client with the API key

#### Server Parameters
- Creates `StdioServerParameters` with the command to run the server script
- Parameters:
  - `command` (str): The executable to run (Python)
  - `args` (list): Arguments to pass to the executable (server script path)

#### agent_callback Function
- **Purpose**: Acts as a bridge between MCP and the Gemini API
- **Parameters**:
  - `request` (CreateMessageRequestParams): Contains messages from the user
- **Returns**: CreateMessageResult with the model's response
- **Process**:
  - Converts MCP message format to GenAI format
  - Sends message to Gemini API
  - Converts response back to MCP format
- **Error Handling**: Catches exceptions from API calls and returns error messages

#### run_client Function
- **Purpose**: Main function to run the client and manage its lifecycle
- **Process**:
  - Establishes connection with the server
  - Initializes the session
  - Lists available tools
  - Demonstrates tool calls
  - Provides interactive command line interface
- **Command Parsing**:
  - `read <file>`: Calls read_file tool
  - `write <file> <content>`: Calls write_file tool
  - `list <directory>`: Calls list_directory tool
- **Error Handling**: Multiple levels of exception handling for different scopes

## MCP Configuration (mcp.json)
- Configuration file used by the MCP system
- Specifies server details:
  - Name: "File Operations Agent"
  - Command: "python"
  - Arguments: Path to the server script
  - Environment variables: None specified

## Usage Scenarios

1. **Interactive File Operations**:
   - User types commands to read, write, or list files
   - Example: `read config.txt`, `write log.txt Hello world`, `list ./data`

2. **AI-Powered File Assistant**:
   - Gemini model can perform file operations through tool calling
   - System prompt defines the assistant's capabilities and behavior

3. **Automated File Handling**:
   - Can be extended to perform automated file operations based on AI decisions
   - Potential for intelligent file management, content analysis, etc.

## Extension Possibilities

1. **Additional File Operations**:
   - Delete files/directories
   - Copy/move files
   - File search with pattern matching
   - File metadata retrieval

2. **Enhanced Security**:
   - Path sanitization
   - Access restrictions
   - Permission handling

3. **UI Integration**:
   - Web interface
   - GUI application
   - Integration with messaging platforms


---
---


## Deep Dive: agent_callback Function

### Purpose and Role
The `agent_callback` function is the crucial bridge between the MCP protocol and the Google Gemini AI model. It:
- Converts MCP message format to Gemini API format
- Sends requests to the Gemini API
- Formats responses back to MCP format
- Handles errors during API calls

### Parameter and Return Analysis
- **Input Parameter**: `request: mcp_types.CreateMessageRequestParams`
  - Contains the conversation history
  - Includes message role ("user" or "assistant")
  - Contains message content (text or other content types)

- **Return Value**: `mcp_types.CreateMessageResult`
  - Returns the AI's response in MCP format
  - Includes role, content, model name, and stop reason
  - Structured for proper display in the client

### Conversion Process
1. **MCP to Gemini Format**:
   ```python
   conversation = []
   for msg in request.messages:
       if isinstance(msg.content, mcp_types.TextContent):
           role = "user" if msg.role == "user" else "model"
           conversation.append(genai_types.Content(role=role, parts=[genai_types.Part.from_text(msg.content.text)]))
   ```
   - Extracts text content from MCP messages
   - Maps MCP roles to Gemini roles (user → user, assistant → model)
   - Creates Content objects with appropriate Parts

2. **API Call**:
   ```python
   response = await client.chats.send_message_async(
       model=model_name,
       contents=conversation,
       config=genai_types.GenerateContentConfig(
           system_instruction=system_prompt,
       )
   )
   ```
   - Uses the newer async API for better performance
   - Passes conversation history to maintain context
   - Sets system instruction to guide model behavior
   - Optional parameters available: temperature, top_p, top_k, etc.

3. **Response Formatting**:
   ```python
   return mcp_types.CreateMessageResult(
       role="assistant",
       content=mcp_types.TextContent(
           type="text",
           text=response_text,
       ),
       model=model_name,
       stopReason="endTurn",
   )
   ```
   - Wraps Gemini response in MCP format
   - Sets role to "assistant"
   - Formats content as TextContent
   - Includes model name for tracking
   - Sets stopReason to indicate normal completion

### Error Handling
- Catches all exceptions during API call
- Returns formatted error message in MCP format
- Sets stopReason to "error" to indicate failure
- Prevents crashes when API calls fail

### Configuration Options
Additional parameters that could be added to the Gemini API call:
- `temperature`: Controls randomness (0.0-1.0)
- `top_p`: Nucleus sampling parameter
- `top_k`: Limits token selection to top K options
- `max_output_tokens`: Limits response length
- `safety_settings`: Controls content filtering
- `candidate_count`: Number of alternative responses to generate

## MCP Architecture Deep Dive

### Protocol Overview
Multi-Modal Chat Protocol (MCP) is designed to standardize interactions between:
- Chat clients (interfaces for users)
- AI models (like Gemini)
- Tools (functions that perform actions)

### Key Components
1. **Messages**: Units of communication with roles and content
   - Roles: user, assistant, system, tool
   - Content types: text, image, audio, etc.

2. **Tools**: Functions registered with the server
   - Each tool has a name, description, and parameters
   - Tools can be called by the model or directly by the client

3. **Sessions**: Managed connections between clients and servers
   - Handle initialization, message passing, and tool calling
   - Maintain conversation state

### Communication Flow
1. User sends message to client
2. Client formats message in MCP and sends to server
3. Server processes message (may call AI model via callback)
4. AI model may request tool calls
5. Server executes tool calls and returns results
6. Response flows back to client and is displayed to user

## Debugging and Development

### Common Issues and Solutions
1. **API Key Issues**:
   - Ensure `.env` file contains `GEMINI_API_KEY=your_key_here`
   - Check that `load_dotenv()` is called before accessing key
   - Verify API key is valid and has correct permissions

2. **Connection Issues**:
   - Verify server script is accessible at the specified path
   - Check for firewall or permission issues
   - Ensure Python has access to run the script

3. **Tool Call Failures**:
   - Validate parameter types match tool definitions
   - Check for file permission issues for file operations
   - Verify paths are correctly formatted for the OS

### Extending the System
1. **Adding New Tools**:
   - Define a new function in the server file
   - Add the `@file_agent.tool()` decorator
   - Include docstring for description
   - Type hint parameters and return values

   Example:
   ```python
   @file_agent.tool()
   def search_file(file_path: str, search_term: str) -> str:
       """Search for a term in a file and return matching lines."""
       try:
           results = []
           with open(file_path, 'r', encoding='utf-8') as f:
               for i, line in enumerate(f, 1):
                   if search_term in line:
                       results.append(f"Line {i}: {line.strip()}")
           return "\n".join(results) if results else f"No matches for '{search_term}' in {file_path}"
       except Exception as e:
           return f"Error searching file: {str(e)}"
   ```

2. **Adding UI**:
   - Replace command line input with a GUI framework
   - Use web frameworks like Flask or FastAPI for web interfaces
   - Maintain the same MCP client structure underneath

## Best Practices

### Security Considerations
1. **Path Validation**:
   - Sanitize file paths to prevent directory traversal attacks
   - Restrict operations to specific directories
   - Validate file extensions for sensitive operations

2. **Content Validation**:
   - Validate content before writing to files
   - Limit file sizes to prevent resource exhaustion
   - Scan content for potentially malicious code

### Performance Optimization
1. **Async Operations**:
   - Use async file I/O for large files
   - Process large directories in chunks
   - Implement caching for frequently accessed files

2. **Model Configuration**:
   - Adjust temperature based on task requirements
   - Use lower values for deterministic tasks
   - Use higher values for creative responses

### Production Deployment
1. **Environment Management**:
   - Use separate .env files for development and production
   - Consider using environment variable management systems
   - Implement secret management for API keys

2. **Logging and Monitoring**:
   - Add structured logging to track operations
   - Monitor API usage and rate limits
   - Implement telemetry for error tracking

3. **Error Recovery**:
   - Implement automatic retry mechanisms for transient errors
   - Add graceful degradation when services are unavailable
   - Maintain fallback options for critical functions
