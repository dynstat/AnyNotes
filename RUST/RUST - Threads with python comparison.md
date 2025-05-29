

A simple threading example in both Rust and Python where multiple threads increment a shared counter.

### 1. Basic Threading Example

#### Rust Version:
```rust
use std::thread;

fn main() {
    // Create a vector to hold thread handles
    let mut handles = vec![];

    // Spawn 3 threads
    for id in 0..3 {
        // Spawn a new thread
        let handle = thread::spawn(move || {
            println!("Thread {} is running", id);
            // Simulate some work
            thread::sleep(std::time::Duration::from_millis(1000));
            println!("Thread {} finished", id);
        });

        handles.push(handle);
    }

    // Wait for all threads to complete
    for handle in handles {
        handle.join().unwrap();
    }

    println!("All threads completed!");
}
```

#### Python Equivalent:
```python
import threading
import time

def worker(id: int):
    print(f"Thread {id} is running")
    # Simulate some work
    time.sleep(1)
    print(f"Thread {id} finished")

def main():
    # Create a list to hold threads
    threads = []

    # Create 3 threads
    for i in range(3):
        thread = threading.Thread(target=worker, args=(i,))
        threads.append(thread)
        thread.start()

    # Wait for all threads to complete
    for thread in threads:
        thread.join()

    print("All threads completed!")

if __name__ == "__main__":
    main()
```

### 2. Shared Counter Example

#### Rust Version:
```rust
use std::sync::{Arc, Mutex};
use std::thread;

fn main() {
    // Create a shared counter
    let counter = Arc::new(Mutex::new(0));
    let mut handles = vec![];

    // Spawn 5 threads
    for thread_id in 0..5 {
        // Clone the Arc for this thread
        let counter_clone = Arc::clone(&counter);

        let handle = thread::spawn(move || {
            // Lock the mutex and increment the counter
            let mut num = counter_clone.lock().unwrap();
            *num += 1;
            println!("Thread {} incremented counter to {}", thread_id, *num);
        });

        handles.push(handle);
    }

    // Wait for all threads
    for handle in handles {
        handle.join().unwrap();
    }

    // Print final value
    println!("Final counter value: {}", *counter.lock().unwrap());
}
```

#### Python Equivalent:
```python
import threading
import time

class Counter:
    def __init__(self):
        self.value = 0
        self.lock = threading.Lock()

def increment_counter(counter: Counter, thread_id: int):
    with counter.lock:
        counter.value += 1
        print(f"Thread {thread_id} incremented counter to {counter.value}")

def main():
    # Create shared counter
    counter = Counter()
    threads = []

    # Create 5 threads
    for i in range(5):
        thread = threading.Thread(
            target=increment_counter, 
            args=(counter, i)
        )
        threads.append(thread)
        thread.start()

    # Wait for all threads
    for thread in threads:
        thread.join()

    print(f"Final counter value: {counter.value}")

if __name__ == "__main__":
    main()
```

### 3. Producer-Consumer Example

#### Rust Version:
```rust
use std::sync::{Arc, Mutex};
use std::thread;
use std::collections::VecDeque;
use std::time::Duration;

fn main() {
    // Shared queue
    let queue = Arc::new(Mutex::new(VecDeque::new()));
    let queue_clone = Arc::clone(&queue);

    // Producer thread
    let producer = thread::spawn(move || {
        for i in 0..5 {
            {
                let mut q = queue_clone.lock().unwrap();
                q.push_back(i);
                println!("Produced: {}", i);
            }
            thread::sleep(Duration::from_millis(100));
        }
    });

    // Consumer thread
    let consumer = thread::spawn(move || {
        for _ in 0..5 {
            {
                let mut q = queue.lock().unwrap();
                if let Some(item) = q.pop_front() {
                    println!("Consumed: {}", item);
                }
            }
            thread::sleep(Duration::from_millis(150));
        }
    });

    // Wait for both threads
    producer.join().unwrap();
    consumer.join().unwrap();
}
```

#### Python Equivalent:
```python
import threading
import queue
import time

def producer(q: queue.Queue):
    for i in range(5):
        q.put(i)
        print(f"Produced: {i}")
        time.sleep(0.1)

def consumer(q: queue.Queue):
    for _ in range(5):
        item = q.get()
        print(f"Consumed: {item}")
        time.sleep(0.15)

def main():
    # Create a shared queue
    shared_queue = queue.Queue()

    # Create producer and consumer threads
    producer_thread = threading.Thread(
        target=producer, 
        args=(shared_queue,)
    )
    consumer_thread = threading.Thread(
        target=consumer, 
        args=(shared_queue,)
    )

    # Start threads
    producer_thread.start()
    consumer_thread.start()

    # Wait for completion
    producer_thread.join()
    consumer_thread.join()

if __name__ == "__main__":
    main()
```

### Key Differences to Note:

1. **Thread Creation**:
   - Rust uses `thread::spawn`
   - Python uses `threading.Thread`

2. **Shared State**:
   - Rust uses `Arc<Mutex<T>>` for thread-safe sharing
   - Python uses built-in synchronization primitives like `Lock()`

3. **Error Handling**:
   - Rust requires explicit handling with `unwrap()` or proper error handling
   - Python uses exceptions

4. **Memory Safety**:
   - Rust enforces memory safety at compile time
   - Python handles memory management automatically

5. **Thread Joining**:
   - Both languages use `.join()` to wait for threads
   - Rust requires unwrapping the result

The main conceptual differences are:
- Rust's ownership system requires explicit sharing with `Arc`
- Rust's thread safety is enforced at compile time
- Python's GIL (Global Interpreter Lock) affects true parallelism
- Python's threading is generally simpler but less performant for CPU-bound tasks
