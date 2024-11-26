
Let me explain panic in Rust with comprehensive examples and how to handle them:

```rust
fn main() {
    // ==========================================
    // 1. WHAT CAN CAUSE PANIC
    // ==========================================
    
    let vec = vec![1, 2, 3];

    // Example 1: Index out of bounds
    // These will panic:
    // let x = vec[5];                  // ❌ Index 5 doesn't exist
    // vec.get(5).unwrap();            // ❌ Unwrap on None
    
    // Example 2: Integer overflow in debug mode
    let mut i: u8 = 255;
    // i += 1;                         // ❌ Panics in debug mode
    
    // Example 3: Unwrapping None
    let option: Option<i32> = None;
    // option.unwrap();                // ❌ Panics
    
    // Example 4: Array/slice index overflow
    let arr = [1, 2, 3];
    // let x = arr[5];                 // ❌ Panics
    
    // ==========================================
    // 2. HOW TO HANDLE POTENTIAL PANICS
    // ==========================================
    
    // Method 1: Using get() instead of direct indexing
    let vec = vec![1, 2, 3];
    
    // Bad way (can panic):
    // let value = vec[5];
    
    // Good way (safe):
    match vec.get(5) {
        Some(value) => println!("Value: {}", value),
        None => println!("Index out of bounds"),
    }
    
    // Method 2: Using if let
    if let Some(value) = vec.get(5) {
        println!("Value: {}", value);
    } else {
        println!("Index out of bounds");
    }

    // Method 3: Using unwrap_or/unwrap_or_else
    let value = vec.get(5).unwrap_or(&0);  // Default value if None
    let value = vec.get(5).unwrap_or_else(|| &compute_default());

    // ==========================================
    // 3. USING RESULT TO HANDLE ERRORS
    // ==========================================
    
    fn get_item(vec: &Vec<i32>, index: usize) -> Result<i32, String> {
        vec.get(index)
            .copied()  // Convert &i32 to i32
            .ok_or_else(|| format!("Index {} out of bounds", index))
    }

    // Using the safe function
    let vec = vec![1, 2, 3];
    match get_item(&vec, 5) {
        Ok(value) => println!("Got value: {}", value),
        Err(e) => println!("Error: {}", e),
    }

    // ==========================================
    // 4. CUSTOM PANIC HOOKS
    // ==========================================
    
    use std::panic;
    
    // Set custom panic hook
    panic::set_hook(Box::new(|panic_info| {
        if let Some(location) = panic_info.location() {
            println!("Panic occurred in file '{}' at line {}", 
                    location.file(), 
                    location.line());
        }
        if let Some(message) = panic_info.payload().downcast_ref::<&str>() {
            println!("Panic message: {}", message);
        }
    }));

    // ==========================================
    // 5. CATCHING PANICS (USE SPARINGLY)
    // ==========================================
    
    let result = std::panic::catch_unwind(|| {
        // Code that might panic
        vec![1, 2, 3][5]
    });
    
    match result {
        Ok(value) => println!("Operation succeeded: {:?}", value),
        Err(_) => println!("Operation panicked"),
    }

    // ==========================================
    // 6. BEST PRACTICES
    // ==========================================
    
    // 1. Safe Vector Access
    let vec = vec![1, 2, 3];
    
    // Instead of this (panic-prone):
    // let first = vec[0];
    
    // Do this (safe):
    let first = vec.first().copied().unwrap_or_default();
    
    // 2. Safe Type Conversion
    let num_str = "123";
    
    // Instead of this:
    // let num = num_str.parse::<i32>().unwrap();
    
    // Do this:
    let num = num_str.parse::<i32>().unwrap_or_default();
    
    // 3. Custom Error Types
    #[derive(Debug)]
    enum VectorError {
        IndexOutOfBounds(usize),
        EmptyVector,
    }

    fn safe_get(vec: &Vec<i32>, index: usize) -> Result<i32, VectorError> {
        if vec.is_empty() {
            return Err(VectorError::EmptyVector);
        }
        vec.get(index)
            .copied()
            .ok_or(VectorError::IndexOutOfBounds(index))
    }

    // ==========================================
    // 7. RECOVERY STRATEGIES
    // ==========================================
    
    let vec = vec![1, 2, 3];
    
    // Strategy 1: Default Values
    let value = vec.get(5).copied().unwrap_or(0);
    
    // Strategy 2: Clamping indices
    let index = 5;
    let safe_index = index.min(vec.len() - 1);
    let value = vec[safe_index];
    
    // Strategy 3: Wrapping indices
    let wrapped_index = index % vec.len();
    let value = vec[wrapped_index];
}

fn compute_default() -> i32 {
    0
}
```

Key Points About Panic:

1. **What is Panic?**
   - A panic is Rust's way of handling unrecoverable errors
   - It unwinds the stack, cleaning up resources
   - Program typically exits with error message
   - Can be caught, but this is rarely the right solution

2. **Common Panic Causes:**
   - Index out of bounds
   - Integer overflow (in debug mode)
   - Unwrapping None
   - Division by zero
   - Stack overflow

3. **How to Prevent Panics:**
   - Use `get()` instead of `[]` for vector access
   - Check bounds before accessing
   - Use `checked_*`, `saturating_*`, or `wrapping_*` for arithmetic
   - Avoid `unwrap()` and `expect()` in production code
   - Use proper error handling with `Result` and `Option`

4. **Error Handling Best Practices:**
   - Use `Result` for recoverable errors
   - Use `Option` for optional values
   - Implement custom error types
   - Consider using crates like `anyhow` or `thiserror`
   - Write defensive code that anticipates errors

5. **When to Use Each Approach:**
   ```rust
   // Use Result when the error is recoverable
   fn fallible_operation() -> Result<(), String> {
       // ...
   }

   // Use Option when the value might not exist
   fn find_element(vec: &Vec<i32>, pred: fn(&i32) -> bool) -> Option<&i32> {
       vec.iter().find(|&&x| pred(&x))
   }

   // Use panic when it's truly unrecoverable
   fn must_succeed(input: i32) {
       assert!(input > 0, "Input must be positive");
       // ...
   }
   ```

Remember:
- Panics are for unrecoverable errors
- Use proper error handling for recoverable situations
- Write defensive code to prevent panics
- Test edge cases thoroughly
- Consider using safe alternatives to panic-prone operations
---
