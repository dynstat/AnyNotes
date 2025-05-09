I'll explain the working, purpose, and flow of the Model Context Protocol (MCP) implementation in your code.

## Overview

The code demonstrates a client-server architecture using the Model Context Protocol (MCP), which is a standardized way for AI clients and servers to communicate. MCP provides a structured protocol for AI applications to exchange information about prompts, resources, and tools.

## Key Components

### 1. Server (`server.py`)

Your server is built using the FastMCP framework from the MCP SDK. It:

- Creates an MCP server instance with the name "Demo"
- Implements tools that the client can call:
  - `fetch_news_articles`: Fetches news using the WorldNewsAPI
  - `add`: Adds two numbers
  - `get_weather`: Returns weather data for a city
- Implements a resource endpoint for personalized greetings

The server runs using stdio (standard input/output) as the transport method.

### 2. Client (`mcp_client.py`)

The client connects to the server and:
- Initializes a connection
- Lists available tools
- Lists available resources
- Calls the `add` tool with parameters
- Calls the `get_weather` tool for London
- Reads the "greeting://Alice" resource
- Processes and displays the results

## Flow Explanation

1. **Initialization**:
   - The client starts and creates a `StdioServerParameters` object that specifies how to launch the server
   - The client creates stdio streams for communication with the server
   - A `ClientSession` is established using these streams

2. **Discovery**:
   - The client queries the server for available tools and resources
   - The server responds with lists of what's available

3. **Tool Calls**:
   - The client calls the `add` tool with parameters (5 and 7)
   - The server executes the addition and returns the result
   - The client calls the `get_weather` tool for London
   - The server returns the weather data

4. **Resource Access**:
   - The client requests the "greeting://Alice" resource
   - The server generates a personalized greeting and returns it
   - The client extracts and displays the greeting

## Architecture Details

The MCP protocol is designed around three core primitives:

1. **Tools**: Functions that can be called with parameters (like `add` and `get_weather`)
2. **Resources**: Data that can be accessed by URI (like `greeting://Alice`)
3. **Prompts**: Templates for interactions (not used in your sample)

The communication happens through standard I/O streams, where:
- The client launches the server as a subprocess
- They exchange JSON-formatted messages according to the MCP protocol
- The client processes and displays the responses

## Code Structure Breakdown

The client uses the MCP SDK's high-level abstractions:
- `StdioServerParameters` configures how to launch the server
- `stdio_client` handles the low-level communication
- `ClientSession` provides a clean API for MCP operations

The server uses `FastMCP` to:
- Define tools with the `@mcp.tool()` decorator
- Define resources with the `@mcp.resource()` decorator
- Handle serialization/deserialization of requests and responses

Each request-response cycle involves:
1. The client sending a request
2. The server processing it
3. The server sending a response
4. The client handling the response

This implementation follows the official MCP SDK patterns for creating AI client-server applications with standardized communication protocols.

---
---

Here's the breakdown of both files:

## `server.py`

This script sets up and runs an MCP server using the `FastMCP` framework. It defines tools and resources that a client can interact with.

```python
# server.py

# Import necessary libraries
from mcp.server.fastmcp import FastMCP  # The MCP server framework
import worldnewsapi  # Library to interact with the WorldNewsAPI
import os  # For accessing environment variables
from dotenv import load_dotenv  # For loading variables from a .env file

# Create an MCP server instance named "Demo"
# This name might be used by clients to identify the server.
mcp = FastMCP("Demo")

# Load environment variables from a file named '.env' in the same directory.
# This is useful for keeping sensitive information like API keys out of the code.
# Ensure you have a .env file with WEATHER_API_KEY=your_api_key
load_dotenv()

# Get the weather API key from the loaded environment variables.
# If the variable isn't set, WEATHER_API_KEY will be None.
WEATHER_API_KEY = os.getenv("WEATHER_API_KEY")

# Configure the WorldNewsAPI client with the API key.
# The structure {'apiKey': WEATHER_API_KEY} is specific to how the worldnewsapi library expects the key.
newsapi_configuration = worldnewsapi.Configuration(api_key={"apiKey": WEATHER_API_KEY})


# Define a tool named 'fetch_news_articles'
# The @mcp.tool() decorator registers the following function as an MCP tool.
# Clients can discover and call this tool by its name ('fetch_news_articles').
@mcp.tool()
def fetch_news_articles(
    # Parameter: Search query for news articles. Defaults to 'politics'.
    query_text="politics",
    # Parameter: Earliest publication date (YYYY-MM-DD). Defaults to '2025-04-17'.
    earliest_date="2025-04-17",
    # Parameter: Latest publication date (YYYY-MM-DD). Defaults to '2025-04-23'.
    latest_date="2025-04-23",
    # Parameter: Maximum number of articles to return. Defaults to 5.
    max_results=5,
) -> dict:  # The function is type-hinted to return a dictionary.
    """
    Fetches news articles from the WorldNewsAPI based on specified parameters
    and returns them as a list of dictionaries.

    Args:
        query_text (str): The text to search for in news articles
        earliest_date (str): Earliest publication date in YYYY-MM-DD format
        latest_date (str): Latest publication date in YYYY-MM-DD format
        max_results (int): Maximum number of results to return

    Returns:
        dict: A dictionary containing either 'all_data' with a list of article details
              or 'error' with an error message.
    """
    try:
        # Create an instance of the WorldNewsAPI client using the configuration.
        newsapi_instance = worldnewsapi.NewsApi(
            worldnewsapi.ApiClient(newsapi_configuration)
        )

        offset = 0  # Initialize offset for pagination (fetching results in batches).
        all_results = []  # List to accumulate results from the API.
        # Ensure max_results is an integer, as it might be passed as a string from the client.
        max_results = int(max_results)

        # Loop to fetch articles in batches until the desired number (max_results) is reached
        # or the API runs out of results.
        while len(all_results) < max_results:
            # Determine how many articles to request in this batch.
            # Request up to 100 (API limit) or the remaining needed, whichever is smaller.
            request_count = min(
                100, max_results - len(all_results)
            )

            # Call the WorldNewsAPI's search_news method with the specified parameters.
            response = newsapi_instance.search_news(
                text=query_text,
                earliest_publish_date=earliest_date,
                latest_publish_date=latest_date,
                sort="publish-time",  # Sort results by publication time. Other options might exist (e.g., 'relevance').
                sort_direction="desc",  # Sort in descending order (newest first). Other option: 'asc'.
                offset=offset,  # Start fetching from this offset.
                number=request_count,  # Number of articles to fetch in this request.
            )

            # Check if the API returned any news articles in this batch.
            if len(response.news) == 0:
                break  # Exit the loop if no more articles are found.

            # Add the fetched articles to the list.
            all_results.extend(response.news)
            # Increment the offset for the next potential batch request.
            offset += 100

        # Convert the raw API response objects into a simpler list of dictionaries.
        articles_list = []
        # Iterate only up to max_results, even if more were fetched due to batching.
        for article in all_results[:max_results]:
            article_dict = {
                "title": article.title,
                "author": article.authors,  # Note: This might be a list.
                "url": article.url,
                # Create a short preview of the text (first 80 chars).
                "text_preview": article.text[:80] + "..." if article.text else "",
                "full_text": article.text,
                "publish_date": article.publish_date,
            }
            articles_list.append(article_dict)

        # Return the formatted list of articles within a dictionary.
        # The key 'all_data' is chosen here; clients will need to know this key.
        return {"all_data": articles_list}

    # Handle potential exceptions during the API call.
    except worldnewsapi.ApiException as e:
        # If an API error occurs, return a dictionary with an 'error' key.
        return {"error": str(e)}


# Define a simple tool named 'add'
@mcp.tool()
def add(a: int, b: int) -> int:  # Takes two integers, returns an integer.
    """Add two numbers"""
    # Perform the addition.
    return a + b


# Define a tool named 'get_weather'
@mcp.tool()
def get_weather(city: str) -> int:  # Takes a city name (string), returns an integer.
    """it gets and retuns the weather of the city"""
    # Placeholder: This currently returns a fixed value (45) regardless of the city.
    # A real implementation would call a weather API here.
    return 45


# Define a resource accessible via a URI pattern 'greeting://{name}'
# The @mcp.resource() decorator registers this function to handle requests
# matching the pattern. '{name}' is a dynamic part captured from the URI.
@mcp.resource("greeting://{name}")
def get_greeting(name: str) -> str:  # Takes the 'name' from the URI, returns a string.
    """Get a personalized greeting"""
    # Generate the greeting string using the captured name.
    return f"Hello, {name}!"


# Start the MCP server.
# transport="stdio": Configures the server to communicate over standard input/output.
# This is typical for when a client script launches the server script as a subprocess.
# Other transports might be available (e.g., websockets), depending on the MCP library version.
mcp.run(transport="stdio")
```

## `mcp_client.py`

This script acts as a client to the MCP server defined in `server.py`. It connects to the server, discovers its capabilities, and interacts with its tools and resources.

```python
import asyncio  # For running asynchronous code.
import sys  # To get the path to the current Python executable.
import os  # Potentially used for environment or path operations (though not directly in this snippet).

# Import necessary components from the MCP client library.
from mcp import ClientSession, StdioServerParameters, types
from mcp.client.stdio import stdio_client


# Define the main asynchronous function where the client logic resides.
async def main():
    print("Starting MCP client application...")

    # Configure how to launch the server process.
    server_params = StdioServerParameters(
        # command: The program to execute to start the server.
        # sys.executable refers to the Python interpreter running *this* client script.
        command=sys.executable,
        # args: A list of command-line arguments to pass to the command.
        # Here, we're telling Python to run the 'server.py' script.
        args=["server.py"],
        # env: Optional dictionary of environment variables to set for the server process.
        # None means it inherits the client's environment.
        env=None,
    )

    # Use a try...except block to catch potential errors during communication.
    try:
        # Start the server process using stdio_client and establish communication streams.
        # 'stdio_client' manages the subprocess and its stdin/stdout pipes.
        # 'read' and 'write' are asynchronous stream objects for communication.
        # 'async with' ensures the server process and streams are cleaned up properly.
        async with stdio_client(server_params) as (read, write):
            # Create an MCP client session using the communication streams.
            # 'ClientSession' handles the MCP protocol details (message formatting, requests, responses).
            # 'async with' ensures the session is properly closed.
            async with ClientSession(read, write) as session:
                # Initialize the MCP connection. This performs a handshake with the server,
                # exchanging capabilities and ensuring both sides are ready.
                await session.initialize()

                # --- Interact with the Server ---

                # 1. List available tools
                print("\n=== Available Tools ===")
                # Send a 'listTools' request to the server.
                tools_result = await session.list_tools()

                # Process the response. The structure might vary slightly based on the server/protocol version.
                # Check if the result exists and has a 'tools' attribute which is a list.
                if tools_result and tools_result.tools:
                    for tool in tools_result.tools:
                        # Extract the first line of the description for a concise summary.
                        desc_lines = tool.description.strip().split("\n")
                        print(f"- {tool.name}: {desc_lines[0].strip()}...")
                else:
                    print("No tools found.") # Handle case where no tools are reported.

                # 2. List available resource patterns
                print("\n=== Available Resources ===")
                # Send a 'listResources' request to the server.
                resources_result = await session.list_resources()

                # Process the response. The structure can vary (sometimes an object with a .resources list, sometimes just a list).
                resources_list = []
                if (
                    hasattr(resources_result, "resources") # Check if it's an object with a 'resources' attribute
                    and resources_result.resources # Check if the attribute is not empty/None
                ):
                    resources_list = resources_result.resources
                elif isinstance(resources_result, list): # Check if the result itself is a list
                    resources_list = resources_result

                if resources_list:
                    for resource_info in resources_list:
                        # Print the resource pattern information (likely includes URI pattern like 'greeting://{name}').
                        print(f"- {resource_info}")
                else:
                    print("No static resources found.")

                # 3. Call the 'add' tool
                print("\n=== Running 'add' Tool ===")
                # Send a 'callTool' request for the 'add' tool.
                # Arguments are passed as a dictionary.
                add_result = await session.call_tool("add", {"a": 5, "b": 7})

                # Process the result. Tool results are often wrapped in a structure.
                # We need to safely extract the actual text result.
                add_output = (
                    # Check if result and content exist, and the first content item has 'text'
                    add_result.content[0].text
                    if add_result
                    and add_result.content
                    and hasattr(add_result.content[0], "text")
                    # Fallback if the expected structure isn't found
                    else str(add_result)
                )
                print(f"Result of 5 + 7 = {add_output}")

                # 4. Call the 'get_weather' tool
                print("\n=== Running 'get_weather' Tool ===")
                # Send a 'callTool' request for 'get_weather' with the 'city' argument.
                weather = await session.call_tool("get_weather", {"city": "London"})

                # Process the result similarly to the 'add' tool.
                weather_output = (
                    weather.content[0].text
                    if weather
                    and weather.content
                    and hasattr(weather.content[0], "text")
                    else str(weather)
                )
                print(f"Weather in London: {weather_output}") # Will print 45 based on the current server code.

                # 5. Read the 'greeting' resource
                print("\n=== Reading 'greeting://Alice' Resource ===")
                # Send a 'readResource' request for the specific URI 'greeting://Alice'.
                # The server's @mcp.resource("greeting://{name}") handler will match this.
                # The result is often a tuple containing metadata and data.
                resource_meta, resource_data = await session.read_resource(
                    "greeting://Alice"
                )

                # Initialize fallback values.
                greeting_text = "Could not extract greeting text"
                actual_mime_type = "N/A"

                # Process the 'resource_data', which can have a complex structure.
                # Based on common patterns, the actual content might be inside a list within the tuple.
                # This robust check prevents errors if the structure is different than expected.
                if (
                    isinstance(resource_data, tuple) # Check if it's a tuple
                    and len(resource_data) > 1 # Check if it has at least two elements
                    and isinstance(resource_data[1], list) # Check if the second element is a list
                    and resource_data[1] # Check if the list is not empty
                ):
                    # Get the first item from the list (assuming the content is here)
                    first_content_item = resource_data[1][0]

                    # Safely access 'text' and 'mimeType' attributes if they exist.
                    if hasattr(first_content_item, "text"):
                        greeting_text = first_content_item.text
                    if hasattr(first_content_item, "mimeType"): # Might not be set by simple resources
                        actual_mime_type = first_content_item.mimeType

                print(f"Greeting: {greeting_text} (MIME type: {actual_mime_type})") # Should print "Hello, Alice!"

            # This message prints after the 'ClientSession' block finishes successfully.
            print("\nClient finished successfully.")

    # Catch any exceptions that occurred within the 'try' block.
    except Exception as e:
        print(f"ERROR in async block: {e}")
        import traceback
        # Print detailed traceback for debugging.
        traceback.print_exc()


# Standard Python entry point check.
if __name__ == "__main__":
    try:
        # Run the main asynchronous function using asyncio's event loop.
        asyncio.run(main())
    # Catch potential errors during the setup or execution of asyncio.run.
    except Exception as e:
        print(f"ERROR during asyncio.run: {e}")
        import traceback
        traceback.print_exc()
```
