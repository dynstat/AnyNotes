[https://blog.algomaster.io/p/15-leetcode-patterns](https://blog.algomaster.io/p/15-leetcode-patterns)
This information is based on and expanded from the [Algomaster.io blog post](https://blog.algomaster.io/p/15-leetcode-patterns).

## Overview
### 1. Sliding Window

**Explanation:**
The Sliding Window technique is used to perform operations on a specific window size of an array or string. As the window "slides" over the data, we process the data within the current window.

**Key Points:**
- Useful for problems involving arrays or strings where we need to find or calculate something among all subarrays or substrings of a certain size.
- Often used to reduce the time complexity from O(n²) to O(n).
- The window size can be fixed or variable, depending on the problem.

**When to use:**
- When dealing with contiguous sequences in an array or string.
- Problems asking for minimum, maximum, longest, shortest, or contained within a specific range.

**Example Problem: Find the maximum sum of a subarray of size k**

```python
def max_sum_subarray(arr, k):
    max_sum = 0
    window_sum = sum(arr[:k])
    max_sum = window_sum
    
    for i in range(k, len(arr)):
        window_sum = window_sum - arr[i-k] + arr[i]
        max_sum = max(max_sum, window_sum)
    
    return max_sum
```

**Explanation of the solution:**
1. Initialize the first window sum by summing the first k elements.
2. Slide the window by one element at a time:
   - Subtract the first element of the previous window.
   - Add the last element of the new window.
3. Keep track of the maximum sum encountered.

### 2. Two Pointers

**Explanation:**
The Two Pointers technique uses two pointers to iterate through the data structure in a single pass. The pointers can move towards each other or in the same direction.

**Key Points:**
- Often used to search pairs in a sorted array or linked list.
- Can be used to compare values at different positions.
- Helps reduce time complexity, often from O(n²) to O(n).

**When to use:**
- Problems involving reversing, searching pairs, or partitioning.
- When dealing with sorted arrays or linked lists.

**Example Problem: Check if a string is a palindrome**

```python
def is_palindrome(s):
    left, right = 0, len(s) - 1
    while left < right:
        if s[left] != s[right]:
            return False
        left += 1
        right -= 1
    return True
```

**Explanation of the solution:**
1. Initialize two pointers: one at the start and one at the end of the string.
2. Move the pointers towards each other, comparing characters.
3. If at any point the characters don't match, it's not a palindrome.
4. If the pointers meet or cross without finding a mismatch, it's a palindrome.

### 3. Fast and Slow Pointers

**Explanation:**
This technique, also known as the "Hare and Tortoise" algorithm, uses two pointers moving at different speeds through a sequence.

**Key Points:**
- Useful for cycle detection in linked lists or arrays.
- Can be used to find the middle element of a linked list.
- Helps solve problems with O(1) space complexity.

**When to use:**
- Cycle detection problems.
- Finding the middle element of a linked list.
- Problems involving repeated numbers in an array.

**Example Problem: Detect a cycle in a linked list**

```python
class ListNode:
    def __init__(self, x):
        self.val = x
        self.next = None

def has_cycle(head):
    if not head or not head.next:
        return False
    
    slow = head
    fast = head.next
    
    while slow != fast:
        if not fast or not fast.next:
            return False
        slow = slow.next
        fast = fast.next.next
    
    return True
```

**Explanation of the solution:**
1. Initialize two pointers: slow (moving one step at a time) and fast (moving two steps at a time).
2. If there's a cycle, the fast pointer will eventually catch up to the slow pointer.
3. If there's no cycle, the fast pointer will reach the end of the list.

### 4. Merge Intervals

**Explanation:**
This pattern deals with problems involving overlapping intervals. It typically involves sorting the intervals and then merging or processing them.

**Key Points:**
- Usually requires sorting the intervals by start time.
- Helps in solving scheduling or time-based problems.
- Often involves comparing the end of one interval with the start of the next.

**When to use:**
- Problems involving time intervals or ranges.
- Scheduling problems.
- When you need to find overlapping intervals or merge them.

**Example Problem: Merge overlapping intervals**

```python
def merge_intervals(intervals):
    intervals.sort(key=lambda x: x[0])
    merged = []
    for interval in intervals:
        if not merged or merged[-1][1] < interval[0]:
            merged.append(interval)
        else:
            merged[-1][1] = max(merged[-1][1], interval[1])
    return merged
```

**Explanation of the solution:**
1. Sort the intervals based on start time.
2. Iterate through the sorted intervals:
   - If the current interval doesn't overlap with the previous one, add it to the result.
   - If it overlaps, merge it with the previous interval by updating the end time.

### 5. Cyclic Sort

**Explanation:**
Cyclic Sort is used when dealing with arrays containing numbers in a given range. It works by placing each number in its correct position.

**Key Points:**
- Useful for problems where array elements are in the range 1 to n.
- Helps solve problems with O(n) time complexity and O(1) space complexity.
- Often used to find missing numbers or duplicates in an array.

**When to use:**
- When the problem involves an array containing numbers in a given range.
- Finding missing numbers or duplicates in an array.

**Example Problem: Find the missing number in an array containing n distinct numbers in the range [0, n]**

```python
def find_missing_number(nums):
    i = 0
    while i < len(nums):
        if nums[i] < len(nums) and nums[i] != i:
            nums[nums[i]], nums[i] = nums[i], nums[nums[i]]
        else:
            i += 1
    
    for i in range(len(nums)):
        if nums[i] != i:
            return i
    return len(nums)
```

**Explanation of the solution:**
1. Iterate through the array, placing each number in its correct position (index equal to its value).
2. After sorting, iterate again to find the index where the value doesn't match the index.
3. That index is the missing number.

### 6. In-place Reversal of a Linked List

**Explanation:**
This pattern is about reversing the links between nodes of a linked list, often without using extra space.

**Key Points:**
- Helps solve problems that require modification of the linked list structure.
- Usually involves changing the next pointers of the nodes.
- Can be used to solve problems like reversing a sub-list or rotating a linked list.

**When to use:**
- When you need to reverse a linked list or a part of it.
- Problems involving rearranging linked list nodes.

**Example Problem: Reverse a linked list**

```python
class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val
        self.next = next

def reverse_list(head):
    prev = None
    current = head
    
    while current:
        next_temp = current.next
        current.next = prev
        prev = current
        current = next_temp
    
    return prev
```

**Explanation of the solution:**
1. Use three pointers: prev, current, and next_temp.
2. Iterate through the list, for each node:
   - Save the next node.
   - Reverse the link to point to the previous node.
   - Move prev and current one step forward.
3. At the end, prev will be pointing to the new head of the reversed list.

### 7. Tree Breadth-First Search (BFS)

**Explanation:**
Tree BFS involves traversing a tree level by level, usually using a queue to keep track of nodes at each level.

**Key Points:**
- Uses a queue data structure to process nodes in level order.
- Useful for problems requiring level-wise processing of tree nodes.
- Can help find the shortest path in an unweighted tree.

**When to use:**
- When you need to traverse a tree level by level.
- Finding the minimum depth of a binary tree.
- Problems involving the closest nodes to the root.

**Example Problem: Level order traversal of a binary tree**

```python
from collections import deque

class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

def level_order_traversal(root):
    if not root:
        return []
    
    result = []
    queue = deque([root])
    
    while queue:
        level_size = len(queue)
        current_level = []
        
        for _ in range(level_size):
            node = queue.popleft()
            current_level.append(node.val)
            
            if node.left:
                queue.append(node.left)
            if node.right:
                queue.append(node.right)
        
        result.append(current_level)
    
    return result
```

**Explanation of the solution:**
1. Use a queue to keep track of nodes at each level.
2. Process all nodes at the current level before moving to the next level.
3. For each node, add its children to the queue for future processing.
4. Keep track of each level's nodes in a separate list.

### 8. Tree Depth-First Search (DFS)

**Explanation:**
Tree DFS involves exploring as far as possible along each branch before backtracking. It can be implemented using recursion or a stack.

**Key Points:**
- Can be implemented in three ways: preorder, inorder, or postorder traversal.
- Uses less memory than BFS for very wide trees.
- Useful for problems requiring exploring paths to leaves or specific node values.

**When to use:**
- When you need to search for nodes far from the root.
- Problems involving path finding or summing values along a path.
- When the tree is very wide (BFS might use too much memory).

**Example Problem: Preorder traversal of a binary tree**

```python
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

def preorder_traversal(root):
    result = []
    
    def dfs(node):
        if not node:
            return
        
        result.append(node.val)
        dfs(node.left)
        dfs(node.right)
    
    dfs(root)
    return result
```

**Explanation of the solution:**
1. Use a recursive helper function to perform DFS.
2. Process the current node first (preorder).
3. Recursively process the left subtree.
4. Recursively process the right subtree.
5. The result list will contain nodes in preorder traversal.

### 9. Two Heaps

**Explanation:**
The Two Heaps pattern uses two heaps (usually a max-heap and a min-heap) to solve problems requiring tracking of the median of a data stream or finding a balance point.

**Key Points:**
- Useful for problems involving finding the median or balancing two halves of data.
- Typically uses a max-heap for the lower half and a min-heap for the upper half of the data.
- Helps in efficiently finding the median or middle elements.

**When to use:**
- Problems involving finding the median of a stream of numbers.
- When you need to balance two halves of data.
- Scheduling problems where you need to find a balance point.

**Example Problem: Find the median of a data stream**

```python
import heapq

class MedianFinder:
    def __init__(self):
        self.small = []  # max heap
        self.large = []  # min heap

    def addNum(self, num: int) -> None:
        if len(self.small) == len(self.large):
            heapq.heappush(self.large, -heapq.heappushpop(self.small, -num))
        else:
            heapq.heappush(self.small, -heapq.heappushpop(self.large, num))

    def findMedian(self) -> float:
        if len(self.small) == len(self.large):
            return (-self.small[0] + self.large[0]) / 2.0
        else:
            return float(self.large[0])
```

**Explanation of the solution:**
1. Use two heaps: a max-heap for the lower half and a min-heap for the upper half.
2. When adding a number, balance the heaps so that their sizes differ by at most 1.
3. To find the median:
   - If the heaps are equal in size, the median is the average of their top elements.
   - If not, the median is the top element of the larger heap.

### 10. Subsets

**Explanation:**
The Subsets pattern deals with problems involving finding all possible combinations or permutations of a set of elements.

**Key Points:**
- Often solved using backtracking or bit manipulation.
- Useful for generating all possible combinations or permutations.
- The number of subsets is usually 2^n for n elements.

**When to use:**
- When you need to generate all possible subsets or combinations.
- Problems involving permutations.
- When dealing with problems that require exploring all possibilities.

**Example Problem: Generate all subsets of a set**

```python
def subsets(nums):
    result = []
    
    def backtrack(start, path):
        result.append(path[:])
        for i in range(start, len(nums)):
            path.append(nums[i])
            backtrack(i + 1, path)
            path.pop()
    
    backtrack(0, [])
    return result
```

**Explanation of the solution:**
1. Use a backtracking approach to generate all subsets.
2. Start with an empty subset and progressively add elements.
3. For each element, we have two choices: include it or not.
4. Recursively generate subsets for the remaining elements.
5. Use a path variable to keep track of the current subset being built.

### 11. Modified Binary Search

**Explanation:**
This pattern involves applying binary search to problems where the traditional binary search might not directly apply, often requiring modifications to the standard algorithm.

**Key Points:**
- Useful for searching in sorted arrays with some modifications.
- Can be applied to problems where the search space is not a straightforward sorted array.
- Often involves identifying a different condition for the binary search.

**When to use:**
- Searching in a sorted array with some property (e.g., rotated sorted array).
- Finding a specific element that satisfies certain conditions in a sorted structure.
- When the problem hints at a O(log n) solution.

**Example Problem: Find the rotation count in a rotated sorted array**

```python
def find_rotation_count(arr):
    left, right = 0, len(arr) - 1
    
    while left <= right:
        if arr[left] <= arr[right]:  # Array is already sorted
            return left
        
        mid = (left + right) // 2
        next = (mid + 1) % len(arr)
        prev = (mid - 1 + len(arr)) % len(arr)
        
        if arr[mid] <= arr[next] and arr[mid] <= arr[prev]:
            return mid
        elif arr[mid] <= arr[right]:
            right = mid - 1
        elif arr[left] <= arr[mid]:
            left = mid + 1
    
    return 0
```

**Explanation of the solution:**
1. Use binary search to find the pivot point (smallest element).
2. Compare the middle element with its neighbors to check if it's the pivot.
3. If not, decide which half to continue searching based on the sorted half.
4. The index of the pivot is the rotation count.

### 12. Top K Elements

**Explanation:**
This pattern is used to find the top K elements among a collection. It often involves using a heap data structure to efficiently keep track of the K elements of interest.

**Key Points:**
- Typically uses a min-heap or max-heap to maintain the top K elements.
- Can solve problems with O(N log K) time complexity, where N is the total number of elements.
- Useful for problems involving sorting or finding a subset of elements with certain properties.

**When to use:**
- When you need to find the K largest or smallest elements in a collection.
- Problems involving partial sorting.
- When dealing with streams of data and need to maintain top K elements dynamically.

**Example Problem: Find the K most frequent elements**

```python
from collections import Counter
import heapq

def top_k_frequent(nums, k):
    # Count the frequency of each number
    count = Counter(nums)
    
    # Use a min heap to keep the K most frequent elements
    heap = []
    for num, freq in count.items():
        heapq.heappush(heap, (freq, num))
        if len(heap) > k:
            heapq.heappop(heap)
    
    # Extract the K most frequent elements from the heap
    return [num for freq, num in heap]
```

**Explanation of the solution:**
1. Use a Counter to count the frequency of each number.
2. Use a min-heap to keep track of the K most frequent elements.
3. If the heap size exceeds K, remove the least frequent element.
4. The heap will contain the K most frequent elements at the end.

### 13. K-way Merge

**Explanation:**
This pattern is used to solve problems that involve merging K sorted arrays or lists. It typically uses a heap to efficiently perform the merge operation.

**Key Points:**
- Often uses a min-heap to keep track of the smallest element from each list.
- Useful for problems involving merging multiple sorted structures.
- Can achieve O(N log K) time complexity, where N is the total number of elements and K is the number of lists.

**When to use:**
- When you need to merge K sorted arrays or lists.
- Problems involving finding the smallest or largest element among K sorted structures.
- When dealing with streams of sorted data from multiple sources.

**Example Problem: Merge K sorted lists**

```python
import heapq

class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val
        self.next = next

def merge_k_lists(lists):
    # Create a min-heap with the first node of each list
    heap = [(head.val, i, head) for i, head in enumerate(lists) if head]
    heapq.heapify(heap)
    
    dummy = ListNode(0)
    current = dummy
    
    while heap:
        val, i, node = heapq.heappop(heap)
        current.next = node
        current = current.next
        
        if node.next:
            heapq.heappush(heap, (node.next.val, i, node.next))
    
    return dummy.next
```

**Explanation of the solution:**
1. Create a min-heap with the first node from each list.
2. Repeatedly pop the smallest node from the heap and add it to the result list.
3. If the popped node has a next node, push that next node onto the heap.
4. Continue until the heap is empty.

### 14. Topological Sort

**Explanation:**
Topological Sort is used to order the vertices of a directed acyclic graph (DAG) such that for every directed edge (u, v), vertex u comes before v in the ordering. It's often used in scheduling problems or dependency resolution.

**Key Points:**
- Can be implemented using DFS or BFS (Kahn's algorithm).
- Useful for problems involving dependencies or prerequisites.
- Can detect cycles in directed graphs.

**When to use:**
- When dealing with problems involving dependencies or prerequisites.
- Scheduling problems where certain tasks must be completed before others.
- When you need to find a valid ordering of elements based on their dependencies.

**Example Problem: Course Schedule (determine if it's possible to finish all courses)**

```python
from collections import defaultdict, deque

def can_finish(num_courses, prerequisites):
    # Create a graph and in-degree count for each course
    graph = defaultdict(list)
    in_degree = [0] * num_courses
    
    for course, prereq in prerequisites:
        graph[prereq].append(course)
        in_degree[course] += 1
    
    # Initialize queue with all courses having no prerequisites
    queue = deque([i for i in range(num_courses) if in_degree[i] == 0])
    
    courses_taken = 0
    while queue:
        course = queue.popleft()
        courses_taken += 1
        
        for next_course in graph[course]:
            in_degree[next_course] -= 1
            if in_degree[next_course] == 0:
                queue.append(next_course)
    
    return courses_taken == num_courses
```

**Explanation of the solution:**
1. Create a graph representation of the course dependencies.
2. Keep track of the in-degree (number of prerequisites) for each course.
3. Start with courses that have no prerequisites.
4. As each course is "taken", decrement the in-degree of its dependent courses.
5. If all courses can be taken, it's possible to finish the schedule.

### 15. Trie

**Explanation:**
A Trie (prefix tree) is a tree-like data structure used to store and search strings efficiently. It's particularly useful for problems involving string prefixes or word dictionaries.

**Key Points:**
- Efficient for insert and search operations, especially for prefix matching.
- Uses less space compared to storing each string separately when there are many common prefixes.
- Useful for autocomplete features, spell checkers, and IP routing tables.

**When to use:**
- When dealing with problems involving string prefixes.
- Implementing features like autocomplete or spell check.
- When you need to efficiently store and retrieve strings from a large set.

**Example Problem: Implement a Trie (Prefix Tree)**

```python
class TrieNode:
    def __init__(self):
        self.children = {}
        self.is_end_of_word = False

class Trie:
    def __init__(self):
        self.root = TrieNode()
    
    def insert(self, word: str) -> None:
        node = self.root
        for char in word:
            if char not in node.children:
                node.children[char] = TrieNode()
            node = node.children[char]
        node.is_end_of_word = True
    
    def search(self, word: str) -> bool:
        node = self.root
        for char in word:
            if char not in node.children:
                return False
            node = node.children[char]
        return node.is_end_of_word
    
    def starts_with(self, prefix: str) -> bool:
        node = self.root
        for char in prefix:
            if char not in node.children:
                return False
            node = node.children[char]
        return True
```

**Explanation of the solution:**
1. Each TrieNode represents a character and has children nodes for subsequent characters.
2. The `insert` method adds a word to the trie, creating new nodes as necessary.
3. The `search` method checks if a word exists in the trie, ensuring it's a complete word.
4. The `starts_with` method checks if any word in the trie starts with the given prefix.

These 15 patterns cover a wide range of common algorithmic problems. Understanding and practicing these patterns can significantly improve your problem-solving skills for coding interviews and competitive programming. Remember that many real-world problems may involve combinations of these patterns or variations of them.


# Explanations 

Certainly! I'll provide detailed explanations, coding examples, and comments for each of the 15 LeetCode patterns. This comprehensive guide is based on the [Algomaster.io blog post](https://blog.algomaster.io/p/15-leetcode-patterns).

## 1. Prefix Sum

**Detailed Explanation:**
- Prefix Sum is a technique where we precompute cumulative sums of an array.
- It allows for constant-time range sum queries.
- The prefix sum array P is constructed such that P[i] = sum of elements from index 0 to i in the original array.
- To find the sum of elements from index i to j, we can use: P[j] - P[i-1] (or P[j] - P[i] + A[i] if i > 0).

**Coding Example: Range Sum Query - Immutable**

```python
class NumArray:
    def __init__(self, nums: List[int]):
        # Initialize the prefix sum array
        self.prefix_sum = [0] * (len(nums) + 1)
        
        # Compute the prefix sum
        for i in range(1, len(self.prefix_sum)):
            self.prefix_sum[i] = self.prefix_sum[i-1] + nums[i-1]

    def sumRange(self, left: int, right: int) -> int:
        # Return the sum of elements from index left to right (inclusive)
        return self.prefix_sum[right + 1] - self.prefix_sum[left]

# Usage
numArray = NumArray([-2, 0, 3, -5, 2, -1])
print(numArray.sumRange(0, 2))  # Output: 1
print(numArray.sumRange(2, 5))  # Output: -1
print(numArray.sumRange(0, 5))  # Output: -3
```

**Detailed Comments:**
```python
class NumArray:
    def __init__(self, nums: List[int]):
        # Initialize the prefix sum array with an extra 0 at the beginning
        # This simplifies our calculations by avoiding edge cases for the first element
        self.prefix_sum = [0] * (len(nums) + 1)
        
        # Compute the prefix sum
        # prefix_sum[i] represents the sum of all elements from index 0 to i-1 in the original array
        for i in range(1, len(self.prefix_sum)):
            self.prefix_sum[i] = self.prefix_sum[i-1] + nums[i-1]

    def sumRange(self, left: int, right: int) -> int:
        # To get the sum from left to right, we subtract the sum up to 'left-1' from the sum up to 'right'
        # We use 'right + 1' because our prefix_sum array is 1-indexed relative to the original array
        return self.prefix_sum[right + 1] - self.prefix_sum[left]
```

## 2. Two Pointers

**Detailed Explanation:**
- The Two Pointers technique uses two pointers to iterate through a data structure.
- Often used with arrays or linked lists, especially when they are sorted.
- Can be used to search for pairs with certain properties, or to partition arrays.
- Common variations include:
  1. Opposite-direction pointers (one at start, one at end)
  2. Same-direction pointers (slow and fast pointers)
  3. Pointers on different arrays or linked lists

**Coding Example: Two Sum II - Input Array is Sorted**

```python
class Solution:
    def twoSum(self, numbers: List[int], target: int) -> List[int]:
        left, right = 0, len(numbers) - 1
        
        while left < right:
            current_sum = numbers[left] + numbers[right]
            
            if current_sum == target:
                # Found the pair, return their 1-indexed positions
                return [left + 1, right + 1]
            elif current_sum < target:
                # Sum is too small, move left pointer to increase the sum
                left += 1
            else:
                # Sum is too large, move right pointer to decrease the sum
                right -= 1
        
        # If we reach here, no solution was found
        return []  # or raise an exception

# Usage
solution = Solution()
print(solution.twoSum([2, 7, 11, 15], 9))  # Output: [1, 2]
```

**Detailed Comments:**
```python
class Solution:
    def twoSum(self, numbers: List[int], target: int) -> List[int]:
        # Initialize two pointers: left at the start and right at the end of the array
        left, right = 0, len(numbers) - 1
        
        while left < right:
            # Calculate the sum of elements at left and right pointers
            current_sum = numbers[left] + numbers[right]
            
            if current_sum == target:
                # If the sum equals the target, we've found our pair
                # Return the indices (1-indexed as per problem requirement)
                return [left + 1, right + 1]
            elif current_sum < target:
                # If the sum is too small, we need a larger sum
                # Move the left pointer to the right to increase the sum
                left += 1
            else:
                # If the sum is too large, we need a smaller sum
                # Move the right pointer to the left to decrease the sum
                right -= 1
        
        # If we've exhausted all possibilities without finding a solution
        return []  # or raise an exception to indicate no solution found
```

## 3. Sliding Window

**Detailed Explanation:**
- The Sliding Window technique is used to process data in a specific window size.
- It's particularly useful for problems involving arrays or strings where we need to find or calculate something among all subarrays or substrings of a certain size.
- Key aspects:
  1. The window size can be fixed or variable.
  2. As we slide the window, we add new elements and remove old ones.
  3. We maintain some data structure to keep track of the window's state.
- Often reduces the time complexity from O(n²) to O(n).

**Coding Example: Maximum Sum Subarray of Size K**

```python
def max_sum_subarray(arr: List[int], k: int) -> int:
    if len(arr) < k:
        return None  # or raise an exception
    
    # Calculate the sum of the first window
    window_sum = sum(arr[:k])
    max_sum = window_sum
    
    # Slide the window and update the max_sum
    for i in range(k, len(arr)):
        # Add the new element and remove the first element of the previous window
        window_sum = window_sum - arr[i-k] + arr[i]
        max_sum = max(max_sum, window_sum)
    
    return max_sum

# Usage
print(max_sum_subarray([1, 4, 2, 10, 23, 3, 1, 0, 20], 4))  # Output: 39
```

**Detailed Comments:**
```python
def max_sum_subarray(arr: List[int], k: int) -> int:
    if len(arr) < k:
        return None  # or raise an exception if the array is smaller than the window size
    
    # Calculate the sum of the first window
    window_sum = sum(arr[:k])
    max_sum = window_sum
    
    # Slide the window and update the max_sum
    for i in range(k, len(arr)):
        # Slide the window:
        # - Subtract the element going out of the window (arr[i-k])
        # - Add the new element coming into the window (arr[i])
        window_sum = window_sum - arr[i-k] + arr[i]
        
        # Update max_sum if the current window_sum is greater
        max_sum = max(max_sum, window_sum)
    
    return max_sum
```

## 4. Fast & Slow Pointers

**Detailed Explanation:**
- The Fast & Slow Pointers technique, also known as the "Tortoise and Hare" algorithm, uses two pointers moving at different speeds.
- It's particularly useful for:
  1. Detecting cycles in a linked list or array.
  2. Finding the middle element of a linked list.
  3. Finding the kth element from the end in a linked list.
- Key aspects:
  1. The slow pointer moves one step at a time.
  2. The fast pointer moves two steps at a time.
  3. If there's a cycle, the fast pointer will eventually catch up to the slow pointer.

**Coding Example: Detect Cycle in a Linked List**

```python
class ListNode:
    def __init__(self, x):
        self.val = x
        self.next = None

class Solution:
    def hasCycle(self, head: ListNode) -> bool:
        if not head or not head.next:
            return False
        
        slow = head
        fast = head.next
        
        while slow != fast:
            if not fast or not fast.next:
                return False
            slow = slow.next
            fast = fast.next.next
        
        return True

# Usage
# Create a linked list with a cycle
head = ListNode(3)
head.next = ListNode(2)
head.next.next = ListNode(0)
head.next.next.next = ListNode(-4)
head.next.next.next.next = head.next  # Create cycle

solution = Solution()
print(solution.hasCycle(head))  # Output: True
```

**Detailed Comments:**
```python
class Solution:
    def hasCycle(self, head: ListNode) -> bool:
        # If the list is empty or has only one node, there can't be a cycle
        if not head or not head.next:
            return False
        
        # Initialize slow pointer to the head
        slow = head
        # Initialize fast pointer to the second node
        fast = head.next
        
        # Continue until the pointers meet or we reach the end of the list
        while slow != fast:
            # If we reach the end of the list, there's no cycle
            if not fast or not fast.next:
                return False
            
            # Move slow pointer one step
            slow = slow.next
            # Move fast pointer two steps
            fast = fast.next.next
        
        # If we exit the loop, it means the pointers met, indicating a cycle
        return True
```

## 5. LinkedList In-place Reversal

**Detailed Explanation:**
- The LinkedList In-place Reversal pattern is used to reverse a linked list or parts of it without using extra space.
- Key aspects:
  1. We use three pointers: prev, current, and next.
  2. We iterate through the list, reversing the links as we go.
  3. This can be done iteratively or recursively.
- Useful for problems involving:
  1. Reversing an entire linked list.
  2. Reversing a sublist within a linked list.
  3. Reordering a linked list.

**Coding Example: Reverse a Linked List**

```python
class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val
        self.next = next

class Solution:
    def reverseList(self, head: ListNode) -> ListNode:
        prev = None
        current = head
        
        while current:
            next_temp = current.next
            current.next = prev
            prev = current
            current = next_temp
        
        return prev

# Usage
# Create a linked list: 1 -> 2 -> 3 -> 4 -> 5
head = ListNode(1)
head.next = ListNode(2)
head.next.next = ListNode(3)
head.next.next.next = ListNode(4)
head.next.next.next.next = ListNode(5)

solution = Solution()
reversed_head = solution.reverseList(head)

# Print the reversed list
while reversed_head:
    print(reversed_head.val, end=" -> ")
    reversed_head = reversed_head.next
print("None")
# Output: 5 -> 4 -> 3 -> 2 -> 1 -> None
```

**Detailed Comments:**
```python
class Solution:
    def reverseList(self, head: ListNode) -> ListNode:
        # Initialize three pointers
        prev = None  # Will be the new tail (None) of the reversed list
        current = head  # Current node we're processing
        
        while current:
            # Store the next node before we change current.next
            next_temp = current.next
            
            # Reverse the link
            current.next = prev
            
            # Move prev and current one step forward
            prev = current
            current = next_temp
        
        # At the end, prev will be pointing to the new head of the reversed list
        return prev
```

## 6. Monotonic Stack

**Detailed Explanation:**
- A Monotonic Stack is a stack that maintains elements in a strictly increasing or decreasing order.
- It's particularly useful for problems involving finding the next greater/smaller element or maintaining a certain order of elements.
- Key aspects:
  1. Elements are pushed onto the stack in a way that maintains the monotonic property.
  2. Elements are popped from the stack when they violate the monotonic property.
  3. The stack can be either increasing or decreasing, depending on the problem.
- Common applications:
  1. Finding the next greater/smaller element.
  2. Solving histogram-related problems.
  3. Maintaining a sliding window minimum/maximum.

**Coding Example: Next Greater Element**

```python
class Solution:
    def nextGreaterElements(self, nums: List[int]) -> List[int]:
        n = len(nums)
        result = [-1] * n
        stack = []
        
        # Iterate through the array twice to handle circular array
        for i in range(2 * n):
            while stack and nums[stack[-1]] < nums[i % n]:
                result[stack.pop()] = nums[i % n]
            if i < n:
                stack.append(i)
        
        return result

# Usage
solution = Solution()
print(solution.nextGreaterElements([1, 2, 1]))  # Output: [2, -1, 2]
```

**Detailed Comments:**
```python
class Solution:
    def nextGreaterElements(self, nums: List[int]) -> List[int]:
        n = len(nums)
        # Initialize result array with -1 (default when no greater element exists)
        result = [-1] * n
        # Stack to store indices of elements
        stack = []
        
        # Iterate through the array twice to handle circular array
        for i in range(2 * n):
            # While stack is not empty and current element is greater than the element at stack top
            while stack and nums[stack[-1]] < nums[i % n]:
                # Pop index from stack and update its next greater element
                result[stack.pop()] = nums[i % n]
            # Push current index onto stack if it's in the first n elements
            if i < n:
                stack.append(i)
        
        return result
```


## 7. Top 'K' Elements (continued)

```python
import heapq

class Solution:
    def findKLargest(self, nums: List[int], k: int) -> List[int]:
        # Create a min heap
        min_heap = []
        
        for num in nums:
            # If the heap size is less than k, just add the element
            if len(min_heap) < k:
                heapq.heappush(min_heap, num)
            # If the current number is larger than the smallest in the heap,
            # remove the smallest and add the current number
            elif num > min_heap[0]:
                heapq.heappop(min_heap)
                heapq.heappush(min_heap, num)
        
        # Convert heap to a list and return
        return list(min_heap)

# Usage
solution = Solution()
print(solution.findKLargest([3,2,1,5,6,4], 2))  # Output: [5, 6]
```

**Detailed Comments:**
```python
import heapq

class Solution:
    def findKLargest(self, nums: List[int], k: int) -> List[int]:
        # Create a min heap to store the k largest elements
        min_heap = []
        
        for num in nums:
            # If the heap size is less than k, just add the element
            if len(min_heap) < k:
                heapq.heappush(min_heap, num)
            # If the current number is larger than the smallest in the heap,
            # remove the smallest and add the current number
            elif num > min_heap[0]:
                # Remove the smallest element
                heapq.heappop(min_heap)
                # Add the current number
                heapq.heappush(min_heap, num)
        
        # At this point, min_heap contains the k largest elements
        # Convert heap to a list and return
        return list(min_heap)
```

## 8. Overlapping Intervals

**Detailed Explanation:**
- The Overlapping Intervals pattern is used to deal with interval-based problems.
- It's particularly useful for merging intervals, finding overlaps, or scheduling problems.
- Key aspects:
  1. Sort intervals based on start times.
  2. Iterate through sorted intervals and merge or process as needed.
  3. Keep track of the last processed interval for comparison.
- Common applications include merging meetings, finding free time slots, or determining if a person can attend all meetings.

**Coding Example: Merge Intervals**

```python
class Solution:
    def mergeIntervals(self, intervals: List[List[int]]) -> List[List[int]]:
        # Sort intervals based on start time
        intervals.sort(key=lambda x: x[0])
        
        merged = []
        for interval in intervals:
            # If merged is empty or current interval does not overlap with previous
            if not merged or merged[-1][1] < interval[0]:
                merged.append(interval)
            else:
                # There is an overlap, so we merge the current and previous intervals
                merged[-1][1] = max(merged[-1][1], interval[1])
        
        return merged

# Usage
solution = Solution()
print(solution.mergeIntervals([[1,3],[2,6],[8,10],[15,18]]))  # Output: [[1,6],[8,10],[15,18]]
```

**Detailed Comments:**
```python
class Solution:
    def mergeIntervals(self, intervals: List[List[int]]) -> List[List[int]]:
        # Sort intervals based on start time
        intervals.sort(key=lambda x: x[0])
        
        merged = []
        for interval in intervals:
            # If merged is empty or current interval does not overlap with previous
            if not merged or merged[-1][1] < interval[0]:
                merged.append(interval)
            else:
                # There is an overlap, so we merge the current and previous intervals
                # We take the max of the end times to ensure we cover the entire range
                merged[-1][1] = max(merged[-1][1], interval[1])
        
        return merged
```

## 9. Modified Binary Search

**Detailed Explanation:**
- Modified Binary Search is an adaptation of the classic binary search algorithm to solve more complex problems.
- It's used when the standard binary search needs to be tweaked to fit specific problem requirements.
- Key aspects:
  1. The search space is typically sorted or partially sorted.
  2. The condition for narrowing the search space may be more complex than a simple comparison.
  3. It often involves finding a specific element that satisfies certain conditions.
- Common applications include searching in rotated sorted arrays, finding peak elements, or searching in a range.

**Coding Example: Search in Rotated Sorted Array**

```python
class Solution:
    def search(self, nums: List[int], target: int) -> int:
        left, right = 0, len(nums) - 1
        
        while left <= right:
            mid = (left + right) // 2
            
            if nums[mid] == target:
                return mid
            
            # Check if the left half is sorted
            if nums[left] <= nums[mid]:
                if nums[left] <= target < nums[mid]:
                    right = mid - 1
                else:
                    left = mid + 1
            # Right half is sorted
            else:
                if nums[mid] < target <= nums[right]:
                    left = mid + 1
                else:
                    right = mid - 1
        
        return -1  # Target not found

# Usage
solution = Solution()
print(solution.search([4,5,6,7,0,1,2], 0))  # Output: 4
```

**Detailed Comments:**
```python
class Solution:
    def search(self, nums: List[int], target: int) -> int:
        left, right = 0, len(nums) - 1
        
        while left <= right:
            mid = (left + right) // 2
            
            if nums[mid] == target:
                return mid
            
            # Check if the left half is sorted
            if nums[left] <= nums[mid]:
                # If target is in the left sorted half
                if nums[left] <= target < nums[mid]:
                    right = mid - 1
                else:
                    left = mid + 1
            # Right half is sorted
            else:
                # If target is in the right sorted half
                if nums[mid] < target <= nums[right]:
                    left = mid + 1
                else:
                    right = mid - 1
        
        return -1  # Target not found
```

## 10. Binary Tree Traversal

**Detailed Explanation:**
- Binary Tree Traversal involves visiting all nodes in a binary tree in a specific order.
- There are three main types of depth-first traversals:
  1. Preorder (Root, Left, Right)
  2. Inorder (Left, Root, Right)
  3. Postorder (Left, Right, Root)
- Additionally, level-order traversal (breadth-first) is also common.
- These traversals can be implemented recursively or iteratively (using a stack or queue).
- Understanding these traversals is crucial for solving many tree-related problems.

**Coding Example: Inorder Traversal**

```python
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

class Solution:
    def inorderTraversal(self, root: TreeNode) -> List[int]:
        result = []
        stack = []
        current = root
        
        while current or stack:
            # Reach the leftmost node of the current subtree
            while current:
                stack.append(current)
                current = current.left
            
            # Process the node
            current = stack.pop()
            result.append(current.val)
            
            # Move to the right subtree
            current = current.right
        
        return result

# Usage
# Create a binary tree: 1 -> 2 -> 3
root = TreeNode(1)
root.right = TreeNode(2)
root.right.left = TreeNode(3)

solution = Solution()
print(solution.inorderTraversal(root))  # Output: [1, 3, 2]
```

**Detailed Comments:**
```python
class Solution:
    def inorderTraversal(self, root: TreeNode) -> List[int]:
        result = []
        stack = []
        current = root
        
        while current or stack:
            # Reach the leftmost node of the current subtree
            while current:
                stack.append(current)
                current = current.left
            
            # Process the node
            current = stack.pop()
            result.append(current.val)
            
            # Move to the right subtree
            current = current.right
        
        return result
```


## 11. Depth-First Search (DFS)

**Detailed Explanation:**
- Depth-First Search (DFS) is a graph traversal algorithm that explores as far as possible along each branch before backtracking.
- Key aspects:
  1. Can be implemented recursively or using a stack for iterative approach.
  2. Useful for problems involving path finding, cycle detection, and topological sorting.
  3. Time complexity is typically O(V + E) where V is the number of vertices and E is the number of edges.
- Common applications include maze solving, finding connected components, and detecting cycles in graphs.

**Coding Example: DFS to Find Path in a Graph**

```python
from typing import List, Dict

class Solution:
    def findPath(self, graph: Dict[int, List[int]], start: int, end: int) -> List[int]:
        path = []
        visited = set()

        def dfs(node):
            visited.add(node)
            path.append(node)

            if node == end:
                return True

            for neighbor in graph[node]:
                if neighbor not in visited:
                    if dfs(neighbor):
                        return True

            path.pop()
            return False

        dfs(start)
        return path if path[-1] == end else []

# Usage
graph = {
    0: [1, 2],
    1: [2],
    2: [3],
    3: []
}
solution = Solution()
print(solution.findPath(graph, 0, 3))  # Output: [0, 1, 2, 3]
```

**Detailed Comments:**
```python
class Solution:
    def findPath(self, graph: Dict[int, List[int]], start: int, end: int) -> List[int]:
        path = []
        visited = set()

        def dfs(node):
            # Mark the current node as visited and add to path
            visited.add(node)
            path.append(node)

            # If we've reached the end, we're done
            if node == end:
                return True

            # Explore all neighbors
            for neighbor in graph[node]:
                if neighbor not in visited:
                    # If we find a path to the end through this neighbor, we're done
                    if dfs(neighbor):
                        return True

            # If we didn't find a path, backtrack
            path.pop()
            return False

        # Start DFS from the start node
        dfs(start)
        # Return the path if we reached the end, otherwise return an empty list
        return path if path[-1] == end else []
```

## 12. Breadth-First Search (BFS)

**Detailed Explanation:**
- Breadth-First Search (BFS) is a graph traversal algorithm that explores all vertices at the present depth before moving to vertices at the next depth level.
- Key aspects:
  1. Implemented using a queue data structure.
  2. Useful for finding the shortest path in unweighted graphs.
  3. Time complexity is O(V + E) where V is the number of vertices and E is the number of edges.
- Common applications include level-order traversal of trees, finding shortest paths, and social networking features like "friends of friends".

**Coding Example: BFS to Find Shortest Path in a Graph**

```python
from collections import deque
from typing import List, Dict

class Solution:
    def shortestPath(self, graph: Dict[int, List[int]], start: int, end: int) -> List[int]:
        queue = deque([[start]])
        visited = set([start])

        while queue:
            path = queue.popleft()
            node = path[-1]

            if node == end:
                return path

            for neighbor in graph[node]:
                if neighbor not in visited:
                    visited.add(neighbor)
                    new_path = list(path)
                    new_path.append(neighbor)
                    queue.append(new_path)

        return []  # No path found

# Usage
graph = {
    0: [1, 2],
    1: [2, 3],
    2: [3],
    3: []
}
solution = Solution()
print(solution.shortestPath(graph, 0, 3))  # Output: [0, 1, 3]
```

**Detailed Comments:**
```python
class Solution:
    def shortestPath(self, graph: Dict[int, List[int]], start: int, end: int) -> List[int]:
        # Initialize queue with the start node path
        queue = deque([[start]])
        # Keep track of visited nodes to avoid cycles
        visited = set([start])

        while queue:
            # Get the next path to explore
            path = queue.popleft()
            node = path[-1]

            # If we've reached the end, we've found the shortest path
            if node == end:
                return path

            # Explore all neighbors
            for neighbor in graph[node]:
                if neighbor not in visited:
                    visited.add(neighbor)
                    # Create a new path including this neighbor
                    new_path = list(path)
                    new_path.append(neighbor)
                    queue.append(new_path)

        return []  # No path found
```

## 13. Matrix Traversal

**Detailed Explanation:**
- Matrix Traversal involves navigating through a 2D grid or matrix, often used in problems related to game boards, image processing, or grid-based pathfinding.
- Key aspects:
  1. Can be implemented using DFS, BFS, or other specialized algorithms.
  2. Often involves checking neighboring cells (usually 4 or 8 directions).
  3. Common techniques include flood fill, island counting, and path finding.
- Time complexity is typically O(m * n) where m and n are the dimensions of the matrix.

**Coding Example: Number of Islands**

```python
from typing import List

class Solution:
    def numIslands(self, grid: List[List[str]]) -> int:
        if not grid:
            return 0

        count = 0
        for i in range(len(grid)):
            for j in range(len(grid[0])):
                if grid[i][j] == '1':
                    self.dfs(grid, i, j)
                    count += 1
        return count

    def dfs(self, grid, i, j):
        if i < 0 or j < 0 or i >= len(grid) or j >= len(grid[0]) or grid[i][j] != '1':
            return
        grid[i][j] = '0'  # Mark as visited
        self.dfs(grid, i+1, j)
        self.dfs(grid, i-1, j)
        self.dfs(grid, i, j+1)
        self.dfs(grid, i, j-1)

# Usage
grid = [
    ["1","1","0","0","0"],
    ["1","1","0","0","0"],
    ["0","0","1","0","0"],
    ["0","0","0","1","1"]
]
solution = Solution()
print(solution.numIslands(grid))  # Output: 3
```

**Detailed Comments:**
```python
class Solution:
    def numIslands(self, grid: List[List[str]]) -> int:
        if not grid:
            return 0

        count = 0
        for i in range(len(grid)):
            for j in range(len(grid[0])):
                if grid[i][j] == '1':
                    self.dfs(grid, i, j)
                    count += 1  # Increment island count
        return count

    def dfs(self, grid, i, j):
        # Check if we're out of bounds or if this cell is not land
        if i < 0 or j < 0 or i >= len(grid) or j >= len(grid[0]) or grid[i][j] != '1':
            return
        grid[i][j] = '0'  # Mark as visited by changing to water
        # Recursively check all adjacent cells
        self.dfs(grid, i+1, j)
        self.dfs(grid, i-1, j)
        self.dfs(grid, i, j+1)
        self.dfs(grid, i, j-1)
```

## 14. Backtracking

**Detailed Explanation:**
- Backtracking is an algorithmic technique for solving problems recursively by trying to build a solution incrementally, one piece at a time, removing those solutions that fail to satisfy the constraints of the problem at any point of time.
- Key aspects:
  1. It's a form of recursion.
  2. The solution is built incrementally.
  3. If a partial solution fails to meet the constraints, it's abandoned (backtracked).
- Common applications include solving puzzles (e.g., sudoku), generating permutations and combinations, and solving the N-Queens problem.

**Coding Example: Generate All Permutations**

```python
from typing import List

class Solution:
    def permute(self, nums: List[int]) -> List[List[int]]:
        def backtrack(start):
            if start == len(nums):
                result.append(nums[:])
                return
            
            for i in range(start, len(nums)):
                nums[start], nums[i] = nums[i], nums[start]  # Swap
                backtrack(start + 1)
                nums[start], nums[i] = nums[i], nums[start]  # Swap back

        result = []
        backtrack(0)
        return result

# Usage
solution = Solution()
print(solution.permute([1,2,3]))
# Output: [[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,2,1],[3,1,2]]
```

**Detailed Comments:**
```python
class Solution:
    def permute(self, nums: List[int]) -> List[List[int]]:
        def backtrack(start):
            # If we've reached the end of the array, we have a complete permutation
            if start == len(nums):
                result.append(nums[:])  # Add a copy of the current permutation
                return
            
            for i in range(start, len(nums)):
                # Swap the current element with the start element
                nums[start], nums[i] = nums[i], nums[start]
                # Recursively generate permutations for the rest of the array
                backtrack(start + 1)
                # Backtrack: swap back to restore the original order
                nums[start], nums[i] = nums[i], nums[start]

        result = []
        backtrack(0)
        return result
```

## 15. Dynamic Programming Patterns

**Detailed Explanation:**
- Dynamic Programming (DP) is a method for solving complex problems by breaking them down into simpler subproblems.
- Key aspects:
  1. Optimal substructure: An optimal solution can be constructed from optimal solutions of its subproblems.
  2. Overlapping subproblems: The same subproblems are solved multiple times.
  3. Memoization or tabulation is used to store solutions to subproblems.
- Common DP patterns include:
  1. 0/1 Knapsack
  2. Unbounded Knapsack
  3. Longest Common Subsequence
  4. Longest Increasing Subsequence
  5. Fibonacci Sequence

**Coding Example: Fibonacci Sequence using DP**

```python
class Solution:
    def fibonacci(self, n: int) -> int:
        if n <= 1:
            return n
        
        dp = [0] * (n + 1)
        dp[1] = 1
        
        for i in range(2, n + 1):
            dp[i] = dp[i-1] + dp[i-2]
        
        return dp[n]

# Usage
solution = Solution()
print(solution.fibonacci(10))  # Output: 55
```

**Detailed Comments:**
```python
class Solution:
    def fibonacci(self, n: int) -> int:
        # Base cases
        if n <= 1:
            return n
        
        # Initialize DP array
        dp = [0] * (n + 1)
        dp[1] = 1  # First Fibonacci number
        
        # Build up the Fibonacci sequence
        for i in range(2, n + 1):
            # Each number is the sum of the two preceding ones
            dp[i] = dp[i-1] + dp[i-2]
        
        # Return the nth Fibonacci number
        return dp[n]
```

These 15 patterns cover a wide range of common algorithmic problems encountered in coding interviews and competitive programming. Understanding and practicing these patterns can significantly improve your problem-solving skills. Remember that many real-world problems may involve combinations of these patterns or variations of them. Regular practice and application of these patterns to diverse problems will help solidify your understanding and improve your coding abilities.