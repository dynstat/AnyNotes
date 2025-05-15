### C++ Iterators and Initialization: A Comprehensive Guide

This guide covers various aspects of iterators, vector initialization, and object creation in C++. It includes detailed explanations and consistent examples to help you understand these concepts better.

---

### 1. Vector Initialization

#### Initializer List
```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec1 = {1, 2, 3, 4, 5}; // Initializer list
    for (const int& num : vec1) {
        std::cout << num << " "; // Output: 1 2 3 4 5
    }
    std::cout << std::endl;
    return 0;
}
```
- **Syntax**: `std::vector<int> vec = {1, 2, 3, 4, 5};`
- **Description**: Initializes the vector with the given elements.
- **Use Case**: Simple and clear way to initialize a vector with known values.

#### Uniform Initialization
```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec2{6, 7, 8, 9, 10}; // Uniform initialization
    for (const int& num : vec2) {
        std::cout << num << " "; // Output: 6 7 8 9 10
    }
    std::cout << std::endl;
    return 0;
}
```
- **Syntax**: `std::vector<int> vec{6, 7, 8, 9, 10};`
- **Description**: Another way to initialize the vector with the given elements.
- **Use Case**: Preferred in modern C++ for its uniformity across different types.

#### Fill Constructor
```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec3(5, 12); // Fill constructor: 5 elements, each initialized to 12
    for (const int& num : vec3) {
        std::cout << num << " "; // Output: 12 12 12 12 12
    }
    std::cout << std::endl;
    return 0;
}
```
- **Syntax**: `std::vector<int> vec(size, value);`
- **Description**: Initializes the vector with a specified number of elements, each set to a given value.
- **Use Case**: Useful when you need a vector of a specific size with all elements initialized to the same value.

---

### 2. Iterators

#### Accessing Elements
```cpp
#include <iostream>
#include <vector>
#include <string>

class Person {
public:
    Person(std::string name, int age) : name_(name), age_(age) {}
    void display() const { std::cout << name_ << " (" << age_ << ")" << std::endl; }
private:
    std::string name_;
    int age_;
};

int main() {
    std::vector<Person> people = {Person("Alice", 30), Person("Bob", 25), Person("Charlie", 35)};
    
    // Using iterator to access elements
    for (auto it = people.begin(); it != people.end(); ++it) {
        it->display(); // Using arrow operator to call member function
    }
    return 0;
}
```
- **Syntax**: `for (auto it = container.begin(); it != container.end(); ++it)`
- **Description**: Iterates over the elements of the container.
- **Use Case**: Accessing and manipulating elements in a container.

#### Position Information
```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> numbers = {10, 20, 30, 40, 50};
    
    auto it = numbers.begin();
    std::cout << "First element: " << *it << std::endl; // Output: 10
    
    it += 2; // Move iterator 2 positions forward
    std::cout << "Third element: " << *it << std::endl; // Output: 30
    
    it = numbers.end() - 1; // Point to the last element
    std::cout << "Last element: " << *it << std::endl; // Output: 50
    
    return 0;
}
```
- **Syntax**: `it += n`, `it = container.end() - 1`
- **Description**: Moves the iterator to a specific position within the container.
- **Use Case**: Accessing elements at specific positions.

#### Moving Through Container
```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    auto it = numbers.begin();
    
    std::cout << "Moving forward:" << std::endl;
    while (it != numbers.end()) {
        std::cout << *it << " "; // Output: 1 2 3 4 5
        ++it; // Move to next element
    }
    std::cout << std::endl;
    
    std::cout << "Moving backward:" << std::endl;
    while (it != numbers.begin()) {
        --it; // Move to previous element
        std::cout << *it << " "; // Output: 5 4 3 2 1
    }
    std::cout << std::endl;
    
    return 0;
}
```
- **Syntax**: `++it`, `--it`
- **Description**: Moves the iterator forward or backward through the container.
- **Use Case**: Traversing the container in both directions.

---

### 3. Range-Based For Loop

#### Using Const Reference
```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // Range-based for loop with const reference
    for (const int& num : numbers) {
        std::cout << num << " "; // Output: 1 2 3 4 5
    }
    std::cout << std::endl;
    
    return 0;
}
```
- **Syntax**: `for (const int& element : container)`
- **Description**: Iterates over the elements of the container without copying them.
- **Use Case**: Efficiently accessing elements without modifying them.

#### Using Value (Creates a Copy)
```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // Range-based for loop with value (creates a copy)
    for (int num : numbers) {
        std::cout << num << " "; // Output: 1 2 3 4 5
    }
    std::cout << std::endl;
    
    return 0;
}
```
- **Syntax**: `for (int element : container)`
- **Description**: Iterates over the elements of the container, creating a copy of each element.
- **Use Case**: When you need to work with copies of the elements.

---

### 4. Object Creation

#### Stack Allocation
```cpp
#include <iostream>

class Rectangle {
public:
    Rectangle(int length, int breadth) : length_(length), breadth_(breadth) {
        std::cout << "Constructor called with length = " << length_ << " and breadth = " << breadth_ << std::endl;
    }
    int getArea() const { return length_ * breadth_; }
private:
    int length_;
    int breadth_;
};

int main() {
    Rectangle rect(7, 6); // Stack allocation
    std::cout << "Area: " << rect.getArea() << std::endl; // Output: Area: 42
    return 0;
}
```
- **Syntax**: `ClassName objectName(parameters);`
- **Description**: Creates an object on the stack.
- **Use Case**: For local objects with limited scope.

#### Heap Allocation
```cpp
#include <iostream>

class Rectangle {
public:
    Rectangle(int length, int breadth) : length_(length), breadth_(breadth) {
        std::cout << "Constructor called with length = " << length_ << " and breadth = " << breadth_ << std::endl;
    }
    int getArea() const { return length_ * breadth_; }
private:
    int length_;
    int breadth_;
};

int main() {
    Rectangle* rect = new Rectangle(7, 6); // Heap allocation
    std::cout << "Area: " << rect->getArea() << std::endl; // Output: Area: 42
    delete rect; // Manually delete the object to free memory
    return 0;
}
```
- **Syntax**: `ClassName* objectName = new ClassName(parameters);`
- **Description**: Creates an object on the heap.
- **Use Case**: For objects that need to persist beyond the scope.

---

### Summary

- **Vector Initialization**: Use initializer lists, uniform initialization, or fill constructors to initialize vectors.
- **Iterators**: Provide a way to traverse and manipulate container elements. Use `*it` for value access and `it->` for member access.
- **Range-Based For Loop**: Preferred for its simplicity and readability. Use `const int&` to avoid copying and ensure elements are not modified.
- **Object Creation**: Use stack allocation for local objects with limited scope and heap allocation for objects that need to persist beyond the scope.


  

### QUESTIONS

Here are 30 questions covering practical and tricky use cases of vectors in C++. These questions are designed to help you learn and practice various aspects of working with vectors:
### Time and Space Complexity Analysis

  

1. Initialize a vector with a specific number of elements, all set to a particular value:

   - Time Complexity: O(n), where n is the size of the vector

   - Space Complexity: O(n)

   - Notable: Uses the vector constructor for efficient initialization

  

2. Remove all duplicate elements from a vector, preserving the original order:

   - Time Complexity: O(n), where n is the size of the vector

   - Space Complexity: O(n) in the worst case

   - Notable: Uses an unordered_set for efficient lookup

  

3. Efficiently insert multiple elements at a specific position in a vector:

   - Time Complexity: O(n + m), where n is the size of the vector and m is the number of elements to insert

   - Space Complexity: O(1) (in-place operation)

   - Notable: Uses vector's insert method for efficient insertion

  

4. Rotate a vector to the right by k positions:

   - Time Complexity: O(n), where n is the size of the vector

   - Space Complexity: O(1) (in-place operation)

   - Notable: Uses reverse operations for efficient rotation

  

5. Find the intersection of two sorted vectors:

   - Time Complexity: O(min(n, m)), where n and m are the sizes of the input vectors

   - Space Complexity: O(min(n, m)) for the result vector

   - Notable: Uses two-pointer technique for efficient comparison

  

6. Merge two sorted vectors into a single sorted vector:

   - Time Complexity: O(n + m), where n and m are the sizes of the input vectors

   - Space Complexity: O(n + m) for the result vector

   - Notable: Uses two-pointer technique for efficient merging

  

7. Efficiently remove all elements from a vector that satisfy a certain condition:

   - Time Complexity: O(n), where n is the size of the vector

   - Space Complexity: O(1) (in-place operation)

   - Notable: Uses the erase-remove idiom for efficient removal

  

8. Reverse a vector in-place:

   - Time Complexity: O(n), where n is the size of the vector

   - Space Complexity: O(1) (in-place operation)

   - Notable: Uses two-pointer technique for efficient swapping

  

9. Check if a vector is a palindrome:

   - Time Complexity: O(n), where n is the size of the vector

   - Space Complexity: O(1)

   - Notable: Uses two-pointer technique for efficient comparison

  

10. Find the longest contiguous increasing subsequence in a vector:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(1)

    - Notable: Uses a single pass through the vector to find the longest subsequence

  

11. Efficiently find the kth largest element in an unsorted vector:

    - Time Complexity: O(n) on average, O(n^2) in the worst case

    - Space Complexity: O(1) (in-place operation)

    - Notable: Uses the QuickSelect algorithm (nth_element) for efficient selection

  

12. Perform in-place partitioning of a vector around a pivot element:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(1) (in-place operation)

    - Notable: Uses the Lomuto partition scheme

  

13. Efficiently compute the running sum of elements in a vector:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(n) for the result vector

    - Notable: Uses a single pass through the vector to compute the running sum

  

14. Find all pairs of elements in a vector that sum up to a given target:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(n) for the hash map

    - Notable: Uses a hash map for efficient lookup of complements

  

15. Efficiently remove all occurrences of a specific element from a vector:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(1) (in-place operation)

    - Notable: Uses the erase-remove idiom for efficient removal

  

16. Perform cyclic rotation of a vector by k positions to the left:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(1) (in-place operation)

    - Notable: Uses reverse operations for efficient rotation

  

17. Find the majority element (occurring more than n/2 times) in a vector, if it exists:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(1)

    - Notable: Uses the Boyer-Moore Voting Algorithm

  

18. Rearrange a vector so that all even elements come before odd elements:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(1) (in-place operation)

    - Notable: Uses two-pointer technique for efficient partitioning

  

19. Efficiently find the maximum subarray sum in a vector:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(1)

    - Notable: Uses Kadane's algorithm for efficient computation

  

20. Perform in-place removal of all duplicate elements from a sorted vector:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(1) (in-place operation)

    - Notable: Uses two-pointer technique for efficient removal

  

21. Efficiently shuffle the elements of a vector randomly:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(1) (in-place operation)

    - Notable: Uses the Fisher-Yates shuffle algorithm

  

22. Find the longest palindromic subsequence in a vector:

    - Time Complexity: O(n^2), where n is the size of the vector

    - Space Complexity: O(n^2) for the DP table

    - Notable: Uses dynamic programming to solve the problem efficiently

  

23. Efficiently find the median of two sorted vectors:

    - Time Complexity: O(log(min(n, m))), where n and m are the sizes of the input vectors

    - Space Complexity: O(1)

    - Notable: Uses binary search to find the partition point efficiently

  

24. Implement in-place Dutch National Flag partitioning on a vector:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(1) (in-place operation)

    - Notable: Uses three-pointer technique for efficient partitioning

  

25. Efficiently find the smallest missing positive integer in an unsorted vector:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(1) (in-place operation)

    - Notable: Uses index mapping technique for efficient search

  

26. Perform in-place matrix transposition using a 1D vector representation:

    - Time Complexity: O(n*m), where n is the number of rows and m is the number of columns

    - Space Complexity: O(n*m) for the transposed matrix

    - Notable: Uses index mapping to convert between 1D and 2D representations

  

27. Implement a sliding window maximum algorithm on a vector:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(k), where k is the size of the sliding window

    - Notable: Uses a deque to maintain the maximum element in the current window

  

28. Perform in-place removal of all elements in a range [start, end) from a vector:

    - Time Complexity: O(n), where n is the size of the vector

    - Space Complexity: O(1) (in-place operation)

    - Notable: Uses two-pointer technique for efficient removal

  

29. Find the longest bitonic subsequence in a vector:

    - Time Complexity: O(n^2), where n is the size of the vector

    - Space Complexity: O(n) for the DP arrays

    - Notable: Uses dynamic programming to solve the problem efficiently

  

30. Perform in-place merging of k sorted vectors into a single sorted vector:

    - Time Complexity: O(N * log(k)), where N is the total number of elements across all vectors and k is the number of vectors

    - Space Complexity: O(k) for the priority queue

    - Notable: Uses a priority queue to efficiently merge multiple sorted vectors

---

Part 1 (Questions 1-10):

1. Initialize a vector with a specific number of elements, all set to a particular value:

```cpp
#include <iostream>
#include <vector>

int main() {
    int size = 5;
    int value = 10;
    
    // Initialize vector with 'size' elements, all set to 'value'
    std::vector<int> vec(size, value);
    
    // Print the vector
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

2. Remove all duplicate elements from a vector, preserving the original order:

```cpp
#include <iostream>
#include <vector>
#include <unordered_set>

std::vector<int> removeDuplicates(std::vector<int>& vec) {
    std::unordered_set<int> seen;
    std::vector<int> result;
    
    for (int num : vec) {
        // If the number hasn't been seen before, add it to the result
        if (seen.find(num) == seen.end()) {
            result.push_back(num);
            seen.insert(num);
        }
    }
    
    return result;
}

int main() {
    std::vector<int> vec = {1, 2, 3, 2, 4, 3, 5};
    std::vector<int> unique = removeDuplicates(vec);
    
    // Print the vector without duplicates
    for (int num : unique) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

3. Efficiently insert multiple elements at a specific position in a vector:

```cpp
#include <iostream>
#include <vector>

void insertMultiple(std::vector<int>& vec, int position, const std::vector<int>& elements) {
    // Use the insert function to add multiple elements at once
    vec.insert(vec.begin() + position, elements.begin(), elements.end());
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::vector<int> toInsert = {10, 20, 30};
    
    insertMultiple(vec, 2, toInsert);
    
    // Print the modified vector
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

4. Rotate a vector to the right by k positions:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

void rotateRight(std::vector<int>& vec, int k) {
    int n = vec.size();
    k = k % n; // Normalize k to be within the vector size
    
    // Reverse the entire vector
    std::reverse(vec.begin(), vec.end());
    // Reverse the first k elements
    std::reverse(vec.begin(), vec.begin() + k);
    // Reverse the remaining elements
    std::reverse(vec.begin() + k, vec.end());
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    int k = 2;
    
    rotateRight(vec, k);
    
    // Print the rotated vector
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

5. Find the intersection of two sorted vectors:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> findIntersection(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    std::vector<int> result;
    int i = 0, j = 0;
    
    while (i < vec1.size() && j < vec2.size()) {
        if (vec1[i] == vec2[j]) {
            // Found a common element, add it to the result
            result.push_back(vec1[i]);
            i++;
            j++;
        } else if (vec1[i] < vec2[j]) {
            i++;
        } else {
            j++;
        }
    }
    
    return result;
}

int main() {
    std::vector<int> vec1 = {1, 2, 4, 5, 6};
    std::vector<int> vec2 = {2, 3, 5, 7};
    
    std::vector<int> intersection = findIntersection(vec1, vec2);
    
    // Print the intersection
    for (int num : intersection) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

6. Merge two sorted vectors into a single sorted vector:

```cpp
#include <iostream>
#include <vector>

std::vector<int> mergeSortedVectors(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    std::vector<int> result;
    int i = 0, j = 0;
    
    while (i < vec1.size() && j < vec2.size()) {
        if (vec1[i] <= vec2[j]) {
            result.push_back(vec1[i]);
            i++;
        } else {
            result.push_back(vec2[j]);
            j++;
        }
    }
    
    // Add remaining elements from vec1, if any
    while (i < vec1.size()) {
        result.push_back(vec1[i]);
        i++;
    }
    
    // Add remaining elements from vec2, if any
    while (j < vec2.size()) {
        result.push_back(vec2[j]);
        j++;
    }
    
    return result;
}

int main() {
    std::vector<int> vec1 = {1, 3, 5, 7};
    std::vector<int> vec2 = {2, 4, 6, 8};
    
    std::vector<int> merged = mergeSortedVectors(vec1, vec2);
    
    // Print the merged vector
    for (int num : merged) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

7. Efficiently remove all elements from a vector that satisfy a certain condition:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

void removeIf(std::vector<int>& vec, bool (*condition)(int)) {
    // Use the erase-remove idiom to efficiently remove elements
    vec.erase(std::remove_if(vec.begin(), vec.end(), condition), vec.end());
}

bool isEven(int n) {
    return n % 2 == 0;
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    removeIf(vec, isEven);
    
    // Print the vector after removing even numbers
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

8. Reverse a vector in-place:

```cpp
#include <iostream>
#include <vector>

void reverseVector(std::vector<int>& vec) {
    int left = 0;
    int right = vec.size() - 1;
    
    while (left < right) {
        // Swap elements at left and right indices
        std::swap(vec[left], vec[right]);
        left++;
        right--;
    }
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    reverseVector(vec);
    
    // Print the reversed vector
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

9. Check if a vector is a palindrome:

```cpp
#include <iostream>
#include <vector>

bool isPalindrome(const std::vector<int>& vec) {
    int left = 0;
    int right = vec.size() - 1;
    
    while (left < right) {
        if (vec[left] != vec[right]) {
            return false;
        }
        left++;
        right--;
    }
    
    return true;
}

int main() {
    std::vector<int> vec1 = {1, 2, 3, 2, 1};
    std::vector<int> vec2 = {1, 2, 3, 4, 5};
    
    std::cout << "vec1 is " << (isPalindrome(vec1) ? "a palindrome" : "not a palindrome") << std::endl;
    std::cout << "vec2 is " << (isPalindrome(vec2) ? "a palindrome" : "not a palindrome") << std::endl;
    
    return 0;
}
```

10. Find the longest contiguous increasing subsequence in a vector:

```cpp
#include <iostream>
#include <vector>

std::vector<int> longestIncreasingSubsequence(const std::vector<int>& vec) {
    if (vec.empty()) {
        return {};
    }
    
    int start = 0;
    int maxLength = 1;
    int currentStart = 0;
    int currentLength = 1;
    
    for (int i = 1; i < vec.size(); i++) {
        if (vec[i] > vec[i-1]) {
            currentLength++;
        } else {
            if (currentLength > maxLength) {
                maxLength = currentLength;
                start = currentStart;
            }
            currentStart = i;
            currentLength = 1;
        }
    }
    
    // Check if the last subsequence is the longest
    if (currentLength > maxLength) {
        maxLength = currentLength;
        start = currentStart;
    }
    
    // Return the longest increasing subsequence
    return std::vector<int>(vec.begin() + start, vec.begin() + start + maxLength);
}

int main() {
    std::vector<int> vec = {1, 2, 3, 1, 2, 3, 4, 5, 1, 2};
    
    std::vector<int> longest = longestIncreasingSubsequence(vec);
    
    // Print the longest increasing subsequence
    for (int num : longest) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```



11. Efficiently find the kth largest element in an unsorted vector:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int findKthLargest(std::vector<int>& nums, int k) {
    // Use nth_element to partially sort the vector
    std::nth_element(nums.begin(), nums.begin() + k - 1, nums.end(), std::greater<int>());
    
    // Return the kth largest element
    return nums[k - 1];
}

int main() {
    std::vector<int> nums = {3, 2, 1, 5, 6, 4};
    int k = 2;
    
    int kthLargest = findKthLargest(nums, k);
    
    std::cout << "The " << k << "th largest element is: " << kthLargest << std::endl;
    
    return 0;
}
```

12. Perform in-place partitioning of a vector around a pivot element:

```cpp
#include <iostream>
#include <vector>

int partition(std::vector<int>& vec, int low, int high) {
    int pivot = vec[high]; // Choose the last element as pivot
    int i = low - 1; // Index of smaller element
    
    for (int j = low; j < high; j++) {
        // If current element is smaller than or equal to pivot
        if (vec[j] <= pivot) {
            i++; // Increment index of smaller element
            std::swap(vec[i], vec[j]);
        }
    }
    std::swap(vec[i + 1], vec[high]);
    return i + 1;
}

int main() {
    std::vector<int> vec = {4, 2, 8, 3, 1, 5, 7, 6};
    int pivotIndex = partition(vec, 0, vec.size() - 1);
    
    std::cout << "Partitioned vector: ";
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << "\nPivot index: " << pivotIndex << std::endl;
    
    return 0;
}
```

13. Efficiently compute the running sum of elements in a vector:

```cpp
#include <iostream>
#include <vector>

std::vector<int> runningSum(const std::vector<int>& nums) {
    std::vector<int> result(nums.size());
    int sum = 0;
    
    for (int i = 0; i < nums.size(); i++) {
        sum += nums[i];
        result[i] = sum;
    }
    
    return result;
}

int main() {
    std::vector<int> nums = {1, 2, 3, 4};
    std::vector<int> result = runningSum(nums);
    
    std::cout << "Running sum: ";
    for (int num : result) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

14. Find all pairs of elements in a vector that sum up to a given target:

```cpp
#include <iostream>
#include <vector>
#include <unordered_map>

std::vector<std::pair<int, int>> findPairsWithSum(const std::vector<int>& nums, int target) {
    std::unordered_map<int, int> numMap;
    std::vector<std::pair<int, int>> result;
    
    for (int i = 0; i < nums.size(); i++) {
        int complement = target - nums[i];
        if (numMap.find(complement) != numMap.end()) {
            result.push_back({nums[i], complement});
        }
        numMap[nums[i]] = i;
    }
    
    return result;
}

int main() {
    std::vector<int> nums = {2, 7, 11, 15, 3, 6};
    int target = 9;
    
    std::vector<std::pair<int, int>> pairs = findPairsWithSum(nums, target);
    
    std::cout << "Pairs with sum " << target << ":" << std::endl;
    for (const auto& pair : pairs) {
        std::cout << pair.first << " + " << pair.second << std::endl;
    }
    
    return 0;
}
```

15. Efficiently remove all occurrences of a specific element from a vector:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

void removeElement(std::vector<int>& nums, int val) {
    // Use the erase-remove idiom
    nums.erase(std::remove(nums.begin(), nums.end(), val), nums.end());
}

int main() {
    std::vector<int> nums = {3, 2, 2, 3, 4, 3};
    int val = 3;
    
    removeElement(nums, val);
    
    std::cout << "Vector after removing " << val << ": ";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

16. Perform cyclic rotation of a vector by k positions to the left:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

void rotateLeft(std::vector<int>& nums, int k) {
    int n = nums.size();
    k = k % n; // Normalize k to be within the vector size
    
    // Reverse the first k elements
    std::reverse(nums.begin(), nums.begin() + k);
    // Reverse the remaining elements
    std::reverse(nums.begin() + k, nums.end());
    // Reverse the entire vector
    std::reverse(nums.begin(), nums.end());
}

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5};
    int k = 2;
    
    rotateLeft(nums, k);
    
    std::cout << "Vector after rotating left by " << k << " positions: ";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

17. Find the majority element (occurring more than n/2 times) in a vector, if it exists:

```cpp
#include <iostream>
#include <vector>

int findMajorityElement(const std::vector<int>& nums) {
    int candidate = nums[0];
    int count = 1;
    
    // Boyer-Moore Voting Algorithm
    for (int i = 1; i < nums.size(); i++) {
        if (count == 0) {
            candidate = nums[i];
            count = 1;
        } else if (nums[i] == candidate) {
            count++;
        } else {
            count--;
        }
    }
    
    // Verify if the candidate is the majority element
    count = 0;
    for (int num : nums) {
        if (num == candidate) {
            count++;
        }
    }
    
    return (count > nums.size() / 2) ? candidate : -1;
}

int main() {
    std::vector<int> nums = {2, 2, 1, 1, 1, 2, 2};
    
    int majorityElement = findMajorityElement(nums);
    
    if (majorityElement != -1) {
        std::cout << "Majority element: " << majorityElement << std::endl;
    } else {
        std::cout << "No majority element found." << std::endl;
    }
    
    return 0;
}
```

18. Rearrange a vector so that all even elements come before odd elements:

```cpp
#include <iostream>
#include <vector>

void rearrangeEvenOdd(std::vector<int>& nums) {
    int left = 0;
    int right = nums.size() - 1;
    
    while (left < right) {
        // Find the first odd number from the left
        while (left < right && nums[left] % 2 == 0) {
            left++;
        }
        
        // Find the first even number from the right
        while (left < right && nums[right] % 2 != 0) {
            right--;
        }
        
        // Swap the odd and even numbers
        if (left < right) {
            std::swap(nums[left], nums[right]);
        }
    }
}

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8};
    
    rearrangeEvenOdd(nums);
    
    std::cout << "Rearranged vector: ";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

19. Efficiently find the maximum subarray sum in a vector:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int maxSubarraySum(const std::vector<int>& nums) {
    int maxSum = nums[0];
    int currentSum = nums[0];
    
    for (int i = 1; i < nums.size(); i++) {
        currentSum = std::max(nums[i], currentSum + nums[i]);
        maxSum = std::max(maxSum, currentSum);
    }
    
    return maxSum;
}

int main() {
    std::vector<int> nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    
    int maxSum = maxSubarraySum(nums);
    
    std::cout << "Maximum subarray sum: " << maxSum << std::endl;
    
    return 0;
}
```

20. Perform in-place removal of all duplicate elements from a sorted vector:

```cpp
#include <iostream>
#include <vector>

int removeDuplicates(std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }
    
    int writeIndex = 1;
    
    for (int i = 1; i < nums.size(); i++) {
        if (nums[i] != nums[writeIndex - 1]) {
            nums[writeIndex] = nums[i];
            writeIndex++;
        }
    }
    
    return writeIndex;
}

int main() {
    std::vector<int> nums = {1, 1, 2, 2, 3, 4, 4, 5};
    
    int newSize = removeDuplicates(nums);
    
    std::cout << "Vector after removing duplicates: ";
    for (int i = 0; i < newSize; i++) {
        std::cout << nums[i] << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```


21. Efficiently shuffle the elements of a vector randomly:

```cpp
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

void shuffleVector(std::vector<int>& nums) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(nums.begin(), nums.end(), g);
}

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::cout << "Original vector: ";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    shuffleVector(nums);
    
    std::cout << "Shuffled vector: ";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```


22. Find the longest palindromic subsequence in a vector:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int longestPalindromeSubseq(const std::vector<int>& nums) {
    int n = nums.size();
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
    
    // All subsequences of length 1 are palindromes
    for (int i = 0; i < n; i++) {
        dp[i][i] = 1;
    }
    
    // Fill the dp table
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i < n - len + 1; i++) {
            int j = i + len - 1;
            if (nums[i] == nums[j] && len == 2) {
                dp[i][j] = 2;
            } else if (nums[i] == nums[j]) {
                dp[i][j] = dp[i+1][j-1] + 2;
            } else {
                dp[i][j] = std::max(dp[i+1][j], dp[i][j-1]);
            }
        }
    }
    
    return dp[0][n-1];
}

int main() {
    std::vector<int> nums = {1, 2, 3, 3, 2, 1, 4};
    
    int length = longestPalindromeSubseq(nums);
    
    std::cout << "Length of longest palindromic subsequence: " << length << std::endl;
    
    return 0;
}
```


23. Efficiently find the median of two sorted vectors:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

double findMedianSortedArrays(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    if (nums1.size() > nums2.size()) {
        return findMedianSortedArrays(nums2, nums1);
    }
    
    int x = nums1.size();
    int y = nums2.size();
    int low = 0;
    int high = x;
    
    while (low <= high) {
        int partitionX = (low + high) / 2;
        int partitionY = (x + y + 1) / 2 - partitionX;
        
        int maxLeftX = (partitionX == 0) ? INT_MIN : nums1[partitionX - 1];
        int minRightX = (partitionX == x) ? INT_MAX : nums1[partitionX];
        
        int maxLeftY = (partitionY == 0) ? INT_MIN : nums2[partitionY - 1];
        int minRightY = (partitionY == y) ? INT_MAX : nums2[partitionY];
        
        if (maxLeftX <= minRightY && maxLeftY <= minRightX) {
            if ((x + y) % 2 == 0) {
                return (std::max(maxLeftX, maxLeftY) + std::min(minRightX, minRightY)) / 2.0;
            } else {
                return std::max(maxLeftX, maxLeftY);
            }
        } else if (maxLeftX > minRightY) {
            high = partitionX - 1;
        } else {
            low = partitionX + 1;
        }
    }
    
    throw std::invalid_argument("Input arrays are not sorted.");
}

int main() {
    std::vector<int> nums1 = {1, 3, 5};
    std::vector<int> nums2 = {2, 4, 6};
    
    double median = findMedianSortedArrays(nums1, nums2);
    
    std::cout << "Median of the two sorted arrays: " << median << std::endl;
    
    return 0;
}
```


24. Implement in-place Dutch National Flag partitioning on a vector:

```cpp
#include <iostream>
#include <vector>

void dutchFlagPartition(std::vector<int>& nums) {
    int low = 0;
    int mid = 0;
    int high = nums.size() - 1;
    
    while (mid <= high) {
        if (nums[mid] == 0) {
            std::swap(nums[low], nums[mid]);
            low++;
            mid++;
        } else if (nums[mid] == 1) {
            mid++;
        } else {
            std::swap(nums[mid], nums[high]);
            high--;
        }
    }
}

int main() {
    std::vector<int> nums = {2, 0, 1, 2, 1, 0, 2, 1, 0};
    
    std::cout << "Original array: ";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    dutchFlagPartition(nums);
    
    std::cout << "Partitioned array: ";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```


25. Efficiently find the smallest missing positive integer in an unsorted vector:

```cpp
#include <iostream>
#include <vector>

int firstMissingPositive(std::vector<int>& nums) {
    int n = nums.size();
    
    // Step 1: Ignore non-positive and numbers greater than n
    for (int i = 0; i < n; i++) {
        if (nums[i] <= 0 || nums[i] > n) {
            nums[i] = n + 1;
        }
    }
    
    // Step 2: Mark the presence of each number in the array
    for (int i = 0; i < n; i++) {
        int num = std::abs(nums[i]);
        if (num <= n) {
            nums[num - 1] = -std::abs(nums[num - 1]);
        }
    }
    
    // Step 3: Find the first missing positive number
    for (int i = 0; i < n; i++) {
        if (nums[i] > 0) {
            return i + 1;
        }
    }
    
    return n + 1;
}

int main() {
    std::vector<int> nums = {3, 4, -1, 1};
    
    int result = firstMissingPositive(nums);
    
    std::cout << "The smallest missing positive integer is: " << result << std::endl;
    
    return 0;
}
```


26. Perform in-place matrix transposition using a 1D vector representation:

```cpp
#include <iostream>
#include <vector>

void transposeMatrix(std::vector<int>& matrix, int rows, int cols) {
    std::vector<int> transposed(rows * cols);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transposed[j * rows + i] = matrix[i * cols + j];
        }
    }
    
    matrix = transposed;
}

void printMatrix(const std::vector<int>& matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << matrix[i * cols + j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::vector<int> matrix = {1, 2, 3, 4, 5, 6};
    int rows = 2;
    int cols = 3;
    
    std::cout << "Original matrix:" << std::endl;
    printMatrix(matrix, rows, cols);
    
    transposeMatrix(matrix, rows, cols);
    
    std::cout << "Transposed matrix:" << std::endl;
    printMatrix(matrix, cols, rows);
    
    return 0;
}
```


27. Implement a sliding window maximum algorithm on a vector:

```cpp
#include <iostream>
#include <vector>
#include <deque>

std::vector<int> maxSlidingWindow(const std::vector<int>& nums, int k) {
    std::vector<int> result;
    std::deque<int> window;
    
    for (int i = 0; i < nums.size(); i++) {
        // Remove elements outside the current window
        if (!window.empty() && window.front() == i - k) {
            window.pop_front();
        }
        
        // Remove smaller elements from the back
        while (!window.empty() && nums[window.back()] < nums[i]) {
            window.pop_back();
        }
        
        window.push_back(i);
        
        // Add maximum of current window to result
        if (i >= k - 1) {
            result.push_back(nums[window.front()]);
        }
    }
    
    return result;
}

int main() {
    std::vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;
    
    std::vector<int> result = maxSlidingWindow(nums, k);
    
    std::cout << "Maximum sliding window: ";
    for (int num : result) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```


28. Perform in-place removal of all elements in a range [start, end) from a vector:

```cpp
#include <iostream>
#include <vector>

void removeRange(std::vector<int>& nums, int start, int end) {
    if (start >= end || start < 0 || end > nums.size()) {
        return;
    }
    
    int shift = end - start;
    for (int i = end; i < nums.size(); i++) {
        nums[i - shift] = nums[i];
    }
    
    nums.resize(nums.size() - shift);
}

int main() {
    std::vector<int> nums = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int start = 3;
    int end = 7;
    
    std::cout << "Original vector: ";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    removeRange(nums, start, end);
    
    std::cout << "Vector after removing range [" << start << ", " << end << "): ";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```


29. Find the longest bitonic subsequence in a vector:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int longestBitonicSubsequence(const std::vector<int>& nums) {
    int n = nums.size();
    std::vector<int> increasing(n, 1);
    std::vector<int> decreasing(n, 1);
    
    // Compute longest increasing subsequence
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[i] > nums[j]) {
                increasing[i] = std::max(increasing[i], increasing[j] + 1);
            }
        }
    }
    
    // Compute longest decreasing subsequence
    for (int i = n - 2; i >= 0; i--) {
        for (int j = n - 1; j > i; j--) {
            if (nums[i] > nums[j]) {
                decreasing[i] = std::max(decreasing[i], decreasing[j] + 1);
            }
        }
    }
    
    // Find the maximum sum of increasing and decreasing subsequences
    int maxLength = 0;
    for (int i = 0; i < n; i++) {
        maxLength = std::max(maxLength, increasing[i] + decreasing[i] - 1);
    }
    
    return maxLength;
}

int main() {
    std::vector<int> nums = {1, 11, 2, 10, 4, 5, 2, 1};
    
    int length = longestBitonicSubsequence(nums);
    
    std::cout << "Length of longest bitonic subsequence: " << length << std::endl;
    
    return 0;
}
```


Certainly! Here's the complete code for question 30:

30. Perform in-place merging of k sorted vectors into a single sorted vector:

```cpp
#include <iostream>
#include <vector>
#include <queue>

struct CompareVectors {
    bool operator()(const std::pair<int, std::pair<int, int>>& a, const std::pair<int, std::pair<int, int>>& b) {
        return a.first > b.first;
    }
};

std::vector<int> mergeKSortedVectors(std::vector<std::vector<int>>& vectors) {
    std::vector<int> result;
    // Priority queue to store the smallest element from each vector
    std::priority_queue<std::pair<int, std::pair<int, int>>, 
                        std::vector<std::pair<int, std::pair<int, int>>>, 
                        CompareVectors> pq;
    
    // Add the first element of each vector to the priority queue
    for (int i = 0; i < vectors.size(); i++) {
        if (!vectors[i].empty()) {
            pq.push({vectors[i][0], {i, 0}});
        }
    }
    
    while (!pq.empty()) {
        auto current = pq.top();
        pq.pop();
        
        int value = current.first;
        int vectorIndex = current.second.first;
        int elementIndex = current.second.second;
        
        result.push_back(value);
        
        // If there are more elements in the current vector, add the next one to the queue
        if (elementIndex + 1 < vectors[vectorIndex].size()) {
            pq.push({vectors[vectorIndex][elementIndex + 1], {vectorIndex, elementIndex + 1}});
        }
    }
    
    return result;
}

int main() {
    std::vector<std::vector<int>> vectors = {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9}
    };
    
    std::vector<int> merged = mergeKSortedVectors(vectors);
    
    std::cout << "Merged sorted vector: ";
    for (int num : merged) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

1. We define a `CompareVectors` struct to create a min-heap based on the first element of each pair.

2. The `mergeKSortedVectors` function takes a vector of vectors as input and returns a single sorted vector.

3. We use a priority queue to keep track of the smallest element from each vector. Each element in the queue is a pair containing the value and its position (vector index and element index).

4. We start by adding the first element of each vector to the priority queue.

5. We then repeatedly pop the smallest element from the queue, add it to the result vector, and add the next element from the same vector (if it exists) to the queue.

6. This process continues until the priority queue is empty, at which point all elements have been merged into the result vector.

7. In the `main` function, we create a sample input of three sorted vectors and demonstrate the merging process.
