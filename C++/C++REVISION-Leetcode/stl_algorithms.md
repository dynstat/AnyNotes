# STL Algorithms

The STL provides a wide range of algorithms that operate on container elements through iterators. These algorithms are implemented as function templates and can work with any container that provides the appropriate iterator type.

## Algorithm Header Files

- `<algorithm>`: General algorithms
- `<numeric>`: Numeric algorithms
- `<execution>` (C++17): Execution policies for parallel algorithms

## Non-Modifying Sequence Operations

### Finding Elements

#### `std::find`

```cpp
template< class InputIt, class T >
InputIt find( InputIt first, InputIt last, const T& value );
```

**Description**: Finds the first occurrence of a value in a range.  
**Parameters**:
- `first`, `last`: Input iterator range
- `value`: Value to find  
**Return Value**: Iterator to the first element equal to `value`, or `last` if not found.  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    auto it = std::find(v.begin(), v.end(), 3);
    
    if (it != v.end()) {
        std::cout << "Found " << *it << " at position " 
                  << std::distance(v.begin(), it) << std::endl;
    } else {
        std::cout << "Element not found" << std::endl;
    }
    
    return 0;
}
```

#### `std::find_if`

```cpp
template< class InputIt, class UnaryPredicate >
InputIt find_if( InputIt first, InputIt last, UnaryPredicate p );
```

**Description**: Finds the first element that satisfies a predicate.  
**Parameters**:
- `first`, `last`: Input iterator range
- `p`: Unary predicate function  
**Return Value**: Iterator to the first element for which `p` returns true, or `last` if not found.  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>

bool isEven(int n) {
    return n % 2 == 0;
}

int main() {
    std::vector<int> v = {1, 3, 5, 2, 4, 6};
    
    auto it = std::find_if(v.begin(), v.end(), isEven);
    
    if (it != v.end()) {
        std::cout << "First even number: " << *it << std::endl;
    }
    
    // Using lambda expression (C++11)
    auto it2 = std::find_if(v.begin(), v.end(), 
                           [](int n) { return n > 4; });
    
    if (it2 != v.end()) {
        std::cout << "First number > 4: " << *it2 << std::endl;
    }
    
    return 0;
}
```

### Counting Elements

#### `std::count`

```cpp
template< class InputIt, class T >
typename iterator_traits<InputIt>::difference_type
count( InputIt first, InputIt last, const T& value );
```

**Description**: Counts elements equal to a value.  
**Parameters**:
- `first`, `last`: Input iterator range
- `value`: Value to count  
**Return Value**: Number of elements equal to `value`.  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3, 1, 2, 1, 4, 5};
    
    int count1 = std::count(v.begin(), v.end(), 1);
    std::cout << "Number of 1s: " << count1 << std::endl;  // Outputs: 3
    
    return 0;
}
```

#### `std::count_if`

```cpp
template< class InputIt, class UnaryPredicate >
typename iterator_traits<InputIt>::difference_type
count_if( InputIt first, InputIt last, UnaryPredicate p );
```

**Description**: Counts elements that satisfy a predicate.  
**Parameters**:
- `first`, `last`: Input iterator range
- `p`: Unary predicate function  
**Return Value**: Number of elements for which `p` returns true.  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    int evenCount = std::count_if(v.begin(), v.end(),
                                 [](int n) { return n % 2 == 0; });
    
    std::cout << "Number of even elements: " << evenCount << std::endl;  // Outputs: 5
    
    int greaterThan5 = std::count_if(v.begin(), v.end(),
                                     [](int n) { return n > 5; });
    
    std::cout << "Elements greater than 5: " << greaterThan5 << std::endl;  // Outputs: 5
    
    return 0;
}
```

### Other Non-Modifying Operations

#### `std::for_each`

```cpp
template< class InputIt, class UnaryFunction >
UnaryFunction for_each( InputIt first, InputIt last, UnaryFunction f );
```

**Description**: Applies a function to each element in a range.  
**Parameters**:
- `first`, `last`: Input iterator range
- `f`: Unary function to apply  
**Return Value**: The function object after it has been applied to each element.  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>

// Function to print an element
void print(int i) {
    std::cout << i << " ";
}

// Function object to accumulate a sum
struct Sum {
    int sum = 0;
    void operator()(int n) { sum += n; }
};

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    // Using a regular function
    std::cout << "Elements: ";
    std::for_each(v.begin(), v.end(), print);
    std::cout << std::endl;
    
    // Using a lambda expression (C++11)
    std::cout << "Squared elements: ";
    std::for_each(v.begin(), v.end(), [](int n) {
        std::cout << n * n << " ";
    });
    std::cout << std::endl;
    
    // Using a function object
    Sum sum = std::for_each(v.begin(), v.end(), Sum());
    std::cout << "Sum: " << sum.sum << std::endl;
    
    return 0;
}
```

#### `std::all_of`, `std::any_of`, `std::none_of` (C++11)

```cpp
template< class InputIt, class UnaryPredicate >
bool all_of( InputIt first, InputIt last, UnaryPredicate p );

template< class InputIt, class UnaryPredicate >
bool any_of( InputIt first, InputIt last, UnaryPredicate p );

template< class InputIt, class UnaryPredicate >
bool none_of( InputIt first, InputIt last, UnaryPredicate p );
```

**Description**:
- `all_of`: Checks if all elements satisfy a predicate
- `any_of`: Checks if at least one element satisfies a predicate
- `none_of`: Checks if no elements satisfy a predicate  
**Parameters**:
- `first`, `last`: Input iterator range
- `p`: Unary predicate function  
**Return Value**: Boolean result of the check.  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v1 = {2, 4, 6, 8, 10};
    std::vector<int> v2 = {1, 3, 5, 7, 9};
    std::vector<int> v3 = {1, 2, 3, 4, 5};
    
    // Check if all elements are even
    bool all_even = std::all_of(v1.begin(), v1.end(), 
                              [](int n) { return n % 2 == 0; });
    std::cout << "All elements in v1 are even: " 
              << (all_even ? "true" : "false") << std::endl;  // Outputs: true
    
    // Check if any element is even
    bool any_even = std::any_of(v2.begin(), v2.end(), 
                              [](int n) { return n % 2 == 0; });
    std::cout << "Any element in v2 is even: " 
              << (any_even ? "true" : "false") << std::endl;  // Outputs: false
    
    // Check if no element is negative
    bool none_negative = std::none_of(v3.begin(), v3.end(), 
                                    [](int n) { return n < 0; });
    std::cout << "No element in v3 is negative: " 
              << (none_negative ? "true" : "false") << std::endl;  // Outputs: true
    
    return 0;
}
```

## Modifying Sequence Operations

### Copying and Moving

#### `std::copy`

```cpp
template< class InputIt, class OutputIt >
OutputIt copy( InputIt first, InputIt last, OutputIt d_first );
```

**Description**: Copies a range of elements.  
**Parameters**:
- `first`, `last`: Input iterator range to copy from
- `d_first`: Output iterator to the beginning of the destination range  
**Return Value**: Iterator to the element past the last copied element.  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

int main() {
    std::vector<int> source = {1, 2, 3, 4, 5};
    std::vector<int> dest(5);
    
    // Copy all elements from source to dest
    std::copy(source.begin(), source.end(), dest.begin());
    
    std::cout << "Copied vector: ";
    for (int n : dest) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // Copy to output stream
    std::cout << "Using copy with ostream_iterator: ";
    std::copy(source.begin(), source.end(), 
             std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    
    return 0;
}
```

#### `std::transform`

```cpp
template< class InputIt, class OutputIt, class UnaryOperation >
OutputIt transform( InputIt first1, InputIt last1, OutputIt d_first, 
                    UnaryOperation unary_op );
```

**Description**: Applies a function to a range and stores the result in another range.  
**Parameters**:
- `first1`, `last1`: Input iterator range to transform
- `d_first`: Output iterator to the beginning of the destination range
- `unary_op`: Function to apply  
**Return Value**: Iterator to the element past the last transformed element.  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>
#include <cctype>

int main() {
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> result1(5);
    
    // Unary operation: square each element
    std::transform(v1.begin(), v1.end(), result1.begin(),
                  [](int x) { return x * x; });
    
    std::cout << "Squared values: ";
    for (int n : result1) {
        std::cout << n << " ";  // Outputs: 1 4 9 16 25
    }
    std::cout << std::endl;
    
    // String example: convert to uppercase
    std::string s = "Hello, World!";
    std::string upper(s.size(), ' ');
    
    std::transform(s.begin(), s.end(), upper.begin(),
                  [](unsigned char c) { return std::toupper(c); });
    
    std::cout << "Original: " << s << std::endl;
    std::cout << "Uppercase: " << upper << std::endl;
    
    return 0;
}
```

### Generating and Filling

#### `std::fill`, `std::fill_n`

```cpp
template< class ForwardIt, class T >
void fill( ForwardIt first, ForwardIt last, const T& value );

template< class OutputIt, class Size, class T >
OutputIt fill_n( OutputIt first, Size count, const T& value );
```

**Description**:
- `fill`: Assigns a value to a range of elements
- `fill_n`: Assigns a value to a specified number of elements  
**Parameters**:
- `first`, `last`: Forward iterator range to fill
- `count`: Number of elements to fill
- `value`: Value to assign  
**Return Value**: `fill_n` returns an iterator to the element past the last filled element.  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

int main() {
    std::vector<int> v(10);
    
    // Fill entire vector with 5
    std::fill(v.begin(), v.end(), 5);
    
    std::cout << "Vector filled with 5: ";
    for (int n : v) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // Fill first half with 1
    std::fill_n(v.begin(), v.size() / 2, 1);
    
    std::cout << "After filling first half with 1: ";
    for (int n : v) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

#### `std::generate`, `std::generate_n`

```cpp
template< class ForwardIt, class Generator >
void generate( ForwardIt first, ForwardIt last, Generator g );

template< class OutputIt, class Size, class Generator >
OutputIt generate_n( OutputIt first, Size count, Generator g );
```

**Description**:
- `generate`: Assigns the result of a generator function to a range of elements
- `generate_n`: Assigns the result of a generator function to a specified number of elements  
**Parameters**:
- `first`, `last`: Forward iterator range to fill
- `count`: Number of elements to fill
- `g`: Generator function  
**Return Value**: `generate_n` returns an iterator to the element past the last generated element.  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>

int main() {
    std::vector<int> v(10);
    
    // Generate sequence 0, 1, 2, ...
    int i = 0;
    std::generate(v.begin(), v.end(), [&i]() { return i++; });
    
    std::cout << "Sequence 0 to 9: ";
    for (int n : v) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // Generate random numbers
    std::mt19937 rng(std::time(nullptr));  // Random number generator
    std::uniform_int_distribution<int> dist(1, 100);
    
    std::generate(v.begin(), v.end(), [&]() { return dist(rng); });
    
    std::cout << "Random numbers: ";
    for (int n : v) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

### Removing and Replacing

#### `std::remove`, `std::remove_if`

```cpp
template< class ForwardIt, class T >
ForwardIt remove( ForwardIt first, ForwardIt last, const T& value );

template< class ForwardIt, class UnaryPredicate >
ForwardIt remove_if( ForwardIt first, ForwardIt last, UnaryPredicate p );
```

**Description**:
- `remove`: Removes all elements equal to `value`
- `remove_if`: Removes all elements that satisfy a predicate  
**Parameters**:
- `first`, `last`: Forward iterator range to remove from
- `value`: Value to remove
- `p`: Unary predicate function  
**Return Value**: Iterator to the new end of the range (elements are shifted, but container size is not changed).  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

void print_vector(const std::vector<int>& v, const std::string& label) {
    std::cout << label << ": ";
    for (int n : v) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 3, 6, 7, 3, 8, 9};
    print_vector(v, "Original vector");
    
    // Remove all 3s (doesn't change the size of the container)
    auto new_end = std::remove(v.begin(), v.end(), 3);
    
    // Elements are moved, but size remains the same
    print_vector(v, "After remove (before erase)");
    
    // Erase the "removed" elements
    v.erase(new_end, v.end());
    print_vector(v, "After erase");
    
    // Remove-erase idiom in one step
    std::vector<int> v2 = {1, 2, 3, 4, 5, 3, 6, 7, 3, 8, 9};
    v2.erase(std::remove(v2.begin(), v2.end(), 3), v2.end());
    print_vector(v2, "After remove-erase idiom");
    
    // Using remove_if to remove even numbers
    std::vector<int> v3 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    v3.erase(std::remove_if(v3.begin(), v3.end(), 
                           [](int n) { return n % 2 == 0; }),
             v3.end());
    print_vector(v3, "After removing even numbers");
    
    return 0;
}
```

#### `std::replace`, `std::replace_if`

```cpp
template< class ForwardIt, class T >
void replace( ForwardIt first, ForwardIt last,
              const T& old_value, const T& new_value );

template< class ForwardIt, class UnaryPredicate, class T >
void replace_if( ForwardIt first, ForwardIt last,
                 UnaryPredicate p, const T& new_value );
```

**Description**:
- `replace`: Replaces all elements equal to `old_value` with `new_value`
- `replace_if`: Replaces all elements that satisfy a predicate with `new_value`  
**Parameters**:
- `first`, `last`: Forward iterator range to replace in
- `old_value`: Value to replace
- `new_value`: Replacement value
- `p`: Unary predicate function  
**Return Value**: None (void).  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 3, 6, 7, 3, 8, 9};
    
    // Replace all 3s with 30
    std::replace(v.begin(), v.end(), 3, 30);
    
    std::cout << "After replacing 3s with 30: ";
    for (int n : v) {
        std::cout << n << " ";  // Outputs: 1 2 30 4 5 30 6 7 30 8 9
    }
    std::cout << std::endl;
    
    // Replace even numbers with 0
    std::replace_if(v.begin(), v.end(), 
                   [](int n) { return n % 2 == 0; }, 0);
    
    std::cout << "After replacing even numbers with 0: ";
    for (int n : v) {
        std::cout << n << " ";  // Outputs: 1 0 30 0 5 30 0 7 30 0 9
    }
    std::cout << std::endl;
    
    return 0;
}
```

## Sorting Operations

#### `std::sort`

```cpp
template< class RandomIt >
void sort( RandomIt first, RandomIt last );

template< class RandomIt, class Compare >
void sort( RandomIt first, RandomIt last, Compare comp );
```

**Description**: Sorts elements in a range.  
**Parameters**:
- `first`, `last`: Random access iterator range to sort
- `comp`: Binary comparison function (optional)  
**Return Value**: None (void).  
**Complexity**: O(n log n) on average

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <functional>  // For std::greater

struct Person {
    std::string name;
    int age;
    
    // For debugging
    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        return os << p.name << " (" << p.age << ")";
    }
};

bool comparePersonByAge(const Person& a, const Person& b) {
    return a.age < b.age;
}

int main() {
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    
    // Sort in ascending order (default)
    std::sort(v.begin(), v.end());
    
    std::cout << "Sorted (ascending): ";
    for (int n : v) {
        std::cout << n << " ";  // Outputs: 1 2 3 4 5 6 7 8 9
    }
    std::cout << std::endl;
    
    // Sort in descending order using std::greater
    std::sort(v.begin(), v.end(), std::greater<int>());
    
    std::cout << "Sorted (descending): ";
    for (int n : v) {
        std::cout << n << " ";  // Outputs: 9 8 7 6 5 4 3 2 1
    }
    std::cout << std::endl;
    
    // Sort custom objects
    std::vector<Person> people = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35},
        {"David", 20}
    };
    
    // Sort by age using named function
    std::sort(people.begin(), people.end(), comparePersonByAge);
    
    std::cout << "People sorted by age: ";
    for (const auto& person : people) {
        std::cout << person << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

#### `std::stable_sort`

```cpp
template< class RandomIt >
void stable_sort( RandomIt first, RandomIt last );

template< class RandomIt, class Compare >
void stable_sort( RandomIt first, RandomIt last, Compare comp );
```

**Description**: Sorts elements in a range while preserving the relative order of equal elements.  
**Parameters**:
- `first`, `last`: Random access iterator range to sort
- `comp`: Binary comparison function (optional)  
**Return Value**: None (void).  
**Complexity**: O(n log² n), or O(n log n) if additional memory is available

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

struct Employee {
    std::string name;
    std::string department;
    int salary;
    
    // For debugging
    friend std::ostream& operator<<(std::ostream& os, const Employee& e) {
        return os << e.name << " (" << e.department << ", $" << e.salary << ")";
    }
};

int main() {
    std::vector<Employee> employees = {
        {"Alice", "Engineering", 90000},
        {"Bob", "Sales", 80000},
        {"Charlie", "Engineering", 85000},
        {"David", "Marketing", 75000},
        {"Eve", "Sales", 82000},
        {"Frank", "Engineering", 92000}
    };
    
    // First sort by salary (this order will be preserved for equal departments)
    std::sort(employees.begin(), employees.end(),
             [](const Employee& a, const Employee& b) {
                 return a.salary > b.salary;
             });
    
    std::cout << "Sorted by salary (descending):" << std::endl;
    for (const auto& employee : employees) {
        std::cout << "  " << employee << std::endl;
    }
    std::cout << std::endl;
    
    // Then stable sort by department (employees in the same department remain sorted by salary)
    std::stable_sort(employees.begin(), employees.end(),
                    [](const Employee& a, const Employee& b) {
                        return a.department < b.department;
                    });
    
    std::cout << "Stable sorted by department (then by salary):" << std::endl;
    for (const auto& employee : employees) {
        std::cout << "  " << employee << std::endl;
    }
    
    return 0;
}
```

## Set Operations

#### `std::set_union`

```cpp
template< class InputIt1, class InputIt2, class OutputIt >
OutputIt set_union( InputIt1 first1, InputIt1 last1,
                    InputIt2 first2, InputIt2 last2,
                    OutputIt d_first );
```

**Description**: Constructs a sorted range consisting of elements present in one or both sorted ranges.  
**Parameters**:
- `first1`, `last1`: First sorted input range
- `first2`, `last2`: Second sorted input range
- `d_first`: Output iterator to the beginning of the destination range  
**Return Value**: Iterator to the end of the constructed range.  
**Complexity**: Linear — O(n + m)

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

int main() {
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2 = {3, 4, 5, 6, 7};
    std::vector<int> result;
    
    // Compute union of two sets
    std::set_union(v1.begin(), v1.end(),
                  v2.begin(), v2.end(),
                  std::back_inserter(result));
    
    std::cout << "Union: ";
    for (int n : result) {
        std::cout << n << " ";  // Outputs: 1 2 3 4 5 6 7
    }
    std::cout << std::endl;
    
    return 0;
}
```

#### `std::set_intersection`

```cpp
template< class InputIt1, class InputIt2, class OutputIt >
OutputIt set_intersection( InputIt1 first1, InputIt1 last1,
                          InputIt2 first2, InputIt2 last2,
                          OutputIt d_first );
```

**Description**: Constructs a sorted range consisting of elements present in both sorted ranges.  
**Parameters**:
- `first1`, `last1`: First sorted input range
- `first2`, `last2`: Second sorted input range
- `d_first`: Output iterator to the beginning of the destination range  
**Return Value**: Iterator to the end of the constructed range.  
**Complexity**: Linear — O(n + m)

**Example**:
```cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

int main() {
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2 = {3, 4, 5, 6, 7};
    std::vector<int> result;
    
    // Compute intersection of two sets
    std::set_intersection(v1.begin(), v1.end(),
                         v2.begin(), v2.end(),
                         std::back_inserter(result));
    
    std::cout << "Intersection: ";
    for (int n : result) {
        std::cout << n << " ";  // Outputs: 3 4 5
    }
    std::cout << std::endl;
    
    return 0;
}
```

## Numeric Operations (from `<numeric>`)

#### `std::accumulate`

```cpp
template< class InputIt, class T >
T accumulate( InputIt first, InputIt last, T init );

template< class InputIt, class T, class BinaryOperation >
T accumulate( InputIt first, InputIt last, T init, BinaryOperation op );
```

**Description**: Computes the sum of a range of elements.  
**Parameters**:
- `first`, `last`: Input iterator range
- `init`: Initial value
- `op`: Binary operation function (optional)  
**Return Value**: The accumulated result.  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <numeric>
#include <iostream>
#include <vector>
#include <string>
#include <functional>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    // Sum of elements
    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "Sum: " << sum << std::endl;  // Outputs: 15
    
    // Product of elements
    int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());
    std::cout << "Product: " << product << std::endl;  // Outputs: 120
    
    // Custom accumulation function
    auto sum_of_squares = std::accumulate(v.begin(), v.end(), 0,
                                        [](int acc, int val) {
                                            return acc + val * val;
                                        });
    std::cout << "Sum of squares: " << sum_of_squares << std::endl;  // Outputs: 55
    
    // String concatenation
    std::vector<std::string> words = {"Hello", ", ", "world", "!"};
    std::string sentence = std::accumulate(words.begin(), words.end(), std::string(""));
    std::cout << "Concatenated: " << sentence << std::endl;  // Outputs: Hello, world!
    
    return 0;
}
```

#### `std::inner_product`

```cpp
template< class InputIt1, class InputIt2, class T >
T inner_product( InputIt1 first1, InputIt1 last1,
                InputIt2 first2, T init );
```

**Description**: Computes the inner product of two ranges of elements.  
**Parameters**:
- `first1`, `last1`: First input iterator range
- `first2`: Beginning of second input range
- `init`: Initial value  
**Return Value**: The inner product value.  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <numeric>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2 = {5, 4, 3, 2, 1};
    
    // Inner product (dot product)
    int result = std::inner_product(v1.begin(), v1.end(), v2.begin(), 0);
    std::cout << "Inner product: " << result << std::endl;  // Outputs: 35 (1*5 + 2*4 + 3*3 + 4*2 + 5*1)
    
    // Custom operations
    int custom = std::inner_product(v1.begin(), v1.end(), v2.begin(), 0,
                                   std::plus<int>(), // Operation for combining elements
                                   std::equal_to<int>()); // Operation for comparing elements
    std::cout << "Count of equal elements: " << custom << std::endl;  // Outputs: 1 (only v1[2] == v2[2])
    
    return 0;
}
```

#### `std::iota` (C++11)

```cpp
template< class ForwardIt, class T >
void iota( ForwardIt first, ForwardIt last, T value );
```

**Description**: Fills a range with sequential values.  
**Parameters**:
- `first`, `last`: Forward iterator range
- `value`: Initial value  
**Return Value**: None (void).  
**Complexity**: Linear — O(n)

**Example**:
```cpp
#include <numeric>
#include <iostream>
#include <vector>
#include <list>

int main() {
    std::vector<int> v(10);
    
    // Fill with 0, 1, 2, ...
    std::iota(v.begin(), v.end(), 0);
    
    std::cout << "Vector with increasing values: ";
    for (int n : v) {
        std::cout << n << " ";  // Outputs: 0 1 2 3 4 5 6 7 8 9
    }
    std::cout << std::endl;
    
    // Fill with 100, 101, 102, ...
    std::list<int> lst(5);
    std::iota(lst.begin(), lst.end(), 100);
    
    std::cout << "List with values starting at 100: ";
    for (int n : lst) {
        std::cout << n << " ";  // Outputs: 100 101 102 103 104
    }
    std::cout << std::endl;
    
    return 0;
}
```

## C++17 Parallel Algorithms

C++17 introduced execution policies to allow algorithms to run in parallel:

```cpp
#include <algorithm>
#include <execution>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>

int main() {
    // Create a large vector
    std::vector<int> v(10000000);
    
    // Fill with random numbers
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    
    std::generate(v.begin(), v.end(), [&]() { return dis(gen); });
    
    // Sequential sort
    auto seq_start = std::chrono::high_resolution_clock::now();
    std::sort(v.begin(), v.end());
    auto seq_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> seq_ms = seq_end - seq_start;
    
    // Shuffle again
    std::shuffle(v.begin(), v.end(), gen);
    
    // Parallel sort
    auto par_start = std::chrono::high_resolution_clock::now();
    std::sort(std::execution::par, v.begin(), v.end());
    auto par_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> par_ms = par_end - par_start;
    
    std::cout << "Sequential sort took: " << seq_ms.count() << " ms\n";
    std::cout << "Parallel sort took: " << par_ms.count() << " ms\n";
    
    return 0;
}
```

## Best Practices

1. **Choose the right algorithm** for your task - the STL offers a wide range of algorithms for different purposes.
2. **Use algorithm composition** - combine algorithms for more complex operations.
3. **Prefer algorithms to hand-written loops** for better readability and performance.
4. **Use appropriate iterator types** - some algorithms require specific iterator capabilities.
5. **Consider using lambda expressions** for custom operations.
6. **Be aware of algorithm complexity** for performance-critical code.
7. **Use execution policies** (C++17) for parallel execution when appropriate.
8. **Remember that algorithms don't change container size** - operations like `remove` need to be paired with `erase`.
9. **Use container-specific member functions** when they exist (e.g., `vector::push_back` instead of `copy` with `back_inserter`).
10. **Consider function objects over function pointers** for better inlining.