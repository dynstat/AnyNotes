# STL Container Adaptors

Container adaptors in the STL provide a different interface for sequence containers. They are not full container classes themselves but wrappers around other containers that provide a specialized interface.

## Stack (`std::stack`)

### Description
A Last-In-First-Out (LIFO) data structure that allows operations only at one end (top) of the container.

### Header
```cpp
#include <stack>
```

### Template Parameters
```cpp
template < class T, class Container = std::deque<T> >
class stack;
```
- `T`: Type of the elements
- `Container`: Type of the underlying container (default is std::deque)

### Common Methods

| Method | Description | Parameters | Return Type | Time Complexity |
|--------|-------------|------------|-------------|-----------------|
| `empty()` | Check if empty | None | bool | O(1) |
| `size()` | Get number of elements | None | size_type | O(1) |
| `top()` | Access top element | None | T& or const T& | O(1) |
| `push(const value_type& value)` | Insert element at top | value | void | O(1) |
| `push(value_type&& value)` | Insert element at top (move) | value | void | O(1) |
| `emplace(Args&&... args)` | Construct and insert element at top | args | void | O(1) |
| `pop()` | Remove top element | None | void | O(1) |
| `swap(stack& other)` | Swap contents | other | void | O(1) |

### Example
```cpp
#include <iostream>
#include <stack>
#include <vector>
#include <string>

int main() {
    // Declaration and initialization
    std::stack<int> stack1;                     // Default uses std::deque as container
    std::stack<int, std::vector<int>> stack2;   // Using std::vector as container
    
    // Pushing elements
    stack1.push(10);
    stack1.push(20);
    stack1.push(30);
    
    // Size and empty
    std::cout << "Size: " << stack1.size() << std::endl;  // Outputs: 3
    std::cout << "Is empty? " << (stack1.empty() ? "Yes" : "No") << std::endl;  // Outputs: No
    
    // Accessing top element
    std::cout << "Top element: " << stack1.top() << std::endl;  // Outputs: 30
    
    // Modifying top element
    stack1.top() = 35;
    std::cout << "Modified top: " << stack1.top() << std::endl;  // Outputs: 35
    
    // Using emplace (constructs in-place)
    std::stack<std::string> name_stack;
    name_stack.emplace("Alice");
    name_stack.emplace("Bob");
    
    // Stack with custom container
    for (int i = 1; i <= 5; ++i) {
        stack2.push(i * 10);
    }
    
    // Popping and processing elements
    std::cout << "Popping elements from stack1: ";
    while (!stack1.empty()) {
        std::cout << stack1.top() << " ";
        stack1.pop();
    }
    std::cout << std::endl;  // Outputs: 35 20 10
    
    std::cout << "Popping elements from stack2: ";
    while (!stack2.empty()) {
        std::cout << stack2.top() << " ";
        stack2.pop();
    }
    std::cout << std::endl;  // Outputs: 50 40 30 20 10
    
    // Swapping stacks
    std::stack<int> s1, s2;
    s1.push(1); s1.push(2); s1.push(3);
    s2.push(10); s2.push(20);
    
    s1.swap(s2);
    
    std::cout << "s1 top after swap: " << s1.top() << std::endl;  // Outputs: 20
    std::cout << "s2 top after swap: " << s2.top() << std::endl;  // Outputs: 3
    
    return 0;
}
```

### Implementation Details
- No iterators are provided
- Elements can only be accessed from the top
- Typical implementation for call stack, expression evaluation, and depth-first search
- The underlying container must support:
  - `back()`
  - `push_back()`
  - `pop_back()`
  - `empty()`
  - `size()`

## Queue (`std::queue`)

### Description
A First-In-First-Out (FIFO) data structure that allows insertion at one end (back) and removal from the other end (front).

### Header
```cpp
#include <queue>
```

### Template Parameters
```cpp
template < class T, class Container = std::deque<T> >
class queue;
```
- `T`: Type of the elements
- `Container`: Type of the underlying container (default is std::deque)

### Common Methods

| Method | Description | Parameters | Return Type | Time Complexity |
|--------|-------------|------------|-------------|-----------------|
| `empty()` | Check if empty | None | bool | O(1) |
| `size()` | Get number of elements | None | size_type | O(1) |
| `front()` | Access first element | None | T& or const T& | O(1) |
| `back()` | Access last element | None | T& or const T& | O(1) |
| `push(const value_type& value)` | Insert element at back | value | void | O(1) |
| `push(value_type&& value)` | Insert element at back (move) | value | void | O(1) |
| `emplace(Args&&... args)` | Construct and insert element at back | args | void | O(1) |
| `pop()` | Remove first element | None | void | O(1) |
| `swap(queue& other)` | Swap contents | other | void | O(1) |

### Example
```cpp
#include <iostream>
#include <queue>
#include <list>
#include <string>

int main() {
    // Declaration and initialization
    std::queue<int> queue1;                   // Default uses std::deque as container
    std::queue<int, std::list<int>> queue2;   // Using std::list as container
    
    // Pushing elements
    queue1.push(10);
    queue1.push(20);
    queue1.push(30);
    
    // Size and empty
    std::cout << "Size: " << queue1.size() << std::endl;  // Outputs: 3
    std::cout << "Is empty? " << (queue1.empty() ? "Yes" : "No") << std::endl;  // Outputs: No
    
    // Accessing front and back elements
    std::cout << "Front element: " << queue1.front() << std::endl;  // Outputs: 10
    std::cout << "Back element: " << queue1.back() << std::endl;    // Outputs: 30
    
    // Modifying front and back elements
    queue1.front() = 15;
    queue1.back() = 35;
    
    std::cout << "Modified front: " << queue1.front() << std::endl;  // Outputs: 15
    std::cout << "Modified back: " << queue1.back() << std::endl;    // Outputs: 35
    
    // Using emplace (constructs in-place)
    std::queue<std::string> name_queue;
    name_queue.emplace("Alice");
    name_queue.emplace("Bob");
    
    // Queue with custom container
    for (int i = 1; i <= 5; ++i) {
        queue2.push(i * 10);
    }
    
    // Popping and processing elements
    std::cout << "Popping elements from queue1: ";
    while (!queue1.empty()) {
        std::cout << queue1.front() << " ";
        queue1.pop();
    }
    std::cout << std::endl;  // Outputs: 15 20 35
    
    std::cout << "Popping elements from queue2: ";
    while (!queue2.empty()) {
        std::cout << queue2.front() << " ";
        queue2.pop();
    }
    std::cout << std::endl;  // Outputs: 10 20 30 40 50
    
    // Swapping queues
    std::queue<int> q1, q2;
    q1.push(1); q1.push(2); q1.push(3);
    q2.push(10); q2.push(20);
    
    q1.swap(q2);
    
    std::cout << "q1 front after swap: " << q1.front() << std::endl;  // Outputs: 10
    std::cout << "q2 front after swap: " << q2.front() << std::endl;  // Outputs: 1
    
    return 0;
}
```

### Implementation Details
- No iterators are provided
- Elements can only be accessed from the front and back
- Typical implementation for breadth-first search, print spoolers, and messaging systems
- The underlying container must support:
  - `front()`
  - `back()`
  - `push_back()`
  - `pop_front()`
  - `empty()`
  - `size()`

## Priority Queue (`std::priority_queue`)

### Description
A container adaptor that provides constant time lookup of the largest (by default) element. Elements are internally arranged in a heap data structure.

### Header
```cpp
#include <queue>
```

### Template Parameters
```cpp
template < class T,
           class Container = std::vector<T>,
           class Compare = std::less<typename Container::value_type> >
class priority_queue;
```
- `T`: Type of the elements
- `Container`: Type of the underlying container (default is std::vector)
- `Compare`: Comparison function (default uses `operator<`)

### Common Methods

| Method | Description | Parameters | Return Type | Time Complexity |
|--------|-------------|------------|-------------|-----------------|
| `empty()` | Check if empty | None | bool | O(1) |
| `size()` | Get number of elements | None | size_type | O(1) |
| `top()` | Access top element | None | const T& | O(1) |
| `push(const value_type& value)` | Insert element | value | void | O(log n) |
| `push(value_type&& value)` | Insert element (move) | value | void | O(log n) |
| `emplace(Args&&... args)` | Construct and insert element | args | void | O(log n) |
| `pop()` | Remove top element | None | void | O(log n) |
| `swap(priority_queue& other)` | Swap contents | other | void | O(1) |

### Example
```cpp
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <functional>  // For std::greater

struct Task {
    std::string name;
    int priority;
    
    // Define comparison operator for ordering
    bool operator<(const Task& other) const {
        // Lower priority value means higher priority
        return priority > other.priority;
    }
};

// Overload insertion operator for Task
std::ostream& operator<<(std::ostream& os, const Task& task) {
    return os << "Task(" << task.name << ", priority=" << task.priority << ")";
}

int main() {
    // Declaration and initialization
    std::priority_queue<int> pq1;                         // Max heap (default)
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq2;  // Min heap
    
    // Pushing elements
    pq1.push(30);
    pq1.push(10);
    pq1.push(50);
    pq1.push(20);
    
    // Size and empty
    std::cout << "Size: " << pq1.size() << std::endl;  // Outputs: 4
    std::cout << "Is empty? " << (pq1.empty() ? "Yes" : "No") << std::endl;  // Outputs: No
    
    // Accessing top element
    std::cout << "Top element (max): " << pq1.top() << std::endl;  // Outputs: 50
    
    // Pushing elements into min heap
    pq2.push(30);
    pq2.push(10);
    pq2.push(50);
    pq2.push(20);
    
    std::cout << "Min heap top: " << pq2.top() << std::endl;  // Outputs: 10
    
    // Using emplace (constructs in-place)
    std::priority_queue<Task> task_queue;
    task_queue.emplace(Task{"Urgent Task", 1});
    task_queue.emplace(Task{"Regular Task", 2});
    task_queue.emplace(Task{"Low Priority Task", 3});
    
    // Popping and processing elements from max heap
    std::cout << "Popping elements from max heap: ";
    while (!pq1.empty()) {
        std::cout << pq1.top() << " ";
        pq1.pop();
    }
    std::cout << std::endl;  // Outputs: 50 30 20 10
    
    // Popping and processing elements from min heap
    std::cout << "Popping elements from min heap: ";
    while (!pq2.empty()) {
        std::cout << pq2.top() << " ";
        pq2.pop();
    }
    std::cout << std::endl;  // Outputs: 10 20 30 50
    
    // Popping tasks by priority
    std::cout << "Processing tasks by priority:" << std::endl;
    while (!task_queue.empty()) {
        std::cout << "  " << task_queue.top() << std::endl;
        task_queue.pop();
    }
    
    // Custom comparison function using lambda
    auto cmp = [](int left, int right) { return (left % 3) < (right % 3); };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> pq3(cmp);
    
    pq3.push(2);  // 2 % 3 = 2
    pq3.push(1);  // 1 % 3 = 1
    pq3.push(5);  // 5 % 3 = 2
    pq3.push(6);  // 6 % 3 = 0
    pq3.push(3);  // 3 % 3 = 0
    
    std::cout << "Custom comparison priority queue: ";
    while (!pq3.empty()) {
        std::cout << pq3.top() << " ";
        pq3.pop();
    }
    std::cout << std::endl;  // Will order based on remainder when divided by 3
    
    return 0;
}
```

### Implementation Details
- No iterators are provided
- Elements are arranged in a heap
- By default, the largest element is at the top (max heap)
- Can be configured as a min heap with a different comparison function
- Top element can be accessed in constant time
- Insertion and removal operations take logarithmic time
- The underlying container must support:
  - random access iterators
  - `front()`
  - `push_back()`
  - `pop_back()`
  - `empty()`
  - `size()`

## Custom Container Adaptor Implementation

Container adaptors can be implemented with any container that provides the required operations. Here's an example of implementing a stack with different underlying containers:

```cpp
#include <iostream>
#include <stack>
#include <vector>
#include <list>
#include <deque>
#include <chrono>
#include <functional>
#include <string>

// Helper function to measure time
template<typename Func>
long long measure_time(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

// Test function for stacks with different containers
void test_stack_with_container(const std::string& container_name, 
                              std::function<void()> push_test,
                              std::function<void()> pop_test) {
    std::cout << "Testing stack with " << container_name << ":\n";
    
    long long push_time = measure_time(push_test);
    std::cout << "  Push time: " << push_time << " μs\n";
    
    long long pop_time = measure_time(pop_test);
    std::cout << "  Pop time: " << pop_time << " μs\n";
    
    std::cout << "  Total time: " << (push_time + pop_time) << " μs\n\n";
}

int main() {
    const int NUM_OPERATIONS = 1000000;
    
    // Test with std::deque (default)
    test_stack_with_container("std::deque",
        [NUM_OPERATIONS]() {
            std::stack<int> s;
            for (int i = 0; i < NUM_OPERATIONS; ++i) {
                s.push(i);
            }
        },
        [NUM_OPERATIONS]() {
            std::stack<int> s;
            for (int i = 0; i < NUM_OPERATIONS; ++i) {
                s.push(i);
            }
            for (int i = 0; i < NUM_OPERATIONS; ++i) {
                s.pop();
            }
        }
    );
    
    // Test with std::vector
    test_stack_with_container("std::vector",
        [NUM_OPERATIONS]() {
            std::stack<int, std::vector<int>> s;
            for (int i = 0; i < NUM_OPERATIONS; ++i) {
                s.push(i);
            }
        },
        [NUM_OPERATIONS]() {
            std::stack<int, std::vector<int>> s;
            for (int i = 0; i < NUM_OPERATIONS; ++i) {
                s.push(i);
            }
            for (int i = 0; i < NUM_OPERATIONS; ++i) {
                s.pop();
            }
        }
    );
    
    // Test with std::list
    test_stack_with_container("std::list",
        [NUM_OPERATIONS]() {
            std::stack<int, std::list<int>> s;
            for (int i = 0; i < NUM_OPERATIONS; ++i) {
                s.push(i);
            }
        },
        [NUM_OPERATIONS]() {
            std::stack<int, std::list<int>> s;
            for (int i = 0; i < NUM_OPERATIONS; ++i) {
                s.push(i);
            }
            for (int i = 0; i < NUM_OPERATIONS; ++i) {
                s.pop();
            }
        }
    );
    
    return 0;
}
```

### Performance Considerations

When choosing a container adaptor, consider:

1. **Stack (`std::stack`)**
   - Use when you need LIFO (Last-In-First-Out) behavior
   - Common applications: function call stack, expression evaluation, backtracking algorithms
   - Default container (std::deque) works well for most cases
   - Use std::vector as underlying container if memory usage is a concern and you know the approximate size in advance

2. **Queue (`std::queue`)**
   - Use when you need FIFO (First-In-First-Out) behavior
   - Common applications: breadth-first search, print spoolers, task scheduling
   - Default container (std::deque) works well for most cases
   - Use std::list if you need constant-time insertion and removal with many elements

3. **Priority Queue (`std::priority_queue`)**
   - Use when you need to process elements based on priority
   - Common applications: Dijkstra's algorithm, event-driven simulation, task scheduling with priorities
   - Default container (std::vector) with a binary heap implementation works efficiently for most cases
   - Min heap vs. max heap depends on your specific requirements
   - Custom comparators allow for flexible prioritization strategies