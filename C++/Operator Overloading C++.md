
[[C++ Basics]]


Operator overloading allows you to define custom behavior for operators when used with user-defined types. Here are examples for several common operators:

### 1. Arithmetic Operators (+, -, *, /, %)

Example: Overloading the + operator for a Complex number class

```cpp
class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }
};
```

Breakdown:
```cpp
Complex operator+(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}
```
- `Complex` is the return type of the overloaded operator.
- `operator+` is the syntax for overloading the + operator.
- `(const Complex& other)` is the parameter, representing the right-hand operand.
- `const` at the end indicates that this operation doesn't modify the current object.

### 2. Comparison Operators (==, !=, <, >, <=, >=)

Example: Overloading the == operator

```cpp
class Person {
private:
    std::string name;
    int age;

public:
    Person(const std::string& n, int a) : name(n), age(a) {}

    bool operator==(const Person& other) const {
        return (name == other.name) && (age == other.age);
    }
};
```

Breakdown:
```cpp
bool operator==(const Person& other) const {
    return (name == other.name) && (age == other.age);
}
```
- `bool` is the return type (true if equal, false otherwise).
- `operator==` is the syntax for overloading the == operator.
- `(const Person& other)` is the parameter, representing the right-hand operand.
- `const` at the end indicates that this operation doesn't modify the current object.

### 3. Assignment Operator (=)

Example: Overloading the assignment operator

```cpp
class MyString {
private:
    char* str;

public:
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] str;
            str = new char[strlen(other.str) + 1];
            strcpy(str, other.str);
        }
        return *this;
    }
};
```

Breakdown:
```cpp
MyString& operator=(const MyString& other) {
    // ... implementation ...
    return *this;
}
```
- `MyString&` is the return type (reference to the current object).
- `operator=` is the syntax for overloading the assignment operator.
- `(const MyString& other)` is the parameter, representing the right-hand operand.

### 4. Increment/Decrement Operators (++, --)

Example: Overloading pre-increment and post-increment operators

```cpp
class Counter {
private:
    int count;

public:
    Counter& operator++() {  // Pre-increment
        ++count;
        return *this;
    }

    Counter operator++(int) {  // Post-increment
        Counter temp = *this;
        ++count;
        return temp;
    }
};
```

Breakdown (Pre-increment):
```cpp
Counter& operator++() {
    ++count;
    return *this;
}
```
- `Counter&` is the return type (reference to the current object).
- `operator++()` is the syntax for overloading the pre-increment operator.

Breakdown (Post-increment):
```cpp
Counter operator++(int) {
    Counter temp = *this;
    ++count;
    return temp;
}
```
- `Counter` is the return type (a copy of the original value).
- `operator++(int)` is the syntax for overloading the post-increment operator. The `int` parameter distinguishes it from pre-increment.

### 5. Function Call Operator ()

Example: Overloading the function call operator

```cpp
class Multiplier {
private:
    int factor;

public:
    Multiplier(int f) : factor(f) {}

    int operator()(int x) const {
        return x * factor;
    }
};
```

Breakdown:
```cpp
int operator()(int x) const {
    return x * factor;
}
```
- `int` is the return type of the function call.
- `operator()` is the syntax for overloading the function call operator.
- `(int x)` is the parameter list for the function call.
- `const` indicates that this operation doesn't modify the object's state.

### 6. Subscript Operator []

Example: Overloading the subscript operator for a custom array class

```cpp
class MyArray {
private:
    int* data;
    int size;

public:
    MyArray(int s) : size(s) {
        data = new int[size];
    }

    int& operator[](int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }
};
```

Breakdown:
```cpp
int& operator[](int index) {
    // ... bounds checking ...
    return data[index];
}
```
- `int&` is the return type (reference to the element).
- `operator[]` is the syntax for overloading the subscript operator.
- `(int index)` is the parameter representing the index.

### 7. Stream Insertion and Extraction Operators (<< and >>)

Example: Overloading << for custom output

```cpp
class Point {
private:
    int x, y;

public:
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};
```

Breakdown:
```cpp
friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}
```
- `friend` allows the function to access private members of Point.
- `std::ostream&` is the return type (reference to the output stream).
- `operator<<` is the syntax for overloading the stream insertion operator.
- `(std::ostream& os, const Point& p)` are the parameters: the stream and the Point object.

These examples cover many of the commonly overloaded operators in C++. Remember that not all operators can be overloaded, and some (like &&, ||, and ,) have special rules. Always consider the semantics and expected behavior when overloading operators to ensure your code remains intuitive and maintainable.