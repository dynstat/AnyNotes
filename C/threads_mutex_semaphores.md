# Understanding Threads, Mutexes, Semaphores, Fork, and Related System Calls in C

In C programming, managing **multiprocessing** and **multithreading** is crucial for creating efficient and responsive applications. This involves using various system calls and synchronization mechanisms such as threads, mutexes, semaphores, and `fork`. Below, we explore these concepts with examples and code tailored for different operating systems.

## Table of Contents

1. [Processes vs. Threads](#processes-vs-threads)
2. [Creating Processes with `fork`](#creating-processes-with-fork)
3. [Multithreading in C](#multithreading-in-c)
   - [POSIX Threads (pthreads) on Unix/Linux/macOS](#posix-threads-pthreads-on-unixlinuxmacos)
   - [Windows Threads](#windows-threads)
4. [Synchronization Mechanisms](#synchronization-mechanisms)
   - [Mutexes](#mutexes)
     - [Using Mutexes with pthreads](#using-mutexes-with-pthreads)
     - [Using Mutexes in Windows](#using-mutexes-in-windows)
   - [Semaphores](#semaphores)
     - [Using Semaphores with pthreads](#using-semaphores-with-pthreads)
     - [Using Semaphores in Windows](#using-semaphores-in-windows)
5. [Example Projects](#example-projects)

---

## Processes vs. Threads

- **Process**: An independent program with its own memory space.
- **Thread**: A lightweight unit of execution within a process, sharing the same memory space.

**Multiprocessing** involves running multiple processes concurrently, while **multithreading** involves multiple threads within the same process.

---

## Creating Processes with `fork`

The `fork()` system call is used in Unix-like operating systems to create a new process by duplicating the calling process.

### Example: Using `fork()` in Unix/Linux/macOS

```c
// unix_fork_example.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Hello from Child! PID: %d\n", getpid());
    } else {
        // Parent process
        printf("Hello from Parent! PID: %d, Child PID: %d\n", getpid(), pid);
    }

    return 0;
}
```

**Explanation:**

- `fork()` creates a new process.
- Child process receives `pid = 0`.
- Parent process receives the child's PID.

**Compilation:**

```bash
gcc unix_fork_example.c -o unix_fork_example
```

### Windows Equivalent

Windows does not have a native `fork()` system call. Instead, processes are created using the `CreateProcess` function.

```c
// windows_fork_example.c
#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // Command to execute (same executable)
    char cmd[] = "windows_fork_example.exe";

    if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed. Error: %lu\n", GetLastError());
        return 1;
    }

    printf("Parent process. PID: %lu\n", GetCurrentProcessId());
    printf("Child process created. PID: %lu\n", pi.dwProcessId);

    // Wait until child process exits
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
```

**Explanation:**

- `CreateProcess` is used to create a new process.
- Parameters include the executable name and process information structures.

**Compilation:**

Use a Windows-compatible compiler like MSVC:

```batch
cl /W4 windows_fork_example.c
```

---

## Multithreading in C

Multithreading allows concurrent execution within a single process. The implementation differs between Unix-like systems and Windows.

### POSIX Threads (pthreads) on Unix/Linux/macOS

POSIX Threads, commonly known as pthreads, is a standard for thread creation and synchronization on Unix-like systems.

#### Example: Creating and Running Threads with pthreads

```c
// pthread_example.c
#include <stdio.h>
#include <pthread.h>

void* thread_function(void* arg) {
    int thread_num = *((int*)arg);
    printf("Hello from Thread %d! ID: %lu\n", thread_num, pthread_self());
    return NULL;
}

int main() {
    pthread_t threads[2];
    int thread_args[2] = {1, 2};

    for (int i = 0; i < 2; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, &thread_args[i]) != 0) {
            perror("pthread_create failed");
            return 1;
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads completed.\n");
    return 0;
}
```

**Explanation:**

- `pthread_create` initializes a new thread.
- Each thread runs `thread_function`.
- `pthread_join` waits for thread completion.

**Compilation:**

```bash
gcc pthread_example.c -o pthread_example -pthread
```

### Windows Threads

Windows uses the Windows API for thread management.

#### Example: Creating and Running Threads in Windows

```c
// windows_thread_example.c
#include <windows.h>
#include <stdio.h>

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    int thread_num = *((int*)lpParam);
    printf("Hello from Thread %d! ID: %lu\n", thread_num, GetCurrentThreadId());
    return 0;
}

int main() {
    HANDLE threads[2];
    DWORD threadIds[2];
    int thread_args[2] = {1, 2};

    for (int i = 0; i < 2; i++) {
        threads[i] = CreateThread(
            NULL,               // default security attributes
            0,                  // default stack size
            ThreadFunction,     // thread function
            &thread_args[i],    // argument to thread function
            0,                  // default creation flags
            &threadIds[i]);     // receive thread identifier

        if (threads[i] == NULL) {
            printf("CreateThread failed. Error: %lu\n", GetLastError());
            return 1;
        }
    }

    // Wait for threads to finish
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    // Close thread handles
    for (int i = 0; i < 2; i++) {
        CloseHandle(threads[i]);
    }

    printf("All threads completed.\n");
    return 0;
}
```

**Explanation:**

- `CreateThread` initializes a new thread.
- Each thread runs `ThreadFunction`.
- `WaitForMultipleObjects` waits for all threads to complete.
- `CloseHandle` cleans up thread handles.

**Compilation:**

Use a Windows-compatible compiler like MSVC:

```batch
cl /W4 windows_thread_example.c
```

---

## Synchronization Mechanisms

When multiple threads or processes access shared resources, synchronization mechanisms prevent race conditions and ensure data integrity. Two common synchronization tools are **mutexes** and **semaphores**.

### Mutexes

A **mutex** (mutual exclusion) ensures that only one thread can access a resource at a time.

#### Using Mutexes with pthreads

```c
// pthread_mutex_example.c
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;
int shared_counter = 0;

void* increment_counter(void* arg) {
    pthread_mutex_lock(&mutex);
    shared_counter++;
    printf("Thread %lu incremented counter to %d\n", pthread_self(), shared_counter);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t threads[5];
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < 5; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
            perror("pthread_create failed");
            return 1;
        }
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    printf("Final counter value: %d\n", shared_counter);
    return 0;
}
```

**Explanation:**

- `pthread_mutex_init` initializes the mutex.
- `pthread_mutex_lock` and `pthread_mutex_unlock` control access.
- Ensures `shared_counter` is safely incremented.

**Compilation:**

```bash
gcc pthread_mutex_example.c -o pthread_mutex_example -pthread
```

#### Using Mutexes in Windows

```c
// windows_mutex_example.c
#include <windows.h>
#include <stdio.h>

HANDLE mutex;
int shared_counter = 0;

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    // Wait for the mutex
    WaitForSingleObject(mutex, INFINITE);

    // Critical section
    shared_counter++;
    printf("Thread %lu incremented counter to %d\n", GetCurrentThreadId(), shared_counter);

    // Release the mutex
    ReleaseMutex(mutex);
    return 0;
}

int main() {
    HANDLE threads[5];
    DWORD threadIds[5];

    // Create a mutex
    mutex = CreateMutex(NULL, FALSE, NULL);
    if (mutex == NULL) {
        printf("CreateMutex failed. Error: %lu\n", GetLastError());
        return 1;
    }

    // Create threads
    for (int i = 0; i < 5; i++) {
        threads[i] = CreateThread(
            NULL,
            0,
            ThreadFunction,
            NULL,
            0,
            &threadIds[i]);

        if (threads[i] == NULL) {
            printf("CreateThread failed. Error: %lu\n", GetLastError());
            return 1;
        }
    }

    // Wait for threads to finish
    WaitForMultipleObjects(5, threads, TRUE, INFINITE);

    // Close thread handles
    for (int i = 0; i < 5; i++) {
        CloseHandle(threads[i]);
    }

    // Close mutex handle
    CloseHandle(mutex);

    printf("Final counter value: %d\n", shared_counter);
    return 0;
}
```

**Explanation:**

- `CreateMutex` initializes the mutex.
- `WaitForSingleObject` acquires the mutex.
- `ReleaseMutex` releases the mutex.
- Ensures `shared_counter` is safely incremented.

**Compilation:**

Use a Windows-compatible compiler like MSVC:

```batch
cl /W4 windows_mutex_example.c
```

### Semaphores

A **semaphore** is a signaling mechanism that controls access based on a counter, allowing a specified number of threads to access a resource simultaneously.

#### Using Semaphores with pthreads

POSIX semaphores (`sem_t`) can be used for synchronization.

```c
// pthread_semaphore_example.c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semaphore;
int shared_resource = 0;

void* access_resource(void* arg) {
    sem_wait(&semaphore); // Decrement semaphore
    shared_resource++;
    printf("Thread %lu accessing resource. Value: %d\n", pthread_self(), shared_resource);
    sleep(1); // Simulate work
    sem_post(&semaphore); // Increment semaphore
    return NULL;
}

int main() {
    pthread_t threads[3];
    sem_init(&semaphore, 0, 2); // Initialize semaphore with value 2

    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, access_resource, NULL) != 0) {
            perror("pthread_create failed");
            return 1;
        }
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaphore);
    printf("Final shared_resource value: %d\n", shared_resource);
    return 0;
}
```

**Explanation:**

- `sem_init` initializes the semaphore with a counter (e.g., 2).
- `sem_wait` decrements the semaphore; blocks if counter is zero.
- `sem_post` increments the semaphore, releasing it.

**Compilation:**

```bash
gcc pthread_semaphore_example.c -o pthread_semaphore_example -pthread
```

#### Using Semaphores in Windows

Windows provides semaphore functions through the Windows API.

```c
// windows_semaphore_example.c
#include <windows.h>
#include <stdio.h>

HANDLE semaphore;
int shared_resource = 0;

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    // Wait for the semaphore (limit to 2 concurrent accesses)
    WaitForSingleObject(semaphore, INFINITE);

    // Critical section
    shared_resource++;
    printf("Thread %lu accessing resource. Value: %d\n", GetCurrentThreadId(), shared_resource);
    Sleep(1000); // Simulate work

    // Release the semaphore
    ReleaseSemaphore(semaphore, 1, NULL);
    return 0;
}

int main() {
    HANDLE threads[3];
    DWORD threadIds[3];

    // Create a semaphore with a maximum count of 2
    semaphore = CreateSemaphore(NULL, 2, 2, NULL);
    if (semaphore == NULL) {
        printf("CreateSemaphore failed. Error: %lu\n", GetLastError());
        return 1;
    }

    // Create threads
    for (int i = 0; i < 3; i++) {
        threads[i] = CreateThread(
            NULL,
            0,
            ThreadFunction,
            NULL,
            0,
            &threadIds[i]);

        if (threads[i] == NULL) {
            printf("CreateThread failed. Error: %lu\n", GetLastError());
            return 1;
        }
    }

    // Wait for threads to finish
    WaitForMultipleObjects(3, threads, TRUE, INFINITE);

    // Close thread handles
    for (int i = 0; i < 3; i++) {
        CloseHandle(threads[i]);
    }

    // Close semaphore handle
    CloseHandle(semaphore);

    printf("Final shared_resource value: %d\n", shared_resource);
    return 0;
}
```

**Explanation:**

- `CreateSemaphore` initializes the semaphore with a maximum count (e.g., 2).
- `WaitForSingleObject` acquires the semaphore.
- `ReleaseSemaphore` releases the semaphore.
- Allows up to two threads to access the resource simultaneously.

**Compilation:**

Use a Windows-compatible compiler like MSVC:

```batch
cl /W4 windows_semaphore_example.c
```

---

## Example Projects

### 1. Thread-Safe Counter using Mutex

**File Path:** `examples/thread_safe_counter.c`

```c:examples/thread_safe_counter.c
#include <stdio.h>
#include <pthread.h>
#include <windows.h> // Include only for Windows. Remove for Unix.

#define NUM_THREADS 5

#ifdef _WIN32
HANDLE mutex;
#else
pthread_mutex_t mutex;
#endif

int counter = 0;

void* increment(void* arg) {
#ifdef _WIN32
    WaitForSingleObject(mutex, INFINITE);
#else
    pthread_mutex_lock(&mutex);
#endif

    // Critical section
    counter++;
    printf("Thread %lu incremented counter to %d\n", 
#ifdef _WIN32
           GetCurrentThreadId(), counter);
    ReleaseMutex(mutex);
#else
           pthread_self(), counter);
    pthread_mutex_unlock(&mutex);
#endif

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

#ifdef _WIN32
    mutex = CreateMutex(NULL, FALSE, NULL);
    if (mutex == NULL) {
        printf("CreateMutex failed. Error: %lu\n", GetLastError());
        return 1;
    }
#else
    pthread_mutex_init(&mutex, NULL);
#endif

    // Create threads
    for(int i = 0; i < NUM_THREADS; i++) {
#ifdef _WIN32
        threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)increment, NULL, 0, NULL);
        if (threads[i] == NULL) {
            printf("CreateThread failed. Error: %lu\n", GetLastError());
            return 1;
        }
#else
        if(pthread_create(&threads[i], NULL, increment, NULL) != 0) {
            perror("pthread_create failed");
            return 1;
        }
#endif
    }

    // Wait for threads to finish
    for(int i = 0; i < NUM_THREADS; i++) {
#ifdef _WIN32
        WaitForSingleObject(threads[i], INFINITE);
        CloseHandle(threads[i]);
#else
        pthread_join(threads[i], NULL);
#endif
    }

#ifdef _WIN32
    CloseHandle(mutex);
#else
    pthread_mutex_destroy(&mutex);
#endif

    printf("Final counter value: %d\n", counter);
    return 0;
}
```

**Explanation:**

- Cross-platform implementation using conditional compilation.
- Uses mutexes to protect the shared `counter`.
- Supports both pthreads and Windows threads.

**Compilation:**

- **Unix/Linux/macOS:**

  ```bash
  gcc thread_safe_counter.c -o thread_safe_counter -pthread
  ```

- **Windows:**

  ```batch
  cl /W4 thread_safe_counter.c
  ```

### 2. Producer-Consumer using Semaphores

**File Path:** `examples/producer_consumer.c`

```c:examples/producer_consumer.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <windows.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

#ifdef _WIN32
HANDLE empty;
HANDLE full;
HANDLE mutex;
#else
sem_t empty;
sem_t full;
pthread_mutex_t mutex;
#endif

#ifdef _WIN32
DWORD WINAPI producer(void* arg) {
#else
void* producer(void* arg) {
#endif
    for(int i = 0; i < NUM_ITEMS; i++) {
#ifdef _WIN32
        WaitForSingleObject(empty, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
#else
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
#endif

        // Produce an item
        buffer[in] = i;
        printf("Produced: %d at %d\n", buffer[in], in);
        in = (in + 1) % BUFFER_SIZE;

#ifdef _WIN32
        ReleaseMutex(mutex);
        ReleaseSemaphore(full, 1, NULL);
#else
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
#endif

        Sleep(500); // Simulate time taken to produce
    }
    return 0;
}

#ifdef _WIN32
DWORD WINAPI consumer(void* arg) {
#else
void* consumer(void* arg) {
#endif
    for(int i = 0; i < NUM_ITEMS; i++) {
#ifdef _WIN32
        WaitForSingleObject(full, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
#else
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
#endif

        // Consume an item
        int item = buffer[out];
        printf("Consumed: %d from %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

#ifdef _WIN32
        ReleaseMutex(mutex);
        ReleaseSemaphore(empty, 1, NULL);
#else
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
#endif

        Sleep(800); // Simulate time taken to consume
    }
    return 0;
}

int main() {
#ifdef _WIN32
    empty = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
    full = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);
    mutex = CreateMutex(NULL, FALSE, NULL);
    if (empty == NULL || full == NULL || mutex == NULL) {
        printf("Semaphore or Mutex initialization failed.\n");
        return 1;
    }
#else
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
#endif

    pthread_t prod, cons;
#ifdef _WIN32
    HANDLE threads[2];
    threads[0] = CreateThread(NULL, 0, producer, NULL, 0, NULL);
    threads[1] = CreateThread(NULL, 0, consumer, NULL, 0, NULL);
    if (threads[0] == NULL || threads[1] == NULL) {
        printf("Thread creation failed.\n");
        return 1;
    }
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);
    CloseHandle(threads[0]);
    CloseHandle(threads[1]);
#else
    if(pthread_create(&prod, NULL, producer, NULL) != 0 ||
       pthread_create(&cons, NULL, consumer, NULL) != 0) {
        perror("Thread creation failed");
        return 1;
    }
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
#endif

#ifdef _WIN32
    CloseHandle(empty);
    CloseHandle(full);
    CloseHandle(mutex);
#else
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
#endif

    printf("Producer and Consumer have finished.\n");
    return 0;
}
```

**Explanation:**

- Implements the Producer-Consumer problem using semaphores and mutexes.
- Uses a circular buffer to store items.
- Synchronizes access using semaphores (`empty`, `full`) and a mutex.

**Compilation:**

- **Unix/Linux/macOS:**

  ```bash
  gcc producer_consumer.c -o producer_consumer -pthread
  ```

- **Windows:**

  ```batch
  cl /W4 producer_consumer.c
  ```


---

# References

- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/)
- [Windows Threading](https://docs.microsoft.com/en-us/windows/win32/procthread/about-threads)
- [Semaphore Synchronization](https://en.wikipedia.org/wiki/Semaphore_(programming))
- [Mutex Synchronization](https://en.wikipedia.org/wiki/Mutual_exclusion)