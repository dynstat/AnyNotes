`if let` is commonly used in Rust when you want to match on a single pattern and ignore all other cases. It's essentially a more concise way to write a `match` expression when you only care about one pattern.

Here are common use cases with examples:

1. **Working with Option**:
    
    ```rust
    // Instead of match
    let some_value: Option<i32> = Some(42);
    match some_value {
    Some(x) => println!("Got value: {}", x),
    None => (), // Do nothing
    }
    
    // Using if let (more concise)
    if let Some(x) = some_value {
    println!("Got value: {}", x);
    }
    ```
    
2. **Working with Result<T, E>**:
    
    ```rust
    fn parse_number(s: &str) -> Result<i32, std::num::ParseIntError> {
    s.parse()
    }
    // Using if let for success case only
    if let Ok(num) = parse_number("42") {
    println!("Successfully parsed: {}", num);
    }
    
    // Using if let for error case only
    if let Err(e) = parse_number("not a number") {
    println!("Failed to parse: {}", e);
    }
    ```
    
3. **Pattern Matching with Enums**:
    
    ```rust
    enum Color {
    Red(u8),
    Blue(u8),
    Green(u8),
    }
    ```
    

4. **Destructuring Structs or Tuples**:
    
    ```rust
    struct Point {
    x: i32,
    y: i32,
    }
    let point = Point { x: 10, y: 20 };
    
    // If we only care about x being 10
    if let Point { x: 10, y } = point {
    println!("Point has x=10 and y={}", y);
    }
    
    // With tuples
    let tuple = (1, "hello", true);
    if let (1, message, _) = tuple {
    println!("First element is 1, message is: {}", message);
    }
    ```
    

5. **Combining with else**:

```rust
let maybe_value: Option<i32> = None;

if let Some(value) = maybe_value {
 println!("Got value: {}", value);
} else {
 println!("No value found");
}
```

6. **Working with References**:
    
```rust
let reference = &Some(5);
if let Some(value) = reference {
println!("Got value: {}", value);
}
```
    

7. **In HashMap Operations** (like in our substring problem):
    
    ```rust
    use std::collections::HashMap;
    let mut map = HashMap::new();
    map.insert("key", 42);
    
    // If we only care about when the key exists
    if let Some(value) = map.get("key") {
    println!("Found value: {}", value);
    }
    ```
    

8. **With Guards**:
    
    ```rust
    let number = Some(42);
    // With additional boolean condition
    if let Some(n) = number && n > 30 {
    println!("Large number: {}", n);
    }
    ```
    

9. **Multiple Patterns** (using |):
    
    ```rust
    enum Direction {
    North,
    South,
    East,
    West,
    }
    
    let dir = Direction::North;
    
    if let Direction::North | Direction::South = dir {
    println!("Going vertical!");
    }
    ```
    

10. **In Loop Contexts**:
    
    ```rust
    let mut iterator = vec![1, 2, 3].into_iter();
    
    // Process values until None is encountered
    while let Some(value) = iterator.next() {
    println!("Got: {}", value);
    }
    ```
    

When to use `if let`:

1. When you only care about one pattern and want to ignore others
    2. When you want more concise code than a full `match`
    3. When you're working with Options/Results and only care about one case
    4. When pattern matching with complex types but only need one specific pattern When NOT to use `if let`:
    5. When you need to handle multiple patterns differently
    6. When you need exhaustive pattern matching
    7. When you need to return values from different match arms In our substring problem context:
        
        ```rust
        // Instead of
        match char_map.get(&ch) {
        Some(&prev_pos) => {
        if prev_pos >= left {
        left = prev_pos + 1;
        }
        }
        None => {}
        } 
        // We could use
        if let Some(&prev_pos) = char_map.get(&ch) {
        if prev_pos >= left {
        left = prev_pos + 1;
        }
        }
        
        ```
        

Both are valid, but the `if let` version is more concise when we don't need to do anything in the `None` case.