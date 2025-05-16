# STL Unordered Associative Containers

Unordered associative containers in the STL implement unsorted (hash table) data structures that provide fast lookup based on keys with average constant-time complexity.

## Unordered Set (`std::unordered_set`)

### Description
A collection of unique keys, hashed by keys. Implemented as a hash table, which provides average constant-time complexity for search, insertion, and deletion.

### Header
```cpp
#include <unordered_set>
```

### Template Parameters
```cpp
template < class Key,
           class Hash = std::hash<Key>,
           class KeyEqual = std::equal_to<Key>,
           class Allocator = std::allocator<Key> >
class unordered_set;
```
- `Key`: Type of the stored elements (keys)
- `Hash`: Function object for hashing keys
- `KeyEqual`: Function object for comparing keys for equality
- `Allocator`: Memory allocator to use

### Common Methods

| Method | Description | Parameters | Return Type | Average Time Complexity |
|--------|-------------|------------|-------------|-------------------------|
| `begin()`, `end()` | Get iterators | None | iterator/const_iterator | O(1) |
| `empty()` | Check if empty | None | bool | O(1) |
| `size()` | Get number of elements | None | size_type | O(1) |
| `max_size()` | Get maximum size | None | size_type | O(1) |
| `clear()` | Clear contents | None | void | O(n) |
| `insert(const value_type& value)` | Insert element | value | pair<iterator, bool> | O(1) |
| `insert(InputIt first, InputIt last)` | Insert range | first, last | void | O(N) |
| `emplace(Args&&... args)` | Construct and insert | args | pair<iterator, bool> | O(1) |
| `erase(const_iterator pos)` | Erase element | pos | iterator | O(1) |
| `erase(const key_type& key)` | Erase by key | key | size_type | O(1) |
| `swap(unordered_set& other)` | Swap contents | other | void | O(1) |
| `count(const key_type& key)` | Count occurrences of key | key | size_type | O(1) |
| `find(const key_type& key)` | Find element with key | key | iterator | O(1) |
| `contains(const key_type& key)` | Check if contains key (C++20) | key | bool | O(1) |
| `equal_range(const key_type& key)` | Get range of equal elements | key | pair<iterator,iterator> | O(1) |
| `bucket_count()` | Get number of buckets | None | size_type | O(1) |
| `load_factor()` | Get load factor | None | float | O(1) |
| `max_load_factor()` | Get maximum load factor | None | float | O(1) |
| `max_load_factor(float ml)` | Set maximum load factor | ml | void | O(1) |
| `rehash(size_type n)` | Set number of buckets | n | void | O(n) |
| `reserve(size_type n)` | Reserve at least n buckets | n | void | O(n) |
| `hash_function()` | Get hash function | None | hasher | O(1) |
| `key_eq()` | Get key equality function | None | key_equal | O(1) |

### Example
```cpp
#include <iostream>
#include <unordered_set>
#include <string>

void printUnorderedSet(const std::unordered_set<int>& us, const std::string& label) {
    std::cout << label << ": ";
    for (const auto& element : us) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Declaration and initialization
    std::unordered_set<int> uset1;                    // Empty unordered_set
    std::unordered_set<int> uset2 = {5, 3, 1, 4, 2};  // Using initializer list (order not preserved)
    
    // Size operations
    std::cout << "Size: " << uset2.size() << std::endl;  // Outputs: 5
    
    // Inserting elements
    uset1.insert(30);
    uset1.insert(10);
    uset1.insert(20);
    uset1.insert(10);  // Duplicate, won't be inserted
    
    printUnorderedSet(uset1, "After inserts");  // Note: Order is not guaranteed
    
    // Using emplace (constructs in-place)
    uset1.emplace(15);
    
    printUnorderedSet(uset1, "After emplace");
    
    // Check if element exists
    bool exists = uset1.find(15) != uset1.end();
    std::cout << "15 exists: " << (exists ? "Yes" : "No") << std::endl;  // Outputs: Yes
    
    // Count occurrences (always 0 or 1 for unordered_set)
    std::cout << "Count of 10: " << uset1.count(10) << std::endl;  // Outputs: 1
    std::cout << "Count of 100: " << uset1.count(100) << std::endl;  // Outputs: 0
    
    // Using contains (C++20)
    // std::cout << "Contains 20: " << (uset1.contains(20) ? "Yes" : "No") << std::endl;
    
    // Erasing elements
    uset1.erase(15);  // Erase by value
    printUnorderedSet(uset1, "After erase");
    
    // Erase using iterator
    auto it = uset1.find(20);
    if (it != uset1.end()) {
        uset1.erase(it);
    }
    printUnorderedSet(uset1, "After erase by iterator");
    
    // Bucket interface
    std::unordered_set<std::string> words = {"apple", "banana", "cherry", "date", "elderberry"};
    
    std::cout << "Bucket count: " << words.bucket_count() << std::endl;
    std::cout << "Load factor: " << words.load_factor() << std::endl;
    std::cout << "Max load factor: " << words.max_load_factor() << std::endl;
    
    // Iterating through buckets
    std::cout << "Bucket contents:" << std::endl;
    for (size_t i = 0; i < words.bucket_count(); ++i) {
        std::cout << "Bucket " << i << ": ";
        for (auto it = words.begin(i); it != words.end(i); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
    
    // Controlling bucket count
    words.rehash(20);  // Set number of buckets to at least 20
    std::cout << "Bucket count after rehash: " << words.bucket_count() << std::endl;
    
    words.reserve(100);  // Reserve space for at least 100 elements
    std::cout << "Bucket count after reserve: " << words.bucket_count() << std::endl;
    
    // Controlling load factor
    words.max_load_factor(0.7f);  // Set maximum load factor to 0.7
    std::cout << "New max load factor: " << words.max_load_factor() << std::endl;
    
    // Creating with custom hash and equality functions
    struct CaseInsensitiveHash {
        size_t operator()(const std::string& key) const {
            std::string lower_key = key;
            std::transform(lower_key.begin(), lower_key.end(), lower_key.begin(),
                          [](unsigned char c){ return std::tolower(c); });
            return std::hash<std::string>{}(lower_key);
        }
    };
    
    struct CaseInsensitiveEqual {
        bool operator()(const std::string& left, const std::string& right) const {
            return std::equal(left.begin(), left.end(), right.begin(), right.end(),
                             [](unsigned char a, unsigned char b) {
                                 return std::tolower(a) == std::tolower(b);
                             });
        }
    };
    
    std::unordered_set<std::string, CaseInsensitiveHash, CaseInsensitiveEqual> case_insensitive_set;
    case_insensitive_set.insert("Hello");
    case_insensitive_set.insert("WORLD");
    
    // Now "hello" and "Hello" are considered the same key
    std::cout << "Count of 'hello': " << case_insensitive_set.count("hello") << std::endl;  // Outputs: 1
    std::cout << "Count of 'HELLO': " << case_insensitive_set.count("HELLO") << std::endl;  // Outputs: 1
    std::cout << "Count of 'world': " << case_insensitive_set.count("world") << std::endl;  // Outputs: 1
    
    // Clearing set
    uset1.clear();
    std::cout << "Size after clear: " << uset1.size() << std::endl;  // Outputs: 0
    
    return 0;
}
```

### Performance Considerations
- Average constant-time complexity for search, insert, erase
- Worst-case linear complexity if many collisions
- No ordering of elements
- Direct iteration through all elements is O(n)
- Iterator invalidation rules are stricter than for associative containers
- Iterators, pointers, and references may be invalidated by rehashing

## Unordered Multiset (`std::unordered_multiset`)

### Description
Similar to unordered_set, but allows multiple elements with the same key.

### Header
```cpp
#include <unordered_set>
```

### Template Parameters
```cpp
template < class Key,
           class Hash = std::hash<Key>,
           class KeyEqual = std::equal_to<Key>,
           class Allocator = std::allocator<Key> >
class unordered_multiset;
```
- Same as std::unordered_set

### Key Differences from `std::unordered_set`

| Feature | `std::unordered_set` | `std::unordered_multiset` |
|---------|----------------------|----------------------------|
| Duplicate keys | Not allowed | Allowed |
| `insert()` return | pair<iterator, bool> | iterator |
| `count()` result | Always 0 or 1 | Can be > 1 |
| `equal_range()` | Often contains at most 1 element | Can contain multiple elements |

### Example
```cpp
#include <iostream>
#include <unordered_set>
#include <string>

void printUnorderedMultiset(const std::unordered_multiset<int>& ums, const std::string& label) {
    std::cout << label << ": ";
    for (const auto& element : ums) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Declaration and initialization
    std::unordered_multiset<int> umset1;                          // Empty unordered_multiset
    std::unordered_multiset<int> umset2 = {5, 3, 1, 4, 2, 3, 1};  // With duplicates
    
    printUnorderedMultiset(umset2, "Initial unordered_multiset");
    
    // Size operations
    std::cout << "Size: " << umset2.size() << std::endl;  // Outputs: 7
    
    // Inserting elements
    umset1.insert(30);
    umset1.insert(10);
    umset1.insert(20);
    umset1.insert(10);  // Duplicate, will be inserted
    
    printUnorderedMultiset(umset1, "After inserts");
    
    // Count occurrences
    std::cout << "Count of 10: " << umset1.count(10) << std::endl;  // Outputs: 2
    
    // Finding elements
    auto it = umset1.find(10);  // Points to one occurrence of 10
    std::cout << "Found: " << *it << std::endl;
    
    // Equal range (returns range of elements with same key)
    auto range = umset1.equal_range(10);
    std::cout << "Equal range elements: ";
    for (auto i = range.first; i != range.second; ++i) {
        std::cout << *i << " ";  // All 10s in the multiset
    }
    std::cout << std::endl;
    
    // Erasing elements
    size_t removed = umset1.erase(10);  // Erase all occurrences of 10
    std::cout << "Number of elements removed: " << removed << std::endl;  // Outputs: 2
    printUnorderedMultiset(umset1, "After erase");
    
    // Erasing a single occurrence
    umset2.insert(3);  // Add another 3
    printUnorderedMultiset(umset2, "Before single erase");
    
    auto it3 = umset2.find(3);  // Find one occurrence of 3
    umset2.erase(it3);  // Erase only this occurrence
    printUnorderedMultiset(umset2, "After single erase");
    
    // Bucket interface (same as unordered_set)
    std::cout << "Bucket count: " << umset2.bucket_count() << std::endl;
    std::cout << "Load factor: " << umset2.load_factor() << std::endl;
    
    // Clearing unordered_multiset
    umset1.clear();
    std::cout << "Size after clear: " << umset1.size() << std::endl;  // Outputs: 0
    
    return 0;
}
```

### Performance Considerations
- Same as std::unordered_set
- Elements with the same key are typically stored in the same bucket
- Finding all occurrences of a key is still an O(1) operation on average
- Removing all occurrences of a key is also O(1) on average

## Unordered Map (`std::unordered_map`)

### Description
An associative container that contains key-value pairs with unique keys. Keys are hashed, which provides average constant-time complexity for search, insertion, and deletion.

### Header
```cpp
#include <unordered_map>
```

### Template Parameters
```cpp
template < class Key,
           class T,
           class Hash = std::hash<Key>,
           class KeyEqual = std::equal_to<Key>,
           class Allocator = std::allocator<std::pair<const Key, T>> >
class unordered_map;
```
- `Key`: Type of the keys
- `T`: Type of the mapped values
- `Hash`: Function object for hashing keys
- `KeyEqual`: Function object for comparing keys for equality
- `Allocator`: Memory allocator to use

### Common Methods

| Method | Description | Parameters | Return Type | Average Time Complexity |
|--------|-------------|------------|-------------|-------------------------|
| `begin()`, `end()` | Get iterators | None | iterator/const_iterator | O(1) |
| `empty()` | Check if empty | None | bool | O(1) |
| `size()` | Get number of elements | None | size_type | O(1) |
| `max_size()` | Get maximum size | None | size_type | O(1) |
| `clear()` | Clear contents | None | void | O(n) |
| `insert(const value_type& value)` | Insert pair | value | pair<iterator, bool> | O(1) |
| `insert(InputIt first, InputIt last)` | Insert range | first, last | void | O(N) |
| `insert_or_assign(const key_type& k, M&& obj)` | Insert or assign (C++17) | k, obj | pair<iterator, bool> | O(1) |
| `emplace(Args&&... args)` | Construct and insert | args | pair<iterator, bool> | O(1) |
| `emplace_hint(const_iterator hint, Args&&... args)` | Construct with hint | hint, args | iterator | O(1) |
| `try_emplace(const key_type& k, Args&&... args)` | Try to emplace (C++17) | k, args | pair<iterator, bool> | O(1) |
| `erase(const_iterator pos)` | Erase element | pos | iterator | O(1) |
| `erase(const key_type& key)` | Erase by key | key | size_type | O(1) |
| `swap(unordered_map& other)` | Swap contents | other | void | O(1) |
| `count(const key_type& key)` | Count occurrences of key | key | size_type | O(1) |
| `find(const key_type& key)` | Find element with key | key | iterator | O(1) |
| `contains(const key_type& key)` | Check if contains key (C++20) | key | bool | O(1) |
| `equal_range(const key_type& key)` | Get range of equal elements | key | pair<iterator,iterator> | O(1) |
| `operator[](const key_type& key)` | Access or insert element | key | mapped_type& | O(1) |
| `at(const key_type& key)` | Access element with bounds checking | key | mapped_type& | O(1) |
| `bucket_count()` | Get number of buckets | None | size_type | O(1) |
| `load_factor()` | Get load factor | None | float | O(1) |
| `max_load_factor()` | Get maximum load factor | None | float | O(1) |
| `max_load_factor(float ml)` | Set maximum load factor | ml | void | O(1) |
| `rehash(size_type n)` | Set number of buckets | n | void | O(n) |
| `reserve(size_type n)` | Reserve at least n buckets | n | void | O(n) |
| `hash_function()` | Get hash function | None | hasher | O(1) |
| `key_eq()` | Get key equality function | None | key_equal | O(1) |

### Example
```cpp
#include <iostream>
#include <unordered_map>
#include <string>

void printUnorderedMap(const std::unordered_map<std::string, int>& um, const std::string& label) {
    std::cout << label << ":" << std::endl;
    for (const auto& [key, value] : um) {  // Structured binding (C++17)
        std::cout << "  " << key << ": " << value << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    // Declaration and initialization
    std::unordered_map<std::string, int> umap1;  // Empty unordered_map
    std::unordered_map<std::string, int> umap2 = {
        {"apple", 5},
        {"banana", 8},
        {"orange", 10}
    };  // Using initializer list
    
    printUnorderedMap(umap2, "Initial unordered_map");
    
    // Size operations
    std::cout << "Size: " << umap2.size() << std::endl;  // Outputs: 3
    
    // Inserting elements
    umap1.insert(std::pair<std::string, int>("one", 1));
    umap1.insert(std::make_pair("two", 2));  // Using make_pair helper
    umap1["three"] = 3;  // Using subscript operator (inserts if not exists)
    
    printUnorderedMap(umap1, "After inserts");
    
    // Using emplace (constructs in-place)
    umap1.emplace("four", 4);  // More efficient than insert
    
    // Check if element exists
    bool exists = umap1.find("two") != umap1.end();
    std::cout << "Key 'two' exists: " << (exists ? "Yes" : "No") << std::endl;  // Outputs: Yes
    
    // Count occurrences (always 0 or 1 for unordered_map)
    std::cout << "Count of 'five': " << umap1.count("five") << std::endl;  // Outputs: 0
    
    // Using contains (C++20)
    // std::cout << "Contains 'four': " << (umap1.contains("four") ? "Yes" : "No") << std::endl;
    
    // Accessing elements
    std::cout << "Value of 'one': " << umap1["one"] << std::endl;  // Using operator[] (inserts if not exists)
    
    try {
        std::cout << "Value of 'one' with at(): " << umap1.at("one") << std::endl;  // Using at() with bounds checking
        std::cout << "Value of 'five' with at(): " << umap1.at("five") << std::endl;  // Will throw out_of_range
    } catch (const std::out_of_range& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    // Modifying elements
    umap1["one"] = 10;  // Modify existing value
    umap1["five"] = 5;  // Insert new key-value pair
    
    printUnorderedMap(umap1, "After modifications");
    
    // C++17 insert_or_assign (similar to operator[] but returns info)
    auto [it, inserted] = umap1.insert_or_assign("one", 100);
    std::cout << "insert_or_assign: " 
              << (inserted ? "Inserted new element" : "Assigned to existing element")
              << " with key=" << it->first << " value=" << it->second << std::endl;
    
    // C++17 try_emplace (only inserts if the key doesn't exist)
    auto [it2, inserted2] = umap1.try_emplace("one", 200);  // Won't modify existing key
    auto [it3, inserted3] = umap1.try_emplace("six", 6);   // Will insert new key
    
    printUnorderedMap(umap1, "After try_emplace");
    
    // Erasing elements
    umap1.erase("five");  // Erase by key
    
    // Erase using iterator
    auto it4 = umap1.find("four");
    if (it4 != umap1.end()) {
        umap1.erase(it4);
    }
    
    printUnorderedMap(umap1, "After erase");
    
    // Hash table statistics
    std::cout << "Bucket count: " << umap1.bucket_count() << std::endl;
    std::cout << "Load factor: " << umap1.load_factor() << std::endl;
    std::cout << "Max load factor: " << umap1.max_load_factor() << std::endl;
    
    // Show bucket distribution
    std::cout << "Bucket distribution:" << std::endl;
    for (size_t i = 0; i < umap1.bucket_count(); ++i) {
        std::cout << "Bucket " << i << " has " << umap1.bucket_size(i) << " elements" << std::endl;
    }
    
    // Find which bucket a key is in
    std::cout << "Key 'one' is in bucket: " << umap1.bucket("one") << std::endl;
    
    // Controlling rehashing
    umap1.rehash(20);  // Set number of buckets to at least 20
    std::cout << "Bucket count after rehash: " << umap1.bucket_count() << std::endl;
    
    umap1.reserve(100);  // Reserve space for at least 100 elements
    std::cout << "Bucket count after reserve: " << umap1.bucket_count() << std::endl;
    
    // Custom hash function example
    struct CaseInsensitiveHash {
        size_t operator()(const std::string& key) const {
            std::string lower_key = key;
            std::transform(lower_key.begin(), lower_key.end(), lower_key.begin(),
                          [](unsigned char c){ return std::tolower(c); });
            return std::hash<std::string>{}(lower_key);
        }
    };
    
    struct CaseInsensitiveEqual {
        bool operator()(const std::string& left, const std::string& right) const {
            return std::equal(left.begin(), left.end(), right.begin(), right.end(),
                             [](unsigned char a, unsigned char b) {
                                 return std::tolower(a) == std::tolower(b);
                             });
        }
    };
    
    std::unordered_map<std::string, int, CaseInsensitiveHash, CaseInsensitiveEqual> case_insensitive_map;
    case_insensitive_map["Hello"] = 1;
    case_insensitive_map["WORLD"] = 2;
    
    // Now "hello" and "Hello" are considered the same key
    std::cout << "Value for 'hello': " << case_insensitive_map["hello"] << std::endl;  // Outputs: 1
    std::cout << "Value for 'HELLO': " << case_insensitive_map["HELLO"] << std::endl;  // Outputs: 1
    
    // Clearing map
    umap1.clear();
    std::cout << "Size after clear: " << umap1.size() << std::endl;  // Outputs: 0
    
    return 0;
}
```

### Performance Considerations
- Average constant-time complexity for search, insert, erase
- Worst-case linear complexity if many collisions
- No ordering of elements
- Iterators, pointers, and references may be invalidated by rehashing
- operator[] and at() provide convenient access, but with different behavior
- Custom hash functions can significantly impact performance

## Unordered Multimap (`std::unordered_multimap`)

### Description
Similar to unordered_map, but allows multiple elements with the same key.

### Header
```cpp
#include <unordered_map>
```

### Template Parameters
```cpp
template < class Key,
           class T,
           class Hash = std::hash<Key>,
           class KeyEqual = std::equal_to<Key>,
           class Allocator = std::allocator<std::pair<const Key, T>> >
class unordered_multimap;
```
- Same as std::unordered_map

### Key Differences from `std::unordered_map`

| Feature | `std::unordered_map` | `std::unordered_multimap` |
|---------|----------------------|---------------------------|
| Duplicate keys | Not allowed | Allowed |
| `insert()` return | pair<iterator, bool> | iterator |
| `count()` result | Always 0 or 1 | Can be > 1 |
| `operator[]` | Available | Not available |
| `at()` | Available | Not available |

### Example
```cpp
#include <iostream>
#include <unordered_map>
#include <string>

void printUnorderedMultimap(const std::unordered_multimap<std::string, int>& umm, const std::string& label) {
    std::cout << label << ":" << std::endl;
    for (const auto& [key, value] : umm) {  // Structured binding (C++17)
        std::cout << "  " << key << ": " << value << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    // Declaration and initialization
    std::unordered_multimap<std::string, int> ummap1;  // Empty unordered_multimap
    std::unordered_multimap<std::string, int> ummap2 = {
        {"apple", 5},
        {"banana", 8},
        {"apple", 10},  // Duplicate key
        {"orange", 10}
    };
    
    printUnorderedMultimap(ummap2, "Initial unordered_multimap");
    
    // Size operations
    std::cout << "Size: " << ummap2.size() << std::endl;  // Outputs: 4
    
    // Inserting elements
    ummap1.insert(std::pair<std::string, int>("one", 1));
    ummap1.insert(std::make_pair("two", 2));
    ummap1.insert(std::make_pair("one", 10));  // Duplicate key, will be inserted
    
    printUnorderedMultimap(ummap1, "After inserts");
    
    // Using emplace (constructs in-place)
    ummap1.emplace("three", 3);
    ummap1.emplace("three", 30);  // Another entry with the same key
    
    printUnorderedMultimap(ummap1, "After emplace");
    
    // Count occurrences
    std::cout << "Count of 'one': " << ummap1.count("one") << std::endl;  // Outputs: 2
    std::cout << "Count of 'four': " << ummap1.count("four") << std::endl;  // Outputs: 0
    
    // Finding elements
    auto it = ummap1.find("one");  // Points to one occurrence of "one"
    if (it != ummap1.end()) {
        std::cout << "Found: " << it->first << ": " << it->second << std::endl;
    }
    
    // Equal range (returns range of elements with same key)
    auto range = ummap1.equal_range("three");
    std::cout << "Elements with key 'three':" << std::endl;
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << "  " << it->first << ": " << it->second << std::endl;
    }
    std::cout << std::endl;
    
    // Erasing elements
    size_t removed = ummap1.erase("one");  // Erase all occurrences of key "one"
    std::cout << "Number of elements removed: " << removed << std::endl;  // Outputs: 2
    
    printUnorderedMultimap(ummap1, "After erase");
    
    // Erasing a single occurrence
    auto it3 = ummap1.find("three");  // Find first occurrence of "three"
    if (it3 != ummap1.end()) {
        ummap1.erase(it3);  // Erase only this occurrence
    }
    
    printUnorderedMultimap(ummap1, "After single erase");
    
    // Hash table statistics
    std::cout << "Bucket count: " << ummap1.bucket_count() << std::endl;
    std::cout << "Load factor: " << ummap1.load_factor() << std::endl;
    std::cout << "Max load factor: " << ummap1.max_load_factor() << std::endl;
    
    // Controlling rehashing
    ummap1.rehash(20);  // Set number of buckets to at least 20
    std::cout << "Bucket count after rehash: " << ummap1.bucket_count() << std::endl;
    
    // Clearing unordered_multimap
    ummap1.clear();
    std::cout << "Size after clear: " << ummap1.size() << std::endl;  // Outputs: 0
    
    return 0;
}
```

### Performance Considerations
- Similar to std::unordered_map
- Elements with the same key are typically stored in the same bucket
- No direct element access via operator[] or at()
- Must use find(), equal_range() to access elements
- Finding all occurrences of a key is an O(1) operation on average (plus the number of elements with that key)

## Hash Function Customization

To use custom types as keys in unordered containers, you need to provide a hash function and an equality comparison function.

### Example with Custom Type
```cpp
#include <iostream>
#include <unordered_map>
#include <string>

// Custom type
struct Person {
    std::string name;
    int age;
    
    // Define equality operator for comparisons
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

// Custom hash function for Person
struct PersonHash {
    size_t operator()(const Person& p) const {
        // Combine hash of name and age
        size_t h1 = std::hash<std::string>{}(p.name);
        size_t h2 = std::hash<int>{}(p.age);
        return h1 ^ (h2 << 1);  // Simple hash combination technique
    }
};

int main() {
    // Using custom type as key in unordered_map
    std::unordered_map<Person, std::string, PersonHash> person_map;
    
    // Insert elements
    person_map[{"Alice", 30}] = "Engineer";
    person_map[{"Bob", 25}] = "Designer";
    person_map[{"Charlie", 35}] = "Manager";
    
    // Access elements
    Person alice{"Alice", 30};
    std::cout << "Alice's job: " << person_map[alice] << std::endl;
    
    // Check if element exists
    Person dave{"Dave", 40};
    if (person_map.find(dave) == person_map.end()) {
        std::cout << "Dave not found in the map" << std::endl;
    }
    
    // Iterate through the map
    std::cout << "People in the map:" << std::endl;
    for (const auto& [person, job] : person_map) {
        std::cout << "  " << person.name << " (" << person.age << "): " << job << std::endl;
    }
    
    return 0;
}
```

### Alternative Method: Specializing std::hash (C++11 and Later)
```cpp
#include <iostream>
#include <unordered_set>
#include <string>

// Custom type
struct Point {
    int x;
    int y;
    
    // Define equality operator for comparisons
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Specialize std::hash for Point
namespace std {
    template<>
    struct hash<Point> {
        size_t operator()(const Point& p) const {
            // Combine hash of x and y
            size_t h1 = std::hash<int>{}(p.x);
            size_t h2 = std::hash<int>{}(p.y);
            return h1 ^ (h2 << 1);
        }
    };
}

int main() {
    // Now we can use Point as a key without specifying hash function
    std::unordered_set<Point> points;
    
    points.insert({1, 2});
    points.insert({3, 4});
    points.insert({5, 6});
    
    // Check if point exists
    if (points.find({3, 4}) != points.end()) {
        std::cout << "Point (3,4) found in the set" << std::endl;
    }
    
    return 0;
}
```

### Performance Considerations for Hash Functions
- A good hash function should:
  - Distribute keys uniformly across buckets
  - Be fast to compute
  - Generate different hash values for different keys
  - Generate the same hash value for equal keys
- Poor hash functions lead to:
  - Many collisions
  - Degraded performance (from O(1) to O(n) in worst case)
  - Increased memory usage
- For complex types, consider combining hash values of components
- Common techniques for combining hashes:
  - XOR (^) with bit shifts
  - Boost's hash_combine
  - Multiply by prime numbers