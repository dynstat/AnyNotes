Let's break down your `main` function step-by-step to clarify why the `print` statement executes **after** the `await` statement, even though the server is still running.

### Your `main` Function

```python
async def main():
    global break_loop
    server = await websockets.serve(
        handle_client,
        "localhost",
        8765,
        max_size=None  # Allow unlimited message size
    )

    print("WebSocket Server started on ws://localhost:8765")
    print("Press Ctrl+C to stop.")

    try:
        # Wait indefinitely for the server to be closed
        await server.wait_closed()
    except asyncio.CancelledError:
        print("Server is shutting down...")
        break_loop = True
    finally:
        server.close()
        await server.wait_closed()
        print("Server has been shut down.")

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nKeyboard interrupt received. Shutting down...")
```

### Why Does the `print` Statement Execute After `await websockets.serve(...)`?

The key to understanding this behavior lies in **how `websockets.serve` and `await` work together**.

1. **`websockets.serve` is a Coroutine That Starts the Server:**
   
   - `websockets.serve` starts the WebSocket server and **immediately returns a `WebSocketServer` object**.
   - This coroutine **does not block indefinitely**; instead, it sets up the server and then completes.

2. **`await` Waits for the Coroutine to Complete:**
   
   - When you `await websockets.serve(...)`, you wait until the server setup is complete and the `WebSocketServer` object is returned.
   - After `await websockets.serve(...)` completes, the server is **actively running in the background**, handling incoming connections asynchronously.

3. **Subsequent `print` Statements Execute Immediately After:**
   
   - Once the server is set up, the `print` statements execute, indicating that the server has started.
   - The server continues to run asynchronously, allowing the `main` coroutine to proceed to `await server.wait_closed()`, which **blocks until the server is explicitly closed**.

### Detailed Step-by-Step Execution

Let's walk through the execution flow:

1. **Start the Server:**
   
   ```python
   server = await websockets.serve(
       handle_client,
       "localhost",
       8765,
       max_size=None  # Allow unlimited message size
   )
   ```
   
   - **What Happens:**
     
     - The `websockets.serve` coroutine is invoked to start the WebSocket server.
     - `await` ensures that the server setup is complete before moving forward.
     - The function **does not wait for the server to close**; it only waits for the server to start.
   
   - **Outcome:**
     
     - Server starts and begins listening for connections.
     - `server` holds the `WebSocketServer` object representing the running server.

2. **Print Server Started Messages:**
   
   ```python
   print("WebSocket Server started on ws://localhost:8765")
   print("Press Ctrl+C to stop.")
   ```
   
   - **What Happens:**
     - These `print` statements execute immediately after the server has started.
     - They provide feedback to the developer/user that the server is up and running.

3. **Wait for Server Closure:**
   
   ```python
   await server.wait_closed()
   ```
   
   - **What Happens:**
     
     - This line **blocks the `main` coroutine** until the server is closed.
     - It's an asynchronous wait, meaning other coroutines can run concurrently if they exist.
   
   - **Blocking Behavior:**
     
     - The `main` function essentially pauses here, allowing the server to continue running.
     - The server remains active, handling client connections in the background.

4. **Handling Shutdown:**
   
   ```python
   except asyncio.CancelledError:
       print("Server is shutting down...")
       break_loop = True
   finally:
       server.close()
       await server.wait_closed()
       print("Server has been shut down.")
   ```
   
   - **What Happens:**
     - When an interruption (like `Ctrl+C`) occurs, `asyncio.CancelledError` is raised.
     - The `finally` block ensures that the server is properly closed and all resources are cleaned up.

### Visualization of Execution Flow

```plaintext
1. asyncio.run(main()) is called.
2. main() coroutine starts executing.
3. await websockets.serve(...) is called.
    a. Server setup begins.
    b. Server starts listening on ws://localhost:8765.
    c. websockets.serve(...) completes and returns the server object.
4. print("WebSocket Server started on ws://localhost:8765") is executed.
5. print("Press Ctrl+C to stop.") is executed.
6. await server.wait_closed() is called.
    a. main() coroutine pauses here.
    b. Server continues running, handling clients asynchronously.
7. Upon shutdown (e.g., Ctrl+C):
    a. asyncio.CancelledError is raised.
    b. "Server is shutting down..." is printed.
    c. Server is closed and cleaned up.
    d. "Server has been shut down." is printed.
```

### Simplified Example to Illustrate `asyncio` Behavior

To further clarify, here's a simplified example that mimics the behavior of starting a server and then waiting for it to close.

```python
import asyncio

async def start_server():
    print("Starting server...")
    # Simulate server setup with a short delay
    await asyncio.sleep(1)
    print("Server has started.")
    return "ServerObject"

async def wait_for_shutdown(server):
    print("Server is running. Waiting for shutdown signal...")
    try:
        # Simulate waiting indefinitely
        await asyncio.Future()  # This will run forever until cancelled
    except asyncio.CancelledError:
        print("Shutdown signal received.")
    finally:
        print("Server is shutting down...")

async def main():
    # Start the server
    server = await start_server()

    # Execute wait_for_shutdown concurrently
    shutdown_task = asyncio.create_task(wait_for_shutdown(server))

    try:
        await shutdown_task
    except KeyboardInterrupt:
        print("KeyboardInterrupt received. Cancelling shutdown_task...")
        shutdown_task.cancel()
        await shutdown_task
        print("Server has been shut down.")

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("Program terminated by user.")
```

**Explanation:**

1. **`start_server` Coroutine:**
   
   - Simulates starting a server with a 1-second delay.
   - Prints messages before and after the delay to indicate server status.

2. **`wait_for_shutdown` Coroutine:**
   
   - Waits indefinitely using `await asyncio.Future()`.
   - Represents the server running and awaiting a shutdown signal.
   - Handles `asyncio.CancelledError` to initiate shutdown procedures.

3. **`main` Coroutine:**
   
   - Awaits `start_server` to simulate server setup.
   - Creates a background task (`shutdown_task`) to wait for shutdown.
   - Awaits `shutdown_task`, allowing it to run indefinitely until a cancellation signal (e.g., `Ctrl+C`) is received.
   - Handles `KeyboardInterrupt` to cancel the shutdown task and perform cleanup.

**Running the Example:**

1. **Start the Program:**
   
   ```
   Starting server...
   (1-second delay)
   Server has started.
   Server is running. Waiting for shutdown signal...
   ```

2. **Terminate the Program with `Ctrl+C`:**
   
   ```
   (User presses Ctrl+C)
   KeyboardInterrupt received. Cancelling shutdown_task...
   Shutdown signal received.
   Server is shutting down...
   Server has been shut down.
   Program terminated by user.
   ```

**Key Takeaways:**

- **`await` in `websockets.serve`:** Completes once the server is set up, **not** when it closes.
- **Execution Flow Continues After `await`:** The `print` statements execute after the server starts because `await websockets.serve(...)` only waits for the server to begin listening, not for the server to stop.
- **Blocking with `await server.wait_closed()`:** This line **pauses** the `main` coroutine, keeping the server running until a shutdown signal is received.
- **Asynchronous Nature:** The server runs in the background, allowing `main` to manage its lifecycle without blocking the entire program.

### Additional Insights

- **`asyncio.run(main())`:** Starts the event loop and runs the `main` coroutine until it's complete.
- **Event Loop:** Manages the execution of asynchronous tasks, allowing multiple coroutines to run concurrently.
- **Concurrency vs. Parallelism:** `asyncio` provides concurrency by interleaving tasks, not parallelism (which requires multiple threads or processes).

### Conclusion

The `await websockets.serve(...)` statement **does not block indefinitely**; it only waits for the server to start. Once the server is up, the subsequent `print` statements execute immediately, indicating that the server is running. The `await server.wait_closed()` line is what **actually blocks** the `main` coroutine, keeping the server active until a shutdown signal is received.

By understanding the distinction between starting the server and waiting for it to close, you can better grasp how `asyncio` manages asynchronous operations without hindering the flow of your program.

---

---

---

### Extra Topics

# Creating Coroutines in Python: With and Without `async`

Coroutines are a fundamental feature in Python for writing asynchronous code. They allow you to pause and resume execution, enabling efficient handling of tasks like I/O operations without blocking the main thread. In Python, coroutines can be created using the `async` syntax or by leveraging generator-based coroutines. Below, we'll explore both methods in detail with examples.

---

## 1. Creating a Coroutine **With** `async`

Python's `async` and `await` keywords provide a straightforward way to define and work with coroutines. This modern approach, introduced in Python 3.5, enhances readability and simplicity.

### Example: Asynchronous Coroutine Using `async`

```python
import asyncio

async def fetch_data():
    """
    Asynchronous coroutine that simulates fetching data.
    """
    print("Start fetching data...")
    await asyncio.sleep(2)  # Simulate an I/O-bound operation with asyncio.sleep
    print("Data fetched successfully!")
    return {"data": "Sample Data"}

async def main():
    """
    Main coroutine to run the fetch_data coroutine.
    """
    result = await fetch_data()  # Await the fetch_data coroutine
    print(f"Result: {result}")

if __name__ == "__main__":
    # Execute the main coroutine using asyncio's event loop
    asyncio.run(main())
```

### Explanation

1. **Importing `asyncio`**: The `asyncio` module provides the foundation for asynchronous programming in Python.

2. **Defining `fetch_data` Coroutine**:
   
   - Declared with the `async def` syntax.
   - Prints a message indicating the start of data fetching.
   - Uses `await asyncio.sleep(2)` to simulate a non-blocking I/O operation (e.g., network request).
   - After the sleep, prints a success message and returns sample data.

3. **Defining `main` Coroutine**:
   
   - Also declared with `async def`.
   - Awaits the `fetch_data` coroutine to retrieve the result.
   - Prints the fetched data.

4. **Running the Coroutine**:
   
   - The `if __name__ == "__main__":` block ensures that the `main` coroutine runs when the script is executed directly.
   - `asyncio.run(main())` starts the event loop and runs the `main` coroutine until completion.

### Output

When you run the script, you should see:

```
Start fetching data...
Data fetched successfully!
Result: {'data': 'Sample Data'}
```

---

## 2. Creating a Coroutine **Without** `async` (Using Generators)

Before the introduction of `async` and `await`, Python used generator-based coroutines. This approach leverages the `yield` statement to create coroutines, which can be more verbose and less intuitive but are still functional.

### Example: Generator-Based Coroutine

```python
import time

def fetch_data():
    """
    Generator-based coroutine that simulates fetching data.
    """
    print("Start fetching data...")
    time.sleep(2)  # Simulate a blocking I/O operation
    print("Data fetched successfully!")
    yield {"data": "Sample Data"}

def main():
    """
    Main function to run the fetch_data coroutine.
    """
    coroutine = fetch_data()  # Initialize the generator
    try:
        result = next(coroutine)  # Advance to the first yield
        print(f"Result: {result}")
    except StopIteration:
        pass  # Coroutine has finished execution

if __name__ == "__main__":
    main()
```

### Explanation

1. **Importing `time`**: The `time` module is used here to simulate a blocking operation with `sleep`.

2. **Defining `fetch_data` Coroutine**:
   
   - A regular function that serves as a generator-based coroutine.
   - Prints a message indicating the start of data fetching.
   - Uses `time.sleep(2)` to simulate a blocking I/O operation.
   - After sleeping, prints a success message and yields sample data.

3. **Defining `main` Function**:
   
   - Initializes the coroutine by calling `fetch_data()`, which returns a generator object.
   - Uses `next(coroutine)` to advance the generator to the first `yield`, effectively running the coroutine until it yields the result.
   - Prints the fetched data.
   - Handles `StopIteration` to gracefully conclude if the coroutine completes.

4. **Running the Coroutine**:
   
   - The `if __name__ == "__main__":` block ensures that the `main` function runs when the script is executed directly.
   - Calls the `main()` function to execute the coroutine.

### Output

When you run the script, you should see:

```
Start fetching data...
Data fetched successfully!
Result: {'data': 'Sample Data'}
```

### Notes

- **Blocking vs. Non-Blocking**: The generator-based coroutine uses `time.sleep`, which is blocking. In contrast, the `async` coroutine uses `await asyncio.sleep`, which is non-blocking, allowing other tasks to run concurrently.

- **Simplicity and Readability**: Using `async` and `await` simplifies coroutine syntax and improves readability, making it the preferred method in modern Python asynchronous programming.

---

## Conclusion

Python offers flexible ways to create coroutines, both with the modern `async`/`await` syntax and the traditional generator-based approach. While generator-based coroutines are still valid, using `async` and `await` is recommended for new projects due to their simplicity and non-blocking nature. Leveraging these coroutines effectively can lead to more efficient and readable asynchronous code.

---

---
