## Tree Data Structure Learning Roadmap

## 1. Basic Tree Concepts (Week 1)
- Tree terminology
  - Root, Node, Edge, Leaf
  - Parent, Child, Siblings
  - Height, Depth, Level
  - Subtree
- Types of Trees
  - Binary Trees
  - N-ary Trees
  - Perfect, Complete, and Full Binary Trees
  - Balanced vs Unbalanced Trees

Here's a basic implementation to understand tree node structure:

```python
class TreeNode:
    def __init__(self, val=0):
        self.val = val
        self.left = None
        self.right = None
```

## 2. Binary Trees (Week 1-2)
- Implementation
- Tree Traversals
  - Inorder (Left-Root-Right)
  - Preorder (Root-Left-Right)
  - Postorder (Left-Right-Root)
  - Level Order (BFS)

Example implementation of traversals:

```python
def inorder(root):
    if not root:
        return
    inorder(root.left)
    print(root.val)
    inorder(root.right)

def preorder(root):
    if not root:
        return
    print(root.val)
    preorder(root.left)
    preorder(root.right)

def postorder(root):
    if not root:
        return
    postorder(root.left)
    postorder(root.right)
    print(root.val)

def levelOrder(root):
    if not root:
        return
    queue = [root]
    while queue:
        node = queue.pop(0)
        print(node.val)
        if node.left:
            queue.append(node.left)
        if node.right:
            queue.append(node.right)
```

## 3. Binary Search Trees (BST) (Week 2-3)
- Properties
- Operations
  - Insertion
  - Deletion
  - Search
  - Finding min/max
  - Successor/Predecessor
- BST Validation

## 4. Advanced Tree Concepts (Week 3-4)
- AVL Trees
- Red-Black Trees
- B-Trees and B+ Trees
- Segment Trees
- Trie (Prefix Trees)

## 5. Common Interview Problems (Week 4-5)
Practice these types of problems:
1. Height/Depth problems
2. Path problems
   - Path Sum
   - Longest Path
   - Path with Maximum Sum
3. View problems
   - Top View
   - Bottom View
   - Left/Right View
4. Lowest Common Ancestor (LCA)
5. Tree Construction
   - From Inorder and Preorder
   - From Inorder and Postorder
6. Tree Modification
   - Flattening
   - Mirroring
   - Balancing

## 6. Advanced Problems (Week 5-6)
- Serialization/Deserialization
- Morris Traversal
- Tree Isomorphism
- Tree DP problems

## Practice Resources
1. LeetCode Tree Problems:
   - Easy: Start with 94, 100, 101, 104, 112
   - Medium: 98, 102, 105, 106, 236
   - Hard: 124, 297, 99

2. Implementation Projects:
   - Build a BST library
   - Implement an AVL tree
   - Create a Trie-based autocomplete system

## Interview Preparation Tips
1. Always start with the brute force approach
2. Think about:
   - Recursive vs Iterative solutions
   - Space-time complexity
   - Edge cases (empty tree, single node, unbalanced tree)

3. Common Patterns to Master:
   - DFS vs BFS usage
   - Using additional data structures (stack, queue)
   - Global variables in recursion
   - Parent pointers
   - Level-wise processing

## Sample Interview Questions to Practice:
1. Check if a binary tree is balanced
2. Convert a BST to a sorted doubly linked list
3. Find the diameter of a binary tree
4. Check if two trees are identical
5. Print all paths from root to leaf

Here's a template for solving tree problems:

```python
def solve_tree_problem(root):
    # 1. Handle base cases
    if not root:
        return None
    
    # 2. Initialize variables if needed
    result = []
    
    # 3. Helper function for recursion if needed
    def dfs(node, current_state):
        # Base case
        if not node:
            return
        
        # Process current node
        # Recurse on children
        dfs(node.left, new_state)
        dfs(node.right, new_state)
    
    # 4. Start the recursion
    dfs(root, initial_state)
    
    # 5. Return result
    return result
```

## Weekly Practice Schedule
- Day 1-2: Learn concepts
- Day 3-4: Implement basic operations
- Day 5-6: Solve easy problems
- Day 7: Review and revise

Remember to:
- Write clean, readable code
- Test with different inputs
- Analyze time and space complexity
- Practice explaining your approach out loud
- Draw the tree structure when solving problems
- Keep track of solved problems and revisit them

---
---

I'll expand on the common algorithms and techniques used with trees.
## 1. Basic Tree Traversal Algorithms

### Depth-First Search (DFS) Variations
```python
class TreeNode:
    def __init__(self, val=0):
        self.val = val
        self.left = None
        self.right = None

# Recursive DFS
def dfs_recursive(root):
    if not root:
        return
    
    # Preorder - Process before recursing
    print(root.val)  # Root
    dfs_recursive(root.left)  # Left
    dfs_recursive(root.right)  # Right

# Iterative DFS using Stack
def dfs_iterative(root):
    if not root:
        return
    
    stack = [root]
    while stack:
        node = stack.pop()
        print(node.val)
        
        # Push right first so left is processed first (LIFO)
        if node.right:
            stack.append(node.right)
        if node.left:
            stack.append(node.left)
```

### Breadth-First Search (BFS)
```python
from collections import deque

def bfs(root):
    if not root:
        return
    
    queue = deque([root])
    while queue:
        level_size = len(queue)
        
        # Process level by level
        for _ in range(level_size):
            node = queue.popleft()
            print(node.val)
            
            if node.left:
                queue.append(node.left)
            if node.right:
                queue.append(node.right)
```

## 2. Binary Search Tree Algorithms

### BST Search
```python
def search_bst(root, target):
    # Iterative approach
    current = root
    while current:
        if target == current.val:
            return current
        elif target < current.val:
            current = current.left
        else:
            current = current.right
    return None
```

### BST Insertion
```python
def insert_bst(root, val):
    if not root:
        return TreeNode(val)
    
    if val < root.val:
        root.left = insert_bst(root.left, val)
    else:
        root.right = insert_bst(root.right, val)
    
    return root
```

### BST Deletion
```python
def delete_bst(root, key):
    if not root:
        return None
    
    # Find the node
    if key < root.val:
        root.left = delete_bst(root.left, key)
    elif key > root.val:
        root.right = delete_bst(root.right, key)
    else:
        # Case 1: Leaf node
        if not root.left and not root.right:
            return None
        
        # Case 2: One child
        if not root.left:
            return root.right
        if not root.right:
            return root.left
        
        # Case 3: Two children
        # Find minimum value in right subtree
        successor = find_min(root.right)
        root.val = successor.val
        root.right = delete_bst(root.right, successor.val)
    
    return root

def find_min(node):
    current = node
    while current.left:
        current = current.left
    return current
```

## 3. Balanced Tree Algorithms

### AVL Tree Rotation Operations
```python
class AVLNode:
    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None
        self.height = 1

def get_height(node):
    if not node:
        return 0
    return node.height

def get_balance(node):
    if not node:
        return 0
    return get_height(node.left) - get_height(node.right)

def right_rotate(y):
    x = y.left
    T2 = x.right
    
    # Perform rotation
    x.right = y
    y.left = T2
    
    # Update heights
    y.height = max(get_height(y.left), get_height(y.right)) + 1
    x.height = max(get_height(x.left), get_height(x.right)) + 1
    
    return x

def left_rotate(x):
    y = x.right
    T2 = y.left
    
    # Perform rotation
    y.left = x
    x.right = T2
    
    # Update heights
    x.height = max(get_height(x.left), get_height(x.right)) + 1
    y.height = max(get_height(y.left), get_height(y.right)) + 1
    
    return y
```

## 4. Advanced Tree Algorithms

### Lowest Common Ancestor (LCA)
```python
def find_lca(root, p, q):
    if not root:
        return None
    
    # If either p or q is the root, that's the LCA
    if root.val == p.val or root.val == q.val:
        return root
    
    # Look for p and q in left and right subtrees
    left = find_lca(root.left, p, q)
    right = find_lca(root.right, p, q)
    
    # If p and q are in different subtrees, root is LCA
    if left and right:
        return root
    
    # Return non-null value
    return left if left else right
```

### Serialize and Deserialize Binary Tree
```python
class Codec:
    def serialize(self, root):
        if not root:
            return "null"
        return f"{root.val},{self.serialize(root.left)},{self.serialize(root.right)}"
    
    def deserialize(self, data):
        def dfs():
            val = next(values)
            if val == "null":
                return None
            
            node = TreeNode(int(val))
            node.left = dfs()
            node.right = dfs()
            return node
        
        values = iter(data.split(','))
        return dfs()
```

### Morris Traversal (Inorder without recursion or stack)
```python
def morris_traversal(root):
    current = root
    while current:
        if not current.left:
            print(current.val)  # Process current
            current = current.right
        else:
            # Find the inorder predecessor
            predecessor = current.left
            while predecessor.right and predecessor.right != current:
                predecessor = predecessor.right
            
            if not predecessor.right:
                # Create temporary link
                predecessor.right = current
                current = current.left
            else:
                # Remove temporary link
                predecessor.right = None
                print(current.val)  # Process current
                current = current.right
```

## 5. Tree Construction Algorithms

### Build Tree from Preorder and Inorder
```python
def buildTree(preorder, inorder):
    if not preorder or not inorder:
        return None
    
    root = TreeNode(preorder[0])
    mid = inorder.index(preorder[0])
    
    root.left = buildTree(preorder[1:mid+1], inorder[:mid])
    root.right = buildTree(preorder[mid+1:], inorder[mid+1:])
    
    return root
```

## 6. Common Tree Operations

### Check if Tree is Balanced
```python
def isBalanced(root):
    def check_height(node):
        if not node:
            return 0
        
        left = check_height(node.left)
        right = check_height(node.right)
        
        # -1 indicates unbalanced subtree
        if left == -1 or right == -1 or abs(left - right) > 1:
            return -1
        
        return max(left, right) + 1
    
    return check_height(root) != -1
```

### Diameter of Binary Tree
```python
def diameterOfBinaryTree(root):
    max_diameter = [0]
    
    def depth(node):
        if not node:
            return 0
        
        left = depth(node.left)
        right = depth(node.right)
        
        # Update max diameter if path through current node is longer
        max_diameter[0] = max(max_diameter[0], left + right)
        
        return max(left, right) + 1
    
    depth(root)
    return max_diameter[0]
```

Time Complexity Analysis:
1. DFS/BFS: O(n) where n is number of nodes
2. BST operations: O(h) where h is height (O(log n) if balanced, O(n) if skewed)
3. AVL Tree operations: O(log n)
4. LCA: O(n)
5. Morris Traversal: O(n) time with O(1) space
6. Tree Construction: O(n)

Remember to:
- Always consider edge cases (null nodes, single node)
- Think about space complexity (recursion stack)
- Consider iterative vs recursive approaches
- Use helper functions for complex operations
- Maintain BST properties when modifying trees
- Balance trees when required

---
---


# Understanding Trees Through Detailed Analogies, Visualizations, and Practical Insights

## 1. Basic Tree Structure

### **Real-life Analogy**: **Family Tree**
- **Root**: The oldest ancestor (e.g., Grandparent).
- **Children**: Direct descendants of a parent.
- **Leaves**: Individuals with no further descendants (youngest generation).
- **Parent**: Direct ancestor of a node.
- **Siblings**: Nodes sharing the same parent.

#### **Family Tree Visual:**
```
           👴 (Root/Grandparent)
          /   \
         👨    👩 (Parents)
        /  \     \
       👧  👦    👶 (Children/Leaves)
```

### **Tree Terminology Explained:**

- **Node**: Basic unit containing data.
- **Edge**: Connection between two nodes.
- **Path**: Sequence of nodes connected by edges.
- **Height of a Tree**: Length of the longest path from root to a leaf.
- **Depth of a Node**: Number of edges from the root to the node.
- **Level of a Node**: Same as depth; root is at level 0.
- **Subtree**: A tree consisting of a node and its descendants.

## 2. Types of Trees

### **Binary Tree**

#### **Analogy**: **Decision Making Process**
Each decision node has two outcomes: "Yes" or "No."

#### **Decision Tree Example:**
```
            Is it raining?
            /           \
       Take umbrella  Is it sunny?
                      /          \
                Wear sunscreen  Regular day
```

#### **Binary Tree Visualization:**
```
        A
       / \
      B   C
     / \   \
    D   E   F
```

### **Binary Search Tree (BST)**

#### **Analogy**: **Library Book Organization**
Books are arranged such that each left child has a lower number (or key) and each right child has a higher number.

#### **BST Visual (Numbers represent book IDs):**
```
            50
          /    \
        30      70
       /  \    /  \
      20  40  60  80
```

### **N-ary Tree**

#### **Analogy**: **File System Directory**
Each folder can contain multiple subfolders or files.

#### **N-ary Tree Visualization:**
```
          Root
        /   |   \
      A     B     C
     / \         / \
    D   E       F   G
```

### **Special Binary Trees**

- **Perfect Binary Tree**: All levels are fully filled.
- **Complete Binary Tree**: All levels are fully filled except possibly the last, which is filled from left to right.
- **Full Binary Tree**: Every node has either 0 or 2 children.
- **Balanced Tree**: Height is minimized; operations are efficient.
- **Unbalanced Tree**: Height is not minimized; can lead to inefficient operations.

## 3. Tree Traversals

### **Analogy**: **Different Ways to Explore a House**

#### **Inorder (Left-Root-Right)**
- **Analogy**: Walking through a house by visiting the basement (left), main floor (root), and upper floor (right).

##### **Visual Output for BST:**
```
    20
  30
    40
50
    60
  70
    80
```

#### **Inorder Traversal Code Example:**
```python
# File: tree_traversals.py

class TreeNode:
    def __init__(self, val=0):
        self.val = val
        self.left = None
        self.right = None

def inorder(root):
    """
    Perform inorder traversal of a binary tree.
    Inorder Traversal Order: Left -> Root -> Right
    """
    if not root:
        return
    inorder(root.left)
    print(root.val, end=' ')
    inorder(root.right)

# Example Usage:
# Constructing the BST:
#         50
#        /  \
#      30    70
#     / \    / \
#   20  40  60  80

if __name__ == "__main__":
    root = TreeNode(50)
    root.left = TreeNode(30)
    root.right = TreeNode(70)
    root.left.left = TreeNode(20)
    root.left.right = TreeNode(40)
    root.right.left = TreeNode(60)
    root.right.right = TreeNode(80)
    
    print("Inorder Traversal:")
    inorder(root)  # Output: 20 30 40 50 60 70 80
```

#### **Preorder (Root-Left-Right)**
- **Analogy**: Entering a room, inspecting the main room (root), then exploring the left and right sub-rooms.

##### **Preorder Traversal Visualization:**
```
50
30
20
40
70
60
80
```

#### **Preorder Traversal Code Example:**
```python
def preorder(root):
    """
    Perform preorder traversal of a binary tree.
    Preorder Traversal Order: Root -> Left -> Right
    """
    if not root:
        return
    print(root.val, end=' ')
    preorder(root.left)
    preorder(root.right)

# Example Usage:
print("\nPreorder Traversal:")
preorder(root)  # Output: 50 30 20 40 70 60 80
```

#### **Postorder (Left-Right-Root)**
- **Analogy**: Cleaning a house by first cleaning all sub-rooms (left and right), then the main room (root).

##### **Postorder Traversal Visualization:**
```
20
40
30
60
80
70
50
```

#### **Postorder Traversal Code Example:**
```python
def postorder(root):
    """
    Perform postorder traversal of a binary tree.
    Postorder Traversal Order: Left -> Right -> Root
    """
    if not root:
        return
    postorder(root.left)
    postorder(root.right)
    print(root.val, end=' ')

# Example Usage:
print("\nPostorder Traversal:")
postorder(root)  # Output: 20 40 30 60 80 70 50
```

#### **Level Order (Breadth-First Search - BFS)**
- **Analogy**: Fire inspection of a building floor by floor.

##### **Level Order Traversal Visualization:**
```
50        # Level 0
30    70   # Level 1
20 40 60 80 # Level 2
```

#### **Level Order Traversal Code Example:**
```python
from collections import deque

def level_order(root):
    """
    Perform level order traversal of a binary tree.
    Traverses the tree level by level from left to right.
    """
    if not root:
        return
    queue = deque([root])
    while queue:
        node = queue.popleft()
        print(node.val, end=' ')
        if node.left:
            queue.append(node.left)
        if node.right:
            queue.append(node.right)

# Example Usage:
print("\nLevel Order Traversal:")
level_order(root)  # Output: 50 30 70 20 40 60 80
```

### **Visualization Tips for Traversals:**
1. **Draw the Tree:** Before implementing traversals, sketch the tree structure.
2. **Use Colors:** Highlight nodes as they are visited.
3. **Trace Steps:** Step through the code manually to understand the traversal order.
4. **Use Online Tools:** Utilize tree visualization tools like [Visualgo](https://visualgo.net/en/bst) to see traversals in action.

## 4. Tree Operations Visualized

### **Insertion in BST**

#### **Analogy**: **Finding Your Seat in a Movie Theater**
- **Start at Entrance (Root)**
- **Lower Number? Go Left Section**
- **Higher Number? Go Right Section**
- **Empty Seat? Sit There!**

#### **Insertion Visualization:**
- **Inserting 35:**
  ```
  Starting at 50:
    35 < 50, go left to 30
  At 30:
    35 > 30, go right to 40
  At 40:
    35 < 40, left child is empty
  Insert 35 as left child of 40
  ```

#### **Insertion Code Example:**
```python
def insert_bst(root, val):
    """
    Insert a value into the BST.
    """
    if not root:
        print(f"Inserting {val} at this position.")
        return TreeNode(val)
    if val < root.val:
        print(f"Going left at {root.val}")
        root.left = insert_bst(root.left, val)
    else:
        print(f"Going right at {root.val}")
        root.right = insert_bst(root.right, val)
    return root

# Example Usage:
print("\nInserting 35:")
root = insert_bst(root, 35)

# Visual Representation After Insertion:
#         50
#        /  \
#      30    70
#     / \    / \
#   20  40  60  80
#      /
#    35
```

### **Deletion in BST**

#### **Analogy**: **Removing a Book from the Library**
- Identify the book (node) to remove.
- Adjust the shelves to maintain order.

#### **Deletion Scenarios:**
1. **Leaf Node**: Simply remove it.
2. **One Child**: Replace node with its child.
3. **Two Children**: Find the inorder successor (smallest in right subtree), replace node's value, and delete successor.

#### **Deletion Visualization:**
- **Deleting 30:**
  - Node 30 has two children (20 and 40).
  - Find inorder successor: 35 (smallest in right subtree of 30).
  - Replace 30 with 35.
  - Delete node 35.

#### **Deletion Code Example:**
```python
def find_min(node):
    """
    Find the node with the minimum value in a BST.
    """
    current = node
    while current.left:
        current = current.left
    return current

def delete_bst(root, key):
    """
    Delete a node with the given key from the BST.
    """
    if not root:
        return root
    if key < root.val:
        print(f"Going left at {root.val} to delete {key}")
        root.left = delete_bst(root.left, key)
    elif key > root.val:
        print(f"Going right at {root.val} to delete {key}")
        root.right = delete_bst(root.right, key)
    else:
        print(f"Found {key}, deleting...")
        # Node with only one child or no child
        if not root.left:
            temp = root.right
            root = None
            return temp
        elif not root.right:
            temp = root.left
            root = None
            return temp
        # Node with two children
        temp = find_min(root.right)
        print(f"Inorder successor is {temp.val}")
        root.val = temp.val
        root.right = delete_bst(root.right, temp.val)
    return root

# Example Usage:
print("\nDeleting 30:")
root = delete_bst(root, 30)

# Visual Representation After Deletion:
#         50
#        /  \
#      35    70
#     / \    / \
#   20  40  60  80
```

### **Search in BST**

#### **Analogy**: **Looking Up a Book in a Library Catalog**
- Start at a central index (root).
- Compare and decide to go left or right based on target.

#### **Search Code Example:**
```python
def search_bst(root, target):
    """
    Search for a target value in the BST.
    Returns the node if found, else None.
    """
    if not root:
        print(f"{target} not found in the BST.")
        return None
    if target == root.val:
        print(f"Found {target} at node.")
        return root
    elif target < root.val:
        print(f"Going left at {root.val} to find {target}")
        return search_bst(root.left, target)
    else:
        print(f"Going right at {root.val} to find {target}")
        return search_bst(root.right, target)

# Example Usage:
print("\nSearching for 60:")
found_node = search_bst(root, 60)  # Output: Found 60 at node.
print("Found Node:", found_node.val if found_node else "None")
```

### **Finding Minimum and Maximum in BST**

#### **Analogy**: **Finding the Smallest and Largest Book IDs**
- **Minimum**: Leftmost node.
- **Maximum**: Rightmost node.

#### **Finding Min and Max Code Example:**
```python
def find_min_value(root):
    """
    Find the minimum value in the BST.
    """
    current = root
    while current.left:
        print(f"Moving left from {current.val} to {current.left.val}")
        current = current.left
    print(f"Minimum value in BST is {current.val}")
    return current.val

def find_max_value(root):
    """
    Find the maximum value in the BST.
    """
    current = root
    while current.right:
        print(f"Moving right from {current.val} to {current.right.val}")
        current = current.right
    print(f"Maximum value in BST is {current.val}")
    return current.val

# Example Usage:
min_val = find_min_value(root)  # Output: 20
max_val = find_max_value(root)  # Output: 80
```

### **Visualization of BST Operations:**

#### **Initial BST:**
```
            50
          /    \
        30      70
       /  \    /  \
     20   40  60   80
         /
        35
```

#### **After Inserting 35:**
```
            50
          /    \
        30      70
       /  \    /  \
     20   40  60   80
         /
        35
```

#### **After Deleting 30:**
```
            50
          /    \
        35      70
       /  \    /  \
     20   40  60   80
```

## 5. Advanced Tree Concepts

### **AVL Trees**

#### **Analogy**: **Balancing a Mobile Hanging**
- **Purpose**: Maintain balance to ensure operations remain efficient.
- **Mechanism**: Perform rotations to keep the tree balanced after insertions and deletions.

#### **AVL Tree Properties:**
- **Balance Factor**: Height difference between left and right subtrees is at most 1.
- **Rotations**:
  - **Right Rotation**
  - **Left Rotation**
  - **Left-Right Rotation**
  - **Right-Left Rotation**

#### **AVL Tree Visualization Before and After Rotation:**
```
Unbalanced AVL Tree:
        30
       /
     20
    /
  10

After Right Rotation:
        20
       /  \
     10    30
```

#### **AVL Tree Rotation Code Example:**
```python
# File: avl_tree.py

class AVLNode:
    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None
        self.height = 1  # New attribute for AVL trees

def get_height(node):
    """
    Get the height of the node.
    """
    if not node:
        return 0
    return node.height

def update_height(node):
    """
    Update the height of the node based on child heights.
    """
    node.height = max(get_height(node.left), get_height(node.right)) + 1

def get_balance(node):
    """
    Get the balance factor of the node.
    """
    if not node:
        return 0
    return get_height(node.left) - get_height(node.right)

def right_rotate(y):
    """
    Perform right rotation.
        y                              x
       / \                           /   \
      x   T3     Right Rotate        z     y
     / \          - - - - - ->      / \   / \
    z   T2                           T1 T2 T3
   /
  T1
    """
    x = y.left
    T2 = x.right

    # Perform rotation
    x.right = y
    y.left = T2

    # Update heights
    update_height(y)
    update_height(x)

    print(f"Performed right rotation on {y.val}")
    return x

def left_rotate(x):
    """
    Perform left rotation.
        x                             y
       / \                           / \
      T1  y        Left Rotate      x   z
         / \      - - - - - ->     / \   \
        T2  z                     T1 T2  T3
    """
    y = x.right
    T2 = y.left

    # Perform rotation
    y.left = x
    x.right = T2

    # Update heights
    update_height(x)
    update_height(y)

    print(f"Performed left rotation on {x.val}")
    return y

def insert_avl(root, key):
    """
    Insert a key into the AVL tree and perform rotations if needed.
    """
    if not root:
        print(f"Inserting {key} as a new AVL node.")
        return AVLNode(key)
    elif key < root.val:
        print(f"Inserting {key} to the left of {root.val}")
        root.left = insert_avl(root.left, key)
    else:
        print(f"Inserting {key} to the right of {root.val}")
        root.right = insert_avl(root.right, key)

    # Update the height of the ancestor node
    update_height(root)

    # Get the balance factor
    balance = get_balance(root)
    print(f"Node {root.val} has balance factor {balance}")

    # Balance the tree
    # Case 1 - Left Left
    if balance > 1 and key < root.left.val:
        return right_rotate(root)
    # Case 2 - Right Right
    if balance < -1 and key > root.right.val:
        return left_rotate(root)
    # Case 3 - Left Right
    if balance > 1 and key > root.left.val:
        root.left = left_rotate(root.left)
        return right_rotate(root)
    # Case 4 - Right Left
    if balance < -1 and key < root.right.val:
        root.right = right_rotate(root.right)
        return left_rotate(root)

    return root

# Example Usage:
if __name__ == "__main__":
    avl_root = None
    elements = [30, 20, 10, 25, 40, 35, 50]
    for elem in elements:
        print(f"\nInserting {elem} into AVL Tree:")
        avl_root = insert_avl(avl_root, elem)

    # Final AVL Tree visualization would require an additional function to print the tree structure.
```

### **Red-Black Trees**

#### **Analogy**: **Traffic Light System**
- Ensures balanced tree using colored nodes (Red and Black) to maintain balance.

#### **Red-Black Tree Properties:**
1. Each node is either red or black.
2. The root is always black.
3. All leaves (NIL nodes) are black.
4. Red nodes cannot have red children (no two reds in a row).
5. Every path from a node to its descendant NIL nodes has the same number of black nodes.

#### **Red-Black Tree Visualization:**
```
        10(B)
       /     \
    5(R)     15(R)
    / \       /  \
  NIL NIL   NIL  NIL
```
*Note: (B) denotes Black, (R) denotes Red*

### **B-Trees and B+ Trees**

#### **Analogy**: **Bookshelf with Multiple Sections**
- Efficient for databases and filesystems where nodes can have multiple children.

#### **B-Tree Properties:**
- All leaf nodes are at the same level.
- A node can have multiple keys and children.
- Designed to minimize disk reads.

#### **B+ Tree Differences:**
- All data is stored at leaf nodes.
- Internal nodes only store keys for navigation.
- Leaf nodes are linked for efficient range queries.

### **Segment Trees**

#### **Analogy**: **Interval Scheduling**
- Efficient for querying ranges and updating intervals.

#### **Use Cases:**
- Range sum queries.
- Range minimum/maximum queries.
- Frequency counts in ranges.

#### **Segment Tree Visualization:**
```
Original Array: [1, 3, 5, 7, 9, 11]
Segment Tree:
                       [36]
                      /    \
                  [9]        [27]
                 /  \       /    \
              [4]    [5]  [16]    [11]
             / \    / \   / \
           [1] [3][5] [7][9][11]
```

### **Trie (Prefix Trees)**

#### **Analogy**: **Dictionary or Autocomplete System**
- Efficient for storing and searching strings with common prefixes.

#### **Use Cases:**
- Autocomplete features.
- Spell checking.
- IP routing.

#### **Trie Visualization:**
```
        Root
       /   \
      c     b
     /       \
    a         a
   /           \
  t             t
 / \
#   s
```
*Words: "cat", "cats", "bat"*

#### **Trie Code Example:**
```python
# File: trie.py

class TrieNode:
    def __init__(self):
        self.children = {}  # Key: character, Value: TrieNode
        self.is_end_of_word = False

class Trie:
    def __init__(self):
        self.root = TrieNode()

    def insert(self, word):
        """
        Insert a word into the Trie.
        """
        node = self.root
        print(f"Inserting '{word}' into Trie:")
        for char in word:
            if char not in node.children:
                print(f"Creating node for character '{char}'")
                node.children[char] = TrieNode()
            node = node.children[char]
            print(f"Moved to node '{char}'")
        node.is_end_of_word = True
        print(f"Word '{word}' inserted.\n")

    def search(self, word):
        """
        Search for a word in the Trie.
        Returns True if the word exists, else False.
        """
        node = self.root
        print(f"Searching for '{word}' in Trie:")
        for char in word:
            if char not in node.children:
                print(f"Character '{char}' not found. Word not present.")
                return False
            node = node.children[char]
            print(f"Moved to node '{char}'")
        if node.is_end_of_word:
            print(f"Word '{word}' found in Trie.\n")
            return True
        else:
            print(f"Word '{word}' not found as complete word in Trie.\n")
            return False

    def starts_with(self, prefix):
        """
        Check if there is any word in the Trie that starts with the given prefix.
        """
        node = self.root
        print(f"Checking prefix '{prefix}' in Trie:")
        for char in prefix:
            if char not in node.children:
                print(f"Character '{char}' not found. No words start with '{prefix}'.")
                return False
            node = node.children[char]
            print(f"Moved to node '{char}'")
        print(f"Prefix '{prefix}' exists in Trie.\n")
        return True

# Example Usage:
if __name__ == "__main__":
    trie = Trie()
    words = ["cat", "cats", "bat"]
    for word in words:
        trie.insert(word)
    
    trie.search("cats")     # Output: True
    trie.search("car")      # Output: False
    trie.starts_with("ca")  # Output: True
    trie.starts_with("ba")  # Output: True
    trie.starts_with("be")  # Output: False
```

## 6. Common Interview Problems Visualized

### **1. Height/Depth Problems**

#### **Find the Height of a Binary Tree**

##### **Analogy**: **Finding the Tallest Branch in a Real Tree**
- The height is the number of edges on the longest downward path from the root to a leaf.

##### **Height Calculation Code Example:**
```python
def tree_height(root):
    """
    Calculate the height of the binary tree.
    """
    if not root:
        return 0
    left_height = tree_height(root.left)
    right_height = tree_height(root.right)
    return max(left_height, right_height) + 1

# Example Usage:
height = tree_height(root)
print(f"\nHeight of the tree: {height}")  # Output depends on current tree structure
```

### **2. Path Problems**

#### **Path Sum**

##### **Analogy**: **Finding a Path in a Maze that Adds Up to a Target Sum**
- Determine if there's a root-to-leaf path where the sum of node values equals a given target.

##### **Path Sum Code Example:**
```python
def has_path_sum(root, target_sum):
    """
    Check if there's a root-to-leaf path with the given sum.
    """
    if not root:
        return False
    target_sum -= root.val
    print(f"At node {root.val}, remaining sum: {target_sum}")
    if not root.left and not root.right:
        return target_sum == 0
    return has_path_sum(root.left, target_sum) or has_path_sum(root.right, target_sum)

# Example Usage:
target = 105  # Example target sum
result = has_path_sum(root, target)
print(f"\nPath sum {target} exists: {result}")
```

#### **Longest Path**

##### **Analogy**: **Identifying the Longest Trail in a Forest**
- Find the length of the longest path from root to any leaf.

##### **Longest Path Code Example:**
```python
def longest_path(root):
    """
    Find the length of the longest path from root to leaf.
    """
    if not root:
        return 0
    left_length = longest_path(root.left)
    right_length = longest_path(root.right)
    return max(left_length, right_length) + 1

# Example Usage:
longest = longest_path(root)
print(f"\nLongest path length: {longest}")
```

### **3. View Problems**

#### **Top View of a Binary Tree**

##### **Analogy**: **Viewing a Tree from Above**
- Nodes visible when the tree is viewed from the top.

##### **Top View Code Example:**
```python
from collections import deque

def top_view(root):
    """
    Print the top view of the binary tree.
    """
    if not root:
        return
    
    # Dictionary to store the first node at each horizontal distance
    hd_node_map = {}
    queue = deque([(root, 0)])  # Pair of node and its horizontal distance

    while queue:
        node, hd = queue.popleft()
        if hd not in hd_node_map:
            hd_node_map[hd] = node.val
            print(f"Node {node.val} at horizontal distance {hd} is visible.")
        if node.left:
            queue.append((node.left, hd - 1))
        if node.right:
            queue.append((node.right, hd + 1))
    
    # Print the top view from left to right
    for hd in sorted(hd_node_map.keys()):
        print(hd_node_map[hd], end=' ')

# Example Usage:
print("\nTop View of the tree:")
top_view(root)  # Output: Nodes visible from the top
```

#### **Bottom View of a Binary Tree**

##### **Analogy**: **Viewing a Tree from Below**
- Nodes visible when the tree is viewed from the bottom.

##### **Bottom View Code Example:**
```python
def bottom_view(root):
    """
    Print the bottom view of the binary tree.
    """
    if not root:
        return
    
    # Dictionary to store the last node at each horizontal distance
    hd_node_map = {}
    queue = deque([(root, 0)])  # Pair of node and its horizontal distance

    while queue:
        node, hd = queue.popleft()
        hd_node_map[hd] = node.val
        print(f"Node {node.val} at horizontal distance {hd} is visible.")
        if node.left:
            queue.append((node.left, hd - 1))
        if node.right:
            queue.append((node.right, hd + 1))
    
    # Print the bottom view from left to right
    for hd in sorted(hd_node_map.keys()):
        print(hd_node_map[hd], end=' ')

# Example Usage:
print("\nBottom View of the tree:")
bottom_view(root)  # Output: Nodes visible from the bottom
```

### **4. Lowest Common Ancestor (LCA)**

#### **Analogy**: **Finding the Nearest Common Ancestor in a Family Tree**
- Identify the deepest node that is an ancestor of both given nodes.

#### **LCA Code Example:**
```python
def find_lca(root, p, q):
    """
    Find the lowest common ancestor of nodes p and q in the BST.
    Assumes both p and q are present in the tree.
    """
    if not root:
        return None
    print(f"Visiting node {root.val} for LCA of {p.val} and {q.val}")
    if p.val < root.val and q.val < root.val:
        return find_lca(root.left, p, q)
    if p.val > root.val and q.val > root.val:
        return find_lca(root.right, p, q)
    print(f"LCA of {p.val} and {q.val} is {root.val}")
    return root

# Example Usage:
node_p = root.left.left  # Node with value 20
node_q = root.left.right.right  # Node with value 35
lca_node = find_lca(root, node_p, node_q)
print(f"\nLowest Common Ancestor: {lca_node.val if lca_node else 'None'}")
```

### **5. Tree Construction**

#### **Build Tree from Inorder and Preorder Traversal**

##### **Analogy**: **Reassembling a Tree from Its Parts**
- Given the inorder and preorder sequences, reconstruct the original tree.

##### **Build Tree Code Example:**
```python
def build_tree(preorder, inorder):
    """
    Build a binary tree from preorder and inorder traversal lists.
    """
    if not preorder or not inorder:
        return None
    # The first element of preorder is the root
    root_val = preorder[0]
    root = TreeNode(root_val)
    print(f"Creating node {root_val} as root.")
    # Find the root in inorder to divide left and right subtrees
    mid = inorder.index(root_val)
    # Recursively build left and right subtrees
    root.left = build_tree(preorder[1:mid+1], inorder[:mid])
    root.right = build_tree(preorder[mid+1:], inorder[mid+1:])
    return root

# Example Usage:
preorder = [50, 30, 20, 40, 35, 70, 60, 80]
inorder = [20, 30, 35, 40, 50, 60, 70, 80]
built_root = build_tree(preorder, inorder)
print("\nInorder Traversal of Built Tree:")
inorder(built_root)  # Should match the inorder list
```

### **6. Tree Modification**

#### **Flattening a Binary Tree to a Linked List**

##### **Analogy**: **Unraveling a Branch into a Straight Line**
- Convert a tree into a right-skewed list while maintaining preorder traversal order.

##### **Flatten Code Example:**
```python
def flatten(root):
    """
    Flatten the binary tree into a linked list in-place.
    """
    if not root:
        return
    flatten(root.left)
    flatten(root.right)
    
    # Store the left and right subtrees
    left = root.left
    right = root.right
    
    # Insert the flattened left subtree between root and flattened right subtree
    root.left = None
    root.right = left
    
    # Find the tail of the new right subtree
    current = root
    while current.right:
        current = current.right
    current.right = right

    print(f"Flattened at node {root.val}")

# Example Usage:
print("\nFlattening the tree:")
flatten(root)

print("\nFlattened Tree Traversal:")
def print_flattened(root):
    while root:
        print(root.val, end=' ')
        root = root.right

print_flattened(root)  # Output should follow preorder: 50 35 20 40 70 60 80
```

#### **Mirroring a Binary Tree**

##### **Analogy**: **Creating a Mirror Image in Water**
- Swap left and right children recursively to create a mirror image.

##### **Mirror Code Example:**
```python
def mirror(root):
    """
    Mirror the binary tree.
    """
    if not root:
        return
    # Swap the left and right children
    root.left, root.right = root.right, root.left
    print(f"Mirrored node {root.val}")
    mirror(root.left)
    mirror(root.right)

# Example Usage:
print("\nMirroring the tree:")
mirror(root)

print("\nInorder Traversal of Mirrored Tree:")
inorder(root)  # Traversal will reflect the mirrored structure
```

#### **Balancing a Binary Tree**

##### **Analogy**: **Rearranging Weights on a Scale to Achieve Balance**
- Use balancing algorithms like AVL or Red-Black to ensure the tree remains balanced after operations.

##### **Balancing Tip:**
- Implement self-balancing trees (AVL, Red-Black) to automate balancing during insertions and deletions.

## 7. Advanced Problems

### **Serialization and Deserialization of Binary Trees**

#### **Analogy**: **Saving and Loading a Tree Structure**
- **Serialization**: Convert a tree into a string representation.
- **Deserialization**: Reconstruct the tree from the string.

#### **Serialize and Deserialize Code Example:**
```python
# File: codec.py

class Codec:
    def serialize(self, root):
        """
        Serialize the binary tree to a string using preorder traversal.
        """
        def helper(node):
            if not node:
                vals.append('N')  # Sentinel for None
                return
            vals.append(str(node.val))
            helper(node.left)
            helper(node.right)
        
        vals = []
        helper(root)
        serialized_str = ' '.join(vals)
        print(f"Serialized Tree: {serialized_str}")
        return serialized_str

    def deserialize(self, data):
        """
        Deserialize the string back to a binary tree.
        """
        def helper():
            val = next(vals)
            if val == 'N':
                return None
            node = TreeNode(int(val))
            node.left = helper()
            node.right = helper()
            return node
        
        vals = iter(data.split())
        root = helper()
        print("Deserialized Tree successfully.")
        return root

# Example Usage:
if __name__ == "__main__":
    codec = Codec()
    serialized = codec.serialize(root)  # Serialize the current tree
    new_root = codec.deserialize(serialized)  # Deserialize to a new tree
    print("\nInorder Traversal of Deserialized Tree:")
    inorder(new_root)  # Should match the original tree's inorder traversal
```

### **Morris Traversal (Inorder without Recursion or Stack)**

#### **Analogy**: **Walking Through a Tree Without a Memory Aid**
- Efficient traversal using threaded binary trees, maintaining O(1) space.

#### **Morris Traversal Code Example:**
```python
def morris_inorder(root):
    """
    Perform inorder traversal using Morris Traversal algorithm.
    """
    current = root
    while current:
        if not current.left:
            print(current.val, end=' ')
            current = current.right
        else:
            # Find the inorder predecessor of current
            predecessor = current.left
            while predecessor.right and predecessor.right != current:
                predecessor = predecessor.right
            if not predecessor.right:
                # Make current the right child of its inorder predecessor
                predecessor.right = current
                print(f"Threading from {predecessor.val} to {current.val}")
                current = current.left
            else:
                # Revert the changes made in the 'if' part
                predecessor.right = None
                print(current.val, end=' ')
                current = current.right

# Example Usage:
print("\nMorris Inorder Traversal:")
morris_inorder(root)  # Output: 20 35 40 50 60 70 80
```

### **Tree Isomorphism**

#### **Analogy**: **Comparing Two Trees for Structural Similarity**
- Determine if two trees have the same structure regardless of node values.

#### **Isomorphic Trees Code Example:**
```python
def are_isomorphic(root1, root2):
    """
    Check if two binary trees are isomorphic.
    """
    if not root1 and not root2:
        return True
    if not root1 or not root2:
        return False
    return (root1.val == root2.val and
            are_isomorphic(root1.left, root2.left) and
            are_isomorphic(root1.right, root2.right))

# Example Usage:
# Assume built_root is another tree constructed similarly
iso = are_isomorphic(root, built_root)
print(f"\nAre the two trees isomorphic? {iso}")
```

### **Tree Dynamic Programming (DP) Problems**

#### **Example Problem**: **Maximum Path Sum**
- **Objective**: Find the path with the maximum sum in the binary tree.

#### **Maximum Path Sum Code Example:**
```python
def max_path_sum(root):
    """
    Find the maximum path sum in the binary tree.
    The path may start and end at any node.
    """
    max_sum = float('-inf')
    
    def helper(node):
        nonlocal max_sum
        if not node:
            return 0
        # Compute max path sum with the current node as the highest node
        left = max(helper(node.left), 0)
        right = max(helper(node.right), 0)
        current_sum = node.val + left + right
        max_sum = max(max_sum, current_sum)
        return node.val + max(left, right)
    
    helper(root)
    return max_sum

# Example Usage:
max_sum = max_path_sum(root)
print(f"\nMaximum Path Sum: {max_sum}")
```

## 8. Practice Resources

### **LeetCode Tree Problems:**
- **Easy:**
  - [Binary Tree Inorder Traversal](https://leetcode.com/problems/binary-tree-inorder-traversal/) (Problem 94)
  - [Symmetric Tree](https://leetcode.com/problems/symmetric-tree/) (Problem 101)
  - [Maximum Depth of Binary Tree](https://leetcode.com/problems/maximum-depth-of-binary-tree/) (Problem 104)
  - [Minimum Depth of Binary Tree](https://leetcode.com/problems/minimum-depth-of-binary-tree/) (Problem 111)
  - [Path Sum](https://leetcode.com/problems/path-sum/) (Problem 112)

- **Medium:**
  - [Binary Tree Maximum Path Sum](https://leetcode.com/problems/binary-tree-maximum-path-sum/) (Problem 124)
  - [Unique Binary Search Trees II](https://leetcode.com/problems/unique-binary-search-trees-ii/) (Problem 95)
  - [Binary Tree Level Order Traversal](https://leetcode.com/problems/binary-tree-level-order-traversal/) (Problem 102)
  - [Construct Binary Tree from Preorder and Inorder Traversal](https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/) (Problem 105)
  - [Binary Tree Zigzag Level Order Traversal](https://leetcode.com/problems/binary-tree-zigzag-level-order-traversal/) (Problem 103)

- **Hard:**
  - [Binary Tree Maximum Path Sum](https://leetcode.com/problems/binary-tree-maximum-path-sum/) (Problem 124)
  - [Serialize and Deserialize N-ary Tree](https://leetcode.com/problems/serialize-and-deserialize-n-ary-tree/) (Problem 428)
  - [Recover Binary Search Tree](https://leetcode.com/problems/recover-binary-search-tree/) (Problem 99)

### **Implementation Projects:**
1. **Build a BST Library**
   - Implement all BST operations (insert, delete, search, traversals).
   - Ensure it handles edge cases and remains balanced if required.

2. **Implement an AVL Tree**
   - Incorporate self-balancing during insertions and deletions.
   - Visualize rotations to understand balancing.

3. **Create a Trie-based Autocomplete System**
   - Build an autocomplete feature that suggests words based on prefixes.
   - Optimize for performance and memory usage.

### **Interactive Learning Tools:**
- **Visualgo**: [Visualize Algorithms](https://visualgo.net/en/bst)
- **Tree Visualizer**: [Binary Tree Visualizer](https://www.cs.usfca.edu/~galles/visualization/BST.html)
- **Coding Platforms**: Use platforms like [LeetCode](https://leetcode.com/), [HackerRank](https://www.hackerrank.com/), and [CodeSignal](https://codesignal.com/) for practice.

## 9. Interview Preparation Tips

### **1. Start with the Brute Force Approach**
- Develop a straightforward solution first.
- Use it to understand the problem deeply before optimizing.

### **2. Analyze Recursive vs Iterative Solutions**
- **Recursive**:
  - Cleaner and more intuitive for tree problems.
  - Utilize the call stack implicitly.
- **Iterative**:
  - Often require explicit stacks or queues.
  - Can be more space-efficient for certain problems.

### **3. Consider Space-Time Complexity**
- **Time Complexity**:
  - Most tree operations are O(n), where n is the number of nodes.
  - Balanced trees offer O(log n) for insertions, deletions, and searches.
- **Space Complexity**:
  - Recursive solutions use O(h) space, where h is the height of the tree.
  - Iterative solutions can use O(n) space in the worst case.

### **4. Handle Edge Cases**
- Empty tree.
- Single-node tree.
- Skewed trees (all left or all right).
- Trees with duplicate values (if applicable).

### **5. Master Common Patterns**
- **DFS vs BFS Usage**:
  - Use DFS for problems requiring exploration to the deepest nodes.
  - Use BFS for level-order traversals and shortest path problems.
  
- **Using Additional Data Structures**:
  - Stacks for iterative DFS.
  - Queues for BFS.
  
- **Global Variables in Recursion**:
  - Useful for keeping track of state across recursive calls.
  
- **Parent Pointers**:
  - Can help in LCA problems or when needing to traverse upwards.

- **Level-wise Processing**:
  - Essential for view problems and breadth-based queries.

### **6. Practice Explaining Your Approach Out Loud**
- Simulate interview scenarios.
- Articulate your thought process clearly and concisely.
- Use diagrams or sketches if allowed.

## 10. Commonly Asked Interview Questions

### **1. Check if a Binary Tree is Balanced**
- **Problem**: Determine if a binary tree is height-balanced.
- **Balanced Tree**: For every node, the height difference between left and right is ≤ 1.

#### **Balanced Tree Code Example:**
```python
def is_balanced(root):
    """
    Check if the binary tree is height-balanced.
    """
    def check(node):
        if not node:
            return 0
        left = check(node.left)
        if left == -1:
            return -1
        right = check(node.right)
        if right == -1:
            return -1
        if abs(left - right) > 1:
            return -1
        return max(left, right) + 1
    
    return check(root) != -1

# Example Usage:
balanced = is_balanced(root)
print(f"\nIs the tree balanced? {balanced}")
```

### **2. Convert a BST to a Sorted Doubly Linked List**
- **Problem**: Transform a BST into a sorted doubly linked list in-place.

#### **Conversion Code Example:**
```python
def bst_to_doubly_linked_list(root):
    """
    Convert BST to a sorted doubly linked list.
    """
    if not root:
        return None
    
    # Inorder traversal to convert BST to DLL
    def inorder(node):
        nonlocal prev, head
        if not node:
            return
        inorder(node.left)
        if prev:
            prev.right = node
            node.left = prev
            print(f"Linking {prev.val} <-> {node.val}")
        else:
            head = node
            print(f"Setting head to {node.val}")
        prev = node
        inorder(node.right)
    
    prev = None
    head = None
    inorder(root)
    return head

# Example Usage:
print("\nConverting BST to Doubly Linked List:")
dll_head = bst_to_doubly_linked_list(root)

print("\nDoubly Linked List Traversal:")
current = dll_head
while current:
    print(current.val, end=' <-> ' if current.right else '')
    current = current.right
```

### **3. Find the Diameter of a Binary Tree**
- **Problem**: The diameter is the length of the longest path between any two nodes in the tree.

#### **Diameter Calculation Code Example:**
```python
def diameter_of_binary_tree(root):
    """
    Calculate the diameter of the binary tree.
    """
    diameter = 0
    
    def depth(node):
        nonlocal diameter
        if not node:
            return 0
        left = depth(node.left)
        right = depth(node.right)
        diameter = max(diameter, left + right)
        return max(left, right) + 1
    
    depth(root)
    return diameter

# Example Usage:
diameter = diameter_of_binary_tree(root)
print(f"\nDiameter of the tree: {diameter}")
```

### **4. Check if Two Trees are Identical**
- **Problem**: Determine if two binary trees are structurally identical and have the same node values.

#### **Identical Trees Code Example:**
```python
def are_identical(root1, root2):
    """
    Check if two binary trees are identical.
    """
    if not root1 and not root2:
        return True
    if not root1 or not root2:
        return False
    return (root1.val == root2.val and
            are_identical(root1.left, root2.left) and
            are_identical(root1.right, root2.right))

# Example Usage:
identical = are_identical(root, built_root)
print(f"\nAre the two trees identical? {identical}")
```

### **5. Print All Paths from Root to Leaf**

#### **Analogy**: **Listing All Trails from Tree Base to Each Leaf**

#### **Print Paths Code Example:**
```python
def print_all_paths(root):
    """
    Print all root-to-leaf paths in the binary tree.
    """
    def helper(node, path):
        if not node:
            return
        path.append(node.val)
        if not node.left and not node.right:
            print(" -> ".join(map(str, path)))
        else:
            helper(node.left, path)
            helper(node.right, path)
        path.pop()
    
    helper(root, [])

# Example Usage:
print("\nAll root-to-leaf paths:")
print_all_paths(root)
```

## 11. Weekly Practice Schedule

### **Week 1: Basic Tree Concepts**
- **Day 1-2**: Learn Tree Terminology and Types.
- **Day 3**: Implement basic tree node structure.
- **Day 4-5**: Practice basic traversals (Inorder, Preorder, Postorder).
- **Day 6-7**: Solve easy traversal problems on LeetCode.

### **Week 2-3: Binary Search Trees (BST)**
- **Day 1-2**: Understand BST properties and implement insertion.
- **Day 3-4**: Implement deletion and search operations.
- **Day 5-6**: Work on traversal-based BST problems.
- **Day 7**: Review and solve medium-level BST problems.

### **Week 4-5: Advanced Tree Concepts and Interview Problems**
- **Day 1-2**: Learn about AVL and Red-Black Trees, implement rotations.
- **Day 3-4**: Study and implement Tries and Segment Trees.
- **Day 5-6**: Practice common interview problems (LCA, Diameter, Path Sum).
- **Day 7**: Solve mixed difficulty problems and revisit challenging ones.

### **Week 6: Advanced Problems and Optimization**
- **Day 1-2**: Implement serialization/deserialization and Morris Traversal.
- **Day 3-4**: Solve advanced DP tree problems.
- **Day 5-6**: Work on optimizing existing solutions for better time and space.
- **Day 7**: Final review, mock interviews, and solving timed problems.

## 12. Common Practices and Use Cases

### **Common Practices:**
1. **Understand the Problem Thoroughly**
   - Read the problem statement multiple times.
   - Identify input and output formats.

2. **Choose the Right Traversal Strategy**
   - Match traversal methods to the problem requirements.

3. **Optimize for Time and Space**
   - Aim for O(n) time complexity where possible.
   - Use O(1) auxiliary space with techniques like Morris Traversal.

4. **Handle Edge Cases Gracefully**
   - Empty trees, single-node trees, skewed trees, etc.

5. **Write Clean and Readable Code**
   - Use meaningful variable names.
   - Comment complex sections for clarity.

6. **Test with Multiple Inputs**
   - Validate solutions against various test cases.

### **Real-World Use Cases:**
1. **Databases and Filesystems**
   - B-Trees and B+ Trees for indexing and efficient data retrieval.

2. **Network Routing**
   - Tries for IP routing and prefix matching.

3. **Compilers**
   - Abstract Syntax Trees (AST) for code parsing and transformation.

4. **Autocomplete Features**
   - Tries for suggesting word completions based on prefixes.

5. **Gaming**
   - Decision trees and quad-trees for game AI and spatial partitioning.

6. **Social Networks**
   - Representing hierarchical relationships like friendships and followings.

## 13. Additional Tips and Best Practices

### **1. Visualize with Diagrams:**
- Draw the tree structure on paper or use digital tools.
- Highlight paths or nodes as you work through problems.

### **2. Utilize Helper Functions:**
- Break down complex operations into smaller helper functions.
- For example, a helper for rotations in AVL trees.

### **3. Maintain Consistent Coding Style:**
- Follow consistent indentation and naming conventions.
- Enhance readability and maintainability.

### **4. Practice Regularly:**
- Consistent practice is key to mastering tree problems.
- Use a variety of problems to cover different aspects of trees.

### **5. Learn from Solutions:**
- After attempting a problem, study efficient solutions.
- Understand different approaches and their trade-offs.

### **6. Time Management During Interviews:**
- Allocate time efficiently between understanding the problem, coding, and testing.
- Prioritize writing a correct and working solution before optimizing.

## 14. Frequently Asked Interview Questions

### **1. Validate a Binary Search Tree (BST)**
- **Problem**: Determine if a given binary tree is a valid BST.
  
#### **Validate BST Code Example:**
```python
def is_valid_bst(root):
    """
    Validate if a binary tree is a valid BST.
    """
    def helper(node, lower, upper):
        if not node:
            return True
        val = node.val
        if val <= lower or val >= upper:
            print(f"Node {val} violates BST properties with bounds ({lower}, {upper})")
            return False
        if not helper(node.left, lower, val):
            return False
        if not helper(node.right, val, upper):
            return False
        return True
    
    return helper(root, float('-inf'), float('inf'))

# Example Usage:
valid = is_valid_bst(root)
print(f"\nIs the tree a valid BST? {valid}")
```

### **2. Recover Binary Search Tree**
- **Problem**: Two nodes of a BST are swapped by mistake. Recover the tree without changing its structure.
  
#### **Recover BST Code Example:**
```python
def recover_tree(root):
    """
    Recover the swapped nodes in the BST.
    """
    first = second = prev = None
    
    def inorder(node):
        nonlocal first, second, prev
        if not node:
            return
        inorder(node.left)
        if prev and node.val < prev.val:
            if not first:
                first = prev
                print(f"First anomaly found at node {prev.val}")
            second = node
            print(f"Second anomaly found at node {node.val}")
        prev = node
        inorder(node.right)
    
    inorder(root)
    if first and second:
        print(f"Swapping {first.val} and {second.val}")
        first.val, second.val = second.val, first.val

# Example Usage:
# Assume two nodes in the BST have been swapped
print("\nRecovering the BST:")
recover_tree(root)

print("\nInorder Traversal After Recovery:")
inorder(root)  # Should display nodes in ascending order
```

### **3. Serialize and Deserialize N-ary Tree**
- **Problem**: Design algorithms to serialize and deserialize an N-ary tree.

#### **Serialize and Deserialize N-ary Tree Code Example:**
```python
# File: codec_nary.py

class NaryTreeNode:
    def __init__(self, val=None, children=None):
        self.val = val
        self.children = children if children is not None else []

class NaryCodec:
    def serialize(self, root):
        """
        Serialize N-ary tree to string.
        """
        serialized = []
        def dfs(node):
            if not node:
                return
            serialized.append(str(node.val))
            serialized.append(str(len(node.children)))  # Number of children
            for child in node.children:
                dfs(child)
        dfs(root)
        serialized_str = ' '.join(serialized)
        print(f"Serialized N-ary Tree: {serialized_str}")
        return serialized_str

    def deserialize(self, data):
        """
        Deserialize string back to N-ary tree.
        """
        if not data:
            return None
        tokens = iter(data.split())
        def dfs():
            val = next(tokens)
            node = NaryTreeNode(int(val))
            num_children = int(next(tokens))
            for _ in range(num_children):
                child = dfs()
                node.children.append(child)
            return node
        root = dfs()
        print("Deserialized N-ary Tree successfully.")
        return root

# Example Usage:
if __name__ == "__main__":
    # Construct N-ary Tree:
    #        1
    #      / | \
    #     2  3  4
    #       / \
    #      5   6
    root = NaryTreeNode(1, [
        NaryTreeNode(2),
        NaryTreeNode(3, [NaryTreeNode(5), NaryTreeNode(6)]),
        NaryTreeNode(4)
    ])
    
    codec = NaryCodec()
    serialized = codec.serialize(root)
    new_root = codec.deserialize(serialized)
    
    # Function to print N-ary tree
    def print_nary_tree(node, level=0):
        if not node:
            return
        print("  " * level + f"Node {node.val}")
        for child in node.children:
            print_nary_tree(child, level + 1)
    
    print("\nDeserialized N-ary Tree Structure:")
    print_nary_tree(new_root)
```

