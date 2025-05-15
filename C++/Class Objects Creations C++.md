Here are different ways to create an object using a class with parameters in C++:

### 1. Using Constructor with Parameters
```cpp
class MyClass {
public:
    int a;
    MyClass(int x) : a(x) {} // Constructor with parameter
};

int main() {
    MyClass obj(10); // Creating object using constructor with parameter
    return 0;
}
```

### 2. Using Default Constructor and Setter Method
```cpp
class MyClass {
public:
    int a;
    MyClass() : a(0) {} // Default constructor
    void setA(int x) { a = x; } // Setter method
};

int main() {
    MyClass obj; // Creating object using default constructor
    obj.setA(10); // Setting value using setter method
    return 0;
}
```

### 3. Using a Static Factory Method
```cpp
class MyClass {
public:
    int a;
    MyClass(int x) : a(x) {} // Private constructor

    static MyClass create(int x) { // Static factory method
        return MyClass(x);
    }
};

int main() {
    MyClass obj = MyClass::create(10); // Creating object using static factory method
    return 0;
}
```

### 4. Using Aggregate Initialization (if no constructors are defined)
```cpp
struct MyClass {
    int a;
};

int main() {
    MyClass obj = {10}; // Creating object using aggregate initialization
    return 0;
}
```

### 5. Using `new` Operator
```cpp
class MyClass {
public:
    int a;
    MyClass(int x) : a(x) {} // Constructor with parameter
};

int main() {
    MyClass* obj = new MyClass(10); // Creating object using new operator
    delete obj; // Don't forget to delete the object to avoid memory leak
    return 0;
}
```

### Explanation with Example
Let's take the first method as an example:

```cpp
class MyClass {
public:
    int a;
    MyClass(int x) : a(x) {} // Constructor with parameter
};

int main() {
    MyClass obj(10); // Creating object using constructor with parameter
    std::cout << obj.a; // Output: 10
    return 0;
}
```
In this example, `MyClass` has a constructor that takes an integer parameter. When creating an object `obj` of `MyClass`, we pass `10` to the constructor, initializing the member variable `a` with `10`.