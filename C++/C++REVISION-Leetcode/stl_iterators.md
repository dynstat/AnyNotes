# STL Iterators

Iterators in the STL are objects that provide a way to traverse through container elements in a sequential manner, abstracting away the underlying implementation details of the container.

## Iterator Categories

STL defines a hierarchy of iterator categories, each with different capabilities:

| Iterator Category | Description | Forward Traversal | Backward Traversal | Random Access | Read | Write | Containers |
|-------------------|-------------|-------------------|---------------------|---------------|------|-------|------------|
| Input Iterator | Read-only, single-pass | Yes | No | No | Yes | No | Stream iterators |
| Output Iterator | Write-only, single-pass | Yes | No | No | No | Yes | Stream iterators |
| Forward Iterator | Multi-pass, forward-only | Yes | No | No | Yes | Yes | `forward_list` |
| Bidirectional Iterator | Multi-pass, both directions | Yes | Yes | No | Yes | Yes | `list`, `set`, `multiset`, `map`, `multimap` |
| Random Access Iterator | Multi-pass, random access | Yes | Yes | Yes | Yes | Yes | `vector`, `deque`, `array` |
| Contiguous Iterator (C++20) | Elements in contiguous memory | Yes | Yes | Yes | Yes | Yes | `vector`, `array`, `string` |

## Common Iterator Operations

| Operation | Description | Input | Output | Forward | Bidirectional | Random Access |
|-----------|-------------|-------|--------|---------|---------------|---------------|
| `*it` | Dereference | ✓ | ✓ | ✓ | ✓ | ✓ |
| `it->member` | Member access | ✓ | ✗ | ✓ | ✓ | ✓ |
| `++it`, `it++` | Increment | ✓ | ✓ | ✓ | ✓ | ✓ |
| `--it`, `it--` | Decrement | ✗ | ✗ | ✗ | ✓ | ✓ |
| `it + n`, `it - n` | Offset | ✗ | ✗ | ✗ | ✗ | ✓ |
| `it += n`, `it -= n` | Compound assignment | ✗ | ✗ | ✗ | ✗ | ✓ |
| `it1 - it2` | Distance | ✗ | ✗ | ✗ | ✗ | ✓ |
| `it1[n]` | Subscript | ✗ | ✗ | ✗ | ✗ | ✓ |
| `it1 < it2` | Comparison | ✗ | ✗ | ✗ | ✗ | ✓ |

## Iterator Types in Containers

Each STL container provides several iterator types:

- `container::iterator`: Mutable iterator for traversing elements
- `container::const_iterator`: Immutable iterator (can't modify elements)
- `container::reverse_iterator`: Mutable iterator that traverses in reverse
- `container::const_reverse_iterator`: Immutable reverse iterator

## Examples

### Basic Iterator Usage

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>

int main() {
    // Vector iterator (Random Access)
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    std::cout << "Vector using iterator:" << std::endl;
    for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";  // Dereferencing iterator
        *it *= 2;                 // Modifying element
    }
    std::cout << std::endl;
    
    // Using auto for iterator type (C++11)
    std::cout << "Vector after modification:" << std::endl;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";  // Outputs: 2 4 6 8 10
    }
    std::cout << std::endl;
    
    // Const iterator
    std::cout << "Using const_iterator:" << std::endl;
    for (std::vector<int>::const_iterator it = vec.cbegin(); it != vec.cend(); ++it) {
        std::cout << *it << " ";
        // *it *= 2;  // Error: can't modify through const_iterator
    }
    std::cout << std::endl;
    
    // Reverse iterator
    std::cout << "Using reverse_iterator:" << std::endl;
    for (std::vector<int>::reverse_iterator it = vec.rbegin(); it != vec.rend(); ++it) {
        std::cout << *it << " ";  // Outputs: 10 8 6 4 2
    }
    std::cout << std::endl;
    
    // List iterator (Bidirectional)
    std::list<int> lst = {10, 20, 30, 40, 50};
    
    std::cout << "List using iterator:" << std::endl;
    for (std::list<int>::iterator it = lst.begin(); it != lst.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Map iterator (Bidirectional)
    std::map<std::string, int> map = {
        {"apple", 5},
        {"banana", 8},
        {"orange", 10}
    };
    
    std::cout << "Map using iterator:" << std::endl;
    for (std::map<std::string, int>::iterator it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
        it->second += 1;  // Modify the value (can't modify the key)
    }
    
    // Set iterator (Bidirectional)
    std::set<int> set = {5, 3, 1, 4, 2};
    
    std::cout << "Set using iterator:" << std::endl;
    for (std::set<int>::iterator it = set.begin(); it != set.end(); ++it) {
        std::cout << *it << " ";  // Outputs: 1 2 3 4 5 (sorted)
        // *it = 10;  // Error: can't modify set elements through iterator
    }
    std::cout << std::endl;
    
    return 0;
}
```

### Range-based for Loop (C++11)

```cpp
#include <iostream>
#include <vector>
#include <map>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // Range-based for loop (uses iterators internally)
    std::cout << "Vector using range-based for:" << std::endl;
    for (int& val : vec) {  // Reference allows modification
        std::cout << val << " ";
        val *= 2;
    }
    std::cout << std::endl;
    
    std::cout << "Vector after modification:" << std::endl;
    for (const int& val : vec) {  // Const reference prevents modification
        std::cout << val << " ";  // Outputs: 2 4 6 8 10
    }
    std::cout << std::endl;
    
    // Range-based for with map
    std::map<std::string, int> map = {
        {"apple", 5},
        {"banana", 8},
        {"orange", 10}
    };
    
    std::cout << "Map using range-based for:" << std::endl;
    for (const auto& [key, value] : map) {  // Structured binding (C++17)
        std::cout << key << ": " << value << std::endl;
    }
    
    return 0;
}
```

### Iterator Functions

```cpp
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // std::advance - advance an iterator by n positions
    auto it = vec.begin();
    std::advance(it, 5);  // Move iterator 5 positions forward
    std::cout << "After advance(5): " << *it << std::endl;  // Outputs: 6
    
    // std::distance - count elements between two iterators
    auto it2 = vec.begin();
    auto it3 = vec.begin() + 5;
    std::cout << "Distance: " << std::distance(it2, it3) << std::endl;  // Outputs: 5
    
    // std::next - get an iterator n positions forward
    auto it4 = std::next(vec.begin(), 3);
    std::cout << "next(begin, 3): " << *it4 << std::endl;  // Outputs: 4
    
    // std::prev - get an iterator n positions backward
    auto it5 = std::prev(vec.end(), 2);
    std::cout << "prev(end, 2): " << *it5 << std::endl;  // Outputs: 9
    
    // std::begin and std::end (C++11) - can be used with arrays
    int arr[] = {11, 22, 33, 44, 55};
    auto arr_begin = std::begin(arr);
    auto arr_end = std::end(arr);
    
    std::cout << "Array using std::begin/end:" << std::endl;
    for (auto it = arr_begin; it != arr_end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

### Stream Iterators

```cpp
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <string>

int main() {
    // istream_iterator for reading from input stream
    std::cout << "Enter numbers (non-number to stop):" << std::endl;
    std::istream_iterator<int> input_begin(std::cin);
    std::istream_iterator<int> input_end;  // Default constructor creates end iterator
    
    std::vector<int> numbers(input_begin, input_end);
    
    // ostream_iterator for writing to output stream
    std::cout << "You entered: ";
    std::ostream_iterator<int> output(std::cout, " ");
    std::copy(numbers.begin(), numbers.end(), output);
    std::cout << std::endl;
    
    // Using with algorithms
    std::cout << "Doubled values: ";
    std::transform(numbers.begin(), numbers.end(), output, 
                  [](int n) { return n * 2; });
    std::cout << std::endl;
    
    // File stream iterators
    std::vector<std::string> words = {"Hello", "world", "from", "C++", "iterators"};
    
    // Writing to a file using ostream_iterator
    std::ofstream outFile("words.txt");
    if (outFile.is_open()) {
        std::ostream_iterator<std::string> file_it(outFile, "\n");
        std::copy(words.begin(), words.end(), file_it);
        outFile.close();
        std::cout << "Data written to words.txt" << std::endl;
    }
    
    // Reading from a file using istream_iterator
    std::ifstream inFile("words.txt");
    if (inFile.is_open()) {
        std::istream_iterator<std::string> file_it(inFile);
        std::istream_iterator<std::string> eos;
        std::cout << "Data read from words.txt:" << std::endl;
        std::copy(file_it, eos, std::ostream_iterator<std::string>(std::cout, " "));
        std::cout << std::endl;
        inFile.close();
    }
    
    return 0;
}
```

### Custom Iterator

```cpp
#include <iostream>
#include <iterator>
#include <cstddef>

// A simple circular buffer implementation with custom iterator
template <typename T, size_t Size>
class CircularBuffer {
private:
    T data[Size];
    size_t head = 0;
    size_t tail = 0;
    size_t count = 0;

public:
    // Custom iterator implementation
    class Iterator {
    private:
        CircularBuffer* buffer;
        size_t index;
        size_t remaining;
        
    public:
        // Iterator traits
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        // Constructor
        Iterator(CircularBuffer* buf, size_t idx, size_t rem) 
            : buffer(buf), index(idx), remaining(rem) {}
        
        // Dereference operator
        reference operator*() const {
            return buffer->data[index];
        }
        
        // Member access operator
        pointer operator->() const {
            return &(buffer->data[index]);
        }
        
        // Prefix increment
        Iterator& operator++() {
            index = (index + 1) % Size;
            --remaining;
            return *this;
        }
        
        // Postfix increment
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        // Equality comparison
        bool operator==(const Iterator& other) const {
            return buffer == other.buffer && 
                   (index == other.index || remaining == 0 || other.remaining == 0);
        }
        
        // Inequality comparison
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };
    
    // Add an element to the buffer
    void push(const T& value) {
        data[tail] = value;
        tail = (tail + 1) % Size;
        
        if (count == Size) {
            head = (head + 1) % Size;  // Overwrite oldest element
        } else {
            ++count;
        }
    }
    
    // Get an element from the buffer
    T pop() {
        if (count == 0) {
            throw std::runtime_error("Buffer is empty");
        }
        
        T value = data[head];
        head = (head + 1) % Size;
        --count;
        return value;
    }
    
    // Check if buffer is empty
    bool empty() const {
        return count == 0;
    }
    
    // Check if buffer is full
    bool full() const {
        return count == Size;
    }
    
    // Get current number of elements
    size_t size() const {
        return count;
    }
    
    // Iterator functions
    Iterator begin() {
        return Iterator(this, head, count);
    }
    
    Iterator end() {
        return Iterator(this, (head + count) % Size, 0);
    }
};

int main() {
    CircularBuffer<int, 5> buffer;
    
    // Fill the buffer
    for (int i = 1; i <= 7; ++i) {
        buffer.push(i);
        std::cout << "Pushed: " << i << ", Buffer size: " << buffer.size() << std::endl;
    }
    
    // Iterate through buffer using our custom iterator
    std::cout << "Buffer contents: ";
    for (auto value : buffer) {
        std::cout << value << " ";  // Outputs: 3 4 5 6 7 (oldest elements were overwritten)
    }
    std::cout << std::endl;
    
    // Use with STL algorithms
    std::cout << "Sum of buffer elements: ";
    int sum = 0;
    for (auto it = buffer.begin(); it != buffer.end(); ++it) {
        sum += *it;
    }
    std::cout << sum << std::endl;
    
    return 0;
}
```

## Iterator Invalidation

Iterators can become invalid when container operations modify the container. Rules vary by container:

| Container | Invalidation Conditions |
|-----------|-------------------------|
| vector | Any operation that changes capacity invalidates all iterators. Insertion/removal invalidates iterators at or after the point of insertion/removal. |
| deque | Any insertion/removal invalidates all iterators. |
| list | Only iterators pointing to removed elements are invalidated. |
| forward_list | Only iterators pointing to removed elements are invalidated. |
| set, map, multiset, multimap | Only iterators pointing to removed elements are invalidated. |
| unordered_set, unordered_map, unordered_multiset, unordered_multimap | Any rehashing invalidates all iterators. |

Example demonstrating iterator invalidation:

```cpp
#include <iostream>
#include <vector>
#include <list>

int main() {
    // Vector iterator invalidation
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    std::cout << "Vector before: ";
    for (int val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // This can cause iterator invalidation
    auto it = vec.begin() + 2;  // Points to 3
    std::cout << "Iterator points to: " << *it << std::endl;
    
    vec.push_back(6);  // May cause reallocation and invalidate iterators
    // Accessing invalidated iterator could cause undefined behavior
    // std::cout << "After push_back, iterator points to: " << *it << std::endl;  // Unsafe!
    
    // List iterator invalidation (more stable)
    std::list<int> lst = {1, 2, 3, 4, 5};
    
    auto lst_it1 = lst.begin();  // Points to 1
    auto lst_it2 = std::next(lst_it1, 2);  // Points to 3
    auto lst_it3 = std::next(lst_it2, 2);  // Points to 5
    
    std::cout << "List iterators point to: " 
              << *lst_it1 << ", " << *lst_it2 << ", " << *lst_it3 << std::endl;
    
    lst.push_back(6);  // Doesn't invalidate iterators
    std::cout << "After push_back, list iterators still point to: " 
              << *lst_it1 << ", " << *lst_it2 << ", " << *lst_it3 << std::endl;
    
    lst.erase(lst_it2);  // Invalidates only lst_it2
    // std::cout << "After erase: " << *lst_it2 << std::endl;  // Unsafe!
    std::cout << "After erase, other iterators still valid: " 
              << *lst_it1 << ", " << *lst_it3 << std::endl;
    
    return 0;
}
```

## Best Practices

1. **Prefer iterators to raw pointers** for STL containers.
2. **Use auto** to simplify iterator declarations when the type is clear from context.
3. **Use const_iterator** when you don't need to modify elements.
4. **Use range-based for loops** when possible for cleaner code.
5. **Be aware of iterator invalidation** rules for each container.
6. **Use algorithms with iterators** rather than manual loops for better performance and readability.
7. **Check if an iterator is valid** before dereferencing it.
8. **Store the end() iterator** once rather than calling it repeatedly in a loop condition.
9. **Use pre-increment (++it)** rather than post-increment (it++) when possible for better performance.
10. **Remember that iterators are not thread-safe** - concurrent modification requires proper synchronization.