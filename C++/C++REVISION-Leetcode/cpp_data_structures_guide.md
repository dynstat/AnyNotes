# Comprehensive Guide to C++ Data Structures and STL

This guide provides detailed information about C++ data structures, including the Standard Template Library (STL), with explanations of their syntax, methods, parameters, return types, and practical code examples.

## Table of Contents

1. [Basic Data Structures](basic_data_structures.md)
   - Arrays (Static & Dynamic)
   - C-Style Strings
   - Structures
   - Unions

2. [STL Sequence Containers](stl_sequence_containers.md)
   - std::vector
   - std::array
   - std::deque
   - std::list
   - std::forward_list

3. [STL Associative Containers](stl_associative_containers.md)
   - std::set
   - std::multiset
   - std::map
   - std::multimap

4. [STL Unordered Associative Containers](stl_unordered_associative_containers.md)
   - std::unordered_set
   - std::unordered_multiset
   - std::unordered_map
   - std::unordered_multimap
   - Hash Function Customization

5. [STL Container Adaptors](stl_container_adaptors.md)
   - std::stack
   - std::queue
   - std::priority_queue

6. [STL Iterators](stl_iterators.md)
   - Iterator Categories
   - Iterator Operations
   - Custom Iterators
   - Iterator Invalidation

7. [STL Algorithms](stl_algorithms.md)
   - Non-Modifying Sequence Operations
   - Modifying Sequence Operations
   - Sorting Operations
   - Set Operations
   - Numeric Operations
   - Parallel Algorithms (C++17)

8. [Other Important C++ Data Structures](other_important_cpp_data_structures.md)
   - Smart Pointers
   - Pair and Tuple
   - Optional, Variant, and Any (C++17)
   - String View (C++17)

## How to Use This Guide

Each section contains:

1. **Detailed Descriptions**: Comprehensive explanations of each data structure
2. **Template Parameters**: Complete listing of template parameters with their purposes
3. **Member Function Tables**: Tables listing all methods with their parameters, return types, and time complexity
4. **Code Examples**: Practical examples demonstrating usage of each data structure
5. **Performance Considerations**: Insights into efficiency and best practices

Whether you're a beginner learning about C++ data structures or an experienced programmer looking for a reference, this guide provides all the information you need to effectively use C++ data structures in your projects.

## Key Concepts

### Container Selection Guidelines

When choosing a container for your C++ application, consider these factors:

1. **Access Patterns**: How will you access the data?
   - Random access → vector, array
   - Sequential access → list, forward_list
   - Key-based access → map, unordered_map

2. **Insertion/Removal Patterns**: Where will elements be added or removed?
   - Beginning/end only → deque, stack, queue
   - Arbitrary positions → list, forward_list
   - Maintaining order → set, map

3. **Memory Constraints**: How important is memory efficiency?
   - Contiguous memory → vector, array
   - Node-based → list, map
   - Overhead concerns → array, forward_list

4. **Performance Requirements**: What operations need to be fast?
   - Fast lookup → unordered_map, unordered_set
   - Fast insertion → list, unordered_map
   - Fast iteration → vector, array
   - Fast sorting → vector (with std::sort)

### Common Container Properties

| Container | Element Order | Iterator Stability | Random Access | Memory Layout | Lookup | Insertion |
|-----------|---------------|-------------------|---------------|---------------|--------|-----------|
| vector | Insertion | ❌ | ✅ | Contiguous | O(n) | O(1) amortized at end |
| array | Insertion | ✅ | ✅ | Contiguous | O(n) | Fixed size |
| deque | Insertion | ❌ | ✅ | Segmented | O(n) | O(1) at ends |
| list | Insertion | ✅ | ❌ | Linked | O(n) | O(1) anywhere |
| forward_list | Insertion | ✅ | ❌ | Linked | O(n) | O(1) after position |
| set, map | Sorted | ✅ | ❌ | Tree | O(log n) | O(log n) |
| unordered_set, unordered_map | Hashed | ❌ | ❌ | Hash table | O(1) avg | O(1) avg |
| stack, queue, priority_queue | Special | N/A | N/A | Adapter | N/A | Special |

### Best Practices Summary

1. **Prefer standard containers** over custom implementations
2. **Prefer contiguous containers** (vector, array) for performance when possible
3. **Use reserve()** to avoid reallocations with vector
4. **Prefer emplace() over push()** for in-place construction
5. **Be mindful of iterator invalidation** rules
6. **Use the right algorithms** with containers for maximum efficiency
7. **Consider move semantics** when working with containers of expensive objects
8. **Understand the strengths and weaknesses** of each container type
9. **Profile your application** to identify bottlenecks before optimizing
10. **Consider C++17/20 additions** like string_view, span, and parallel algorithms