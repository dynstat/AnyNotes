Below are two parts as requested:

---
## Part 1: Very Brief Summary

- **Sliding Window:** Expand/contract a window with two indices over the string/array to maintain a desired property in _O(n)_.
    
- **Prefix-Sum + Hash Map:** Keep cumulative sums and use a map to count how often a needed “complement” has appeared → _O(n)_.
    
- **Two Pointers:** For sorted arrays, move left/right pointers inward to count or find target-sum pairs in _O(n)_.
    
- **Dynamic Programming:** Build a 2D table `dp[i][j]` to answer questions (e.g. palindromes) in _O(n²)_.
    
- **Suffix Structures / Binary-Search + Hashing / Bitmasking:** More advanced, for fast substring queries or counting distinct substrings in near-linear time.
    

---

## Part 2: Detailed ASCII Diagrams with Arrows & Pointers

### 1. Sliding-Window (Longest unique-char substring)

**String**: `"abcaeb"`  
Track `start`→, `end`→, and a `seen` set.

```
Indices:   0   1   2   3   4   5
String:    a   b   c   a   e   b

Initialize:
 start=0, end=–1, seen={}
 max_len=0

Step 1: move end → 0
      start         end
         ↓           ↓
 window = [● _ _ _ _ _]
 seen={a}
 max_len = 1

Step 2: move end → 1
      start           end
         ↓             ↓
 window = [● ● _ _ _ _]
 seen={a,b}
 max_len = 2

Step 3: move end → 2
	  start             end
		 ↓               ↓
 window = [● ● ● _ _ _]
 seen={a,b,c}
 max_len = 3

Step 4: move end → 3 (char='a', duplicate!)
      start               end
         ↓                 ↓
 window before shrink: [● ● ● ● _ _]
                      indices 0–3

 → duplicate `'a'` at end; shrink by moving start until no duplicate:
   * remove s[0]='a'
   start → 1
   seen={b,c}

      new window:        [ _ ● ● ● _ _]
                         ^     ^
                        1     3

 → now add new 'a':
 seen={b,c,a}
 window indices 1–3
 max_len stays 3

Step 5: move end → 4 ('e')
      start               end
         ↓                 ↓
 window = [ _ ● ● ● ● _]
           (b c a e)
 seen={b,c,a,e}
 size=4 → max_len=4

Step 6: move end → 5 ('b', duplicate)
      start                 end
         ↓                   ↓
 window before shrink: [ _ ● ● ● ● ●]
                     indices 1–5

 → remove s[1]='b' → start=2, seen={c,a,e}
      [ _ _ ● ● ● ●]
         ^       ^
        2        5

 → add 'b':
      [ _ _ ● ● ● ●]
 gives window (c,a,e,b) indices 2–5
 size=4 → max_len=4

→ **Result:** max unique-substring = `"caeb"`, length 4.
```

---

### 2. Prefix-Sum + Hash Map (Count subarrays summing to k)

**Array**: `[1,2,–1,2,1]`, **k = 3**

```
i   num   prefix_sum   need=prefix_sum–k   freq map         count
–1        0            —                   {0:1}            0

0    1     1            1–3=–2             {0:1}            0
                                → freq[–2]=0
                                    add freq[1]=1

1    2     3            3–3=0              {0:1,1:1}       ↑1
                                → freq[0]=1 → count=1
                                    add freq[3]=1

2   –1     2            2–3=–1             {...,3:1}        1
                                → freq[–1]=0
                                    add freq[2]=1

3    2     4            4–3=1              {...,2:1}       ↑2
                                → freq[1]=1 → count=2
                                    add freq[4]=1

4    1     5            5–3=2              {...,4:1}       ↑3
                                → freq[2]=1 → count=3
                                    add freq[5]=1

→ **Total count = 3**  
Subarrays: [1,2], [2,–1,2], [–1,2,1].
```

---

### 3. Two-Pointers (Count pairs ≤ target in sorted array)

**Array**: `[1,2,3,4,5]`, **target**=6

```
 arr = [1, 2, 3, 4, 5]
 left → 0, right → 4, count = 0

Step A:
 left=0 →1, right=4→5
   sum = 1+5 =6 ≤6
   ⇒ all indices between left and right with left fixed:
      count += (right – left) = 4  (pairs: 1+5,1+4,1+3,1+2)
   then left→1

Step B:
 left=1→2, right=4→5
   sum = 2+5 =7 >6 → right→3

Step C:
 left=1→2, right=3→4
   sum = 2+4 =6 ≤6
   ⇒ count += (3–1)=2  (pairs: 2+4,2+3)
   then left→2

Step D:
 left=2→3, right=3→4
   sum = 3+4 =7 >6 → right→2

Now left==right → stop.

→ **Total count = 4 + 2 = 6 pairs.**
```

---

### 4. Dynamic Programming (Palindromic Substrings)

**String**: `"ababa"` (length 5)  
Build `dp[i][j]` table where `True` means `s[i..j]` is palindrome.

```
   i\j  0   1   2   3   4    ← j
       'a' 'b' 'a' 'b' 'a'
i
0 'a'  [•]  [ ]  [ ]  [ ]  [ ]
1 'b'       [•]  [ ]  [ ]  [ ]
2 'a'           [•]  [ ]  [ ]
3 'b'                [•]  [ ]
4 'a'                     [•]

Legend: • = True, blank = False

Step L=1 (length=1): every single char is a palindrome → fill diagonal:
 count = 5

Step L=2 (length=2):
 check dp[i][i+1]:
  (0,1)="ab"? no
  (1,2)="ba"? no
  (2,3)="ab"? no
  (3,4)="ba"? no
 no new trues → count=5

Step L=3 (length=3):
 check (0,2): "aba" → s[0]==s[2] & dp[1][1]=True → dp[0][2]=True → count=6  
 ASCII pointer:
   i=0         j=2
   'a'—'b'—'a'
    ↑         ↑
 dp[1][1] used → expands

 check (1,3): "bab" → dp[1][3]=True → count=7  
 check (2,4): "aba" → dp[2][4]=True → count=8

Step L=4 (length=4):
 (0,3)="abab"? s[0]!=s[3] → False  
 (1,4)="baba"? s[1]!=s[4] → False

Step L=5 (length=5):
 (0,4)="ababa"? s[0]==s[4] & dp[1][3]=True → dp[0][4]=True → count=9  

→ **Total palindromic substrings = 9.**  
List: a, b, a, b, a, aba, bab, aba, ababa.
```

---

# AGAIN - Fixed diagrams
Let's fix and reformat the **ASCII diagrams** for **Subarrays and Substrings**, with perfect alignment, arrows, and step-by-step visuals. This will help you **visualize sliding windows, prefix sums, two pointers, and dynamic programming (DP)** easily.

---

## ✅ PART 1: Substrings – Sliding Window (Longest Unique Characters)

**Problem**: Find the longest substring without repeating characters.  
**Input**: `"abcaeb"`

### 🔁 Sliding Window with ASCII Diagram

```text
String:    a   b   c   a   e   b
Indices:   0   1   2   3   4   5

Step 1:
start → 0
end   → 0     [a]
seen = {a}
----------------------------
Window:     a
            ↑start=end

Step 2:
end   → 1     [a b]
seen = {a, b}
----------------------------
Window:     a   b
            ↑   ↑
          start end

Step 3:
end   → 2     [a b c]
seen = {a, b, c}
----------------------------
Window:     a   b   c
            ↑       ↑
          start     end

Step 4:
end   → 3     [a b c a] → 'a' is repeated
→ Slide start until 'a' is removed
→ Remove s[start=0]='a', start++
→ seen = {b, c}
→ Now add 'a' again → seen = {b, c, a}
----------------------------
Window:         b   c   a
                ↑       ↑
              start     end

Step 5:
end   → 4     [b c a e]
seen = {b, c, a, e}
----------------------------
Window:         b   c   a   e
                ↑           ↑
              start         end

Step 6:
end   → 5     [b c a e b] → 'b' is repeated
→ Remove s[start=1]='b', start++
→ seen = {c, a, e}
→ Now add 'b' → seen = {c, a, e, b}
----------------------------
Window:             c   a   e   b
                    ↑           ↑
                  start         end

🎯 Final Result: Longest = 4 → "caeb"
```

---

## ✅ PART 2: Subarrays – Prefix Sum + HashMap (Count Sum = k)

**Problem**: Count number of subarrays whose sum = `k=3`  
**Input**: `[1, 2, -1, 2, 1]`

### 🧮 Prefix Sum Diagram

```text
Index:     -1   0   1   2   3   4
Array:          1   2  -1   2   1
PrefixSum:  0   1   3   2   4   5

Goal: Find how many times (prefix_sum - k) has occurred

Step-by-step:
i=0 → prefix=1 → 1–3 = –2 not found → freq = {0:1, 1:1}
-----------------------------------------------------
i=1 → prefix=3 → 3–3 =  0 found (1x) → count += 1
                ↑     ↑
              target  found in map
→ freq = {0:1, 1:1, 3:1}

i=2 → prefix=2 → 2–3 = –1 not found → freq = {..., 2:1}
i=3 → prefix=4 → 4–3 =  1 found (1x) → count += 1
i=4 → prefix=5 → 5–3 =  2 found (1x) → count += 1

🎯 Final Result: Count = 3
Subarrays:
  [1, 2]
  [2, -1, 2]
  [-1, 2, 1]
```

---

## ✅ PART 3: Subarrays – Two Pointers (Pairs with sum ≤ target)

**Problem**: Count valid pairs with sum ≤ `k = 6`  
**Input**: `[1, 2, 3, 4, 5]` (Sorted)

### 🔁 Two Pointers Diagram

```text
Array:       1   2   3   4   5
             ↑               ↑
           left            right

Step 1: 1+5=6 → valid → count += right–left = 4
→ Pairs: (1,2), (1,3), (1,4), (1,5)
→ move left++

Array:       1   2   3   4   5
                 ↑           ↑
               left        right

Step 2: 2+5=7 → too large → move right--

Array:       1   2   3   4   5
                 ↑       ↑
               left    right

Step 3: 2+4=6 → valid → count += 2 → move left++

Array:       1   2   3   4   5
                     ↑   ↑
                   left right

Step 4: 3+4=7 → too large → right--

Now left==right → done.

🎯 Final Result: 6 pairs
```

---

## ✅ PART 4: Substrings – DP Table (Count Palindromes)

**Problem**: Count palindromic substrings  
**Input**: `"ababa"`

### 🧾 DP Matrix Diagram

```text
    j→   0   1   2   3   4
        [a] [b] [a] [b] [a]
i↓
 0 a    T   F   T   F   T
 1 b        T   F   T   F
 2 a            T   F   T
 3 b                T   F
 4 a                    T

T = palindrome substring
Fill Rules:
- dp[i][i] = True (single char)
- dp[i][j] = True if s[i]==s[j] and dp[i+1][j-1] is True

Steps:
- len=1 → a,b,a,b,a → 5 palindromes
- len=3 → "aba", "bab", "aba" → +3
- len=5 → "ababa" → +1

🎯 Total = 9 palindromic substrings
```

---

## Summary Table

|Technique|Use Case|Time Complexity|Key Idea|
|---|---|---|---|
|Sliding Window|Longest substring (unique)|O(n)|Move `start/end` to maintain condition|
|Prefix Sum + Hash|Subarray sums = k|O(n)|Map prev sums to find complements|
|Two Pointers|Sorted array, pair sums|O(n)|Narrow range from both ends|
|Dynamic Programming|Palindromic substrings|O(n²)|Build from small substrings|

---

# CODE (in py, C++ and Rust)

Here's the code with detailed comments:

---

## 🔸 1. Longest Substring Without Repeating Characters (Sliding Window)

This technique efficiently finds the longest substring within a given string that does not contain any repeating characters. It uses a "sliding window" approach, where a window (representing the current substring) expands from the right and shrinks from the left when a repeating character is encountered.

### ✅ Python Code:

Python

```python
def longest_unique_substring(s: str) -> int:
    seen = set()              # set: stores unique characters in the current window
    left = 0                  # int: start index of the window
    max_len = 0               # int: maximum length found so far

    # Iterate through the string with 'right' as the end of the window
    for right in range(len(s)):  # right: int, current index / end of window
        # While the character at s[right] is already in our 'seen' set (i.e., it's a repeat)
        while s[right] in seen:
            # Remove the character at the 'left' of the window from 'seen'
            seen.remove(s[left]) # s[left]: char, character being removed
            # Shrink the window from the left
            left += 1
        # Add the current character s[right] to our 'seen' set
        seen.add(s[right]) # s[right]: char, character being added
        # Update max_len with the maximum of its current value and the size of the current window
        max_len = max(max_len, right - left + 1) # (right - left + 1): int, current window size
    return max_len

print(longest_unique_substring("abcaeb"))  # Output: 4 ("bcae")
```

### ✅ C++ Code:

C++

```c++
#include <iostream>
#include <unordered_set> // For unordered_set
#include <string>        // For string
#include <algorithm>     // For std::max

// using namespace std; // It's generally better to qualify names with std::

// Function to find the length of the longest substring without repeating characters
// s: const std::string& (input string, passed by const reference)
// Returns: int (length of the longest unique substring)
int longestUniqueSubstring(const std::string& s) {
    // unordered_set<char>: stores unique characters in the current window
    std::unordered_set<char> seen;
    int left = 0;      // int: start index of the window
    int maxLen = 0;    // int: maximum length found so far

    // Iterate through the string with 'right' as the end of the window
    // s.length(): returns size_t (unsigned integer type), but int is fine for typical string lengths
    for (int right = 0; right < s.length(); ++right) { // right: int, current index / end of window
        // s[right]: char, current character being checked

        // while (seen.find(s[right]) != seen.end())
        // seen.find(s[right]): searches for s[right] in the set.
        //   Input: char (value to search for)
        //   Returns: iterator to the element if found, or seen.end() if not found.
        // seen.end(): returns an iterator to the element following the last element.
        while (seen.count(s[right])) { // .count() is often more idiomatic for checking existence
                                       // seen.count(s[right]): checks if s[right] is in the set
                                       //   Input: char (value to count)
                                       //   Returns: size_type (1 if present, 0 if not)
            // seen.erase(s[left]): removes s[left] from the set.
            //   Input: char (value to remove)
            //   Returns: size_type (number of elements removed, typically 1 or 0)
            seen.erase(s[left]); // s[left]: char, character being removed from window start
            ++left; // Shrink the window from the left
        }
        // seen.insert(s[right]): inserts s[right] into the set.
        //   Input: char (value to insert)
        //   Returns: std::pair<iterator, bool> (iterator to inserted/existing element, bool indicating if insertion took place)
        seen.insert(s[right]); // s[right]: char, character being added to window end

        // maxLen = std::max(maxLen, right - left + 1);
        // std::max: returns the larger of two values.
        //   Input: int, int
        //   Returns: int
        maxLen = std::max(maxLen, right - left + 1); // (right - left + 1): int, current window size
    }
    return maxLen; // int: the maximum length found
}

int main() {
    // std::string: a sequence of characters
    std::string test_str = "abcaeb";
    // std::cout: standard output stream object
    // <<: stream insertion operator
    // std::endl: manipulator to insert a newline and flush the stream
    std::cout << longestUniqueSubstring(test_str) << std::endl; // Output: 4
    return 0; // int: indicates successful execution
}
```

### ✅ Rust Code:

Rust

```rust
use std::collections::HashSet; // To use HashSet for storing unique characters
use std::cmp::max;            // To use the max function

// Function to find the length of the longest substring without repeating characters
// s: &str (input string slice)
// Returns: usize (length of the longest unique substring, usize is an unsigned integer type for sizes/indices)
fn longest_unique_substring(s: &str) -> usize {
    // seen: HashSet<char>: stores unique characters in the current window
    let mut seen: HashSet<char> = HashSet::new();
    // left: usize: start index of the window
    // max_len: usize: maximum length found so far
    let (mut left, mut max_len) = (0, 0);

    // chars: Vec<char>: Rust strings are UTF-8 encoded and not directly indexable by integer.
    // s.chars(): returns an iterator over the characters of the string slice.
    //   Input: (self)
    //   Returns: std::str::Chars (an iterator)
    // .collect(): consumes the iterator and collects elements into a collection, here a Vec<char>.
    //   Input: (self)
    //   Returns: Vec<char> (a vector of characters)
    let chars: Vec<char> = s.chars().collect();

    // Iterate through the character vector with 'right' as the end of the window
    // 0..chars.len(): creates a range from 0 up to (but not including) chars.len()
    // chars.len(): returns the number of elements in the vector.
    //   Input: (self)
    //   Returns: usize
    for right in 0..chars.len() { // right: usize, current index / end of window
        // chars[right]: char, current character being checked

        // while seen.contains(&chars[right])
        // seen.contains(&key): checks if the set contains the given key.
        //   Input: &char (a reference to the character to check)
        //   Returns: bool (true if present, false otherwise)
        while seen.contains(&chars[right]) {
            // seen.remove(&chars[left]): removes the character from the set.
            //   Input: &char (a reference to the character to remove)
            //   Returns: bool (true if the value was present and removed, false otherwise)
            seen.remove(&chars[left]); // chars[left]: char, character being removed from window start
            left += 1; // Shrink the window from the left (left: usize)
        }
        // seen.insert(chars[right]): inserts the character into the set.
        //   Input: char (the character to insert)
        //   Returns: bool (true if the value was not present and inserted, false if already present)
        seen.insert(chars[right]); // chars[right]: char, character being added to window end

        // max_len = max(max_len, right - left + 1);
        // max: returns the greater of two values.
        //   Input: usize, usize
        //   Returns: usize
        max_len = max(max_len, right - left + 1); // (right - left + 1): usize, current window size
    }
    max_len // usize: the maximum length found
}

fn main() {
    // test_str: &str (string slice)
    let test_str = "abcaeb";
    // println!: a macro for printing to the console
    // {}: placeholder for the value of longest_unique_substring(test_str)
    println!("{}", longest_unique_substring(test_str)); // Output: 4
}
```

---

## 🔸 2. Count Subarrays With Sum = k (Prefix Sum + HashMap)

This technique counts the number of contiguous subarrays whose elements sum up to a target value `k`. It leverages the concept of prefix sums (the sum of elements from the start of the array up to the current position) and a hash map to efficiently track frequencies of these prefix sums. The core idea is that if `prefix_sum[i] - prefix_sum[j] == k`, then the subarray `arr[j+1...i]` sums to `k`. This can be rewritten as `prefix_sum[j] == prefix_sum[i] - k`.

### ✅ Python Code:

Python

```python
from collections import defaultdict

# arr: list[int] (input list of integers)
# k: int (target sum)
# Returns: int (count of subarrays summing to k)
def count_subarrays_sum_k(arr: list[int], k: int) -> int:
    prefix_sum = 0            # int: current cumulative sum from the start of the array
    count = 0                 # int: total count of subarrays found
    # freq: defaultdict(int): stores the frequency of each prefix_sum encountered.
    # If a prefix_sum is accessed that isn't yet a key, it defaults to 0.
    freq = defaultdict(int)
    freq[0] = 1               # Initialize with prefix_sum 0 having a frequency of 1
                              # (to handle subarrays starting from the beginning)

    # Iterate through each number in the input array
    for num in arr:           # num: int, current number from the array
        prefix_sum += num     # Update the current prefix_sum

        # If (prefix_sum - k) exists in freq, it means there was a previous point
        # where the sum was 'prefix_sum - k'. The elements between that point
        # and the current position sum up to 'k'.
        # freq[prefix_sum - k]: int, number of times (prefix_sum - k) has been seen
        count += freq[prefix_sum - k]

        # Increment the frequency of the current prefix_sum
        freq[prefix_sum] += 1
    return count

print(count_subarrays_sum_k([1, 2, -1, 2, 1], 3))  # Output: 3
# Subarrays: [1, 2], [2, -1, 2], [2, 1]
```

### ✅ C++ Code:

C++

```c++
#include <iostream>
#include <unordered_map> // For unordered_map
#include <vector>        // For vector

// using namespace std; // Generally better to qualify

// Function to count subarrays with a sum equal to k
// nums: const std::vector<int>& (input vector of integers, passed by const reference)
// k: int (target sum)
// Returns: int (count of subarrays summing to k)
int countSubarraysSumK(const std::vector<int>& nums, int k) {
    // freq: std::unordered_map<int, int>: stores the frequency of each prefix_sum.
    // Key: int (prefix_sum), Value: int (frequency)
    std::unordered_map<int, int> freq;
    freq[0] = 1; // Initialize with prefix_sum 0 having a frequency of 1

    int prefix_sum = 0; // int: current cumulative sum
    int count = 0;      // int: total count of subarrays found

    // Iterate through each number in the input vector
    // const int& num: iterating by const reference to avoid copying (for simple int, copy is fine too)
    for (int num : nums) { // num: int, current number from the vector
        prefix_sum += num; // Update the current prefix_sum

        // freq.count(prefix_sum - k): checks if (prefix_sum - k) is a key in the map.
        //   Input: int (key to search for)
        //   Returns: size_type (1 if key exists, 0 otherwise)
        if (freq.count(prefix_sum - k)) {
            // freq.at(prefix_sum - k): accesses the value associated with the key (prefix_sum - k).
            //   Input: int (key)
            //   Returns: int& (reference to the value)
            //   Throws std::out_of_range if key not found, so count check is important.
            // Alternatively, freq[prefix_sum - k] can be used if we are sure it exists or if default construction is okay.
            count += freq.at(prefix_sum - k);
        }
        // ++freq[prefix_sum]: increments the frequency of the current prefix_sum.
        // If prefix_sum is not in map, it's inserted with value 0, then incremented to 1.
        //   freq[key] returns a reference to the value.
        ++freq[prefix_sum];
    }
    return count; // int: the total count
}

int main() {
    // nums: std::vector<int>: a dynamic array of integers
    std::vector<int> nums = {1, 2, -1, 2, 1};
    // k_val: int: the target sum
    int k_val = 3;
    std::cout << countSubarraysSumK(nums, k_val) << std::endl; // Output: 3
    return 0; // int: indicates successful execution
}
```

### ✅ Rust Code:

Rust

```rust
use std::collections::HashMap; // To use HashMap for storing prefix sum frequencies

// Function to count subarrays with a sum equal to k
// nums: &[i32] (input slice of 32-bit integers)
// k: i32 (target sum, a 32-bit integer)
// Returns: i32 (count of subarrays summing to k)
fn count_subarrays_sum_k(nums: &[i32], k: i32) -> i32 {
    // freq: HashMap<i32, i32>: stores the frequency of each prefix_sum.
    // Key: i32 (prefix_sum), Value: i32 (frequency)
    let mut freq: HashMap<i32, i32> = HashMap::new();
    // freq.insert(key, value): inserts a key-value pair into the map.
    //   Input: i32, i32
    //   Returns: Option<i32> (the old value if the key was present, otherwise None)
    freq.insert(0, 1); // Initialize with prefix_sum 0 having a frequency of 1

    // prefix_sum: i32: current cumulative sum
    // count: i32: total count of subarrays found
    let (mut prefix_sum, mut count) = (0, 0);

    // Iterate through each number in the input slice
    // &num: takes a reference to each element in nums. num will be of type &i32.
    // *num dereferences it to get the i32 value.
    for &num in nums { // num: i32, current number from the slice
        prefix_sum += num; // Update the current prefix_sum (prefix_sum: i32)

        // freq.get(&(prefix_sum - k)): attempts to get a reference to the value associated with key (prefix_sum - k).
        //   Input: &i32 (reference to the key)
        //   Returns: Option<&i32> (Some(&value) if key exists, None otherwise)
        // .unwrap_or(&0): if `get` returns Some(&value), it unwraps to &value. If None, it returns &0 (a reference to a temporary 0).
        //   Input: &i32 (default value reference)
        //   Returns: &i32
        // The outer `*` dereferences this `&i32` to get an `i32` value.
        count += *freq.get(&(prefix_sum - k)).unwrap_or(&0); // (prefix_sum - k): i32

        // freq.entry(prefix_sum): gets an Entry enum for the key `prefix_sum`.
        //   Input: i32 (key)
        //   Returns: std::collections::hash_map::Entry<i32, i32>
        // .or_insert(0): if the entry is vacant, inserts 0 and returns a mutable reference to it.
        //                If occupied, returns a mutable reference to the existing value.
        //   Input: i32 (default value to insert if vacant)
        //   Returns: &mut i32 (mutable reference to the value in the map)
        // The outer `*` dereferences this `&mut i32` to get an `i32` (for reading, though here it's part of `+=`)
        // and then `+= 1` increments the value in the map.
        *freq.entry(prefix_sum).or_insert(0) += 1;
    }
    count // i32: the total count
}

fn main() {
    // nums: array of i32, which will be passed as a slice &[i32]
    let nums = [1, 2, -1, 2, 1];
    // k_val: i32: the target sum
    let k_val = 3;
    println!("{}", count_subarrays_sum_k(&nums, k_val)); // Output: 3
}
```

---

## 🔸 3. Count Pairs with Sum ≤ k (Two Pointers on Sorted Array)

This technique efficiently counts pairs of elements in a sorted array whose sum is less than or equal to a target value k. It uses two pointers, one starting at the beginning (left) and the other at the end (right) of the array.

If arr[left] + arr[right] <= k, then all pairs (arr[left], arr[left+1]), (arr[left], arr[left+2]), ..., (arr[left], arr[right]) satisfy the condition. There are right - left such pairs. We then move left one step to the right to find new pairs.

If arr[left] + arr[right] > k, the sum is too large, so we move right one step to the left to try and decrease the sum.

### ✅ Python Code:

Python

```python
# arr: list[int] (input list of integers)
# k: int (target sum limit)
# Returns: int (count of pairs (arr[i], arr[j]) with i < j such that arr[i] + arr[j] <= k)
def count_pairs_le_k(arr: list[int], k: int) -> int:
    # arr.sort(): sorts the list in place. Time complexity O(n log n).
    arr.sort()
    left = 0                  # int: pointer to the beginning of the array
    right = len(arr) - 1      # int: pointer to the end of the array
    count = 0                 # int: total count of pairs found

    # Loop as long as the left pointer is to the left of the right pointer
    while left < right:
        current_sum = arr[left] + arr[right] # int: sum of elements at current pointers
        if current_sum <= k:
            # If arr[left] + arr[right] <= k, then arr[left] can be paired with
            # arr[left+1], arr[left+2], ..., arr[right].
            # There are (right - left) such elements.
            count += (right - left) # Add the number of valid pairs found with arr[left]
            left += 1               # Move the left pointer to the right to find new pairs
        else:
            # If arr[left] + arr[right] > k, the sum is too large.
            # To decrease the sum, move the right pointer to the left.
            right -= 1
    return count

print(count_pairs_le_k([1, 2, 3, 4, 5], 6))  # Output: 6
# Sorted array: [1, 2, 3, 4, 5], k = 6
# Pairs:
# (1,2) sum=3 (<=6)
# (1,3) sum=4 (<=6)
# (1,4) sum=5 (<=6)
# (1,5) sum=6 (<=6) -> with left=0 (1), right can be 1,2,3,4. (right-left = 4-0 = 4 pairs)
# (2,3) sum=5 (<=6)
# (2,4) sum=6 (<=6) -> with left=1 (2), right can be 2,3. (right-left = 3-1 = 2 pairs)
# Total = 4 + 2 = 6
```

### ✅ C++ Code:

C++

```c++
#include <iostream>
#include <vector>        // For vector
#include <algorithm>     // For std::sort

// using namespace std; // Generally better to qualify

// Function to count pairs with sum less than or equal to k
// arr: std::vector<int> (input vector of integers, passed by value as it's modified by sort)
//      Could be passed by reference `std::vector<int>& arr` if modification in place is desired by caller.
// k: int (target sum limit)
// Returns: int (count of pairs)
int countPairsLEK(std::vector<int> arr, int k) { // arr is copied here
    // std::sort(arr.begin(), arr.end()): sorts the vector in place.
    //   arr.begin(): returns an iterator to the beginning of the vector.
    //     Input: (none)
    //     Returns: std::vector<int>::iterator
    //   arr.end(): returns an iterator to the element following the end of the vector.
    //     Input: (none)
    //     Returns: std::vector<int>::iterator
    std::sort(arr.begin(), arr.end());

    int left = 0;             // int: pointer to the beginning of the sorted array
    // arr.size(): returns the number of elements in the vector.
    //   Input: (none)
    //   Returns: size_t (unsigned integer type)
    // Cast to int or ensure right is not negative if arr is empty (though `left < right` handles empty).
    int right = static_cast<int>(arr.size()) - 1; // int: pointer to the end
    int count = 0;            // int: total count of pairs found

    // Loop as long as the left pointer is to the left of the right pointer
    while (left < right) {
        // arr[left]: int, element at left pointer
        // arr[right]: int, element at right pointer
        long long current_sum = static_cast<long long>(arr[left]) + arr[right]; // Use long long to prevent overflow if sum is large
        if (current_sum <= k) {
            // If arr[left] + arr[right] <= k, all elements from arr[left+1] to arr[right]
            // can be paired with arr[left]. There are (right - left) such elements.
            count += (right - left); // (right - left): int, number of valid pairs for current arr[left]
            ++left;                  // Move the left pointer to the right
        } else {
            // If arr[left] + arr[right] > k, the sum is too large.
            --right;                 // Move the right pointer to the left
        }
    }
    return count; // int: the total count
}

int main() {
    // input_arr: std::vector<int>: a dynamic array of integers
    std::vector<int> input_arr = {1, 2, 3, 4, 5};
    // k_val: int: the target sum limit
    int k_val = 6;
    std::cout << countPairsLEK(input_arr, k_val) << std::endl; // Output: 6
    return 0; // int: indicates successful execution
}
```

### ✅ Rust Code:

Rust

```rust
use std::cmp::Ordering; // Not strictly needed here but often used with sorting/comparisons

// Function to count pairs with sum less than or equal to k
// arr: mut Vec<i32> (input mutable vector of 32-bit integers, ownership is taken)
// k: i32 (target sum limit, a 32-bit integer)
// Returns: i32 (count of pairs)
fn count_pairs_le_k(mut arr: Vec<i32>, k: i32) -> i32 {
    // arr.sort(): sorts the vector in place.
    //   Input: (self: &mut Vec<i32>)
    //   Returns: () (unit type, indicating no value is returned)
    arr.sort(); // Sorts in ascending order by default

    // left: usize: pointer to the beginning of the sorted array (indices are usize)
    // right: usize: pointer to the end of the sorted array
    let (mut left, mut right) = (0, if arr.is_empty() { 0 } else { arr.len() - 1 });
    // arr.len(): returns the number of elements in the vector.
    //   Input: (self: &Vec<i32>)
    //   Returns: usize
    // arr.is_empty(): checks if the vector is empty.
    //   Input: (self: &Vec<i32>)
    //   Returns: bool

    let mut count = 0; // i32: total count of pairs found

    // Loop as long as the left pointer is to the left of the right pointer
    // and right is a valid index (especially for empty or single-element arrays after arr.len() - 1)
    while left < right && !arr.is_empty() { // Ensure right is valid if array could be empty
        // arr[left]: i32, element at left pointer
        // arr[right]: i32, element at right pointer
        let current_sum: i64 = (arr[left] as i64) + (arr[right] as i64); // Cast to i64 to prevent overflow
        let k_i64 = k as i64; // Cast k to i64 for comparison

        if current_sum <= k_i64 {
            // If arr[left] + arr[right] <= k, all elements from index left+1 to right
            // can be paired with arr[left]. There are (right - left) such elements.
            // (right - left): usize, difference in indices
            // as i32: casts usize to i32
            count += (right - left) as i32;
            left += 1; // Move the left pointer to the right (left: usize)
        } else {
            // If arr[left] + arr[right] > k, the sum is too large.
            if right == 0 { // Prevent underflow if right is already 0
                break;
            }
            right -= 1; // Move the right pointer to the left (right: usize)
        }
    }
    count // i32: the total count
}

fn main() {
    // input_vec: Vec<i32>: a heap-allocated, resizable array of i32
    let input_vec = vec![1, 2, 3, 4, 5];
    // k_val: i32: the target sum limit
    let k_val = 6;
    // The vec! macro creates a new Vec<T>
    // count_pairs_le_k takes ownership of input_vec here because it's not passed by reference.
    println!("{}", count_pairs_le_k(input_vec, k_val)); // Output: 6
}
```

---

📚 **Summary Table (Unchanged, for reference):**

|   |   |   |   |   |   |
|---|---|---|---|---|---|
|**Use Case**|**Strategy**|**Time Complexity**|**Python**|**C++**|**Rust**|
|Longest Unique Substring|Sliding Window|O(n)|✅|✅|✅|
|Count Subarrays Sum = K|Prefix Sum + HashMap|O(n)|✅|✅|✅|
|Count Pairs ≤ K|Two Pointers|O(n log n)*|✅|✅|✅|

* Dominated by the sorting step. The two-pointer scan itself is O(n).

---
