# Corrected Subarray Sum Example with Frequency HashMap

**Input:** `nums = [3, 4, 7, 2, -3, 1, 4, 2, -6, 8, -1]`, `k = 7`

## Complete Step-by-Step Trace

**Initialization:**

- `freq = {0: 1}` (we can form sum 0 with empty prefix)
- `prefix_sum = 0`
- `count = 0`

|Step|i|nums[i]|prefix_sum|psum - k|freq[psum-k]|count|freq hashmap after update|
|---|---|---|---|---|---|---|---|
|Init|-|-|0|-|-|0|`{0: 1}`|
|1|0|3|3|-4|0|0|`{0: 1, 3: 1}`|
|2|1|4|7|0|1 ✅|1|`{0: 1, 3: 1, 7: 1}`|
|3|2|7|14|7|1 ✅|2|`{0: 1, 3: 1, 7: 1, 14: 1}`|
|4|3|2|16|9|0|2|`{0: 1, 3: 1, 7: 1, 14: 1, 16: 1}`|
|5|4|-3|13|6|0|2|`{0: 1, 3: 1, 7: 1, 14: 1, 16: 1, 13: 1}`|
|6|5|1|14|7|1 ✅|3|`{0: 1, 3: 1, 7: 1, 14: 2, 16: 1, 13: 1}`|
|7|6|4|18|11|0|3|`{0: 1, 3: 1, 7: 1, 14: 2, 16: 1, 13: 1, 18: 1}`|
|8|7|2|20|13|1 ✅|4|`{0: 1, 3: 1, 7: 1, 14: 2, 16: 1, 13: 1, 18: 1, 20: 1}`|
|9|8|-6|14|7|1 ✅|5|`{0: 1, 3: 1, 7: 1, 14: 3, 16: 1, 13: 1, 18: 1, 20: 1}`|
|10|9|8|22|15|0|5|`{0: 1, 3: 1, 7: 1, 14: 3, 16: 1, 13: 1, 18: 1, 20: 1, 22: 1}`|
|11|10|-1|21|14|3 ✅|8|`{0: 1, 3: 1, 7: 1, 14: 3, 16: 1, 13: 1, 18: 1, 20: 1, 22: 1, 21: 1}`|

## Detailed Analysis of Each Match

### Step 2 (i=1): Found 1 subarray

- `prefix_sum = 7`, looking for `freq[0] = 1`
- **Subarray found:** from start to i=1 → `[3, 4]`
- **Sum check:** 3 + 4 = **7** ✅

### Step 3 (i=2): Found 1 subarray

- `prefix_sum = 14`, looking for `freq[7] = 1`
- **Subarray found:** from after where prefix_sum was 7 (after i=1) to i=2 → `[7]`
- **Sum check:** 7 = **7** ✅

### Step 6 (i=5): Found 1 subarray

- `prefix_sum = 14`, looking for `freq[7] = 1`
- **Subarray found:** from after where prefix_sum was 7 (after i=1) to i=5 → `[7, 2, -3, 1]`
- **Sum check:** 7 + 2 + (-3) + 1 = **7** ✅

### Step 8 (i=7): Found 1 subarray

- `prefix_sum = 20`, looking for `freq[13] = 1`
- **Subarray found:** from after where prefix_sum was 13 (after i=4) to i=7 → `[1, 4, 2]`
- **Sum check:** 1 + 4 + 2 = **7** ✅

### Step 9 (i=8): Found 1 subarray

- `prefix_sum = 14`, looking for `freq[7] = 1`
- **Subarray found:** from after where prefix_sum was 7 (after i=1) to i=8 → `[7, 2, -3, 1, 4, 2, -6]`
- **Sum check:** 7 + 2 + (-3) + 1 + 4 + 2 + (-6) = **7** ✅

### Step 11 (i=10): Found 3 subarrays

- `prefix_sum = 21`, looking for `freq[14] = 3`
- This means prefix_sum was 14 at three different positions: i=2, i=5, and i=8

**Subarray 1:** from after i=2 to i=10 → `[2, -3, 1, 4, 2, -6, 8, -1]`

- **Sum check:** 2 + (-3) + 1 + 4 + 2 + (-6) + 8 + (-1) = **7** ✅

**Subarray 2:** from after i=5 to i=10 → `[4, 2, -6, 8, -1]`

- **Sum check:** 4 + 2 + (-6) + 8 + (-1) = **7** ✅

**Subarray 3:** from after i=8 to i=10 → `[8, -1]`

- **Sum check:** 8 + (-1) = **7** ✅

## Final Answer: 8 Valid Subarrays

1. **[3, 4]** (indices 0-1) = **7** ✅
2. **[7]** (index 2) = **7** ✅
3. **[7, 2, -3, 1]** (indices 2-5) = **7** ✅
4. **[1, 4, 2]** (indices 5-7) = **7** ✅
5. **[7, 2, -3, 1, 4, 2, -6]** (indices 2-8) = **7** ✅
6. **[2, -3, 1, 4, 2, -6, 8, -1]** (indices 3-10) = **7** ✅
7. **[4, 2, -6, 8, -1]** (indices 6-10) = **7** ✅
8. **[8, -1]** (indices 9-10) = **7** ✅

## Visual Representation

```
Array:  [3, 4, 7, 2, -3, 1, 4, 2, -6, 8, -1]
Index:   0  1  2  3   4  5  6  7   8  9  10

Subarrays that sum to 7:
[3, 4] ——————————————————————————————————————— indices 0-1
       [7] ———————————————————————————————————— index 2
       [7, 2, -3, 1] ——————————————————————————— indices 2-5  
                [1, 4, 2] —————————————————————— indices 5-7
       [7, 2, -3, 1, 4, 2, -6] ————————————————— indices 2-8
          [2, -3, 1, 4, 2, -6, 8, -1] —————————— indices 3-10
                   [4, 2, -6, 8, -1] —————————— indices 6-10
                            [8, -1] ——————————— indices 9-10
```

**Total: 8 subarrays**