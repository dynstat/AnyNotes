# STL Associative Containers

Associative containers in the STL implement sorted data structures that provide fast lookup based on keys.

## Set (`std::set`)

### Description
A collection of unique keys, sorted by keys. Implemented as a balanced binary search tree (typically red-black tree).

### Header
```cpp
#include <set>
```

### Template Parameters
```cpp
template < class Key, 
           class Compare = std::less<Key>,
           class Allocator = std::allocator<Key> >
class set;
```
- `Key`: Type of the stored elements (keys)
- `Compare`: Function object for comparing keys (default uses `operator<`)
- `Allocator`: Memory allocator to use (default is standard allocator)

### Common Methods

| Method | Description | Parameters | Return Type | Time Complexity |
|--------|-------------|------------|-------------|-----------------|
| `begin()`, `end()` | Get iterators | None | iterator/const_iterator | O(1) |
| `empty()` | Check if empty | None | bool | O(1) |
| `size()` | Get number of elements | None | size_type | O(1) |
| `max_size()` | Get maximum size | None | size_type | O(1) |
| `clear()` | Clear contents | None | void | O(n) |
| `insert(const value_type& value)` | Insert element | value | pair<iterator, bool> | O(log n) |
| `insert(InputIt first, InputIt last)` | Insert range | first, last | void | O(N log n) |
| `emplace(Args&&... args)` | Construct and insert | args | pair<iterator, bool> | O(log n) |
| `erase(const_iterator pos)` | Erase element | pos | iterator | O(1) |
| `erase(const key_type& key)` | Erase by key | key | size_type | O(log n) |
| `swap(set& other)` | Swap contents | other | void | O(1) |
| `count(const key_type& key)` | Count occurrences of key | key | size_type | O(log n) |
| `find(const key_type& key)` | Find element with key | key | iterator | O(log n) |
| `contains(const key_type& key)` | Check if contains key (C++20) | key | bool | O(log n) |
| `equal_range(const key_type& key)` | Get range of equal elements | key | pair<iterator,iterator> | O(log n) |
| `lower_bound(const key_type& key)` | Get iterator to first element not less than key | key | iterator | O(log n) |
| `upper_bound(const key_type& key)` | Get iterator to first element greater than key | key | iterator | O(log n) |
| `key_comp()` | Get key comparison function | None | key_compare | O(1) |
| `value_comp()` | Get value comparison function | None | value_compare | O(1) |

### Example
```cpp
#include <iostream>
#include <set>
#include <string>
#include <functional>  // For std::greater

void printSet(const std::set<int>& s, const std::string& label) {
    std::cout << label << ": ";
    for (const auto& element : s) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Declaration and initialization
    std::set<int> set1;                    // Empty set
    std::set<int> set2 = {5, 3, 1, 4, 2};  // Using initializer list (will be sorted)
    std::set<int, std::greater<int>> set3 = {1, 2, 3, 4, 5};  // Using custom comparator (descending order)
    
    // Size operations
    std::cout << "Size: " << set2.size() << std::endl;  // Outputs: 5
    
    // Inserting elements
    set1.insert(30);
    set1.insert(10);
    set1.insert(20);
    set1.insert(10);  // Duplicate, won't be inserted
    
    printSet(set1, "Set1 after inserts");  // Outputs: 10 20 30 (sorted)
    
    // Using emplace (constructs in-place)
    set1.emplace(15);
    
    printSet(set1, "After emplace");  // Outputs: 10 15 20 30
    
    // Check if element exists
    bool exists = set1.find(15) != set1.end();
    std::cout << "15 exists: " << (exists ? "Yes" : "No") << std::endl;  // Outputs: Yes
    
    // Count occurrences (always 0 or 1 for set)
    std::cout << "Count of 10: " << set1.count(10) << std::endl;  // Outputs: 1
    std::cout << "Count of 100: " << set1.count(100) << std::endl;  // Outputs: 0
    
    // Using contains (C++20)
    // std::cout << "Contains 20: " << (set1.contains(20) ? "Yes" : "No") << std::endl;
    
    // Erasing elements
    set1.erase(15);  // Erase by value
    printSet(set1, "After erase");  // Outputs: 10 20 30
    
    // Erase using iterator
    auto it = set1.find(20);
    if (it != set1.end()) {
        set1.erase(it);
    }
    printSet(set1, "After erase by iterator");  // Outputs: 10 30
    
    // Range operations
    std::set<int> set4 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Lower bound (first element >= value)
    auto lower = set4.lower_bound(4);
    std::cout << "Lower bound of 4: " << *lower << std::endl;  // Outputs: 4
    
    // Upper bound (first element > value)
    auto upper = set4.upper_bound(4);
    std::cout << "Upper bound of 4: " << *upper << std::endl;  // Outputs: 5
    
    // Equal range (pair of lower_bound and upper_bound)
    auto range = set4.equal_range(4);
    std::cout << "Equal range of 4: " << *range.first << " to " << *range.second << std::endl;  // Outputs: 4 to 5
    
    // Erasing a range
    set4.erase(set4.lower_bound(4), set4.upper_bound(7));
    printSet(set4, "After range erase");  // Outputs: 1 2 3 8 9 10
    
    // Set operations (C++17 and earlier needs to be implemented manually)
    std::set<int> a = {1, 2, 3, 4, 5};
    std::set<int> b = {4, 5, 6, 7, 8};
    std::set<int> result;
    
    // Union
    std::set_union(a.begin(), a.end(), b.begin(), b.end(), 
                  std::inserter(result, result.begin()));
    printSet(result, "Union of sets");  // Outputs: 1 2 3 4 5 6 7 8
    
    // Intersection
    result.clear();
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                         std::inserter(result, result.begin()));
    printSet(result, "Intersection of sets");  // Outputs: 4 5
    
    // Difference
    result.clear();
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                       std::inserter(result, result.begin()));
    printSet(result, "Difference of sets (a - b)");  // Outputs: 1 2 3
    
    // Symmetric difference
    result.clear();
    std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(),
                                 std::inserter(result, result.begin()));
    printSet(result, "Symmetric difference");  // Outputs: 1 2 3 6 7 8
    
    // Custom comparator example
    printSet(set3, "Descending order set");  // Outputs: 5 4 3 2 1
    
    // Clearing set
    set1.clear();
    std::cout << "Size after clear: " << set1.size() << std::endl;  // Outputs: 0
    
    return 0;
}
```

### Performance Considerations
- All operations maintain internal order
- All modifications maintain balanced tree structure
- No direct way to modify a key (must remove and reinsert)
- Bidirectional iterators (can move forward and backward)
- Iterators remain valid after insertion (except for erased element)

## Multiset (`std::multiset`)

### Description
Similar to set, but allows multiple elements with the same key.

### Header
```cpp
#include <set>
```

### Template Parameters
```cpp
template < class Key, 
           class Compare = std::less<Key>,
           class Allocator = std::allocator<Key> >
class multiset;
```
- Same as std::set

### Key Differences from `std::set`

| Feature | `std::set` | `std::multiset` |
|---------|------------|-----------------|
| Duplicate keys | Not allowed | Allowed |
| `insert()` return | pair<iterator, bool> | iterator |
| `count()` result | Always 0 or 1 | Can be > 1 |
| `equal_range()` | Often contains at most 1 element | Can contain multiple elements |

### Example
```cpp
#include <iostream>
#include <set>
#include <string>

void printMultiset(const std::multiset<int>& ms, const std::string& label) {
    std::cout << label << ": ";
    for (const auto& element : ms) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Declaration and initialization
    std::multiset<int> mset1;                          // Empty multiset
    std::multiset<int> mset2 = {5, 3, 1, 4, 2, 3, 1};  // With duplicates
    
    printMultiset(mset2, "Initial multiset");  // Outputs: 1 1 2 3 3 4 5
    
    // Size operations
    std::cout << "Size: " << mset2.size() << std::endl;  // Outputs: 7
    
    // Inserting elements
    mset1.insert(30);
    mset1.insert(10);
    mset1.insert(20);
    mset1.insert(10);  // Duplicate, will be inserted
    
    printMultiset(mset1, "After inserts");  // Outputs: 10 10 20 30
    
    // Count occurrences
    std::cout << "Count of 10: " << mset1.count(10) << std::endl;  // Outputs: 2
    
    // Finding elements
    auto it = mset1.find(10);  // Points to first occurrence of 10
    std::cout << "Found: " << *it << std::endl;  // Outputs: 10
    
    // Equal range (returns range of elements with same key)
    auto range = mset1.equal_range(10);
    std::cout << "Equal range elements: ";
    for (auto i = range.first; i != range.second; ++i) {
        std::cout << *i << " ";  // Outputs: 10 10
    }
    std::cout << std::endl;
    
    // Erasing elements
    size_t removed = mset1.erase(10);  // Erase all occurrences of 10
    std::cout << "Number of elements removed: " << removed << std::endl;  // Outputs: 2
    printMultiset(mset1, "After erase");  // Outputs: 20 30
    
    // Erasing a single occurrence
    mset2.insert(3);  // Add another 3
    printMultiset(mset2, "Before single erase");  // Outputs: 1 1 2 3 3 3 4 5
    
    auto it3 = mset2.find(3);  // Find first occurrence of 3
    mset2.erase(it3);  // Erase only this occurrence
    printMultiset(mset2, "After single erase");  // Outputs: 1 1 2 3 3 4 5
    
    // Lower bound and upper bound
    mset2.insert(3);  // Add back a 3
    auto lower = mset2.lower_bound(3);
    auto upper = mset2.upper_bound(3);
    
    std::cout << "Elements in [lower, upper) for key=3: ";
    for (auto it = lower; it != upper; ++it) {
        std::cout << *it << " ";  // All 3s in the multiset
    }
    std::cout << std::endl;
    
    // Clearing multiset
    mset1.clear();
    std::cout << "Size after clear: " << mset1.size() << std::endl;  // Outputs: 0
    
    return 0;
}
```

### Performance Considerations
- Same as std::set
- All elements with the same key are stored in insertion order
- Finding all occurrences of a key is efficient with equal_range

## Map (`std::map`)

### Description
A sorted associative container that contains key-value pairs with unique keys. Keys are sorted using the comparison function.

### Header
```cpp
#include <map>
```

### Template Parameters
```cpp
template < class Key,
           class T,
           class Compare = std::less<Key>,
           class Allocator = std::allocator<std::pair<const Key, T>> >
class map;
```
- `Key`: Type of the keys
- `T`: Type of the mapped values
- `Compare`: Function object for comparing keys
- `Allocator`: Memory allocator to use

### Common Methods

| Method | Description | Parameters | Return Type | Time Complexity |
|--------|-------------|------------|-------------|-----------------|
| `begin()`, `end()` | Get iterators | None | iterator/const_iterator | O(1) |
| `empty()` | Check if empty | None | bool | O(1) |
| `size()` | Get number of elements | None | size_type | O(1) |
| `max_size()` | Get maximum size | None | size_type | O(1) |
| `clear()` | Clear contents | None | void | O(n) |
| `insert(const value_type& value)` | Insert pair | value | pair<iterator, bool> | O(log n) |
| `insert(InputIt first, InputIt last)` | Insert range | first, last | void | O(N log n) |
| `insert_or_assign(const key_type& k, M&& obj)` | Insert or assign (C++17) | k, obj | pair<iterator, bool> | O(log n) |
| `emplace(Args&&... args)` | Construct and insert | args | pair<iterator, bool> | O(log n) |
| `emplace_hint(const_iterator hint, Args&&... args)` | Construct with hint | hint, args | iterator | O(log n) |
| `try_emplace(const key_type& k, Args&&... args)` | Try to emplace (C++17) | k, args | pair<iterator, bool> | O(log n) |
| `erase(const_iterator pos)` | Erase element | pos | iterator | O(1) |
| `erase(const key_type& key)` | Erase by key | key | size_type | O(log n) |
| `swap(map& other)` | Swap contents | other | void | O(1) |
| `count(const key_type& key)` | Count occurrences of key | key | size_type | O(log n) |
| `find(const key_type& key)` | Find element with key | key | iterator | O(log n) |
| `contains(const key_type& key)` | Check if contains key (C++20) | key | bool | O(log n) |
| `equal_range(const key_type& key)` | Get range of equal elements | key | pair<iterator,iterator> | O(log n) |
| `lower_bound(const key_type& key)` | Get iterator to first element not less than key | key | iterator | O(log n) |
| `upper_bound(const key_type& key)` | Get iterator to first element greater than key | key | iterator | O(log n) |
| `operator[](const key_type& key)` | Access or insert element | key | mapped_type& | O(log n) |
| `at(const key_type& key)` | Access element with bounds checking | key | mapped_type& | O(log n) |
| `key_comp()` | Get key comparison function | None | key_compare | O(1) |
| `value_comp()` | Get value comparison function | None | value_compare | O(1) |

### Example
```cpp
#include <iostream>
#include <map>
#include <string>

void printMap(const std::map<std::string, int>& m, const std::string& label) {
    std::cout << label << ":" << std::endl;
    for (const auto& [key, value] : m) {  // Structured binding (C++17)
        std::cout << "  " << key << ": " << value << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    // Declaration and initialization
    std::map<std::string, int> map1;  // Empty map
    std::map<std::string, int> map2 = {
        {"apple", 5},
        {"banana", 8},
        {"orange", 10}
    };  // Using initializer list
    
    printMap(map2, "Initial map");
    
    // Size operations
    std::cout << "Size: " << map2.size() << std::endl;  // Outputs: 3
    
    // Inserting elements
    map1.insert(std::pair<std::string, int>("one", 1));
    map1.insert(std::make_pair("two", 2));  // Using make_pair helper
    map1["three"] = 3;  // Using subscript operator (inserts if not exists)
    
    printMap(map1, "Map1 after inserts");
    
    // Using emplace (constructs in-place)
    map1.emplace("four", 4);  // More efficient than insert
    
    // Check if element exists
    bool exists = map1.find("two") != map1.end();
    std::cout << "Key 'two' exists: " << (exists ? "Yes" : "No") << std::endl;  // Outputs: Yes
    
    // Count occurrences (always 0 or 1 for map)
    std::cout << "Count of 'five': " << map1.count("five") << std::endl;  // Outputs: 0
    
    // Using contains (C++20)
    // std::cout << "Contains 'four': " << (map1.contains("four") ? "Yes" : "No") << std::endl;
    
    // Accessing elements
    std::cout << "Value of 'one': " << map1["one"] << std::endl;  // Using operator[] (inserts if not exists)
    
    try {
        std::cout << "Value of 'one' with at(): " << map1.at("one") << std::endl;  // Using at() with bounds checking
        std::cout << "Value of 'five' with at(): " << map1.at("five") << std::endl;  // Will throw out_of_range
    } catch (const std::out_of_range& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    // Modifying elements
    map1["one"] = 10;  // Modify existing value
    map1["five"] = 5;  // Insert new key-value pair
    
    printMap(map1, "After modifications");
    
    // C++17 insert_or_assign (similar to operator[] but returns info)
    auto [it, inserted] = map1.insert_or_assign("one", 100);
    std::cout << "insert_or_assign: " 
              << (inserted ? "Inserted new element" : "Assigned to existing element")
              << " with key=" << it->first << " value=" << it->second << std::endl;
    
    // C++17 try_emplace (only inserts if the key doesn't exist)
    auto [it2, inserted2] = map1.try_emplace("one", 200);  // Won't modify existing key
    auto [it3, inserted3] = map1.try_emplace("six", 6);   // Will insert new key
    
    printMap(map1, "After try_emplace");
    
    // Erasing elements
    map1.erase("five");  // Erase by key
    
    // Erase using iterator
    auto it4 = map1.find("four");
    if (it4 != map1.end()) {
        map1.erase(it4);
    }
    
    printMap(map1, "After erase");
    
    // Iterating through the map
    std::cout << "Using iterators:" << std::endl;
    for (auto it = map1.begin(); it != map1.end(); ++it) {
        std::cout << "  " << it->first << ": " << it->second << std::endl;
    }
    std::cout << std::endl;
    
    // Range operations
    std::map<char, int> char_map = {
        {'a', 1}, {'b', 2}, {'c', 3}, {'d', 4}, {'e', 5}
    };
    
    // Lower bound and upper bound
    auto lower = char_map.lower_bound('c');  // First element with key not less than 'c'
    auto upper = char_map.upper_bound('c');  // First element with key greater than 'c'
    
    std::cout << "Lower bound of 'c': " << lower->first << " -> " << lower->second << std::endl;
    std::cout << "Upper bound of 'c': " << upper->first << " -> " << upper->second << std::endl;
    
    // Equal range
    auto range = char_map.equal_range('c');
    std::cout << "Equal range for 'c': from (" 
              << range.first->first << ", " << range.first->second << ") to ("
              << range.second->first << ", " << range.second->second << ")" << std::endl;
    
    // Clearing map
    map1.clear();
    std::cout << "Size after clear: " << map1.size() << std::endl;  // Outputs: 0
    
    return 0;
}
```

### Performance Considerations
- Keys are immutable (cannot be modified directly)
- Always sorted according to the comparison function
- Bidirectional iterators
- Operator[] provides convenient access but inserts default-constructed value if key not found
- at() provides bounds checking but throws exception if key not found

## Multimap (`std::multimap`)

### Description
Similar to map, but allows multiple elements with the same key.

### Header
```cpp
#include <map>
```

### Template Parameters
```cpp
template < class Key,
           class T,
           class Compare = std::less<Key>,
           class Allocator = std::allocator<std::pair<const Key, T>> >
class multimap;
```
- Same as std::map

### Key Differences from `std::map`

| Feature | `std::map` | `std::multimap` |
|---------|------------|-----------------|
| Duplicate keys | Not allowed | Allowed |
| `insert()` return | pair<iterator, bool> | iterator |
| `count()` result | Always 0 or 1 | Can be > 1 |
| `operator[]` | Available | Not available |
| `at()` | Available | Not available |

### Example
```cpp
#include <iostream>
#include <map>
#include <string>

void printMultimap(const std::multimap<std::string, int>& mm, const std::string& label) {
    std::cout << label << ":" << std::endl;
    for (const auto& [key, value] : mm) {  // Structured binding (C++17)
        std::cout << "  " << key << ": " << value << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    // Declaration and initialization
    std::multimap<std::string, int> mmap1;  // Empty multimap
    std::multimap<std::string, int> mmap2 = {
        {"apple", 5},
        {"banana", 8},
        {"apple", 10},  // Duplicate key
        {"orange", 10}
    };
    
    printMultimap(mmap2, "Initial multimap");
    
    // Size operations
    std::cout << "Size: " << mmap2.size() << std::endl;  // Outputs: 4
    
    // Inserting elements
    mmap1.insert(std::pair<std::string, int>("one", 1));
    mmap1.insert(std::make_pair("two", 2));
    mmap1.insert(std::make_pair("one", 10));  // Duplicate key, will be inserted
    
    printMultimap(mmap1, "After inserts");
    
    // Using emplace (constructs in-place)
    mmap1.emplace("three", 3);
    mmap1.emplace("three", 30);  // Another entry with the same key
    
    printMultimap(mmap1, "After emplace");
    
    // Count occurrences
    std::cout << "Count of 'one': " << mmap1.count("one") << std::endl;  // Outputs: 2
    std::cout << "Count of 'four': " << mmap1.count("four") << std::endl;  // Outputs: 0
    
    // Finding elements
    auto it = mmap1.find("one");  // Points to first occurrence of "one"
    if (it != mmap1.end()) {
        std::cout << "Found: " << it->first << ": " << it->second << std::endl;
    }
    
    // Equal range (returns range of elements with same key)
    auto range = mmap1.equal_range("three");
    std::cout << "Elements with key 'three':" << std::endl;
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << "  " << it->first << ": " << it->second << std::endl;
    }
    std::cout << std::endl;
    
    // Erasing elements
    size_t removed = mmap1.erase("one");  // Erase all occurrences of key "one"
    std::cout << "Number of elements removed: " << removed << std::endl;  // Outputs: 2
    
    printMultimap(mmap1, "After erase");
    
    // Erasing a single occurrence
    auto it3 = mmap1.find("three");  // Find first occurrence of "three"
    if (it3 != mmap1.end()) {
        mmap1.erase(it3);  // Erase only this occurrence
    }
    
    printMultimap(mmap1, "After single erase");
    
    // Lower bound and upper bound
    std::multimap<char, int> char_mmap = {
        {'a', 1}, {'b', 2}, {'c', 3}, {'c', 30}, {'c', 300}, {'d', 4}, {'e', 5}
    };
    
    auto lower = char_mmap.lower_bound('c');
    auto upper = char_mmap.upper_bound('c');
    
    std::cout << "Elements in range ['c', after 'c'):" << std::endl;
    for (auto it = lower; it != upper; ++it) {
        std::cout << "  " << it->first << ": " << it->second << std::endl;
    }
    std::cout << std::endl;
    
    // Clearing multimap
    mmap1.clear();
    std::cout << "Size after clear: " << mmap1.size() << std::endl;  // Outputs: 0
    
    return 0;
}
```

### Performance Considerations
- Similar to std::map
- Elements with the same key are stored in insertion order
- No direct element access via operator[] or at()
- Must use find(), lower_bound(), upper_bound(), or equal_range() to access elements