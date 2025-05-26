The "Largest Rectangle in Histogram" problem (LeetCode 84) is solved efficiently using a monotonic increasing stack. This stack helps determine, for each bar, the widest rectangle where that bar is the shortest.

**Core Logic:**

1.  **Iterate with a Sentinel:** We iterate through the `heights` array, appending a 0 at the end. This sentinel bar (height 0) ensures all bars in the stack are processed.
2.  **Monotonic Increasing Stack:** The stack stores indices of bars, maintaining an order where bar heights are increasing from the bottom to the top of the stack.
3.  **Processing Each Bar (`heights[i]`):**
    *   **While `heights[i]` is shorter than `heights[stack.top()]`**:
        *   This means the bar at `stack.top()` (let's call its index `top_idx` and height `h_top`) has found its **right boundary**: the current bar `i`.
        *   Its **left boundary** is the index of the bar now at the top of the stack (after `top_idx` is popped). If the stack becomes empty, the left boundary is effectively the start of the histogram (index -1).
        *   Calculate the `width = i - (new_stack_top_idx_if_exists_else_-1) - 1`.
        *   Calculate `area = h_top * width`.
        *   Update `max_area`.
        *   Pop `top_idx` from the stack and repeat this check.
    *   **Push `i` to stack:** After processing all taller bars (or if `heights[i]` was not shorter), push the current index `i` onto the stack. This maintains the increasing order or starts a new one if `heights[i]` is taller.

**Simplified Visualization (`heights = [2,1,5,6,2,3]`, appended with 0):**

*   **`[2,1,5,6,2,3,0]`**
*   **`i=0, h=2`**: Stack: `[0]` (Indices)
*   **`i=1, h=1`**: `1 < h[0](2)`. Pop `0`.
    *   Bar `h[0]=2`. Right boundary `i=1`. Left boundary (stack empty) `-1`. Width `1 - (-1) - 1 = 1`. Area `2*1=2`. `max_area=2`.
    *   Stack: `[]`. Push `1`. Stack: `[1]`.
*   **`i=2, h=5`**: `5 > h[1](1)`. Push `2`. Stack: `[1,2]`.
*   **`i=3, h=6`**: `6 > h[2](5)`. Push `3`. Stack: `[1,2,3]`.
*   **`i=4, h=2`**:
    *   `2 < h[3](6)`. Pop `3`. Bar `h[3]=6`. Right `i=4`. Left `stack.top()=2`. Width `4-2-1=1`. Area `6*1=6`. `max_area=6`. Stack: `[1,2]`.
    *   `2 < h[2](5)`. Pop `2`. Bar `h[2]=5`. Right `i=4`. Left `stack.top()=1`. Width `4-1-1=2`. Area `5*2=10`. `max_area=10`. Stack: `[1]`.
    *   `2 > h[1](1)`. Push `4`. Stack: `[1,4]`.
*   **`i=5, h=3`**: `3 > h[4](2)`. Push `5`. Stack: `[1,4,5]`.
*   **`i=6, h=0` (Sentinel):**
    *   `0 < h[5](3)`. Pop `5`. Bar `h[5]=3`. Right `i=6`. Left `stack.top()=4`. Width `6-4-1=1`. Area `3*1=3`. `max_area=10`. Stack: `[1,4]`.
    *   `0 < h[4](2)`. Pop `4`. Bar `h[4]=2`. Right `i=6`. Left `stack.top()=1`. Width `6-1-1=4`. Area `2*4=8`. `max_area=10`. Stack: `[1]`.
    *   `0 < h[1](1)`. Pop `1`. Bar `h[1]=1`. Right `i=6`. Left (stack empty) `-1`. Width `6-(-1)-1=6`. Area `1*6=6`. `max_area=10`. Stack: `[]`.
    *   Push `6` (index of sentinel, though it won't be processed further for area).

The final `max_area` is 10. The stack efficiently identifies for each bar (when it's popped) the exact span (left and right shorter bars) where it can be the shortest bar in a rectangle.

```mermaid
graph TD
    Start --> IterateBars{Iterate `i` from `0` to `n` (where `heights[n]` is appended 0)};
    IterateBars --> CheckCondition{While stack `S` is not empty AND (`i == n` OR `heights[i] < heights[S.top()]`)};
    CheckCondition -- True --> ProcessPoppedBar{
        Height `H = heights[S.pop()]`
        Width `W = S.empty() ? i : i - S.top() - 1`
        `max_area = max(max_area, H * W)`
    };
    ProcessPoppedBar --> CheckCondition;
    CheckCondition -- False --> MaybePush{If `i < n`};
    MaybePush -- True --> PushToStack[Push `i` to `S`];
    PushToStack --> IterateBars;
    MaybePush -- False --> IterateBars;
    IterateBars -- Iteration complete --> ReturnMax[Return `max_area`];
    ReturnMax --> End;
```