
# Creating Custom File-Interacting Agents: From Basics to IDE Integration

Let's explore how to build agents that can interact with files and operate like the AI assistants in IDEs such as Cursor, VSCode, and Windsurf. I'll provide detailed, practical examples to help you understand both how to create these agents and how they work in actual IDEs.

## Part 1: Understanding Agent Architecture in IDEs

IDE-based AI assistants typically follow this architecture:

1. **Client Component** - The IDE extension/plugin that:
   - Captures context (open files, cursor position, selections)
   - Provides UI for user interaction
   - Sends requests to the agent
   - Applies changes returned by the agent

2. **Agent Component** - The AI-powered backend that:
   - Receives IDE context
   - Uses an LLM to understand requests
   - Performs file operations or generates code
   - Returns structured responses to the client

3. **Communication Protocol** - How the client and agent talk to each other (often using MCP or similar protocols)

## Part 2: Creating Basic File-Interacting Agents

### Example 1: Simple File Operations Agent using MCP Python SDK

This example creates an MCP server that can read and write files:

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
        files = os.listdir(directory_path)
        return "\n".join(files)
    except Exception as e:
        return f"Error listing directory: {str(e)}"

if __name__ == "__main__":
    file_agent.run()
```

### Example 2: Creating the Client to Connect to Our File Agent

```python
# file_agent_client.py
import asyncio
from mcp import ClientSession, StdioServerParameters, types
from mcp.client.stdio import stdio_client
import openai  # You'd use your preferred LLM provider

# Initialize your LLM
openai_client = openai.AsyncOpenAI(api_key="your-api-key")

# Create server parameters
server_params = StdioServerParameters(
    command="python",
    args=["file_agent_server.py"],
)

# Agent logic that uses an external LLM
async def agent_callback(
    request: types.CreateMessageRequestParams,
) -> types.CreateMessageResult:
    # Extract the conversation history
    conversation = []
    for msg in request.messages:
        if isinstance(msg.content, types.TextContent):
            conversation.append({
                "role": msg.role,
                "content": msg.content.text,
            })
        # Handle other content types as needed
    
    # Call the LLM to decide what to do
    try:
        completion = await openai_client.chat.completions.create(
            model="gpt-4",
            messages=[
                {"role": "system", "content": "You are a helpful file assistant. You can read files, write to files, and list directories."},
                *conversation
            ],
            temperature=0.7,
        )
        
        # Get the response text
        response_text = completion.choices[0].message.content
        
        return types.CreateMessageResult(
            role="assistant",
            content=types.TextContent(
                type="text",
                text=response_text,
            ),
            model="gpt-4",
            stopReason="endTurn",
        )
    except Exception as e:
        return types.CreateMessageResult(
            role="assistant",
            content=types.TextContent(
                type="text",
                text=f"Error: {str(e)}",
            ),
            stopReason="error",
        )

async def run_client():
    async with stdio_client(server_params) as (read, write):
        async with ClientSession(
            read, write, sampling_callback=agent_callback
        ) as session:
            # Initialize the connection
            await session.initialize()
            
            # List available tools
            tools = await session.list_tools()
            print("Available tools:")
            for tool in tools:
                print(f"- {tool.name}: {tool.description}")
            
            # Example: Use the tool to list files in the current directory
            result = await session.call_tool("list_directory", {"directory_path": "."})
            print(f"\nFiles in current directory:\n{result}")
            
            # Example: Read a specific file
            file_to_read = "file_agent_server.py"  # This file itself
            result = await session.call_tool("read_file", {"file_path": file_to_read})
            print(f"\nContents of {file_to_read}:\n{result[:200]}...")  # Show first 200 chars
            
            # Interactive loop - real client would integrate with an interface
            print("\nEnter messages (or type 'exit' to quit):")
            while True:
                user_input = input("> ")
                if user_input.lower() == 'exit':
                    break
                
                # Here, the client would trigger the server and eventually 
                # the agent_callback would be called
                # For simplicity, we'll directly call a tool based on input
                if user_input.startswith("read "):
                    file_path = user_input[5:].strip()
                    result = await session.call_tool("read_file", {"file_path": file_path})
                    print(result)
                elif user_input.startswith("write "):
                    parts = user_input[6:].split(" ", 1)
                    if len(parts) == 2:
                        file_path, content = parts
                        result = await session.call_tool("write_file", {
                            "file_path": file_path, 
                            "content": content
                        })
                        print(result)
                elif user_input.startswith("list "):
                    dir_path = user_input[5:].strip()
                    result = await session.call_tool("list_directory", {"directory_path": dir_path})
                    print(result)
                else:
                    print("Unknown command. Use 'read <file>', 'write <file> <content>', or 'list <directory>'")

if __name__ == "__main__":
    asyncio.run(run_client())
```

## Part 3: Advanced IDE-Like Agent using OpenAI Agents SDK

For more sophisticated IDE functionality, the OpenAI Agents SDK provides higher-level abstractions. Here's an example:

```python
# ide_assistant_agent.py
import os
import re
from agents import Agent, tool, Runner
from pydantic import BaseModel, Field
import asyncio

# Define models for the tool inputs
class ReadFileInput(BaseModel):
    path: str = Field(..., description="Path to the file to read")

class WriteFileInput(BaseModel):
    path: str = Field(..., description="Path to the file to write")
    content: str = Field(..., description="Content to write to the file")

class SearchCodeInput(BaseModel):
    query: str = Field(..., description="Search term to find in code")
    directory: str = Field(default=".", description="Directory to search in")
    file_pattern: str = Field(default="*.py", description="File pattern to match")

class GenerateCodeInput(BaseModel):
    specification: str = Field(..., description="Description of code to generate")
    language: str = Field(default="python", description="Programming language")
    
# Define the tools
@tool(ReadFileInput)
async def read_file(ctx, inputs: ReadFileInput) -> str:
    """Read the contents of a file."""
    try:
        with open(inputs.path, 'r', encoding='utf-8') as f:
            content = f.read()
        return content
    except Exception as e:
        return f"Error reading file: {str(e)}"

@tool(WriteFileInput)
async def write_file(ctx, inputs: WriteFileInput) -> str:
    """Write content to a file."""
    try:
        os.makedirs(os.path.dirname(os.path.abspath(inputs.path)), exist_ok=True)
        with open(inputs.path, 'w', encoding='utf-8') as f:
            f.write(inputs.content)
        return f"Successfully wrote to {inputs.path}"
    except Exception as e:
        return f"Error writing to file: {str(e)}"

@tool(SearchCodeInput)
async def search_code(ctx, inputs: SearchCodeInput) -> str:
    """Search for code that matches a pattern in files."""
    import glob
    
    results = []
    pattern = re.compile(inputs.query, re.IGNORECASE)
    
    for file_path in glob.glob(f"{inputs.directory}/**/{inputs.file_pattern}", recursive=True):
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                for i, line in enumerate(f, 1):
                    if pattern.search(line):
                        results.append(f"{file_path}:{i}: {line.strip()}")
        except Exception:
            # Skip files that can't be read
            pass
    
    if results:
        return "Found matches:\n" + "\n".join(results)
    else:
        return "No matches found."

@tool(GenerateCodeInput)
async def generate_code(ctx, inputs: GenerateCodeInput) -> str:
    """Generate code based on specification."""
    # Note: In a real implementation, this would likely make a separate LLM call
    # with specialized system prompts for code generation
    return f"[Generated {inputs.language} code would be returned here based on: {inputs.specification}]"

# Create the IDE assistant agent
ide_assistant = Agent(
    name="IDE Assistant",
    instructions="""You are an intelligent IDE assistant that helps with coding tasks.
You can read files, write to files, search through code, and generate new code.
When asked to perform a task, use the appropriate tools to accomplish it.
Be precise and helpful, focusing on solving the user's coding problems efficiently.
Explain your actions and provide context where helpful.""",
    tools=[read_file, write_file, search_code, generate_code],
)

# Example usage
async def main():
    # Example 1: Read a Python file
    result1 = await Runner.run(ide_assistant, "Show me the contents of this file")
    print("RESULT 1:", result1.final_output)
    
    # Example 2: Find all print statements in the current directory
    result2 = await Runner.run(ide_assistant, "Find all instances of 'print(' in Python files")
    print("\nRESULT 2:", result2.final_output)
    
    # Example 3: Generate a simple API endpoint
    result3 = await Runner.run(
        ide_assistant, 
        "Create a FastAPI endpoint that returns the current time"
    )
    print("\nRESULT 3:", result3.final_output)
    
    # Example 4: Update a configuration file
    result4 = await Runner.run(
        ide_assistant, 
        "Add a new database configuration to config.py with host='localhost', port=5432, user='admin'"
    )
    print("\nRESULT 4:", result4.final_output)

if __name__ == "__main__":
    asyncio.run(main())
```

To expose this as an MCP server that other applications (like an IDE) could connect to:

```python
# ide_assistant_server.py
import asyncio
from agents import Agent, tool
from agents.mcp import serve
from ide_assistant_agent import ide_assistant  # Import the agent we defined

async def main():
    # Serve the agent as an MCP server
    await serve(ide_assistant, host="127.0.0.1", port=8000)

if __name__ == "__main__":
    asyncio.run(main())
```

## Part 4: How IDE Assistants Actually Work

Let's examine how AI assistants operate in actual IDEs:

### Cursor's Implementation

Cursor integrates AI assistants through these components:

1. **Editor Context Management**:
   - Tracks open files and editor state
   - Captures cursor position, selections, and visible text
   - Monitors project structure and dependencies

2. **Request Handling**:
   ```javascript
   // Simplified conceptual Cursor extension code
   function handleUserQuery(query) {
     // 1. Collect context
     const activeEditor = getActiveEditor();
     const visibleCode = activeEditor.getVisibleText();
     const selectedText = activeEditor.getSelectedText();
     const cursorPosition = activeEditor.getCursorPosition();
     const fileTree = getProjectFileTree();
     
     // 2. Send to AI service
     const response = await aiService.processQuery({
       query,
       context: {
         visibleCode,
         selectedText,
         cursorPosition,
         fileTree,
         recentFiles: getRecentlyOpenedFiles()
       }
     });
     
     // 3. Apply changes if requested
     if (response.changes) {
       activeEditor.applyEdits(response.changes);
     }
     
     // 4. Display response to user
     showResponseInUI(response.message);
   }
   ```

3. **Edit Application**:
   - Parses response from the AI assistant
   - Applies code edits or file operations
   - Handles multi-file operations when needed

### VSCode AI Assistant Implementation

VSCode's GitHub Copilot and similar extensions work similarly:

1. **Extension Registration**:
   ```typescript
   // Conceptual VSCode extension code
   export function activate(context: vscode.ExtensionContext) {
     // Register commands
     const commandHandler = vscode.commands.registerCommand(
       'ai-assistant.query', 
       handleUserQuery
     );
     
     // Add command to context
     context.subscriptions.push(commandHandler);
     
     // Start file watchers and event listeners
     setupFileWatchers(context);
     setupEditorListeners(context);
   }
   ```

2. **Context Collection**:
   ```typescript
   async function collectContext() {
     const activeEditor = vscode.window.activeTextEditor;
     if (!activeEditor) return null;
     
     // Get file content and metadata
     const document = activeEditor.document;
     const fileContent = document.getText();
     const fileName = document.fileName;
     const languageId = document.languageId;
     
     // Get project info
     const workspaceFolder = vscode.workspace.getWorkspaceFolder(document.uri);
     const workspacePath = workspaceFolder?.uri.fsPath || '';
     
     // Get open files and recent edits
     const openFiles = vscode.workspace.textDocuments.map(doc => ({
       name: doc.fileName,
       language: doc.languageId,
       isActive: doc === document
     }));
     
     return {
       file: {
         content: fileContent,
         name: fileName,
         language: languageId
       },
       workspace: {
         path: workspacePath,
         openFiles: openFiles
       },
       selection: activeEditor.selection,
       visibleRanges: activeEditor.visibleRanges
     };
   }
   ```

3. **Communication with AI Backend**:
   ```typescript
   async function callAssistantAPI(query: string, context: any) {
     // In real implementations, this might use MCP or a custom protocol
     const response = await fetch('https://ai-assistant-api.example.com/query', {
       method: 'POST',
       headers: {
         'Content-Type': 'application/json',
         'Authorization': `Bearer ${apiKey}`
       },
       body: JSON.stringify({
         query,
         context
       })
     });
     
     return await response.json();
   }
   ```

## Part 5: Building a Complete IDE Agent System

Let's create a more comprehensive example that combines the MCP Python SDK with a frontend component:

### Step 1: The Backend Agent Server (MCP)

```python
# ide_agent_backend.py
from mcp.server.fastmcp import FastMCP
import os
import re
import glob
import json
from typing import List, Dict, Any, Optional

# Initialize the MCP server
ide_agent = FastMCP("IDE Assistant")

# Define resource for retrieving file contents
@ide_agent.resource("file://{file_path}")
def get_file_content(file_path: str) -> str:
    """Get the content of a file as a resource."""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            return f.read()
    except Exception as e:
        return f"Error reading file: {str(e)}"

# File operations tools
@ide_agent.tool()
def read_file(file_path: str) -> str:
    """Read a file's contents."""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            return f.read()
    except Exception as e:
        return f"Error reading file: {str(e)}"

@ide_agent.tool()
def write_file(file_path: str, content: str) -> str:
    """Write content to a file."""
    try:
        directory = os.path.dirname(os.path.abspath(file_path))
        os.makedirs(directory, exist_ok=True)
        
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(content)
        return f"Successfully wrote to {file_path}"
    except Exception as e:
        return f"Error writing to file: {str(e)}"

@ide_agent.tool()
def list_directory(directory_path: str = ".", pattern: str = "*") -> str:
    """List files in a directory, optionally filtered by pattern."""
    try:
        matches = glob.glob(os.path.join(directory_path, pattern))
        files = [os.path.relpath(f, directory_path) for f in matches]
        return json.dumps(files)
    except Exception as e:
        return f"Error listing directory: {str(e)}"

@ide_agent.tool()
def search_in_files(search_term: str, directory: str = ".", file_pattern: str = "*") -> str:
    """Search for a term in files matching pattern in directory."""
    results = []
    
    try:
        for file_path in glob.glob(os.path.join(directory, "**", file_pattern), recursive=True):
            if os.path.isfile(file_path):
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        for i, line in enumerate(f, 1):
                            if search_term in line:
                                results.append(f"{file_path}:{i}: {line.strip()}")
                except Exception:
                    # Skip files that can't be read
                    pass
    except Exception as e:
        return f"Error searching files: {str(e)}"
    
    if results:
        return "\n".join(results)
    else:
        return f"No matches found for '{search_term}'."

@ide_agent.tool()
def analyze_code(file_path: str) -> str:
    """Analyze a code file and provide insights."""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        file_extension = os.path.splitext(file_path)[1].lower()
        
        # Count lines
        line_count = content.count('\n') + 1
        
        # Basic stats
        stats = {
            "line_count": line_count,
            "character_count": len(content),
            "file_type": file_extension,
        }
        
        # Language-specific analysis could be added here
        if file_extension == '.py':
            # Count functions and classes
            function_matches = re.findall(r'def\s+(\w+)\s*\(', content)
            class_matches = re.findall(r'class\s+(\w+)\s*[\(:]', content)
            
            stats["function_count"] = len(function_matches)
            stats["class_count"] = len(class_matches)
            stats["functions"] = function_matches
            stats["classes"] = class_matches
            
        return json.dumps(stats, indent=2)
    except Exception as e:
        return f"Error analyzing code: {str(e)}"

@ide_agent.tool()
def create_unit_test(file_path: str, function_name: Optional[str] = None) -> str:
    """Generate a unit test for the specified file or function."""
    try:
        # Get the file content
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # This would typically use an LLM to generate tests
        # For this example, we'll return a placeholder
        return f"# Generated unit test for {file_path}\n# Function: {function_name or 'all'}\n\n# Placeholder for actual test generation\n\ndef test_{function_name or 'example'}():\n    assert True"
    except Exception as e:
        return f"Error creating unit test: {str(e)}"

@ide_agent.prompt()
def explain_code(file_path: str) -> str:
    """Create a prompt to explain the code in the specified file."""
    return f"""Please explain the following code in detail, breaking down:
1. What it does
2. How it works
3. Any potential improvements

File: {file_path}

```
{read_file(file_path)}
```
"""

@ide_agent.prompt()
def add_feature(file_path: str, feature_description: str) -> str:
    """Create a prompt to add a new feature to an existing file."""
    return f"""Please implement the following feature:

Feature to add: {feature_description}

Current file ({file_path}):
```
{read_file(file_path)}
```

Provide the updated file with the new feature implemented. The response should be the entire file content with your changes integrated.
"""

# Run the server
if __name__ == "__main__":
    ide_agent.run()
```

### Step 2: A Frontend Client for the IDE (Example in a WebApp)

This would be a simplified version of what an IDE extension might do. We'll use Python with Flask for this example:

```python
# ide_frontend.py
import asyncio
import threading
from flask import Flask, request, jsonify, render_template
from mcp import ClientSession, types
from mcp.client.http import http_client
import openai

# Initialize Flask app
app = Flask(__name__)

# OpenAI client for the agent's "brain"
openai_client = openai.AsyncOpenAI(api_key="your-api-key")

# Global variables
mcp_session = None
session_lock = threading.Lock()

# Agent callback (LLM-based reasoning)
async def agent_callback(request: types.CreateMessageRequestParams) -> types.CreateMessageResult:
    # Extract conversation history
    conversation = []
    for msg in request.messages:
        if isinstance(msg.content, types.TextContent):
            conversation.append({
                "role": msg.role,
                "content": msg.content.text,
            })
    
    # Call the LLM to generate a response
    try:
        completion = await openai_client.chat.completions.create(
            model="gpt-4",
            messages=[
                {"role": "system", "content": 
                    """You are an intelligent IDE assistant. You help with coding tasks by:
                    1. Reading files when needed
                    2. Writing or updating files when requested
                    3. Searching across code
                    4. Analyzing code patterns
                    5. Generating test cases
                    
                    Use the available tools to help the user. Be specific, clear, and helpful.
                    When suggesting code changes, explain what you're changing and why."""
                },
                *conversation
            ],
            temperature=0.2,
        )
        
        response_text = completion.choices[0].message.content
        
        return types.CreateMessageResult(
            role="assistant",
            content=types.TextContent(
                type="text",
                text=response_text,
            ),
            model="gpt-4",
            stopReason="endTurn",
        )
    except Exception as e:
        print(f"Error calling LLM: {e}")
        return types.CreateMessageResult(
            role="assistant",
            content=types.TextContent(
                type="text",
                text=f"I encountered an error: {str(e)}",
            ),
            stopReason="error",
        )

# Initialize the MCP connection
async def initialize_mcp():
    global mcp_session
    
    server_url = "http://localhost:8000"  # URL where the IDE agent MCP server is running
    
    try:
        async with http_client(server_url) as (read_stream, write_stream):
            async with ClientSession(
                read_stream,
                write_stream,
                sampling_callback=agent_callback
            ) as session:
                await session.initialize()
                
                with session_lock:
                    mcp_session = session
                
                # Keep the session alive indefinitely
                while True:
                    await asyncio.sleep(1)
    except Exception as e:
        print(f"MCP connection error: {e}")
        # Attempt to reconnect after a delay
        await asyncio.sleep(5)
        asyncio.create_task(initialize_mcp())

# Start the MCP connection in a background task
def start_mcp_connection():
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    loop.create_task(initialize_mcp())
    loop.run_forever()

# Start the MCP connection thread
threading.Thread(target=start_mcp_connection, daemon=True).start()

# Flask routes
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/tools', methods=['GET'])
async def list_tools():
    if mcp_session is None:
        return jsonify({"error": "MCP session not initialized"}), 503
    
    with session_lock:
        tools = await asyncio.to_thread(lambda: asyncio.run(mcp_session.list_tools()))
    
    return jsonify({
        "tools": [{"name": t.name, "description": t.description} for t in tools]
    })

@app.route('/api/call-tool', methods=['POST'])
async def call_tool():
    if mcp_session is None:
        return jsonify({"error": "MCP session not initialized"}), 503
    
    data = request.json
    tool_name = data.get('name')
    arguments = data.get('arguments', {})
    
    with session_lock:
        result = await asyncio.to_thread(
            lambda: asyncio.run(mcp_session.call_tool(tool_name, arguments))
        )
    
    return jsonify({"result": result})

@app.route('/api/query', methods=['POST'])
async def query_assistant():
    if mcp_session is None:
        return jsonify({"error": "MCP session not initialized"}), 503
    
    data = request.json
    user_query = data.get('query')
    
    # In a real implementation, this would trigger a more complex interaction
    # with the agent, potentially multiple tool calls, etc.
    # For simplicity, we'll just pass the query directly to a tool
    tool_result = "This would be the result of processing your query through the agent."
    
    return jsonify({"response": tool_result})

if __name__ == '__main__':
    app.run(debug=True, port=5000)
```

### Step 3: HTML Template (simplified)

```html
<!-- templates/index.html -->
<!DOCTYPE html>
<html>
<head>
    <title>IDE Assistant</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            display: flex;
            height: 100vh;
        }
        
        #sidebar {
            width: 250px;
            background-color: #f5f5f5;
            border-right: 1px solid #ddd;
            padding: 15px;
            overflow-y: auto;
        }
        
        #editor {
            flex-grow: 1;
            display: flex;
            flex-direction: column;
        }
        
        #code-editor {
            flex-grow: 1;
            border: 1px solid #ddd;
            margin: 15px;
            padding: 10px;
            font-family: monospace;
            white-space: pre;
            overflow: auto;
        }
        
        #assistant {
            height: 200px;
            border-top: 1px solid #ddd;
            padding: 15px;
            display: flex;
            flex-direction: column;
        }
        
        #messages {
            flex-grow: 1;
            overflow-y: auto;
            margin-bottom: 10px;
        }
        
        #input-area {
            display: flex;
        }
        
        #query-input {
            flex-grow: 1;
            padding: 8px;
            border: 1px solid #ddd;
            border-radius: 4px;
        }
        
        button {
            margin-left: 10px;
            padding: 8px 15px;
            background-color: #4285f4;
            color: white;
            border: none;
            border-radius: 4px;
            cursor: pointer;
        }
        
        button:hover {
            background-color: #3367d6;
        }
        
        .file {
            padding: 5px;
            cursor: pointer;
        }
        
        .file:hover {
            background-color: #e5e5e5;
        }
    </style>
</head>
<body>
    <div id="sidebar">
        <h3>Files</h3>
        <div id="file-browser">
            <div class="file" onclick="loadFile('main.py')">main.py</div>
            <div class="file" onclick="loadFile('utils.py')">utils.py</div>
            <div class="file" onclick="loadFile('config.json')">config.json</div>
        </div>
        
        <h3>Tools</h3>
        <div id="tools-list">
            <button onclick="callTool('read_file')">Read File</button>
            <button onclick="callTool('search_in_files')">Search</button>
            <button onclick="callTool('analyze_code')">Analyze</button>
        </div>
    </div>
    
    <div id="editor">
        <div id="code-editor" contenteditable="true"></div>
        
        <div id="assistant">
            <div id="messages"></div>
            <div id="input-area">
                <input id="query-input" type="text" placeholder="Ask your assistant...">
                <button onclick="sendQuery()">Send</button>
            </div>
        </div>
    </div>
    
    <script>
        // Simplified frontend JavaScript
        let currentFile = null;
        
        function loadFile(filename) {
            fetch('/api/call-tool', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    name: 'read_file',
                    arguments: {
                        file_path: filename
                    }
                })
            })
            .then(response => response.json())
            .then(data => {
                document.getElementById('code-editor').textContent = data.result;
                currentFile = filename;
            })
            .catch(error => {
                console.error('Error loading file:', error);
                document.getElementById('messages').innerHTML += 
                    `<p><strong>Error:</strong> Failed to load ${filename}</p>`;
            });
        }
        
        function callTool(toolName) {
            // Simplified - would need UI for tool arguments
            let args = {};
            
            if (toolName === 'read_file') {
                const filename = prompt('Enter file path:');
                if (!filename) return;
                args = { file_path: filename };
            } else if (toolName === 'search_in_files') {
                const term = prompt('Enter search term:');
                if (!term) return;
                args = { search_term: term };
            } else if (toolName === 'analyze_code') {
                if (!currentFile) {
                    alert('Please open a file first');
                    return;
                }
                args = { file_path: currentFile };
            }
            
            fetch('/api/call-tool', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    name: toolName,
                    arguments: args
                })
            })
            .then(response => response.json())
            .then(data => {
                document.getElementById('messages').innerHTML += 
                    `<p><strong>Tool Result:</strong><br>${data.result}</p>`;
            })
            .catch(error => {
                console.error('Error calling tool:', error);
            });
        }
        
        function sendQuery() {
            const input = document.getElementById('query-input');
            const query = input.value.trim();
            
            if (!query) return;
            
            document.getElementById('messages').innerHTML += 
                `<p><strong>You:</strong> ${query}</p>`;
            input.value = '';
            
            fetch('/api/query', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    query: query
                })
            })
            .then(response => response.json())
            .then(data => {
                document.getElementById('messages').innerHTML += 
                    `<p><strong>Assistant:</strong> ${data.response}</p>`;
            })
            .catch(error => {
                console.error('Error querying assistant:', error);
            });
        }
        
        // Initialize
        document.getElementById('query-input').addEventListener('keypress', function(e) {
            if (e.key === 'Enter') {
                sendQuery();
            }
        });
        
        // List available tools
        fetch('/api/tools')
            .then(response => response.json())
            .then(data => {
                if (data.tools) {
                    const toolsList = document.getElementById('tools-list');
                    toolsList.innerHTML = '';
                    
                    data.tools.forEach(tool => {
                        const button = document.createElement('button');
                        button.textContent = tool.name;
                        button.title = tool.description;
                        button.onclick = () => callTool(tool.name);
                        toolsList.appendChild(button);
                    });
                }
            })
            .catch(error => {
                console.error('Error fetching tools:', error);
            });
    </script>
</body>
</html>
```

## How IDE Integrations Actually Work

In real IDEs like Cursor or VSCode, these components are integrated more tightly:

1. **Extension API Integration**: IDEs provide extension APIs that allow capturing:
   - Precise cursor position and selection
   - Language services information (syntax tree, symbols)
   - Project structure and dependencies
   - Runtime and debug information

2. **Command Registration**: Extensions register commands that can be invoked via keyboard shortcuts or menu:
   ```javascript
   // VSCode example
   vscode.commands.registerCommand('ai-assistant.explain', () => {
     const editor = vscode.window.activeTextEditor;
     const selection = editor.selection;
     const selectedText = editor.document.getText(selection);
     
     // Call agent with context
     explainCode(selectedText, getFileContext(editor.document));
   });
   ```

3. **Language Server Protocol (LSP)**: Many IDE extensions use LSP to provide language features:
   - They can analyze code semantically
   - Provide accurate completions based on imports and types
   - Generate code that respects project structure

4. **File System Integration**: IDEs provide file system APIs:
   ```javascript
   // VSCode example
   const writeFile = async (path, content) => {
     const uri = vscode.Uri.file(path);
     const encoder = new TextEncoder();
     const data = encoder.encode(content);
     await vscode.workspace.fs.writeFile(uri, data);
   };
   ```

5. **UI Extensions**: Adding custom views, panels, and UI elements:
   ```javascript
   // VSCode example
   const panel = vscode.window.createWebviewPanel(
     'aiAssistant',
     'AI Assistant',
     vscode.ViewColumn.Beside,
     {
       enableScripts: true
     }
   );
   
   panel.webview.html = getWebviewContent();
   panel.webview.onDidReceiveMessage(message => {
     // Handle messages from the webview
   });
   ```

## Practical Considerations for Building IDE Agents

1. **Context Management**:
   - Limited LLM context windows require efficient context selection
   - Need to track what files/code the LLM has seen
   - Consider using embeddings to retrieve relevant code

2. **Error Handling**:
   - File operations can fail for many reasons
   - LLM responses might not be valid code
   - Network issues can interrupt agent communication

3. **Performance Considerations**:
   - Use incremental updates when possible
   - Cache file contents and metadata
   - Consider using thread/worker pools for operations

4. **Security**:
   - Be careful with file write operations
   - Validate generated code before execution
   - Consider adding guardrails for sensitive operations

5. **API Design Best Practices**:
   - Tools should have clear, single responsibilities
   - Error messages should be descriptive
   - Consider adding tool dependencies/composition

---
---
