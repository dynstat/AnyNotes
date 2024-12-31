### Usage

```rust
use std::collections::{HashMap, HashSet};
```
### Important Note

The behavior while using the `&` symbol as "key" in HashMap depends on whether the type implements the `Copy` trait or not. Let me demonstrate with different types:

```rust
use std::collections::HashMap;

fn main() {
    let mut map = HashMap::new();
    
    // 1. Types that implement Copy (automatically copied):
    let num = 42;          // i32 implements Copy
    let character = 'a';   // char implements Copy
    let boolean = true;    // bool implements Copy
    
    map.insert(num, 1);      // num is copied, original still usable
    println!("num is still here: {}", num);  // Works fine!
    
    // 2. Types that don't implement Copy (ownership is moved):
    let string = String::from("hello");  // String doesn't implement Copy
    let vector = vec![1, 2, 3];          // Vec doesn't implement Copy
    
    map.insert(string, 2);    // ownership of string is moved to map
    // println!("{}", string); // ❌ Error! string was moved
    
    // 3. Using references to avoid moving:
    let string2 = String::from("world");
    map.insert(&string2, 3);   // only inserting a reference
    println!("{}", string2);   // ✅ Works! we still own string2
    
    // 4. Types that implement Clone (must explicitly clone):
    let string3 = String::from("test");
    map.insert(string3.clone(), 4);  // explicitly create a copy
    println!("{}", string3);         // ✅ Works! we used a clone
}
```

Here's a more practical example with our substring problem:

```rust
use std::collections::HashMap;

impl Solution {
    pub fn length_of_longest_substring(s: String) -> i32 {
        let mut char_map = HashMap::new();
        
        // Example with char (implements Copy):
        let c = 'a';
        char_map.insert(c, 1);    // c is copied
        println!("c: {}", c);     // ✅ Still works!
        
        // Example with String (doesn't implement Copy):
        let s1 = String::from("hello");
        char_map.insert(s1, 2);   // s1 is moved
        // println!("{}", s1);     // ❌ Error! s1 was moved
        
        // Three ways to handle non-Copy types:
        
        // 1. Use clone() (makes a copy)
        let s2 = String::from("world");
        char_map.insert(s2.clone(), 3);
        println!("s2: {}", s2);   // ✅ Works because we cloned
        
        // 2. Use reference
        let s3 = String::from("test");
        char_map.insert(&s3, 4);  // insert reference
        println!("s3: {}", s3);   // ✅ Works because we used reference
        
        // 3. Let it move if you don't need the original anymore
        let s4 = String::from("done");
        char_map.insert(s4, 5);   // moved, but that's okay
        
        0 // dummy return
    }
}
```

Quick reference for common types:

```rust
// Types that implement Copy (automatically copied):
- All number types (i32, u64, f32, etc.)
- char
- bool
- tuples if all their components implement Copy
- arrays and fixed-size arrays if elements implement Copy
- &T (references themselves are Copy)

// Types that don't implement Copy (ownership is moved):
- String
- Vec<T>
- HashMap<K, V>
- Most other collection types
- Most custom structs (unless Copy is explicitly derived)
```

When to use what:
```rust
let mut map = HashMap::new();

// For Copy types:
let x = 42;
map.insert(x, 1);         // Simple! x is copied

// For non-Copy types, choose based on needs:
let s = String::from("hello");

// 1. If you need to keep the original:
map.insert(s.clone(), 1); // Make a copy
map.insert(&s, 1);        // Use a reference

// 2. If you don't need the original:
map.insert(s, 1);         // Let it move
```

This is why in our original substring problem (leetcode 3), we could use `char` directly without worrying about ownership - because `char` implements `Copy`!


--- 
---

Now about Rust's ownership rules! Let me break it down:

```rust
use std::collections::HashMap;

fn main() {
    let mut map: HashMap<char, i32> = HashMap::new();
    let c = 'a';

    // Looking up methods (only need to read/reference the key):
    map.contains_key(&c);  // needs & because it only needs to look at the value
    map.get(&c);          // same - only needs to read
    map.get_mut(&c);      // same - only needs to read the key (even though it can modify the value)

    // Insert method (different behavior based on type):
    map.insert(c, 1);     // no & needed because 'char' implements Copy
    println!("{}", c);    // c still usable because it was copied

    // Let's compare with String (which doesn't implement Copy):
    let s = String::from("hello");
    
    map.contains_key(&s); // needs & - only reading
    map.get(&s);         // needs & - only reading
    map.insert(s, 1);    // no & because insert takes ownership
    // println!("{}", s); // ❌ Error! s was moved into the map
}
```

Here's why this happens:

1. **For lookup methods** (`contains_key`, `get`, `get_mut`):
```rust
// Simplified method signatures:
fn contains_key(&self, k: &K) -> bool
fn get(&self, k: &K) -> Option<&V>
fn get_mut(&mut self, k: &K) -> Option<&mut V>

// These only need to read the key to check if it exists
// They don't need ownership of the key
```

2. **For insert method**:
```rust
// Simplified method signature:
fn insert(&mut self, k: K, v: V) -> Option<V>

// This takes ownership of both key and value
// Because they need to be stored in the map
```

Here's a more detailed example showing the difference between `Copy` and non-`Copy` types:

```rust
use std::collections::HashMap;

fn main() {
    let mut map = HashMap::new();

    // With Copy types (like char, i32, bool):
    let num = 42;
    map.contains_key(&num);  // needs reference for lookup
    map.insert(num, 1);      // copies num into map
    println!("{}", num);     // ✅ still works because num was copied

    // With non-Copy types (like String):
    let string = String::from("hello");
    map.contains_key(&string);  // needs reference for lookup
    map.insert(string, 1);      // moves string into map
    // println!("{}", string);   // ❌ Error! string was moved

    // If you need to keep the original String:
    let string2 = String::from("world");
    map.contains_key(&string2);     // reference for lookup
    map.insert(string2.clone(), 1); // clone if you need to keep original
    println!("{}", string2);        // ✅ works because we cloned
}
```

To make it even clearer, here's what happens with different types:

```rust
use std::collections::HashMap;

fn main() {
    let mut map = HashMap::new();

    // 1. Copy types (char, i32, etc.)
    let x = 'a';
    map.insert(x, 1);     // x is copied into map
    println!("{}", x);    // ✅ Still works!

    // 2. Non-Copy types (String, Vec, etc.)
    let s = String::from("hello");
    map.insert(s, 2);     // s is moved into map
    // println!("{}", s); // ❌ Error!

    // 3. Using references
    let s2 = String::from("world");
    map.insert(&s2, 3);   // stores reference instead
    println!("{}", s2);   // ✅ Works!

    // 4. Using clone
    let s3 = String::from("test");
    map.insert(s3.clone(), 4); // stores clone, keeps original
    println!("{}", s3);   // ✅ Works!
}
```

The key points to remember:

1. Lookup methods (`contains_key`, `get`, etc.):
   - Always take references (`&`)
   - Don't need ownership
   - Just need to read the key

2. `insert` method:
   - Takes ownership of key and value
   - For `Copy` types: makes a copy automatically
   - For non-`Copy` types: moves the value into the map
   - Can use `.clone()` if you need to keep the original

This is why in our substring problem, since we're using `char` (which implements `Copy`), we don't need to worry about ownership with `insert`, but we still need references for lookups!

---

We can use `insert` with a reference `&c`, but there are some important considerations. Let me demonstrate:

```rust
use std::collections::HashMap;

fn main() {
    // 1. HashMap with owned types
    let mut map1: HashMap<char, i32> = HashMap::new();
    let c = 'a';
    
    map1.insert(c, 1);     // Works - char is copied
    map1.insert(&c, 1);    // Also works - but unnecessary for Copy types
    
    // 2. HashMap explicitly using references as keys
    let mut map2: HashMap<&char, i32> = HashMap::new();
    
    map2.insert(&c, 1);    // This is natural here because map expects &char
    // map2.insert(c, 1);  // ❌ Error! Map expects &char but got char

    // 3. With String (non-Copy type)
    let mut map3: HashMap<String, i32> = HashMap::new();
    let s = String::from("hello");
    
    map3.insert(s, 1);         // Takes ownership of s
    // map3.insert(&s, 1);     // ❌ Error! Type mismatch: map expects String, not &String
    
    // 4. HashMap with reference keys
    let mut map4: HashMap<&String, i32> = HashMap::new();
    let s2 = String::from("world");
    
    map4.insert(&s2, 1);       // Works - map expects &String
    println!("{}", s2);        // ✅ s2 still usable because we only stored its reference
}
```

Here's a more detailed example showing different scenarios:

```rust
use std::collections::HashMap;

fn main() {
    // Example 1: Regular HashMap with char keys
    let mut regular_map: HashMap<char, i32> = HashMap::new();
    let c = 'a';
    
    // All these work because char implements Copy
    regular_map.insert(c, 1);      // Most straightforward
    regular_map.insert(&c, 1);     // Works but unnecessary
    println!("c is still here: {}", c);  // ✅ Works either way
    
    // Example 2: HashMap with reference keys
    let mut ref_map: HashMap<&char, i32> = HashMap::new();
    
    ref_map.insert(&c, 1);         // Correct - map expects &char
    // ref_map.insert(c, 1);       // ❌ Error! Wrong type
    
    // Example 3: With String (non-Copy type)
    let mut string_map: HashMap<String, i32> = HashMap::new();
    let s = String::from("hello");
    
    string_map.insert(s.clone(), 1);    // Works - clones the string
    // string_map.insert(&s, 1);        // ❌ Error! Wrong type
    
    // Example 4: HashMap with String references
    let mut string_ref_map: HashMap<&String, i32> = HashMap::new();
    
    string_ref_map.insert(&s, 1);       // Works - stores reference
    println!("Can still use s: {}", s);  // ✅ Works because we only stored reference
}
```

Key points:

1. For `Copy` types like `char`:
```rust
let mut map: HashMap<char, i32> = HashMap::new();
let c = 'a';

map.insert(c, 1);     // Preferred - simple and clear
map.insert(&c, 1);    // Works but unnecessary
```

2. For non-`Copy` types like `String`:
```rust
let mut map: HashMap<String, i32> = HashMap::new();
let s = String::from("hello");

map.insert(s, 1);         // Takes ownership
// map.insert(&s, 1);     // ❌ Error! Wrong type

// If you need to keep the original:
map.insert(s.clone(), 1); // Clone if you need to keep original
```

3. If you explicitly want to store references:
```rust
let mut map: HashMap<&String, i32> = HashMap::new();
let s = String::from("hello");

map.insert(&s, 1);        // Correct - storing reference
println!("{}", s);        // ✅ Still have access to s
```

Best practices:
1. For `Copy` types: Use direct values (without `&`)
2. For non-`Copy` types:
   - Use direct values if you don't need the original anymore
   - Use `clone()` if you need to keep the original
   - Use a reference-based HashMap if you need to keep references to the originals

In our substring problem context, since we're using `char` (a `Copy` type), it's clearest and most idiomatic to use:
```rust
char_map.insert(current_char, right);  // Without &
```

----
### Now some more, common examples and use cases


```rust
use std::collections::HashMap;

fn main() {
    // Creating a new empty HashMap
    let mut scores: HashMap<String, i32> = HashMap::new();

    // Inserting key-value pairs
    println!("\n=== Inserting Values ===");
    scores.insert(String::from("Blue"), 10);
    scores.insert(String::from("Red"), 25);
    println!("After inserting: {:?}", scores);

    // Accessing values
    println!("\n=== Accessing Values ===");
    match scores.get("Blue") {
        Some(value) => println!("Blue team's score: {}", value),
        None => println!("Blue team not found"),
    }

    // Using entry API for conditional insertion
    println!("\n=== Using Entry API ===");
    scores.entry(String::from("Yellow")).or_insert(50);
    // Won't update because Blue already exists
    scores.entry(String::from("Blue")).or_insert(50);
    println!("After entry API: {:?}", scores);

    // Updating a value based on old value
    println!("\n=== Updating Values ===");
    let blue_score = scores.entry(String::from("Blue")).or_insert(0);
    *blue_score += 15; // Increment Blue's score by 15
    println!("After updating Blue's score: {:?}", scores);

    // Checking if key exists
    println!("\n=== Checking Keys ===");
    println!("Does Green exist? {}", scores.contains_key("Green"));
    println!("Does Blue exist? {}", scores.contains_key("Blue"));

    // Removing a key-value pair
    println!("\n=== Removing Values ===");
    if let Some(removed_value) = scores.remove("Red") {
        println!("Removed Red's score: {}", removed_value);
    }
    println!("After removal: {:?}", scores);

    // Iterating over key-value pairs
    println!("\n=== Iterating Over HashMap ===");
    for (key, value) in &scores {
        println!("{}: {}", key, value);
    }

    // Getting the number of entries
    println!("\n=== HashMap Size ===");
    println!("Number of teams: {}", scores.len());

    // Clearing the HashMap
    println!("\n=== Clearing HashMap ===");
    scores.clear();
    println!("After clearing: {:?}", scores);

    // Example with custom struct as key
    println!("\n=== Using Custom Types ===");
    #[derive(Hash, Eq, PartialEq, Debug)]
    struct Student {
        id: u32,
        name: String,
    }

    let mut grades: HashMap<Student, char> = HashMap::new();
    let student1 = Student {
        id: 1,
        name: String::from("Alice"),
    };
    
    grades.insert(student1, 'A');
    println!("Student grades: {:?}", grades);
}
```

### **Best Practices for Using and Managing HashMaps in Rust**

HashMaps are a versatile data structure, but they require careful handling to ensure performance, prevent errors, and avoid common pitfalls. Here's a comprehensive guide on best practices, including various cases for usage, adding, updating, and managing data effectively.

---

## **Best Practices**

### **1. Proper Initialization**

- **Empty Initialization**: Use `HashMap::new()` or `HashMap::with_capacity(n)` to avoid resizing overhead if the number of entries is predictable.
    
    ```rust
    use std::collections::HashMap;
    
    let mut map = HashMap::with_capacity(100); // Preallocate memory for 100 entries
    ```
    
    **Why**: Allocating memory upfront improves performance when inserting a large number of elements.
    

---

### **2. Ensure Key Hashing is Consistent**

- **Use types with proper `Eq` and `Hash` traits**:
    
    - Use primitive types like integers, strings, or types that implement the `Hash` and `Eq` traits.
    - Avoid mutable keys or keys that might change while in the HashMap.
    
    ```rust
    let mut map = HashMap::new();
    map.insert(String::from("key1"), 10); // Valid because String implements `Eq` and `Hash`.
    ```
    
    **Why**: Inconsistent hashing or equality logic can lead to incorrect behavior or collisions.
    

---

### **3. Handle Absence of Keys Gracefully**

- Use `get`, `get_mut`, or `entry` APIs to handle absent keys safely without panicking:
    
    - **Check for Existence**:
        
        ```rust
        if map.contains_key("Alice") {
            println!("Key exists");
        }
        ```
        
    - **Retrieve Values Safely**:
        
        ```rust
        match map.get("Alice") {
            Some(value) => println!("Value: {}", value),
            None => println!("Key not found"),
        }
        ```
        
    - **Insert or Modify Values Efficiently**:
        
        ```rust
        map.entry("Alice".to_string()).or_insert(30);
        ```
        
    
    **Why**: Accessing a non-existent key directly can panic or cause bugs.
    

---

### **4. Prefer `entry` API for Insertion/Modification**

- The `entry` API provides an elegant way to insert, update, or conditionally modify values.
    
    #### Insert if Key Does Not Exist:
    
    ```rust
    map.entry("Alice".to_string()).or_insert(30);
    ```
    
    #### Modify an Existing Value:
    
    ```rust
    map.entry("Alice".to_string()).and_modify(|age| *age += 1).or_insert(30);
    ```
    
    **Why**: This avoids redundant lookups and simplifies logic for conditional updates.
    

In the following code:

```rust
map.entry("Alice".to_string()).and_modify(|age| *age += 1).or_insert(30);
```

The `and_modify` method operates on the **value** of the entry, not the key. To understand this, let's break it down:

### **`map.entry("Alice".to_string())`**

This creates an entry for the key `"Alice"` in the map. The result of calling `.entry()` on the map is an `Entry` enum, which can either be an `OccupiedEntry` (if the key exists) or a `VacantEntry` (if the key does not exist).

- **If the key exists**: It provides access to the value associated with the key.
- **If the key doesn't exist**: It allows you to insert a new value for the key.

### **`and_modify(|age| *age += 1)`**

- The `and_modify` method is called on the `Entry` object.
- It is only applicable if the entry already exists (i.e., if the key is already present in the map).
- The closure passed to `and_modify` receives a mutable reference to the **value** of the entry, which allows you to modify it.

Here, `age` is a mutable reference to the value associated with `"Alice"` (which is an `i32`). The closure modifies the value by incrementing it by 1: `*age += 1`.

**Key Point**: `and_modify` always works with the value of the entry, never with the key.

### **`or_insert(30)`**

- This part of the code is executed if the key `"Alice"` is **not found** in the map (i.e., if the entry is vacant).
- The `or_insert` method inserts a value for the key if it doesn't exist, and returns a mutable reference to the value.
- Here, if `"Alice"` is not found, it inserts `30` as the value for `"Alice"`.

### **Putting It All Together**

- If `"Alice"` exists in the map, `and_modify` will increment its associated value by 1.
- If `"Alice"` does not exist, `or_insert(30)` will insert `30` as the value for `"Alice"`.

### **How do we know it's modifying the value?**

- The closure passed to `and_modify` is written as `|age| *age += 1`, where `age` is a reference to the **value** (not the key). The use of `*age` dereferences this reference and allows modification of the value.
- If it were modifying the key, the closure would receive a reference to the key, but since keys are typically immutable and cannot be modified directly, the closure is designed to modify the value instead.

### **Therefore**

- `and_modify` works with the **value** of the entry.
- The `|age| *age += 1` part shows that it's modifying the value.


---

### **5. Handle Hash Collisions**

- Rust's `HashMap` uses a robust hashing algorithm (`SipHash` by default), but:
    
    - For large datasets, use **custom hasher implementations** if needed.
    - Use the `ahash` crate for performance-critical tasks where hashing speed is vital.
    
    ```rust
    use std::collections::HashMap;
    use ahash::AHasher;
    
    let mut map: HashMap<String, i32, std::hash::BuildHasherDefault<AHasher>> = HashMap::default();
    ```
    
    **Why**: Reduces performance issues due to hash collisions.
    

---

### **6. Use Iterators for Bulk Operations**

- Iterate over keys, values, or both for bulk reads or transformations:
    
    ```rust
    for (key, value) in &map {
        println!("{}: {}", key, value);
    }
    ```
    
- **Extend HashMap from Another Source**:
    
    ```rust
    map.extend(vec![("Dave".to_string(), 40), ("Eve".to_string(), 45)]);
    ```
    
    **Why**: Efficient handling of data transformations.
    

---

### **7. Avoid Frequent Resizing**

- Use `HashMap::with_capacity` when the size is predictable.
    
- Check the `capacity` and `len` to determine when resizing occurs.
    
    ```rust
    println!("Capacity: {}", map.capacity());
    ```
    
    **Why**: Reduces memory reallocation overhead.
    

---

### **8. Free Resources Explicitly When Needed**

- Use `HashMap::shrink_to_fit` or drop the map explicitly to release memory.
    
    ```rust
    map.shrink_to_fit();
    ```
    
    **Why**: Ensures memory is not unnecessarily held.
    

---

## **Common Scenarios**

### **1. Creating a Default Value for Missing Keys**

```rust
let value = map.entry("Alice".to_string()).or_insert_with(|| 50);
```

**Why**: This ensures a default value is inserted for missing keys in a lazy manner.

---

### **2. Counting Occurrences of Elements**

```rust
let mut count_map: HashMap<char, usize> = HashMap::new();
let input = "hello world";

for c in input.chars() {
    *count_map.entry(c).or_insert(0) += 1;
}

for (c, count) in &count_map {
    println!("{}: {}", c, count);
}
```

---

### **3. Swapping and Clearing Maps**

```rust
let mut map1: HashMap<String, i32> = HashMap::new();
let mut map2: HashMap<String, i32> = HashMap::new();

map1.insert("Alice".to_string(), 30);
std::mem::swap(&mut map1, &mut map2);

map2.clear();
```

**Why**: Useful for temporary replacements or batch resets.

---

## **Differences Between Ordered and Unordered Maps**

|**Aspect**|**HashMap**|**BTreeMap**|
|---|---|---|
|**Order**|No guarantees (unordered).|Keys are always sorted.|
|**Access Performance**|O(1) average (hashing).|O(log N) (tree traversal).|
|**Insertion Performance**|O(1) average.|O(log N).|
|**Use Case**|Fast lookups where order is irrelevant.|When sorted order of keys is needed.|
|**Memory Usage**|Lower for most cases.|Higher due to tree structure overhead.|

---

### **Conclusion**

- Use `HashMap` for unordered, fast operations.
- Use `BTreeMap` when the key order matters.
- Follow best practices like using `entry` API, handling absent keys, and optimizing memory allocation to avoid common pitfalls.