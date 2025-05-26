## ✅ Overview of LeetCode 974 - **Subarray Sums Divisible by K**

**Problem Statement:**  
Given an integer array `nums` and an integer `k`, return the number of **subarrays** such that the sum of the subarray is divisible by `k`.

### Example:

```text
Input: nums = [4,5,0,-2,-3,1], k = 5  
Output: 7

Explanation:
Subarrays with sum divisible by 5:  
[4, 5, 0, -2, -3, 1] → [4], [5], [0], [4,5,0,-2,-3,1], [5,0], [0,-2,-3], [-2,-3,1]
Total: 7
```

---

# 🧠 Intuition:
## Optimized Approach

To solve this problem efficiently, we use **prefix sums** and the concept of **modulo arithmetic**. (If two prefix sums have the same remainder when divided by `k`, the subarray between them is divisible by `k`.)

The `subarrays_div_by_k` function is solving a problem where we need to count the number of subarrays whose sum is divisible by a given integer `k`. However, it's using a more optimal approach that doesn't explicitly check each possible subarray. Let me break down how it works:

The key insight is using a mathematical property: if two prefix sums have the same remainder when divided by `k`, then the subarray between those two points has a sum divisible by `k`.

Here's a visual walkthrough using your first example:
```
nums = [4, 5, 0, -2, -3, 1]
k = 5
```

| Index | Value | Prefix Sum | Modulo (remainder when divided by 5) | Action | Subarrays Found |
|-------|-------|------------|--------------------------------------|--------|-----------------|
| Start | N/A   | 0          | 0                                    | mod_count = {0: 1} | 0 |
| 0     | 4     | 4          | 4                                    | mod_count = {0: 1, 4: 1} | 0 |
| 1     | 5     | 9          | 4                                    | We found another prefix with modulo 4! This means we have a subarray divisible by 5. | 1 |
| 2     | 0     | 9          | 4                                    | Another modulo 4 (we already have 2). This gives us 2 more subarrays. | 3 |
| 3     | -2    | 7          | 2                                    | mod_count = {0: 1, 4: 3, 2: 1} | 3 |
| 4     | -3    | 4          | 4                                    | Another modulo 4 (we already have 3). This gives us 3 more subarrays. | 6 |
| 5     | 1     | 5          | 0                                    | We found another prefix with modulo 0 (we have 1 at start). This gives us 1 more subarray. | 7 |

Let's trace through a few key subarrays to make sure we understand:

1. One subarray whose sum is divisible by 5 is `[5]` at index 1. Its sum is 5, which is divisible by 5.
2. From index 1 to index 4: `[5, 0, -2, -3]` has a sum of 0, which is divisible by 5.
3. The entire array `[4, 5, 0, -2, -3, 1]` has a sum of 5, which is divisible by 5.

Why don't we need to explicitly check each subarray? Rather than generating all possible subarrays (there would be n(n+1)/2 of them, which is O(n²)), we use this prefix sum and modulo approach:

1. We maintain a running prefix sum as we go through the array.
2. For each position, we calculate the modulo (remainder when divided by k) of the current prefix sum.
3. If we've seen this modulo before, it means there exists a previous prefix sum such that:
   * current_prefix_sum % k = previous_prefix_sum % k
   * This means: (current_prefix_sum - previous_prefix_sum) % k = 0
   * Therefore, the subarray between these two points has a sum divisible by k.

The HashMap `mod_count` is tracking how many times we've seen each modulo value so far. When we encounter a modulo we've seen before, we know we can form that many new subarrays whose sum is divisible by k.

Special case: We initialize `mod_count` with `{0: 1}` to handle subarrays starting from the beginning of the array whose sum is directly divisible by k.

The line `result += *mod_count.get(&modulo).unwrap_or(&0)` is adding the count of previous prefix sums with the same modulo, because each such previous occurrence forms a valid subarray with the current position.

This approach is O(n) time complexity instead of the O(n²) approach of checking each possible subarray explicitly.

For negative numbers like in your example, there's an extra step: when the modulo is negative, we add k to it to get the equivalent positive modulo. This ensures consistent handling regardless of whether the prefix sum is positive or negative.


## Brute-Force Approach

Let me show you how a brute force approach would work, with visualization of each step for the example:

```rust
fn subarrays_div_by_k_brute_force(nums: Vec<i32>, k: i32) -> i32 {
    let n = nums.len();
    let mut count = 0;
    
    // Consider all possible starting positions
    for start in 0..n {
        let mut sum = 0;
        
        // Consider all possible ending positions for each starting position
        for end in start..n {
            // Add the current element to the running sum
            sum += nums[end];
            
            // Check if the sum of this subarray is divisible by k
            if sum % k == 0 {
                count += 1;
            }
        }
    }
    
    count
}
```

Now let's trace through the example `nums = [4, 5, 0, -2, -3, 1]` and `k = 5`:

**Visual Trace:**

```
start = 0 (element 4)
```

| End | Subarray | Running Sum | Is Sum % 5 == 0? | Count |
|-----|----------|-------------|------------------|-------|
| 0   | [4]      | 4           | No               | 0     |
| 1   | [4, 5]   | 9           | No               | 0     |
| 2   | [4, 5, 0] | 9           | No               | 0     |
| 3   | [4, 5, 0, -2] | 7           | No               | 0     |
| 4   | [4, 5, 0, -2, -3] | 4           | No               | 0     |
| 5   | [4, 5, 0, -2, -3, 1] | 5           | Yes              | 1     |

```
start = 1 (element 5)
```

| End | Subarray | Running Sum | Is Sum % 5 == 0? | Count |
|-----|----------|-------------|------------------|-------|
| 1   | [5]      | 5           | Yes              | 2     |
| 2   | [5, 0]   | 5           | Yes              | 3     |
| 3   | [5, 0, -2] | 3           | No               | 3     |
| 4   | [5, 0, -2, -3] | 0           | Yes              | 4     |
| 5   | [5, 0, -2, -3, 1] | 1           | No               | 4     |

```
start = 2 (element 0)
```

| End | Subarray | Running Sum | Is Sum % 5 == 0? | Count |
|-----|----------|-------------|------------------|-------|
| 2   | [0]      | 0           | Yes              | 5     |
| 3   | [0, -2]  | -2          | No               | 5     |
| 4   | [0, -2, -3] | -5          | Yes              | 6     |
| 5   | [0, -2, -3, 1] | -4          | No               | 6     |

```
start = 3 (element -2)
```

| End | Subarray | Running Sum | Is Sum % 5 == 0? | Count |
|-----|----------|-------------|------------------|-------|
| 3   | [-2]     | -2          | No               | 6     |
| 4   | [-2, -3] | -5          | Yes              | 7     |
| 5   | [-2, -3, 1] | -4          | No               | 7     |

```
start = 4 (element -3)
```

| End | Subarray | Running Sum | Is Sum % 5 == 0? | Count |
|-----|----------|-------------|------------------|-------|
| 4   | [-3]     | -3          | No               | 7     |
| 5   | [-3, 1]  | -2          | No               | 7     |

```
start = 5 (element 1)
```

| End | Subarray | Running Sum | Is Sum % 5 == 0? | Count |
|-----|----------|-------------|------------------|-------|
| 5   | [1]      | 1           | No               | 7     |

**Final result: 7 subarrays with sum divisible by 5.**

These are the 7 subarrays:
1. `[4, 5, 0, -2, -3, 1]` (sum = 5)
2. `[5]` (sum = 5) 
3. `[5, 0]` (sum = 5)
4. `[5, 0, -2, -3]` (sum = 0)
5. `[0]` (sum = 0)
6. `[0, -2, -3]` (sum = -5)
7. `[-2, -3]` (sum = -5)

This brute force approach considers all `n(n+1)/2` possible subarrays (where n is the length of the array), making it an O(n²) algorithm. For our example with n=6, that's 21 possible subarrays that we check one by one.

In contrast, the prefix sum approach in the original solution is O(n) because it only needs to scan the array once, using the mathematical property that two prefix sums with the same remainder when divided by k define a subarray with a sum divisible by k.

The difference becomes even more significant with larger arrays. Imagine an array with 10,000 elements:
- Brute force: ~50,000,000 operations
- Prefix sum: ~10,000 operations

This explains why the original solution (using prefix sums and the HashMap) is preferred for this problem, especially for large inputs.

---

## 🔍 Part 1: Brief Summary (Normal and Optimized)

|Type|Approach|Time Complexity|Space Complexity|Key Idea|
|---|---|---|---|---|
|Normal|Brute-force with prefix sum|O(n²)|O(1)|Calculate every subarray sum, check if divisible by `k`|
|Optimized|Prefix sum + Hash Map (modulo counting)|O(n)|O(k)|Count mod values, reuse if prefix sums have same modulo remainder|

---

# ✅ Part 2: Detailed Solutions in Python, C++, and Rust

---

## 🐍 PYTHON SOLUTION (Optimized)

```python
from collections import defaultdict

def subarraysDivByK(nums, k):
    mod_count = defaultdict(int)
    mod_count[0] = 1  # base case: subarray from start is divisible
    prefix_sum = 0
    result = 0

    for num in nums:
        prefix_sum += num
        mod = prefix_sum % k
        if mod < 0:  # fix negative modulus
            mod += k
        result += mod_count[mod]
        mod_count[mod] += 1

    return result
```

### ✅ Explanation:

- `defaultdict(int)` → initializes the hash map with default value 0.
    
- `prefix_sum` → running sum of array.
    
- `mod` → remainder of prefix_sum with `k`. If same remainder seen before, then subarray in between has sum divisible by `k`.
    
- `mod_count[mod] += 1` → track how many times this mod value has occurred.
    

---

## 🧾 C++ SOLUTION (Both Normal and Optimized)

---

### 🔸 **Normal (Brute Force) - O(n²)**

```cpp
#include <vector>
using namespace std;

int subarraysDivByK(vector<int>& nums, int k) {
    int count = 0;
    for (int start = 0; start < nums.size(); ++start) {
        int sum = 0;
        for (int end = start; end < nums.size(); ++end) {
            sum += nums[end];
            if (sum % k == 0)
                count++;
        }
    }
    return count;
}
```

### 🔸 **Optimized (Prefix Sum + HashMap)**

```cpp
#include <vector>
#include <unordered_map>
using namespace std;

int subarraysDivByK(vector<int>& nums, int k) {
    unordered_map<int, int> modCount;
    modCount[0] = 1;
    int prefixSum = 0, result = 0;

    for (int num : nums) {
        prefixSum += num;
        int mod = prefixSum % k;
        if (mod < 0) mod += k; // handle negative mods

        result += modCount[mod];
        modCount[mod]++;
    }

    return result;
}
```

### 🔍 C++ Concepts Breakdown:

|Item|Used For|
|---|---|
|`unordered_map`|Hash map to track frequency of modulo values (O(1) access)|
|`prefixSum`|To compute running sum|
|`mod = sum % k`|Key insight: same modulo → difference divisible by k|
|`mod < 0`|C++ `%` operator keeps sign → adjust for negative remainders|
|`modCount[mod]++`|Increment count of that mod in the hash map|

---

## 🦀 RUST SOLUTION (Optimized)

```rust
use std::collections::HashMap;

pub fn subarrays_div_by_k(nums: Vec<i32>, k: i32) -> i32 {
    let mut mod_count = HashMap::new();
    mod_count.insert(0, 1);
    let mut prefix_sum = 0;
    let mut result = 0;

    for num in nums {
        prefix_sum += num;
        let mut modulo = prefix_sum % k;
        if modulo < 0 {
            modulo += k;
        }

        result += *mod_count.get(&modulo).unwrap_or(&0);
        *mod_count.entry(modulo).or_insert(0) += 1;
    }

    result
}
```

### 🔍 RUST Concepts Breakdown:

|Rust Element|Purpose|
|---|---|
|`HashMap::new()`|Creates a hash map to count mod frequencies|
|`insert(0, 1)`|Seed the map for base case (zero mod)|
|`prefix_sum % k`|Compute mod to track divisibility pattern|
|`modulo += k` if negative|Rust handles `%` like C/C++ — need to correct for negative mod|
|`get(&modulo).unwrap_or(&0)`|Safely get value or default to 0|
|`entry(modulo).or_insert(0) += 1`|Insert new or increment existing count|


Okay, let's break down those two lines of Rust code from your `subarrays_div_by_k` function. They are indeed doing something similar to the Python lines above, but Rust's HashMap API and its ownership/borrowing system make the syntax a bit different.

At a high level, both lines are interacting with `mod_count`, which is a `HashMap`. This `HashMap` is used to store the frequency of each `modulo` value encountered so far.

Let's look at the first line:
`result += *mod_count.get(&modulo).unwrap_or(&0);`

*   **`mod_count.get(&modulo)`**:
    *   This part attempts to retrieve a reference to the value associated with the key `modulo` in the `mod_count` HashMap.
    *   In Rust, when a key might not exist in a `HashMap`, the `get` method doesn't directly return the value or raise an error like Python's `[]` might if the key is absent. Instead, it returns an `Option<&V>`, where `V` is the type of the value (in this case, `i32`).
    *   An `Option` can be either `Some(value)` if the key exists, or `None` if it doesn't. Here, `&modulo` is used because `get` expects a reference to the key.

*   **`.unwrap_or(&0)`**:
    *   This method is called on the `Option` returned by `get()`.
    *   If the `Option` is `Some(value_reference)`, `unwrap_or` "unwraps" it and gives you `value_reference`.
    *   If the `Option` is `None` (meaning `modulo` was not found in `mod_count`), `unwrap_or` returns the default value you provide. In this case, the default is `&0` (a reference to the integer `0`).
    *   So, this expression will always yield a reference to an `i32`: either the count found in the map or `0`.

*   **`*...`** (Dereference operator):
    *   The `*` in front of `mod_count.get(&modulo).unwrap_or(&0)` dereferences the reference obtained from `unwrap_or`. This means it gets the actual `i32` value (the count or `0`) that the reference was pointing to.

*   **`result += ...`**:
    *   Finally, this actual `i32` value is added to your `result` variable.

**Why is it like this?**
Rust's design emphasizes explicitness and safety, especially around potentially missing values. The `Option` type forces you to consider the case where a key might not be in the `HashMap`. `unwrap_or` is one way to handle this by providing a sensible default.

**Comparison to Python:**
Your Python equivalent: `result += mod_count[mod]`
This Python code assumes `mod` is *always* in `mod_count`. If `mod` is not in `mod_count`, this line would raise a `KeyError`.
A more robust Python equivalent that mirrors the Rust line's logic would be:
```python
result += mod_count.get(mod, 0)
```
Here, Python's `dict.get(key, default)` method directly returns the value for `key` if `key` is in the dictionary, else returns `default`. This is more concise than the Rust version because Python's dynamic typing and standard library design handle this common pattern differently.

---

Now, let's look at the second line:
`*mod_count.entry(modulo).or_insert(0) += 1;`

*   **`mod_count.entry(modulo)`**:
    *   This is a very useful method on `HashMap`s in Rust. It returns an `Entry` enum.
    *   An `Entry` represents a potential slot in the map for the given key (`modulo`). It can be one of two variants:
        *   `Entry::Occupied(entry)`: If the key `modulo` already exists in the map. `entry` gives you access to the existing key-value pair.
        *   `Entry::Vacant(entry)`: If the key `modulo` does not exist in the map. `entry` allows you to insert a value for this key.

*   **`.or_insert(0)`**:
    *   This method is called on the `Entry`.
    *   If the `Entry` is `Vacant` (key `modulo` not found), `or_insert(0)` inserts the value `0` for the key `modulo` into the `HashMap`. It then returns a mutable reference (`&mut i32`) to this newly inserted `0`.
    *   If the `Entry` is `Occupied` (key `modulo` found), `or_insert(0)` does nothing to the map but returns a mutable reference (`&mut i32`) to the existing value.
    *   So, regardless of whether the key was already there, this expression gives you a mutable reference to the count associated with `modulo` (initializing it to `0` if it wasn't present).

*   **`*...`** (Dereference operator):
    *   The `*` in front of `mod_count.entry(modulo).or_insert(0)` dereferences the mutable reference. This gives you direct access to the `i32` value itself, allowing you to modify it.

*   **`... += 1`**:
    *   The value obtained by dereferencing (the count) is then incremented by `1`. Because we had a *mutable* reference, this modification happens directly in the `HashMap`.

**Why is it like this?**
The `entry` API is designed to be efficient. It avoids doing two lookups (one to check if the key exists, and another to insert or update). It performs a single lookup and then allows you to operate on the entry. This is particularly important in Rust for managing ownership and borrowing correctly when modifying collections.

**Comparison to Python:**
Your Python equivalent: `mod_count[mod] += 1`
Again, if `mod` is not in `mod_count`, this Python line would raise a `KeyError` when trying to access `mod_count[mod]` on the right-hand side of `+=` (which is roughly `mod_count[mod] = mod_count[mod] + 1`).

A more robust Python equivalent that mirrors the Rust line's logic:
```python
if mod in mod_count:
    mod_count[mod] += 1
else:
    mod_count[mod] = 1
```
Or, more idiomatically in Python using `get` with a default for the initial value:
```python
mod_count[mod] = mod_count.get(mod, 0) + 1
```
Or, if `mod_count` was a `collections.defaultdict(int)`:
```python
# (assuming from collections import defaultdict; mod_count = defaultdict(int))
mod_count[mod] += 1
```
The `defaultdict(int)` approach in Python is very concise for this pattern, as it automatically initializes a key with `int()` (which is `0`) if the key is accessed and doesn't exist.

**In summary:**

*   **`result += *mod_count.get(&modulo).unwrap_or(&0);`**
    *   Safely gets the count for `modulo` from `mod_count`.
    *   If `modulo` isn't found, it defaults to `0`.
    *   Adds this count (or `0`) to `result`.
    *   Rust's `Option` and `unwrap_or` make handling missing keys explicit.

*   **`*mod_count.entry(modulo).or_insert(0) += 1;`**
    *   Efficiently gets a mutable reference to the count for `modulo`.
    *   If `modulo` isn't in `mod_count`, it first inserts `0` as its count.
    *   Then, it increments that count by `1` directly in the `HashMap`.
    *   Rust's `entry` API is an efficient and idiomatic way to handle this "get-or-insert-then-modify" pattern.

The Rust versions are more verbose primarily due to Rust's strong type system, its emphasis on explicit error/option handling (via `Option` and `Result`), and its ownership/borrowing rules which necessitate careful handling of references (like `&` and `&mut`) and dereferencing (`*`). Python, being dynamically typed and having a garbage collector, can offer more concise syntax for these common dictionary operations, often by implicitly handling cases or providing higher-level abstractions like `defaultdict`.


---

## 📊 Sample Run Walkthrough:

**Input:**

```text
nums = [4, 5, 0, -2, -3, 1], k = 5
```

|Index|Prefix Sum|Prefix % K|Previous Count of mod|Subarrays Added|Map (mod → count)|
|---|---|---|---|---|---|
|0|4|4|0|0|{0:1, 4:1}|
|1|9|4|1|1|{0:1, 4:2}|
|2|9|4|2|2|{0:1, 4:3}|
|3|7|2|0|0|{0:1, 4:3, 2:1}|
|4|4|4|3|3|{0:1, 4:4, 2:1}|
|5|5|0|1|1|{0:2, 4:4, 2:1}|

Total = **1+2+3+1 = 7**

---

## 📚 Useful References:

- Python defaultdict: [https://docs.python.org/3/library/collections.html#collections.defaultdict](https://docs.python.org/3/library/collections.html#collections.defaultdict)
    
- C++ unordered_map: [https://en.cppreference.com/w/cpp/container/unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map)
    
- Rust HashMap: [https://doc.rust-lang.org/std/collections/struct.HashMap.html](https://doc.rust-lang.org/std/collections/struct.HashMap.html)
    
- Modulo math tricks: [https://mathworld.wolfram.com/Modulo.html](https://mathworld.wolfram.com/Modulo.html)
    

---

Would you like a visual diagram for the prefix mod mapping?