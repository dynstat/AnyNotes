# Basic Data Structures in C++

## Arrays

### Static Arrays

#### Description
Fixed-size sequential collection of elements of the same type stored in contiguous memory locations.

#### Syntax
```cpp
data_type array_name[array_size];
```

#### Parameters
- `data_type`: Type of data to be stored (int, float, char, etc.)
- `array_name`: Identifier for the array
- `array_size`: Number of elements (constant expression)

#### Access and Operations
- **Element Access**: `array_name[index]` (0-based indexing)
- **Size Calculation**: `sizeof(array_name) / sizeof(data_type)`

#### Example
```cpp
#include <iostream>

int main() {
    // Declaration and initialization
    int numbers[5] = {10, 20, 30, 40, 50};
    
    // Element access
    std::cout << "Third element: " << numbers[2] << std::endl;  // Outputs: 30
    
    // Modifying elements
    numbers[1] = 25;
    
    // Iterating through array
    for (int i = 0; i < 5; i++) {
        std::cout << numbers[i] << " ";  // Outputs: 10 25 30 40 50
    }
    std::cout << std::endl;
    
    // Size calculation
    int size = sizeof(numbers) / sizeof(int);
    std::cout << "Array size: " << size << std::endl;  // Outputs: 5
    
    return 0;
}
```

#### Limitations
- Fixed size determined at compile time
- Cannot be resized
- No built-in bounds checking

### Dynamic Arrays

#### Description
Runtime-allocated arrays that can be sized based on program input.

#### Syntax
```cpp
data_type* array_name = new data_type[size];
delete[] array_name;  // Deallocation
```

#### Parameters
- `data_type`: Type of data to be stored
- `array_name`: Pointer to the first element
- `size`: Number of elements (can be a variable)

#### Example
```cpp
#include <iostream>

int main() {
    int size;
    std::cout << "Enter array size: ";
    std::cin >> size;
    
    // Dynamic allocation
    int* dynamicArray = new int[size];
    
    // Initialization
    for (int i = 0; i < size; i++) {
        dynamicArray[i] = i * 10;
    }
    
    // Access and print
    for (int i = 0; i < size; i++) {
        std::cout << dynamicArray[i] << " ";
    }
    std::cout << std::endl;
    
    // Deallocation (memory management)
    delete[] dynamicArray;
    
    return 0;
}
```

#### Key Points
- Allocated on the heap (free store)
- Requires manual memory management
- Size can be determined at runtime
- Pointer arithmetic applies

## C-Style Strings

#### Description
Character arrays terminated by the null character `'\0'`.

#### Syntax
```cpp
char string_name[size];
```

#### Examples
```cpp
#include <iostream>
#include <cstring>  // For string functions

int main() {
    // Declaration and initialization
    char greeting[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
    char simplified[] = "Hello";  // Compiler adds '\0'
    
    // String length
    std::cout << "Length: " << strlen(simplified) << std::endl;  // Outputs: 5
    
    // String copy
    char destination[20];
    strcpy(destination, simplified);
    std::cout << "Copied: " << destination << std::endl;
    
    // String concatenation
    char combined[20] = "Hello, ";
    strcat(combined, "World!");
    std::cout << "Combined: " << combined << std::endl;
    
    // String comparison
    if (strcmp("apple", "banana") < 0) {
        std::cout << "apple comes before banana lexicographically" << std::endl;
    }
    
    return 0;
}
```

#### Common C-String Functions
| Function | Description | Parameters | Return Type |
|----------|-------------|------------|-------------|
| `strlen(str)` | Gets string length | const char* str | size_t |
| `strcpy(dest, src)` | Copies string | char* dest, const char* src | char* |
| `strcat(dest, src)` | Concatenates strings | char* dest, const char* src | char* |
| `strcmp(str1, str2)` | Compares strings | const char* str1, const char* str2 | int |
| `strchr(str, ch)` | Finds character | const char* str, int ch | char* |
| `strstr(str, substr)` | Finds substring | const char* str, const char* substr | char* |

#### Limitations
- Fixed size
- Manual memory management
- Prone to buffer overflows
- No range checking

## Structures

#### Description
User-defined data type that groups related variables of different data types.

#### Syntax
```cpp
struct structure_name {
    data_type1 member1;
    data_type2 member2;
    // ...
};
```

#### Member Access
- Dot operator (`.`) for structure variables
- Arrow operator (`->`) for structure pointers

#### Example
```cpp
#include <iostream>
#include <string>

struct Student {
    int id;
    std::string name;
    float gpa;
    
    // Structures can also contain member functions
    void display() {
        std::cout << "ID: " << id << ", Name: " << name << ", GPA: " << gpa << std::endl;
    }
};

int main() {
    // Declaration and initialization
    Student s1 = {1001, "Alice", 3.8};
    
    // Member access using dot operator
    std::cout << "Student Name: " << s1.name << std::endl;
    
    // Modifying members
    s1.gpa = 3.9;
    
    // Using member function
    s1.display();
    
    // Using structure pointer
    Student* ptr = &s1;
    ptr->id = 1002;  // Arrow operator for pointer access
    ptr->display();
    
    // Array of structures
    Student class_list[3] = {
        {1001, "Alice", 3.9},
        {1002, "Bob", 3.5},
        {1003, "Charlie", 3.7}
    };
    
    for (int i = 0; i < 3; i++) {
        class_list[i].display();
    }
    
    return 0;
}
```

#### Key Points
- Can contain members of different types
- Can include member functions
- Used as building blocks for more complex data structures
- Basis for object-oriented programming in C++

## Unions

#### Description
Special data type that allows storing different data types in the same memory location.

#### Syntax
```cpp
union union_name {
    data_type1 member1;
    data_type2 member2;
    // ...
};
```

#### Example
```cpp
#include <iostream>

union Value {
    int integer;
    float floating;
    char character;
};

int main() {
    Value v;
    
    v.integer = 42;
    std::cout << "Integer: " << v.integer << std::endl;
    
    v.floating = 3.14f;  // This overwrites the integer value
    std::cout << "Float: " << v.floating << std::endl;
    std::cout << "Integer is now: " << v.integer << std::endl;  // Garbage value
    
    v.character = 'A';  // This overwrites the float value
    std::cout << "Character: " << v.character << std::endl;
    
    // Size of union is the size of its largest member
    std::cout << "Size of union: " << sizeof(v) << " bytes" << std::endl;
    
    return 0;
}
```

#### Key Points
- All members share the same memory location
- Only one member can hold a value at a time
- Size equals the size of the largest member
- Useful for memory-constrained environments
- No type safety between members