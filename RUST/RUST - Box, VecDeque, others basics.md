Below is a detailed explanation of several fundamental Rust concepts along with simple code examples and analogies to help you visualize what they are and why they’re used.

---

## 1. **Box\<T\>**

### What is a `Box`?
- **Purpose:**  
  In Rust, almost all values are stored on the *stack* by default. A `Box<T>` is a smart pointer that allocates its data on the *heap*. This is useful when you want data to live longer than the current stack frame or when you have a value whose size isn’t known at compile time.

- **Analogy:**  
  Think of a `Box` as a **shipping container**. When you want to move a large object that might not fit in your small backpack (the stack), you pack it in a large crate (the heap) and just carry the crate’s address (the pointer).

- **Simple Example:**  
  Here’s a simple example that “boxes” an integer:

  ```rust:examples/box_example.rs
  fn main() {
      // Allocate an integer on the heap using Box
      let boxed_number = Box::new(5);
      println!("The boxed number is: {}", boxed_number);
  }
  ```

  **Comments in the code:**
  - `Box::new(5)` creates an integer on the heap.
  - We then print the value of the boxed integer.

- **Without `Box`:**  
  Without `Box`, the integer would be stored on the stack. For small, fixed-size data, that is usually fine. However, for large or self-referential structures (like linked lists or trees), you would need to allocate on the heap.

---

## 2. **Option\<T\>**

### What is an `Option`?
- **Purpose:**  
  `Option` is an *enumeration (enum)* that represents a value that might be there or might be missing. It has two variants:
  - `Some(value)`: Indicates a value is present.
  - `None`: Indicates no value.

- **Analogy:**  
  Imagine a cereal box that might contain a **prize**. The prize is only there if you open it—if not, you have an empty box. In Rust, this is handled by `Option`.

- **Simple Example:**

  ```rust:examples/option_example.rs
  fn main() {
      let some_number: Option<i32> = Some(10);
      let no_number: Option<i32> = None;

      // Using match to handle both cases
      match some_number {
          Some(n) => println!("We have a number: {}", n),
          None => println!("No number provided"),
      }

      // You can also use if let for a cleaner syntax if only handling one case:
      if let Some(n) = some_number {
          println!("Using if let: {}", n);
      }
  }
  ```

  **Comments in the code:**
  - We wrap an integer with `Some(10)` to indicate a value is present.
  - `None` represents the absence of a value.
  - The `match` statement lets you “unpack” the Option to safely handle both possibilities.

- **Without `Option`:**  
  Without `Option`, you might return a special value (like `-1` or `null`) to indicate absence. But that approach can lead to errors if the special value could be a valid data point. `Option` forces you to handle the “missing” case explicitly.

---

## 3. **Rc\<T\> (Reference Counting Pointer)**

### What is `Rc`?
- **Purpose:**  
  `Rc<T>` stands for *reference counted*. Use it when multiple parts of your program need shared ownership of the same data. It keeps a count of how many references exist to the value; when the count drops to zero, the value is cleaned up.

- **Analogy:**  
  Imagine you own a popular book in a library. Many people (or sections of the library) want to refer to it. The library keeps a count of how many copies (references) are currently in use. When no one is using the book anymore, it can be shelved away.

- **Simple Example:**

  ```rust:examples/rc_example.rs
  use std::rc::Rc;

  fn main() {
      // Create an Rc pointer to a string
      let shared_string = Rc::new(String::from("Hello, Rc!"));

      // Clone increases the reference count
      let reference1 = Rc::clone(&shared_string);
      let reference2 = Rc::clone(&shared_string);

      println!("Reference count: {}", Rc::strong_count(&shared_string));
      println!("Reference1: {}", reference1);
      println!("Reference2: {}", reference2);
  }
  ```

  **Comments in the code:**
  - `Rc::new(String::from("..."))` creates a reference-counted String.
  - Cloning using `Rc::clone` does not copy the data but only increases the reference count.
  - `Rc::strong_count` shows how many owners are sharing the data.

- **Without `Rc`:**  
  Without `Rc`, you would need to pass ownership around, which can be very complex when many parts of your program need the same data.

---

## 4. **RefCell\<T\>**

### What is `RefCell`?
- **Purpose:**  
  `RefCell<T>` allows for **interior mutability**. This means you can mutate the data even if the `RefCell` itself is not mutable. It enforces borrowing rules at runtime (instead of compile time), thus giving you flexibility at the cost of potential panics if you misuse it.

- **Analogy:**  
  Think of `RefCell` as a safe deposit box with a smart lock. You can open (borrow) the box to change its contents, but the lock ensures that if someone else is already changing the content or if you try to open it for writing twice, the lock will prevent you (or panic).

- **Simple Example:**

  ```rust:examples/refcell_example.rs
  use std::cell::RefCell;

  fn main() {
      // Create a RefCell that holds an integer
      let number = RefCell::new(10);

      {
          // Borrow a mutable reference and change the value
          let mut mutable_ref = number.borrow_mut();
          *mutable_ref += 5;
      } // mutable_ref goes out of scope, releasing the mutable borrow

      // Borrow an immutable reference to read the value
      println!("The value is: {}", number.borrow());
  }
  ```

  **Comments in the code:**
  - `RefCell::new(10)` creates a new `RefCell` holding the value 10.
  - `borrow_mut()` lets you get a mutable reference temporarily.
  - When the mutable reference goes out of scope, you can safely borrow the value immutably.

- **Without `RefCell`:**  
  Typically, Rust enforces borrow rules at compile time, so if you want to mutate something when you only have an immutable reference, you’d run into errors. `RefCell` gives you a workaround—but you have to be careful since errors will show up at runtime instead.

---

## 5. **Collections**

### a. **Vec\<T\>**
- **What is it?**  
  A `Vec` (vector) is a growable array. It’s one of the most commonly used collections in Rust.

- **Analogy:**  
  Think of `Vec` as a **dynamic bookshelf**. You can add more books (values) to the shelf any time, and it automatically adjusts its size.

- **Simple Example:**

  ```rust:examples/vec_example.rs
  fn main() {
      // Create a new empty vector of integers
      let mut numbers: Vec<i32> = Vec::new();

      // Add some numbers to the vector
      numbers.push(1);
      numbers.push(2);
      numbers.push(3);

      println!("The vector is: {:?}", numbers);
  }
  ```

  **Comments in the code:**
  - `Vec::new()` creates an empty vector.
  - `push()` method appends values to the vector.
  - `{:?}` in `println!` is used with vectors (and many other types) because they implement the `Debug` trait.

### b. **VecDeque\<T\>**
- **What is it?**  
  `VecDeque` is a double-ended queue. You can efficiently add or remove items from both the beginning and the end.

- **Analogy:**  
  Imagine a **line at a ticket counter** where occasionally someone might be added at the front (VIP guest) or at the end (regular customer). A `VecDeque` lets you do both quickly.

- **Simple Example:**

  ```rust:examples/vecdeque_example.rs
  use std::collections::VecDeque;

  fn main() {
      // Create a new VecDeque
      let mut deque: VecDeque<i32> = VecDeque::new();
      
      // Push items at the back and the front
      deque.push_back(1);
      deque.push_back(2);
      deque.push_front(0);
      
      println!("VecDeque after push operations: {:?}", deque);
      
      // Remove items from both ends
      let front_item = deque.pop_front();
      let back_item = deque.pop_back();
      
      println!("Popped from front: {:?}", front_item);
      println!("Popped from back: {:?}", back_item);
      println!("VecDeque now: {:?}", deque);
  }
  ```

  **Comments in the code:**
  - `push_front` and `push_back` add items to the beginning and end respectively.
  - `pop_front` and `pop_back` remove items from each end.
  - This is useful when you need to treat a collection as a queue where insertion and removal happen at both ends.

- **Without these Collections:**  
  Without `Vec` or `VecDeque`, you would have to implement your own dynamic array or queue from scratch. This can be error-prone and much more complex, involving manual memory management.

---

## 6. **The `::` Operator (Path Separator)**

### What is the `::` Operator?
- **Purpose:**  
  The double colon `::` is used in Rust to access items (such as functions, types, or constants) within modules or namespaces.

- **Analogy:**  
  Think of it as similar to a **file path on your computer**. For example, if you want a file inside a folder, you might use `folder::file`.

- **Simple Example:**

  ```rust:examples/colon_operator_example.rs
  // We use the standard library's Vec by writing std::vec::Vec,
  // but commonly you would simply write "Vec" because it's in the prelude.
  
  fn main() {
      // Creating a vector using the :: operator to refer to it in the std::vec module.
      let numbers: std::vec::Vec<i32> = vec![1, 2, 3];
      println!("Numbers: {:?}", numbers);
  }
  ```

  **Comments in the code:**
  - `std::vec::Vec` specifies that `Vec` is the type defined within the `vec` module of the `std` (standard library).
  - This helps keep larger projects structured and avoids name conflicts.

---

## 7. **The `#[derive()]` Attribute**

### What is `#[derive()]`?
- **Purpose:**  
  This attribute tells the Rust compiler to automatically generate implementations for certain *traits* (i.e., a set of behaviors) for your types. Common traits include `Debug` (for printing the type), `Clone` (for duplicating the type), and `PartialEq` (for equality checks).

- **Analogy:**  
  Imagine you have a new gadget (your custom type), and you want it to have a **standard set of features** (like a built-in camera or Bluetooth). Instead of building those features from scratch, you ask a manufacturer (the compiler) to add them for you. That’s what `#[derive()]` does.

- **Simple Example:**

  ```rust:examples/derive_example.rs
  #[derive(Debug, Clone, PartialEq)]
  struct Point {
      x: i32,
      y: i32,
  }

  fn main() {
      let p1 = Point { x: 2, y: 3 };
      // Now we can print the point using Debug
      println!("Point: {:?}", p1);

      // We can clone it because of the Clone trait
      let _p2 = p1.clone();

      // And we can compare points because of PartialEq
      assert!(p1 == Point { x: 2, y: 3 });
  }
  ```

  **Comments in the code:**
  - `#[derive(Debug, Clone, PartialEq)]` automatically implements these traits.
  - This saves you from writing boilerplate code.

---

## 8. **`Some` and `None` in the Context of `Option`**

### What are `Some` and `None`?
- **Purpose:**  
  They are the two variants of the `Option` enum:
  - `Some(value)` holds a value.
  - `None` represents the absence of a value.
  
- **Analogy:**  
  As mentioned earlier, imagine opening a cereal box to find a **prize** (`Some(prize)`) or nothing at all (`None`).

- **Simple Example (Recap):**

  ```rust:examples/some_none_example.rs
  fn main() {
      let possible_value: Option<&str> = Some("Hello, world!");
      let no_value: Option<&str> = None;

      // Process the Option using match
      match possible_value {
          Some(msg) => println!("Message: {}", msg),
          None => println!("No message available."),
      }
  }
  ```

  **Comments in the code:**
  - When a value is present, it is wrapped in `Some`.
  - `None` explicitly indicates that there is no value.

---

## **Summary**

- **Box\<T\>:**  
  Allocates data on the heap (like using a shipping container for heavy items).

- **Option\<T\>:**  
  Represents an optional value with either `Some(value)` or `None` (like a prize in a cereal box).

- **Rc\<T\>:**  
  Enables multiple owners for the same data via reference counting (like a popular book shared by many).

- **RefCell\<T\>:**  
  Allows interior mutability with runtime-checked borrow rules (like a locked safe that only opens when it’s safe to do so).

- **Vec\<T\> and VecDeque\<T\>:**  
  These are dynamic collection types (a dynamic bookshelf and a double-ended queue respectively).

- **The `::` Operator:**  
  Helps navigate modules and namespaces (like file paths).

- **`#[derive()]`:**  
  Automates the implementation of common traits to reduce boilerplate code.

- **Some / None:**  
  The two variants of the `Option` type, clearly indicating presence or absence of a value.

Using these advanced constructs makes your code easier to manage and ensures memory safety and clarity in large and complex projects. Without them, you’d have to manually manage many details like memory allocation, null checks, and ownership logic—which is both error-prone and cumbersome.

---
---

# Understanding Option and Result in Rust

Rust's `Option` and `Result` enums are fundamental to its error handling and null safety. Let's explore them in depth with examples, comparing them to approaches in C and Python.

## Option\<T\>: Handling Optional Values

`Option<T>` represents a value that might be present (`Some(T)`) or absent (`None`).

### Basic Structure

```rust
enum Option<T> {
    Some(T),  // Contains a value of type T
    None,     // Represents no value
}
```

### Use Case 1: Returning a Value That Might Not Exist

```rust
fn find_user_by_id(id: u32) -> Option<User> {
    if id == 0 {
        return None;  // No user with ID 0
    }
    // Otherwise return the user
    Some(User { id, name: format!("User {}", id) })
}

fn main() {
    match find_user_by_id(1) {
        Some(user) => println!("Found user: {}", user.name),
        None => println!("User not found"),
    }
}
```

#### Comparison with C:
```c
// C approach - using NULL pointers
struct User* find_user_by_id(unsigned int id) {
    if (id == 0) {
        return NULL;  // No user with ID 0
    }
    // Allocate memory for user
    struct User* user = malloc(sizeof(struct User));
    if (user == NULL) {
        // Handle allocation failure
        return NULL;
    }
    user->id = id;
    // Set name (simplified)
    // ...
    return user;
}

// Usage - requires NULL check
struct User* user = find_user_by_id(1);
if (user != NULL) {
    printf("Found user: %s\n", user->name);
    free(user);  // Must remember to free memory
} else {
    printf("User not found\n");
}
```

#### Comparison with Python:
```python
def find_user_by_id(id):
    if id == 0:
        return None  # No user with ID 0
    return {"id": id, "name": f"User {id}"}

# Usage
user = find_user_by_id(1)
if user:  # Implicit None check
    print(f"Found user: {user['name']}")
else:
    print("User not found")
```

### Use Case 2: Accessing a Map/Dictionary Value

```rust
use std::collections::HashMap;

fn main() {
    let mut user_ages = HashMap::new();
    user_ages.insert("Alice", 30);
    user_ages.insert("Bob", 25);
    
    // get() returns an Option
    match user_ages.get("Charlie") {
        Some(age) => println!("Charlie is {} years old", age),
        None => println!("Charlie's age is unknown"),
    }
    
    // Using if let for cleaner syntax
    if let Some(age) = user_ages.get("Alice") {
        println!("Alice is {} years old", age);
    }
    
    // Using unwrap_or to provide a default
    let bob_age = *user_ages.get("Bob").unwrap_or(&0);
    println!("Bob's age is {}", bob_age);
}
```

#### Comparison with C:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simplified hash map implementation
struct Entry {
    char* key;
    int value;
};

struct HashMap {
    struct Entry* entries;
    size_t size;
    size_t capacity;
};

// Find an entry in the map
int* find_in_map(struct HashMap* map, const char* key) {
    for (size_t i = 0; i < map->size; i++) {
        if (strcmp(map->entries[i].key, key) == 0) {
            return &map->entries[i].value;
        }
    }
    return NULL;  // Not found
}

int main() {
    // Initialize map (simplified)
    struct HashMap map = {/* ... */};
    
    // Find Charlie's age
    int* charlie_age = find_in_map(&map, "Charlie");
    if (charlie_age != NULL) {
        printf("Charlie is %d years old\n", *charlie_age);
    } else {
        printf("Charlie's age is unknown\n");
    }
    
    // For Bob, provide a default if not found
    int* bob_age = find_in_map(&map, "Bob");
    int actual_bob_age = (bob_age != NULL) ? *bob_age : 0;
    printf("Bob's age is %d\n", actual_bob_age);
    
    // Clean up (simplified)
    // ...
    
    return 0;
}
```

#### Comparison with Python:
```python
user_ages = {"Alice": 30, "Bob": 25}

# Using get() with a default value
charlie_age = user_ages.get("Charlie")
if charlie_age is not None:
    print(f"Charlie is {charlie_age} years old")
else:
    print("Charlie's age is unknown")

# Direct access with try/except
try:
    alice_age = user_ages["Alice"]
    print(f"Alice is {alice_age} years old")
except KeyError:
    print("Alice's age is unknown")

# Using get with default
bob_age = user_ages.get("Bob", 0)
print(f"Bob's age is {bob_age}")
```

### Use Case 3: Parsing Input

```rust
fn parse_age(input: &str) -> Option<u32> {
    input.trim().parse().ok()  // parse() returns Result, ok() converts to Option
}

fn main() {
    let inputs = ["42", "twenty", ""];
    
    for &input in &inputs {
        match parse_age(input) {
            Some(age) => println!("Parsed age: {}", age),
            None => println!("Couldn't parse '{}'", input),
        }
    }
    
    // Using map to transform the value if it exists
    let input = "30";
    let age_next_year = parse_age(input).map(|age| age + 1);
    println!("Next year's age: {:?}", age_next_year);
}
```

#### Comparison with C:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Returns 1 on success, 0 on failure, and stores result in out_age
int parse_age(const char* input, unsigned int* out_age) {
    if (input == NULL || out_age == NULL || strlen(input) == 0) {
        return 0;  // Invalid input
    }
    
    char* endptr;
    errno = 0;  // Reset error number
    
    long result = strtol(input, &endptr, 10);
    
    // Check for conversion errors
    if (errno != 0 || *endptr != '\0' || result < 0) {
        return 0;  // Parsing failed
    }
    
    *out_age = (unsigned int)result;
    return 1;  // Success
}

int main() {
    const char* inputs[] = {"42", "twenty", ""};
    
    for (int i = 0; i < 3; i++) {
        unsigned int age;
        if (parse_age(inputs[i], &age)) {
            printf("Parsed age: %u\n", age);
        } else {
            printf("Couldn't parse '%s'\n", inputs[i]);
        }
    }
    
    // For age next year
    const char* input = "30";
    unsigned int age;
    if (parse_age(input, &age)) {
        unsigned int next_year = age + 1;
        printf("Next year's age: %u\n", next_year);
    } else {
        printf("Couldn't calculate next year's age\n");
    }
    
    return 0;
}
```

#### Comparison with Python:
```python
def parse_age(input_str):
    try:
        return int(input_str.strip())
    except ValueError:
        return None

inputs = ["42", "twenty", ""]

for input_str in inputs:
    age = parse_age(input_str)
    if age is not None:
        print(f"Parsed age: {age}")
    else:
        print(f"Couldn't parse '{input_str}'")

# For age next year
input_str = "30"
age = parse_age(input_str)
age_next_year = age + 1 if age is not None else None
print(f"Next year's age: {age_next_year}")
```

## Result\<T, E\>: Handling Operations That Can Fail

`Result<T, E>` represents either success (`Ok(T)`) or failure (`Err(E)`).

### Basic Structure

```rust
enum Result<T, E> {
    Ok(T),    // Contains a success value of type T
    Err(E),   // Contains an error value of type E
}
```

### Use Case 1: File Operations

```rust
use std::fs::File;
use std::io::{self, Read};

fn read_file_contents(path: &str) -> Result<String, io::Error> {
    let mut file = File::open(path)?;  // ? operator propagates errors
    let mut contents = String::new();
    file.read_to_string(&mut contents)?;
    Ok(contents)
}

fn main() {
    match read_file_contents("config.txt") {
        Ok(contents) => println!("File contents: {}", contents),
        Err(error) => println!("Error reading file: {}", error),
    }
    
    // Using if let for success case only
    if let Ok(contents) = read_file_contents("data.txt") {
        println!("Successfully read {} bytes", contents.len());
    }
    
    // Using unwrap_or_else for custom error handling
    let fallback_contents = "default config";
    let config = read_file_contents("config.txt").unwrap_or_else(|err| {
        println!("Warning: {}", err);
        fallback_contents.to_string()
    });
}
```

#### Comparison with C:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Returns dynamically allocated string on success, NULL on failure
char* read_file_contents(const char* path, char* error_msg, size_t error_msg_size) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        snprintf(error_msg, error_msg_size, "Failed to open file: %s", strerror(errno));
        return NULL;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    
    // Allocate buffer
    char* buffer = malloc(file_size + 1);
    if (buffer == NULL) {
        snprintf(error_msg, error_msg_size, "Memory allocation failed");
        fclose(file);
        return NULL;
    }
    
    // Read file
    size_t read_size = fread(buffer, 1, file_size, file);
    if (read_size != file_size) {
        snprintf(error_msg, error_msg_size, "Failed to read file: %s", strerror(errno));
        free(buffer);
        fclose(file);
        return NULL;
    }
    
    buffer[file_size] = '\0';  // Null-terminate
    fclose(file);
    return buffer;
}

int main() {
    char error_msg[256];
    char* contents = read_file_contents("config.txt", error_msg, sizeof(error_msg));
    
    if (contents != NULL) {
        printf("File contents: %s\n", contents);
        free(contents);  // Must remember to free
    } else {
        printf("Error reading file: %s\n", error_msg);
    }
    
    // For fallback case
    char* config = read_file_contents("config.txt", error_msg, sizeof(error_msg));
    if (config == NULL) {
        printf("Warning: %s\n", error_msg);
        config = strdup("default config");  // Allocate new string
        if (config == NULL) {
            printf("Memory allocation failed\n");
            return 1;
        }
    }
    
    printf("Config: %s\n", config);
    free(config);
    
    return 0;
}
```

#### Comparison with Python:
```python
def read_file_contents(path):
    try:
        with open(path, 'r') as file:
            return file.read()
    except Exception as e:
        return None, str(e)  # Return tuple with None and error message

# Usage
result = read_file_contents("config.txt")
if isinstance(result, tuple):  # Check if it's an error result
    _, error = result
    print(f"Error reading file: {error}")
else:
    contents = result
    print(f"File contents: {contents}")

# Alternative with exceptions
try:
    with open("data.txt", 'r') as file:
        contents = file.read()
    print(f"Successfully read {len(contents)} bytes")
except Exception as e:
    print(f"Error: {e}")

# For fallback case
try:
    with open("config.txt", 'r') as file:
        config = file.read()
except Exception as e:
    print(f"Warning: {e}")
    config = "default config"

print(f"Config: {config}")
```

### Use Case 2: Network Requests

```rust
use reqwest;
use serde::{Deserialize, Serialize};

#[derive(Deserialize, Debug)]
struct User {
    id: u32,
    name: String,
}

async fn fetch_user(id: u32) -> Result<User, reqwest::Error> {
    let url = format!("https://api.example.com/users/{}", id);
    let response = reqwest::get(&url).await?;
    let user = response.json::<User>().await?;
    Ok(user)
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    match fetch_user(1).await {
        Ok(user) => println!("User: {:?}", user),
        Err(e) => println!("Failed to fetch user: {}", e),
    }
    
    // Using ? operator for error propagation
    let user = fetch_user(2).await?;
    println!("Successfully fetched user: {}", user.name);
    
    Ok(())
}
```

#### Comparison with C (using libcurl):
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback for curl
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        printf("Not enough memory\n");
        return 0;
    }
    
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

struct User {
    unsigned int id;
    char *name;
};

// Returns 1 on success, 0 on failure
int fetch_user(unsigned int id, struct User *user, char *error, size_t error_size) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    int success = 0;
    
    chunk.memory = malloc(1);
    chunk.size = 0;
    
    curl = curl_easy_init();
    if (curl) {
        char url[256];
        snprintf(url, sizeof(url), "https://api.example.com/users/%u", id);
        
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            snprintf(error, error_size, "curl_easy_perform() failed: %s", 
                     curl_easy_strerror(res));
        } else {
            // Parse JSON
            struct json_object *json = json_tokener_parse(chunk.memory);
            if (json == NULL) {
                snprintf(error, error_size, "Failed to parse JSON response");
            } else {
                struct json_object *j_id, *j_name;
                
                if (json_object_object_get_ex(json, "id", &j_id) &&
                    json_object_object_get_ex(json, "name", &j_name)) {
                    
                    user->id = json_object_get_int(j_id);
                    user->name = strdup(json_object_get_string(j_name));
                    success = 1;
                } else {
                    snprintf(error, error_size, "Missing fields in JSON response");
                }
                
                json_object_put(json);  // Free JSON object
            }
        }
        
        curl_easy_cleanup(curl);
    } else {
        snprintf(error, error_size, "Failed to initialize curl");
    }
    
    free(chunk.memory);
    return success;
}

int main() {
    struct User user;
    char error[256];
    
    if (fetch_user(1, &user, error, sizeof(error))) {
        printf("User: id=%u, name=%s\n", user.id, user.name);
        free(user.name);  // Must remember to free
    } else {
        printf("Failed to fetch user: %s\n", error);
    }
    
    return 0;
}
```

#### Comparison with Python:
```python
import requests

def fetch_user(id):
    try:
        response = requests.get(f"https://api.example.com/users/{id}")
        response.raise_for_status()  # Raise exception for HTTP errors
        return response.json()
    except requests.RequestException as e:
        return None, str(e)

# Usage
result = fetch_user(1)
if isinstance(result, tuple):
    _, error = result
    print(f"Failed to fetch user: {error}")
else:
    user = result
    print(f"User: {user}")

# Alternative with exceptions
try:
    response = requests.get("https://api.example.com/users/2")
    response.raise_for_status()
    user = response.json()
    print(f"Successfully fetched user: {user['name']}")
except requests.RequestException as e:
    print(f"Error: {e}")
```

### Use Case 3: Custom Error Types

```rust
#[derive(Debug)]
enum DatabaseError {
    ConnectionFailed(String),
    QueryFailed(String),
    NoResults,
}

struct User {
    id: u32,
    name: String,
}

fn get_user_from_db(id: u32) -> Result<User, DatabaseError> {
    // Simulate database connection
    if id == 0 {
        return Err(DatabaseError::ConnectionFailed("Database offline".to_string()));
    }
    
    // Simulate query execution
    if id > 100 {
        return Err(DatabaseError::QueryFailed("Invalid user ID".to_string()));
    }
    
    // Simulate no results
    if id == 99 {
        return Err(DatabaseError::NoResults);
    }
    
    // Success case
    Ok(User {
        id,
        name: format!("User {}", id),
    })
}

fn main() {
    // Handle different error types
    match get_user_from_db(99) {
        Ok(user) => println!("Found user: {}", user.name),
        Err(DatabaseError::ConnectionFailed(msg)) => {
            println!("Connection error: {}", msg);
            // Retry logic could go here
        },
        Err(DatabaseError::QueryFailed(msg)) => {
            println!("Query error: {}", msg);
            // Log error details
        },
        Err(DatabaseError::NoResults) => {
            println!("User not found");
            // Show "not found" UI
        },
    }
    
    // Using map_err to convert error types
    let result = get_user_from_db(0)
        .map_err(|e| format!("Database error: {:?}", e));
    
    println!("Result with mapped error: {:?}", result);
}
```

#### Comparison with C:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Error types as enum
typedef enum {
    ERROR_NONE,
    ERROR_CONNECTION_FAILED,
    ERROR_QUERY_FAILED,
    ERROR_NO_RESULTS
} ErrorType;

// Error structure
typedef struct {
    ErrorType type;
    char message[256];
} DatabaseError;

typedef struct {
    unsigned int id;
    char name[100];
} User;

// Returns 1 on success, 0 on failure and populates error
int get_user_from_db(unsigned int id, User* user, DatabaseError* error) {
    // Simulate database connection
    if (id == 0) {
        error->type = ERROR_CONNECTION_FAILED;
        strcpy(error->message, "Database offline");
        return 0;
    }
    
    // Simulate query execution
    if (id > 100) {
        error->type = ERROR_QUERY_FAILED;
        strcpy(error->message, "Invalid user ID");
        return 0;
    }
    
    // Simulate no results
    if (id == 99) {
        error->type = ERROR_NO_RESULTS;
        strcpy(error->message, "No user found");
        return 0;
    }
    
    // Success case
    user->id = id;
    sprintf(user->name, "User %u", id);
    return 1;
}

int main() {
    User user;
    DatabaseError error;
    
    if (get_user_from_db(99, &user, &error)) {
        printf("Found user: %s\n", user.name);
    } else {
        // Handle different error types
        switch (error.type) {
            case ERROR_CONNECTION_FAILED:
                printf("Connection error: %s\n", error.message);
                // Retry logic could go here
                break;
            case ERROR_QUERY_FAILED:
                printf("Query error: %s\n", error.message);
                // Log error details
                break;
            case ERROR_NO_RESULTS:
                printf("User not found\n");
                // Show "not found" UI
                break;
            default:
                printf("Unknown error\n");
        }
    }
    
    return 0;
}
```

#### Comparison with Python:
```python
class DatabaseError(Exception):
    """Base class for database exceptions"""
    pass

class ConnectionError(DatabaseError):
    """Raised when database connection fails"""
    pass

class QueryError(DatabaseError):
    """Raised when query execution fails"""
    pass

class NoResultsError(DatabaseError):
    """Raised when query returns no results"""
    pass

class User:
    def __init__(self, id, name):
        self.id = id
        self.name = name

def get_user_from_db(id):
    # Simulate database connection
    if id == 0:
        raise ConnectionError("Database offline")
    
    # Simulate query execution
    if id > 100:
        raise QueryError("Invalid user ID")
    
    # Simulate no results
    if id == 99:
        raise NoResultsError("No user found")
    
    # Success case
    return User(id, f"User {id}")

# Usage with exception handling
try:
    user = get_user_from_db(99)
    print(f"Found user: {user.name}")
except ConnectionError as e:
    print(f"Connection error: {e}")
    # Retry logic could go here
except QueryError as e:
    print(f"Query error: {e}")
    # Log error details
except NoResultsError:
    print("User not found")
    # Show "not found" UI
except DatabaseError as e:
    print(f"Other database error: {e}")
```

## Combining Option and Result

### Use Case: Chaining Operations

```rust
use std::fs::File;
use std::io::{self, Read};
use std::path::Path;

fn read_config_value(file_path: &str, key: &str) -> Result<Option<String>, io::Error> {
    // Read the file
    let mut file = File::open(file_path)?;
    let mut contents = String::new();
    file.read_to_string(&mut contents)?;
    
    // Find the key (simplified implementation)
    for line in contents.lines() {
        if let Some(value) = line.strip_prefix(&format!("{}=", key)) {
            return Ok(Some(value.to_string()));
        }
    }
    
    // Key not found, but no error occurred
    Ok(None)
}

fn main() -> Result<(), io::Error> {
    let config_path = "config.txt";
    
    // Check if file exists
    if !Path::new(config_path).exists() {
        println!("Config file not found, using defaults");
        return Ok(());
    }
    
    // Read a config value
    match read_config_value(config_path, "database_url")? {
        Some(url) => println!("Database URL: {}", url),
        None => println!("Database URL not specified in config"),
    }
    
    // Chaining operations with and_then
    let port = read_config_value(config_path, "port")?
        .and_then(|s| s.parse::<u16>().ok());
    
    match port {
        Some(p) => println!("Port: {}", p),
        None => println!("Invalid or missing port"),
    }
    
    Ok(())
}
```

#### Comparison with C:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Error codes
#define SUCCESS 0
#define ERROR_FILE_NOT_FOUND 1
#define ERROR_READ_FAILED 2

// Result structure for config value
typedef struct {
    int error_code;
    int found;  // 1 if value found, 0 if not found
    char value[256];
} ConfigResult;

ConfigResult read_config_value(const char* file_path, const char* key) {
    ConfigResult result = {SUCCESS, 0, ""};
    FILE* file = fopen(file_path, "r");
    
    if (file == NULL) {
        result.error_code = ERROR_FILE_NOT_FOUND;
        return result;
    }
    
    char line[512];
    char search_key[256];
    sprintf(search_key, "%s=", key);
    size_t key_len = strlen(search_key);
    
    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        // Check if line starts with key
        if (strncmp(line, search_key, key_len) == 0) {
            result.found = 1;
            strcpy(result.value, line + key_len);
            break;
        }
    }
    
    if (ferror(file)) {
        result.error_code = ERROR_READ_FAILED;
    }
    
    fclose(file);
    return result;
}

int main() {
    const char* config_path = "config.txt";
    
    // Check if file exists
    FILE* test_file = fopen(config_path, "r");
    if (test_file == NULL) {
        printf("Config file not found, using defaults\n");
        return 0;
    }
    fclose(test_file);
    
    // Read database URL
    ConfigResult db_result = read_config_value(config_path, "database_url");
    if (db_result.error_code != SUCCESS) {
        printf("Error reading config: %d\n", db_result.error_code);
        return 1;
    }
    
    if (db_result.found) {
        printf("Database URL: %s\n", db_result.value);
    } else {
        printf("Database URL not specified in config\n");
    }
    
    // Read port
    ConfigResult port_result = read_config_value(config_path, "port");
    if (port_result.error_code != SUCCESS) {
        printf("Error reading config: %d\n", port_result.error_code);
        return 1;
    }
    
    if (port_result.found) {
        // Try to parse as integer
        char* endptr;
        errno = 0;
        long port = strtol(port_result.value, &endptr, 10);
        
        if (errno != 0 || *endptr != '\0' || port < 0 || port > 65535) {
            printf("Invalid port\n");
        } else {
            printf("Port: %ld\n", port);
        }
    } else {
        printf("Port not specified in config\n");
    }
    
    return 0;
}
```


#### Comparison with Python:
```python
import os

config_path = "config.txt"

# Check if file exists
if not os.path.exists(config_path):
    print("Config file not found, using defaults")
else:
    # Read database URL
    result = read_config_value(config_path, "database_url")
    if isinstance(result, tuple):
        _, error = result
        print(f"Error reading config: {error}")
    elif result is not None:
        print(f"Database URL: {result}")
    else:
        print("Database URL not specified in config")
    
    # Read port
    result = read_config_value(config_path, "port")
    if isinstance(result, tuple):
        _, error = result
        print(f"Error reading config: {error}")
    elif result is not None:
        try:
            port = int(result)
            print(f"Port: {port}")
        except ValueError:
            print("Invalid port")
    else:
        print("Port not specified in config")
```

## Advanced Patterns with Option and Result

### Pattern 1: Fallback Chain with `or_else` and `unwrap_or`

```rust
fn get_config_value(key: &str) -> Option<String> {
    // Try to get from environment first
    std::env::var(key).ok().or_else(|| {
        // Then try config file
        let config_file = "config.txt";
        if let Ok(contents) = std::fs::read_to_string(config_file) {
            for line in contents.lines() {
                if let Some(value) = line.strip_prefix(&format!("{}=", key)) {
                    return Some(value.to_string());
                }
            }
        }
        None
    })
}

fn main() {
    // Get database URL with fallback
    let db_url = get_config_value("DATABASE_URL")
        .unwrap_or_else(|| "sqlite::memory:".to_string());
    
    println!("Using database URL: {}", db_url);
    
    // Get port with fallback
    let port_str = get_config_value("PORT");
    let port = port_str
        .and_then(|s| s.parse::<u16>().ok())
        .unwrap_or(8080);
    
    println!("Using port: {}", port);
}
```

#### Comparison with C:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns dynamically allocated string or NULL
char* get_config_value(const char* key) {
    // Try environment first
    char* env_value = getenv(key);
    if (env_value != NULL) {
        return strdup(env_value);  // Must be freed by caller
    }
    
    // Then try config file
    const char* config_file = "config.txt";
    FILE* file = fopen(config_file, "r");
    if (file == NULL) {
        return NULL;
    }
    
    char line[512];
    char search_key[256];
    sprintf(search_key, "%s=", key);
    size_t key_len = strlen(search_key);
    char* result = NULL;
    
    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        // Check if line starts with key
        if (strncmp(line, search_key, key_len) == 0) {
            result = strdup(line + key_len);
            break;
        }
    }
    
    fclose(file);
    return result;
}

int main() {
    // Get database URL with fallback
    char* db_url = get_config_value("DATABASE_URL");
    if (db_url == NULL) {
        db_url = strdup("sqlite::memory:");
        if (db_url == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }
    }
    
    printf("Using database URL: %s\n", db_url);
    free(db_url);  // Must remember to free
    
    // Get port with fallback
    char* port_str = get_config_value("PORT");
    unsigned short port = 8080;  // Default
    
    if (port_str != NULL) {
        char* endptr;
        long port_val = strtol(port_str, &endptr, 10);
        if (*endptr == '\0' && port_val >= 0 && port_val <= 65535) {
            port = (unsigned short)port_val;
        }
        free(port_str);  // Must remember to free
    }
    
    printf("Using port: %hu\n", port);
    
    return 0;
}
```

#### Comparison with Python:
```python
import os

def get_config_value(key):
    # Try environment first
    if key in os.environ:
        return os.environ[key]
    
    # Then try config file
    config_file = "config.txt"
    try:
        with open(config_file, 'r') as file:
            for line in file:
                if line.startswith(f"{key}="):
                    return line.strip().split('=', 1)[1]
    except FileNotFoundError:
        pass
    
    return None

# Get database URL with fallback
db_url = get_config_value("DATABASE_URL") or "sqlite::memory:"
print(f"Using database URL: {db_url}")

# Get port with fallback
port_str = get_config_value("PORT")
try:
    port = int(port_str) if port_str is not None else 8080
except ValueError:
    port = 8080

print(f"Using port: {port}")
```

### Pattern 2: Collecting Results

```rust
fn parse_numbers(input: &str) -> Vec<Result<i32, std::num::ParseIntError>> {
    input.split(',')
        .map(|s| s.trim().parse::<i32>())
        .collect()
}

fn main() {
    let input = "10, 20, thirty, 40";
    let results = parse_numbers(input);
    
    // Method 1: Collect only successful values
    let valid_numbers: Vec<i32> = results.iter()
        .filter_map(|r| r.as_ref().ok().copied())
        .collect();
    
    println!("Valid numbers: {:?}", valid_numbers);
    
    // Method 2: Partition into successes and failures
    let input = "1, 2, 3, four, 5";
    let results = parse_numbers(input);
    
    let (valid, errors): (Vec<_>, Vec<_>) = results.into_iter()
        .partition(Result::is_ok);
    
    let valid_numbers: Vec<i32> = valid.into_iter()
        .map(Result::unwrap)
        .collect();
    
    let error_indices: Vec<usize> = errors.iter()
        .enumerate()
        .map(|(i, _)| i)
        .collect();
    
    println!("Valid numbers: {:?}", valid_numbers);
    println!("Errors at indices: {:?}", error_indices);
    
    // Method 3: All-or-nothing with collect
    let input = "1, 2, 3, 4, 5";
    let numbers: Result<Vec<i32>, _> = input.split(',')
        .map(|s| s.trim().parse::<i32>())
        .collect();  // Collects into Result<Vec<T>, E>
    
    match numbers {
        Ok(nums) => println!("All numbers valid: {:?}", nums),
        Err(e) => println!("Error parsing numbers: {}", e),
    }
}
```

#### Comparison with C:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Structure to hold parsing results
typedef struct {
    int* values;
    int* error_flags;  // 1 if error, 0 if success
    size_t count;
} ParseResult;

ParseResult parse_numbers(const char* input) {
    // Count commas to estimate array size
    size_t count = 1;
    for (const char* p = input; *p; p++) {
        if (*p == ',') count++;
    }
    
    // Allocate arrays
    ParseResult result;
    result.values = malloc(count * sizeof(int));
    result.error_flags = malloc(count * sizeof(int));
    result.count = count;
    
    if (result.values == NULL || result.error_flags == NULL) {
        // Handle allocation failure
        free(result.values);
        free(result.error_flags);
        result.values = NULL;
        result.error_flags = NULL;
        result.count = 0;
        return result;
    }
    
    // Parse each number
    char* input_copy = strdup(input);
    if (input_copy == NULL) {
        free(result.values);
        free(result.error_flags);
        result.values = NULL;
        result.error_flags = NULL;
        result.count = 0;
        return result;
    }
    
    char* token = strtok(input_copy, ",");
    size_t i = 0;
    
    while (token != NULL && i < count) {
        // Skip whitespace
        while (*token == ' ') token++;
        
        // Parse number
        char* endptr;
        errno = 0;
        long value = strtol(token, &endptr, 10);
        
        if (errno != 0 || *endptr != '\0') {
            // Parsing error
            result.error_flags[i] = 1;
            result.values[i] = 0;  // Default value
        } else {
            // Success
            result.error_flags[i] = 0;
            result.values[i] = (int)value;
        }
        
        i++;
        token = strtok(NULL, ",");
    }
    
    free(input_copy);
    return result;
}

// Function to collect only valid numbers
int* collect_valid_numbers(ParseResult* result, size_t* out_count) {
    // Count valid numbers
    size_t valid_count = 0;
    for (size_t i = 0; i < result->count; i++) {
        if (!result->error_flags[i]) {
            valid_count++;
        }
    }
    
    // Allocate array for valid numbers
    int* valid_numbers = malloc(valid_count * sizeof(int));
    if (valid_numbers == NULL) {
        *out_count = 0;
        return NULL;
    }
    
    // Fill array
    size_t j = 0;
    for (size_t i = 0; i < result->count; i++) {
        if (!result->error_flags[i]) {
            valid_numbers[j++] = result->values[i];
        }
    }
    
    *out_count = valid_count;
    return valid_numbers;
}

// Function to get error indices
size_t* collect_error_indices(ParseResult* result, size_t* out_count) {
    // Count errors
    size_t error_count = 0;
    for (size_t i = 0; i < result->count; i++) {
        if (result->error_flags[i]) {
            error_count++;
        }
    }
    
    // Allocate array for error indices
    size_t* error_indices = malloc(error_count * sizeof(size_t));
    if (error_indices == NULL) {
        *out_count = 0;
        return NULL;
    }
    
    // Fill array
    size_t j = 0;
    for (size_t i = 0; i < result->count; i++) {
        if (result->error_flags[i]) {
            error_indices[j++] = i;
        }
    }
    
    *out_count = error_count;
    return error_indices;
}

// Function to check if all parsing succeeded
int all_valid(ParseResult* result) {
    for (size_t i = 0; i < result->count; i++) {
        if (result->error_flags[i]) {
            return 0;  // Found an error
        }
    }
    return 1;  // All valid
}

int main() {
    const char* input = "10, 20, thirty, 40";
    ParseResult result = parse_numbers(input);
    
    if (result.values == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    // Method 1: Collect only valid numbers
    size_t valid_count;
    int* valid_numbers = collect_valid_numbers(&result, &valid_count);
    
    printf("Valid numbers: ");
    for (size_t i = 0; i < valid_count; i++) {
        printf("%d ", valid_numbers[i]);
    }
    printf("\n");
    
    // Method 2: Get error indices
    size_t error_count;
    size_t* error_indices = collect_error_indices(&result, &error_count);
    
    printf("Errors at indices: ");
    for (size_t i = 0; i < error_count; i++) {
        printf("%zu ", error_indices[i]);
    }
    printf("\n");
    
    // Method 3: All-or-nothing check
    const char* input2 = "1, 2, 3, 4, 5";
    ParseResult result2 = parse_numbers(input2);
    
    if (all_valid(&result2)) {
        printf("All numbers valid: ");
        for (size_t i = 0; i < result2.count; i++) {
            printf("%d ", result2.values[i]);
        }
        printf("\n");
    } else {
        printf("Error parsing numbers\n");
    }
    
    // Clean up
    free(valid_numbers);
    free(error_indices);
    free(result.values);
    free(result.error_flags);
    free(result2.values);
    free(result2.error_flags);
    
    return 0;
}
```

#### Comparison with Python:
```python
def parse_numbers(input_str):
    results = []
    for item in input_str.split(','):
        try:
            num = int(item.strip())
            results.append(num)  # Success
        except ValueError as e:
            results.append(e)  # Store the error
    return results

# Method 1: Collect only valid numbers
input_str = "10, 20, thirty, 40"
results = parse_numbers(input_str)

valid_numbers = [x for x in results if isinstance(x, int)]
print(f"Valid numbers: {valid_numbers}")

# Method 2: Partition into successes and failures
input_str = "1, 2, 3, four, 5"
results = parse_numbers(input_str)

valid_numbers = [x for x in results if isinstance(x, int)]
error_indices = [i for i, x in enumerate(results) if isinstance(x, ValueError)]

print(f"Valid numbers: {valid_numbers}")
print(f"Errors at indices: {error_indices}")

# Method 3: All-or-nothing approach
input_str = "1, 2, 3, 4, 5"
try:
    # Using a different approach for all-or-nothing
    numbers = [int(item.strip()) for item in input_str.split(',')]
    print(f"All numbers valid: {numbers}")
except ValueError as e:
    print(f"Error parsing numbers: {e}")
```

### Pattern 3: Early Returns with the `?` Operator

```rust
use std::fs::File;
use std::io::{self, Read};
use std::path::Path;

fn read_and_parse_config() -> Result<Vec<i32>, String> {
    // Open the file, convert io::Error to String
    let mut file = File::open("numbers.txt")
        .map_err(|e| format!("Failed to open file: {}", e))?;
    
    // Read the contents
    let mut contents = String::new();
    file.read_to_string(&mut contents)
        .map_err(|e| format!("Failed to read file: {}", e))?;
    
    // Parse the numbers
    let numbers: Result<Vec<i32>, _> = contents
        .lines()
        .map(|line| line.parse::<i32>())
        .collect();
    
    // Convert parse errors to String
    numbers.map_err(|e| format!("Failed to parse number: {}", e))
}

fn main() {
    match read_and_parse_config() {
        Ok(numbers) => println!("Config numbers: {:?}", numbers),
        Err(e) => println!("Error: {}", e),
    }
}
```

#### Comparison with C:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Error codes
#define ERROR_NONE 0
#define ERROR_FILE_OPEN 1
#define ERROR_FILE_READ 2
#define ERROR_PARSE 3
#define ERROR_MEMORY 4

// Result structure
typedef struct {
    int error_code;
    char error_message[256];
    int* numbers;
    size_t count;
} ConfigResult;

ConfigResult read_and_parse_config() {
    ConfigResult result = {ERROR_NONE, "", NULL, 0};
    
    // Open the file
    FILE* file = fopen("numbers.txt", "r");
    if (file == NULL) {
        result.error_code = ERROR_FILE_OPEN;
        snprintf(result.error_message, sizeof(result.error_message),
                "Failed to open file: %s", strerror(errno));
        return result;
    }
    
    // Count lines to allocate array
    size_t line_count = 0;
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') line_count++;
    }
    if (ferror(file)) {
        result.error_code = ERROR_FILE_READ;
        snprintf(result.error_message, sizeof(result.error_message),
                "Failed to read file: %s", strerror(errno));
        fclose(file);
        return result;
    }
    
    // Add one more if file doesn't end with newline
    if (line_count > 0) line_count++;
    
    // Allocate array
    result.numbers = malloc(line_count * sizeof(int));
    if (result.numbers == NULL) {
        result.error_code = ERROR_MEMORY;
        snprintf(result.error_message, sizeof(result.error_message),
                "Failed to allocate memory");
        fclose(file);
        return result;
    }
    
    // Reset file position
    rewind(file);
    
    // Read and parse each line
    char line[256];
    size_t i = 0;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        // Skip empty lines
        if (strlen(line) == 0) continue;
        
        // Parse number
        char* endptr;
        errno = 0;
        long value = strtol(line, &endptr, 10);
        
        if (errno != 0 || *endptr != '\0') {
            result.error_code = ERROR_PARSE;
            snprintf(result.error_message, sizeof(result.error_message),
                    "Failed to parse number: %s", line);
            free(result.numbers);
            result.numbers = NULL;
            fclose(file);
            return result;
        }
        
        result.numbers[i++] = (int)value;
    }
    
    if (ferror(file)) {
        result.error_code = ERROR_FILE_READ;
        snprintf(result.error_message, sizeof(result.error_message),
                "Failed to read file: %s", strerror(errno));
        free(result.numbers);
        result.numbers = NULL;
        fclose(file);
        return result;
    }
    
    result.count = i;
    fclose(file);
    return result;
}

int main() {
    ConfigResult result = read_and_parse_config();
    
    if (result.error_code == ERROR_NONE) {
        printf("Config numbers: ");
        for (size_t i = 0; i < result.count; i++) {
            printf("%d ", result.numbers[i]);
        }
        printf("\n");
        free(result.numbers);
    } else {
        printf("Error: %s\n", result.error_message);
    }
    
    return 0;
}
```

#### Comparison with Python:
```python
def read_and_parse_config():
    try:
        # Open and read the file
        with open("numbers.txt", "r") as file:
            contents = file.read()
    except FileNotFoundError:
        return None, "Failed to open file: File not found"
    except IOError as e:
        return None, f"Failed to read file: {e}"
    
    # Parse the numbers
    numbers = []
    for line in contents.splitlines():
        if not line.strip():
            continue
        
        try:
            numbers.append(int(line))
        except ValueError:
            return None, f"Failed to parse number: {line}"
    
    return numbers, None

# Usage
numbers, error = read_and_parse_config()
if error:
    print(f"Error: {error}")
else:
    print(f"Config numbers: {numbers}")
```

## Key Differences and Benefits

### 1. Explicitness vs. Implicitness

- **Rust**: Makes potential failures explicit in the type system. You can't ignore an `Option` or `Result`.
- **C**: Relies on return codes, out parameters, or NULL pointers. Easy to forget to check.
- **Python**: Uses exceptions which can be silently propagated or forgotten.

### 2. Composition and Chaining

- **Rust**: Rich set of combinators (`map`, `and_then`, `or_else`, etc.) for elegant composition.
- **C**: Requires verbose error checking at each step.
- **Python**: Can use exceptions for flow control, but lacks the functional composition tools.

### 3. Type Safety

- **Rust**: Compiler enforces handling of all cases.
- **C**: No compile-time guarantees; easy to forget error cases.
- **Python**: Runtime checks only; errors appear during execution.

### 4. Memory Safety

- **Rust**: No memory leaks or use-after-free bugs due to ownership system.
- **C**: Manual memory management prone to leaks and dangling pointers.
- **Python**: Garbage collection handles memory, but at a performance cost.

## Conclusion

Rust's `Option` and `Result` types provide a powerful, expressive, and safe way to handle optional values and errors. They force developers to explicitly handle all possible cases, preventing many common bugs that plague other languages.

While C and Python can achieve similar functionality, they require more discipline from the programmer and offer fewer guarantees. Rust's approach combines the performance of C with safety guarantees that exceed even those of garbage-collected languages like Python.

---
