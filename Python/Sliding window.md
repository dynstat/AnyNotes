## Sliding Window using leetcode 3258 - Count Substrings That Satisfy K-Constraint.
### 1. **Optimized Sliding Window Approach**

#### **Intuition**
- The sliding window technique allows us to efficiently count valid substrings by expanding and shrinking a window over the string.  
- A substring is valid as long as at least one of the constraints (`zeros ≤ k` or `ones ≤ k`) holds.  
- We expand the window by moving the right pointer and shrink it by moving the left pointer only when both constraints are violated (`zeros > k and ones > k`).

#### **Approach**
1. Use two pointers (`left` and `right`) to form a window over the string.
2. Track the number of `0`s and `1`s in the current window.
3. Expand the window by incrementing `right` and updating counts.
4. Shrink the window by incrementing `left` if both constraints are violated.
5. At each valid position, add all substrings ending at the `right` pointer to the result.

#### **Complexity**
- **Time Complexity:**  
  - Each character is processed at most twice (once when expanding and once when shrinking), so the complexity is **O(n)**.
- **Space Complexity:**  
  - Only a few variables (`zeros`, `ones`, `left`, etc.) are used, so the complexity is **O(1)**.

---

### 2. **Two Pointers Method (Sliding Window)**

#### **Intuition**
- Similar to the sliding window approach, but instead of maintaining a dynamic window, it processes substrings starting at every index `left`.  
- This approach counts valid substrings by iterating through possible `right` endpoints for each `left`.

#### **Approach**
1. For each starting position `left` in the string, initialize counts for `0`s and `1`s.
2. Expand the window by incrementing `right` and updating counts.
3. Check whether the current substring satisfies the constraints (`zeros ≤ k` or `ones ≤ k`).
4. If valid, increment the count. If invalid and both constraints are violated (`zeros > k and ones > k`), break out of the loop.
5. Move to the next starting position.

#### **Complexity**
- **Time Complexity:**  
  - For every starting position, we process all possible substrings ending at `right`. This leads to a complexity of **O(n²)**.
- **Space Complexity:**  
  - Uses only a few variables (`zeros`, `ones`, `left`, etc.), so the complexity is **O(1)**.

---

### 3. **Brute Force Approach**

#### **Intuition**
- Generate all possible substrings of the string and check each one individually for the constraints.  
- A substring is valid if at least one of the constraints (`zeros ≤ k` or `ones ≤ k`) holds.

#### **Approach**
1. Generate all substrings by iterating over all possible starting and ending positions (`start`, `end`).
2. For each substring, count the number of `0`s and `1`s.
3. Check whether the substring satisfies the constraints (`zeros ≤ k` or `ones ≤ k`).
4. If valid, increment the result counter.

#### **Complexity**
- **Time Complexity:**  
  - Generating all substrings takes **O(n²)**, and counting `0`s and `1`s in each substring takes up to **O(n)**.  
  - Total complexity is **O(n³)**.
- **Space Complexity:**  
  - Temporary storage for substrings or counters is used, so the complexity is **O(1)**.

---

### Summary of Complexities

| **Approach**                 | **Time Complexity** | **Space Complexity** |
|-------------------------------|---------------------|-----------------------|
| Optimized Sliding Window      | O(n)               | O(1)                 |
| Two Pointers Sliding Window   | O(n²)              | O(1)                 |
| Brute Force                   | O(n³)              | O(1)                 |

# Code
```python3 []
class Solution:
    # OPTIMIZED SLIDING WINDOW 
    def countKConstraintSubstrings(self, s: str, k: int) -> int:
        left = 0
        zeros, ones = 0, 0
        count = 0

        for right in range(len(s)):
            # Expand the window
            if s[right] == '0':
                zeros += 1
            else:
                ones += 1

            # Shrink the window if constraints are violated
            while zeros > k and ones > k:
                if s[left] == '0':
                    zeros -= 1
                else:
                    ones -= 1
                left += 1

            # Add the number of valid substrings ending at 'right'
            count += (right - left + 1)

        return count



    # 2 POINTERS method Sliding Window
    def countKConstraintSubstrings_sliding(self, s: str, k: int) -> int:
        n = len(s)
        count = 0
        
        # For each starting point
        for left in range(n):
            zeros = 0
            ones = 0
            
            # Expand window until either count exceeds k
            for right in range(left, n):
                if s[right] == '0':
                    zeros += 1
                else:
                    ones += 1
                    
                # If either count is <= k, it's a valid substring
                if zeros <= k or ones <= k:
                    count += 1
                # If both counts exceed k, break inner loop
                elif zeros > k and ones > k:
                    break
                    
        return count


    # BRUTE FORCE APPROACH
    def countKConstraintSubstrings_bf(self, s: str, k: int) -> int:
        s_len = len(s)
        ans = 0
        for start in range(s_len):
            for end in range(start, s_len):
                substr = s[start: end+1]
                # conditions -> no. of 0s == k || no. of 1s == k
                sub_idx = 0
                count_0 = 0
                count_1 = 0
                while sub_idx < len(substr):
                    if substr[sub_idx] == '0':
                        count_0 += 1
                    else:
                        count_1 += 1
                    sub_idx += 1
                # print(f"substr = {substr}, count_0 = {count_0}, count_1 = {count_1}")
                if not (count_0 > k and count_1 > k):
                    ans += 1
                # else:
                #     break
        return ans                    


        
```