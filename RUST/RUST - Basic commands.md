# Day 1: Getting Started with Rust (Extended)

## Expanding Your Understanding: Basic Rust Commands and Package Management

Building upon our initial exploration of Rust and setting up the development environment, today we'll delve into the essential **Rust commands**, **package management with Cargo**, and **best practices** for managing Rust projects. Understanding these commands and practices is crucial for efficient Rust development.

---

## **1. Essential Rust Commands**

Rust primarily uses two tools for development:

- **`rustup`**: Manages Rust toolchains (compilers, Cargo, etc.).
- **`cargo`**: Rust's package manager and build system.

### **1.1 `rustup` Commands**

`rustup` is the official toolchain installer for Rust. It allows you to install, update, and manage multiple versions of Rust and associated tools.

#### **1.1.1 Installing Rust**

If you haven't installed Rust yet, use the following command:

```powershell
# Download and install Rust using rustup
rustup-init.exe
```

**Steps:**

1. **Download `rustup-init.exe`:**
   - Visit the [Rust installation page](https://www.rust-lang.org/tools/install).
   - Download the installer for Windows.

2. **Run the Installer:**
   - Execute the downloaded `rustup-init.exe` file.
   - Follow the on-screen prompts to complete the installation.

#### **1.1.2 Updating Rust and Cargo**

Rust and Cargo receive regular updates. Keeping them up-to-date ensures you have the latest features and security patches.

```powershell
# Update Rust toolchain to the latest stable version
rustup update
```

**Output:**
```
info: syncing channel updates for 'stable-i686-pc-windows-msvc'
info: latest update on 2023-10-01, rust version 1.70.0 (your_version_here)
info: downloading component 'rustc'
info: downloading component 'cargo'
info: downloading component 'rust-std'
info: installing component 'rustc'
info: installing component 'cargo'
info: installing component 'rust-std'
info: syncing channel updates for 'nightly-i686-pc-windows-msvc'
```

**Explanation:**

- **`rustup update`**: Updates all installed toolchains to their latest versions.

#### **1.1.3 Checking Installed Toolchains**

```powershell
# List all installed toolchains
rustup toolchain list
```

**Output:**
```
stable-i686-pc-windows-msvc (default)
nightly-i686-pc-windows-msvc
```

**Explanation:**

- **`rustup toolchain list`**: Displays all Rust toolchains installed on your system.

#### **1.1.4 Switching Toolchains**

You can have multiple Rust toolchains (e.g., stable, beta, nightly) installed simultaneously.

```powershell
# Install the nightly toolchain
rustup toolchain install nightly

# Set nightly as the default toolchain
rustup default nightly
```

**Explanation:**

- **`rustup toolchain install nightly`**: Installs the nightly version of Rust.
- **`rustup default nightly`**: Sets nightly as the default toolchain for all Rust projects.

#### **1.1.5 Uninstalling a Toolchain**

```powershell
# Uninstall the nightly toolchain
rustup toolchain uninstall nightly
```

**Explanation:**

- **`rustup toolchain uninstall`**: Removes a specified toolchain from your system.

### **1.2 `cargo` Commands**

`Cargo` is Rust's powerful package manager and build system. It handles project creation, dependency management, building, testing, and more.

#### **1.2.1 Creating a New Project**

```powershell
# Create a new binary project named 'my_app'
cargo new my_app
```

```powershell
# Create a new library project named 'my_lib'
cargo new my_lib --lib
```

**Explanation:**

- **`cargo new my_app`**: Initializes a new binary (executable) Rust project.
- **`cargo new my_lib --lib`**: Initializes a new library Rust project.

**Project Structure Example (`my_app`):**
```
my_app/
├── Cargo.toml
└── src
    └── main.rs
```

**Project Structure Example (`my_lib`):**
```
my_lib/
├── Cargo.toml
└── src
    └── lib.rs
```

#### **1.2.2 Building a Project**

```powershell
# Navigate to the project directory
cd my_app

# Build the project in debug mode
cargo build
```

**Output:**
```
   Compiling my_app v0.1.0 (C:\Path\To\my_app)
    Finished dev [unoptimized + debuginfo] target(s) in 1.23s
```

**Explanation:**

- **`cargo build`**: Compiles the project in debug mode, optimizing for compilation speed and debuggability.

```powershell
# Build the project in release mode (optimized)
cargo build --release
```

**Output:**
```
   Compiling my_app v0.1.0 (C:\Path\To\my_app)
    Finished release [optimized + debuginfo] target(s) in 2.34s
```

**Explanation:**

- **`--release`**: Compiles the project with optimizations, suitable for production builds.

#### **1.2.3 Running a Project**

```powershell
# Run the project (builds in debug mode if not already built)
cargo run
```

**Output:**
```
   Compiling my_app v0.1.0 (C:\Path\To\my_app)
    Finished dev [unoptimized + debuginfo] target(s) in 1.23s
     Running `target\debug\my_app.exe`
Hello, world!
```

**Explanation:**

- **`cargo run`**: Combines building and running the project. It compiles the project if there are changes and then executes the binary.

#### **1.2.4 Checking the Project**

```powershell
# Check the project for errors without building
cargo check
```

**Output:**
```
    Checking my_app v0.1.0 (C:\Path\To\my_app)
    Finished dev [unoptimized + debuginfo] target(s) in 0.89s
```

**Explanation:**

- **`cargo check`**: Quickly checks the code for errors without producing a binary. It's faster than a full build, useful for iterative development.

#### **1.2.5 Testing the Project**

```powershell
# Run all tests in the project
cargo test
```

**Output:**
```
running 1 test
test tests::it_works ... ok

test result: ok. 1 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out
```

**Explanation:**

- **`cargo test`**: Executes all tests defined in your project, ensuring code correctness.

#### **1.2.6 Updating Dependencies**

```powershell
# Update all dependencies to the latest versions allowed by Cargo.toml
cargo update
```

**Explanation:**

- **`cargo update`**: Updates dependencies specified in `Cargo.toml` to the latest versions that meet the version requirements.

#### **1.2.7 Adding Dependencies**

Cargo manages dependencies through the `Cargo.toml` file. You can add dependencies manually or use the `cargo add` command provided by the `cargo-edit` crate.

**Installing `cargo-edit`:**

```powershell
# Install cargo-edit to use cargo add and cargo remove
cargo install cargo-edit
```

**Adding a Dependency with `cargo add`:**

```powershell
# Add the 'serde' crate with the 'derive' feature
cargo add serde --features derive
```

**Output:**
```
    Updating crates.io index
    Adding serde v1.0 to dependencies
```

**Explanation:**

- **`cargo add serde --features derive`**: Adds the `serde` crate to your project's dependencies with the specified features.

**Manual Addition:**

Alternatively, edit `Cargo.toml` directly:

```toml
[dependencies]
serde = { version = "1.0", features = ["derive"] }
```

#### **1.2.8 Removing Dependencies**

```powershell
# Remove the 'serde' crate from dependencies
cargo remove serde
```

**Output:**
```
    Removing serde from Cargo.toml
```

**Explanation:**

- **`cargo remove serde`**: Deletes the `serde` dependency from `Cargo.toml`.

**Manual Removal:**

Alternatively, delete or comment out the dependency in `Cargo.toml`:

```toml
[dependencies]
# serde = { version = "1.0", features = ["derive"] }
```

#### **1.2.9 Listing Installed Packages**

Cargo doesn't have a direct equivalent to Python's `pip list`, but you can view dependencies by inspecting the `Cargo.toml` and `Cargo.lock` files.

**Viewing Dependencies:**

```powershell
# List dependencies declared in Cargo.toml
cat Cargo.toml
```

**Viewing Locked Dependencies:**

```powershell
# List all dependencies, including transitive ones, from Cargo.lock
cargo tree
```

**Explanation:**

- **`cargo tree`**: Displays a hierarchical view of dependencies, showing both direct and transitive dependencies.

**Example:**

```powershell
cargo tree
```

**Output:**
```
my_app v0.1.0 (C:\Path\To\my_app)
├── serde v1.0.130
│   ├── serde_derive v1.0.130
│   └── serde_json v1.0.68
└── serde_json v1.0.68
```

### **1.3 `cargo install` and `cargo uninstall`**

Cargo also manages executable binaries through the `cargo install` and `cargo uninstall` commands.

#### **1.3.1 Installing Binaries**

```powershell
# Install the 'ripgrep' binary from crates.io
cargo install ripgrep
```

**Output:**
```
    Downloading ripgrep v13.0.0
   Installing ripgrep v13.0.0
    Finished release [optimized] target(s) in 10.23s
     Installed package `ripgrep v13.0.0` (executable `rg`) binary at ...
```

**Explanation:**

- **`cargo install ripgrep`**: Downloads, builds, and installs the `ripgrep` binary, making it available system-wide.

#### **1.3.2 Uninstalling Binaries**

```powershell
# Uninstall the 'ripgrep' binary
cargo uninstall ripgrep
```

**Output:**
```
Removing ripgrep v13.0.0
```

**Explanation:**

- **`cargo uninstall ripgrep`**: Removes the installed `ripgrep` binary from your system.

#### **1.3.3 Listing Installed Binaries**

Cargo doesn't provide a direct command to list all installed binaries. However, you can check the installation directory.

**Default Installation Path:**

```powershell
# Default Cargo binary installation path on Windows
%USERPROFILE%\.cargo\bin
```

**List Installed Binaries:**

```powershell
# List all binaries in Cargo's bin directory
Get-ChildItem "$env:USERPROFILE\.cargo\bin"
```

**Example Output:**
```
    Directory: C:\Users\YourName\.cargo\bin

Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
-a----       10/20/2023   10:00 AM            1234 rg.exe
-a----       10/20/2023   09:50 AM             567 other_binary.exe
```

**Explanation:**

- **`%USERPROFILE%\.cargo\bin`**: The directory where Cargo installs binaries.
- **`Get-ChildItem`**: PowerShell command to list files in the specified directory.

---

## **2. Package Management in Rust**

Rust leverages Cargo for comprehensive package management, analogous to Python's `pip`. It handles dependency resolution, versioning, and compilation.

### **2.1 Adding Dependencies**

Dependencies are external crates (libraries) that your project relies on.

#### **2.1.1 Using `cargo add`**

```powershell
# Add the 'rand' crate for random number generation
cargo add rand

# Add the 'tokio' crate for asynchronous programming with specific features
cargo add tokio --features full
```

**Output:**
```
    Adding rand v0.8
    Adding tokio v1.21 with features: full
```

**Explanation:**

- **`cargo add rand`**: Adds the `rand` crate with the latest version compatible with your project.
- **`cargo add tokio --features full`**: Adds the `tokio` crate with the `full` feature enabled, enabling comprehensive async support.

#### **2.1.2 Manual Addition**

Alternatively, manually edit `Cargo.toml`:

```toml
[dependencies]
rand = "0.8"
tokio = { version = "1.21", features = ["full"] }
```

**Explanation:**

- **Version Specification:** Defines which versions of the crate are permissible, following [Semantic Versioning](https://semver.org/).
- **Feature Flags:** Enables optional functionalities provided by the crate.

### **2.2 Updating Dependencies**

Keeping dependencies updated ensures you benefit from bug fixes, performance improvements, and new features.

#### **2.2.1 Updating All Dependencies**

```powershell
# Update all dependencies to the latest versions allowed by Cargo.toml
cargo update
```

**Explanation:**

- **`cargo update`**: Fetches the latest versions of dependencies that match the version constraints in `Cargo.toml`.

#### **2.2.2 Updating a Specific Dependency**

```powershell
# Update only the 'rand' crate
cargo update -p rand
```

**Explanation:**

- **`-p rand`**: Specifies the package to update, limiting the operation to the `rand` crate.

### **2.3 Removing Dependencies**

Removing unnecessary dependencies helps keep your project lean and reduces potential security vulnerabilities.

#### **2.3.1 Using `cargo remove`**

```powershell
# Remove the 'rand' crate from dependencies
cargo remove rand
```

**Output:**
```
    Removing rand from Cargo.toml
```

**Explanation:**

- **`cargo remove rand`**: Deletes the `rand` dependency from `Cargo.toml`.

#### **2.3.2 Manual Removal**

Alternatively, edit `Cargo.toml` to delete or comment out the dependency:

```toml
[dependencies]
# rand = "0.8"
```

**Explanation:**

- **Manual Edit:** Directly modifying `Cargo.toml` to remove dependencies.

### **2.4 Viewing and Managing Dependency Versions**

#### **2.4.1 Specifying Exact Versions**

```toml
[dependencies]
serde = "1.0.130"  # Exact version 1.0.130
```

**Explanation:**

- **Exact Version:** Ensures your project uses the specified version without accepting patches or minor updates.

#### **2.4.2 Version Ranges**

```toml
[dependencies]
serde = "^1.0"      # Compatible with 1.x versions
tokio = ">=1.14, <1.21"  # Versions 1.14 up to, but not including, 1.21
```

**Explanation:**

- **Caret (`^`):** Allows updates that do not modify the left-most non-zero digit (e.g., `^1.0` accepts `1.0.1`, `1.1.0`, etc., but not `2.0.0`).
- **Inequalities (`>=`, `<`):** Define specific version constraints.

#### **2.4.3 Using `cargo tree` for Dependency Inspection**

```powershell
# Display the dependency graph
cargo tree
```

**Output:**
```
my_app v0.1.0 (C:\Path\To\my_app)
├── serde v1.0.130
│   ├── serde_derive v1.0.130
│   └── serde_json v1.0.68
└── tokio v1.21.0
    ├── mio v0.8.3
    └── ...
```

**Explanation:**

- **`cargo tree`**: Visualizes the entire dependency graph, including transitive dependencies, helping identify potential conflicts or unnecessary dependencies.

---

## **3. Managing and Updating Rust and Cargo**

Ensuring your Rust toolchain and Cargo are up-to-date is essential for leveraging the latest features and maintaining project compatibility.

### **3.1 Updating Rust and Cargo**

Rust and Cargo are typically updated together via `rustup`.

```powershell
# Update Rust and Cargo to the latest stable versions
rustup update stable
```

**Explanation:**

- **`rustup update stable`**: Updates the stable toolchain, including `rustc` and Cargo, to the latest version.

### **3.2 Updating Cargo Independently**

Cargo is bundled with the Rust toolchain, so updating Cargo independently isn't necessary. However, to ensure Cargo is updated, you can update the Rust toolchain:

```powershell
# Update to the latest stable Rust and Cargo
rustup update
```

**Explanation:**

- **`rustup update`**: Updates all installed toolchains, ensuring both Rust and Cargo are current.

### **3.3 Installing Specific Cargo Versions**

If needed, you can install specific toolchain versions that include particular Cargo versions.

```powershell
# Install Rust 1.69.0, which includes a specific Cargo version
rustup toolchain install 1.69.0
```

**Explanation:**

- **`rustup toolchain install <version>`**: Installs a specific Rust toolchain, including its associated Cargo version.

### **3.4 Verifying Cargo Version**

```powershell
# Check the installed Cargo version
cargo --version
```

**Output:**
```
cargo 1.70.0 (your_version_here)
```

**Explanation:**

- **`cargo --version`**: Displays the current Cargo version, ensuring you're using the intended version.

---

## **4. Best Practices for Managing Rust Projects**

Adhering to best practices ensures your Rust projects remain organized, efficient, and maintainable.

### **4.1 Project Initialization**

- **Use Cargo for Initialization:** Always initialize projects with Cargo (`cargo new` or `cargo init`) to leverage its features.
  
  ```powershell
  # Initialize a new binary project
  cargo new my_app
  
  # Initialize a new library project
  cargo new my_lib --lib
  ```

### **4.2 Dependency Management**

- **Add Dependencies Gradually:** Start with essential crates to minimize bloat.
- **Use Semantic Versioning:** Follow [Semantic Versioning](https://semver.org/) to manage dependencies (`^1.0` for compatibility).
- **Leverage `cargo add` and `cargo remove`:** Streamlines adding/removing dependencies.
  
  ```powershell
  # Add a dependency
  cargo add regex
  
  # Remove a dependency
  cargo remove regex
  ```

### **4.3 Organizing Code**

- **Use Modules and Submodules:** Encapsulate related functionalities.
  
  **Example:**

  ```
  my_app/
  ├── Cargo.toml
  └── src
      ├── main.rs
      └── utils/
          ├── mod.rs
          ├── math.rs
          └── strings.rs
  ```

  **`utils/mod.rs`:**

  ```rust:path/src/utils/mod.rs
  pub mod math;
  pub mod strings;
  ```

  **`utils/math.rs`:**

  ```rust:path/src/utils/math.rs
  /// Adds two numbers together.
  pub fn add(a: i32, b: i32) -> i32 {
      a + b
  }
  ```

  **`utils/strings.rs`:**

  ```rust:path/src/utils/strings.rs
  /// Converts a string to uppercase.
  pub fn to_uppercase(s: &str) -> String {
      s.to_uppercase()
  }
  ```

  **`main.rs`:**

  ```rust:path/src/main.rs
  mod utils;
  
  use utils::math::add;
  use utils::strings::to_uppercase;
  
  fn main() {
      let sum = add(5, 7);
      let shout = to_uppercase("hello");
  
      println!("Sum: {}", sum);
      println!("Shout: {}", shout);
  }
  ```

### **4.4 Version Control with Git**

- **Initialize Git Repository:**
  
  ```powershell
  # Initialize Git in the project directory
  git init
  ```

- **Use `.gitignore` to Exclude Unnecessary Files:**
  
  **Example `.gitignore`:**

  ```
  # Generated by Cargo
  /target/
  
  # IDE directories
  .vscode/
  
  # Binaries
  *.exe
  *.dll
  *.so
  *.dylib
  ```

- **Commit Regularly with Meaningful Messages:**
  
  ```powershell
  git add .
  git commit -m "Initial commit with project structure"
  ```

### **4.5 Writing Documentation**

- **Use Doc Comments (`///` or `//!`):**
  
  ```rust:path/src/utils/math.rs
  /// Adds two integers together.
  ///
  /// # Examples
  ///
  /// ```
  /// let result = my_app::utils::math::add(2, 3);
  /// assert_eq!(result, 5);
  /// ```
  pub fn add(a: i32, b: i32) -> i32 {
      a + b
  }
  ```

- **Generate Documentation with Cargo:**
  
  ```powershell
  # Generate and open documentation in the browser
  cargo doc --open
  ```

### **4.6 Testing**

- **Write Unit Tests:**
  
  ```rust:path/src/utils/math.rs
  /// Adds two integers together.
  pub fn add(a: i32, b: i32) -> i32 {
      a + b
  }
  
  #[cfg(test)]
  mod tests {
      use super::*;
  
      #[test]
      fn it_adds_two_numbers() {
          assert_eq!(add(2, 3), 5);
      }
  }
  ```

- **Run Tests with Cargo:**
  
  ```powershell
  cargo test
  ```

### **4.7 Continuous Integration (CI)**

- **Automate Testing with GitHub Actions:**
  
  **Example Workflow (`.github/workflows/rust.yml`):**
  
  ```yaml
  name: Rust CI
  
  on:
    push:
      branches: [ main ]
    pull_request:
      branches: [ main ]
  
  jobs:
    build:
      runs-on: windows-latest
  
      steps:
      - uses: actions/checkout@v2
      - name: Install Rust
        uses: actions-rs/toolchain@v1
        with:
          toolchain: stable
      - name: Build
        run: cargo build --verbose
      - name: Run Tests
        run: cargo test --verbose
  ```

**Explanation:**

- **Workflow Triggers:** On pushes and pull requests to the `main` branch.
- **Jobs:**
  - **Checkout Code:** Uses `actions/checkout@v2` to clone the repository.
  - **Install Rust:** Uses `actions-rs/toolchain@v1` to set up the Rust toolchain.
  - **Build Project:** Compiles the project.
  - **Run Tests:** Executes all defined tests.

---

## **5. Practical Examples**

Let's consolidate our understanding with practical examples demonstrating command usage and project management best practices.

### **5.1 Initializing and Managing a Project**

#### **5.1.1 Creating a New Project**

```powershell
# Create a new binary project named 'calculator'
cargo new calculator
```

**Project Structure:**
```
calculator/
├── Cargo.toml
└── src
    └── main.rs
```

#### **5.1.2 Adding Dependencies**

```powershell
# Navigate to the project directory
cd calculator

# Add 'clap' for command-line argument parsing with 'derive' feature
cargo add clap --features derive

# Add 'serde' for serialization with 'derive' feature
cargo add serde --features derive

# Add 'serde_json' for JSON handling
cargo add serde_json
```

**Updated `Cargo.toml`:**

```toml
[package]
name = "calculator"
version = "0.1.0"
edition = "2021"

[dependencies]
clap = { version = "4.1", features = ["derive"] }
serde = { version = "1.0", features = ["derive"] }
serde_json = "1.0"
```

#### **5.1.3 Writing Modular Code with Dependencies**

**`src/main.rs`:**

```rust:path/calculator/src/main.rs
mod operations;

use clap::Parser;
use operations::{add, subtract, multiply};

/// Simple Calculator Application
#[derive(Parser)]
struct Args {
    /// First number
    a: i32,

    /// Second number
    b: i32,

    /// Operation: add, subtract, multiply
    operation: String,
}

fn main() {
    let args = Args::parse();

    let result = match args.operation.as_str() {
        "add" => add(args.a, args.b),
        "subtract" => subtract(args.a, args.b),
        "multiply" => multiply(args.a, args.b),
        _ => {
            eprintln!("Unsupported operation: {}", args.operation);
            std::process::exit(1);
        }
    };

    println!("Result: {}", result);
}
```

**`src/operations/mod.rs`:**

```rust:path/calculator/src/operations/mod.rs
pub mod add;
pub mod subtract;
pub mod multiply;
```

**`src/operations/add.rs`:**

```rust:path/calculator/src/operations/add.rs
/// Adds two integers together.
///
/// # Examples
///
/// ```
/// let sum = calculator::operations::add::add(2, 3);
/// assert_eq!(sum, 5);
/// ```
pub fn add(a: i32, b: i32) -> i32 {
    a + b
}
```

**`src/operations/subtract.rs`:**

```rust:path/calculator/src/operations/subtract.rs
/// Subtracts the second integer from the first.
///
/// # Examples
///
/// ```
/// let difference = calculator::operations::subtract::subtract(5, 3);
/// assert_eq!(difference, 2);
/// ```
pub fn subtract(a: i32, b: i32) -> i32 {
    a - b
}
```

**`src/operations/multiply.rs`:**

```rust:path/calculator/src/operations/multiply.rs
/// Multiplies two integers together.
///
/// # Examples
///
/// ```
/// let product = calculator::operations::multiply::multiply(4, 5);
/// assert_eq!(product, 20);
/// ```
pub fn multiply(a: i32, b: i32) -> i32 {
    a * b
}
```

### **5.2 Running the Calculator Application**

#### **5.2.1 Adding Functionality**

```powershell
# Run the 'add' operation
cargo run -- add 10 5
```

**Output:**
```
Result: 15
```

```powershell
# Run the 'subtract' operation
cargo run -- subtract 10 5
```

**Output:**
```
Result: 5
```

```powershell
# Run the 'multiply' operation
cargo run -- multiply 10 5
```

**Output:**
```
Result: 50
```

#### **5.2.2 Handling Unsupported Operations**

```powershell
# Attempt to run an unsupported operation
cargo run -- divide 10 5
```

**Output:**
```
Unsupported operation: divide
```

**Explanation:**

- The application gracefully handles unsupported operations by displaying an error message and exiting with a non-zero status code.

### **5.3 Testing the Calculator Operations**

**`src/operations/add.rs`:**

```rust:path/calculator/src/operations/add.rs
/// Adds two integers together.
///
/// # Examples
///
/// ```
/// let sum = calculator::operations::add::add(2, 3);
/// assert_eq!(sum, 5);
/// ```
pub fn add(a: i32, b: i32) -> i32 {
    a + b
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_add() {
        assert_eq!(add(2, 3), 5);
        assert_eq!(add(-1, 1), 0);
        assert_eq!(add(0, 0), 0);
    }
}
```

**`src/operations/subtract.rs`:**

```rust:path/calculator/src/operations/subtract.rs
/// Subtracts the second integer from the first.
///
/// # Examples
///
/// ```
/// let difference = calculator::operations::subtract::subtract(5, 3);
/// assert_eq!(difference, 2);
/// ```
pub fn subtract(a: i32, b: i32) -> i32 {
    a - b
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_subtract() {
        assert_eq!(subtract(5, 3), 2);
        assert_eq!(subtract(0, 0), 0);
        assert_eq!(subtract(-2, -3), 1);
    }
}
```

**`src/operations/multiply.rs`:**

```rust:path/calculator/src/operations/multiply.rs
/// Multiplies two integers together.
///
/// # Examples
///
/// ```
/// let product = calculator::operations::multiply::multiply(4, 5);
/// assert_eq!(product, 20);
/// ```
pub fn multiply(a: i32, b: i32) -> i32 {
    a * b
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_multiply() {
        assert_eq!(multiply(4, 5), 20);
        assert_eq!(multiply(-2, 3), -6);
        assert_eq!(multiply(0, 5), 0);
    }
}
```

**Running Tests:**

```powershell
cargo test
```

**Output:**
```
running 3 tests
test operations::add::tests::test_add ... ok
test operations::subtract::tests::test_subtract ... ok
test operations::multiply::tests::test_multiply ... ok

test result: ok. 3 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out
```

**Explanation:**

- **`#[cfg(test)]`**: Compiles the enclosed module only when running tests.
- **`#[test]`**: Marks a function as a test case.
- **`cargo test`**: Runs all test cases in the project.

---

## **6. Summary and Next Steps**

Today, you've:

- **Learned Essential Rust Commands:**
  - Managed Rust toolchains with `rustup`.
  - Utilized Cargo for project creation, building, running, testing, and managing dependencies.
  - Installed and uninstalled Cargo binaries.

- **Understood Package Management:**
  - Added, updated, and removed dependencies using `cargo add` and `cargo remove`.
  - Explored versioning and dependency inspection with `cargo tree`.

- **Adopted Best Practices:**
  - Organized code with modules and submodules.
  - Maintained version control with Git.
  - Documented code and wrote comprehensive tests.

- **Implemented Practical Examples:**
  - Created a modular calculator application.
  - Managed dependencies and extended functionality.
  - Ran and tested the application effectively.

**Next Steps:**

- **Practice:** Continue experimenting with Cargo commands and managing dependencies.
- **Prepare for Day 2:** Dive into Rust basics, focusing on variables and mutability.

---

## **Additional Resources**

- **Official Rust Book:** [The Rust Programming Language](https://doc.rust-lang.org/book/)
- **Cargo Documentation:** [Cargo Book](https://doc.rust-lang.org/cargo/)
- **Cargo-Edit Documentation:** [Cargo-Edit GitHub](https://github.com/killercup/cargo-edit)
- **Cheatsheet:** [Rust & Cargo Cheatsheet](https://cheats.rs/)
- **Cargo Commands Reference:** [Cargo Commands](https://doc.rust-lang.org/cargo/commands/index.html)

Stay disciplined, experiment actively, and don't hesitate to revisit topics as needed. Tomorrow, we'll delve deeper into Rust's fundamental concepts like variables and mutability. Happy coding!