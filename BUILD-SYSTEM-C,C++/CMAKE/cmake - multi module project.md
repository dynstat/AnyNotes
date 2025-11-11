In case, your CMake project becomes a **multi-module architecture**, where each component (`dependent`, `independent`, `driver`) has:

* Its **own headers (`include/`)**
* Its **own source code (`src/`)**
* Its **own tests (`tests/`)**
* Possibly its own **entry point (like `main.c`)** (only one will become the main `executable`)

---

## 🔧 Objective

Build the project such that:

* `dependent` and `independent` are **modular libraries**
* `driver` contains `main.c`, and links these libraries
* Everything is clean, reusable, and independently testable

---

## 🧱 Folder Layout

```
MyProject/
├── CMakeLists.txt              <-- root
├── CMake/                      <-- custom cmake files (optional)
│
├── dependent/
│   ├── include/
│   │   └── dependent/          <-- e.g. dependent/foo.h
│   ├── src/
│   │   └── foo.c
│   ├── tests/
│   │   └── test_dependent.c
│   └── CMakeLists.txt
│
├── independent/
│   ├── include/
│   │   └── independent/
│   ├── src/
│   │   └── bar.cpp
│   ├── tests/
│   │   └── test_independent.cpp
│   └── CMakeLists.txt
│
├── driver/
│   ├── include/
│   │   └── driver/
│   ├── src/
│   │   └── main.c
│   └── CMakeLists.txt
```

---
---

## ✅ `MyProject/CMakeLists.txt` (Root)

```cmake
# Specify the minimum required version of CMake to use
cmake_minimum_required(VERSION 3.16)

# Declare the project name and supported programming languages
project(MyProject LANGUAGES C CXX)

# Set the C standard and require it
set(CMAKE_C_STANDARD 99)                 # Enforce C99 standard
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set the C++ standard and require it
set(CMAKE_CXX_STANDARD 20)               # Enforce C++20 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories for built binaries and libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)   # Static/shared libs (e.g., .a/.so)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)   # Shared libs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)   # Executables

# Include subdirectories (each has its own CMakeLists.txt)
add_subdirectory(dependent)              # Add dependent module
add_subdirectory(independent)            # Add independent module
add_subdirectory(driver)                 # Add driver (main app)
```

---

## ✅ `MyProject/dependent/CMakeLists.txt`

```cmake
# Collect all source files under src/ recursively
file(GLOB_RECURSE DEP_SRC "src/*.c")         # All .c files under src/
file(GLOB_RECURSE DEP_HEADERS "include/*.h") # All .h files under include/

# Define a static library target named "dependent"
add_library(dependent STATIC ${DEP_SRC} ${DEP_HEADERS})  
# 'STATIC' means it will be compiled into a .a or .lib

# Expose include/ path to anything that links against 'dependent'
target_include_directories(dependent 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# 'PUBLIC' means both this target and others that link it will see this include path
```

---

## ✅ `MyProject/independent/CMakeLists.txt`

```cmake
# Collect C++ source and header files
file(GLOB_RECURSE IND_SRC "src/*.cpp")       # All .cpp files under src/
file(GLOB_RECURSE IND_HEADERS "include/*.h") # All headers under include/

# Define the static library 'independent'
add_library(independent STATIC ${IND_SRC} ${IND_HEADERS})  

# Set the include path visible to users of this library
target_include_directories(independent 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
```

---

## ✅ `MyProject/driver/CMakeLists.txt`

```cmake
# Collect main driver source and header files
file(GLOB_RECURSE DRIVER_SRC "src/*.c")        # All .c files under src/
file(GLOB_RECURSE DRIVER_HEADERS "include/*.h")# All headers under include/

# Create the executable 'my_app' from these files
add_executable(my_app ${DRIVER_SRC} ${DRIVER_HEADERS})

# Add local include directory (driver/include/) as private
target_include_directories(my_app 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# 'PRIVATE' means only this target will see this include path

# Link the main app with the dependent and independent libraries
target_link_libraries(my_app 
    PRIVATE dependent
    PRIVATE independent
)
# 'PRIVATE' means the linked libs are used internally, and not propagated to targets that depend on my_app
```

---

### 🔁 Explanation Summary of All Key CMake Commands

| Command                             | Meaning                                               |
| ----------------------------------- | ----------------------------------------------------- |
| `cmake_minimum_required(...)`       | Ensures compatibility with specific CMake features    |
| `project(...)`                      | Defines project name, languages, and version          |
| `set(CMAKE_*_STANDARD ...)`         | Sets language standard versions (C/C++)               |
| `set(CMAKE_*_OUTPUT_DIRECTORY ...)` | Controls where to place compiled output               |
| `add_subdirectory(...)`             | Includes other directories with CMakeLists.txt        |
| `file(GLOB_RECURSE ...)`            | Collects files using pattern matching (wildcard)      |
| `add_library(...)`                  | Defines a static/shared library target                |
| `add_executable(...)`               | Defines an executable target (main program)           |
| `target_include_directories(...)`   | Specifies include paths for headers                   |
| `target_link_libraries(...)`        | Links libraries to targets (ensures dependency chain) |

---

### 🧪 Want to add tests too?

If each module (`dependent`, `independent`) has its own `tests/` folder, add this at the end of their `CMakeLists.txt`:

```cmake
enable_testing()

# Add test target
add_executable(test_dependent tests/test_dependent.c)
target_link_libraries(test_dependent PRIVATE dependent)
add_test(NAME DepTest COMMAND test_dependent)
```

Then you can run all tests via:

```bash
cmake --build . --target test
ctest
```

---

Here's a **complete breakdown** of:

---

## ✅ \[1] **Using external libraries as dependencies**

Static (`.a`, `.lib`) or shared (`.so`, `.dll`, `.dylib`)
→ When **your project builds an executable**

---

## ✅ \[2] **Creating your own static or shared libraries as output**

---

# 📦 1. Using External Libraries in Your Executable Project

---

### 📁 Typical Directory Structure

```
MyProject/
├── CMakeLists.txt
├── main.c
├── lib/
│   ├── mylib.a               ← static lib
│   └── mylib.so / mylib.dll  ← shared lib
├── include/
│   └── mylib.h
```

---

## 🔧 A. `CMakeLists.txt` to link static or shared lib

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyApp LANGUAGES C)

# Add the executable from your main source
add_executable(my_app main.c)

# Include path to external headers
target_include_directories(my_app PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Path to the libraries
link_directories(${CMAKE_SOURCE_DIR}/lib)

# For STATIC linking
target_link_libraries(my_app PRIVATE mylib)   # Links to libmylib.a

# OR for SHARED linking (same line, different library file used)
# If mylib.so or mylib.dll is in the path, same `target_link_libraries()` applies

# Optional: specify type explicitly (not mandatory here if you're linking `.a` or `.so` directly)
# target_link_libraries(my_app PRIVATE $<TARGET_FILE:mylib>)
```

---

## 📌 Where to Put External Dependencies

| Type        | Recommended Location    | Notes                                                                 |
| ----------- | ----------------------- | --------------------------------------------------------------------- |
| Headers     | `include/`              | Needed in compile phase                                               |
| Static libs | `lib/mylib.a` or `.lib` | Linked at compile-time                                                |
| Shared libs | `lib/mylib.so/.dll`     | Needed at runtime (place near binary or use `LD_LIBRARY_PATH`/`PATH`) |

---

## ⚠️ Best Practices

* Use `target_include_directories()` not global `include_directories()`
* Use `target_link_libraries()` not global `link_libraries()`
* Use `link_directories()` **only if** not using targets

---

# 🛠️ 2. Creating Your Own Static / Shared Libraries

---

## A. Static Library Output

### 📁 Structure:

```
MyProject/
├── CMakeLists.txt
├── include/
│   └── mylib.h
├── src/
│   └── mylib.c
```

### 🧾 `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyLib LANGUAGES C)

# Collect sources
file(GLOB SRC_FILES src/*.c)

# Create static library
add_library(mylib STATIC ${SRC_FILES})

# Expose public headers
target_include_directories(mylib PUBLIC ${CMAKE_SOURCE_DIR}/include)
```

### 🛠 Output:

* `libmylib.a` (Linux/macOS)
* `mylib.lib` (Windows)

---

## B. Shared Library Output

```cmake
# Same structure, just change STATIC → SHARED
add_library(mylib SHARED ${SRC_FILES})

# Headers
target_include_directories(mylib PUBLIC ${CMAKE_SOURCE_DIR}/include)
```

### 🛠 Output:

* `libmylib.so` (Linux)
* `mylib.dll` + `mylib.lib` (Windows)
* `libmylib.dylib` (macOS)

---

## 📌 Notes on Static vs Shared

| Feature         | Static (`STATIC`)      | Shared (`SHARED`)                    |
| --------------- | ---------------------- | ------------------------------------ |
| File name       | `.a`, `.lib`           | `.so`, `.dll`, `.dylib`              |
| Link Time       | Compile/link time      | Compile-time + Load at runtime       |
| Portability     | Easy (no runtime deps) | Must ship `.so` / `.dll` with binary |
| Binary Size     | Larger                 | Smaller                              |
| Runtime speed   | Faster                 | Slight overhead                      |
| Hotfix possible | ❌                      | ✅ Replace `.so` without recompiling  |

---

## ✅ Final Example: Executable that Links Your Library

```
MyProject/
├── CMakeLists.txt
├── include/
│   └── mylib.h
├── src/
│   └── mylib.c
├── app/
│   ├── CMakeLists.txt
│   └── main.c
```

### 🔧 Root `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES C)

add_subdirectory(src)
add_subdirectory(app)
```

### 🔧 `src/CMakeLists.txt`

```cmake
file(GLOB SRC src/*.c)
add_library(mylib STATIC ${SRC})  # or SHARED

target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
```

### 🔧 `app/CMakeLists.txt`

```cmake
add_executable(my_app main.c)
target_include_directories(my_app PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(my_app PRIVATE mylib)
```

---


Here is a **complete, structured, and detailed guide** on:

---

## 🔁 **CMake Command Order**

**Which commands must go before/after others**, with reasons, common practices, and variations.

Also includes **real project structure best practices** with modular or monolithic builds.

---

# ✅ BASIC STRUCTURE & ORDERING

### 🧾 A typical `CMakeLists.txt` should follow this order:

```
1. cmake_minimum_required
2. project
3. global settings (C/C++ standards, output dirs)
4. include custom modules (optional)
5. find_package (external dependencies)
6. include_directories / link_directories (legacy - avoid)
7. add_subdirectory / add_library / add_executable
8. target_include_directories
9. target_compile_definitions / target_compile_options
10. target_link_libraries
11. install() / testing() / custom_commands()
```

---

### 🔁 ORDER MATTERS: EXPLANATION

| Step | Command(s)                                                       | Must Come **Before**                    | Reason                                               |
| ---- | ---------------------------------------------------------------- | --------------------------------------- | ---------------------------------------------------- |
| 1    | `cmake_minimum_required()`                                       | Everything                              | Initializes CMake version requirements               |
| 2    | `project(...)`                                                   | Everything else                         | Sets project-wide settings and variables             |
| 3    | `set(CMAKE_C_STANDARD ...)`<br>`set(CMAKE_OUTPUT_DIRECTORY ...)` | `add_library` or `add_executable`       | These affect how and where targets are compiled      |
| 4    | `include(...)` (custom cmake scripts)                            | before target defs                      | Needed to load functions/macros                      |
| 5    | `find_package(...)`                                              | before linking targets                  | External libraries must be found before use          |
| 6    | `add_subdirectory(...)`                                          | before defining dependencies on subdirs | Adds child CMakeLists to parse                       |
| 7    | `add_library(...)`, `add_executable(...)`                        | before configuring them                 | You can't configure a target that doesn't exist      |
| 8    | `target_include_directories(...)`                                | after target is added                   | Target must exist first                              |
| 9    | `target_compile_definitions`, `target_compile_options`           | after target is added                   | Same reason as above                                 |
| 10   | `target_link_libraries(...)`                                     | after library/executable is created     | You must link to an existing target                  |
| 11   | `install(...)`, `enable_testing()`                               | end                                     | For packaging or testing the already defined targets |

---

# 🔧 PRACTICAL EXAMPLES

---

## 🧩 Minimal Static Library Project

```cmake
cmake_minimum_required(VERSION 3.16)  # [1]
project(MyLibProject LANGUAGES C)     # [2]

set(CMAKE_C_STANDARD 99)              # [3]
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB SRC_FILES src/*.c)          # [7] (prepare file list)

add_library(mylib STATIC ${SRC_FILES})       # [7] (define target)

target_include_directories(mylib              # [8]
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(mylib PRIVATE USE_FAST)  # [9]

# Optional install step
install(TARGETS mylib DESTINATION lib)        # [11]
install(DIRECTORY include/ DESTINATION include)
```

---

## 🧩 Application Using Static Lib

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyApp LANGUAGES C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(lib_mylib)           # [6] Must be before linking to mylib

add_executable(my_app main.c)         # [7]

target_include_directories(my_app PRIVATE ${CMAKE_SOURCE_DIR}/include)  # [8]
target_link_libraries(my_app PRIVATE mylib)                              # [10]
```

---

## 🔁 Correct vs Incorrect Orders

### ❌ Wrong

```cmake
target_link_libraries(my_app PRIVATE mylib)
add_executable(my_app main.c)
```

This fails because you try to link a target **before defining it**.

---

### ✅ Correct

```cmake
add_executable(my_app main.c)
target_link_libraries(my_app PRIVATE mylib)
```

---

# 🛠️ ADVANCED CASES

---

## 📦 External Library with `find_package()`

```cmake
# Must go before target_link_libraries
find_package(OpenSSL REQUIRED)

add_executable(secure_app main.c)

target_link_libraries(secure_app PRIVATE OpenSSL::SSL OpenSSL::Crypto)
```

---

## 🧪 Adding Unit Tests

```cmake
enable_testing()

add_executable(test_foo test/test_foo.c)
target_link_libraries(test_foo PRIVATE mylib)

add_test(NAME FooTest COMMAND test_foo)
```

---

# 📌 Best Practices Summary Table

| Practice                                                                  | Recommended? | Reason                                            |
| ------------------------------------------------------------------------- | ------------ | ------------------------------------------------- |
| Use `target_*` over global `include_directories`, `link_libraries`        | ✅            | Modern CMake is target-centric                    |
| Always define targets before configuring them                             | ✅            | Ensures CMake does not error                      |
| Use `add_subdirectory()` for your own code, `find_package()` for external | ✅            | Clean dependency control                          |
| Keep external libs in `lib/` or use package managers like `vcpkg`/`conan` | ✅            | Easier to maintain and build                      |
| Avoid `link_directories()` unless absolutely necessary                    | ❌            | Can lead to ambiguous behavior                    |
| Avoid `file(GLOB ...)` for production builds                              | ⚠️           | May not catch renamed files in incremental builds |

---

# 🧠 Final Advice

* **Think of CMake as target-oriented and layered**
* Always **define targets**, then **configure them**, then **link them**
* Order is not just syntactic—it affects the dependency graph
* **Modularize**: split large projects using `add_subdirectory()` and per-folder `CMakeLists.txt`

---
