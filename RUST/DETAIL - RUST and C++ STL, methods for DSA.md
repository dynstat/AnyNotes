Absolutely! Rust’s standard library provides several core collection types, each with its own set of methods. Below, I’ll cover the most commonly used collections in Rust, their key methods, and provide concise code examples for each, including parameter and return types, and variable explanations.

---

# 1. `Vec<T>` (Growable Vector)

A contiguous growable array type, like `ArrayList` in Java or `std::vector` in C++.

## Common Methods

| Method          | Signature                                                    | Description                                                   |     |
| --------------- | ------------------------------------------------------------ | ------------------------------------------------------------- | --- |
| `new`           | `fn new() -> Vec<T>`                                         | Creates a new, empty vector                                   |     |
| `with_capacity` | `fn with_capacity(cap: usize) -> Vec<T>`                     | Creates with pre-allocated space                              |     |
| `push`          | `fn push(&mut self, value: T)`                               | Appends an element to the end                                 |     |
| `pop`           | `fn pop(&mut self) -> Option<T>`                             | Removes and returns the last element (if any)                 |     |
| `get`           | `fn get(&self, index: usize) -> Option<&T>`                  | Gets a reference to an element by index (if any)              |     |
| `get_mut`       | `fn get_mut(&mut self, index: usize) -> Option<&mut T>`      | Gets a mutable reference to an element by index               |     |
| `first`         | `fn first(&self) -> Option<&T>`                              | Returns a reference to the first element (if any)             |     |
| `last`          | `fn last(&self) -> Option<&T>`                               | Returns a reference to the last element (if any)              |     |
| `first_mut`     | `fn first_mut(&mut self) -> Option<&mut T>`                  | Returns a mutable reference to the first element              |     |
| `last_mut`      | `fn last_mut(&mut self) -> Option<&mut T>`                   | Returns a mutable reference to the last element               |     |
| `insert`        | `fn insert(&mut self, index: usize, element: T)`             | Inserts an element at a specific index                        |     |
| `remove`        | `fn remove(&mut self, index: usize) -> T`                    | Removes element by index (panics if out of bounds)            |     |
| `len`           | `fn len(&self) -> usize`                                     | Returns the number of elements                                |     |
| `is_empty`      | `fn is_empty(&self) -> bool`                                 | Checks if the vector is empty                                 |     |
| `clear`         | `fn clear(&mut self)`                                        | Removes all elements                                          |     |
| `truncate`      | `fn truncate(&mut self, len: usize)`                         | Shortens the vector, keeping the first `len` elements         |     |
| `resize`        | `fn resize(&mut self, new_len: usize, value: T)`             | Resizes the vector, filling new spots with `value` (T: Clone) |     |
| `extend`        | `fn extend<I: IntoIterator<Item = T>>(&mut self, iter: I)`   | Extends the vector with an iterator's elements                |     |
| `contains`      | `fn contains(&self, x: &T) -> bool`                          | Checks if vector contains an element (T: PartialEq)           |     |
| `iter`          | `fn iter(&self) -> std::slice::Iter<T>`                      | Returns an iterator over references to elements               |     |
| `iter_mut`      | `fn iter_mut(&mut self) -> std::slice::IterMut<T>`           | Returns an iterator over mutable refs to elements             |     |
| `drain`         | `fn drain<R>(&mut self, range: R) -> std::vec::Drain<T>`     | Creates a draining iterator removing elements in range        |     |
| `retain`        | `fn retain<F>(&mut self, f: F)` where `F: FnMut(&T) -> bool` | Retains only elements for which predicate `f` is true         |     |
| `sort`          | `fn sort(&mut self)` where `T: Ord`                          | Sorts the vector (T: Ord)                                     |     |
| `sort_unstable` | `fn sort_unstable(&mut self)` where `T: Ord`                 | Sorts the vector unstably (T: Ord)                            |     |
| `dedup`         | `fn dedup(&mut self)` where `T: PartialEq`                   | Removes consecutive duplicate elements (T: PartialEq)         |     |

### Example

```rust
fn main() {
    let mut v: Vec<i32> = Vec::new(); // v: Vec<i32>, creates a new, empty vector of i32

    // push: fn push(&mut self, value: T)
    v.push(10); // Appends 10 to v. v is now [10]
    v.push(20); // Appends 20 to v. v is now [10, 20]
    v.push(30); // Appends 30 to v. v is now [10, 20, 30]
    println!("After pushes: {:?}", v); // Output: After pushes: [10, 20, 30]

    // len: fn len(&self) -> usize
    let length: usize = v.len(); // length: usize, gets the number of elements. length is 3.
    println!("Length: {}", length); // Output: Length: 3

    // get: fn get(&self, index: usize) -> Option<&T>
    // Safely get an element by index using match
    let first_element_opt: Option<&i32> = v.get(0); // first_element_opt: Option<&i32>
    match first_element_opt {
        Some(value) => println!("First element (match): {}", value), // value: &i32. Output: First element (match): 10
        None => println!("First element not found."),
    }

    // Using unwrap_or to provide a default value if Option is None
    let non_existent_element: &i32 = v.get(5).unwrap_or(&-1); // non_existent_element: &i32. Index 5 is out of bounds.
    println!("Element at index 5 (unwrap_or): {}", non_existent_element); // Output: Element at index 5 (unwrap_or): -1

    // Using expect to panic with a custom message if Option is None
    // Use expect when you are sure the value should be Some and want to panic with a specific message if it's None.
    // let _ = v.get(5).expect("Element at index 5 should exist!"); // This would panic.
    if v.len() > 0 {
        let first_element_expect: &i32 = v.get(0).expect("Vector should have a first element."); // first_element_expect: &i32
        println!("First element (expect): {}", first_element_expect); // Output: First element (expect): 10
    }

    // Example with first(): fn first(&self) -> Option<&T>
    if let Some(first_val) = v.first() { // first_val: &i32
        println!("First element (using first()): {}", first_val); // Output: First element (using first()): 10
    }

    // pop: fn pop(&mut self) -> Option<T>
    // Safely pop an element using match
    let last_element_option: Option<i32> = v.pop(); // last_element_option: Option<i32>. v is now [10, 20]
    match last_element_option {
        Some(value) => println!("Popped element (match): {}", value), // value: i32. Output: Popped element (match): 30
        None => println!("Vector was empty, nothing to pop."),
    }
    println!("After pop: {:?}", v); // Output: After pop: [10, 20]

    // Using unwrap_or_else to compute a default value if Option is None
    let mut another_vec = vec![5]; // another_vec: Vec<i32>
    let popped_val: i32 = another_vec.pop().unwrap_or_else(|| {
        // This closure is executed only if pop() returns None.
        println!("Vector was empty (or became empty), returning default from unwrap_or_else for pop.");
        -1 // Default value if None
    });
    println!("Popped element (unwrap_or_else): {}", popped_val); // Output: Popped element (unwrap_or_else): 5
    
    let popped_again: i32 = another_vec.pop().unwrap_or_else(|| { // another_vec is now empty
        println!("Vector was empty (or became empty), returning default from unwrap_or_else for pop."); // This will print
        -100 // Default value
    });
    println!("Popped again (unwrap_or_else): {}", popped_again); // Output: Popped again (unwrap_or_else): -100


    // insert: fn insert(&mut self, index: usize, element: T)
    v.insert(1, 15); // Inserts 15 at index 1. v is now [10, 15, 20]
    println!("After insert: {:?}", v); // Output: After insert: [10, 15, 20]

    // remove: fn remove(&mut self, index: usize) -> T
    // Note: remove panics if index is out of bounds. Always check len or use get/pop for safe removal if index might be invalid.
    if v.len() > 0 { // or if index < v.len()
        let removed_element: i32 = v.remove(0); // removed_element: i32. v is now [15, 20]
        println!("Removed element: {}", removed_element); // Output: Removed element: 10
    }
    println!("After remove: {:?}", v); // Output: After remove: [15, 20]

    // contains: fn contains(&self, x: &T) -> bool (where T: PartialEq<T>)
    let has_fifteen: bool = v.contains(&15); // has_fifteen: bool
    println!("Does v contain 15? {}", has_fifteen); // Output: Does v contain 15? true

    // clear: fn clear(&mut self)
    v.clear(); // Removes all elements. v is now []
    println!("After clear, is vector empty? {}", v.is_empty()); // Output: After clear, is vector empty? true

    // Example of iter: fn iter(&self) -> std::slice::Iter<T>
    let mut example_vec = vec![1, 2, 3, 4, 5]; // example_vec: Vec<i32>
    println!("Iterating over example_vec:");
    for item /* item: &i32 */ in example_vec.iter() {
        print!("{} ", item); // Output: 1 2 3 4 5
    }
    println!();

    // Example of retain: fn retain<F>(&mut self, f: F) where F: FnMut(&T) -> bool
    example_vec.retain(|&x| x % 2 == 0); // Retain only even numbers. x: i32 (due to pattern matching &x)
    println!("After retain (even numbers): {:?}", example_vec); // Output: After retain (even numbers): [2, 4]

    // Example of sort: fn sort(&mut self) where T: Ord
    let mut unsorted_vec = vec![3, 1, 4, 1, 5, 9, 2, 6]; // unsorted_vec: Vec<i32>
    unsorted_vec.sort();
    println!("Sorted vector: {:?}", unsorted_vec); // Output: Sorted vector: [1, 1, 2, 3, 4, 5, 6, 9]

    // Example of dedup: fn dedup(&mut self) where T: PartialEq
    // dedup removes *consecutive* duplicates. For global deduplication, sort first.
    unsorted_vec.dedup(); 
    println!("After dedup (on sorted vector): {:?}", unsorted_vec); // Output: After dedup (on sorted vector): [1, 2, 3, 4, 5, 6, 9]
}
```

---

# 2. `String` (Growable UTF-8 Text)

A heap-allocated, growable, UTF-8 encoded string. `String` is to `&str` as `Vec<T>` is to `&[T]`. <mcreference link="https://doc.rust-lang.org/rust-by-example/std/str.html" index="2">2</mcreference>

## Common Methods

| Method                | Signature                                                           | Description                                                                 |
|-----------------------|--------------------------------------------------------------------|-----------------------------------------------------------------------------|
| `new`                 | `fn new() -> String`                                               | Creates a new empty `String`.                                                 |
| `from`                | `fn from(s: &str) -> String`                                       | Creates a new `String` from a string slice (`&str`).                          |
| `to_string`           | `fn to_string(&self) -> String` (via `ToString` trait)             | Converts a `&str` or other types to a `String`.                             |
| `with_capacity`       | `fn with_capacity(capacity: usize) -> String`                      | Creates a new empty `String` with a specified capacity.                     |
| `capacity`            | `fn capacity(&self) -> usize`                                      | Returns the total capacity of the string in bytes.                          |
| `reserve`             | `fn reserve(&mut self, additional: usize)`                         | Ensures that there is capacity for at least `additional` more bytes.        |
| `shrink_to_fit`       | `fn shrink_to_fit(&mut self)`                                      | Shrinks the capacity of the `String` as much as possible.                   |
| `push_str`            | `fn push_str(&mut self, string: &str)`                             | Appends a given string slice onto the end of this `String`. <mcreference link="https://medium.com/@tarungudipalli/exploring-rusts-string-a-comprehensive-guide-with-examples-25f398ade356" index="1">1</mcreference>                 |
| `push`                | `fn push(&mut self, ch: char)`                                     | Appends the given `char` to the end of this `String`.                         |
| `pop`                 | `fn pop(&mut self) -> Option<char>`                                | Removes the last character from the string and returns it. Returns `None` if empty. |
| `remove`              | `fn remove(&mut self, idx: usize) -> char`                         | Removes a character at a specific byte index and returns it. Panics if `idx` is not on a `char` boundary or out of bounds. |
| `truncate`            | `fn truncate(&mut self, new_len: usize)`                           | Shortens the `String` to the specified length. Panics if `new_len` is not on a `char` boundary. |
| `clear`               | `fn clear(&mut self)`                                              | Empties the `String`.                                                       |
| `len`                 | `fn len(&self) -> usize`                                           | Returns the length of the `String` in bytes (not number of characters).     |
| `is_empty`            | `fn is_empty(&self) -> bool`                                       | Returns `true` if the `String` has a length of zero.                        |
| `split_whitespace`    | `fn split_whitespace(&self) -> SplitWhitespace`                    | Splits the string by whitespace. Returns an iterator.                       |
| `split`               | `fn split<'a, P>(&'a self, pat: P) -> Split<'a, P>` where `P: Pattern<'a>` | Splits the string by a pattern. Returns an iterator.                        |
| `replace`             | `fn replace<'a, P>(&'a self, from: P, to: &str) -> String` where `P: Pattern<'a>` | Replaces all matches of a pattern with another string. <mcreference link="https://medium.com/@tarungudipalli/exploring-rusts-string-a-comprehensive-guide-with-examples-25f398ade356" index="1">1</mcreference>                      |
| `replacen`            | `fn replacen<'a, P>(&'a self, pat: P, to: &str, count: usize) -> String` where `P: Pattern<'a>` | Replaces at most `count` matches of a pattern.                              |
| `to_lowercase`        | `fn to_lowercase(&self) -> String`                                 | Returns a new `String` with all alphabetic characters converted to lowercase. |
| `to_uppercase`        | `fn to_uppercase(&self) -> String`                                 | Returns a new `String` with all alphabetic characters converted to uppercase. |
| `trim`                | `fn trim(&self) -> &str`                                           | Returns a string slice with leading and trailing whitespace removed. <mcreference link="https://medium.com/@tarungudipalli/exploring-rusts-string-a-comprehensive-guide-with-examples-25f398ade356" index="1">1</mcreference>        |
| `starts_with`         | `fn starts_with<'a, P>(&'a self, pat: P) -> bool` where `P: Pattern<'a>` | Returns `true` if the string starts with the given pattern.                 |
| `ends_with`           | `fn ends_with<'a, P>(&'a self, pat: P) -> bool` where `P: Pattern<'a>`   | Returns `true` if the string ends with the given pattern.                   |
| `contains`            | `fn contains<'a, P>(&'a self, pat: P) -> bool` where `P: Pattern<'a>`    | Returns `true` if the string contains the given pattern. <mcreference link="https://medium.com/@jasonhz227/understanding-and-manipulating-strings-in-rust-2-20-45b9b36d881b" index="3">3</mcreference>                    |
| `as_str`              | `fn as_str(&self) -> &str`                                         | Extracts a string slice containing the entire `String`.                     |
| `as_bytes`            | `fn as_bytes(&self) -> &[u8]`                                      | Converts the `String` to a byte slice.                                      |
| `chars`               | `fn chars(&self) -> Chars`                                         | Returns an iterator over the `char`s of the string. <mcreference link="https://www.programiz.com/rust/string" index="4">4</mcreference>                         |
| `parse`               | `fn parse<F: FromStr>(&self) -> Result<F, F::Err>`                 | Parses this string slice into another type. <mcreference link="https://medium.com/@tarungudipalli/exploring-rusts-string-a-comprehensive-guide-with-examples-25f398ade356" index="1">1</mcreference>                               |

### Example

```rust
fn main() {
    // Creation
    let mut s1 = String::new(); // s1: String, ""
    s1.push_str("Hello");     // s1: String, "Hello"

    let s2 = String::from(" World"); // s2: String, " World"
    let s3 = "Hello".to_string();    // s3: String, "Hello"

    // Concatenation
    let mut s4 = s3 + &s2; // s4: String, "Hello World" (s3 is moved here)
                         // Note: `add` method for String takes `self` and `&str`
    s4.push('!');        // s4: String, "Hello World!"

    let s5 = format!("{}-{}!", "Rust", "rocks"); // s5: String, "Rust-rocks!"

    // Length and Capacity
    let len = s5.len(); // len: usize, 11 (bytes, not characters for UTF-8)
    println!("Length of s5: {}", len);

    let mut s_cap = String::with_capacity(10);
    println!("Initial capacity of s_cap: {}", s_cap.capacity()); // 10
    s_cap.push_str("Rustacean");
    println!("Capacity after push: {}", s_cap.capacity()); // Might be 10 or more
    s_cap.shrink_to_fit();
    println!("Capacity after shrink: {}", s_cap.capacity()); // Likely 9

    // Modification
    let mut s_mod = String::from("Test String");
    s_mod.replace_range(0..4, "Prod"); // s_mod: "Prod String"
    println!("s_mod after replace_range: {}", s_mod);

    let popped_char = s_mod.pop(); // popped_char: Option<char>, Some('g')
    match popped_char {
        Some(c) => println!("Popped char: {}", c),
        None => println!("String was empty, nothing to pop."),
    }
    // s_mod: "Prod Strin"

    // Careful with remove and truncate due to UTF-8 char boundaries
    let mut s_utf8 = String::from("नमस्ते"); // "नमस्ते" (Namaste in Hindi)
    println!("s_utf8: {}, len: {}", s_utf8, s_utf8.len()); // len is 18 bytes for 6 chars
    // s_utf8.remove(1); // This would panic as 1 is not a char boundary.
    // To safely remove or truncate, ensure you are on a char boundary.
    // For example, to remove the last char:
    if let Some(last_char_idx) = s_utf8.char_indices().last().map(|(i, _)| i) {
        s_utf8.truncate(last_char_idx);
        println!("s_utf8 after truncate last char: {}", s_utf8); // "नमस्त"
    }

    s_mod.clear(); // s_mod: ""
    assert!(s_mod.is_empty());

    // Searching and Checking
    let s_search = String::from("The quick brown fox");
    assert!(s_search.contains("quick"));
    assert!(s_search.starts_with("The"));
    assert!(s_search.ends_with("fox"));

    // Iteration and Slicing
    let s_iter = String::from(" iterating ");
    for word in s_iter.split_whitespace() {
        println!("Word: {}", word); // "iterating"
    }

    for c in s_iter.chars() {
        println!("Char: {}", c);
    }

    let s_slice = &s_search[4..9]; // s_slice: &str, "quick"
    println!("Slice: {}", s_slice);
    // Note: Slicing uses byte indices. Panics if indices are not on char boundaries or out of bounds.

    // Parsing
    let num_str = "42";
    match num_str.parse::<i32>() {
        Ok(n) => println!("Parsed number: {}", n), // n: i32, 42
        Err(e) => println!("Failed to parse: {}", e),
    }

    let float_str = "3.14";
    let pi: f64 = float_str.parse().expect("Failed to parse float"); // pi: f64, 3.14
    println!("Parsed float: {}", pi);

    // Conversion to &str
    let s_to_str = String::from("example");
    let str_ref: &str = s_to_str.as_str();
    println!("String as &str: {}", str_ref);
}
```

---

# 3. `HashMap<K, V>`

A hash map implemented with quadratic probing and SIMD lookup. <mcreference link="https://doc.rust-lang.org/std/collections/struct.HashMap.html" index="1">1</mcreference> It stores key-value pairs, where keys must implement `Eq` and `Hash`. <mcreference link="https://doc.rust-lang.org/rust-by-example/std/hash.html" index="3">3</mcreference>

## Common Methods

| Method                | Signature                                                                 | Description                                                                                                |
|-----------------------|---------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------|
| `new`                 | `fn new() -> HashMap<K, V, S>` (S defaults to `RandomState`)              | Creates an empty `HashMap`. <mcreference link="https://www.programiz.com/rust/hashmap" index="2">2</mcreference>                                                                      |
| `with_capacity`       | `fn with_capacity(capacity: usize) -> HashMap<K, V, S>`                   | Creates an empty `HashMap` with at least the specified capacity. <mcreference link="https://doc.rust-lang.org/std/collections/struct.HashMap.html" index="1">1</mcreference>                               |
| `capacity`            | `fn capacity(&self) -> usize`                                             | Returns the number of elements the map can hold without reallocating.                                      |
| `reserve`             | `fn reserve(&mut self, additional: usize)`                                | Reserves capacity for at least `additional` more elements to be inserted.                                  |
| `shrink_to_fit`       | `fn shrink_to_fit(&mut self)`                                             | Shrinks the capacity of the map as much as possible.                                                       |
| `insert`              | `fn insert(&mut self, k: K, v: V) -> Option<V>`                           | Inserts a key-value pair. If the key already existed, the old value is returned. <mcreference link="https://www.programiz.com/rust/hashmap" index="2">2</mcreference>                         |
| `get`                 | `fn get<Q: ?Sized>(&self, k: &Q) -> Option<&V>` where `K: Borrow<Q>`, `Q: Hash + Eq` | Returns a reference to the value corresponding to the key. Returns `None` if key not found. <mcreference link="https://www.programiz.com/rust/hashmap" index="2">2</mcreference>                |
| `get_mut`             | `fn get_mut<Q: ?Sized>(&mut self, k: &Q) -> Option<&mut V>` where `K: Borrow<Q>`, `Q: Hash + Eq` | Returns a mutable reference to the value corresponding to the key.                                         |
| `get_key_value`       | `fn get_key_value<Q: ?Sized>(&self, k: &Q) -> Option<(&K, &V)>` where `K: Borrow<Q>`, `Q: Hash + Eq` | Returns the key-value pair corresponding to the supplied key.                                              |
| `contains_key`        | `fn contains_key<Q: ?Sized>(&self, k: &Q) -> bool` where `K: Borrow<Q>`, `Q: Hash + Eq` | Returns `true` if the map contains a value for the specified key.                                          |
| `remove`              | `fn remove<Q: ?Sized>(&mut self, k: &Q) -> Option<V>` where `K: Borrow<Q>`, `Q: Hash + Eq` | Removes a key from the map, returning the value at the key if the key was previously in the map. <mcreference link="https://www.programiz.com/rust/hashmap" index="2">2</mcreference>         |
| `remove_entry`        | `fn remove_entry<Q: ?Sized>(&mut self, k: &Q) -> Option<(K, V)>` where `K: Borrow<Q>`, `Q: Hash + Eq` | Removes a key from the map, returning the stored key and value if the key was previously in the map.       |
| `len`                 | `fn len(&self) -> usize`                                                  | Returns the number of elements in the map.                                                                 |
| `is_empty`            | `fn is_empty(&self) -> bool`                                              | Returns `true` if the map contains no elements.                                                            |
| `clear`               | `fn clear(&mut self)`                                                     | Clears the map, removing all key-value pairs.                                                              |
| `iter`                | `fn iter(&self) -> Iter<K, V>`                                            | Returns an iterator visiting all key-value pairs in arbitrary order. Immutable references.                 |
| `iter_mut`            | `fn iter_mut(&mut self) -> IterMut<K, V>`                                 | Returns an iterator visiting all key-value pairs in arbitrary order, with mutable references to the values. |
| `keys`                | `fn keys(&self) -> Keys<K, V>`                                            | Returns an iterator visiting all keys in arbitrary order.                                                  |
| `values`              | `fn values(&self) -> Values<K, V>`                                        | Returns an iterator visiting all values in arbitrary order.                                                |
| `values_mut`          | `fn values_mut(&mut self) -> ValuesMut<K, V>`                             | Returns an iterator visiting all values mutably in arbitrary order.                                        |
| `entry`               | `fn entry(&mut self, key: K) -> Entry<K, V, S>`                           | Gets the given key’s corresponding entry in the map for in-place manipulation. <mcreference link="https://doc.rust-lang.org/std/collections/struct.HashMap.html" index="1">1</mcreference>                           |
| `retain`              | `fn retain<F>(&mut self, f: F)` where `F: FnMut(&K, &mut V) -> bool`      | Retains only the elements specified by the predicate.                                                      |

### Entry API Methods (via `entry()`)

| Method (`entry(key).<method>`) | Signature (Simplified)                                  | Description                                                                                                                               |
|--------------------------------|---------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------|
| `or_insert`                    | `fn or_insert(self, default: V) -> &mut V`              | Inserts `default` if the key does not exist, then returns a mutable reference to the value.                                               |
| `or_insert_with`               | `fn or_insert_with<F: FnOnce() -> V>(self, default: F) -> &mut V` | Inserts the result of `default()` if the key does not exist, then returns a mutable reference. Useful for lazily computed values. <mcreference link="https://doc.rust-lang.org/std/collections/struct.HashMap.html" index="1">1</mcreference> |
| `and_modify`                   | `fn and_modify<F: FnOnce(&mut V)>(self, f: F) -> Self`   | If the entry exists, applies the function `f` to its value.                                                                               |
| `or_default`                   | `fn or_default(self) -> &mut V` (where `V: Default`)    | Inserts the default value of `V` if the key does not exist, then returns a mutable reference.                                             |

### Example

```rust
use std::collections::HashMap;

fn main() {
    // Creation
    let mut scores: HashMap<String, i32> = HashMap::new();

    // Insertion
    scores.insert(String::from("Blue"), 10); // scores: {"Blue": 10}
    scores.insert(String::from("Yellow"), 50);

    // Safe Retrieval using get() and match
    let team_name = String::from("Blue");
    match scores.get(&team_name) { // team_name: &String, value: Option<&i32>
        Some(score) => println!("Score for {}: {}", team_name, score),
        None => println!("Team {} not found.", team_name),
    }

    // Safe Retrieval using get() and if let
    if let Some(score) = scores.get("Yellow") { // score: &i32
        println!("Yellow's score: {}", score);
    }

    // Using contains_key
    let has_red = scores.contains_key("Red"); // has_red: bool, false
    println!("Contains Red team? {}", has_red);

    // Iteration
    println!("All scores:");
    for (key, value) in &scores { // key: &String, value: &i32
        println!("{}: {}", key, value);
    }

    // Entry API: Insert if not present, or update existing
    // Increment score for Blue, or insert 0 if not present
    let blue_score_entry = scores.entry(String::from("Blue")).or_insert(0);
    *blue_score_entry += 10; // blue_score_entry: &mut i32. Blue score is now 20.
    println!("Updated Blue score: {}", blue_score_entry);

    // Insert Green with a default score if it doesn't exist
    scores.entry(String::from("Green")).or_insert_with(|| {
        println!("Green team not found, adding with score 30.");
        30 // value to insert
    });
    // scores: {"Blue": 20, "Yellow": 50, "Green": 30}

    // Modifying a value if it exists using entry().and_modify()
    scores.entry(String::from("Yellow"))
          .and_modify(|score| { *score *= 2; }) // score: &mut i32. Yellow score is now 100.
          .or_insert(0); // This or_insert(0) won't be called if Yellow exists.

    println!("Yellow's score after and_modify: {}", scores["Yellow"]);

    // Removal
    let removed_yellow_score: Option<i32> = scores.remove("Yellow"); // removed_yellow_score: Option<i32>, Some(100)
    match removed_yellow_score {
        Some(score) => println!("Removed Yellow team with score: {}", score),
        None => println!("Yellow team not found for removal."),
    }

    // Length and Clearing
    println!("Number of teams: {}", scores.len()); // len: usize
    scores.clear();
    assert!(scores.is_empty());
    println!("HashMap cleared, is_empty: {}", scores.is_empty());

    // Using get_mut for mutable access
    let mut player_health = HashMap::new();
    player_health.insert(String::from("Alice"), 100);

    if let Some(health) = player_health.get_mut("Alice") { // health: &mut i32
        *health -= 20;
    }
    println!("Alice's health: {:?}", player_health.get("Alice")); // Some(80)

    // Retain: Keep only entries where score > 15 (example if map wasn't cleared)
    let mut high_scores = HashMap::new();
    high_scores.insert(String::from("A"), 10);
    high_scores.insert(String::from("B"), 20);
    high_scores.insert(String::from("C"), 5);
    high_scores.retain(|_key, value| *value > 15);
    // high_scores will now only contain {"B": 20}
    println!("High scores (>15): {:?}", high_scores);
}
```

---

# 4. `HashSet<T>`

A hash set implemented as a `HashMap` where the value is `()`. <mcreference link="https://doc.rust-lang.org/std/collections/struct.HashSet.html" index="1">1</mcreference> It stores unique values. Elements must implement `Eq` and `Hash`. <mcreference link="https://doc.rust-lang.org/std/collections/struct.HashSet.html" index="1">1</mcreference>

## Common Methods

| Method                | Signature                                                                 | Description                                                                                                                               |
|-----------------------|---------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------|
| `new`                 | `fn new() -> HashSet<T, S>` (S defaults to `RandomState`)                 | Creates an empty `HashSet`. <mcreference link="https://www.programiz.com/rust/hashset" index="2">2</mcreference>                                                                                             |
| `with_capacity`       | `fn with_capacity(capacity: usize) -> HashSet<T, S>`                      | Creates an empty `HashSet` with at least the specified capacity. <mcreference link="https://doc.rust-lang.org/std/collections/struct.HashSet.html" index="1">1</mcreference>                                                                |
| `capacity`            | `fn capacity(&self) -> usize`                                             | Returns the number of elements the set can hold without reallocating. <mcreference link="https://doc.rust-lang.org/std/collections/struct.HashSet.html" index="1">1</mcreference>                                                        |
| `reserve`             | `fn reserve(&mut self, additional: usize)`                                | Reserves capacity for at least `additional` more elements to be inserted.                                                                 |
| `shrink_to_fit`       | `fn shrink_to_fit(&mut self)`                                             | Shrinks the capacity of the set as much as possible.                                                                                      |
| `insert`              | `fn insert(&mut self, value: T) -> bool`                                  | Adds a value to the set. Returns `true` if the value was not already present. <mcreference link="https://www.programiz.com/rust/hashset" index="2">2</mcreference>                                                       |
| `contains`            | `fn contains<Q: ?Sized>(&self, value: &Q) -> bool` where `T: Borrow<Q>`, `Q: Hash + Eq` | Returns `true` if the set contains the specified value. <mcreference link="https://www.programiz.com/rust/hashset" index="2">2</mcreference>                                                                     |
| `get`                 | `fn get<Q: ?Sized>(&self, value: &Q) -> Option<&T>` where `T: Borrow<Q>`, `Q: Hash + Eq` | Returns a reference to the value in the set, if any, that is equal to the given value.                                                  |
| `remove`              | `fn remove<Q: ?Sized>(&self, value: &Q) -> bool` where `T: Borrow<Q>`, `Q: Hash + Eq`    | Removes a value from the set. Returns `true` if the value was present. <mcreference link="https://www.programiz.com/rust/hashset" index="2">2</mcreference>                                                              |
| `take`                | `fn take<Q: ?Sized>(&mut self, value: &Q) -> Option<T>` where `T: Borrow<Q>`, `Q: Hash + Eq` | Removes and returns the value in the set, if any, that is equal to the given one.                                                       |
| `len`                 | `fn len(&self) -> usize`                                                  | Returns the number of elements in the set. <mcreference link="https://www.programiz.com/rust/hashset" index="2">2</mcreference>                                                                                    |
| `is_empty`            | `fn is_empty(&self) -> bool`                                              | Returns `true` if the set contains no elements. <mcreference link="https://www.programiz.com/rust/hashset" index="2">2</mcreference>                                                                          |
| `clear`               | `fn clear(&mut self)`                                                     | Clears the set, removing all values. <mcreference link="https://www.programiz.com/rust/hashset" index="2">2</mcreference>                                                                                       |
| `iter`                | `fn iter(&self) -> Iter<T>`                                               | Returns an iterator visiting all elements in arbitrary order. <mcreference link="https://doc.rust-lang.org/std/collections/struct.HashSet.html" index="1">1</mcreference>                                                                 |
| `drain`               | `fn drain(&mut self) -> Drain<T, S>`                                      | Clears the set, returning all elements in an iterator. <mcreference link="https://www.programiz.com/rust/hashset" index="2">2</mcreference>                                                                      |
| `retain`              | `fn retain<F>(&mut self, f: F)` where `F: FnMut(&T) -> bool`               | Retains only the elements specified by the predicate.                                                                                     |
| `is_disjoint`         | `fn is_disjoint(&self, other: &HashSet<T, S>) -> bool`                    | Returns `true` if the set has no elements in common with `other`.                                                                         |
| `is_subset`           | `fn is_subset(&self, other: &HashSet<T, S>) -> bool`                      | Returns `true` if the set is a subset of `other`.                                                                                         |
| `is_superset`         | `fn is_superset(&self, other: &HashSet<T, S>) -> bool`                    | Returns `true` if the set is a superset of `other`.                                                                                       |
| `union`               | `fn union<'a>(&'a self, other: &'a HashSet<T, S>) -> Union<'a, T, S>`     | Returns an iterator over the union of `self` and `other`. <mcreference link="https://www.programiz.com/rust/hashset" index="2">2</mcreference>                                                                       |
| `intersection`        | `fn intersection<'a>(&'a self, other: &'a HashSet<T, S>) -> Intersection<'a, T, S>` | Returns an iterator over the intersection of `self` and `other`. <mcreference link="https://doc.rust-lang.org/rust-by-example/std/hash/hashset.html" index="4">4</mcreference>                                                              |
| `difference`          | `fn difference<'a>(&'a self, other: &'a HashSet<T, S>) -> Difference<'a, T, S>` | Returns an iterator over the difference of `self` and `other` (elements in `self` but not in `other`). <mcreference link="https://doc.rust-lang.org/rust-by-example/std/hash/hashset.html" index="4">4</mcreference>                               |
| `symmetric_difference`| `fn symmetric_difference<'a>(&'a self, other: &'a HashSet<T, S>) -> SymmetricDifference<'a, T, S>` | Returns an iterator over the symmetric difference (elements in one set or the other, but not both). <mcreference link="https://doc.rust-lang.org/rust-by-example/std/hash/hashset.html" index="4">4</mcreference> |

### Example

```rust
use std::collections::HashSet;

fn main() {
    // Creation and Insertion
    let mut set_a: HashSet<i32> = HashSet::new();
    set_a.insert(1); // value: i32, inserted: bool (true)
    set_a.insert(2);
    set_a.insert(3);
    let was_inserted = set_a.insert(1); // value: i32, inserted: bool (false, as 1 already exists)
    println!("Inserting 1 again, was_inserted: {}", was_inserted);

    let mut set_b: HashSet<i32> = HashSet::from([3, 4, 5]); // Create from an array

    // Contains and Get
    if set_a.contains(&2) { // value: &i32, contains_result: bool
        println!("Set A contains 2");
    }
    match set_a.get(&3) { // value: &i32, get_result: Option<&i32>
        Some(val) => println!("Got {} from Set A", val),
        None => println!("Value not found in Set A"),
    }

    // Removal
    let was_removed = set_a.remove(&2); // value: &i32, removed_result: bool (true)
    println!("Removing 2 from Set A, was_removed: {}", was_removed);
    // set_a is now {1, 3}

    // Take (remove and return value)
    let taken_value: Option<i32> = set_b.take(&5); // value: &i32, taken_value: Option<i32> (Some(5))
    println!("Taken value from Set B: {:?}", taken_value);
    // set_b is now {3, 4}

    // Length and Emptiness
    println!("Set A length: {}", set_a.len()); // len: usize
    println!("Is Set B empty? {}", set_b.is_empty()); // is_empty: bool

    // Iteration
    println!("Elements in Set A:");
    for val in set_a.iter() { // val: &i32
        println!("- {}", val);
    }

    // Set Operations
    println!("Set A: {:?}", set_a); // {1, 3}
    println!("Set B: {:?}", set_b); // {3, 4}

    // Union: elements in A or B or both
    let union_set: HashSet<_> = set_a.union(&set_b).collect(); // union_set: HashSet<&i32>
    println!("Union (A | B): {:?}", union_set.iter().map(|&x| x).collect::<HashSet<i32>>()); // {1, 3, 4}

    // Intersection: elements in both A and B
    let intersection_set: HashSet<_> = set_a.intersection(&set_b).collect(); // intersection_set: HashSet<&i32>
    println!("Intersection (A & B): {:?}", intersection_set.iter().map(|&x| x).collect::<HashSet<i32>>()); // {3}

    // Difference: elements in A but not in B
    let difference_set_ab: HashSet<_> = set_a.difference(&set_b).collect(); // difference_set_ab: HashSet<&i32>
    println!("Difference (A - B): {:?}", difference_set_ab.iter().map(|&x| x).collect::<HashSet<i32>>()); // {1}

    // Difference: elements in B but not in A
    let difference_set_ba: HashSet<_> = set_b.difference(&set_a).collect(); // difference_set_ba: HashSet<&i32>
    println!("Difference (B - A): {:?}", difference_set_ba.iter().map(|&x| x).collect::<HashSet<i32>>()); // {4}

    // Symmetric Difference: elements in A or B but not both
    let sym_diff_set: HashSet<_> = set_a.symmetric_difference(&set_b).collect(); // sym_diff_set: HashSet<&i32>
    println!("Symmetric Difference (A ^ B): {:?}", sym_diff_set.iter().map(|&x| x).collect::<HashSet<i32>>()); // {1, 4}

    // Subset and Superset
    let set_c = HashSet::from([1, 3]);
    println!("Is {:?} a subset of {:?}? {}", set_c, set_a, set_c.is_subset(&set_a)); // true
    println!("Is {:?} a superset of {:?}? {}", set_a, set_c, set_a.is_superset(&set_c)); // true

    // Disjoint
    let set_d = HashSet::from([10, 11]);
    println!("Are {:?} and {:?} disjoint? {}", set_a, set_d, set_a.is_disjoint(&set_d)); // true

    // Retain: Keep only elements satisfying a predicate
    let mut numbers = HashSet::from([1, 2, 3, 4, 5, 6]);
    numbers.retain(|&x| x % 2 == 0); // x: &i32. Retain even numbers.
    println!("Retained even numbers: {:?}", numbers); // {2, 4, 6}

    // Clear
    set_a.clear();
    println!("Set A after clear: {:?}, is_empty: {}", set_a, set_a.is_empty());
}
```

---

# 5. `VecDeque<T>`

A double-ended queue implemented with a growable ring buffer. It allows for efficient addition and removal of elements from both ends.

## Common Methods

| Method                | Signature                                                     | Description                                                                 |
|-----------------------|---------------------------------------------------------------|-----------------------------------------------------------------------------|
| `new`                 | `fn new() -> VecDeque<T>`                                     | Creates a new empty deque.                                                  |
| `with_capacity`       | `fn with_capacity(capacity: usize) -> VecDeque<T>`            | Creates an empty deque with space for at least `capacity` elements.         |
| `capacity`            | `fn capacity(&self) -> usize`                                 | Returns the number of elements the deque can hold without reallocating.     |
| `reserve`             | `fn reserve(&mut self, additional: usize)`                    | Reserves capacity for at least `additional` more elements.                  |
| `shrink_to_fit`       | `fn shrink_to_fit(&mut self)`                                 | Shrinks the capacity of the deque as much as possible.                      |
| `push_front`          | `fn push_front(&mut self, value: T)`                          | Adds an element to the front of the deque.                                  |
| `push_back`           | `fn push_back(&mut self, value: T)`                           | Adds an element to the back of the deque.                                   |
| `pop_front`           | `fn pop_front(&mut self) -> Option<T>`                        | Removes and returns the element from the front, or `None` if empty.         |
| `pop_back`            | `fn pop_back(&mut self) -> Option<T>`                         | Removes and returns the element from the back, or `None` if empty.          |
| `front`               | `fn front(&self) -> Option<&T>`                               | Returns a reference to the front element, or `None` if empty.               |
| `back`                | `fn back(&self) -> Option<&T>`                                | Returns a reference to the back element, or `None` if empty.                |
| `front_mut`           | `fn front_mut(&mut self) -> Option<&mut T>`                   | Returns a mutable reference to the front element, or `None` if empty.       |
| `back_mut`            | `fn back_mut(&mut self) -> Option<&mut T>`                    | Returns a mutable reference to the back element, or `None` if empty.        |
| `get`                 | `fn get(&self, index: usize) -> Option<&T>`                   | Returns a reference to the element at `index`, or `None` if out of bounds.  |
| `get_mut`             | `fn get_mut(&mut self, index: usize) -> Option<&mut T>`       | Returns a mutable reference to the element at `index`, or `None` if out of bounds. |
| `len`                 | `fn len(&self) -> usize`                                      | Returns the number of elements in the deque.                                |
| `is_empty`            | `fn is_empty(&self) -> bool`                                  | Returns `true` if the deque contains no elements.                           |
| `clear`               | `fn clear(&mut self)`                                         | Removes all elements from the deque.                                        |
| `insert`              | `fn insert(&mut self, index: usize, element: T)`              | Inserts an element at `index`. Panics if `index > len`.                     |
| `remove`              | `fn remove(&mut self, index: usize) -> Option<T>`             | Removes and returns the element at `index`. Returns `None` if `index` is out of bounds. |
| `swap`                | `fn swap(&mut self, i: usize, j: usize)`                      | Swaps elements at indices `i` and `j`. Panics if out of bounds.             |
| `make_contiguous`     | `fn make_contiguous(&mut self) -> &mut [T]`                   | Rearranges elements to be contiguous and returns a mutable slice.           |
| `rotate_left`         | `fn rotate_left(&mut self, mid: usize)`                       | Rotates the deque to the left by `mid` elements.                            |
| `rotate_right`        | `fn rotate_right(&mut self, mid: usize)`                      | Rotates the deque to the right by `mid` elements.                           |
| `iter`                | `fn iter(&self) -> Iter<'_, T>`                               | Returns an iterator over the elements.                                      |
| `iter_mut`            | `fn iter_mut(&mut self) -> IterMut<'_, T>`                    | Returns a mutable iterator over the elements.                               |
| `drain`               | `fn drain<R>(&mut self, range: R) -> Drain<'_, T, A>`          | Creates a draining iterator that removes the specified range and yields the removed items. |
| `extend`              | `fn extend<I: IntoIterator<Item = T>>(&mut self, iter: I)`    | Extends the deque with the contents of an iterator.                         |
| `append`              | `fn append(&mut self, other: &mut VecDeque<T>)`               | Moves all elements from `other` into `self`, leaving `other` empty.         |
| `try_reserve`         | `fn try_reserve(&mut self, additional: usize) -> Result<(), TryReserveError>` | Tries to reserve capacity for at least `additional` more elements. Returns `Err` on allocation failure or overflow. <mcreference link="https://doc.rust-lang.org/std/collections/struct.VecDeque.html" index="1">1</mcreference> |
| `binary_search`       | `fn binary_search(&self, x: &T) -> Result<usize, usize>` (where T: Ord) | Searches for `x` in a sorted deque. Returns `Ok(index)` or `Err(insertion_point)`. See also `partition_point`. <mcreference link="https://doc.rust-lang.org/std/collections/struct.VecDeque.html" index="1">1</mcreference> |

### Example

```rust
use std::collections::VecDeque;

fn main() {
    // Creation
    let mut dq: VecDeque<i32> = VecDeque::new();
    println!("Initial deque: {:?}, len: {}, capacity: {}", dq, dq.len(), dq.capacity());

    let mut dq_cap = VecDeque::with_capacity(2); // Initial capacity
    println!("Deque with capacity: {:?}, len: {}, capacity: {}", dq_cap, dq_cap.len(), dq_cap.capacity());

    // Fallible reservation with try_reserve <mcreference link="https://doc.rust-lang.org/std/collections/struct.VecDeque.html" index="1">1</mcreference>
    match dq_cap.try_reserve(10) {
        Ok(_) => println!("Capacity successfully reserved. New capacity: {}", dq_cap.capacity()),
        Err(e) => println!("Failed to reserve capacity: {:?}", e),
    }
    dq_cap.shrink_to_fit();
    println!("After shrink_to_fit: {:?}, len: {}, capacity: {}", dq_cap, dq_cap.len(), dq_cap.capacity());

    // Adding elements
    dq.push_front(10);
    dq.push_back(20);
    dq.push_front(5);
    dq.push_back(30);
    println!("After pushes: {:?}", dq); // Expected: [5, 10, 20, 30]

    dq.insert(2, 15); // Insert 15 at index 2
    println!("After insert(2, 15): {:?}", dq); // Expected: [5, 10, 15, 20, 30]

    // Accessing elements (safe handling with Option)
    if let Some(first) = dq.front() {
        println!("Front element: {}", first);
    }
    if let Some(val_at_idx) = dq.get(2) {
        println!("Element at index 2: {}", val_at_idx);
    }

    // Modifying elements (safe with Option)
    if let Some(first_mut) = dq.front_mut() {
        *first_mut *= 2; // Modify the front element
    }
    println!("After modifying front: {:?}", dq); // Expected: [10, 10, 15, 20, 30]

    // Removing elements (safe with Option)
    if let Some(popped_front) = dq.pop_front() {
        println!("Popped from front: {}", popped_front);
    }
    println!("After pop_front: {:?}", dq); // Expected: [10, 15, 20, 30]

    if let Some(removed_val) = dq.remove(1) { // Remove element at index 1 (which is 15)
        println!("Removed element at index 1: {}", removed_val);
    }
    println!("After remove(1): {:?}", dq); // Expected: [10, 20, 30]

    // Make contiguous and operate on slice <mcreference link="https://doc.rust-lang.org/std/collections/struct.VecDeque.html" index="1">1</mcreference>
    // Elements might be wrapped around in the ring buffer.
    // make_contiguous() rearranges them into a single slice.
    let slice: &mut [i32] = dq.make_contiguous();
    println!("Made contiguous: {:?}", slice);
    slice.sort(); // Now we can use slice methods like sort
    println!("After sort on contiguous slice: {:?}", dq); // dq reflects the sort. Expected: [10, 20, 30]

    // Binary search on a sorted deque (requires elements to be sorted first) <mcreference link="https://doc.rust-lang.org/std/collections/struct.VecDeque.html" index="1">1</mcreference>
    // Example: dq is now sorted: [10, 20, 30]
    match dq.binary_search(&20) {
        Ok(index) => println!("Found 20 at index: {}", index), // Expected: Ok(1)
        Err(insertion_point) => println!("20 not found, could be inserted at: {}", insertion_point),
    }
    match dq.binary_search(&15) {
        Ok(index) => println!("Found 15 at index: {}", index),
        Err(insertion_point) => println!("15 not found, could be inserted at: {}", insertion_point), // Expected: Err(1)
    }
    // partition_point can also be used on sorted deques to find division points.

    // Length and capacity
    println!("Current length: {}, capacity: {}", dq.len(), dq.capacity());

    // Iteration
    println!("Iterating over elements:");
    for x in dq.iter() {
        print!("{} ", x);
    }
    println!();

    // Mutable iteration
    println!("Iterating and modifying elements (add 1 to each):");
    for x in dq.iter_mut() {
        *x += 1;
    }
    println!("After mutable iteration: {:?}", dq);

    // Rotation
    dq.rotate_left(1);
    println!("After rotate_left(1): {:?}", dq);
    dq.rotate_right(1);
    println!("After rotate_right(1) (back to original sorted + 1): {:?}", dq);

    // Extend and Append
    let mut dq2 = VecDeque::from([100, 200]);
    dq.extend(dq2.iter().copied()); // extend with an iterator
    println!("After extend with dq2: {:?}", dq);
    // dq2 is still [100, 200]

    let mut dq3 = VecDeque::from([300, 400]);
    dq.append(&mut dq3); // moves all elements from dq3
    println!("After append with dq3: {:?}", dq);
    println!("dq3 after append: {:?}", dq3); // Expected: []

    // Clear
    dq.clear();
    println!("After clear: {:?}, len: {}, is_empty: {}", dq, dq.len(), dq.is_empty());

    // Example of `make_contiguous` when elements wrap around
    let mut wrapped_dq: VecDeque<i32> = VecDeque::with_capacity(4);
    wrapped_dq.push_back(1);
    wrapped_dq.push_back(2);
    wrapped_dq.push_back(3);
    wrapped_dq.push_back(4);
    println!("Wrapped_dq initial: {:?}", wrapped_dq); // [1, 2, 3, 4]
    if let Some(_) = wrapped_dq.pop_front() {}
    if let Some(_) = wrapped_dq.pop_front() {}
    // wrapped_dq is now [3, 4] with capacity 4, head might be at index 2
    println!("Wrapped_dq after pops: {:?}", wrapped_dq);
    wrapped_dq.push_back(5); // [3, 4, 5]
    wrapped_dq.push_back(6); // [3, 4, 5, 6]
    // At this point, the internal buffer might look like [5, 6, 3, 4] (example)
    println!("Wrapped_dq before make_contiguous: {:?}", wrapped_dq);
    let contiguous_slice = wrapped_dq.make_contiguous();
    println!("Wrapped_dq after make_contiguous (slice): {:?}", contiguous_slice);
    println!("Wrapped_dq after make_contiguous (deque): {:?}", wrapped_dq);
}
    for x in dq.iter_mut() {
        *x += 1;
    }
    println!("After iter_mut: {:?}", dq);

    // Other operations
    dq.push_back(41); // dq is now [11, 21, 31, 41]
    dq.rotate_left(1); // Rotate left by 1
    // dq is now [21, 31, 41, 11]
    println!("After rotate_left(1): {:?}", dq);

    dq.rotate_right(2); // Rotate right by 2
    // dq is now [41, 11, 21, 31]
    println!("After rotate_right(2): {:?}", dq);

    // Make contiguous and get a slice
    let slice = dq.make_contiguous();
    println!("Contiguous slice: {:?}", slice);
    slice.sort(); // Can sort the slice now
    println!("Sorted contiguous slice (reflects in deque): {:?}", dq);

    // Drain elements
    let drained: Vec<i32> = dq.drain(1..3).collect(); // Drain elements from index 1 up to (but not including) 3
    // Assuming dq was [11, 21, 31, 41] after sort
    // Drained [21, 31]. dq is now [11, 41]
    println!("Drained elements: {:?}, deque after drain: {:?}", drained, dq);

    // Append and Extend
    let mut other_dq = VecDeque::from(vec![100, 200]);
    dq.append(&mut other_dq); // other_dq is now empty
    println!("After append: {:?}, other_dq: {:?}", dq, other_dq);

    dq.extend(vec![300, 400].into_iter());
    println!("After extend: {:?}", dq);

    // Binary search (requires sorted deque)
    dq.make_contiguous().sort(); // Ensure it's sorted for binary_search
    println!("Sorted deque for binary_search: {:?}", dq);
    match dq.binary_search(&41) {
        Ok(pos) => println!("Found 41 at index {}", pos),
        Err(pos) => println!("41 not found, could be inserted at {}", pos),
    }
    match dq.binary_search(&50) {
        Ok(pos) => println!("Found 50 at index {}", pos),
        Err(pos) => println!("50 not found, could be inserted at {}", pos),
    }

    // Clear the deque
    dq.clear();
    println!("After clear: {:?}, is_empty: {}", dq, dq.is_empty());
}
```

---

# 6. `BTreeMap<K, V>`

An ordered map based on a B-Tree. Keys are stored in sorted order. `BTreeMap` is suitable when you need to iterate over keys in a specific order or perform range queries.

## Common Methods

| Method                | Signature                                                            | Description                                                                                          |
|-----------------------|----------------------------------------------------------------------|------------------------------------------------------------------------------------------------------|
| `new`                 | `fn new() -> BTreeMap<K, V>`                                         | Creates a new empty `BTreeMap`.                                                                      |
| `insert`              | `fn insert(&mut self, key: K, value: V) -> Option<V>`                | Inserts a key-value pair. If the key already existed, the old value is returned.                     |
| `get`                 | `fn get<Q: ?Sized>(&self, key: &Q) -> Option<&V>` (where `K: Borrow<Q>`, `Q: Ord`) | Returns a reference to the value corresponding to the key, or `None`.                                |
| `get_mut`             | `fn get_mut<Q: ?Sized>(&mut self, key: &Q) -> Option<&mut V>` (where `K: Borrow<Q>`, `Q: Ord`) | Returns a mutable reference to the value corresponding to the key, or `None`.                        |
| `get_key_value`       | `fn get_key_value<Q: ?Sized>(&self, key: &Q) -> Option<(&K, &V)>` (where `K: Borrow<Q>`, `Q: Ord`) | Returns the key-value pair corresponding to the supplied key.                                        |
| `remove`              | `fn remove<Q: ?Sized>(&mut self, key: &Q) -> Option<V>` (where `K: Borrow<Q>`, `Q: Ord`) | Removes a key from the map, returning the value at the key if the key was previously in the map.     |
| `remove_entry`        | `fn remove_entry<Q: ?Sized>(&mut self, key: &Q) -> Option<(K, V)>` (where `K: Borrow<Q>`, `Q: Ord`) | Removes a key from the map, returning the stored key and value if the key was previously in the map. |
| `contains_key`        | `fn contains_key<Q: ?Sized>(&self, key: &Q) -> bool` (where `K: Borrow<Q>`, `Q: Ord`) | Returns `true` if the map contains a value for the specified key.                                    |
| `len`                 | `fn len(&self) -> usize`                                             | Returns the number of elements in the map.                                                           |
| `is_empty`            | `fn is_empty(&self) -> bool`                                         | Returns `true` if the map contains no elements.                                                      |
| `clear`               | `fn clear(&mut self)`                                                | Removes all elements from the map.                                                                   |
| `iter`                | `fn iter(&self) -> Iter<'_, K, V>`                                   | Gets an iterator over the entries of the map, sorted by key.                                         |
| `iter_mut`            | `fn iter_mut(&mut self) -> IterMut<'_, K, V>`                         | Gets a mutable iterator over the entries of the map, sorted by key.                                  |
| `keys`                | `fn keys(&self) -> Keys<'_, K, V>`                                   | Gets an iterator over the keys of the map, in sorted order.                                          |
| `values`              | `fn values(&self) -> Values<'_, K, V>`                               | Gets an iterator over the values of the map, in order by key.                                        |
| `values_mut`          | `fn values_mut(&mut self) -> ValuesMut<'_, K, V>`                     | Gets a mutable iterator over the values of the map, in order by key.                                 |
| `entry`               | `fn entry(&mut self, key: K) -> Entry<'_, K, V>`                      | Gets an `Entry` for in-place manipulation of an entry.                                               |
| `first_key_value`     | `fn first_key_value(&self) -> Option<(&K, &V)>`                       | Returns a reference to the first key-value pair in the map, or `None` if it is empty.                |
| `last_key_value`      | `fn last_key_value(&self) -> Option<(&K, &V)>`                        | Returns a reference to the last key-value pair in the map, or `None` if it is empty.                 |
| `pop_first`           | `fn pop_first(&mut self) -> Option<(K, V)>`                           | Removes and returns the first element from the map. Returns `None` if the map is empty.            |
| `pop_last`            | `fn pop_last(&mut self) -> Option<(K, V)>`                            | Removes and returns the last element from the map. Returns `None` if the map is empty.             |
| `range`               | `fn range<R>(&self, range: R) -> Range<'_, K, V>` (where `R: RangeBounds<K>`) | Gets an iterator over a sub-range of entries in the map.                                             |
| `append`              | `fn append(&mut self, other: &mut BTreeMap<K, V>)`                   | Moves all elements from `other` into `Self`, leaving `other` empty.                                  |

### Example

```rust
use std::collections::BTreeMap;
use std::collections::btree_map::Entry; // Required for the Entry API

fn main() {
    // Creation
    let mut scores = BTreeMap::new();
    println!("Initial map: {:?}, len: {}", scores, scores.len());

    // Insertion
    scores.insert(String::from("Alice"), 95);
    scores.insert(String::from("Bob"), 88);
    scores.insert(String::from("Charlie"), 92);
    println!("After inserts: {:?}", scores);

    // Safe retrieval using Option
    match scores.get("Alice") {
        Some(score) => println!("Alice's score: {}", score),
        None => println!("Alice not found."),
    }

    if let Some(bob_score) = scores.get_mut("Bob") {
        *bob_score += 2; // Bob got 2 bonus points
        println!("Bob's updated score: {}", bob_score);
    }
    println!("Scores after Bob's update: {:?}", scores);

    // Checking for existence
    if scores.contains_key("David") {
        println!("David is in the map.");
    } else {
        println!("David is not in the map.");
    }

    // Removing an entry
    match scores.remove("Charlie") {
        Some(score) => println!("Removed Charlie, who had score: {}", score),
        None => println!("Charlie was not found to remove."),
    }
    println!("Scores after removing Charlie: {:?}", scores);

    // Using the Entry API for conditional updates/insertions
    // Insert if not present, or update if present
    scores.entry(String::from("Alice")).and_modify(|s| *s += 3).or_insert(100);
    scores.entry(String::from("David")).or_insert(75);
    println!("Scores after Entry API usage: {:?}", scores);

    // Get an entry and operate on it
    let eve_entry = scores.entry(String::from("Eve"));
    match eve_entry {
        Entry::Occupied(mut o) => {
            println!("Eve exists with score: {}. Updating to 90.", o.get());
            *o.get_mut() = 90;
        }
        Entry::Vacant(v) => {
            println!("Eve does not exist. Inserting score 80.");
            v.insert(80);
        }
    }
    println!("Scores after Eve's entry manipulation: {:?}", scores);

    // Iteration (BTreeMap iterates in sorted order of keys)
    println!("Iterating over scores (sorted by name):");
    for (name, score) in scores.iter() {
        println!("{}: {}", name, score);
    }

    println!("Iterating over keys:");
    for name in scores.keys() {
        println!("Name: {}", name);
    }

    println!("Iterating over values (mutable):");
    for score in scores.values_mut() {
        *score = (*score as f32 * 1.05) as i32; // Give everyone a 5% boost (approx)
    }
    println!("Scores after 5% boost: {:?}", scores);

    // First and Last elements
    if let Some((name, score)) = scores.first_key_value() {
        println!("First entry (alphabetically): {}: {}", name, score);
    }
    if let Some((name, score)) = scores.last_key_value() {
        println!("Last entry (alphabetically): {}: {}", name, score);
    }

    // Pop first and last
    if let Some((name, score)) = scores.pop_first() {
        println!("Popped first: {}: {}", name, score);
    }
    println!("Scores after pop_first: {:?}", scores);

    // Range queries
    println!("Scores for names between 'B' and 'E':");
    // Note: For String keys, range bounds need to be actual Strings or string slices that can be borrowed.
    for (name, score) in scores.range(String::from("B")..String::from("E")) {
        println!("{}: {}", name, score);
    }

    // Length, emptiness, and clearing
    println!("Current number of entries: {}", scores.len());
    println!("Is the map empty? {}", scores.is_empty());

    scores.clear();
    println!("Map after clear: {:?}, is_empty: {}", scores, scores.is_empty());
}
```

---

# 7. `BTreeSet<T>`

An ordered set based on a B-Tree. Elements are stored in sorted order. `BTreeSet` is suitable when you need to iterate over elements in a specific order or perform set operations efficiently while maintaining order. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BTreeSet.html" index="1">1</mcreference>

## Common Methods

| Method                | Signature                                                            | Description                                                                                                |
|-----------------------|----------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------|
| `new`                 | `fn new() -> BTreeSet<T>`                                            | Creates a new empty `BTreeSet`. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BTreeSet.html" index="1">1</mcreference>                                                              |
| `insert`              | `fn insert(&mut self, value: T) -> bool`                             | Adds a value to the set. Returns `true` if the value was not already present. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BTreeSet.html" index="1">1</mcreference>                             |
| `contains`            | `fn contains<Q: ?Sized>(&self, value: &Q) -> bool` (where `T: Borrow<Q>`, `Q: Ord`) | Returns `true` if the set contains an element equal to the value. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BTreeSet.html" index="1">1</mcreference>                                  |
| `remove`              | `fn remove<Q: ?Sized>(&mut self, value: &Q) -> bool` (where `T: Borrow<Q>`, `Q: Ord`) | Removes a value from the set. Returns `true` if the value was present in the set. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BTreeSet.html" index="1">1</mcreference>                        |
| `get`                 | `fn get<Q: ?Sized>(&self, value: &Q) -> Option<&T>` (where `T: Borrow<Q>`, `Q: Ord`) | Returns a reference to the value in the set, if any, that is equal to the given value.                       |
| `take`                | `fn take<Q: ?Sized>(&mut self, value: &Q) -> Option<T>` (where `T: Borrow<Q>`, `Q: Ord`) | Removes and returns the value in the set, if any, that is equal to the given value.                          |
| `len`                 | `fn len(&self) -> usize`                                             | Returns the number of elements in the set. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BTreeSet.html" index="1">1</mcreference>                                                              |
| `is_empty`            | `fn is_empty(&self) -> bool`                                         | Returns `true` if the set contains no elements. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BTreeSet.html" index="1">1</mcreference>                                                        |
| `clear`               | `fn clear(&mut self)`                                                | Removes all elements from the set. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BTreeSet.html" index="1">1</mcreference>                                                                  |
| `iter`                | `fn iter(&self) -> Iter<'_, T>`                                      | Gets an iterator that visits the elements in the set in ascending order. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BTreeSet.html" index="1">1</mcreference>                                  |
| `first`               | `fn first(&self) -> Option<&T>`                                      | Returns a reference to the first element in the set, or `None` if it is empty. <mcreference link="https://github.com/rust-lang/rust/issues/62924" index="3">3</mcreference>                             |
| `last`                | `fn last(&self) -> Option<&T>`                                       | Returns a reference to the last element in the set, or `None` if it is empty. <mcreference link="https://github.com/rust-lang/rust/issues/62924" index="3">3</mcreference>                              |
| `pop_first`           | `fn pop_first(&mut self) -> Option<T>`                               | Removes and returns the first element from the set. Returns `None` if the set is empty. <mcreference link="https://github.com/rust-lang/rust/issues/62924" index="3">3</mcreference>                   |
| `pop_last`            | `fn pop_last(&mut self) -> Option<T>`                                | Removes and returns the last element from the set. Returns `None` if the set is empty. <mcreference link="https://github.com/rust-lang/rust/issues/62924" index="3">3</mcreference>                    |
| `range`               | `fn range<R>(&self, range: R) -> Range<'_, T>` (where `R: RangeBounds<T>`) | Gets an iterator over a sub-range of elements in the set. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BTreeSet.html" index="1">1</mcreference>                                          |
| `append`              | `fn append(&mut self, other: &mut BTreeSet<T>)`                      | Moves all elements from `other` into `Self`, leaving `other` empty.                                        |
| `split_off`           | `fn split_off<Q: ?Sized>(&mut self, key: &Q) -> BTreeSet<T>` (where `T: Borrow<Q>`, `Q: Ord`) | Splits the set into two at the given key. Returns everything after the key, including the key if present.    |
| `is_disjoint`         | `fn is_disjoint(&self, other: &BTreeSet<T>) -> bool`                 | Returns `true` if the set has no elements in common with `other`. <mcreference link="https://github.com/rust-lang/rust/blob/master/library/alloc/src/collections/btree/set.rs" index="5">5</mcreference>                                  |
| `is_subset`           | `fn is_subset(&self, other: &BTreeSet<T>) -> bool`                   | Returns `true` if the set is a subset of `other`. <mcreference link="https://github.com/rust-lang/rust/blob/master/library/alloc/src/collections/btree/set.rs" index="5">5</mcreference>                                                |
| `is_superset`         | `fn is_superset(&self, other: &BTreeSet<T>) -> bool`                 | Returns `true` if the set is a superset of `other`. <mcreference link="https://github.com/rust-lang/rust/blob/master/library/alloc/src/collections/btree/set.rs" index="5">5</mcreference>                                              |
| `union`               | `fn union<'a>(&'a self, other: &'a BTreeSet<T>) -> Union<'a, T>`     | Visits the elements representing the union, in ascending order. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BTreeSet.html" index="1">1</mcreference>                                      |
| `intersection`        | `fn intersection<'a>(&'a self, other: &'a BTreeSet<T>) -> Intersection<'a, T>` | Visits the elements representing the intersection, in ascending order. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BTreeSet.html" index="1">1</mcreference>                                |
| `difference`          | `fn difference<'a>(&'a self, other: &'a BTreeSet<T>) -> Difference<'a, T>` | Visits the elements representing the difference, in ascending order. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BTreeSet.html" index="1">1</mcreference>                                    |
| `symmetric_difference`| `fn symmetric_difference<'a>(&'a self, other: &'a BTreeSet<T>) -> SymmetricDifference<'a, T>` | Visits the elements representing the symmetric difference, in ascending order. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BTreeSet.html" index="1">1</mcreference>                        |

### Example

```rust
use std::collections::BTreeSet;
use std::ops::Bound::Included;

fn main() {
    // Creation and Insertion
    let mut set1 = BTreeSet::new();
    set1.insert(3);
    set1.insert(1);
    set1.insert(4);
    set1.insert(1); // Duplicates are ignored
    println!("Set 1: {:?}, len: {}", set1, set1.len()); // Elements are sorted: {1, 3, 4}

    // Contains and Get
    if set1.contains(&3) {
        println!("Set 1 contains 3.");
    }
    match set1.get(&4) {
        Some(val) => println!("Got value from Set 1: {}", val),
        None => println!("Value not found in Set 1."),
    }

    // Removal and Take
    let removed = set1.remove(&1);
    println!("Removed 1 from Set 1? {}, Set 1: {:?}", removed, set1);

    if let Some(taken_val) = set1.take(&3) {
        println!("Took value {} from Set 1. Set 1: {:?}", taken_val, set1);
    }

    // Re-populate for set operations
    set1.insert(1);
    set1.insert(2);
    set1.insert(3);
    set1.insert(5);
    println!("Set 1 (re-populated): {:?}", set1); // {1, 2, 3, 5}

    let mut set2 = BTreeSet::new();
    set2.insert(2);
    set2.insert(3);
    set2.insert(4);
    set2.insert(6);
    println!("Set 2: {:?}", set2); // {2, 3, 4, 6}

    // Iteration
    println!("Iterating over Set 1:");
    for val in set1.iter() {
        print!("{} ", val);
    }
    println!();

    // First and Last elements
    if let Some(first) = set1.first() {
        println!("First element of Set 1: {}", first);
    }
    if let Some(last) = set2.last() {
        println!("Last element of Set 2: {}", last);
    }

    // Pop first and last
    let mut set_for_pop = BTreeSet::from([10, 20, 30, 40]);
    if let Some(first_popped) = set_for_pop.pop_first() {
        println!("Popped first from set_for_pop: {}, remaining: {:?}", first_popped, set_for_pop);
    }
    if let Some(last_popped) = set_for_pop.pop_last() {
        println!("Popped last from set_for_pop: {}, remaining: {:?}", last_popped, set_for_pop);
    }

    // Range
    println!("Elements in Set 1 between 2 and 5 (inclusive):");
    for val in set1.range((Included(&2), Included(&5))) {
        print!("{} ", val);
    }
    println!();

    // Set Operations
    // Union: elements in either set1 or set2
    let union_set: BTreeSet<_> = set1.union(&set2).cloned().collect();
    println!("Union (Set 1 | Set 2): {:?}", union_set); // {1, 2, 3, 4, 5, 6}

    // Intersection: elements in both set1 and set2
    let intersection_set: BTreeSet<_> = set1.intersection(&set2).cloned().collect();
    println!("Intersection (Set 1 & Set 2): {:?}", intersection_set); // {2, 3}

    // Difference: elements in set1 but not in set2
    let difference_set: BTreeSet<_> = set1.difference(&set2).cloned().collect();
    println!("Difference (Set 1 - Set 2): {:?}", difference_set); // {1, 5}

    // Symmetric Difference: elements in set1 or set2 but not both
    let sym_diff_set: BTreeSet<_> = set1.symmetric_difference(&set2).cloned().collect();
    println!("Symmetric Difference (Set 1 ^ Set 2): {:?}", sym_diff_set); // {1, 4, 5, 6}

    // Subset, Superset, Disjoint
    let subset_a = BTreeSet::from([1, 2]);
    let subset_b = BTreeSet::from([1, 2, 3]);
    let disjoint_c = BTreeSet::from([10, 11]);

    println!("{:?} is subset of {:?}? {}", subset_a, subset_b, subset_a.is_subset(&subset_b)); // true
    println!("{:?} is superset of {:?}? {}", subset_b, subset_a, subset_b.is_superset(&subset_a)); // true
    println!("{:?} is disjoint with {:?}? {}", subset_a, disjoint_c, subset_a.is_disjoint(&disjoint_c)); // true
    println!("{:?} is disjoint with {:?}? {}", subset_a, subset_b, subset_a.is_disjoint(&subset_b)); // false

    // Append
    let mut set_to_append_to = BTreeSet::from([100, 200]);
    let mut set_to_append_from = BTreeSet::from([300, 400]);
    set_to_append_to.append(&mut set_to_append_from);
    println!("After append: {:?}, appended_from is empty: {}", set_to_append_to, set_to_append_from.is_empty());

    // Split_off
    let mut split_set = BTreeSet::from([1, 2, 3, 4, 5, 6]);
    let split_off_part = split_set.split_off(&4);
    println!("Original set after split_off(&4): {:?}", split_set); // {1, 2, 3}
    println!("Split-off part: {:?}", split_off_part); // {4, 5, 6}

    // Clear and check emptiness
    set1.clear();
    println!("Set 1 after clear: {:?}, is_empty: {}", set1, set1.is_empty());
}
```

---

# 8. `LinkedList<T>`

A doubly-linked list.

## Common Methods

| Method                | Signature                                              | Description                        |
|-----------------------|-------------------------------------------------------|------------------------------------|
| `new`                 | `fn new() -> LinkedList<T>`                           | Creates a new empty `LinkedList`.  |
| `push_back`           | `fn push_back(&mut self, value: T)`                   | Appends an element to the back of the list. |
| `push_front`          | `fn push_front(&mut self, value: T)`                  | Appends an element to the front of the list. |
| `pop_back`            | `fn pop_back(&mut self) -> Option<T>`                 | Removes the last element from the list and returns it, or `None` if it is empty. |
| `pop_front`           | `fn pop_front(&mut self) -> Option<T>`                | Removes the first element from the list and returns it, or `None` if it is empty. |
| `front`               | `fn front(&self) -> Option<&T>`                       | Provides a reference to the front element, or `None` if the list is empty. |
| `back`                | `fn back(&self) -> Option<&T>`                        | Provides a reference to the back element, or `None` if the list is empty. |
| `front_mut`           | `fn front_mut(&mut self) -> Option<&mut T>`           | Provides a mutable reference to the front element, or `None` if the list is empty. |
| `back_mut`            | `fn back_mut(&mut self) -> Option<&mut T>`            | Provides a mutable reference to the back element, or `None` if the list is empty. |
| `len`                 | `fn len(&self) -> usize`                              | Returns the number of elements in the list. |
| `is_empty`            | `fn is_empty(&self) -> bool`                          | Returns `true` if the list contains no elements. |
| `clear`               | `fn clear(&mut self)`                                 | Removes all elements from the list. |
| `contains`            | `fn contains(&self, x: &T) -> bool where T: PartialEq<T>` | Returns `true` if the list contains an element equal to the given value. |
| `append`              | `fn append(&mut self, other: &mut Self)`              | Moves all elements from `other` to the end of the list. `other` is emptied. |
| `iter`                | `fn iter(&self) -> Iter<T>`                           | Returns an iterator over the elements of the list. |
| `iter_mut`            | `fn iter_mut(&mut self) -> IterMut<T>`                | Returns a mutable iterator over the elements of the list. |

### Example

```rust
use std::collections::LinkedList;

fn main() {
    // 1. Creation
    let mut list1: LinkedList<i32> = LinkedList::new();
    println!("Initially, list1 is empty: {}", list1.is_empty()); // true

    // 2. Pushing elements
    list1.push_back(2);    // list1: [2]
    list1.push_front(1); // list1: [1, 2]
    list1.push_back(3);    // list1: [1, 2, 3]
    println!("After pushes, list1: {:?}", list1.iter().collect::<Vec<&i32>>()); // [1, 2, 3]
    println!("Length of list1: {}", list1.len()); // 3

    // 3. Popping elements (safely with Option)
    if let Some(front_val) = list1.pop_front() {
        println!("Popped from front: {}", front_val); // 1
    }
    // list1: [2, 3]
    if let Some(back_val) = list1.pop_back() {
        println!("Popped from back: {}", back_val); // 3
    }
    // list1: [2]
    println!("After pops, list1: {:?}", list1.iter().collect::<Vec<&i32>>()); // [2]

    // 4. Accessing front and back elements (safely with Option)
    list1.push_front(0); // list1: [0, 2]
    list1.push_back(5);  // list1: [0, 2, 5]

    if let Some(front_ref) = list1.front() {
        println!("Front element: {}", front_ref); // 0
    }
    if let Some(back_ref) = list1.back() {
        println!("Back element: {}", back_ref); // 5
    }

    // 5. Mutable access to front and back
    // Ensure there's a non-zero element at the front for a clear modification example
    if list1.front() == Some(&0) { // Check if front is 0
        list1.pop_front(); // remove 0
        list1.push_front(10); // list1: [10, 2, 5]
    } else if list1.is_empty() { // Or if list became empty somehow
        list1.push_front(10);
    }
    
    if let Some(front_mut_ref) = list1.front_mut() {
        *front_mut_ref += 5; 
        println!("Modified front element: {}", front_mut_ref); 
    }
     if let Some(back_mut_ref) = list1.back_mut() {
        *back_mut_ref -= 1;  
        println!("Modified back element: {}", back_mut_ref); 
    }
    println!("After mutable access, list1: {:?}", list1.iter().collect::<Vec<&i32>>()); 

    // 6. Iteration
    println!("Iterating through list1:");
    for val in list1.iter() {
        print!("{} ", val); 
    }
    println!();

    // 7. Mutable iteration
    println!("Doubling each element in list1 using iter_mut:");
    for val_mut in list1.iter_mut() {
        *val_mut *= 2;
    }
    println!("After iter_mut, list1: {:?}", list1.iter().collect::<Vec<&i32>>()); 

    // 8. Contains
    // Ensure list1 has known values for contains check, e.g., after doubling [10,2,5] -> [15,2,4] -> [30,4,8]
    // Example: if list1 was [15, 2, 4] then after doubling it's [30, 4, 8]
    println!("Does list1 contain 4? {}", list1.contains(&4)); 
    println!("Does list1 contain 100? {}", list1.contains(&100)); 

    // 9. Append
    let mut list2: LinkedList<i32> = LinkedList::new();
    list2.push_back(100);
    list2.push_back(200);
    
    list1.append(&mut list2); 
    println!("After appending list2 to list1, list1: {:?}", list1.iter().collect::<Vec<&i32>>());
    println!("list2 is empty after append: {}", list2.is_empty()); // true

    // 10. Clear
    list1.clear();
    println!("After clearing, list1 is empty: {}", list1.is_empty()); // true
    println!("Length of list1 after clear: {}", list1.len()); // 0
}
```

---

# 9. `BinaryHeap<T>`

A priority queue implemented with a binary heap.

## Common Methods

| Method                | Signature                                                     | Description                                                                                                |
|-----------------------|---------------------------------------------------------------|------------------------------------------------------------------------------------------------------------|
| `new`                 | `fn new() -> BinaryHeap<T>`                                   | Creates an empty `BinaryHeap` as a max-heap. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BinaryHeap.html" index="1">1</mcreference> <mcreference link="https://rust-guide.com/en/documentation/collections/BinaryHeap" index="4">4</mcreference>                                                              |
| `with_capacity`       | `fn with_capacity(capacity: usize) -> BinaryHeap<T>`          | Creates an empty `BinaryHeap` with at least the specified capacity. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BinaryHeap.html" index="1">1</mcreference> <mcreference link="https://doc.rust-lang.org/std/collections/binary_heap/struct.BinaryHeap.html" index="5">5</mcreference>                                        |
| `push`                | `fn push(&mut self, value: T)`                                | Adds an element to the `BinaryHeap`. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BinaryHeap.html" index="1">1</mcreference> <mcreference link="https://rust-guide.com/en/documentation/collections/BinaryHeap" index="4">4</mcreference>                                                                        |
| `pop`                 | `fn pop(&mut self) -> Option<T>`                              | Removes the greatest element from the `BinaryHeap` and returns it, or `None` if it is empty. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BinaryHeap.html" index="1">1</mcreference> <mcreference link="https://rust-guide.com/en/documentation/collections/BinaryHeap" index="4">4</mcreference>             |
| `peek`                | `fn peek(&self) -> Option<&T>`                                | Returns a reference to the greatest element in the `BinaryHeap`, or `None` if it is empty. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BinaryHeap.html" index="1">1</mcreference> <mcreference link="https://rust-guide.com/en/documentation/collections/BinaryHeap" index="4">4</mcreference>         |
| `peek_mut`            | `fn peek_mut(&mut self) -> Option<PeekMut<T>>`                | Returns a mutable reference to the greatest item in the binary heap, or `None` if it is empty. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BinaryHeap.html" index="1">1</mcreference> <mcreference link="https://www.gyata.ai/rust/binaryheap" index="2">2</mcreference> |
| `len`                 | `fn len(&self) -> usize`                                      | Returns the number of elements in the `BinaryHeap`. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BinaryHeap.html" index="1">1</mcreference> <mcreference link="https://rust-guide.com/en/documentation/collections/BinaryHeap" index="4">4</mcreference>                                                            |
| `is_empty`            | `fn is_empty(&self) -> bool`                                  | Returns `true` if the `BinaryHeap` contains no elements. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BinaryHeap.html" index="1">1</mcreference>                                                      |
| `clear`               | `fn clear(&mut self)`                                         | Removes all elements from the `BinaryHeap`. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BinaryHeap.html" index="1">1</mcreference>                                                                 |
| `iter`                | `fn iter(&self) -> Iter<T>`                                   | Returns an iterator visiting all values in the underlying vector, in arbitrary order. <mcreference link="https://docs.rs/heapless/latest/heapless/binary_heap/struct.BinaryHeap.html" index="3">3</mcreference>                     |
| `into_vec`            | `fn into_vec(self) -> Vec<T>`                                 | Consumes the `BinaryHeap` and returns the underlying `Vec<T>` in arbitrary order.                               |
| `into_sorted_vec`     | `fn into_sorted_vec(self) -> Vec<T>`                          | Consumes the `BinaryHeap` and returns a `Vec<T>` with elements sorted in descending order.                      |
| `reserve`             | `fn reserve(&mut self, additional: usize)`                    | Reserves capacity for at least `additional` more elements to be inserted in the given `BinaryHeap`. <mcreference link="https://www.gyata.ai/rust/binaryheap" index="2">2</mcreference>        |
| `try_reserve`         | `fn try_reserve(&mut self, additional: usize) -> Result<(), TryReserveError>` | Tries to reserve capacity for at least `additional` more elements. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BinaryHeap.html" index="1">1</mcreference> <mcreference link="https://doc.rust-lang.org/std/collections/binary_heap/struct.BinaryHeap.html" index="5">5</mcreference>                                  |
| `shrink_to_fit`       | `fn shrink_to_fit(&mut self)`                                 | Shrinks the capacity of the `BinaryHeap` as much as possible.                                                |
| `extend`              | `fn extend<I: IntoIterator<Item = T>>(&mut self, iter: I)`    | Extends the heap with the elements from an iterator. <mcreference link="https://doc.rust-lang.org/std/collections/struct.BinaryHeap.html" index="1">1</mcreference>                                                    |

### Example

```rust
use std::collections::BinaryHeap;
use std::cmp::Reverse; // For min-heap behavior

fn main() {
    // 1. Creation (Max-Heap by default)
    let mut max_heap: BinaryHeap<i32> = BinaryHeap::new();
    println!("Max-heap is_empty: {}", max_heap.is_empty()); // true

    // With capacity
    let mut heap_with_cap: BinaryHeap<i32> = BinaryHeap::with_capacity(5);
    println!("Heap with capacity: {}", heap_with_cap.capacity() >= 5); // true

    // 2. Pushing elements (Max-Heap)
    max_heap.push(3);
    max_heap.push(1);
    max_heap.push(4);
    max_heap.push(1);
    max_heap.push(5);
    // max_heap: elements are [5, 3, 4, 1, 1] (internal representation, not sorted order for iteration)
    println!("Max-heap length: {}", max_heap.len()); // 5

    // 3. Peeking and Popping (Max-Heap - safely with Option)
    if let Some(top_val) = max_heap.peek() {
        println!("Max-heap peek: {}", top_val); // 5
    }

    if let Some(popped_val) = max_heap.pop() {
        println!("Max-heap popped: {}", popped_val); // 5
    }
    // max_heap after pop: [4, 3, 1, 1]
    println!("Max-heap after pop, peek: {:?}", max_heap.peek()); // Some(4)

    // 4. Mutable Peek (Max-Heap)
    if let Some(mut top_mut_ref) = max_heap.peek_mut() {
        // PeekMut<T> dereferences to &mut T
        *top_mut_ref += 10; // Modifies the top element (4 -> 14)
        // Note: After modification, the heap property might be violated if not handled carefully.
        // BinaryHeap::sift_down or sift_up might be needed internally if this were a public API for re-heapifying.
        // However, PeekMut itself ensures the heap invariant is maintained after the PeekMut is dropped.
    }
    println!("Max-heap after peek_mut and modification, peek: {:?}", max_heap.peek()); // Some(14) (assuming 4 was at top, now 14)
    // Pop to see the effect
    println!("Max-heap popped after peek_mut: {:?}", max_heap.pop()); // Some(14)

    // 5. Iteration (order is arbitrary, not sorted)
    println!("Max-heap iteration (arbitrary order):");
    for val in max_heap.iter() {
        print!("{} ", val);
    }
    println!(); // e.g., 3 1 1 (order depends on internal structure)

    // 6. Clearing the heap
    max_heap.clear();
    println!("Max-heap is_empty after clear: {}", max_heap.is_empty()); // true

    // 7. Creating a Min-Heap using Reverse
    let mut min_heap: BinaryHeap<Reverse<i32>> = BinaryHeap::new();
    min_heap.push(Reverse(3));
    min_heap.push(Reverse(1));
    min_heap.push(Reverse(4));
    min_heap.push(Reverse(1));
    min_heap.push(Reverse(5));

    if let Some(Reverse(min_val)) = min_heap.peek() {
        println!("Min-heap peek: {}", min_val); // 1
    }
    if let Some(Reverse(popped_min)) = min_heap.pop() {
        println!("Min-heap popped: {}", popped_min); // 1
    }
    println!("Min-heap after pop, peek: {:?}", min_heap.peek().map(|r| r.0)); // Some(1)

    // 8. Extend
    let mut heap_to_extend: BinaryHeap<i32> = BinaryHeap::new();
    heap_to_extend.push(10);
    let data_to_add = vec![20, 5, 15];
    heap_to_extend.extend(data_to_add);
    println!("Heap after extend, peek: {:?}", heap_to_extend.peek()); // Some(20)

    // 9. into_sorted_vec (consumes the heap)
    let sorted_vec = heap_to_extend.into_sorted_vec();
    println!("Heap converted to sorted vec (descending): {:?}", sorted_vec); // [20, 15, 10, 5]
    // heap_to_extend is no longer valid here

    // 10. try_reserve (for fallible allocation)
    let mut another_heap: BinaryHeap<i32> = BinaryHeap::new();
    match another_heap.try_reserve(100) {
        Ok(_) => println!("Successfully reserved space in another_heap."),
        Err(e) => println!("Failed to reserve space: {}", e),
    }
}
```

---

# Summary Table

| Collection      | Key Methods (not exhaustive)                                                                                                                               |
|-----------------|------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `Vec<T>`        | `new`, `with_capacity`, `push`, `pop`, `insert`, `remove`, `get`, `get_mut`, `len`, `is_empty`, `clear`, `contains`, `sort`, `reverse`, `iter`, `iter_mut`, `drain`, `retain`, `split_off` |
| `String`        | `new`, `from`, `with_capacity`, `push_str`, `push`, `pop`, `remove`, `insert_str`, `replace_range`, `len`, `is_empty`, `clear`, `contains`, `split_whitespace`, `lines`, `chars` |
| `HashMap<K, V>` | `new`, `with_capacity`, `insert`, `get`, `get_mut`, `remove`, `contains_key`, `len`, `is_empty`, `clear`, `keys`, `values`, `values_mut`, `iter`, `iter_mut`, `entry`, `extend` |
| `HashSet<T>`    | `new`, `with_capacity`, `insert`, `contains`, `remove`, `get`, `take`, `len`, `is_empty`, `clear`, `iter`, `union`, `intersection`, `difference`, `symmetric_difference`, `is_disjoint`, `is_subset`, `is_superset` |
| `VecDeque<T>`   | `new`, `with_capacity`, `push_back`, `push_front`, `pop_back`, `pop_front`, `get`, `get_mut`, `len`, `is_empty`, `clear`, `contains`, `rotate_left`, `rotate_right`, `iter`, `iter_mut`, `drain`, `append`, `extend` |
| `BTreeMap<K, V>`| `new`, `insert`, `get`, `get_mut`, `remove`, `contains_key`, `len`, `is_empty`, `clear`, `iter`, `iter_mut`, `keys`, `values`, `entry`, `range`, `first_key_value`, `last_key_value`, `pop_first`, `pop_last`, `append` |
| `BTreeSet<T>`   | `new`, `insert`, `contains`, `remove`, `get`, `take`, `len`, `is_empty`, `clear`, `iter`, `range`, `first`, `last`, `pop_first`, `pop_last`, `union`, `intersection`, `difference`, `symmetric_difference`, `is_disjoint`, `is_subset`, `is_superset`, `append`, `split_off` |
| `LinkedList<T>` | `new`, `push_back`, `push_front`, `pop_back`, `pop_front`, `front`, `back`, `front_mut`, `back_mut`, `len`, `is_empty`, `clear`, `contains`, `iter`, `iter_mut`, `append` |
| `BinaryHeap<T>` | `new`, `with_capacity`, `push`, `pop`, `peek`, `peek_mut`, `len`, `is_empty`, `clear`, `iter`, `into_sorted_vec`, `reserve`, `try_reserve`, `extend`                               |


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


