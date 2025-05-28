Absolutely! Rust’s standard library provides several core collection types, each with its own set of methods. Below, I’ll cover the most commonly used collections in Rust, their key methods, and provide concise code examples for each, including parameter and return types, and variable explanations.

---

# 1. `Vec<T>` (Growable Vector)

A contiguous growable array type, like `ArrayList` in Java or `std::vector` in C++.

## Common Methods

| Method                | Signature                                              | Description                        |
|-----------------------|-------------------------------------------------------|------------------------------------|
| `new`                 | `fn new() -> Vec<T>`                                  | Creates a new, empty vector        |
| `with_capacity`       | `fn with_capacity(cap: usize) -> Vec<T>`              | Creates with pre-allocated space   |
| `push`                | `fn push(&mut self, value: T)`                        | Appends an element                 |
| `pop`                 | `fn pop(&mut self) -> Option<T>`                      | Removes and returns last element   |
| `get`                 | `fn get(&self, index: usize) -> Option<&T>`           | Gets element by index              |
| `remove`              | `fn remove(&mut self, index: usize) -> T`             | Removes element by index           |
| `insert`              | `fn insert(&mut self, index: usize, element: T)`      | Inserts at index                   |
| `len`                 | `fn len(&self) -> usize`                              | Number of elements                 |
| `is_empty`            | `fn is_empty(&self) -> bool`                          | Checks if empty                    |
| `clear`               | `fn clear(&mut self)`                                 | Removes all elements               |
| `contains`            | `fn contains(&self, x: &T) -> bool`                   | Checks if contains element         |
| `iter`                | `fn iter(&self) -> Iter<T>`                           | Returns iterator                   |

### Example

```rust
fn main() {
    let mut v: Vec<i32> = Vec::new(); // new vector of i32
    v.push(10); // push value 10
    v.push(20);
    let len: usize = v.len(); // get length
    let first: Option<&i32> = v.get(0); // get first element
    let last: Option<i32> = v.pop(); // pop last element
    v.insert(1, 15); // insert 15 at index 1
    let removed: i32 = v.remove(0); // remove element at index 0
    let is_empty: bool = v.is_empty(); // check if empty
    v.clear(); // clear all elements
}
```

---

# 2. `String` (Growable UTF-8 Text)

A heap-allocated, growable string.

## Common Methods

| Method                | Signature                                              | Description                        |
|-----------------------|-------------------------------------------------------|------------------------------------|
| `new`                 | `fn new() -> String`                                  | Creates new empty string           |
| `from`                | `fn from<T: AsRef<str>>(s: T) -> String`              | Creates from &str or String        |
| `push_str`            | `fn push_str(&mut self, string: &str)`                | Appends a string slice             |
| `push`                | `fn push(&mut self, ch: char)`                        | Appends a character                |
| `len`                 | `fn len(&self) -> usize`                              | Length in bytes                    |
| `is_empty`            | `fn is_empty(&self) -> bool`                          | Checks if empty                    |
| `clear`               | `fn clear(&mut self)`                                 | Clears the string                  |
| `replace`             | `fn replace(&self, from: &str, to: &str) -> String`   | Replaces substrings                |
| `contains`            | `fn contains(&self, pat: &str) -> bool`               | Checks for substring               |
| `as_str`              | `fn as_str(&self) -> &str`                            | Returns &str slice                 |

### Example

```rust
fn main() {
    let mut s: String = String::from("Hello");
    s.push(' '); // add space
    s.push_str("World!"); // append string
    let len: usize = s.len(); // get length
    let contains: bool = s.contains("World"); // check substring
    let replaced: String = s.replace("World", "Rust"); // replace substring
    s.clear(); // clear string
}
```

---

# 3. `HashMap<K, V>`

A hash map (dictionary) for key-value pairs.

## Common Methods

| Method                | Signature                                              | Description                        |
|-----------------------|-------------------------------------------------------|------------------------------------|
| `new`                 | `fn new() -> HashMap<K, V>`                           | Creates new empty map              |
| `insert`              | `fn insert(&mut self, k: K, v: V) -> Option<V>`       | Inserts key-value, returns old     |
| `get`                 | `fn get(&self, k: &K) -> Option<&V>`                  | Gets value by key                  |
| `remove`              | `fn remove(&mut self, k: &K) -> Option<V>`            | Removes key, returns value         |
| `contains_key`        | `fn contains_key(&self, k: &K) -> bool`               | Checks if key exists               |
| `len`                 | `fn len(&self) -> usize`                              | Number of entries                  |
| `is_empty`            | `fn is_empty(&self) -> bool`                          | Checks if empty                    |
| `clear`               | `fn clear(&mut self)`                                 | Removes all entries                |
| `iter`                | `fn iter(&self) -> Iter<K, V>`                        | Iterator over key-value pairs      |

### Example

```rust
use std::collections::HashMap;

fn main() {
    let mut map: HashMap<String, i32> = HashMap::new();
    map.insert("a".to_string(), 1); // insert key-value
    map.insert("b".to_string(), 2);
    let value: Option<&i32> = map.get(&"a".to_string()); // get value by key
    let contains: bool = map.contains_key(&"b".to_string()); // check key
    let removed: Option<i32> = map.remove(&"a".to_string()); // remove key
    let len: usize = map.len(); // get length
    map.clear(); // clear all entries
}
```

---

# 4. `HashSet<T>`

A set of unique values.

## Common Methods

| Method                | Signature                                              | Description                        |
|-----------------------|-------------------------------------------------------|------------------------------------|
| `new`                 | `fn new() -> HashSet<T>`                              | Creates new empty set              |
| `insert`              | `fn insert(&mut self, value: T) -> bool`              | Inserts value, returns if new      |
| `contains`            | `fn contains(&self, value: &T) -> bool`               | Checks if value exists             |
| `remove`              | `fn remove(&mut self, value: &T) -> bool`             | Removes value                      |
| `len`                 | `fn len(&self) -> usize`                              | Number of elements                 |
| `is_empty`            | `fn is_empty(&self) -> bool`                          | Checks if empty                    |
| `clear`               | `fn clear(&mut self)`                                 | Removes all elements               |
| `iter`                | `fn iter(&self) -> Iter<T>`                           | Iterator over values               |

### Example

```rust
use std::collections::HashSet;

fn main() {
    let mut set: HashSet<i32> = HashSet::new();
    set.insert(1); // insert value
    set.insert(2);
    let contains: bool = set.contains(&1); // check if contains
    let removed: bool = set.remove(&2); // remove value
    let len: usize = set.len(); // get length
    set.clear(); // clear set
}
```

---

# 5. `VecDeque<T>`

A double-ended queue.

## Common Methods

| Method                | Signature                                              | Description                        |
|-----------------------|-------------------------------------------------------|------------------------------------|
| `new`                 | `fn new() -> VecDeque<T>`                             | Creates new empty deque            |
| `push_back`           | `fn push_back(&mut self, value: T)`                   | Appends to back                    |
| `push_front`          | `fn push_front(&mut self, value: T)`                  | Appends to front                   |
| `pop_back`            | `fn pop_back(&mut self) -> Option<T>`                 | Removes from back                  |
| `pop_front`           | `fn pop_front(&mut self) -> Option<T>`                | Removes from front                 |
| `get`                 | `fn get(&self, index: usize) -> Option<&T>`           | Gets by index                      |
| `len`                 | `fn len(&self) -> usize`                              | Number of elements                 |
| `is_empty`            | `fn is_empty(&self) -> bool`                          | Checks if empty                    |
| `clear`               | `fn clear(&mut self)`                                 | Removes all elements               |

### Example

```rust
use std::collections::VecDeque;

fn main() {
    let mut dq: VecDeque<i32> = VecDeque::new();
    dq.push_back(1); // add to back
    dq.push_front(0); // add to front
    let front: Option<i32> = dq.pop_front(); // remove from front
    let back: Option<i32> = dq.pop_back(); // remove from back
    let len: usize = dq.len(); // get length
    dq.clear(); // clear deque
}
```

---

# 6. `BTreeMap<K, V>`

A sorted map (ordered by key).

## Common Methods

| Method                | Signature                                              | Description                        |
|-----------------------|-------------------------------------------------------|------------------------------------|
| `new`                 | `fn new() -> BTreeMap<K, V>`                          | Creates new empty map              |
| `insert`              | `fn insert(&mut self, k: K, v: V) -> Option<V>`       | Inserts key-value, returns old     |
| `get`                 | `fn get(&self, k: &K) -> Option<&V>`                  | Gets value by key                  |
| `remove`              | `fn remove(&mut self, k: &K) -> Option<V>`            | Removes key, returns value         |
| `contains_key`        | `fn contains_key(&self, k: &K) -> bool`               | Checks if key exists               |
| `len`                 | `fn len(&self) -> usize`                              | Number of entries                  |
| `is_empty`            | `fn is_empty(&self) -> bool`                          | Checks if empty                    |
| `clear`               | `fn clear(&mut self)`                                 | Removes all entries                |
| `iter`                | `fn iter(&self) -> Iter<K, V>`                        | Iterator over key-value pairs      |

### Example

```rust
use std::collections::BTreeMap;

fn main() {
    let mut map: BTreeMap<i32, &str> = BTreeMap::new();
    map.insert(1, "a");
    map.insert(2, "b");
    let value: Option<&&str> = map.get(&1);
    let contains: bool = map.contains_key(&2);
    let removed: Option<&str> = map.remove(&1);
    let len: usize = map.len();
    map.clear();
}
```

---

# 7. `BTreeSet<T>`

A sorted set.

## Common Methods

| Method                | Signature                                              | Description                        |
|-----------------------|-------------------------------------------------------|------------------------------------|
| `new`                 | `fn new() -> BTreeSet<T>`                             | Creates new empty set              |
| `insert`              | `fn insert(&mut self, value: T) -> bool`              | Inserts value, returns if new      |
| `contains`            | `fn contains(&self, value: &T) -> bool`               | Checks if value exists             |
| `remove`              | `fn remove(&mut self, value: &T) -> bool`             | Removes value                      |
| `len`                 | `fn len(&self) -> usize`                              | Number of elements                 |
| `is_empty`            | `fn is_empty(&self) -> bool`                          | Checks if empty                    |
| `clear`               | `fn clear(&mut self)`                                 | Removes all elements               |
| `iter`                | `fn iter(&self) -> Iter<T>`                           | Iterator over values               |

### Example

```rust
use std::collections::BTreeSet;

fn main() {
    let mut set: BTreeSet<i32> = BTreeSet::new();
    set.insert(1);
    set.insert(2);
    let contains: bool = set.contains(&1);
    let removed: bool = set.remove(&2);
    let len: usize = set.len();
    set.clear();
}
```

---

# 8. `LinkedList<T>`

A doubly-linked list.

## Common Methods

| Method                | Signature                                              | Description                        |
|-----------------------|-------------------------------------------------------|------------------------------------|
| `new`                 | `fn new() -> LinkedList<T>`                           | Creates new empty list             |
| `push_back`           | `fn push_back(&mut self, value: T)`                   | Appends to back                    |
| `push_front`          | `fn push_front(&mut self, value: T)`                  | Appends to front                   |
| `pop_back`            | `fn pop_back(&mut self) -> Option<T>`                 | Removes from back                  |
| `pop_front`           | `fn pop_front(&mut self) -> Option<T>`                | Removes from front                 |
| `len`                 | `fn len(&self) -> usize`                              | Number of elements                 |
| `is_empty`            | `fn is_empty(&self) -> bool`                          | Checks if empty                    |
| `clear`               | `fn clear(&mut self)`                                 | Removes all elements               |
| `iter`                | `fn iter(&self) -> Iter<T>`                           | Iterator over values               |

### Example

```rust
use std::collections::LinkedList;

fn main() {
    let mut list: LinkedList<i32> = LinkedList::new();
    list.push_back(1);
    list.push_front(0);
    let front: Option<i32> = list.pop_front();
    let back: Option<i32> = list.pop_back();
    let len: usize = list.len();
    list.clear();
}
```

---

# 9. `BinaryHeap<T>`

A priority queue implemented with a binary heap.

## Common Methods

| Method                | Signature                                              | Description                        |
|-----------------------|-------------------------------------------------------|------------------------------------|
| `new`                 | `fn new() -> BinaryHeap<T>`                           | Creates new empty heap             |
| `push`                | `fn push(&mut self, value: T)`                        | Adds value                         |
| `pop`                 | `fn pop(&mut self) -> Option<T>`                      | Removes greatest value             |
| `peek`                | `fn peek(&self) -> Option<&T>`                        | Returns greatest value             |
| `len`                 | `fn len(&self) -> usize`                              | Number of elements                 |
| `is_empty`            | `fn is_empty(&self) -> bool`                          | Checks if empty                    |
| `clear`               | `fn clear(&mut self)`                                 | Removes all elements               |
| `iter`                | `fn iter(&self) -> Iter<T>`                           | Iterator over values               |

### Example

```rust
use std::collections::BinaryHeap;

fn main() {
    let mut heap: BinaryHeap<i32> = BinaryHeap::new();
    heap.push(5);
    heap.push(1);
    heap.push(10);
    let max: Option<&i32> = heap.peek();
    let popped: Option<i32> = heap.pop();
    let len: usize = heap.len();
    heap.clear();
}
```

---

# Summary Table

| Collection      | Key Methods (not exhaustive)                                                                 |
|-----------------|---------------------------------------------------------------------------------------------|
| `Vec<T>`        | `new`, `push`, `pop`, `get`, `remove`, `insert`, `len`, `is_empty`, `clear`, `contains`    |
| `String`        | `new`, `from`, `push_str`, `push`, `len`, `is_empty`, `clear`, `replace`, `contains`        |
| `HashMap`       | `new`, `insert`, `get`, `remove`, `contains_key`, `len`, `is_empty`, `clear`                |
| `HashSet`       | `new`, `insert`, `contains`, `remove`, `len`, `is_empty`, `clear`                           |
| `VecDeque`      | `new`, `push_back`, `push_front`, `pop_back`, `pop_front`, `get`, `len`, `is_empty`, `clear`|
| `BTreeMap`      | `new`, `insert`, `get`, `remove`, `contains_key`, `len`, `is_empty`, `clear`                |
| `BTreeSet`      | `new`, `insert`, `contains`, `remove`, `len`, `is_empty`, `clear`                           |
| `LinkedList`    | `new`, `push_back`, `push_front`, `pop_back`, `pop_front`, `len`, `is_empty`, `clear`       |
| `BinaryHeap`    | `new`, `push`, `pop`, `peek`, `len`, `is_empty`, `clear`                                    |

---

## Notes

- All collections are generic over their element types (`T`, `K`, `V`).
- Most methods are available via traits like `std::iter::Iterator` for iteration.
- For more advanced usage, see the [Rust std::collections documentation](https://doc.rust-lang.org/std/collections/).

---
---
Here’s a comprehensive guide to **C++ data structures and STL containers**, including their most important methods, required header files, and concise code examples with parameter and return types.  
This covers both **built-in/normal data structures** and **all major STL containers**.

---

# 1. Built-in/Normal Data Structures

## 1.1. Array

- **Header:** None (built-in)
- **Declaration:** `int arr[10];`
- **Key Operations:** Access, assignment, size (fixed at compile time)

```cpp
int arr[5]; // array of 5 ints
arr[0] = 10; // assign
int x = arr[0]; // access
int n = sizeof(arr) / sizeof(arr[0]); // get size
```

---

## 1.2. C-Style String

- **Header:** `<cstring>`
- **Declaration:** `char str[100];`
- **Key Operations:** Copy, compare, length

```cpp
#include <cstring>
char str1[10] = "hello";
char str2[10];
strcpy(str2, str1); // copy
int cmp = strcmp(str1, str2); // compare
size_t len = strlen(str1); // length
```

---

# 2. STL Containers

---

## 2.1. `std::vector<T>`

- **Header:** `<vector>`
- **Dynamic array**

| Method                | Signature (C++)                                   | Description                        |
|-----------------------|--------------------------------------------------|------------------------------------|
| `push_back`           | `void push_back(const T& val)`                   | Add element at end                 |
| `pop_back`            | `void pop_back()`                                | Remove last element                |
| `size`                | `size_t size() const`                            | Number of elements                 |
| `empty`               | `bool empty() const`                             | Check if empty                     |
| `clear`               | `void clear()`                                   | Remove all elements                |
| `at`                  | `T& at(size_t idx)`                              | Access with bounds checking        |
| `operator[]`          | `T& operator[](size_t idx)`                      | Access without bounds checking     |
| `insert`              | `iterator insert(iterator pos, const T& val)`    | Insert at position                 |
| `erase`               | `iterator erase(iterator pos)`                   | Remove at position                 |
| `begin/end`           | `iterator begin()/end()`                         | Iterators                          |

```cpp
#include <vector>
std::vector<int> v;
v.push_back(10);
v.push_back(20);
int x = v[0]; // 10
v.pop_back();
size_t n = v.size();
v.clear();
```

---

## 2.2. `std::string`

- **Header:** `<string>`
- **Dynamic string**

| Method                | Signature                                         | Description                        |
|-----------------------|--------------------------------------------------|------------------------------------|
| `size/length`         | `size_t size() const`                            | Length                             |
| `empty`               | `bool empty() const`                             | Check if empty                     |
| `clear`               | `void clear()`                                   | Remove all characters              |
| `append`              | `string& append(const string& str)`              | Append string                      |
| `push_back`           | `void push_back(char c)`                         | Append character                   |
| `substr`              | `string substr(size_t pos, size_t len)`          | Substring                          |
| `find`                | `size_t find(const string& str)`                 | Find substring                     |
| `replace`             | `string& replace(...)`                           | Replace substring                  |
| `c_str`               | `const char* c_str() const`                      | C-string pointer                   |

```cpp
#include <string>
std::string s = "Hello";
s += " World";
s.push_back('!');
size_t len = s.size();
std::string sub = s.substr(0, 5); // "Hello"
s.clear();
```

---

## 2.3. `std::deque<T>`

- **Header:** `<deque>`
- **Double-ended queue**

| Method                | Signature                                         | Description                        |
|-----------------------|--------------------------------------------------|------------------------------------|
| `push_back`           | `void push_back(const T& val)`                   | Add at end                         |
| `push_front`          | `void push_front(const T& val)`                  | Add at front                       |
| `pop_back`            | `void pop_back()`                                | Remove from end                    |
| `pop_front`           | `void pop_front()`                               | Remove from front                  |
| `size`                | `size_t size() const`                            | Number of elements                 |
| `clear`               | `void clear()`                                   | Remove all elements                |

```cpp
#include <deque>
std::deque<int> dq;
dq.push_back(1);
dq.push_front(0);
dq.pop_back();
dq.pop_front();
```

---

## 2.4. `std::list<T>`

- **Header:** `<list>`
- **Doubly-linked list**

| Method                | Signature                                         | Description                        |
|-----------------------|--------------------------------------------------|------------------------------------|
| `push_back`           | `void push_back(const T& val)`                   | Add at end                         |
| `push_front`          | `void push_front(const T& val)`                  | Add at front                       |
| `pop_back`            | `void pop_back()`                                | Remove from end                    |
| `pop_front`           | `void pop_front()`                               | Remove from front                  |
| `insert`              | `iterator insert(iterator pos, const T& val)`    | Insert at position                 |
| `erase`               | `iterator erase(iterator pos)`                   | Remove at position                 |
| `size`                | `size_t size() const`                            | Number of elements                 |
| `clear`               | `void clear()`                                   | Remove all elements                |

```cpp
#include <list>
std::list<int> l;
l.push_back(1);
l.push_front(0);
l.pop_back();
l.pop_front();
```

---

## 2.5. `std::stack<T>`

- **Header:** `<stack>`
- **LIFO stack (adapter, usually over `deque`)**

| Method                | Signature                                         | Description                        |
|-----------------------|--------------------------------------------------|------------------------------------|
| `push`                | `void push(const T& val)`                        | Add element                        |
| `pop`                 | `void pop()`                                     | Remove top                         |
| `top`                 | `T& top()`                                       | Access top                         |
| `empty`               | `bool empty() const`                             | Check if empty                     |
| `size`                | `size_t size() const`                            | Number of elements                 |

```cpp
#include <stack>
std::stack<int> s;
s.push(1);
s.push(2);
int t = s.top();
s.pop();
```

---

## 2.6. `std::queue<T>`

- **Header:** `<queue>`
- **FIFO queue (adapter, usually over `deque`)**

| Method                | Signature                                         | Description                        |
|-----------------------|--------------------------------------------------|------------------------------------|
| `push`                | `void push(const T& val)`                        | Add element                        |
| `pop`                 | `void pop()`                                     | Remove front                       |
| `front`               | `T& front()`                                     | Access front                       |
| `back`                | `T& back()`                                      | Access back                        |
| `empty`               | `bool empty() const`                             | Check if empty                     |
| `size`                | `size_t size() const`                            | Number of elements                 |

```cpp
#include <queue>
std::queue<int> q;
q.push(1);
q.push(2);
int f = q.front();
q.pop();
```

---

## 2.7. `std::priority_queue<T>`

- **Header:** `<queue>`
- **Max-heap by default**

| Method                | Signature                                         | Description                        |
|-----------------------|--------------------------------------------------|------------------------------------|
| `push`                | `void push(const T& val)`                        | Add element                        |
| `pop`                 | `void pop()`                                     | Remove top                         |
| `top`                 | `T& top()`                                       | Access top                         |
| `empty`               | `bool empty() const`                             | Check if empty                     |
| `size`                | `size_t size() const`                            | Number of elements                 |

```cpp
#include <queue>
std::priority_queue<int> pq;
pq.push(3);
pq.push(1);
pq.push(2);
int t = pq.top(); // 3
pq.pop();
```

---

## 2.8. `std::set<T>`, `std::multiset<T>`

- **Header:** `<set>`
- **Ordered set (unique or duplicate elements)**

| Method                | Signature                                         | Description                        |
|-----------------------|--------------------------------------------------|------------------------------------|
| `insert`              | `iterator insert(const T& val)`                  | Insert element                     |
| `erase`               | `size_t erase(const T& val)`                     | Remove element                     |
| `find`                | `iterator find(const T& val)`                    | Find element                       |
| `count`               | `size_t count(const T& val)`                     | Count occurrences                  |
| `size`                | `size_t size() const`                            | Number of elements                 |
| `clear`               | `void clear()`                                   | Remove all elements                |

```cpp
#include <set>
std::set<int> s;
s.insert(1);
s.insert(2);
bool found = s.find(1) != s.end();
s.erase(2);
```

---

## 2.9. `std::map<K, V>`, `std::multimap<K, V>`

- **Header:** `<map>`
- **Ordered map (unique or duplicate keys)**

| Method                | Signature                                         | Description                        |
|-----------------------|--------------------------------------------------|------------------------------------|
| `insert`              | `pair<iterator, bool> insert(const pair<K,V>&)`  | Insert key-value                   |
| `erase`               | `size_t erase(const K& key)`                     | Remove by key                      |
| `find`                | `iterator find(const K& key)`                    | Find by key                        |
| `count`               | `size_t count(const K& key)`                     | Count occurrences                  |
| `operator[]`          | `V& operator[](const K& key)`                    | Access/insert by key               |
| `size`                | `size_t size() const`                            | Number of elements                 |
| `clear`               | `void clear()`                                   | Remove all elements                |

```cpp
#include <map>
std::map<std::string, int> m;
m["a"] = 1;
m.insert({"b", 2});
int x = m["a"];
m.erase("b");
```

---

## 2.10. `std::unordered_set<T>`, `std::unordered_multiset<T>`

- **Header:** `<unordered_set>`
- **Hash set (unique or duplicate elements, unordered)**

| Method                | Signature                                         | Description                        |
|-----------------------|--------------------------------------------------|------------------------------------|
| `insert`              | `iterator insert(const T& val)`                  | Insert element                     |
| `erase`               | `size_t erase(const T& val)`                     | Remove element                     |
| `find`                | `iterator find(const T& val)`                    | Find element                       |
| `count`               | `size_t count(const T& val)`                     | Count occurrences                  |
| `size`                | `size_t size() const`                            | Number of elements                 |
| `clear`               | `void clear()`                                   | Remove all elements                |

```cpp
#include <unordered_set>
std::unordered_set<int> us;
us.insert(1);
us.insert(2);
bool found = us.find(1) != us.end();
us.erase(2);
```

---

## 2.11. `std::unordered_map<K, V>`, `std::unordered_multimap<K, V>`

- **Header:** `<unordered_map>`
- **Hash map (unique or duplicate keys, unordered)**

| Method                | Signature                                         | Description                        |
|-----------------------|--------------------------------------------------|------------------------------------|
| `insert`              | `pair<iterator, bool> insert(const pair<K,V>&)`  | Insert key-value                   |
| `erase`               | `size_t erase(const K& key)`                     | Remove by key                      |
| `find`                | `iterator find(const K& key)`                    | Find by key                        |
| `count`               | `size_t count(const K& key)`                     | Count occurrences                  |
| `operator[]`          | `V& operator[](const K& key)`                    | Access/insert by key               |
| `size`                | `size_t size() const`                            | Number of elements                 |
| `clear`               | `void clear()`                                   | Remove all elements                |

```cpp
#include <unordered_map>
std::unordered_map<std::string, int> um;
um["a"] = 1;
um.insert({"b", 2});
int x = um["a"];
um.erase("b");
```

---

# Summary Table

| Container                | Header           | Key Methods (not exhaustive)                                  |
|--------------------------|------------------|---------------------------------------------------------------|
| Array                    | -                | `[]`, assignment, `sizeof`                                   |
| C-String                 | `<cstring>`      | `strcpy`, `strcmp`, `strlen`                                 |
| `std::vector`            | `<vector>`       | `push_back`, `pop_back`, `size`, `clear`, `at`, `[]`         |
| `std::string`            | `<string>`       | `size`, `append`, `push_back`, `substr`, `find`, `clear`     |
| `std::deque`             | `<deque>`        | `push_back`, `push_front`, `pop_back`, `pop_front`           |
| `std::list`              | `<list>`         | `push_back`, `push_front`, `pop_back`, `pop_front`           |
| `std::stack`             | `<stack>`        | `push`, `pop`, `top`, `empty`, `size`                        |
| `std::queue`             | `<queue>`        | `push`, `pop`, `front`, `back`, `empty`, `size`              |
| `std::priority_queue`    | `<queue>`        | `push`, `pop`, `top`, `empty`, `size`                        |
| `std::set`/`multiset`    | `<set>`          | `insert`, `erase`, `find`, `count`, `size`, `clear`          |
| `std::map`/`multimap`    | `<map>`          | `insert`, `erase`, `find`, `count`, `[]`, `size`, `clear`    |
| `std::unordered_set`     | `<unordered_set>`| `insert`, `erase`, `find`, `count`, `size`, `clear`          |
| `std::unordered_map`     | `<unordered_map>`| `insert`, `erase`, `find`, `count`, `[]`, `size`, `clear`    |

---

## Notes

- All STL containers support iterators (`begin()`, `end()`) for traversal.
- For more advanced usage, see [cppreference STL documentation](https://en.cppreference.com/w/cpp/container).
- All containers are generic (`template<class T>` or `template<class K, class V>`).

---
---


