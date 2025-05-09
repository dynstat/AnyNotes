
Let's clarify the "core thing" and then create the simple package structure similar to FastMCP.

**The "Core Thing" Explained Simply**

Imagine you have a collection of useful functions (your **Tools**) and pieces of data (your **Resources**). You want a central system to:

1.  **Know about them:** This is **Registration**. You tell the system, "Here's a tool named 'calculate' that runs this function," or "Here's a resource at '/status' that gives back this data." You do this by adding them to a central list or directory, which we call a **Manager** (`ToolManager`, `ResourceManager`).
2.  **Find them when needed:** This is **Lookup/Discovery**. When a request comes in asking for the 'calculate' tool or the '/status' resource, the system looks it up in the Manager's list using the name or URI.
3.  **Use them:** This is **Execution/Access**. Once found, the system runs the tool's function with the provided arguments or gets the data from the resource.
4.  **Make them accessible:** This is the **API Layer** (like a web server using FastAPI). It provides standard URLs (endpoints) that external programs (or users) can call to trigger the lookup and execution/access steps. It handles receiving requests and sending back results over the network.

So, the **core mechanism** is this **Register -> Look Up -> Execute/Access** pattern, facilitated by Managers and exposed through an API. FastMCP provides sophisticated base classes, managers, and server integration to make this robust and feature-rich (handling argument validation, different data types, async operations, etc.), but the underlying principle is this registration and lookup flow.

**Creating Your Own Simple `simple_mcp` Package**

Let's create a basic package structure. Imagine you have a folder named `simple_mcp_project` in your workspace. Inside it, create a folder named `simple_mcp`.

**1. `simple_mcp/exceptions.py`** (For custom error types)

```python
# simple_mcp/exceptions.py
# Defines custom exceptions for better error handling.

class SimpleMCPError(Exception):
    """Base exception for the simple_mcp package."""
    pass

class ToolNotFound(SimpleMCPError):
    """Raised when a requested tool cannot be found in the manager."""
    pass

class ResourceNotFound(SimpleMCPError):
    """Raised when a requested resource cannot be found in the manager."""
    pass

class RegistrationError(SimpleMCPError):
    """Raised if there's an error during tool or resource registration."""
    pass
```

**2. `simple_mcp/core.py`** (Base classes for Tool and Resource)

```python
# simple_mcp/core.py
# Defines the basic structure for tools and resources.

import inspect
from typing import Callable, Any
from .exceptions import SimpleMCPError

class SimpleTool:
    """Represents a single executable tool (a function wrapper)."""
    def __init__(self, fn: Callable, name: str, description: str | None = None):
        if not callable(fn):
            raise TypeError("Tool function must be callable.")
        if not name:
             raise ValueError("Tool must have a name.")

        self.fn = fn
        self.name = name
        # Get description from docstring if not provided
        self.description = description or inspect.getdoc(fn) or f"Tool '{name}'"
        # Basic check for async function (can be enhanced)
        self.is_async = inspect.iscoroutinefunction(fn)

    async def execute(self, **kwargs) -> Any:
        """Executes the tool's function with given keyword arguments."""
        print(f"Executing tool '{self.name}' with args: {kwargs}")
        try:
            # Check if the function is async and await if necessary
            if self.is_async:
                return await self.fn(**kwargs)
            else:
                # Execute synchronous function directly
                return self.fn(**kwargs)
        except Exception as e:
            print(f"Error executing tool {self.name}: {e}")
            # Re-raise the original exception for the caller to handle
            raise SimpleMCPError(f"Execution failed for tool '{self.name}'") from e

    def __repr__(self):
        return f"<SimpleTool(name='{self.name}')>"


class SimpleResource:
    """Represents a readable resource (data provider)."""
    def __init__(self, uri: str, read_fn: Callable[[], Any], name: str | None = None, description: str | None = None):
        if not callable(read_fn):
            raise TypeError("Resource read function must be callable.")
        if not uri or not uri.startswith('/'):
             raise ValueError("Resource URI must be a non-empty string starting with '/'.")

        self.uri = uri
        # The function to call when reading the resource
        self._read_fn = read_fn
        # Use URI as name if not provided
        self.name = name or uri
        self.description = description or f"Resource at '{uri}'"
        # Basic check for async function
        self.is_async_reader = inspect.iscoroutinefunction(read_fn)

    async def read(self) -> Any:
        """Reads the resource's content by calling its read function."""
        print(f"Reading resource '{self.uri}' (Name: {self.name})")
        try:
            # Await if the reader function is async
            if self.is_async_reader:
                return await self._read_fn()
            else:
                 # Execute synchronous function directly
                return self._read_fn()
        except Exception as e:
            print(f"Error reading resource {self.uri}: {e}")
            # Re-raise the original exception
            raise SimpleMCPError(f"Failed to read resource '{self.uri}'") from e

    def __repr__(self):
        return f"<SimpleResource(uri='{self.uri}', name='{self.name}')>"

```

**3. `simple_mcp/managers.py`** (Managers to hold collections)

```python
# simple_mcp/managers.py
# Manages collections of tools and resources.

from typing import Dict, List
from .core import SimpleTool, SimpleResource
from .exceptions import ToolNotFound, ResourceNotFound

class SimpleToolManager:
    """Manages a dictionary of registered tools."""
    def __init__(self):
        # Stores tools with their names as keys
        self._tools: Dict[str, SimpleTool] = {}
        print("Tool Manager initialized.")

    def add_tool(self, tool: SimpleTool):
        """Registers a tool, replacing existing ones with the same name (with warning)."""
        if not isinstance(tool, SimpleTool):
            raise TypeError("Can only add SimpleTool instances.")
        if tool.name in self._tools:
            print(f"Warning: Overwriting existing tool '{tool.name}'")
        self._tools[tool.name] = tool
        print(f"Tool '{tool.name}' added to manager.")

    def get_tool(self, name: str) -> SimpleTool:
        """Retrieves a tool by its name."""
        tool = self._tools.get(name)
        if not tool:
            print(f"Error: Tool '{name}' not found in manager.")
            raise ToolNotFound(f"Tool '{name}' is not registered.")
        return tool

    def list_tools(self) -> List[SimpleTool]:
        """Returns a list of all registered tools."""
        return list(self._tools.values())

    def has_tool(self, name: str) -> bool:
        """Checks if a tool with the given name exists."""
        return name in self._tools

class SimpleResourceManager:
    """Manages a dictionary of registered resources."""
    def __init__(self):
        # Stores resources with their URIs as keys
        self._resources: Dict[str, SimpleResource] = {}
        print("Resource Manager initialized.")

    def add_resource(self, resource: SimpleResource):
        """Registers a resource, replacing existing ones with the same URI (with warning)."""
        if not isinstance(resource, SimpleResource):
             raise TypeError("Can only add SimpleResource instances.")
        if resource.uri in self._resources:
            print(f"Warning: Overwriting existing resource '{resource.uri}'")
        self._resources[resource.uri] = resource
        print(f"Resource '{resource.uri}' added to manager.")

    def get_resource(self, uri: str) -> SimpleResource:
        """Retrieves a resource by its URI."""
        resource = self._resources.get(uri)
        if not resource:
            print(f"Error: Resource '{uri}' not found in manager.")
            raise ResourceNotFound(f"Resource with URI '{uri}' is not registered.")
        return resource

    def list_resources(self) -> List[SimpleResource]:
        """Returns a list of all registered resources."""
        return list(self._resources.values())

    def has_resource(self, uri: str) -> bool:
        """Checks if a resource with the given URI exists."""
        return uri in self._resources

```

**4. `simple_mcp/server.py`** (The main server class with registration helpers)

```python
# simple_mcp/server.py
# The main server class that holds managers and provides registration methods.

from typing import Callable, Any
from .managers import SimpleToolManager, SimpleResourceManager
from .core import SimpleTool, SimpleResource
from .exceptions import RegistrationError

class SimpleMCPServer:
    """
    A simple MCP server that holds tool and resource managers
    and provides decorators for easy registration.
    """
    def __init__(self):
        print("Initializing SimpleMCPServer...")
        self.tool_manager = SimpleToolManager()
        self.resource_manager = SimpleResourceManager()
        print("SimpleMCPServer initialized with managers.")

    def tool(self, name: str | None = None, description: str | None = None):
        """
        Decorator factory to register a function as a SimpleTool.

        Args:
            name: The name to register the tool under. If None, uses the function's name.
            description: A description for the tool. If None, uses the function's docstring.

        Returns:
            A decorator function.
        """
        def decorator(fn: Callable):
            # Determine the tool's name (function name or provided name)
            tool_name = name or getattr(fn, '__name__', None)
            if not tool_name or tool_name == '<lambda>':
                 raise RegistrationError("Tool name cannot be empty or '<lambda>'. Provide a name or use a named function.")

            # Create the SimpleTool instance
            tool_instance = SimpleTool(fn=fn, name=tool_name, description=description)

            # Add the tool to the manager
            self.tool_manager.add_tool(tool_instance)

            # Return the original function so it can still be called directly if needed
            return fn
        return decorator

    def resource(self, uri: str, name: str | None = None, description: str | None = None):
        """
        Decorator factory to register a function as a SimpleResource reader.

        Args:
            uri: The URI path for the resource (must start with '/').
            name: An optional friendly name for the resource. Defaults to the URI.
            description: An optional description.

        Returns:
            A decorator function.
        """
        def decorator(read_fn: Callable[[], Any]):
             # Validate URI
             if not uri or not isinstance(uri, str) or not uri.startswith('/'):
                 raise RegistrationError(f"Invalid URI '{uri}'. Must be a string starting with '/'.")

             # Create the SimpleResource instance
             resource_instance = SimpleResource(
                 uri=uri,
                 read_fn=read_fn,
                 name=name,
                 description=description
             )

             # Add the resource to the manager
             self.resource_manager.add_resource(resource_instance)

             # Return the original reader function
             return read_fn
        return decorator

```

**5. `simple_mcp/__init__.py`** (Makes it a package and exports main components)

```python
# simple_mcp/__init__.py
# Makes the directory a Python package and exports key components.

# Import the main server class to be easily accessible
from .server import SimpleMCPServer

# Optionally import base classes and exceptions if users might need them directly
from .core import SimpleTool, SimpleResource
from .exceptions import SimpleMCPError, ToolNotFound, ResourceNotFound, RegistrationError

# Control what gets imported when someone does 'from simple_mcp import *'
__all__ = [
    "SimpleMCPServer",
    "SimpleTool",         # Optional export
    "SimpleResource",     # Optional export
    "SimpleMCPError",     # Optional export
    "ToolNotFound",       # Optional export
    "ResourceNotFound",   # Optional export
    "RegistrationError",  # Optional export
]

print("simple_mcp package loaded.")

```

**How to Use Your `simple_mcp` Package**

Now, outside the `simple_mcp` folder (e.g., in `simple_mcp_project/my_server.py`), you can create your server:

```python
# my_server.py (Place this file *outside* the simple_mcp folder, e.g., next to it)

# Need FastAPI & Uvicorn: pip install fastapi uvicorn[standard]
from fastapi import FastAPI, HTTPException, Body, Response, status
from typing import Dict, Any
import uvicorn
import platform # For example tool

# --- Import your package ---
# Assumes 'simple_mcp' directory is in the same parent directory or installed
try:
    from simple_mcp import SimpleMCPServer, ToolNotFound, ResourceNotFound
except ImportError:
    print("Error: Could not import simple_mcp package.")
    print("Ensure the simple_mcp directory is in the correct location relative to my_server.py")
    import sys
    sys.exit(1)

# --- 1. Create an instance of your MCP Server ---
print("Creating MCP server instance...")
mcp = SimpleMCPServer()
print("MCP server instance created.")

# --- 2. Define and Register Tools ---
print("Registering tools...")

@mcp.tool() # Use the decorator from your package
def add(x: int, y: int) -> int:
    """Adds two integer numbers."""
    print(f"Tool 'add' called with x={x}, y={y}")
    return x + y

@mcp.tool(name="system_platform", description="Gets the OS platform name.")
def get_os_details() -> str:
    """Returns the operating system platform identifier."""
    print("Tool 'system_platform' called.")
    return platform.system()

# Example of an async tool
import asyncio
@mcp.tool()
async def wait_and_echo(message: str, delay: float = 1.0) -> str:
    """Waits for a specified delay and then echoes the message."""
    print(f"Tool 'wait_and_echo' called with message='{message}', delay={delay}")
    await asyncio.sleep(delay)
    return f"Waited {delay}s, echoed: {message}"

print("Tools registered.")

# --- 3. Define and Register Resources ---
print("Registering resources...")

@mcp.resource(uri="/server/info", name="server_info")
def get_server_details() -> Dict[str, Any]:
    """Provides basic information about this server."""
    print("Resource '/server/info' read.")
    # In a real app, this might be dynamic
    return {
        "name": "My Simple MCP Server",
        "version": "0.1.0",
        "status": "Operational",
        "registered_tools": len(mcp.tool_manager.list_tools()),
        "registered_resources": len(mcp.resource_manager.list_resources()),
    }

@mcp.resource(uri="/config/default_user")
def get_default_user() -> str:
    """Returns the default username."""
    print("Resource '/config/default_user' read.")
    return "guest"

# Example of an async resource reader
@mcp.resource(uri="/data/dynamic_value")
async def fetch_dynamic_data() -> float:
    """Simulates fetching a dynamic value asynchronously."""
    print("Resource '/data/dynamic_value' reading...")
    await asyncio.sleep(0.5) # Simulate network delay
    import random
    value = round(random.random() * 100, 2)
    print(f"Resource '/data/dynamic_value' read completed. Value: {value}")
    return value


print("Resources registered.")

# --- 4. Create FastAPI App to Expose the MCP ---
print("Creating FastAPI application...")
api = FastAPI(
    title="My Simple MCP Server API",
    description="Exposes tools and resources managed by simple_mcp.",
    version="1.0",
)
print("FastAPI application created.")

@api.get("/tools", summary="List available tools")
async def list_tools_api():
    """Returns a list of registered tools with their names and descriptions."""
    print("API Request: GET /tools")
    tools = mcp.tool_manager.list_tools()
    return [{"name": t.name, "description": t.description} for t in tools]

@api.post("/tools/{tool_name}/execute", summary="Execute a specified tool")
async def execute_tool_api(tool_name: str, arguments: Dict[str, Any] = Body(..., embed=True)):
    """
    Executes the tool identified by `tool_name`.
    The required arguments for the tool should be provided in the request body
    as a JSON object under the key 'arguments'.
    Example Body: `{"arguments": {"x": 5, "y": 10}}`
    """
    print(f"API Request: POST /tools/{tool_name}/execute with args: {arguments}")
    try:
        tool = mcp.tool_manager.get_tool(tool_name) # Raises ToolNotFound if not found
        # Pass the arguments dictionary using **
        result = await tool.execute(**arguments)
        return {"tool_name": tool_name, "result": result}
    except ToolNotFound as e:
        print(f"API Error 404: {e}")
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail=str(e))
    except TypeError as e:
        # Handle errors where provided arguments don't match tool signature
        print(f"API Error 400 (TypeError): {e}")
        raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST, detail=f"Invalid arguments for tool '{tool_name}': {e}")
    except Exception as e:
        # Catch other errors during tool execution
        print(f"API Error 500: Tool execution failed - {e}")
        raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR, detail=f"Error executing tool '{tool_name}': {e}")

@api.get("/resources", summary="List available resources")
async def list_resources_api():
    """Returns a list of registered resources with their URIs, names, and descriptions."""
    print("API Request: GET /resources")
    resources = mcp.resource_manager.list_resources()
    return [{"uri": r.uri, "name": r.name, "description": r.description} for r in resources]

# Use :path converter to capture the full URI, including slashes
@api.get("/resources/{resource_uri:path}", summary="Read a specific resource")
async def read_resource_api(resource_uri: str):
    """
    Reads the content of the resource identified by `resource_uri`.
    The URI path should match the one used during registration (e.g., '/server/info').
    """
    # Ensure the path starts with '/' to match registration convention
    if not resource_uri.startswith('/'):
        resource_uri = '/' + resource_uri
    print(f"API Request: GET /resources{resource_uri}")

    try:
        resource = mcp.resource_manager.get_resource(resource_uri) # Raises ResourceNotFound
        content = await resource.read()

        # Return content appropriately (FastAPI handles dict/list to JSON)
        if isinstance(content, (dict, list)):
            return content
        else:
            # For other types (str, int, float, etc.), return as plain text
            return Response(content=str(content), media_type="text/plain")

    except ResourceNotFound as e:
        print(f"API Error 404: {e}")
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail=str(e))
    except Exception as e:
        # Catch other errors during resource reading
        print(f"API Error 500: Resource reading failed - {e}")
        raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR, detail=f"Error reading resource '{resource_uri}': {e}")


# --- 5. Run the Server (using Uvicorn) ---
if __name__ == "__main__":
    print("-" * 30)
    print("Attempting to start the FastAPI server using Uvicorn...")
    print(f"Registered Tools ({len(mcp.tool_manager.list_tools())}):")
    for t in mcp.tool_manager.list_tools():
        print(f"  - {t.name}")
    print(f"Registered Resources ({len(mcp.resource_manager.list_resources())}):")
    for r in mcp.resource_manager.list_resources():
        print(f"  - {r.uri} (Name: {r.name})")
    print("-" * 30)
    print("Server will run on http://127.0.0.1:8001")
    print("Access API docs at http://127.0.0.1:8001/docs")
    print("To run: uvicorn my_server:api --reload --port 8001")
    print("-" * 30)

    # host="127.0.0.1" makes it accessible only locally
    # port=8001 is an alternative to the default 8000
    uvicorn.run("my_server:api", host="127.0.0.1", port=8001, reload=True) # reload=True for development
```

Now you have:

1.  A reusable `simple_mcp` package with the core logic.
2.  An example (`my_server.py`) showing how to *use* your package to define tools and resources and expose them via a FastAPI web server.

You can run `my_server.py` (e.g., `python my_server.py` or `uvicorn my_server:api --reload --port 8001`), and then interact with it using tools like `curl` or by visiting `http://127.0.0.1:8001/docs` in your browser to see the automatically generated API documentation.

---
----
---

Yes, the code provided for the `simple_mcp` package and the `my_server.py` example *should* be working code, assuming:

1.  You have Python installed.
2.  You have installed the necessary dependencies: `fastapi` and `uvicorn`. You can install them using pip:
    ```bash
    pip install fastapi "uvicorn[standard]"
    ```
3.  You saved the files with the correct names (`exceptions.py`, `core.py`, etc.) inside a folder named `simple_mcp`.
4.  You saved the `my_server.py` file *outside* the `simple_mcp` folder (e.g., in the same directory *containing* the `simple_mcp` folder).
5.  You run the server using the command: `uvicorn my_server:api --reload --port 8001`

Let's break down how it all works, step by step:

**1. The `simple_mcp` Package (The Reusable Core Logic)**

This package is designed to provide the fundamental building blocks for creating your MCP server without needing to rewrite the management logic every time.

*   **`simple_mcp/__init__.py`:**
    *   **Purpose:** This file signals to Python that the `simple_mcp` directory should be treated as a package. It allows you to import code from other files within this directory using relative paths (like `from .core import SimpleTool`).
    *   **Imports:** It imports the key classes (`SimpleMCPServer`, `SimpleTool`, `SimpleResource`, etc.) from the other modules within the package.
    *   **`__all__`:** This list defines which symbols (classes, functions) are exported when someone uses a wildcard import (`from simple_mcp import *`). It helps control the public interface of your package.
    *   **Goal Contribution:** Makes the core components easily accessible to users of the package (like `my_server.py`) via a simple `from simple_mcp import ...`.

*   **`simple_mcp/exceptions.py`:**
    *   **Purpose:** Defines custom exception classes specific to this package.
    *   **Classes:**
        *   `SimpleMCPError`: A base class for all errors from this package. Catching this allows handling any error originating from `simple_mcp`.
        *   `ToolNotFound`, `ResourceNotFound`: Specific errors raised when a lookup fails in the managers.
        *   `RegistrationError`: Raised if something goes wrong during the `.tool()` or `.resource()` decorator setup.
    *   **Goal Contribution:** Provides more specific error information than generic Python exceptions (like `KeyError` or `ValueError`). This allows code using the package (like the FastAPI endpoints) to catch specific `simple_mcp` problems (e.g., catch `ToolNotFound` and return a 404 error) distinctly from other potential issues.

*   **`simple_mcp/core.py`:**
    *   **Purpose:** Defines the fundamental data structures representing a Tool and a Resource.
    *   **`SimpleTool` Class:**
        *   Represents a single, executable function.
        *   `__init__`: Takes the function (`fn`), a `name`, and an optional `description`. It validates inputs, automatically gets the description from the function's docstring if not provided (using `inspect.getdoc`), and checks if the function is asynchronous (using `inspect.iscoroutinefunction`).
        *   `execute`: This method is called to run the tool. It handles calling the underlying function (`self.fn`) correctly, whether it's synchronous or asynchronous (`await self.fn(...)`). It wraps the execution in a `try...except` block to catch errors and raise a `SimpleMCPError` for consistency.
    *   **`SimpleResource` Class:**
        *   Represents a piece of data that can be read via a function.
        *   `__init__`: Takes a `uri` (identifier starting with '/'), a reader function (`read_fn`), and optional `name` and `description`. It performs validation and checks if the reader function is async.
        *   `read`: Called to get the resource's content. It calls the underlying reader function (`self._read_fn`), handling sync/async correctly, and wraps it in error handling similar to `SimpleTool.execute`.
    *   **`inspect` Module:** This standard Python module is used for **introspection** – examining the properties of objects (like functions) at runtime.
        *   `inspect.getdoc(fn)`: Retrieves the documentation string (docstring) written within the function definition (`"""This function does..."""`).
        *   `inspect.iscoroutinefunction(fn)`: Checks if a function was defined using `async def`.
    *   **Goal Contribution:** Provides standardized Python objects (`SimpleTool`, `SimpleResource`) to represent the core concepts. Using `inspect` automates fetching descriptions and detecting async functions, reducing boilerplate code when defining tools/resources.

*   **`simple_mcp/managers.py`:**
    *   **Purpose:** These classes act as registries or containers for all the tools and resources your server knows about.
    *   **`SimpleToolManager`:**
        *   `_tools`: A dictionary (`Dict[str, SimpleTool]`) storing `SimpleTool` objects, keyed by their unique names.
        *   `add_tool`: Adds a `SimpleTool` instance to the `_tools` dictionary. It prints a warning if a tool with the same name is already present.
        *   `get_tool`: Looks up a tool by name in the dictionary. Raises `ToolNotFound` (from `exceptions.py`) if the name isn't found.
        *   `list_tools`, `has_tool`: Provide ways to get all tools or check for a specific one.
    *   **`SimpleResourceManager`:** Works identically to the `ToolManager`, but manages `SimpleResource` objects keyed by their URI in the `_resources` dictionary. Raises `ResourceNotFound`.
    *   **Goal Contribution:** Centralizes the management (storage and retrieval) of tools and resources. This separation of concerns means the server object doesn't need to manage these collections directly. It prevents tools/resources from getting "lost" and provides a single point of access.

*   **`simple_mcp/server.py`:**
    *   **Purpose:** This is the main class that brings everything together. It holds the managers and provides convenient ways (decorators) to register tools and resources.
    *   **`SimpleMCPServer` Class:**
        *   `__init__`: Creates instances of `SimpleToolManager` and `SimpleResourceManager` when a `SimpleMCPServer` object is created.
        *   `tool()` decorator: This is a **decorator factory**. When you call `@mcp.tool()`, it *returns* the actual decorator function (`decorator`). This inner `decorator` receives the function you defined (like `add`) as its argument (`fn`). Inside `decorator`:
            1.  It determines the `tool_name`.
            2.  It creates a `SimpleTool` instance using the passed function (`fn`) and other details.
            3.  It calls `self.tool_manager.add_tool()` to register the newly created tool instance.
            4.  It returns the original function (`fn`), which is standard practice for decorators that don't need to replace the function entirely.
        *   `resource()` decorator: Works exactly like the `tool()` decorator, but for resources. It creates a `SimpleResource` instance and registers it with `self.resource_manager.add_resource()`.
    *   **`Callable` from `typing`:** This is a type hint indicating that a variable or parameter is expected to be a function or something else that can be called.
    *   **Goal Contribution:** Provides the main user-facing object (`SimpleMCPServer`) to interact with. The decorators (`@mcp.tool()`, `@mcp.resource()`) make adding new tools and resources very easy and readable, hiding the underlying creation and registration steps within the `SimpleTool`/`SimpleResource` and Manager classes.

**2. The `my_server.py` Example (Using the `simple_mcp` Package)**

This script demonstrates how to *use* the `simple_mcp` package to build a runnable web server.

*   **Imports:**
    *   `fastapi` related: `FastAPI` is the main class for the web app. `HTTPException` is used to return proper HTTP errors (like 404 Not Found, 500 Internal Server Error). `Body` is used to define how request body data is expected. `Response` allows customizing the HTTP response (used for plain text). `status` provides standard HTTP status codes (like `status.HTTP_404_NOT_FOUND`).
    *   `uvicorn`: This is the **ASGI server**. FastAPI defines *what* to do with requests, but Uvicorn is the program that actually listens for HTTP connections and runs the FastAPI application.
    *   `platform`, `asyncio`: Used within the example tools/resources.
    *   `from simple_mcp import ...`: Imports the necessary components from the package we just defined. The `try...except` block makes the error message clearer if the package isn't found.
*   **`mcp = SimpleMCPServer()`:** Creates the central object. This automatically creates the internal `ToolManager` and `ResourceManager`.
*   **Tool/Resource Registration (`@mcp.tool()`, `@mcp.resource()`):** This is where the decorators from `simple_mcp.server` are used. When Python loads this script, it executes these decorators:
    *   For `@mcp.tool()` on `add`, it creates a `SimpleTool(fn=add, name='add', ...)` and adds it to `mcp.tool_manager`.
    *   For `@mcp.resource(uri="/server/info")` on `get_server_details`, it creates a `SimpleResource(uri='/server/info', read_fn=get_server_details, ...)` and adds it to `mcp.resource_manager`.
*   **`api = FastAPI(...)`:** Creates the web application instance.
*   **API Endpoints (`@api.get`, `@api.post`):** These FastAPI decorators define the web routes.
    *   `/tools` (GET): Fetches the list of tools from `mcp.tool_manager.list_tools()` and returns a simplified list (name, description) as JSON.
    *   `/tools/{tool_name}/execute` (POST):
        *   Takes `tool_name` from the URL path.
        *   Expects arguments in the JSON request body under the key `"arguments"` (due to `Body(..., embed=True)`).
        *   Uses `mcp.tool_manager.get_tool()` to find the tool. If not found, `ToolNotFound` is raised, caught, and turned into a 404 `HTTPException`.
        *   Calls `await tool.execute(**arguments)`. The `await` is crucial for handling both sync and async tools correctly (it does nothing extra for sync tools but correctly waits for async ones). The `**arguments` unpacks the dictionary from the request body into keyword arguments for the tool function.
        *   Handles `TypeError` (if arguments don't match the tool function, e.g., missing args, wrong types) by returning a 400 Bad Request.
        *   Handles any other exception during execution by returning a 500 Internal Server Error.
    *   `/resources` (GET): Similar to `/tools`, lists resources from `mcp.resource_manager`.
    *   `/resources/{resource_uri:path}` (GET):
        *   Takes the `resource_uri` from the path (the `:path` allows it to contain slashes).
        *   Uses `mcp.resource_manager.get_resource()`. Raises `ResourceNotFound` -> 404 if not found.
        *   Calls `await resource.read()` to get the content.
        *   Checks the type of the content. If it's a dict or list, FastAPI automatically returns it as JSON. Otherwise, it's converted to a string and returned as plain text using `fastapi.Response`.
        *   Handles other exceptions during reading -> 500 error.
*   **`if __name__ == "__main__": ... uvicorn.run(...)`:**
    *   This standard Python construct ensures the code inside only runs when the script is executed directly (not when imported as a module).
    *   `uvicorn.run("my_server:api", ...)`: This tells Uvicorn to run the FastAPI application instance named `api` found inside the file `my_server.py`.
        *   `host="127.0.0.1"`: Makes the server only listen for connections from the local machine.
        *   `port=8001`: Specifies the network port to listen on.
        *   `reload=True`: Tells Uvicorn to automatically restart the server whenever it detects changes in `my_server.py` or imported files (like those in `simple_mcp`). Very useful during development.

In essence, `simple_mcp` provides the *framework* (base classes, managers, registration helpers), and `my_server.py` *uses* that framework to define specific tools/resources and expose them via a standard FastAPI web API run by Uvicorn.

---
---


You are absolutely right! My apologies for overlooking the explanation of those specific imports. Let's break them down:

1.  **`import inspect`**
    *   **What it is:** The `inspect` module is a standard Python library (meaning you don't need to install anything extra) that provides tools for **introspection**. Introspection means examining the properties and capabilities of Python objects *while the program is running*.
    *   **Why it's used here:**
        *   In `simple_mcp/core.py` (within `SimpleTool` and `SimpleResource` `__init__` methods):
            *   `inspect.getdoc(fn)`: This function looks at the function object (`fn` or `read_fn`) and retrieves its **docstring** (the string literal written right after the `def` line, often enclosed in triple quotes `"""Docstring goes here"""`). This allows the `SimpleTool` and `SimpleResource` to automatically get a description if the user writes a docstring for their function, reducing the need to pass the `description` argument explicitly.
            *   `inspect.iscoroutinefunction(fn)`: This function checks if the provided function (`fn` or `read_fn`) was defined using `async def`. This is crucial for the `execute` and `read` methods to know whether they need to use `await` when calling the underlying function.
    *   **Goal Contribution:** `inspect` helps make the `simple_mcp` framework smarter and requires less boilerplate from the user. It automatically pulls documentation and detects asynchronous functions, leading to cleaner tool/resource definitions and correct execution logic.

2.  **`from typing import Callable, Any`**
    *   **What it is:** The `typing` module is another standard Python library specifically designed to support **type hints**. Type hints allow you to indicate the expected data types for variables, function arguments, and return values. They don't change how the code *runs* (Python is still dynamically typed), but they significantly improve code readability, help catch errors early (using tools like MyPy), and make code easier for others (and your future self) to understand.
    *   **`Callable`:**
        *   **What it means:** This type hint indicates that a variable or parameter is expected to hold something that can be "called" like a function. This includes actual functions defined with `def` or `lambda`, methods bound to objects, and even class objects themselves (as calling a class creates an instance).
        *   **Why it's used:** In `simple_mcp/core.py`, the `fn` parameter in `SimpleTool.__init__` and the `read_fn` parameter in `SimpleResource.__init__` are type-hinted as `Callable`. This clearly signals that these classes expect to be given a function to wrap. It also helps static type checkers verify that you're passing a function where one is expected.
    *   **`Any`:**
        *   **What it means:** This is a special type hint that essentially means "any type is allowed here." It's the most flexible type hint.
        *   **Why it's used:**
            *   Return types: In `SimpleTool.execute` and `SimpleResource.read`, the return type is hinted as `Any` (`-> Any`). This is because a tool or resource could potentially return *any* kind of data (a string, an integer, a list, a custom object, etc.), and we don't want to restrict it unnecessarily at the core level.
            *   Function arguments: Sometimes used within `Callable` (e.g., `Callable[..., Any]`) to indicate a function that can accept any arguments and return any type. In our simplified code, we used `Callable` directly, but more complex scenarios might use `Any` within it.
    *   **Goal Contribution:** `Callable` and `Any` (and `typing` in general) make the code's *intentions* clearer. `Callable` documents that functions are expected inputs, and `Any` acknowledges the flexibility needed for tool/resource return values. They aid in development and maintenance by making expected data types explicit.

3.  **`from typing import Dict, List`**
    *   **What it is:** These are also from the `typing` module. They provide ways to specify the types for items *within* collection types like dictionaries and lists.
    *   **`Dict`:**
        *   **What it means:** Represents the built-in `dict` type (a dictionary). You usually specify the expected type of the keys and the values. For example, `Dict[str, int]` means a dictionary where keys are strings and values are integers.
        *   **Why it's used:** In `simple_mcp/managers.py`, the internal storage for tools and resources (`_tools` and `_resources`) are type-hinted as `Dict[str, SimpleTool]` and `Dict[str, SimpleResource]`, respectively. This clearly states that these dictionaries map string keys (the tool name or resource URI) to instances of `SimpleTool` or `SimpleResource`.
    *   **`List`:**
        *   **What it means:** Represents the built-in `list` type. You specify the expected type of the elements within the list. For example, `List[str]` means a list containing only strings.
        *   **Why it's used:** In `simple_mcp/managers.py`, the `list_tools()` and `list_resources()` methods are type-hinted to return `List[SimpleTool]` and `List[SimpleResource]`. This tells anyone calling these methods that they will receive a list containing objects of that specific type.
    *   **Goal Contribution:** `Dict` and `List` provide more specific type information for collections. This makes it immediately clear what kind of data is stored in the managers' dictionaries and what kind of data is returned by the listing methods, improving clarity and enabling better static analysis.

In summary:

*   `inspect` is for looking *inside* objects at runtime (docstrings, async status).
*   `typing` (with `Callable`, `Any`, `Dict`, `List`) is for *annotating* code with expected data types to improve understanding and catch potential errors during development.
---



