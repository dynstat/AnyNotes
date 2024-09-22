# In-Depth Explanation of Futures in Python

Futures are a fundamental concept in concurrent and asynchronous programming in Python. They provide a way to handle operations that may not have completed yet but are expected to complete in the future. This detailed guide explores **Futures** in Python, covering their purpose, implementation, usage in different modules, and practical examples to illustrate their functionality.

---

## Table of Contents

1. [What is a Future?](#what-is-a-future)
2. [Types of Futures in Python](#types-of-futures-in-python)
   - [1. `concurrent.futures.Future`](#1-concurrentfuturesfuture)
   - [2. `asyncio.Future`](#2-asynciofuture)
3. [Key Differences Between `concurrent.futures.Future` and `asyncio.Future`](#key-differences-between-concurrentfuturesfuture-and-asynciofuture)
4. [Using `concurrent.futures.Future`](#using-concurrentfuturesfuture)
   - [Example: ThreadPoolExecutor with Futures](#example-threadpoolexecutor-with-futures)
5. [Using `asyncio.Future`](#using-asynciofuture)
   - [Example: asyncio with Futures](#example-asyncio-with-futures)
6. [Combining Futures with Callbacks](#combining-futures-with-callbacks)
7. [Error Handling with Futures](#error-handling-with-futures)
8. [Best Practices](#best-practices)
9. [Conclusion](#conclusion)

---

## What is a Future?

A **Future** is an abstraction that represents a value or result that may not yet be available but will be at some point in the future. It acts as a placeholder for the eventual outcome of an asynchronous or concurrent operation. Futures allow you to attach callbacks, handle results once they are available, and manage the flow of your program without blocking the main thread.

### Key Characteristics

- **Placeholder**: Represents a result that will be available later.
- **Stateful**: Can be in various states such as pending, running, completed, or cancelled.
- **Callbacks**: Support attaching functions that execute upon completion.
- **Thread-Safe**: Can be safely shared across threads (especially in `concurrent.futures`).

---

## Types of Futures in Python

Python provides two primary implementations of Futures:

1. [`concurrent.futures.Future`](https://docs.python.org/3/library/concurrent.futures.html#concurrent.futures.Future)
2. [`asyncio.Future`](https://docs.python.org/3/library/asyncio-future.html#future)

Each serves different purposes and is designed to work within their respective concurrency frameworks.

### 1. `concurrent.futures.Future`

Part of the `concurrent.futures` module, this Future is used in thread and process pools to represent the execution of a callable asynchronously. It is designed to work with synchronous code executed in separate threads or processes.

### 2. `asyncio.Future`

Part of the `asyncio` module, this Future is used within asynchronous coroutines and the event loop. It integrates seamlessly with `async`/`await` syntax to manage asynchronous tasks.

---

## Key Differences Between `concurrent.futures.Future` and `asyncio.Future`

| Feature                   | `concurrent.futures.Future`                             | `asyncio.Future`                                           |
| ------------------------- | ------------------------------------------------------- | ---------------------------------------------------------- |
| **Module**                | `concurrent.futures`                                    | `asyncio`                                                  |
| **Primary Use Case**      | Thread and process pools for executing callables        | Asynchronous programming with coroutines                   |
| **Integration**           | Works with `Executor` classes like `ThreadPoolExecutor` | Works with `async`/`await` and the event loop              |
| **Methods**               | `.result()`, `.add_done_callback()`, `.done()`, etc.    | `.result()`, `.add_done_callback()`, `.set_result()`, etc. |
| **Event Loop Dependency** | No                                                      | Yes                                                        |
| **Cancellation Support**  | Limited                                                 | More comprehensive                                         |

Understanding these differences is crucial for effectively utilizing Futures in Python's concurrent and asynchronous paradigms.

---

## Using `concurrent.futures.Future`

The `concurrent.futures` module provides a high-level interface for asynchronously executing callables using threads or processes. The `Future` objects returned by executors represent the execution of these callables.

### Example: ThreadPoolExecutor with Futures

Below is an example demonstrating how to use `concurrent.futures.Future` with `ThreadPoolExecutor` to perform asynchronous operations.

```python
# concurrent_futures_example.py

from concurrent.futures import ThreadPoolExecutor, Future
import time
from typing import Any

def compute_square(number: int) -> int:
    """
    Computes the square of a given number after a delay.

    Args:
        number (int): The number to square.

    Returns:
        int: The square of the number.
    """
    print(f"Computing square of {number}")
    time.sleep(2)  # Simulate a time-consuming computation
    result = number * number
    print(f"Computed square of {number}: {result}")
    return result

def main():
    """
    Main function to execute compute_square asynchronously using ThreadPoolExecutor.
    """
    # Create a ThreadPoolExecutor with 2 worker threads
    with ThreadPoolExecutor(max_workers=2) as executor:
        # Submit tasks to the executor
        future1: Future = executor.submit(compute_square, 3)
        future2: Future = executor.submit(compute_square, 4)

        # Attach callbacks to futures
        future1.add_done_callback(lambda fut: print(f"Result from future1: {fut.result()}"))
        future2.add_done_callback(lambda fut: print(f"Result from future2: {fut.result()}"))

        # Optionally, wait for all futures to complete
        print("Waiting for futures to complete...")
        # This will block until both futures are done
        result1 = future1.result()
        result2 = future2.result()
        print(f"Final Results: {result1}, {result2}")

if __name__ == "__main__":
    main()
```

### Explanation

1. **Importing Modules**:
   
   - `ThreadPoolExecutor` and `Future` from `concurrent.futures` handle asynchronous execution.
   - `time` is used to simulate a delay.
   - `typing.Any` for type annotations.

2. **Defining `compute_square` Function**:
   
   - Simulates a time-consuming computation by sleeping for 2 seconds.
   - Computes and returns the square of the input number.
   - Prints messages to trace the computation.

3. **Defining `main` Function**:
   
   - Creates a `ThreadPoolExecutor` with 2 worker threads to execute tasks concurrently.
   - Submits two tasks (`compute_square(3)` and `compute_square(4)`) to the executor, returning `Future` objects.
   - Attaches callbacks to each `Future` to print results once they are available.
   - Calls `.result()` on each `Future` to retrieve the results, which also blocks until the result is ready.
   - Prints the final results after both computations are complete.

4. **Execution**:
   
   - The `main` function is executed when the script runs directly.
   - Outputs trace messages to demonstrate asynchronous execution.

### Output

When you run the script, you should observe the following output with a delay of approximately 2 seconds:

```
Computing square of 3
Computing square of 4
Waiting for futures to complete...
Computed square of 3: 9
Result from future1: 9
Computed square of 4: 16
Result from future2: 16
Final Results: 9, 16
```

### File Structure

```plaintext
project/
├── concurrent_futures_example.py
```

---

## Using `asyncio.Future`

The `asyncio` module provides infrastructure for writing single-threaded concurrent code using coroutines. `asyncio.Future` is used within the event loop to represent the result of an asynchronous operation.

### Example: `asyncio` with Futures

This example demonstrates how to use `asyncio.Future` within coroutines to manage asynchronous tasks.

```python
# asyncio_future_example.py

import asyncio
from typing import Any

async def fetch_data(future: asyncio.Future, data: Any, delay: float):
    """
    Simulates fetching data asynchronously and sets the result on the future.

    Args:
        future (asyncio.Future): The future to set the result on.
        data (Any): The data to set as the result.
        delay (float): Delay in seconds before setting the result.
    """
    print(f"Fetching data with delay of {delay} seconds...")
    await asyncio.sleep(delay)  # Simulate an asynchronous operation
    future.set_result(data)
    print(f"Data fetched and future set: {data}")

async def main():
    """
    Main coroutine to demonstrate asyncio.Future usage.
    """
    loop = asyncio.get_running_loop()

    # Create a Future object
    future1: asyncio.Future = loop.create_future()
    future2: asyncio.Future = loop.create_future()

    # Schedule fetch_data coroutines to set the futures after delays
    asyncio.create_task(fetch_data(future1, {"id": 1, "value": "Data1"}, 2))
    asyncio.create_task(fetch_data(future2, {"id": 2, "value": "Data2"}, 3))

    print("Waiting for futures to complete...")

    # Await the results of the futures
    result1 = await future1
    print(f"Received result for future1: {result1}")

    result2 = await future2
    print(f"Received result for future2: {result2}")

if __name__ == "__main__":
    asyncio.run(main())
```

### Explanation

1. **Importing Modules**:
   
   - `asyncio` for asynchronous operations.
   - `typing.Any` for type annotations.

2. **Defining `fetch_data` Coroutine**:
   
   - Accepts a `Future`, some `data`, and a `delay`.
   - Simulates an asynchronous operation using `await asyncio.sleep(delay)`.
   - Sets the result of the future using `future.set_result(data)`.
   - Prints messages to trace the process.

3. **Defining `main` Coroutine**:
   
   - Retrieves the current event loop.
   - Creates two `asyncio.Future` objects using `loop.create_future()`.
   - Schedules two `fetch_data` coroutines to set results on the futures after 2 and 3 seconds, respectively, using `asyncio.create_task()`.
   - Prints a waiting message.
   - Awaits the results of both futures and prints them upon completion.

4. **Execution**:
   
   - The `main` coroutine is executed within the event loop using `asyncio.run(main())`.
   - Outputs trace messages to demonstrate asynchronous execution.

### Output

When you run the script, you should observe the following output with delays:

```
Fetching data with delay of 2 seconds...
Fetching data with delay of 3 seconds...
Waiting for futures to complete...
Data fetched and future set: {'id': 1, 'value': 'Data1'}
Received result for future1: {'id': 1, 'value': 'Data1'}
Data fetched and future set: {'id': 2, 'value': 'Data2'}
Received result for future2: {'id': 2, 'value': 'Data2'}
```

### File Structure

```plaintext
project/
├── asyncio_future_example.py
```

---

## Combining Futures with Callbacks

Futures allow attaching callback functions that execute once the future is completed. This feature enables asynchronous event handling and decouples the execution of tasks from their result processing.

### Example: Attaching Callbacks to Futures

```python
# future_with_callbacks.py

from concurrent.futures import ThreadPoolExecutor, Future
import time

def task(n: int) -> int:
    """
    Simulates a time-consuming task by sleeping and returning a result.

    Args:
        n (int): Number to process.

    Returns:
        int: Processed result.
    """
    print(f"Task {n} started.")
    time.sleep(1)
    result = n * 10
    print(f"Task {n} completed with result: {result}")
    return result

def callback(fut: Future):
    """
    Callback function to process the result of a future.

    Args:
        fut (Future): The future object.
    """
    try:
        result = fut.result()
        print(f"Callback received result: {result}")
    except Exception as e:
        print(f"Callback received an exception: {e}")

def main():
    """
    Main function to demonstrate attaching callbacks to futures.
    """
    with ThreadPoolExecutor(max_workers=2) as executor:
        futures = []
        for i in range(5):
            fut = executor.submit(task, i)
            fut.add_done_callback(callback)
            futures.append(fut)

        # Optionally wait for all futures to complete
        for fut in futures:
            fut.result()

if __name__ == "__main__":
    main()
```

### Explanation

1. **Defining `task` Function**:
   
   - Simulates a task by sleeping for 1 second.
   - Processes the input number by multiplying it by 10.
   - Prints start and completion messages.

2. **Defining `callback` Function**:
   
   - Receives a `Future` object.
   - Attempts to retrieve the result using `.result()`.
   - Prints the result or handles exceptions if any occurred during task execution.

3. **Defining `main` Function**:
   
   - Creates a `ThreadPoolExecutor` with 2 worker threads.
   - Submits 5 tasks to the executor, each processing a different number.
   - Attaches the `callback` function to each `Future` using `.add_done_callback()`.
   - Optionally waits for all futures to complete by calling `.result()` on each.

4. **Execution**:
   
   - The `main` function is executed when the script runs directly.
   - Demonstrates how callbacks are invoked upon future completion.

### Output

When you run the script, you should observe the following output with overlapping task executions:

```
Task 0 started.
Task 1 started.
Task 0 completed with result: 0
Callback received result: 0
Task 2 started.
Task 1 completed with result: 10
Callback received result: 10
Task 3 started.
Task 2 completed with result: 20
Callback received result: 20
Task 4 started.
Task 3 completed with result: 30
Callback received result: 30
Task 4 completed with result: 40
Callback received result: 40
```

### File Structure

```plaintext
project/
├── future_with_callbacks.py
```

---

## Error Handling with Futures

Futures not only represent successful completions but also allow handling exceptions that occur during task execution.

### Example: Handling Exceptions in Futures

```python
# future_exception_handling.py

from concurrent.futures import ThreadPoolExecutor, Future
import time

def faulty_task(n: int) -> int:
    """
    Simulates a faulty task that raises an exception for a specific input.

    Args:
        n (int): Number to process.

    Returns:
        int: Processed result.

    Raises:
        ValueError: If n is negative.
    """
    print(f"Faulty Task {n} started.")
    time.sleep(1)
    if n < 0:
        raise ValueError(f"Negative input not allowed: {n}")
    result = n * 2
    print(f"Faulty Task {n} completed with result: {result}")
    return result

def callback(fut: Future):
    """
    Callback function to handle the result or exception of a future.

    Args:
        fut (Future): The future object.
    """
    try:
        result = fut.result()
        print(f"Callback received result: {result}")
    except Exception as e:
        print(f"Callback received an exception: {e}")

def main():
    """
    Main function to demonstrate exception handling with futures.
    """
    with ThreadPoolExecutor(max_workers=2) as executor:
        inputs = [5, -1, 10]
        futures = []
        for n in inputs:
            fut = executor.submit(faulty_task, n)
            fut.add_done_callback(callback)
            futures.append(fut)

        # Optionally wait for all futures to complete
        for fut in futures:
            try:
                fut.result()
            except Exception:
                pass  # Exceptions are already handled in the callback

if __name__ == "__main__":
    main()
```

### Explanation

1. **Defining `faulty_task` Function**:
   
   - Simulates a task that may fail if the input number is negative.
   - Sleeps for 1 second to simulate processing.
   - Raises a `ValueError` for negative inputs.
   - Otherwise, returns twice the input number.

2. **Defining `callback` Function**:
   
   - Attempts to retrieve the result using `.result()`.
   - Catches and prints exceptions if they occurred during task execution.

3. **Defining `main` Function**:
   
   - Creates a `ThreadPoolExecutor` with 2 worker threads.
   - Submits three tasks: two valid inputs (`5`, `10`) and one invalid input (`-1`).
   - Attaches the `callback` function to each `Future`.
   - Waits for all tasks to complete, handling exceptions if necessary.

4. **Execution**:
   
   - The `main` function is executed when the script runs directly.
   - Demonstrates how exceptions within tasks are propagated and handled.

### Output

When you run the script, you should observe the following output with exception handling:

```
Faulty Task 5 started.
Faulty Task -1 started.
Faulty Task 5 completed with result: 10
Callback received result: 10
Faulty Task 10 started.
Callback received an exception: Negative input not allowed: -1
Faulty Task 10 completed with result: 20
Callback received result: 20
```

### File Structure

```plaintext
project/
├── future_exception_handling.py
```

---

## Best Practices

1. **Avoid Blocking the Event Loop**: When using `asyncio.Future`, ensure that no blocking operations are performed within coroutines to maintain responsiveness.

2. **Proper Exception Handling**: Always handle exceptions in your callbacks and when retrieving `Future` results to prevent unhandled exceptions from crashing your program.

3. **Use Executors Appropriately**: Leverage `ThreadPoolExecutor` for I/O-bound tasks and `ProcessPoolExecutor` for CPU-bound tasks when using `concurrent.futures`.

4. **Limit the Number of Workers**: When using thread or process pools, limit the number of worker threads/processes to prevent excessive resource consumption.

5. **Clean Up Resources**: Ensure that executors are properly shut down using context managers (`with` statement) to free up system resources.

6. **Prefer `async`/`await` with `asyncio.Future`**: Utilize `async`/`await` syntax for cleaner and more readable asynchronous code when working with `asyncio.Future`.

7. **Thread Safety**: When sharing `Future` objects across threads, ensure thread safety to prevent race conditions and other concurrency issues.

---

## Conclusion

Futures in Python provide a robust mechanism for handling asynchronous and concurrent operations. Whether you are working with multithreading, multiprocessing, or asynchronous coroutines, understanding how to effectively utilize `concurrent.futures.Future` and `asyncio.Future` is essential for writing efficient and maintainable code.

- **`concurrent.futures.Future`** is ideal for managing asynchronous execution in thread and process pools, allowing you to execute callables concurrently and handle their results or exceptions.

- **`asyncio.Future`** seamlessly integrates with Python's asynchronous programming paradigm, enabling you to manage coroutines and their outcomes within an event loop.

---

---

---

---

# Working with Futures in Python Using `async` and `await`

Futures are a pivotal component in Python's asynchronous programming paradigm, especially when dealing with concurrent operations. They act as placeholders for results that are yet to be computed, enabling seamless coordination between different parts of an asynchronous application. This guide delves into practical examples of using Futures with `async` and `await`, illustrating how to effectively implement them in modern Python code. Additionally, it outlines best practices to ensure robust and efficient asynchronous applications.

---

## Table of Contents

1. [Understanding `asyncio.Future`](#understanding-asynciofuture)
2. [Basic Example: Creating and Awaiting a Future](#basic-example-creating-and-awaiting-a-future)
3. [Advanced Example: Using Futures with Callbacks](#advanced-example-using-futures-with-callbacks)
4. [Handling Exceptions in Futures](#handling-exceptions-in-futures)
5. [Best Practices](#best-practices)
6. [Project Structure](#project-structure)
7. [Conclusion](#conclusion)

---

## Understanding `asyncio.Future`

In Python's `asyncio` library, a **Future** represents a result that may not yet be available but will be at some point. It acts as a synchronization primitive, allowing coroutines to wait for results without blocking the event loop. Futures are integral to the asynchronous workflow, facilitating coordination between producers and consumers of data.

### Key Characteristics

- **State Management**: A Future can be in pending, done, or canceled states.
- **Result Storage**: It holds the eventual result of an asynchronous operation.
- **Callbacks**: Allows attaching functions that execute upon completion.
- **Integration with Coroutines**: Works seamlessly with `async` and `await` constructs.

---

## Basic Example: Creating and Awaiting a Future

This example demonstrates how to create a Future, set its result asynchronously, and await its completion within a coroutine.

```python
import asyncio

async def set_future_result(fut: asyncio.Future, delay: float, result: str):
    """
    Simulates an asynchronous operation that sets the result of a Future after a delay.

    Args:
        fut (asyncio.Future): The Future to set the result on.
        delay (float): Time in seconds to wait before setting the result.
        result (str): The result to set on the Future.
    """
    await asyncio.sleep(delay)  # Simulate asynchronous work
    fut.set_result(result)       # Set the result of the Future
    print(f"Future result set to: {result}")

async def main():
    """
    Main coroutine that creates a Future, schedules its result to be set asynchronously,
    and awaits the Future's result.
    """
    loop = asyncio.get_running_loop()
    future = loop.create_future()  # Create a Future object

    # Schedule the coroutine to set the Future's result after 2 seconds
    asyncio.create_task(set_future_result(future, 2, "Hello, Future!"))

    print("Waiting for the Future to be set...")
    result = await future  # Await the Future's result
    print(f"Received Future result: {result}")

if __name__ == "__main__":
    asyncio.run(main())
```

### Explanation

1. **Importing `asyncio`**: Essential for asynchronous programming and managing the event loop.
2. **`set_future_result` Coroutine**:
   - Accepts a `Future`, a delay, and a result.
   - Awaits for the specified delay using `asyncio.sleep` to simulate an asynchronous task.
   - Sets the result of the Future using `fut.set_result(result)`.
3. **`main` Coroutine**:
   - Retrieves the current event loop using `asyncio.get_running_loop()`.
   - Creates a Future object with `loop.create_future()`.
   - Schedules the `set_future_result` coroutine as a task to run concurrently.
   - Awaits the Future's result, effectively pausing until the result is set.
4. **Execution Block**:
   - Runs the `main` coroutine within the event loop using `asyncio.run(main())`.

### Output

When you execute the script, the output will be:

```
Waiting for the Future to be set...
Future result set to: Hello, Future!
Received Future result: Hello, Future!
```

---

## Advanced Example: Using Futures with Callbacks

Futures allow attaching callback functions that execute once the Future is completed. This decouples the task execution from result handling, enhancing modularity.

```python
import asyncio

def future_callback(fut: asyncio.Future):
    """
    Callback function to process the result of a Future.

    Args:
        fut (asyncio.Future): The Future object.
    """
    try:
        result = fut.result()  # Retrieve the result of the Future
        print(f"Callback received result: {result}")
    except Exception as e:
        print(f"Callback encountered an exception: {e}")

async def perform_task(fut: asyncio.Future, delay: float, data: str):
    """
    Simulates an asynchronous task that sets the result of a Future.

    Args:
        fut (asyncio.Future): The Future to set the result on.
        delay (float): Time in seconds to wait before setting the result.
        data (str): The data to set as the result.
    """
    await asyncio.sleep(delay)     # Simulate asynchronous work
    fut.set_result(data)           # Set the result of the Future
    print(f"Task completed and result set to: {data}")

async def main():
    """
    Main coroutine that creates Futures, attaches callbacks, and schedules tasks.
    """
    loop = asyncio.get_running_loop()
    future1 = loop.create_future()
    future2 = loop.create_future()

    # Attach callbacks to the Futures
    future1.add_done_callback(future_callback)
    future2.add_done_callback(future_callback)

    # Schedule tasks to set the Futures' results
    asyncio.create_task(perform_task(future1, 1, "First Result"))
    asyncio.create_task(perform_task(future2, 2, "Second Result"))

    print("Tasks scheduled. Awaiting Futures...")
    await asyncio.gather(future1, future2)  # Await both Futures

if __name__ == "__main__":
    asyncio.run(main())
```

### Explanation

1. **`future_callback` Function**:
   - Acts as a callback for when the Future is completed.
   - Tries to retrieve the result using `fut.result()`.
   - Handles exceptions if the Future resulted in an error.
2. **`perform_task` Coroutine**:
   - Simulates an asynchronous operation by sleeping for a specified delay.
   - Sets the result of the provided Future.
   - Prints a confirmation message upon completion.
3. **`main` Coroutine**:
   - Creates two Future objects.
   - Attaches the `future_callback` to both Futures using `.add_done_callback()`.
   - Schedules the `perform_task` coroutine for each Future with different delays and data.
   - Uses `asyncio.gather()` to await both Futures concurrently.
4. **Execution Block**:
   - Runs the `main` coroutine within the event loop using `asyncio.run(main())`.

### Output

Executing the script will produce:

```
Tasks scheduled. Awaiting Futures...
Task completed and result set to: First Result
Callback received result: First Result
Task completed and result set to: Second Result
Callback received result: Second Result
```

---

## Handling Exceptions in Futures

Proper exception handling ensures that your application can gracefully manage errors occurring within asynchronous tasks. Futures propagate exceptions to awaiters and callbacks, allowing for centralized error management.

```python
import asyncio

def exception_callback(fut: asyncio.Future):
    """
    Callback function to handle the result or exception of a Future.

    Args:
        fut (asyncio.Future): The Future object.
    """
    try:
        result = fut.result()
        print(f"Callback received result: {result}")
    except Exception as e:
        print(f"Callback encountered an exception: {e}")

async def faulty_task(fut: asyncio.Future, delay: float):
    """
    Simulates a faulty asynchronous task that raises an exception.

    Args:
        fut (asyncio.Future): The Future to set the result or exception on.
        delay (float): Time in seconds to wait before raising an exception.
    """
    await asyncio.sleep(delay)  # Simulate asynchronous work
    try:
        raise ValueError("An error occurred in the faulty task!")
    except Exception as e:
        fut.set_exception(e)     # Set the exception on the Future
        print("Faulty task encountered an exception and set it on the Future.")

async def successful_task(fut: asyncio.Future, delay: float, data: str):
    """
    Simulates a successful asynchronous task.

    Args:
        fut (asyncio.Future): The Future to set the result on.
        delay (float): Time in seconds to wait before setting the result.
        data (str): The data to set as the result.
    """
    await asyncio.sleep(delay)     # Simulate asynchronous work
    fut.set_result(data)           # Set the result of the Future
    print(f"Successful task set result to: {data}")

async def main():
    """
    Main coroutine that demonstrates exception handling with Futures.
    """
    loop = asyncio.get_running_loop()
    future_success = loop.create_future()
    future_failure = loop.create_future()

    # Attach callbacks to handle results and exceptions
    future_success.add_done_callback(exception_callback)
    future_failure.add_done_callback(exception_callback)

    # Schedule tasks
    asyncio.create_task(successful_task(future_success, 1, "Success Data"))
    asyncio.create_task(faulty_task(future_failure, 2))

    print("Tasks scheduled. Awaiting Futures...")
    await asyncio.gather(future_success, future_failure, return_exceptions=True)

if __name__ == "__main__":
    asyncio.run(main())
```

### Explanation

1. **`exception_callback` Function**:
   - Acts as a callback to process both successful results and exceptions.
   - Uses `fut.result()` to retrieve the result or catch exceptions.
2. **`faulty_task` Coroutine**:
   - Simulates an operation that raises an exception after a delay.
   - Catches the exception and sets it on the Future using `fut.set_exception(e)`.
   - Prints a message indicating the exception has been set.
3. **`successful_task` Coroutine**:
   - Simulates a successful asynchronous operation by setting a result after a delay.
   - Prints a confirmation message upon setting the result.
4. **`main` Coroutine**:
   - Creates two Future objects: one for a successful task and one for a faulty task.
   - Attaches the `exception_callback` to both Futures.
   - Schedules both tasks using `asyncio.create_task()`.
   - Awaits both Futures using `asyncio.gather()`, with `return_exceptions=True` to prevent the coroutine from stopping on exceptions.
5. **Execution Block**:
   - Runs the `main` coroutine within the event loop using `asyncio.run(main())`.

### Output

Running the script will yield:

```
Tasks scheduled. Awaiting Futures...
Successful task set result to: Success Data
Callback received result: Success Data
Faulty task encountered an exception and set it on the Future.
Callback encountered an exception: An error occurred in the faulty task!
```

---

## Best Practices

Adhering to best practices ensures that your asynchronous applications are efficient, maintainable, and less prone to bugs.

1. **Leverage High-Level APIs**:
   
   - Prefer using high-level asyncio constructs like `asyncio.create_task()` and `asyncio.gather()` over manually managing Futures when possible.

2. **Avoid Blocking the Event Loop**:
   
   - Refrain from performing CPU-bound or blocking I/O operations within coroutines. Use thread or process pools for such tasks.

3. **Handle Exceptions Gracefully**:
   
   - Always handle exceptions in your coroutines and callbacks to prevent unhandled exceptions from crashing the application.

4. **Use Callbacks Judiciously**:
   
   - While callbacks can be powerful, excessive use can lead to hard-to-maintain code. Use them when necessary and prefer `await` for sequential operations.

5. **Clean Up Resources**:
   
   - Ensure that any resources (like network connections or files) are properly closed, even when exceptions occur. Use `try...finally` blocks or context managers.

6. **Limit the Number of Concurrent Tasks**:
   
   - To prevent resource exhaustion, limit the number of concurrent tasks, especially when dealing with network or I/O operations. Use semaphores or pools as needed.

7. **Document Your Code**:
   
   - Clearly document the purpose and behavior of your coroutines and callbacks to aid future maintenance and collaboration.

8. **Prefer `async`/`await` Over Callbacks for Control Flow**:
   
   - Using `async` and `await` leads to more readable and maintainable code compared to callback-based approaches.

---

## Project Structure

Organizing your project files thoughtfully enhances readability and maintainability. Below is the recommended structure for the examples provided:

```plaintext
project/
├── examples/
│   ├── async_future_basic.py
│   ├── async_future_callbacks.py
│   └── async_future_exception.py
└── README.md
```

- **`examples/` Directory**: Contains all the example scripts demonstrating different usages of Futures with `async` and `await`.
- **`README.md`**: Provides documentation and explanations for the project and examples.

---

## Advanced Example: Using Futures with Callbacks and Error Handling

This example demonstrates a more complex scenario involving multiple Futures, callbacks, error handling, and asynchronous operations.

```python
# advanced_futures_example.py

import asyncio
from typing import Any, Optional
from dataclasses import dataclass
from datetime import datetime
import logging

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s'
)
logger = logging.getLogger(__name__)

@dataclass
class TaskResult:
    """Data class to store task results."""
    task_id: str
    timestamp: datetime
    data: Any
    error: Optional[Exception] = None
    status: str = "pending"

class AsyncTaskManager:
    """Manages asynchronous tasks with callbacks and error handling."""
    
    def __init__(self):
        """Initialize the task manager."""
        self.results = {}
        self.futures = {}
        
    async def execute_task(self, task_id: str, delay: float, should_fail: bool = False) -> None:
        """
        Execute an asynchronous task with potential failure.

        Args:
            task_id (str): Unique identifier for the task
            delay (float): Simulated processing time in seconds
            should_fail (bool): Flag to simulate task failure
        """
        try:
            logger.info(f"Starting task {task_id}")
            await asyncio.sleep(delay)  # Simulate work
            
            if should_fail:
                raise ValueError(f"Simulated failure for task {task_id}")
                
            result = TaskResult(
                task_id=task_id,
                timestamp=datetime.now(),
                data=f"Processed data for {task_id}",
                status="completed"
            )
            
        except Exception as e:
            logger.error(f"Error in task {task_id}: {str(e)}")
            result = TaskResult(
                task_id=task_id,
                timestamp=datetime.now(),
                data=None,
                error=e,
                status="failed"
            )
            
        finally:
            if task_id in self.futures and not self.futures[task_id].done():
                if result.error:
                    self.futures[task_id].set_exception(result.error)
                else:
                    self.futures[task_id].set_result(result)
                    
            self.results[task_id] = result

    def task_callback(self, task_id: str, fut: asyncio.Future) -> None:
        """
        Callback function to handle task completion or failure.

        Args:
            task_id (str): Task identifier
            fut (asyncio.Future): Completed future object
        """
        try:
            result = fut.result()
            logger.info(f"Task {task_id} completed successfully: {result}")
        except Exception as e:
            logger.error(f"Task {task_id} failed: {str(e)}")
            
    async def schedule_task(self, task_id: str, delay: float, should_fail: bool = False) -> asyncio.Future:
        """
        Schedule a task and return its Future.

        Args:
            task_id (str): Task identifier
            delay (float): Processing time in seconds
            should_fail (bool): Flag to simulate failure

        Returns:
            asyncio.Future: Future representing the scheduled task
        """
        loop = asyncio.get_running_loop()
        future = loop.create_future()
        
        # Store the future
        self.futures[task_id] = future
        
        # Add callback
        future.add_done_callback(lambda f: self.task_callback(task_id, f))
        
        # Schedule the task
        asyncio.create_task(self.execute_task(task_id, delay, should_fail))
        
        return future

async def main():
    """Main function to demonstrate the AsyncTaskManager."""
    task_manager = AsyncTaskManager()
    
    # Schedule multiple tasks with different configurations
    tasks = [
        ("task1", 2.0, False),  # Normal task
        ("task2", 1.5, True),   # Task that will fail
        ("task3", 3.0, False),  # Normal task with longer delay
    ]
    
    futures = []
    for task_id, delay, should_fail in tasks:
        future = await task_manager.schedule_task(task_id, delay, should_fail)
        futures.append(future)
    
    # Wait for all tasks to complete, handling exceptions
    try:
        results = await asyncio.gather(*futures, return_exceptions=True)
        
        # Process results
        for task_id, result in zip([t[0] for t in tasks], results):
            if isinstance(result, Exception):
                logger.error(f"Task {task_id} failed with error: {result}")
            else:
                logger.info(f"Task {task_id} final result: {result}")
                
    except Exception as e:
        logger.error(f"Error during task execution: {str(e)}")
        
    finally:
        # Print final status of all tasks
        logger.info("\nFinal Task Status:")
        for task_id, result in task_manager.results.items():
            logger.info(f"{task_id}: {result.status}")

if __name__ == "__main__":
    asyncio.run(main())
```

### Key Features and Concepts

1. **Structured Data Management**:
   - Uses `@dataclass` for organizing task results
   - Maintains separate dictionaries for futures and results

2. **Comprehensive Error Handling**:
   - Try-except blocks at multiple levels
   - Proper error propagation through futures
   - Logging of errors and status updates

3. **Callback System**:
   - Task-specific callbacks
   - Result processing in callbacks
   - Error handling in callbacks

4. **Resource Management**:
   - Proper cleanup of futures
   - Status tracking for all tasks
   - Final status reporting

### Usage Example

To run this example:

```python
# Run the script directly
python advanced_futures_example.py
```

### Expected Output

```
2024-01-01 10:00:00,000 - INFO - Starting task task1
2024-01-01 10:00:00,000 - INFO - Starting task task2
2024-01-01 10:00:00,000 - INFO - Starting task task3
2024-01-01 10:00:01,500 - ERROR - Error in task task2: Simulated failure for task task2
2024-01-01 10:00:01,500 - ERROR - Task task2 failed: Simulated failure for task task2
2024-01-01 10:00:02,000 - INFO - Task task1 completed successfully: TaskResult(...)
2024-01-01 10:00:03,000 - INFO - Task task3 completed successfully: TaskResult(...)

Final Task Status:
2024-01-01 10:00:03,000 - INFO - task1: completed
2024-01-01 10:00:03,000 - INFO - task2: failed
2024-01-01 10:00:03,000 - INFO - task3: completed
```

### Benefits of This Implementation

1. **Robust Error Handling**:
   - Graceful handling of exceptions at multiple levels
   - Detailed error logging and status tracking
   - Proper propagation of errors through futures

2. **Scalability**:
   - Can handle multiple concurrent tasks
   - Efficient resource management
   - Easy to extend with additional functionality

3. **Maintainability**:
   - Clear separation of concerns
   - Well-structured code with proper documentation
   - Easy to debug with comprehensive logging

4. **Flexibility**:
   - Configurable task parameters
   - Easy to add new task types
   - Customizable callback system

This implementation provides a solid foundation for building complex asynchronous applications with proper error handling and task management.

