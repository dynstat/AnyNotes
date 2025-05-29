Competitive programming platforms like LeetCode, HackerRank, and Codeforces present a variety of algorithmic challenges that require efficient and effective use of programming language features. Rust, known for its performance and safety, is an excellent choice for such competitions. To excel in solving competitive programming questions in Rust, it's essential to be familiar with a set of functions and methods that streamline common tasks.

Below is a comprehensive list of important functions and methods in Rust, categorized by their use cases, along with short code examples demonstrating their usage. These examples include necessary imports and dependencies to ensure they are self-contained and executable.

---

## **1. Input and Output**

Efficient input/output handling is crucial in competitive programming to avoid timeouts, especially with large datasets.

### **a. Fast Input Reading using `BufReader`**

Rust's standard input can be slow for large inputs. Using `BufReader` significantly speeds up reading.

```rust
use std::io::{self, BufRead};

fn main() {
    let stdin = io::stdin();
    let mut iterator = stdin.lock().lines();

    // Read the first line
    if let Some(Ok(line)) = iterator.next() {
        let n: usize = line.trim().parse().unwrap();
        println!("Number: {}", n);
    }

    // Read multiple lines
    for line in iterator {
        if let Ok(num_str) = line {
            let num: i32 = num_str.trim().parse().unwrap();
            println!("Read number: {}", num);
        }
    }
}
```

### **b. Reading Multiple Integers in a Single Line**

Splitting a line into tokens and parsing them into integers.

```rust
use std::io::{self, BufRead};

fn main() {
    let stdin = io::stdin();
    let line = stdin.lock().lines().next().unwrap().unwrap();
    let numbers: Vec<i32> = line
        .split_whitespace()
        .map(|s| s.parse().expect("parse error"))
        .collect();
    
    println!("Numbers: {:?}", numbers);
}
```

### **c. Printing Output with `println!` Macro**

Standard way to print output in Rust.

```rust
fn main() {
    let x = 10;
    let y = 20;
    println!("The value of x is {} and y is {}", x, y);
}
```

---

## **2. Data Structures**

Efficient data structures can significantly optimize your solutions.

### **a. Vector (`Vec`) Operations**

Vectors are dynamic arrays in Rust, widely used in competitive programming.

```rust
fn main() {
    let mut vec = Vec::new();

    // Push elements
    vec.push(1);
    vec.push(2);
    vec.push(3);
    println!("Vector after push: {:?}", vec);

    // Access elements
    if let Some(first) = vec.get(0) {
        println!("First element: {}", first);
    }

    // Sorting
    vec.sort();
    println!("Sorted Vector: {:?}", vec);

    // Iterating
    for num in &vec {
        println!("Number: {}", num);
    }
}
```

### **b. HashMap Operations**

HashMaps are essential for key-value storage with average constant-time complexity for insertions and lookups.

```rust
use std::collections::HashMap;

fn main() {
    let mut map = HashMap::new();

    // Insert key-value pairs
    map.insert("apple", 3);
    map.insert("banana", 2);
    map.insert("orange", 5);

    // Access values
    if let Some(&count) = map.get("banana") {
        println!("Bananas: {}", count);
    }

    // Iterate over key-value pairs
    for (key, value) in &map {
        println!("{}: {}", key, value);
    }

    // Update a value
    *map.get_mut("apple").unwrap() += 1;
    println!("Updated Apple Count: {}", map["apple"]);
}
```

### **c. Binary Heap (`BinaryHeap`)**

Useful for priority queues, especially in algorithms like Dijkstra's.

```rust
use std::collections::BinaryHeap;

fn main() {
    let mut heap = BinaryHeap::new();

    // Push elements
    heap.push(5);
    heap.push(1);
    heap.push(3);
    println!("Binary Heap: {:?}", heap);

    // Pop the largest element
    if let Some(top) = heap.pop() {
        println!("Top element: {}", top);
    }

    // Remaining heap
    println!("Heap after pop: {:?}", heap);
}
```

### **d. BTreeSet Operations**

Useful for ordered sets where you need to traverse elements in a specific order.

```rust
use std::collections::BTreeSet;

fn main() {
    let mut set = BTreeSet::new();

    // Insert elements
    set.insert(3);
    set.insert(1);
    set.insert(2);
    println!("BTreeSet: {:?}", set);

    // Check for existence
    if set.contains(&2) {
        println!("Set contains 2");
    }

    // Iterate in order
    for num in &set {
        println!("Number: {}", num);
    }
}
```

---

## **3. String Manipulation**

Handling strings efficiently is often required, especially in parsing problems.

### **a. Splitting Strings**

Splitting strings based on whitespace or other delimiters.

```rust
fn main() {
    let s = "Hello World from Rust";

    // Split by whitespace
    let words: Vec<&str> = s.split_whitespace().collect();
    println!("Words: {:?}", words);

    // Split by a specific character
    let csv = "1,2,3,4,5";
    let numbers: Vec<&str> = csv.split(',').collect();
    println!("CSV Numbers: {:?}", numbers);
}
```

### **b. Parsing Strings to Numbers**

Converting string slices to numerical types.

```rust
fn main() {
    let s = "12345";
    let number: i32 = s.parse().expect("Not a number!");
    println!("Parsed number: {}", number);

    let float_str = "3.1415";
    let pi: f64 = float_str.parse().expect("Not a float!");
    println!("Parsed float: {}", pi);
}
```

### **c. String Conversion and Iteration**

Converting between `String` and `&str`, and iterating over characters.

```rust
fn main() {
    let s = String::from("Rust");
    let slice: &str = &s;
    println!("Slice: {}", slice);

    // Iterating over characters
    for c in s.chars() {
        println!("Character: {}", c);
    }

    // Collecting characters into a String
    let reversed: String = s.chars().rev().collect();
    println!("Reversed: {}", reversed);
}
```

---

## **4. Iterators and Their Methods**

Iterators provide powerful ways to process collections succinctly and efficiently.

### **a. `map` and `collect`**

Transforming elements of a collection.

```rust
fn main() {
    let numbers = vec![1, 2, 3, 4, 5];

    // Square each number
    let squares: Vec<i32> = numbers.iter().map(|x| x * x).collect();
    println!("Squares: {:?}", squares);
}
```

### **b. `filter` and `collect`**

Filtering elements based on a condition.

```rust
fn main() {
    let numbers = vec![1, 2, 3, 4, 5, 6];

    // Filter even numbers
    let evens: Vec<i32> = numbers.iter().filter(|&&x| x % 2 == 0).cloned().collect();
    println!("Even Numbers: {:?}", evens);
}
```

### **c. `fold` for Accumulation**

Aggregating elements into a single value.

```rust
fn main() {
    let numbers = vec![1, 2, 3, 4, 5];

    // Sum of numbers
    let sum: i32 = numbers.iter().fold(0, |acc, &x| acc + x);
    println!("Sum: {}", sum);

    // Product of numbers
    let product: i32 = numbers.iter().fold(1, |acc, &x| acc * x);
    println!("Product: {}", product);
}
```

### **d. `enumerate`**

Obtaining index-value pairs while iterating.

```rust
fn main() {
    let fruits = vec!["apple", "banana", "cherry"];

    for (index, fruit) in fruits.iter().enumerate() {
        println!("{}: {}", index, fruit);
    }
}
```

---

## **5. Sorting and Searching**

Efficient sorting and searching are fundamental in many algorithms.

### **a. Sorting a Vector**

Using Rust's built-in sorting methods.

```rust
fn main() {
    let mut numbers = vec![4, 2, 5, 1, 3];
    
    // Sort in ascending order
    numbers.sort();
    println!("Sorted Ascending: {:?}", numbers);

    // Sort in descending order
    numbers.sort_by(|a, b| b.cmp(a));
    println!("Sorted Descending: {:?}", numbers);
}
```

### **b. Binary Search**

Performing binary search on a sorted collection.

```rust
fn main() {
    let numbers = vec![1, 3, 5, 7, 9, 11];
    let target = 7;

    match numbers.binary_search(&target) {
        Ok(index) => println!("Found at index: {}", index),
        Err(_) => println!("Not found"),
    }
}
```

### **c. `max` and `min` Functions**

Determining the maximum and minimum between two values.

```rust
fn main() {
    let a = 10;
    let b = 20;

    let maximum = std::cmp::max(a, b);
    let minimum = std::cmp::min(a, b);

    println!("Max: {}, Min: {}", maximum, minimum);
}
```

---

## **6. Mathematical Functions**

Mathematical operations are frequently required in algorithmic problems.

### **a. Greatest Common Divisor (GCD) and Least Common Multiple (LCM)**

Calculating GCD and LCM using Rust's standard library.

```rust
fn main() {
    let a = 48;
    let b = 180;

    let gcd = gcd(a, b);
    let lcm = lcm(a, b);

    println!("GCD: {}, LCM: {}", gcd, lcm);
}

// Function to compute GCD
fn gcd(a: u64, b: u64) -> u64 {
    if b == 0 { a } else { gcd(b, a % b) }
}

// Function to compute LCM
fn lcm(a: u64, b: u64) -> u64 {
    (a * b) / gcd(a, b)
}
```

### **b. Power Function**

Calculating exponents, useful in combinatorics and dynamic programming.

```rust
fn main() {
    let base = 2;
    let exponent = 10;
    
    let result = power(base, exponent);
    println!("{}^{} = {}", base, exponent, result);
}

// Recursive power function
fn power(base: i32, exp: u32) -> i32 {
    if exp == 0 { 1 }
    else { base * power(base, exp - 1) }
}
```

### **c. Bit Manipulation**

Performing operations at the bit level, essential for certain algorithms.

```rust
fn main() {
    let x = 5; // 0101 in binary
    let y = 3; // 0011 in binary

    let and = x & y;
    let or = x | y;
    let xor = x ^ y;
    let not = !x;

    println!("AND: {}, OR: {}, XOR: {}, NOT: {}", and, or, xor, not);
}
```

---

## **7. Utility Functions**

Various utility functions simplify common tasks.

### **a. Tuple Destructuring**

Extracting multiple values from a single compound value.

```rust
fn main() {
    let pair = (10, "hello");

    let (number, greeting) = pair;
    println!("Number: {}, Greeting: {}", number, greeting);
}
```

### **b. Option and Result Handling**

Managing values that may or may not be present or operations that may fail.

```rust
fn main() {
    // Option example
    let some_number: Option<i32> = Some(10);
    if let Some(n) = some_number {
        println!("Number is {}", n);
    } else {
        println!("No number found");
    }

    // Result example
    let parsed: Result<i32, _> = "42".parse();
    match parsed {
        Ok(n) => println!("Parsed number: {}", n),
        Err(e) => println!("Error parsing number: {}", e),
    }
}
```

### **c. Enumerate with Iterators**

Using `enumerate` within iterator chains.

```rust
fn main() {
    let fruits = vec!["apple", "banana", "cherry"];

    let indexed_fruits: Vec<(usize, &str)> = fruits.iter()
        .enumerate()
        .collect();

    println!("Indexed Fruits: {:?}", indexed_fruits);
}
```

---

## **8. Additional Useful Functions and Macros**

### **a. `assert_eq!` Macro for Testing**

Ensures that two expressions are equal, useful for internal testing.

```rust
fn main() {
    let a = 5;
    let b = 5;
    assert_eq!(a, b);
    println!("Assertion passed!");
}
```

### **b. `swap` Method for Vectors**

Swapping elements in a vector, useful in sorting algorithms.

```rust
fn main() {
    let mut vec = vec![1, 2, 3, 4, 5];
    vec.swap(0, 4); // Swap first and last elements
    println!("Swapped Vector: {:?}", vec);
}
```

### **c. `split_at` Method**

Splits a slice into two at the specified index.

```rust
fn main() {
    let nums = [1, 2, 3, 4, 5];
    let (left, right) = nums.split_at(2);
    println!("Left: {:?}, Right: {:?}", left, right);
}
```

---

## **9. Common Algorithms and Their Rust Implementations**

### **a. Depth-First Search (DFS)**

Implementing DFS using recursion.

```rust
use std::collections::VecDeque;

fn main() {
    // Example graph represented as an adjacency list
    let graph = vec![
        vec![1, 2],
        vec![0, 3, 4],
        vec![0, 4],
        vec![1],
        vec![1, 2],
    ];
    let start = 0;
    let visited = dfs(&graph, start);
    println!("Visited Nodes: {:?}", visited);
}

fn dfs(graph: &Vec<Vec<usize>>, start: usize) -> Vec<usize> {
    let mut visited = vec![];
    let mut stack = vec![start];
    let mut seen = vec![false; graph.len()];

    while let Some(node) = stack.pop() {
        if !seen[node] {
            seen[node] = true;
            visited.push(node);
            for &neighbor in &graph[node] {
                if !seen[neighbor] {
                    stack.push(neighbor);
                }
            }
        }
    }

    visited
}
```

### **b. Breadth-First Search (BFS)**

Implementing BFS using a queue.

```rust
use std::collections::VecDeque;

fn main() {
    // Example graph represented as an adjacency list
    let graph = vec![
        vec![1, 2],
        vec![0, 3, 4],
        vec![0, 4],
        vec![1],
        vec![1, 2],
    ];
    let start = 0;
    let visited = bfs(&graph, start);
    println!("Visited Nodes: {:?}", visited);
}

fn bfs(graph: &Vec<Vec<usize>>, start: usize) -> Vec<usize> {
    let mut visited = vec![];
    let mut queue = VecDeque::new();
    let mut seen = vec![false; graph.len()];

    queue.push_back(start);
    seen[start] = true;

    while let Some(node) = queue.pop_front() {
        visited.push(node);
        for &neighbor in &graph[node] {
            if !seen[neighbor] {
                seen[neighbor] = true;
                queue.push_back(neighbor);
            }
        }
    }

    visited
}
```

### **c. Binary Search Tree (BST) Insertion**

Implementing a simple BST insert function.

```rust
use std::cmp::Ordering;

#[derive(Debug)]
struct TreeNode {
    val: i32,
    left: Option<Box<TreeNode>>,
    right: Option<Box<TreeNode>>,
}

impl TreeNode {
    fn new(val: i32) -> Self {
        TreeNode { val, left: None, right: None }
    }

    fn insert(&mut self, val: i32) {
        match val.cmp(&self.val) {
            Ordering::Less => {
                if let Some(ref mut left) = self.left {
                    left.insert(val);
                } else {
                    self.left = Some(Box::new(TreeNode::new(val)));
                }
            },
            Ordering::Greater => {
                if let Some(ref mut right) = self.right {
                    right.insert(val);
                } else {
                    self.right = Some(Box::new(TreeNode::new(val)));
                }
            },
            Ordering::Equal => {} // Duplicate values are not inserted
        }
    }
}

fn main() {
    let mut root = TreeNode::new(10);
    root.insert(5);
    root.insert(15);
    root.insert(3);
    root.insert(7);
    println!("BST Root: {:?}", root);
}
```

---

## **10. Modular Code with Functions**

Breaking code into reusable functions enhances readability and maintainability.

```rust
use std::io::{self, BufRead};

// Function to read a line and parse it into a vector of integers
fn read_numbers() -> Vec<i32> {
    let stdin = io::stdin();
    let line = stdin.lock().lines().next().unwrap().unwrap();
    line.split_whitespace()
        .map(|s| s.parse().expect("parse error"))
        .collect()
}

// Function to find the maximum number in a vector
fn find_max(nums: &Vec<i32>) -> i32 {
    *nums.iter().max().expect("Vector is empty")
}

fn main() {
    println!("Enter numbers separated by space:");
    let numbers = read_numbers();
    let max_num = find_max(&numbers);
    println!("Maximum number is {}", max_num);
}
```

---

## **Conclusion**

Mastering the functions and methods outlined above will significantly enhance your ability to tackle competitive programming problems in Rust. Here's a quick recap of the covered areas:

1. **Input and Output:** Efficient reading and writing using `BufReader` and `println!`.
2. **Data Structures:** Utilizing `Vec`, `HashMap`, `BinaryHeap`, and `BTreeSet` for optimal data management.
3. **String Manipulation:** Splitting, parsing, and iterating over strings effectively.
4. **Iterators:** Leveraging iterator methods like `map`, `filter`, `fold`, and `enumerate` for concise code.
5. **Sorting and Searching:** Implementing sorting algorithms and binary search for optimal performance.
6. **Mathematical Functions:** Performing GCD, LCM, exponentiation, and bit manipulation.
7. **Utility Functions:** Utilizing tuples, `Option`/`Result`, and `assert_eq!` for robust code.
8. **Common Algorithms:** Implementing DFS, BFS, and BST operations for foundational algorithmic challenges.
9. **Modular Code:** Writing reusable functions to enhance code readability and maintainability.

**Tips for Success:**

- **Practice Regularly:** Implement these functions and incorporate them into solving various problems on platforms like LeetCode and HackerRank.
- **Understand the Standard Library:** Familiarize yourself with Rust's [Standard Library](https://doc.rust-lang.org/std/) for more advanced functionalities.
- **Optimize for Performance:** Rust's performance is one of its key strengths. Always consider time and space complexity.
- **Write Clean Code:** Use Rust's ownership and borrowing principles to write efficient and bug-free code.
- **Seek Feedback:** Engage with the Rust community on platforms like [Rust Programming Language Forum](https://users.rust-lang.org/) or [Reddit's r/rust](https://www.reddit.com/r/rust/) to seek feedback and learn best practices.


