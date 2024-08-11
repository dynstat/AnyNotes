138. Copy List with Random Pointer
Medium
Topics
Companies
Hint
A linked list of length n is given such that each node contains an additional random pointer, which could point to any node in the list, or null.

Construct a deep copy of the list. The deep copy should consist of exactly n brand new nodes, where each new node has its value set to the value of its corresponding original node. Both the next and random pointer of the new nodes should point to new nodes in the copied list such that the pointers in the original list and copied list represent the same list state. None of the pointers in the new list should point to nodes in the original list.

For example, if there are two nodes X and Y in the original list, where X.random --> Y, then for the corresponding two nodes x and y in the copied list, x.random --> y.

Return the head of the copied linked list.

The linked list is represented in the input/output as a list of n nodes. Each node is represented as a pair of [val, random_index] where:

val: an integer representing Node.val
random_index: the index of the node (range from 0 to n-1) that the random pointer points to, or null if it does not point to any node.
Your code will only be given the head of the original linked list.

 

Example 1:


Input: head = [[7,null],[13,0],[11,4],[10,2],[1,0]]
Output: [[7,null],[13,0],[11,4],[10,2],[1,0]]
Example 2:


Input: head = [[1,1],[2,1]]
Output: [[1,1],[2,1]]
Example 3:



Input: head = [[3,null],[3,0],[3,null]]
Output: [[3,null],[3,0],[3,null]]
 

Constraints:

0 <= n <= 1000
-104 <= Node.val <= 104
Node.random is null or is pointing to some node in the linked list.


### Solution:

```python
# Definition for a Node.
class Node:
    def __init__(self, x: int, next: "Node" = None, random: "Node" = None):
        self.val = int(x)
        self.next = next
        self.random = random

    def __repr__(self):
        return (
            f"({str(id(self))[-1:-4:-1]})[{self.val}, {self.random.val}]"
            if self.random
            else f"({str(id(self))[-1:-4:-1]})[{self.val}, None]"
        )


class Solution:
    def copyRandomList(self, head):
        if not head:
            return None

        # Step 1: Create a copy of each node and link them side-by-side in a single list.
        current = head
        while current:
            new_node = Node(current.val, current.next, None)
            current.next = new_node
            current = new_node.next

        # Step 2: Assign random pointers for the copy nodes.
        current = head
        while current:
            if current.random:
                current.next.random = current.random.next
            current = current.next.next

        # Step 3: Restore the original list, and extract the copy list.
        current = head
        copy_head = head.next
        while current:
            copy = current.next
            current.next = copy.next
            if copy.next:
                copy.next = copy.next.next
            current = current.next

        return copy_head


# Test and print results
if __name__ == "__main__":
    # Create test cases
    def create_linked_list(values, random_indices):
        nodes = [Node(val) for val in values]
        for i, node in enumerate(nodes):
            if i < len(nodes) - 1:
                node.next = nodes[i + 1]
            if random_indices[i] is not None:
                node.random = nodes[random_indices[i]]
        return nodes[0] if nodes else None

    # Test case 1: Simple list with random pointers
    head1 = create_linked_list([7, 13, 11, 10, 1], [None, 0, 4, 2, 0])

    # Test case 2: List with self-referencing random pointers
    head2 = create_linked_list([1, 2, 3], [1, 2, 0])

    # Test case 3: Empty list
    head3 = None

    # Test case 4: Single node with self-referencing random pointer
    head4 = create_linked_list([1], [0])

    solution = Solution()

    # Helper function to print the linked list
    def print_linked_list(head):
        current = head
        while current:
            random_val = current.random.val if current.random else None
            print(f"[{current.val}, {random_val}]", end=" -> ")
            current = current.next
        print("None")

    # Test and print results
    for i, test_case in enumerate([head1, head2, head3, head4], 1):
        print(f"\nTest case {i}:")
        print("Original list:")
        print_linked_list(test_case)

        copied = solution.copyRandomList(test_case)

        print("Copied list:")
        print_linked_list(copied)

```

### Explanation with Example

Let's break down the `copyRandomList` function step-by-step with an example.

#### Example Linked List
Consider a linked list with nodes having values `[1, 2, 3]` and random pointers `[None, 0, 1]`.

#### Step 1: Create a copy of each node and link them side-by-side
- **Initial List**: `1 -> 2 -> 3`
- **After Step 1**: `1 -> 1' -> 2 -> 2' -> 3 -> 3'`
  - Each original node is followed by its copy.

#### Step 2: Assign random pointers for the copy nodes
- **Original Random Pointers**: `1.random = None`, `2.random = 1`, `3.random = 2`
- **After Step 2**:
  - `1'.random = None`
  - `2'.random = 1'` (since `2.random` points to `1`, `2'.random` should point to `1'`)
  - `3'.random = 2'` (since `3.random` points to `2`, `3'.random` should point to `2'`)

#### Step 3: Restore the original list and extract the copy list
- **Before Step 3**: `1 -> 1' -> 2 -> 2' -> 3 -> 3'`
- **After Step 3**:
  - Restore original list: `1 -> 2 -> 3`
  - Extract copy list: `1' -> 2' -> 3'`

### Detailed Steps in Code

1. **Step 1: Create Copies**
   ```python
   current = head
   while current:
       new_node = Node(current.val, current.next, None)  # Create new node
       current.next = new_node  # Link new node after current
       current = new_node.next  # Move to the next original node
   ```

2. **Step 2: Assign Random Pointers**
   ```python
   current = head
   while current:
       if current.random:
           current.next.random = current.random.next  # Set random pointer for the copy
       current = current.next.next  # Move to the next original node
   ```

3. **Step 3: Restore and Extract**
   ```python
   current = head
   copy_head = head.next  # Head of the copied list
   while current:
       copy = current.next  # Copy node
       current.next = copy.next  # Restore original list
       if copy.next:
           copy.next = copy.next.next  # Link copy nodes
       current = current.next  # Move to the next original node
   ```

### Visualization

#### Initial List
```
1 -> 2 -> 3
```

#### After Step 1
```
1 -> 1' -> 2 -> 2' -> 3 -> 3'
```

#### After Step 2
```
1 -> 1' -> 2 -> 2' -> 3 -> 3'
     |         |         |
    None      1'        2'
```

#### After Step 3
- **Original List**: `1 -> 2 -> 3`
- **Copied List**: `1' -> 2' -> 3'`

This step-by-step breakdown should help you visualize how the `copyRandomList` function works.