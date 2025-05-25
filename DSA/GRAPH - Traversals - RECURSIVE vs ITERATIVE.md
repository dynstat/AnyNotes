Below is a detailed explanation of common graph traversal methods, along with fully commented examples for both recursive and iterative algorithms. We will use a simple graph represented as an adjacency list (a Python dictionary) and cover:

- **Depth-First Search (DFS)**  
  • Recursive  
  • Iterative

- **Breadth-First Search (BFS)** (iterative)

We’ll also explain the thought process behind each approach.

---

## Graph Representation

Let’s create our own sample graph. In this graph, the nodes are represented by letters, and the edges are stored as list values in a dictionary. For example:

- **Graph Structure:**  
  - A connects to B and C  
  - B connects to D and E  
  - C connects to F  
  - D, E, and F have no outgoing edges  

The graph looks like this (directed for simplicity):

```
      A
     / \
    B   C
   / \   \
  D   E   F
```

---

## 1. Depth-First Search (DFS)

DFS explores as far as possible along each branch before backtracking. There are two common ways to implement DFS:

### **a) DFS – Recursive Approach**

**Conceptual Steps:**

1. **Start at a Node:** Visit the starting vertex and mark it as visited.
2. **Recurse on Neighbors:** For each neighbor of the current vertex that hasn’t been visited, call the function recursively.
3. **Backtracking:** Once all paths from the node are explored, the recursion unwinds.

**Notes:**  
- The call stack implicitly serves as a stack to remember the nodes to process next.  
- A *visited* set is used to avoid reprocessing nodes (especially in cases where graphs contain cycles).

```python:title
# File: graph_traversals.py:path/to/graph_traversals.py
def dfs_recursive(graph, start, visited=None):
    """
    Depth-First Search (DFS) Recursive Implementation.
    
    Args:
        graph (dict): A dictionary representing the adjacency list of the graph.
        start: The starting vertex.
        visited (set, optional): Set of already visited nodes. If None, it will be initialized.

    This function prints the node as soon as it is visited.
    """
    if visited is None:
        visited = set()

    # Mark the current node as visited and process it (print it)
    visited.add(start)
    print(start, end=" ")

    # Recurse over all the neighbors
    for neighbor in graph[start]:
        if neighbor not in visited:
            dfs_recursive(graph, neighbor, visited)
```

**Detailed Explanation:**

- **Initialization:**  
  The function checks if the `visited` set is provided. If not, it initializes one.
  
- **Visiting Nodes:**  
  The current node (`start`) is added to the `visited` set and is printed.

- **Recursion:**  
  For each neighbor in the adjacency list of the current node, a recursive call is made if that neighbor has not yet been visited.

---

### **b) DFS – Iterative Approach**

**Conceptual Steps:**

1. **Initialize a Stack:** Start with a stack containing the starting node.
2. **Process Nodes:** While the stack is not empty, pop a node, mark it as visited, and process it.
3. **Push Neighbors:** Push the unvisited neighbors onto the stack.  
   *Tip:* Pushing the neighbors in **reverse order** can help achieve an order similar to the recursive solution.

**Notes:**  
- This method uses an explicit stack to simulate the call stack used in recursion.

```python:title
# File: graph_traversals.py:path/to/graph_traversals.py
def dfs_iterative(graph, start):
    """
    Depth-First Search (DFS) Iterative Implementation.
    
    Args:
        graph (dict): The graph represented as an adjacency list.
        start: The starting vertex.
    
    This function uses an explicit stack to perform DFS and prints the visited nodes.
    """
    visited = set()      # To keep track of visited nodes
    stack = [start]      # Initialize the stack with the starting node

    while stack:
        # Pop a node from the stack
        node = stack.pop()
        if node not in visited:
            visited.add(node)
            print(node, end=" ")
            # Push all unvisited neighbors in reverse order
            # This ensures that the left-most neighbor (first in the list) is processed first.
            for neighbor in reversed(graph[node]):
                if neighbor not in visited:
                    stack.append(neighbor)
```

**Detailed Explanation:**

- **Stack Initialization:**  
  A list named `stack` is used to store nodes to visit, and the starting node is placed on it.

- **Processing Loop:**  
  The loop continues until the stack is empty. On each iteration, a node is popped:
  
  - If it hasn’t been visited, it is marked as visited and printed.
  - Its neighbors are then pushed onto the stack. Reversing the list of neighbors helps maintain the same order as the recursive approach (if the original order is important).

---

## 2. Breadth-First Search (BFS) – Iterative Approach

BFS explores the graph layer by layer, starting at the root (or starting vertex) and gradually expanding to all reachable nodes.

**Conceptual Steps:**

1. **Initialize a Queue:** Start with a queue containing the starting node.
2. **Process Nodes:** Dequeue a node, mark it as visited, and print it.
3. **Enqueue Neighbors:** Add all unvisited neighbors to the queue.

**Notes:**  
- BFS is useful when you need to find the shortest path on unweighted graphs.
- A `deque` from the `collections` module is commonly used for efficient queue operations.

```python:title
# File: graph_traversals.py:path/to/graph_traversals.py
from collections import deque

def bfs_iterative(graph, start):
    """
    Breadth-First Search (BFS) Iterative Implementation.
    
    Args:
        graph (dict): The graph represented as an adjacency list.
        start: The starting vertex.
    
    This function uses a queue (deque) to perform BFS and prints the visited nodes.
    """
    visited = set()         # Set to keep track of visited nodes.
    queue = deque([start])  # Initialize the queue with the starting node.
    visited.add(start)      # Mark the starting node as visited.

    while queue:
        node = queue.popleft()  # Dequeue a node from the front.
        print(node, end=" ")
        # Enqueue all unvisited neighbors and mark them visited.
        for neighbor in graph[node]:
            if neighbor not in visited:
                visited.add(neighbor)
                queue.append(neighbor)
```

**Detailed Explanation:**

- **Queue Initialization:**  
  We initialize a queue (using `deque`) with the starting node. The starting node is immediately marked as visited to avoid duplicate entries (especially important in cyclic graphs).

- **Processing Loop:**  
  In each iteration, the node at the front of the queue is processed (printed), and all its unvisited neighbors are enqueued and marked as visited. This ensures that all nodes at the current depth are processed before moving on to the next depth level.

---

## Bringing It All Together

Below is the complete file with our sample graph and all three traversal functions. We also include a main section to demonstrate running the traversals.

```python:title
# File: graph_traversals.py:path/to/graph_traversals.py
from collections import deque

# Sample graph represented as an adjacency list.
# Graph Structure:
#       A
#      / \
#     B   C
#    / \   \
#   D   E   F
graph = {
    'A': ['B', 'C'],
    'B': ['D', 'E'],
    'C': ['F'],
    'D': [],
    'E': ['F'],
    'F': []
}

def dfs_recursive(graph, start, visited=None):
    """
    Depth-First Search (DFS) Recursive Implementation.
    
    Args:
        graph (dict): A dictionary representing the adjacency list of the graph.
        start: The starting vertex.
        visited (set, optional): Set of already visited nodes. If None, it will be initialized.

    This function prints the node as soon as it is visited.
    """
    if visited is None:
        visited = set()

    visited.add(start)      # Mark the current node as visited
    print(start, end=" ")   # Process the current node

    # Recurse over all the neighbors
    for neighbor in graph[start]:
        if neighbor not in visited:
            dfs_recursive(graph, neighbor, visited)

def dfs_iterative(graph, start):
    """
    Depth-First Search (DFS) Iterative Implementation.
    
    Args:
        graph (dict): The graph represented as an adjacency list.
        start: The starting vertex.
    
    This function uses an explicit stack to perform DFS and prints the visited nodes.
    """
    visited = set()      # To keep track of visited nodes
    stack = [start]      # Initialize the stack with the starting node

    while stack:
        node = stack.pop()  # Pop a node from the stack
        if node not in visited:
            visited.add(node)
            print(node, end=" ")
            # Push all unvisited neighbors in reverse order
            for neighbor in reversed(graph[node]):
                if neighbor not in visited:
                    stack.append(neighbor)

def bfs_iterative(graph, start):
    """
    Breadth-First Search (BFS) Iterative Implementation.
    
    Args:
        graph (dict): The graph represented as an adjacency list.
        start: The starting vertex.
    
    This function uses a queue (deque) to perform BFS and prints the visited nodes.
    """
    visited = set()         # Set to keep track of visited nodes.
    queue = deque([start])  # Initialize the queue with the starting node.
    visited.add(start)      # Mark the starting node as visited.

    while queue:
        node = queue.popleft()  # Dequeue a node from the front.
        print(node, end=" ")
        # Enqueue all unvisited neighbors and mark them visited.
        for neighbor in graph[node]:
            if neighbor not in visited:
                visited.add(neighbor)
                queue.append(neighbor)

# Example usage
if __name__ == '__main__':
    print("DFS Recursive Traversal:")
    dfs_recursive(graph, 'A')      # Expected order: A B D E F C
    print("\n\nDFS Iterative Traversal:")
    dfs_iterative(graph, 'A')        # Expected order: A C F B E D  (depending on neighbor reversal)
    print("\n\nBFS Iterative Traversal:")
    bfs_iterative(graph, 'A')        # Expected order: A B C D E F
```

---

## Summary of the Mental Approach

1. **DFS (Recursive):**
   - **Conceptualize** the problem as “visit the node, then visit all its neighbors.”
   - **Use Recursion** to let the call stack remember the path.
   - **Keep Track** of visited nodes to avoid processing nodes more than once.

2. **DFS (Iterative):**
   - **Simulate the Call Stack** with an explicit stack.
   - **Push Neighbors:** Decide the order you'd like to process the neighbors (often reversed relative to the recursive order).

3. **BFS (Iterative):**
   - **Use a Queue** to process nodes in the order they are discovered – this naturally proceeds level by level.
   - **Mark Nodes as Visited** as soon as you add them to the queue to prevent duplicates.

These techniques are foundational for many more complex graph algorithms like pathfinding, cycle detection, and topological sorting.

