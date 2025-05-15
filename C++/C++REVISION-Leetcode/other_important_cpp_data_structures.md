# Other Important C++ Data Structures

This section covers important C++ data structures and utilities that don't fall into the categories of containers or algorithms but are essential parts of modern C++ programming.

## Smart Pointers (`<memory>`)

Smart pointers are wrapper classes that manage the lifetime of dynamically allocated objects, helping prevent memory leaks and other resource management issues.

### `std::unique_ptr`

A smart pointer that owns and manages another object through a pointer and disposes of that object when the unique_ptr goes out of scope.

#### Template Parameters
```cpp
template<
    class T,
    class Deleter = std::default_delete<T>
> class unique_ptr;
```
- `T`: Type of the managed object
- `Deleter`: Type of the deleter used to destroy the object

#### Common Methods

| Method | Description | Parameters | Return Type | Complexity |
|--------|-------------|------------|-------------|------------|
| `(constructor)` | Constructs a unique_ptr | Various | N/A | O(1) |
| `(destructor)` | Destroys the managed object | None | N/A | O(1) |
| `operator=` | Assigns to the unique_ptr | unique_ptr or nullptr | unique_ptr& | O(1) |
| `release()` | Releases ownership of the managed object | None | T* | O(1) |
| `reset()` | Replaces the managed object | pointer (optional) | void | O(1) |
| `swap()` | Swaps the managed objects | unique_ptr& | void | O(1) |
| `get()` | Returns a pointer to the managed object | None | T* | O(1) |
| `operator*` | Dereferences the stored pointer | None | T& | O(1) |
| `operator->` | Dereferences the stored pointer for member access | None | T* | O(1) |
| `operator bool` | Checks if the unique_ptr owns an object | None | bool | O(1) |
| `make_unique()` (C++14) | Creates a unique_ptr with a new object | constructor args | unique_ptr<T> | O(1) |

#### Example
```cpp
#include <iostream>
#include <memory>
#include <string>

class Resource {
private:
    std::string name;
    
public:
    Resource(const std::string& n) : name(n) {
        std::cout << "Resource " << name << " acquired\n";
    }
    
    void use() const {
        std::cout << "Using resource " << name << "\n";
    }
    
    ~Resource() {
        std::cout << "Resource " << name << " released\n";
    }
};

void useResource(const Resource* r) {
    if (r) {
        r->use();
    }
}

int main() {
    // Creating a unique_ptr
    std::unique_ptr<Resource> res1(new Resource("First"));
    
    // Using the managed object
    res1->use();
    
    // Passing the raw pointer to a function
    useResource(res1.get());
    
    // Creating with make_unique (C++14)
    auto res2 = std::make_unique<Resource>("Second");
    
    // Moving ownership
    std::unique_ptr<Resource> res3 = std::move(res1);
    
    // res1 is now nullptr
    if (!res1) {
        std::cout << "res1 is nullptr after move\n";
    }
    
    // Release ownership (you become responsible for deletion)
    Resource* raw = res3.release();
    std::cout << "Released from unique_ptr\n";
    
    // Manual deletion required after release
    delete raw;
    
    // Reset with a new object
    res1.reset(new Resource("Fourth"));
    
    // Automatic cleanup at end of scope
    return 0;
}
```

#### Key Characteristics
- Exclusive ownership - can't be copied, only moved
- Automatically deletes the object when going out of scope
- Can use custom deleters for specialized resource management
- Zero overhead compared to raw pointers when using default deleter
- Safe for use in STL containers and algorithms after C++11

### `std::shared_ptr`

A smart pointer that retains shared ownership of an object through a pointer. Several shared_ptr objects may own the same object, and the object is destroyed when the last shared_ptr owning it is destroyed.

#### Template Parameters
```cpp
template<
    class T
> class shared_ptr;
```
- `T`: Type of the managed object

#### Common Methods

| Method | Description | Parameters | Return Type | Complexity |
|--------|-------------|------------|-------------|------------|
| `(constructor)` | Constructs a shared_ptr | Various | N/A | O(1) |
| `(destructor)` | Decrements reference count, destroys object if count is 0 | None | N/A | O(1) |
| `operator=` | Assigns to the shared_ptr | shared_ptr, weak_ptr, etc. | shared_ptr& | O(1) |
| `reset()` | Replaces the managed object | pointer (optional) | void | O(1) |
| `swap()` | Swaps the managed objects | shared_ptr& | void | O(1) |
| `get()` | Returns a pointer to the managed object | None | T* | O(1) |
| `operator*` | Dereferences the stored pointer | None | T& | O(1) |
| `operator->` | Dereferences the stored pointer for member access | None | T* | O(1) |
| `operator bool` | Checks if the shared_ptr owns an object | None | bool | O(1) |
| `use_count()` | Returns the number of shared_ptr objects managing the object | None | long | O(1) |
| `unique()` | Checks if the shared_ptr is the only shared_ptr managing the object | None | bool | O(1) |
| `make_shared()` | Creates a shared_ptr with a new object | constructor args | shared_ptr<T> | O(1) |

#### Example
```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <string>

class Resource {
private:
    std::string name;
    
public:
    Resource(const std::string& n) : name(n) {
        std::cout << "Resource " << name << " acquired\n";
    }
    
    void use() const {
        std::cout << "Using resource " << name << "\n";
    }
    
    ~Resource() {
        std::cout << "Resource " << name << " released\n";
    }
};

void processResource(std::shared_ptr<Resource> res) {
    std::cout << "Reference count in function: " << res.use_count() << "\n";
    if (res) {
        res->use();
    }
}

int main() {
    // Creating a shared_ptr
    std::shared_ptr<Resource> res1(new Resource("First"));
    
    // Reference count is 1
    std::cout << "Reference count: " << res1.use_count() << "\n";
    
    // Creating a copy increases reference count
    std::shared_ptr<Resource> res2 = res1;
    std::cout << "Reference count after copy: " << res1.use_count() << "\n";
    
    // Creating with make_shared (more efficient)
    auto res3 = std::make_shared<Resource>("Second");
    
    // Storing in a container
    std::vector<std::shared_ptr<Resource>> resources;
    resources.push_back(res1);
    resources.push_back(res3);
    
    std::cout << "Reference count of res1 after pushing to vector: " << res1.use_count() << "\n";
    
    // Passing to a function
    processResource(res1);
    
    // Reset one reference
    res1.reset();
    std::cout << "Reference count of res2 after res1.reset(): " << res2.use_count() << "\n";
    
    // Reset all references to release the resource
    res2.reset();
    resources.clear();
    
    // Create a new shared_ptr
    auto res4 = std::make_shared<Resource>("Fourth");
    
    // Automatic cleanup when all shared_ptrs go out of scope
    return 0;
}
```

#### Key Characteristics
- Shared ownership - can be copied freely
- Reference counting overhead
- Thread-safe reference count (but not thread-safe object access)
- Automatic deletion when the last reference is gone
- Prefer `make_shared` for better efficiency (single allocation for control block and object)
- Can cause reference cycles if not careful

### `std::weak_ptr`

A smart pointer that holds a non-owning ("weak") reference to an object that is managed by a `std::shared_ptr`. It must be converted to a `shared_ptr` to access the referenced object.

#### Template Parameters
```cpp
template<
    class T
> class weak_ptr;
```
- `T`: Type of the referenced object

#### Common Methods

| Method | Description | Parameters | Return Type | Complexity |
|--------|-------------|------------|-------------|------------|
| `(constructor)` | Constructs a weak_ptr | Various | N/A | O(1) |
| `(destructor)` | Destroys the weak_ptr | None | N/A | O(1) |
| `operator=` | Assigns to the weak_ptr | weak_ptr, shared_ptr | weak_ptr& | O(1) |
| `reset()` | Releases the reference | None | void | O(1) |
| `swap()` | Swaps the referenced objects | weak_ptr& | void | O(1) |
| `use_count()` | Returns the number of shared_ptr objects referencing the object | None | long | O(1) |
| `expired()` | Checks if the referenced object has been deleted | None | bool | O(1) |
| `lock()` | Creates a shared_ptr that manages the referenced object | None | shared_ptr<T> | O(1) |

#### Example
```cpp
#include <iostream>
#include <memory>
#include <string>

class Person {
public:
    std::string name;
    std::shared_ptr<Person> spouse;  // Would create a reference cycle
    std::weak_ptr<Person> weakSpouse;  // Solution to avoid reference cycle
    
    Person(const std::string& n) : name(n) {
        std::cout << "Person " << name << " created\n";
    }
    
    ~Person() {
        std::cout << "Person " << name << " destroyed\n";
    }
};

int main() {
    // Create shared_ptrs to two Person objects
    auto alice = std::make_shared<Person>("Alice");
    auto bob = std::make_shared<Person>("Bob");
    
    // Create a reference cycle (this would cause a memory leak)
    // alice->spouse = bob;
    // bob->spouse = alice;
    
    // Instead, use weak_ptr to avoid the cycle
    alice->weakSpouse = bob;
    bob->weakSpouse = alice;
    
    std::cout << "Reference count for alice: " << alice.use_count() << "\n";
    std::cout << "Reference count for bob: " << bob.use_count() << "\n";
    
    // Using weak_ptr:
    // Check if the referenced object exists
    if (!alice->weakSpouse.expired()) {
        // Convert to shared_ptr to access the object
        auto spouse = alice->weakSpouse.lock();
        if (spouse) {
            std::cout << alice->name << "'s spouse is " << spouse->name << "\n";
        }
    }
    
    // Demonstrate what happens when the shared_ptr is reset
    bob.reset();
    
    std::cout << "After bob.reset()\n";
    std::cout << "Reference count for alice: " << alice.use_count() << "\n";
    
    // Try to access bob through alice's weakSpouse
    if (alice->weakSpouse.expired()) {
        std::cout << "Alice's spouse pointer has expired\n";
    } else {
        auto spouse = alice->weakSpouse.lock();
        if (spouse) {
            std::cout << "This should not happen: " << spouse->name << " still exists\n";
        } else {
            std::cout << "Lock returns nullptr because the object is gone\n";
        }
    }
    
    return 0;
}
```

#### Key Characteristics
- Non-owning reference to an object managed by shared_ptr
- Does not affect the reference count
- Must be converted to shared_ptr to access the object
- Helps break reference cycles in shared_ptr
- Useful for caching and observer pattern implementations

## Pair and Tuple

### `std::pair` (`<utility>`)

A simple container for two heterogeneous objects.

#### Template Parameters
```cpp
template<
    class T1,
    class T2
> struct pair;
```
- `T1`: Type of the first element
- `T2`: Type of the second element

#### Members
- `first`: First element
- `second`: Second element

#### Common Functions

| Function | Description | Parameters | Return Type | Complexity |
|----------|-------------|------------|-------------|------------|
| `(constructor)` | Constructs a pair | Various | N/A | O(1) |
| `operator=` | Assigns to the pair | pair | pair& | O(1) |
| `swap()` | Swaps the elements | pair& | void | O(1) |
| `make_pair()` | Creates a pair object | Two values | pair | O(1) |

#### Example
```cpp
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <map>

int main() {
    // Creating a pair
    std::pair<std::string, int> person1("Alice", 30);
    
    // Accessing elements
    std::cout << "Name: " << person1.first << ", Age: " << person1.second << "\n";
    
    // Using make_pair
    auto person2 = std::make_pair("Bob", 25);
    
    // Type inference with auto
    std::cout << "Name: " << person2.first << ", Age: " << person2.second << "\n";
    
    // Using pair in a vector
    std::vector<std::pair<std::string, int>> people;
    people.push_back(person1);
    people.push_back(person2);
    people.emplace_back("Charlie", 35);  // In-place construction
    
    // Iterating over pairs
    for (const auto& person : people) {
        std::cout << person.first << " is " << person.second << " years old\n";
    }
    
    // Structured bindings (C++17)
    for (const auto& [name, age] : people) {
        std::cout << name << " is " << age << " years old (using structured binding)\n";
    }
    
    // Pairs are returned by map operations
    std::map<std::string, int> ages;
    
    // insert() returns a pair<iterator, bool>
    auto result = ages.insert({"Alice", 30});
    std::cout << "Insert successful? " << (result.second ? "Yes" : "No") << "\n";
    
    // Using tie (pre-C++17)
    std::string name;
    int age;
    std::tie(name, age) = person1;
    std::cout << "Unpacked with tie: " << name << ", " << age << "\n";
    
    return 0;
}
```

### `std::tuple` (`<tuple>`)

A fixed-size collection of heterogeneous values.

#### Template Parameters
```cpp
template<
    class... Types
> class tuple;
```
- `Types...`: Types of the elements

#### Common Functions

| Function | Description | Parameters | Return Type | Complexity |
|----------|-------------|------------|-------------|------------|
| `(constructor)` | Constructs a tuple | Various | N/A | O(1) |
| `operator=` | Assigns to the tuple | tuple | tuple& | O(1) |
| `swap()` | Swaps the elements | tuple& | void | O(1) |
| `make_tuple()` | Creates a tuple object | Multiple values | tuple | O(1) |
| `get<I>(tuple)` | Access element by index | tuple | Type& | O(1) |
| `std::tuple_size<tuple>` | Get number of elements | N/A | size_t | O(1) |
| `std::tuple_element<I, tuple>` | Get type of element I | N/A | Type | O(1) |

#### Example
```cpp
#include <iostream>
#include <tuple>
#include <string>
#include <vector>

// Function returning a tuple
std::tuple<std::string, int, double> getPerson() {
    return {"Alice", 30, 62.5};
}

int main() {
    // Creating a tuple
    std::tuple<std::string, int, double> person1("Alice", 30, 62.5);
    
    // Using make_tuple
    auto person2 = std::make_tuple("Bob", 25, 70.0);
    
    // Accessing elements with get<I>
    std::cout << "Name: " << std::get<0>(person1) << "\n";
    std::cout << "Age: " << std::get<1>(person1) << "\n";
    std::cout << "Weight: " << std::get<2>(person1) << "\n";
    
    // Getting the tuple size at compile time
    std::cout << "Tuple size: " << std::tuple_size<decltype(person1)>::value << "\n";
    
    // Unpacking a tuple with tie (pre-C++17)
    std::string name;
    int age;
    double weight;
    std::tie(name, age, weight) = person2;
    std::cout << "Unpacked: " << name << ", " << age << ", " << weight << "\n";
    
    // Unpacking with structured bindings (C++17)
    auto [name2, age2, weight2] = getPerson();
    std::cout << "Structured binding: " << name2 << ", " << age2 << ", " << weight2 << "\n";
    
    // Ignoring elements with std::ignore
    std::string name3;
    double weight3;
    std::tie(name3, std::ignore, weight3) = person1;
    std::cout << "Partial unpack: " << name3 << ", " << weight3 << "\n";
    
    // Vector of tuples
    std::vector<std::tuple<std::string, int, std::string>> employees;
    employees.emplace_back("Alice", 30, "Engineering");
    employees.emplace_back("Bob", 25, "Marketing");
    
    // Iterating with structured bindings
    for (const auto& [name, age, department] : employees) {
        std::cout << name << " (" << age << ") works in " << department << "\n";
    }
    
    return 0;
}
```

## Optional, Variant, and Any (C++17)

### `std::optional` (`<optional>`)

A container for an optional value, either containing a value or not.

#### Template Parameters
```cpp
template<
    class T
> class optional;
```
- `T`: Type of the contained value

#### Common Methods

| Method | Description | Parameters | Return Type | Complexity |
|--------|-------------|------------|-------------|------------|
| `(constructor)` | Constructs an optional object | Various | N/A | O(1) |
| `(destructor)` | Destroys the optional object | None | N/A | O(1) |
| `operator=` | Assigns to the optional | Various | optional& | O(1) |
| `operator*` | Accesses the contained value | None | T& | O(1) |
| `operator->` | Accesses the contained value for member access | None | T* | O(1) |
| `operator bool` | Checks if the optional contains a value | None | bool | O(1) |
| `has_value()` | Checks if the optional contains a value | None | bool | O(1) |
| `value()` | Access the contained value (throws if empty) | None | T& | O(1) |
| `value_or()` | Returns the contained value or a default | const U& | T | O(1) |
| `reset()` | Destroys the contained value | None | void | O(1) |
| `emplace()` | Constructs a value in-place | constructor args | T& | O(1) |

#### Example
```cpp
#include <iostream>
#include <optional>
#include <string>

// Function that might not return a value
std::optional<std::string> findUserById(int id) {
    if (id == 1) {
        return "Alice";
    } else if (id == 2) {
        return "Bob";
    }
    return std::nullopt;  // No value
}

// Function that uses optional parameters
std::string formatName(const std::string& name, std::optional<std::string> title = std::nullopt) {
    if (title) {
        return *title + " " + name;
    }
    return name;
}

int main() {
    // Creating optionals
    std::optional<int> opt1;  // Empty optional
    std::optional<int> opt2 = 42;  // Optional containing a value
    std::optional<std::string> opt3 = "Hello";
    
    // Checking if optional has a value
    if (opt1.has_value()) {
        std::cout << "opt1 has value: " << *opt1 << "\n";
    } else {
        std::cout << "opt1 has no value\n";
    }
    
    // Using operator bool
    if (opt2) {
        std::cout << "opt2 has value: " << *opt2 << "\n";
    }
    
    // Accessing value with value() (throws std::bad_optional_access if empty)
    try {
        std::cout << "opt3 value: " << opt3.value() << "\n";
        std::cout << "opt1 value: " << opt1.value() << "\n";  // This will throw
    } catch (const std::bad_optional_access& e) {
        std::cout << "Exception caught: " << e.what() << "\n";
    }
    
    // Using value_or for default value
    std::cout << "opt1 value_or: " << opt1.value_or(0) << "\n";
    std::cout << "opt2 value_or: " << opt2.value_or(0) << "\n";
    
    // Function returning optional
    auto user1 = findUserById(1);
    auto user3 = findUserById(3);
    
    std::cout << "User 1: " << (user1 ? *user1 : "Not found") << "\n";
    std::cout << "User 3: " << (user3 ? *user3 : "Not found") << "\n";
    
    // Optional parameters
    std::cout << formatName("Smith") << "\n";  // Just "Smith"
    std::cout << formatName("Smith", "Dr.") << "\n";  // "Dr. Smith"
    
    // Modifying the contained value
    if (user1) {
        *user1 += " (Admin)";
        std::cout << "Modified user 1: " << *user1 << "\n";
    }
    
    // In-place construction
    std::optional<std::pair<int, std::string>> opt_pair;
    opt_pair.emplace(1, "one");
    if (opt_pair) {
        std::cout << "Pair: " << opt_pair->first << ", " << opt_pair->second << "\n";
    }
    
    // Reset to empty state
    opt2.reset();
    std::cout << "After reset, opt2 has value: " << (opt2.has_value() ? "yes" : "no") << "\n";
    
    return 0;
}
```

### `std::variant` (`<variant>`)

A type-safe union that can hold a value of any one of its alternative types.

#### Template Parameters
```cpp
template<
    class... Types
> class variant;
```
- `Types...`: Alternative types the variant can hold

#### Common Methods

| Method | Description | Parameters | Return Type | Complexity |
|--------|-------------|------------|-------------|------------|
| `(constructor)` | Constructs a variant | Various | N/A | O(1) |
| `(destructor)` | Destroys the variant | None | N/A | O(1) |
| `operator=` | Assigns to the variant | Various | variant& | O(1) |
| `index()` | Returns the zero-based index of the active alternative | None | size_t | O(1) |
| `valueless_by_exception()` | Checks if the variant is in an invalid state | None | bool | O(1) |
| `emplace<I>()` | Constructs a value in-place | constructor args | T& | O(1) |
| `get<I>()` | Accesses the value of type I (throws if wrong type) | None | T& | O(1) |
| `get_if<I>()` | Accesses the value of type I (returns nullptr if wrong type) | variant* | T* | O(1) |
| `visit()` | Applies a visitor to the variant | visitor, variant(s) | Return type of visitor | O(1) |
| `holds_alternative<T>()` | Checks if the variant holds a value of type T | None | bool | O(1) |

#### Example
```cpp
#include <iostream>
#include <variant>
#include <string>
#include <vector>

// Visitor using overloaded pattern (C++17)
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;  // Deduction guide

int main() {
    // Creating variants
    std::variant<int, std::string, double> var1;  // Holds int by default
    std::variant<int, std::string> var2 = "Hello";  // Holds string
    std::variant<int, std::string> var3 = 42;  // Holds int
    
    // Checking which type is active
    std::cout << "var1 index: " << var1.index() << "\n";  // 0 (int)
    std::cout << "var2 index: " << var2.index() << "\n";  // 1 (string)
    
    // Checking if a specific type is active
    std::cout << "var2 holds string: " << std::holds_alternative<std::string>(var2) << "\n";
    std::cout << "var3 holds string: " << std::holds_alternative<std::string>(var3) << "\n";
    
    // Accessing value with get (throws std::bad_variant_access if wrong type)
    try {
        std::cout << "var3 as int: " << std::get<int>(var3) << "\n";
        std::cout << "var3 as string: " << std::get<std::string>(var3) << "\n";  // This will throw
    } catch (const std::bad_variant_access& e) {
        std::cout << "Exception caught: " << e.what() << "\n";
    }
    
    // Using get_if (safer, returns nullptr if wrong type)
    if (auto pval = std::get_if<int>(&var3)) {
        std::cout << "var3 contains int: " << *pval << "\n";
    } else if (auto pval = std::get_if<std::string>(&var3)) {
        std::cout << "var3 contains string: " << *pval << "\n";
    }
    
    // Changing the value/type
    var3 = "Now I'm a string";
    std::cout << "After assignment, var3 holds string: " 
              << std::holds_alternative<std::string>(var3) << "\n";
    
    // In-place construction
    var1.emplace<2>(3.14);  // Activate the double alternative
    
    // Visiting a variant
    std::visit([](const auto& val) {
        std::cout << "Value: " << val << "\n";
    }, var1);
    
    // Vector of variants
    std::vector<std::variant<int, std::string, double>> vars;
    vars.push_back(42);
    vars.push_back("Hello");
    vars.push_back(3.14);
    
    // Visit with multiple overloads
    for (const auto& v : vars) {
        std::visit(overloaded {
            [](int i) { std::cout << "Int: " << i << "\n"; },
            [](const std::string& s) { std::cout << "String: " << s << "\n"; },
            [](double d) { std::cout << "Double: " << d << "\n"; }
        }, v);
    }
    
    return 0;
}
```

### `std::any` (`<any>`)

A type-safe container that can hold a value of any copyable type.

#### Common Methods

| Method | Description | Parameters | Return Type | Complexity |
|--------|-------------|------------|-------------|------------|
| `(constructor)` | Constructs an any object | Various | N/A | O(1) |
| `(destructor)` | Destroys the any object | None | N/A | O(1) |
| `operator=` | Assigns to the any | Various | any& | O(1) |
| `emplace<T>()` | Constructs a value in-place | constructor args | T& | O(1) |
| `reset()` | Destroys the contained object | None | void | O(1) |
| `has_value()` | Checks if the any contains a value | None | bool | O(1) |
| `type()` | Returns the type info of the contained value | None | std::type_info | O(1) |
| `any_cast<T>()` | Casts the contained value to type T | any | T | O(1) |

#### Example
```cpp
#include <iostream>
#include <any>
#include <string>
#include <vector>
#include <typeinfo>

// Function that can take any type of value
void processValue(const std::any& value) {
    if (!value.has_value()) {
        std::cout << "Empty value\n";
        return;
    }
    
    try {
        if (value.type() == typeid(int)) {
            std::cout << "Int value: " << std::any_cast<int>(value) << "\n";
        } else if (value.type() == typeid(std::string)) {
            std::cout << "String value: " << std::any_cast<std::string>(value) << "\n";
        } else if (value.type() == typeid(double)) {
            std::cout << "Double value: " << std::any_cast<double>(value) << "\n";
        } else {
            std::cout << "Unknown type: " << value.type().name() << "\n";
        }
    } catch (const std::bad_any_cast& e) {
        std::cout << "Bad cast: " << e.what() << "\n";
    }
}

int main() {
    // Creating any objects
    std::any a1;  // Empty
    std::any a2 = 42;  // Holds int
    std::any a3 = std::string("Hello");  // Holds string
    
    // Checking if any has a value
    std::cout << "a1 has value: " << a1.has_value() << "\n";
    std::cout << "a2 has value: " << a2.has_value() << "\n";
    
    // Accessing value with any_cast (throws std::bad_any_cast if wrong type)
    try {
        std::cout << "a2 as int: " << std::any_cast<int>(a2) << "\n";
        std::cout << "a2 as string: " << std::any_cast<std::string>(a2) << "\n";  // This will throw
    } catch (const std::bad_any_cast& e) {
        std::cout << "Exception caught: " << e.what() << "\n";
    }
    
    // Safer access with pointer version of any_cast
    if (int* pval = std::any_cast<int>(&a2)) {
        std::cout << "a2 contains int: " << *pval << "\n";
    } else {
        std::cout << "a2 does not contain int\n";
    }
    
    // Getting the type info
    std::cout << "a3 type: " << a3.type().name() << "\n";
    
    // Changing the value/type
    a2 = 3.14;  // Now holds double
    std::cout << "After assignment, a2 type: " << a2.type().name() << "\n";
    
    // In-place construction
    a1.emplace<std::vector<int>>({1, 2, 3, 4, 5});
    
    // Reset to empty state
    a3.reset();
    std::cout << "After reset, a3 has value: " << a3.has_value() << "\n";
    
    // Using any with a function
    processValue(a1);
    processValue(a2);
    processValue(a3);  // Empty
    processValue(std::vector<double>{1.1, 2.2, 3.3});  // Unknown type
    
    // Vector of any
    std::vector<std::any> values = {42, std::string("Hello"), 3.14, std::vector<int>{1, 2, 3}};
    
    for (const auto& val : values) {
        processValue(val);
    }
    
    return 0;
}
```

## String View (C++17)

### `std::string_view` (`<string_view>`)

A non-owning reference to a string or a substring.

#### Common Methods

| Method | Description | Parameters | Return Type | Complexity |
|--------|-------------|------------|-------------|------------|
| `(constructor)` | Constructs a string_view | Various | N/A | O(1) |
| `operator=` | Assigns to the string_view | string_view | string_view& | O(1) |
| `operator[]` | Access character at position | size_t | char | O(1) |
| `at()` | Access character with bounds checking | size_t | char | O(1) |
| `front()` | Access first character | None | char | O(1) |
| `back()` | Access last character | None | char | O(1) |
| `data()` | Get pointer to data | None | const char* | O(1) |
| `size()`, `length()` | Get the size | None | size_t | O(1) |
| `empty()` | Check if empty | None | bool | O(1) |
| `substr()` | Create a substring view | pos, count | string_view | O(1) |
| `compare()` | Compare with another string_view | Various | int | O(n) |
| `find()` | Find substring | Various | size_t | O(n) |
| `rfind()` | Find substring from end | Various | size_t | O(n) |
| `remove_prefix()` | Remove characters from beginning | count | void | O(1) |
| `remove_suffix()` | Remove characters from end | count | void | O(1) |

#### Example
```cpp
#include <iostream>
#include <string_view>
#include <string>

// Function that takes a string_view instead of a string reference
void analyzeString(std::string_view sv) {
    std::cout << "String: " << sv << "\n";
    std::cout << "Length: " << sv.length() << "\n";
    if (!sv.empty()) {
        std::cout << "First char: " << sv.front() << "\n";
        std::cout << "Last char: " << sv.back() << "\n";
    }
}

int main() {
    // Creating string_views
    std::string s = "Hello, World!";
    std::string_view sv1 = s;  // View of a std::string
    std::string_view sv2 = "C++ String View";  // View of a string literal
    
    std::cout << "sv1: " << sv1 << "\n";
    std::cout << "sv2: " << sv2 << "\n";
    
    // Creating a substring view (no copy)
    std::string_view sv3 = sv1.substr(0, 5);  // "Hello"
    std::cout << "sv3 (substring): " << sv3 << "\n";
    
    // Modifying the view (not the underlying string)
    sv1.remove_prefix(7);  // Skip "Hello, "
    std::cout << "sv1 after remove_prefix: " << sv1 << "\n";  // "World!"
    
    sv1.remove_suffix(1);  // Remove '!'
    std::cout << "sv1 after remove_suffix: " << sv1 << "\n";  // "World"
    
    // Original string is unchanged
    std::cout << "Original string: " << s << "\n";  // "Hello, World!"
    
    // String operations
    auto pos = sv2.find("String");
    if (pos != std::string_view::npos) {
        std::cout << "'String' found at position: " << pos << "\n";
    }
    
    // Comparing string_views
    std::string_view sv4 = "Hello";
    std::string_view sv5 = "Hello";
    std::cout << "sv4 == sv5: " << (sv4 == sv5 ? "true" : "false") << "\n";
    std::cout << "sv4 < sv1: " << (sv4 < sv1 ? "true" : "false") << "\n";
    
    // Passing to functions
    analyzeString(sv3);  // Pass string_view
    analyzeString("Direct literal");  // Implicit conversion
    analyzeString(s);  // Implicit conversion from std::string
    
    // Accessing individual characters
    for (char c : sv4) {
        std::cout << c << " ";
    }
    std::cout << "\n";
    
    // Caution: Dangling string_view
    std::string_view dangerous;
    {
        std::string temp = "Temporary string";
        dangerous = temp;  // dangerous now points to memory of temp
    }  // temp is destroyed here
    
    // Dangerous! The string_view refers to deallocated memory
    // std::cout << "Dangerous: " << dangerous << "\n";  // Undefined behavior
    
    return 0;
}
```

## Key Characteristics of `string_view`

- Non-owning, lightweight reference to string data
- No memory allocation or copying
- Constant time substring operations
- Compatible with strings, string literals, and character arrays
- Great for function parameters instead of `const std::string&`
- Be careful with lifetime issues - the referenced string must outlive the string_view

## Best Practices for C++ Data Structures

1. **Use smart pointers** instead of raw pointers for memory management
   - Prefer `std::unique_ptr` for exclusive ownership
   - Use `std::shared_ptr` when shared ownership is needed
   - Use `std::weak_ptr` to break reference cycles

2. **Choose the right container** for your use case
   - `std::vector` for most sequential container needs
   - `std::unordered_map/set` for fast lookup by key
   - `std::map/set` when ordered traversal is needed

3. **Use modern C++ features** to improve code safety and readability
   - `std::optional` for functions that may not return a value
   - `std::variant` for type-safe unions
   - `std::string_view` for string arguments that don't need ownership

4. **Avoid unnecessary copying**
   - Pass large objects by const reference
   - Use move semantics for transferring ownership
   - Consider pass-by-value for small, copyable types

5. **Be aware of iterator invalidation** rules
   - Container operations can invalidate iterators
   - Don't use iterators after modifying a container

6. **Use standard algorithms** with containers
   - Prefer standard algorithms to hand-written loops
   - Combine algorithms for more complex operations

7. **Consider memory efficiency**
   - Use reserve() for vectors when size is known
   - Use emplace() instead of push() when constructing elements in-place
   - Be mindful of overhead for containers like std::map