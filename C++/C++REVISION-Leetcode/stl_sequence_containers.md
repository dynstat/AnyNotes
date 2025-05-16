# STL Sequence Containers

Sequence containers in the STL store elements in a linear sequence, allowing precise control over the position of each element.

## Vector (`std::vector`)

### Description
A dynamic array that grows automatically as elements are added. Elements are stored contiguously in memory, allowing direct random access.

### Header
```cpp
#include <vector>
```

### Template Parameters
```cpp
template < class T, class Allocator = std::allocator<T> >
class vector;
```
- `T`: Type of the elements
- `Allocator`: Memory allocator to use (default is standard allocator)

### Constructors

| Constructor | Description | Parameters |
|-------------|-------------|------------|
| `vector()` | Default constructor | None |
| `vector(size_type count)` | Size constructor | count: number of elements |
| `vector(size_type count, const T& value)` | Fill constructor | count: number of elements, value: value to fill with |
| `vector(InputIt first, InputIt last)` | Range constructor | first, last: iterators defining range |
| `vector(const vector& other)` | Copy constructor | other: vector to copy |
| `vector(vector&& other)` | Move constructor | other: vector to move from |
| `vector(std::initializer_list<T> init)` | Initializer list constructor | init: initializer list |

### Common Methods

| Method | Description | Parameters | Return Type | Time Complexity |
|--------|-------------|------------|-------------|-----------------|
| `at(size_type pos)` | Access element with bounds checking | pos: position | T& or const T& | O(1) |
| `operator[](size_type pos)` | Access element | pos: position | T& or const T& | O(1) |
| `front()` | Access first element | None | T& or const T& | O(1) |
| `back()` | Access last element | None | T& or const T& | O(1) |
| `data()` | Get pointer to data | None | T* or const T* | O(1) |
| `begin()`, `end()` | Get iterators | None | iterator/const_iterator | O(1) |
| `empty()` | Check if empty | None | bool | O(1) |
| `size()` | Get number of elements | None | size_type | O(1) |
| `max_size()` | Get maximum size | None | size_type | O(1) |
| `reserve(size_type new_cap)` | Reserve capacity | new_cap: new capacity | void | O(n) |
| `capacity()` | Get current capacity | None | size_type | O(1) |
| `shrink_to_fit()` | Reduce capacity to fit size | None | void | O(n) |
| `clear()` | Clear contents | None | void | O(n) |
| `insert(const_iterator pos, const T& value)` | Insert element | pos: position, value: value | iterator | O(n) |
| `insert(const_iterator pos, T&& value)` | Insert element (move) | pos: position, value: value | iterator | O(n) |
| `insert(const_iterator pos, size_type count, const T& value)` | Insert multiple elements | pos, count, value | iterator | O(n) |
| `insert(const_iterator pos, InputIt first, InputIt last)` | Insert range | pos, first, last | iterator | O(n) |
| `emplace(const_iterator pos, Args&&... args)` | Construct and insert | pos, args | iterator | O(n) |
| `erase(const_iterator pos)` | Erase element | pos: position | iterator | O(n) |
| `erase(const_iterator first, const_iterator last)` | Erase range | first, last | iterator | O(n) |
| `push_back(const T& value)` | Add element at end | value | void | Amortized O(1) |
| `push_back(T&& value)` | Add element at end (move) | value | void | Amortized O(1) |
| `emplace_back(Args&&... args)` | Construct and add at end | args | T& | Amortized O(1) |
| `pop_back()` | Remove last element | None | void | O(1) |
| `resize(size_type count)` | Change size | count | void | O(n) |
| `resize(size_type count, const value_type& value)` | Change size and fill | count, value | void | O(n) |
| `swap(vector& other)` | Swap contents | other | void | O(1) |

### Example
```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main() {
    // Different ways to initialize a vector
    std::vector<int> vec1;                        // Empty vector
    std::vector<int> vec2(5);                     // Vector with 5 elements, initialized to 0
    std::vector<int> vec3(5, 10);                 // Vector with 5 elements, all initialized to 10
    std::vector<int> vec4 = {1, 2, 3, 4, 5};      // Using initializer list
    std::vector<int> vec5(vec4.begin(), vec4.end()); // Using iterators
    std::vector<int> vec6 = vec4;                 // Copy constructor
    
    // Size and capacity
    std::cout << "Size: " << vec4.size() << std::endl;         // Outputs: 5
    std::cout << "Capacity: " << vec4.capacity() << std::endl; // Capacity >= Size
    
    // Reserving space
    vec1.reserve(10);  // Reserve space for 10 elements
    std::cout << "Capacity after reserve: " << vec1.capacity() << std::endl;
    
    // Adding elements
    vec1.push_back(10);
    vec1.push_back(20);
    vec1.push_back(30);
    
    // Accessing elements
    std::cout << "First element: " << vec1[0] << std::endl;        // No bounds checking
    std::cout << "Second element: " << vec1.at(1) << std::endl;    // With bounds checking
    std::cout << "Front: " << vec1.front() << std::endl;           // First element
    std::cout << "Back: " << vec1.back() << std::endl;             // Last element
    
    // Iterating through vector
    std::cout << "Elements: ";
    for (const auto& element : vec1) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    
    // Using iterators
    std::cout << "Using iterators: ";
    for (auto it = vec1.begin(); it != vec1.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Inserting elements
    vec1.insert(vec1.begin() + 1, 15);  // Insert 15 at position 1
    
    // Using emplace_back (more efficient than push_back for objects)
    std::vector<std::string> names;
    names.emplace_back("Alice");  // Constructs string in-place
    names.push_back("Bob");       // Creates temporary string, then moves it
    
    // Removing elements
    vec1.pop_back();  // Remove last element
    vec1.erase(vec1.begin());  // Remove first element
    
    // Resizing
    vec1.resize(5);  // Resize to 5 elements (adds 0s if growing)
    vec1.resize(7, 100);  // Resize to 7, filling new elements with 100
    
    // Clearing vector
    vec1.clear();  // Removes all elements
    std::cout << "Size after clear: " << vec1.size() << std::endl;  // Outputs: 0
    
    // Using algorithms with vectors
    std::vector<int> nums = {5, 2, 8, 1, 9};
    std::sort(nums.begin(), nums.end());  // Sort in ascending order
    
    std::cout << "Sorted: ";
    for (const auto& num : nums) {
        std::cout << num << " ";  // Outputs: 1 2 5 8 9
    }
    std::cout << std::endl;
    
    return 0;
}
```

### Memory Management
- Vector allocates memory dynamically
- When current capacity is exceeded, vector typically doubles its capacity
- Memory layout is contiguous, enabling fast element access
- `shrink_to_fit()` reduces capacity to fit current size

### Performance Considerations
- Random access: O(1)
- Insertion/removal at the end: Amortized O(1)
- Insertion/removal in the middle: O(n)
- Search: O(n) or O(log n) if sorted
- Memory overhead: Low compared to other containers

## Array (`std::array`)

### Description
Fixed-size array with compile-time size that provides STL container functionality.

### Header
```cpp
#include <array>
```

### Template Parameters
```cpp
template < class T, std::size_t N >
class array;
```
- `T`: Type of the elements
- `N`: Number of elements (fixed at compile time)

### Common Methods

| Method | Description | Parameters | Return Type | Time Complexity |
|--------|-------------|------------|-------------|-----------------|
| `at(size_type pos)` | Access element with bounds checking | pos: position | T& or const T& | O(1) |
| `operator[](size_type pos)` | Access element | pos: position | T& or const T& | O(1) |
| `front()` | Access first element | None | T& or const T& | O(1) |
| `back()` | Access last element | None | T& or const T& | O(1) |
| `data()` | Get pointer to data | None | T* or const T* | O(1) |
| `begin()`, `end()` | Get iterators | None | iterator/const_iterator | O(1) |
| `empty()` | Check if empty | None | bool | O(1) |
| `size()` | Get number of elements | None | size_type | O(1) |
| `max_size()` | Get maximum size (same as size) | None | size_type | O(1) |
| `fill(const T& value)` | Fill with value | value | void | O(n) |
| `swap(array& other)` | Swap contents | other | void | O(n) |

### Example
```cpp
#include <iostream>
#include <array>
#include <algorithm>

int main() {
    // Declaration and initialization
    std::array<int, 5> arr1 = {1, 2, 3, 4, 5};
    std::array<int, 5> arr2;  // Uninitialized values
    std::array<int, 5> arr3{};  // Zero-initialized
    
    // Size (fixed at compile time)
    std::cout << "Size: " << arr1.size() << std::endl;  // Always 5
    
    // Element access
    std::cout << "Element at index 2: " << arr1[2] << std::endl;
    std::cout << "Element at index 3 with bounds checking: " << arr1.at(3) << std::endl;
    
    // First and last elements
    std::cout << "First element: " << arr1.front() << std::endl;
    std::cout << "Last element: " << arr1.back() << std::endl;
    
    // Iterating
    std::cout << "Elements: ";
    for (const auto& element : arr1) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    
    // Using iterators
    std::cout << "Using iterators: ";
    for (auto it = arr1.begin(); it != arr1.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Filling with a value
    arr2.fill(10);
    std::cout << "After fill: ";
    for (const auto& element : arr2) {
        std::cout << element << " ";  // Outputs: 10 10 10 10 10
    }
    std::cout << std::endl;
    
    // Swapping arrays
    arr1.swap(arr2);
    
    // Checking if empty (will be false as size is fixed)
    std::cout << "Is empty? " << (arr1.empty() ? "Yes" : "No") << std::endl;
    
    // Getting raw array pointer
    int* data_ptr = arr1.data();
    std::cout << "First element via pointer: " << *data_ptr << std::endl;
    
    // Algorithms with arrays
    std::array<int, 5> nums = {5, 2, 8, 1, 9};
    std::sort(nums.begin(), nums.end());
    
    std::cout << "Sorted: ";
    for (const auto& num : nums) {
        std::cout << num << " ";  // Outputs: 1 2 5 8 9
    }
    std::cout << std::endl;
    
    return 0;
}
```

### Comparison with C-Style Arrays
- Size is part of the type, known at compile time
- Provides STL container interface (iterators, methods)
- Can be passed by value (copies entire array)
- Supports bounds checking with `at()`
- Compatible with STL algorithms

## Deque (`std::deque`)

### Description
Double-ended queue allowing fast insertion and deletion at both ends.

### Header
```cpp
#include <deque>
```

### Template Parameters
```cpp
template < class T, class Allocator = std::allocator<T> >
class deque;
```
- `T`: Type of the elements
- `Allocator`: Memory allocator to use (default is standard allocator)

### Common Methods

| Method | Description | Parameters | Return Type | Time Complexity |
|--------|-------------|------------|-------------|-----------------|
| `at(size_type pos)` | Access element with bounds checking | pos: position | T& or const T& | O(1) |
| `operator[](size_type pos)` | Access element | pos: position | T& or const T& | O(1) |
| `front()` | Access first element | None | T& or const T& | O(1) |
| `back()` | Access last element | None | T& or const T& | O(1) |
| `begin()`, `end()` | Get iterators | None | iterator/const_iterator | O(1) |
| `empty()` | Check if empty | None | bool | O(1) |
| `size()` | Get number of elements | None | size_type | O(1) |
| `max_size()` | Get maximum size | None | size_type | O(1) |
| `shrink_to_fit()` | Reduce capacity to fit size | None | void | O(n) |
| `clear()` | Clear contents | None | void | O(n) |
| `insert(const_iterator pos, const T& value)` | Insert element | pos: position, value: value | iterator | O(n) |
| `erase(const_iterator pos)` | Erase element | pos: position | iterator | O(n) |
| `push_back(const T& value)` | Add element at end | value | void | O(1) |
| `push_front(const T& value)` | Add element at beginning | value | void | O(1) |
| `emplace_back(Args&&... args)` | Construct and add at end | args | void | O(1) |
| `emplace_front(Args&&... args)` | Construct and add at beginning | args | void | O(1) |
| `pop_back()` | Remove last element | None | void | O(1) |
| `pop_front()` | Remove first element | None | void | O(1) |
| `resize(size_type count)` | Change size | count | void | O(n) |
| `swap(deque& other)` | Swap contents | other | void | O(1) |

### Example
```cpp
#include <iostream>
#include <deque>
#include <string>

int main() {
    // Declaration and initialization
    std::deque<int> deq1;                      // Empty deque
    std::deque<int> deq2(5, 10);               // Deque with 5 elements, all initialized to 10
    std::deque<int> deq3 = {1, 2, 3, 4, 5};    // Using initializer list
    
    // Size operations
    std::cout << "Size: " << deq3.size() << std::endl;  // Outputs: 5
    
    // Adding elements at both ends
    deq1.push_back(10);   // Add at end
    deq1.push_back(20);
    deq1.push_front(5);   // Add at beginning
    deq1.push_front(0);
    
    // Current deq1: [0, 5, 10, 20]
    
    // Accessing elements
    std::cout << "First element: " << deq1.front() << std::endl;  // Outputs: 0
    std::cout << "Last element: " << deq1.back() << std::endl;    // Outputs: 20
    std::cout << "Element at index 2: " << deq1[2] << std::endl;  // Outputs: 10
    std::cout << "Element at index 1 with bounds checking: " << deq1.at(1) << std::endl;  // Outputs: 5
    
    // Removing elements from both ends
    deq1.pop_front();  // Remove from beginning
    deq1.pop_back();   // Remove from end
    
    // Current deq1: [5, 10]
    
    // Iterating through deque
    std::cout << "Elements: ";
    for (const auto& element : deq1) {
        std::cout << element << " ";  // Outputs: 5 10
    }
    std::cout << std::endl;
    
    // Using iterators
    std::cout << "Using iterators: ";
    for (auto it = deq1.begin(); it != deq1.end(); ++it) {
        std::cout << *it << " ";  // Outputs: 5 10
    }
    std::cout << std::endl;
    
    // Inserting in the middle
    deq1.insert(deq1.begin() + 1, 7);  // Insert 7 at position 1
    // Current deq1: [5, 7, 10]
    
    // Using emplace operations
    std::deque<std::string> names;
    names.emplace_front("Alice");  // Constructs string in-place at the front
    names.emplace_back("Bob");     // Constructs string in-place at the back
    
    std::cout << "Names: ";
    for (const auto& name : names) {
        std::cout << name << " ";  // Outputs: Alice Bob
    }
    std::cout << std::endl;
    
    // Resizing
    deq1.resize(5);  // Resize to 5 elements (adds 0s if growing)
    deq1.resize(7, 100);  // Resize to 7, filling new elements with 100
    
    // Clearing deque
    deq1.clear();  // Removes all elements
    std::cout << "Size after clear: " << deq1.size() << std::endl;  // Outputs: 0
    
    return 0;
}
```

### Memory Management
- Elements are not stored contiguously
- Typically implemented as a sequence of fixed-size arrays
- Allows O(1) insertion/deletion at both ends
- Random access still O(1) but slower than vector

### Performance Considerations
- Random access: O(1) but with more overhead than vector
- Insertion/removal at ends: O(1)
- Insertion/removal in the middle: O(n)
- Higher memory overhead compared to vector
- No reallocation when growing from the ends

## List (`std::list`)

### Description
Doubly-linked list implementation allowing constant time insertion and removal anywhere in the sequence.

### Header
```cpp
#include <list>
```

### Template Parameters
```cpp
template < class T, class Allocator = std::allocator<T> >
class list;
```
- `T`: Type of the elements
- `Allocator`: Memory allocator to use (default is standard allocator)

### Common Methods

| Method | Description | Parameters | Return Type | Time Complexity |
|--------|-------------|------------|-------------|-----------------|
| `front()` | Access first element | None | T& or const T& | O(1) |
| `back()` | Access last element | None | T& or const T& | O(1) |
| `begin()`, `end()` | Get iterators | None | iterator/const_iterator | O(1) |
| `empty()` | Check if empty | None | bool | O(1) |
| `size()` | Get number of elements | None | size_type | O(1) |
| `max_size()` | Get maximum size | None | size_type | O(1) |
| `clear()` | Clear contents | None | void | O(n) |
| `insert(const_iterator pos, const T& value)` | Insert element | pos: position, value: value | iterator | O(1) |
| `emplace(const_iterator pos, Args&&... args)` | Construct and insert | pos, args | iterator | O(1) |
| `erase(const_iterator pos)` | Erase element | pos: position | iterator | O(1) |
| `push_back(const T& value)` | Add element at end | value | void | O(1) |
| `push_front(const T& value)` | Add element at beginning | value | void | O(1) |
| `emplace_back(Args&&... args)` | Construct and add at end | args | void | O(1) |
| `emplace_front(Args&&... args)` | Construct and add at beginning | args | void | O(1) |
| `pop_back()` | Remove last element | None | void | O(1) |
| `pop_front()` | Remove first element | None | void | O(1) |
| `resize(size_type count)` | Change size | count | void | O(n) |
| `swap(list& other)` | Swap contents | other | void | O(1) |
| `merge(list& other)` | Merge sorted lists | other | void | O(n) |
| `splice(const_iterator pos, list& other)` | Move elements from other list | pos, other | void | O(1) |
| `remove(const T& value)` | Remove elements with value | value | size_type | O(n) |
| `remove_if(UnaryPredicate p)` | Remove elements fulfilling predicate | p | size_type | O(n) |
| `reverse()` | Reverse the order | None | void | O(n) |
| `unique()` | Remove consecutive duplicates | None | size_type | O(n) |
| `sort()` | Sort elements | None | void | O(n log n) |

### Example
```cpp
#include <iostream>
#include <list>
#include <string>
#include <algorithm>

void printList(const std::list<int>& list, const std::string& label) {
    std::cout << label << ": ";
    for (const auto& element : list) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Declaration and initialization
    std::list<int> list1;                    // Empty list
    std::list<int> list2(5, 10);             // List with 5 elements, all initialized to 10
    std::list<int> list3 = {1, 2, 3, 4, 5};  // Using initializer list
    
    // Size operations
    std::cout << "Size: " << list3.size() << std::endl;  // Outputs: 5
    
    // Adding elements at both ends
    list1.push_back(30);    // Add at end
    list1.push_back(40);
    list1.push_front(20);   // Add at beginning
    list1.push_front(10);
    
    printList(list1, "After push operations");  // Outputs: 10 20 30 40
    
    // Accessing elements
    std::cout << "First element: " << list1.front() << std::endl;  // Outputs: 10
    std::cout << "Last element: " << list1.back() << std::endl;    // Outputs: 40
    
    // Removing elements from both ends
    list1.pop_front();  // Remove from beginning
    list1.pop_back();   // Remove from end
    
    printList(list1, "After pop operations");  // Outputs: 20 30
    
    // Iterating using iterators
    std::cout << "Using iterators: ";
    for (auto it = list1.begin(); it != list1.end(); ++it) {
        std::cout << *it << " ";  // Outputs: 20 30
    }
    std::cout << std::endl;
    
    // Inserting in the middle
    auto it = list1.begin();
    std::advance(it, 1);  // Move iterator to position 1
    list1.insert(it, 25);  // Insert 25 at position 1
    
    printList(list1, "After insert");  // Outputs: 20 25 30
    
    // Using emplace operations
    std::list<std::string> names;
    names.emplace_front("Alice");  // Constructs string in-place at the front
    names.emplace_back("Bob");     // Constructs string in-place at the back
    
    // Insert at specific position with emplace
    auto nameIt = names.begin();
    std::advance(nameIt, 1);
    names.emplace(nameIt, "Charlie");  // Insert between Alice and Bob
    
    std::cout << "Names: ";
    for (const auto& name : names) {
        std::cout << name << " ";  // Outputs: Alice Charlie Bob
    }
    std::cout << std::endl;
    
    // List-specific operations
    std::list<int> list4 = {1, 2, 2, 3, 3, 3, 4, 5, 5};
    list4.unique();  // Remove consecutive duplicates
    printList(list4, "After unique");  // Outputs: 1 2 3 4 5
    
    std::list<int> list5 = {10, 20, 30};
    list4.splice(list4.begin(), list5);  // Move all elements from list5 to beginning of list4
    printList(list4, "After splice");  // Outputs: 10 20 30 1 2 3 4 5
    printList(list5, "List5 after splice");  // Outputs: (empty)
    
    list4.sort();  // Sort the elements
    printList(list4, "After sort");  // Outputs: 1 2 3 4 5 10 20 30
    
    list4.reverse();  // Reverse the order
    printList(list4, "After reverse");  // Outputs: 30 20 10 5 4 3 2 1
    
    // Remove specific value
    list4.remove(10);
    printList(list4, "After removing 10");  // Outputs: 30 20 5 4 3 2 1
    
    // Remove elements matching predicate
    list4.remove_if([](int n){ return n % 2 == 0; });  // Remove even numbers
    printList(list4, "After removing even numbers");  // Outputs: 5 3 1
    
    // Merge sorted lists
    std::list<int> list6 = {2, 4, 6};
    std::list<int> list7 = {1, 3, 5, 7};
    list6.sort();
    list7.sort();
    list6.merge(list7);  // Merge and maintain sorted order
    printList(list6, "After merge");  // Outputs: 1 2 3 4 5 6 7
    
    // Clearing list
    list6.clear();
    std::cout << "Size after clear: " << list6.size() << std::endl;  // Outputs: 0
    
    return 0;
}
```

### Memory Management
- Elements are allocated individually (non-contiguous)
- Each element stores pointers to previous and next elements
- No reallocation when inserting/removing elements
- Higher memory overhead per element

### Performance Considerations
- No random access: O(n) to access arbitrary element
- Fast insertion/removal anywhere: O(1) once position is found
- Finding an element: O(n)
- Bidirectional iterators (can move forward and backward)
- No iterator invalidation on insertion/removal (except for erased element)

## Forward List (`std::forward_list`)

### Description
Singly-linked list implementation optimized for insertion and removal at any point.

### Header
```cpp
#include <forward_list>
```

### Template Parameters
```cpp
template < class T, class Allocator = std::allocator<T> >
class forward_list;
```
- `T`: Type of the elements
- `Allocator`: Memory allocator to use (default is standard allocator)

### Common Methods

| Method | Description | Parameters | Return Type | Time Complexity |
|--------|-------------|------------|-------------|-----------------|
| `front()` | Access first element | None | T& or const T& | O(1) |
| `begin()`, `end()` | Get iterators | None | iterator/const_iterator | O(1) |
| `before_begin()` | Get iterator to before-first element | None | iterator/const_iterator | O(1) |
| `empty()` | Check if empty | None | bool | O(1) |
| `max_size()` | Get maximum size | None | size_type | O(1) |
| `clear()` | Clear contents | None | void | O(n) |
| `insert_after(const_iterator pos, const T& value)` | Insert after position | pos, value | iterator | O(1) |
| `emplace_after(const_iterator pos, Args&&... args)` | Construct and insert after | pos, args | iterator | O(1) |
| `erase_after(const_iterator pos)` | Erase element after position | pos | iterator | O(1) |
| `push_front(const T& value)` | Add element at beginning | value | void | O(1) |
| `emplace_front(Args&&... args)` | Construct and add at beginning | args | void | O(1) |
| `pop_front()` | Remove first element | None | void | O(1) |
| `resize(size_type count)` | Change size | count | void | O(n) |
| `swap(forward_list& other)` | Swap contents | other | void | O(1) |
| `merge(forward_list& other)` | Merge sorted lists | other | void | O(n) |
| `splice_after(const_iterator pos, forward_list& other)` | Move elements after position | pos, other | void | O(1) |
| `remove(const T& value)` | Remove elements with value | value | void | O(n) |
| `remove_if(UnaryPredicate p)` | Remove elements fulfilling predicate | p | void | O(n) |
| `reverse()` | Reverse the order | None | void | O(n) |
| `unique()` | Remove consecutive duplicates | None | void | O(n) |
| `sort()` | Sort elements | None | void | O(n log n) |

### Example
```cpp
#include <iostream>
#include <forward_list>
#include <string>
#include <algorithm>

void printList(const std::forward_list<int>& list, const std::string& label) {
    std::cout << label << ": ";
    for (const auto& element : list) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Declaration and initialization
    std::forward_list<int> flist1;                    // Empty forward_list
    std::forward_list<int> flist2(5, 10);             // 5 elements, all initialized to 10
    std::forward_list<int> flist3 = {1, 2, 3, 4, 5};  // Using initializer list
    
    // Adding elements at the front
    flist1.push_front(30);
    flist1.push_front(20);
    flist1.push_front(10);
    
    printList(flist1, "After push operations");  // Outputs: 10 20 30
    
    // Accessing the first element
    std::cout << "First element: " << flist1.front() << std::endl;  // Outputs: 10
    
    // Removing the first element
    flist1.pop_front();
    printList(flist1, "After pop_front");  // Outputs: 20 30
    
    // Iterating using iterators
    std::cout << "Using iterators: ";
    for (auto it = flist1.begin(); it != flist1.end(); ++it) {
        std::cout << *it << " ";  // Outputs: 20 30
    }
    std::cout << std::endl;
    
    // Inserting after a position
    auto it = flist1.begin();  // Points to first element (20)
    flist1.insert_after(it, 25);  // Insert 25 after 20
    
    printList(flist1, "After insert_after");  // Outputs: 20 25 30
    
    // Using emplace operations
    std::forward_list<std::string> names;
    names.emplace_front("Alice");  // Constructs string in-place at the front
    
    // Insert at specific position with emplace_after
    auto nameIt = names.begin();
    names.emplace_after(nameIt, "Bob");  // Insert after Alice
    
    std::cout << "Names: ";
    for (const auto& name : names) {
        std::cout << name << " ";  // Outputs: Alice Bob
    }
    std::cout << std::endl;
    
    // Forward_list-specific operations
    std::forward_list<int> flist4 = {1, 2, 2, 3, 3, 3, 4, 5, 5};
    flist4.unique();  // Remove consecutive duplicates
    printList(flist4, "After unique");  // Outputs: 1 2 3 4 5
    
    // Using before_begin() to insert at the beginning
    flist4.insert_after(flist4.before_begin(), 0);
    printList(flist4, "After insert at beginning");  // Outputs: 0 1 2 3 4 5
    
    std::forward_list<int> flist5 = {10, 20, 30};
    flist4.splice_after(flist4.before_begin(), flist5);  // Move all elements from flist5 after before_begin
    printList(flist4, "After splice_after");  // Outputs: 10 20 30 0 1 2 3 4 5
    printList(flist5, "flist5 after splice_after");  // Outputs: (empty)
    
    flist4.sort();  // Sort the elements
    printList(flist4, "After sort");  // Outputs: 0 1 2 3 4 5 10 20 30
    
    flist4.reverse();  // Reverse the order
    printList(flist4, "After reverse");  // Outputs: 30 20 10 5 4 3 2 1 0
    
    // Remove specific value
    flist4.remove(10);
    printList(flist4, "After removing 10");  // Outputs: 30 20 5 4 3 2 1 0
    
    // Remove elements matching predicate
    flist4.remove_if([](int n){ return n % 2 == 0; });  // Remove even numbers
    printList(flist4, "After removing even numbers");  // Outputs: 5 3 1
    
    // Merge sorted lists
    std::forward_list<int> flist6 = {2, 4, 6};
    std::forward_list<int> flist7 = {1, 3, 5, 7};
    flist6.sort();
    flist7.sort();
    flist6.merge(flist7);  // Merge and maintain sorted order
    printList(flist6, "After merge");  // Outputs: 1 2 3 4 5 6 7
    
    // Clearing list
    flist6.clear();
    std::cout << "Is empty after clear? " << (flist6.empty() ? "Yes" : "No") << std::endl;  // Outputs: Yes
    
    return 0;
}
```

### Memory Management
- Singly-linked list (each element has only a forward pointer)
- Lower memory overhead than std::list
- No back() method or backward iteration

### Performance Considerations
- No random access: O(n) to access arbitrary element
- Fast insertion/removal after a known position: O(1)
- No direct way to insert/remove at the end (must traverse)
- Finding an element: O(n)
- Forward-only iteration
- Memory-efficient when compared to std::list