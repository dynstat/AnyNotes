Let's delve deeply into the **Graph** data structure, covering its fundamentals, various representations, comprehensive operations with detailed code examples, best practices, and a dedicated section tailored for interview preparation. 

---

## Table of Contents

1. [Introduction to Graphs](#introduction-to-graphs)
2. [Graph Representations](#graph-representations)
   - [Adjacency List](#adjacency-list)
   - [Adjacency Matrix](#adjacency-matrix)
   - [Edge List](#edge-list)
3. [Graph Implementation in Python](#graph-implementation-in-python)
   - [Using Adjacency List](#using-adjacency-list)
   - [Using Adjacency Matrix](#using-adjacency-matrix)
   - [Using Edge List](#using-edge-list)
4. [Graph Operations with Code](#graph-operations-with-code)
   - [Adding a Vertex](#adding-a-vertex)
   - [Adding an Edge](#adding-an-edge)
   - [Removing a Vertex](#removing-a-vertex)
   - [Removing an Edge](#removing-an-edge)
   - [Getting Neighbors](#getting-neighbors)
   - [Displaying the Graph](#displaying-the-graph)
5. [Graph Traversal Algorithms](#graph-traversal-algorithms)
   - [Depth-First Search (DFS)](#depth-first-search-dfs)
     - [Recursive DFS](#recursive-dfs)
     - [Iterative DFS](#iterative-dfs)
   - [Breadth-First Search (BFS)](#breadth-first-search-bfs)
     - [Iterative BFS](#iterative-bfs)
6. [Advanced Graph Algorithms](#advanced-graph-algorithms)
   - [Cycle Detection](#cycle-detection)
   - [Topological Sorting](#topological-sorting)
   - [Shortest Path Algorithms](#shortest-path-algorithms)
     - [Dijkstra’s Algorithm](#dijkstras-algorithm)
     - [Bellman-Ford Algorithm](#bellman-ford-algorithm)
   - [Connected Components](#connected-components)
7. [Best Practices in Graph Implementation](#best-practices-in-graph-implementation)
   - [Choosing the Right Representation](#choosing-the-right-representation)
   - [Handling Edge Cases](#handling-edge-cases)
   - [Optimizing for Performance](#optimizing-for-performance)
8. [Graph Data Structure: Interview Preparation](#graph-data-structure-interview-preparation)
   - [Common Interview Problems](#common-interview-problems)
   - [Problem-Solving Strategies](#problem-solving-strategies)
   - [Sample Interview Questions and Solutions](#sample-interview-questions-and-solutions)
9. [Conclusion](#conclusion)

---

## Introduction to Graphs

A **Graph** is a data structure consisting of a finite set of **vertices** (also called **nodes**) and a set of **edges** that connect pairs of vertices. Graphs are incredibly versatile and are used to model numerous real-world systems such as social networks, transportation systems, web page link structures, and more.

**Key Characteristics:**

- **Directed vs. Undirected:**
  - *Directed Graph (Digraph):* Edges have a direction (from source to destination).
  - *Undirected Graph:* Edges have no direction; they simply connect two vertices.

- **Weighted vs. Unweighted:**
  - *Weighted Graph:* Edges carry weights (e.g., cost, distance).
  - *Unweighted Graph:* Edges do not carry weights; they are simply connections.

- **Cyclic vs. Acyclic:**
  - *Cyclic Graph:* Contains cycles (a path that starts and ends at the same vertex).
  - *Acyclic Graph:* Contains no cycles.

- **Connected vs. Disconnected:**
  - *Connected Graph:* There's a path between every pair of vertices.
  - *Disconnected Graph:* At least two vertices have no path connecting them.

---

## Graph Representations

Graphs can be represented in various ways, each with its advantages and trade-offs. The primary representations are:

### Adjacency List

An **Adjacency List** represents a graph as a collection of lists. Each list corresponds to a vertex and contains the vertices adjacent to it.

**Advantages:**

- **Space Efficient:** Ideal for sparse graphs.
- **Efficient Iteration:** Quickly access all neighbors of a vertex.

**Disadvantages:**

- **Edge Existence Check:** Not as efficient as adjacency matrices for checking the existence of a specific edge.

**Example:**

```plaintext
Graph:
    A
   / \
  B   C
 / \   \
D   E   F

Adjacency List:
{
    'A': ['B', 'C'],
    'B': ['D', 'E'],
    'C': ['F'],
    'D': [],
    'E': [],
    'F': []
}
```

### Adjacency Matrix

An **Adjacency Matrix** uses a 2D array to represent connections between vertices. The cell at row *i* and column *j* indicates the presence (and possibly the weight) of an edge from vertex *i* to vertex *j*.

**Advantages:**

- **Constant-Time Edge Checks:** Quickly determine if an edge exists between two vertices.
- **Simplicity:** Easy to implement for small graphs.

**Disadvantages:**

- **Space Inefficient:** Uses O(V²) space, making it unsuitable for large, sparse graphs.
- **Inefficient Iteration:** Iterating over neighbors requires checking each cell in a row.

**Example:**

```plaintext
Graph:
    A
   / \
  B   C
 / \   \
D   E   F

Adjacency Matrix:
    A B C D E F
A [0 1 1 0 0 0]
B [0 0 0 1 1 0]
C [0 0 0 0 0 1]
D [0 0 0 0 0 0]
E [0 0 0 0 0 0]
F [0 0 0 0 0 0]
```

### Edge List

An **Edge List** represents a graph as a list of its edges. Each edge is a pair (or tuple) of vertices, and optionally, the weight.

**Advantages:**

- **Space Efficient for Sparse Graphs:** Particularly when edges are stored as pairs.
- **Simplistic Representation:** Easy to iterate over all edges.

**Disadvantages:**

- **Edge Checks:** Inefficient for checking if a specific edge exists.
- **Neighbor Access:** Requires iterating through all edges to find neighbors.

**Example:**

```plaintext
Graph:
    A
   / \
  B   C
 / \   \
D   E   F

Edge List:
[('A', 'B'), ('A', 'C'), ('B', 'D'), ('B', 'E'), ('C', 'F')]
```

---

## Graph Implementation in Python

Let’s explore comprehensive implementations of Graphs in Python using different representations, enabling various operations effectively.

### Using Adjacency List

The adjacency list is the most commonly used graph representation, especially for algorithms that need to iterate over neighbors efficiently.

#### Implementation

**File: `graph_adjacency_list.py`**

```python:graph_adjacency_list.py:path/to/graph_adjacency_list.py
from typing import Any, Dict, List, Tuple, Optional

class GraphAdjacencyList:
    """
    Graph implementation using an Adjacency List representation.
    """

    def __init__(self, directed: bool = False):
        """
        Initialize the graph.
        
        Args:
            directed (bool): If True, graph is directed; otherwise, undirected.
        """
        self.directed: bool = directed
        self.adj_list: Dict[Any, List[Tuple[Any, Optional[int]]]] = {}
    
    def add_vertex(self, vertex: Any) -> None:
        """
        Add a vertex to the graph.
        
        Args:
            vertex (Any): The vertex to add.
            
        Best Practice:
            - Check if vertex already exists to prevent duplication.
        """
        if vertex not in self.adj_list:
            self.adj_list[vertex] = []
            print(f"Vertex '{vertex}' added.")
        else:
            print(f"Vertex '{vertex}' already exists.")
    
    def add_edge(self, source: Any, destination: Any, weight: Optional[int] = None) -> None:
        """
        Add an edge to the graph.
        
        Args:
            source (Any): The starting vertex.
            destination (Any): The ending vertex.
            weight (Optional[int]): The weight of the edge. If None, treated as unweighted.
            
        Best Practices:
            - Add both vertices if they do not exist.
            - For undirected graphs, add edge in both directions.
        """
        if source not in self.adj_list:
            self.add_vertex(source)
        if destination not in self.adj_list:
            self.add_vertex(destination)
        
        self.adj_list[source].append((destination, weight))
        print(f"Edge added from '{source}' to '{destination}' with weight {weight}.")
        
        if not self.directed:
            self.adj_list[destination].append((source, weight))
            print(f"Edge added from '{destination}' to '{source}' with weight {weight} (undirected).")
    
    def remove_edge(self, source: Any, destination: Any) -> None:
        """
        Remove an edge from the graph.
        
        Args:
            source (Any): The starting vertex.
            destination (Any): The ending vertex.
            
        Best Practices:
            - Check if the edge exists before attempting to remove.
            - For undirected graphs, remove both directions.
        """
        if source in self.adj_list:
            original_length = len(self.adj_list[source])
            self.adj_list[source] = [
                (node, wt) for (node, wt) in self.adj_list[source] if node != destination
            ]
            if len(self.adj_list[source]) < original_length:
                print(f"Edge removed from '{source}' to '{destination}'.")
        if not self.directed:
            if destination in self.adj_list:
                original_length = len(self.adj_list[destination])
                self.adj_list[destination] = [
                    (node, wt) for (node, wt) in self.adj_list[destination] if node != source
                ]
                if len(self.adj_list[destination]) < original_length:
                    print(f"Edge removed from '{destination}' to '{source}' (undirected).")
    
    def remove_vertex(self, vertex: Any) -> None:
        """
        Remove a vertex and all associated edges from the graph.
        
        Args:
            vertex (Any): The vertex to remove.
            
        Best Practices:
            - Remove the vertex from all adjacency lists.
        """
        if vertex in self.adj_list:
            # Remove all edges from this vertex
            del self.adj_list[vertex]
            print(f"Vertex '{vertex}' removed.")
            
            # Remove edges pointing to this vertex
            for src in self.adj_list:
                original_length = len(self.adj_list[src])
                self.adj_list[src] = [
                    (node, wt) for (node, wt) in self.adj_list[src] if node != vertex
                ]
                if len(self.adj_list[src]) < original_length:
                    print(f"Edge removed from '{src}' to '{vertex}'.")
    
    def get_neighbors(self, vertex: Any) -> List[Tuple[Any, Optional[int]]]:
        """
        Get all neighbors of a vertex.
        
        Args:
            vertex (Any): The vertex whose neighbors are to be retrieved.
        
        Returns:
            List[Tuple[Any, Optional[int]]]: A list of tuples containing neighbor vertices and edge weights.
        """
        return self.adj_list.get(vertex, [])
    
    def __str__(self) -> str:
        """
        Return a string representation of the graph.
        """
        graph_str = ""
        for vertex in self.adj_list:
            neighbors = ', '.join([f"{nbr}({wt})" if wt is not None else f"{nbr}" for nbr, wt in self.adj_list[vertex]])
            graph_str += f"{vertex} -> {neighbors}\n"
        return graph_str
```

#### Explanation

- **Initialization (`__init__`):**
  - **`directed`:** Indicates if the graph is directed (`True`) or undirected (`False`).
  - **`adj_list`:** A dictionary where each key is a vertex, and the value is a list of tuples representing the adjacent vertices and their corresponding edge weights.

- **Adding Vertices (`add_vertex`):**
  - Checks for the existence of the vertex to avoid duplication.
  - Initializes an empty list for storing neighbors.

- **Adding Edges (`add_edge`):**
  - Ensures both source and destination vertices exist by adding them if necessary.
  - Appends the destination (and weight if provided) to the source's adjacency list.
  - For undirected graphs, also appends the source to the destination's adjacency list.

- **Removing Edges (`remove_edge`):**
  - Filters out the specified edge from the source's adjacency list.
  - For undirected graphs, removes the opposite direction as well.

- **Removing Vertices (`remove_vertex`):**
  - Deletes the vertex from the adjacency list.
  - Iterates through all vertices to remove edges pointing to the deleted vertex.

- **Getting Neighbors (`get_neighbors`):**
  - Retrieves the list of adjacent vertices and their weights for a given vertex.

- **String Representation (`__str__`):**
  - Provides a readable string representation of the graph, showing each vertex and its connected neighbors with weights.

### Using Adjacency Matrix

The adjacency matrix represents a graph as a 2D matrix. Each cell `(i, j)` indicates the presence (and weight) of an edge from vertex `i` to vertex `j`.

#### Implementation

**File: `graph_adjacency_matrix.py`**

```python:graph_adjacency_matrix.py:path/to/graph_adjacency_matrix.py
from typing import Any, List, Optional

class GraphAdjacencyMatrix:
    """
    Graph implementation using an Adjacency Matrix representation.
    """

    def __init__(self, vertices: Optional[List[Any]] = None, directed: bool = False):
        """
        Initialize the graph with a list of vertices.
        
        Args:
            vertices (Optional[List[Any]]): Initial list of vertices.
            directed (bool): If True, graph is directed; otherwise, undirected.
        """
        self.directed: bool = directed
        self.vertices: List[Any] = []
        self.vertex_indices: Dict[Any, int] = {}
        self.adj_matrix: List[List[Optional[int]]] = []
        
        if vertices:
            for vertex in vertices:
                self.add_vertex(vertex)
    
    def add_vertex(self, vertex: Any) -> None:
        """
        Add a vertex to the graph.
        
        Args:
            vertex (Any): The vertex to add.
        """
        if vertex in self.vertex_indices:
            print(f"Vertex '{vertex}' already exists.")
            return
        
        self.vertices.append(vertex)
        self.vertex_indices[vertex] = len(self.vertices) - 1
        # Expand the adjacency matrix with a new row and column
        for row in self.adj_matrix:
            row.append(None)
        self.adj_matrix.append([None] * len(self.vertices))
        print(f"Vertex '{vertex}' added.")
    
    def add_edge(self, source: Any, destination: Any, weight: Optional[int] = 1) -> None:
        """
        Add an edge to the graph.
        
        Args:
            source (Any): The starting vertex.
            destination (Any): The ending vertex.
            weight (Optional[int]): The weight of the edge. Default is 1.
        """
        if source not in self.vertex_indices:
            self.add_vertex(source)
        if destination not in self.vertex_indices:
            self.add_vertex(destination)
        
        src_idx = self.vertex_indices[source]
        dest_idx = self.vertex_indices[destination]
        self.adj_matrix[src_idx][dest_idx] = weight
        print(f"Edge added from '{source}' to '{destination}' with weight {weight}.")
        
        if not self.directed:
            self.adj_matrix[dest_idx][src_idx] = weight
            print(f"Edge added from '{destination}' to '{source}' with weight {weight} (undirected).")
    
    def remove_edge(self, source: Any, destination: Any) -> None:
        """
        Remove an edge from the graph.
        
        Args:
            source (Any): The starting vertex.
            destination (Any): The ending vertex.
        """
        if source in self.vertex_indices and destination in self.vertex_indices:
            src_idx = self.vertex_indices[source]
            dest_idx = self.vertex_indices[destination]
            self.adj_matrix[src_idx][dest_idx] = None
            print(f"Edge removed from '{source}' to '{destination}'.")
            
            if not self.directed:
                self.adj_matrix[dest_idx][src_idx] = None
                print(f"Edge removed from '{destination}' to '{source}' (undirected).")
    
    def remove_vertex(self, vertex: Any) -> None:
        """
        Remove a vertex and all associated edges from the graph.
        
        Args:
            vertex (Any): The vertex to remove.
        """
        if vertex not in self.vertex_indices:
            print(f"Vertex '{vertex}' does not exist.")
            return
        
        idx = self.vertex_indices[vertex]
        # Remove the vertex from vertices list
        self.vertices.pop(idx)
        # Remove the corresponding row and column from adjacency matrix
        self.adj_matrix.pop(idx)
        for row in self.adj_matrix:
            row.pop(idx)
        # Update vertex indices
        self.vertex_indices = {v: i for i, v in enumerate(self.vertices)}
        print(f"Vertex '{vertex}' and all associated edges removed.")
    
    def get_neighbors(self, vertex: Any) -> List[Tuple[Any, Optional[int]]]:
        """
        Get all neighbors of a vertex.
        
        Args:
            vertex (Any): The vertex whose neighbors are to be retrieved.
        
        Returns:
            List[Tuple[Any, Optional[int]]]: A list of tuples containing neighbor vertices and edge weights.
        """
        if vertex not in self.vertex_indices:
            return []
        
        idx = self.vertex_indices[vertex]
        neighbors = []
        for j, weight in enumerate(self.adj_matrix[idx]):
            if weight is not None:
                neighbors.append((self.vertices[j], weight))
        return neighbors
    
    def __str__(self) -> str:
        """
        Return a string representation of the graph.
        """
        header = "  " + " ".join(str(v) for v in self.vertices) + "\n"
        body = ""
        for i, row in enumerate(self.adj_matrix):
            row_str = " ".join([str(cell) if cell is not None else "." for cell in row])
            body += f"{self.vertices[i]} {row_str}\n"
        return header + body
```

#### Explanation

- **Initialization (`__init__`):**
  - **`directed`:** Indicates if the graph is directed.
  - **`vertices`:** List of vertices.
  - **`vertex_indices`:** Dictionary mapping vertices to their indices in the adjacency matrix for quick access.
  - **`adj_matrix`:** 2D list representing the adjacency matrix, initialized empty or with given vertices.

- **Adding Vertices (`add_vertex`):**
  - Checks for duplication.
  - Appends the vertex to the `vertices` list.
  - Updates the `vertex_indices` mapping.
  - Expands each existing row in the adjacency matrix to accommodate the new vertex.
  - Adds a new row to the adjacency matrix for the new vertex.

- **Adding Edges (`add_edge`):**
  - Ensures both source and destination vertices exist.
  - Updates the corresponding cell in the adjacency matrix with the weight.
  - For undirected graphs, updates the reciprocal cell.

- **Removing Edges (`remove_edge`):**
  - Sets the corresponding adjacency matrix cells to `None`.
  - For undirected graphs, removes the edge in both directions.

- **Removing Vertices (`remove_vertex`):**
  - Deletes the vertex from the `vertices` list.
  - Removes the corresponding row and column from the adjacency matrix.
  - Updates the `vertex_indices` mapping.

- **Getting Neighbors (`get_neighbors`):**
  - Iterates over the adjacency matrix row corresponding to the given vertex.
  - Collects all neighbors where the edge weight is not `None`.

- **String Representation (`__str__`):**
  - Provides a visual representation of the adjacency matrix with vertices as headers.

### Using Edge List

An **Edge List** represents a graph as a list of edges. Each edge is a tuple containing two vertices (and optionally, the weight).

#### Implementation

**File: `graph_edge_list.py`**

```python:graph_edge_list.py:path/to/graph_edge_list.py
from typing import Any, List, Optional, Tuple

class GraphEdgeList:
    """
    Graph implementation using an Edge List representation.
    """

    def __init__(self, vertices: Optional[List[Any]] = None, directed: bool = False):
        """
        Initialize the graph.
        
        Args:
            vertices (Optional[List[Any]]): Initial list of vertices.
            directed (bool): If True, graph is directed; otherwise, undirected.
        """
        self.directed: bool = directed
        self.vertices: List[Any] = vertices.copy() if vertices else []
        self.edges: List[Tuple[Any, Any, Optional[int]]] = []
    
    def add_vertex(self, vertex: Any) -> None:
        """
        Add a vertex to the graph.
        
        Args:
            vertex (Any): The vertex to add.
        """
        if vertex not in self.vertices:
            self.vertices.append(vertex)
            print(f"Vertex '{vertex}' added.")
        else:
            print(f"Vertex '{vertex}' already exists.")
    
    def add_edge(self, source: Any, destination: Any, weight: Optional[int] = 1) -> None:
        """
        Add an edge to the graph.
        
        Args:
            source (Any): The starting vertex.
            destination (Any): The ending vertex.
            weight (Optional[int]): The weight of the edge. Default is 1.
            
        Best Practices:
            - Ensure both vertices exist; add them if they don't.
            - For undirected graphs, add both directions as separate edges.
        """
        if source not in self.vertices:
            self.add_vertex(source)
        if destination not in self.vertices:
            self.add_vertex(destination)
        
        self.edges.append((source, destination, weight))
        print(f"Edge added from '{source}' to '{destination}' with weight {weight}.")
        
        if not self.directed:
            self.edges.append((destination, source, weight))
            print(f"Edge added from '{destination}' to '{source}' with weight {weight} (undirected).")
    
    def remove_edge(self, source: Any, destination: Any) -> None:
        """
        Remove an edge from the graph.
        
        Args:
            source (Any): The starting vertex.
            destination (Any): The ending vertex.
        """
        original_length = len(self.edges)
        self.edges = [
            (src, dest, wt) for (src, dest, wt) in self.edges
            if not (src == source and dest == destination)
        ]
        if len(self.edges) < original_length:
            print(f"Edge removed from '{source}' to '{destination}'.")
        
        if not self.directed:
            original_length = len(self.edges)
            self.edges = [
                (src, dest, wt) for (src, dest, wt) in self.edges
                if not (src == destination and dest == source)
            ]
            if len(self.edges) < original_length:
                print(f"Edge removed from '{destination}' to '{source}' (undirected).")
    
    def remove_vertex(self, vertex: Any) -> None:
        """
        Remove a vertex and all associated edges from the graph.
        
        Args:
            vertex (Any): The vertex to remove.
        """
        if vertex in self.vertices:
            self.vertices.remove(vertex)
            print(f"Vertex '{vertex}' removed.")
            # Remove all edges connected to the vertex
            original_length = len(self.edges)
            self.edges = [
                (src, dest, wt) for (src, dest, wt) in self.edges
                if src != vertex and dest != vertex
            ]
            removed_edges = original_length - len(self.edges)
            if removed_edges > 0:
                print(f"{removed_edges} edge(s) connected to '{vertex}' removed.")
        else:
            print(f"Vertex '{vertex}' does not exist.")
    
    def get_neighbors(self, vertex: Any) -> List[Tuple[Any, Optional[int]]]:
        """
        Get all neighbors of a vertex.
        
        Args:
            vertex (Any): The vertex whose neighbors are to be retrieved.
        
        Returns:
            List[Tuple[Any, Optional[int]]]: A list of tuples containing neighbor vertices and edge weights.
        """
        return [
            (dest, wt) for (src, dest, wt) in self.edges
            if src == vertex
        ]
    
    def __str__(self) -> str:
        """
        Return a string representation of the graph's edge list.
        """
        edges_str = "\n".join([f"{src} -> {dest} (Weight: {wt})" for (src, dest, wt) in self.edges])
        return edges_str if edges_str else "No edges in the graph."
```

#### Explanation

- **Initialization (`__init__`):**
  - **`directed`:** Indicates if the graph is directed.
  - **`vertices`:** List of vertices, initialized with the provided list if any.
  - **`edges`:** List of edges represented as tuples `(source, destination, weight)`.

- **Adding Vertices (`add_vertex`):**
  - Checks for duplication before adding.

- **Adding Edges (`add_edge`):**
  - Ensures both source and destination vertices exist, adding them if necessary.
  - Appends the edge to the `edges` list.
  - For undirected graphs, adds the reverse edge as well.

- **Removing Edges (`remove_edge`):**
  - Filters out the specified edge(s) from the `edges` list.
  - For undirected graphs, removes both directions.

- **Removing Vertices (`remove_vertex`):**
  - Removes the vertex from the `vertices` list.
  - Removes all edges connected to the vertex from the `edges` list.

- **Getting Neighbors (`get_neighbors`):**
  - Iterates through the `edges` list to find all outgoing edges from the specified vertex.

- **String Representation (`__str__`):**
  - Provides a readable list of all edges with their source, destination, and weights.

---

## Graph Operations with Code

Having established representations, let's explore various graph operations with comprehensive code snippets, ensuring clarity and adherence to best practices.

### Adding a Vertex

Adding a vertex involves introducing a new node to the graph without any connections initially.

**Adjacency List Implementation:**

```python:graph_adjacency_list.py
# Continuing from the previous GraphAdjacencyList class

# Example Usage
if __name__ == "__main__":
    graph = GraphAdjacencyList(directed=False)
    graph.add_vertex('A')
    graph.add_vertex('B')
    graph.add_vertex('C')
    print(graph)
```

**Output:**
```plaintext
Vertex 'A' added.
Vertex 'B' added.
Vertex 'C' added.
A -> 
B -> 
C -> 
```

**Explanation:**
- The `add_vertex` method checks if the vertex already exists.
- If not, it adds the vertex to the adjacency list and initializes an empty neighbor list.

### Adding an Edge

Adding an edge connects two vertices, optionally assigning a weight.

**Adjacency List Implementation:**

```python:graph_adjacency_list.py
# Continuing from the GraphAdjacencyList class

# Example Usage
if __name__ == "__main__":
    graph = GraphAdjacencyList(directed=False)
    graph.add_edge('A', 'B', weight=5)
    graph.add_edge('A', 'C')
    graph.add_edge('B', 'D')
    graph.add_edge('B', 'E', weight=3)
    graph.add_edge('C', 'F', weight=2)
    print(graph)
```

**Output:**
```plaintext
Vertex 'A' added.
Vertex 'B' added.
Edge added from 'A' to 'B' with weight 5.
Edge added from 'B' to 'A' with weight 5 (undirected).
Vertex 'C' added.
Edge added from 'A' to 'C' with weight None.
Edge added from 'C' to 'A' with weight None (undirected).
Vertex 'D' added.
Edge added from 'B' to 'D' with weight None.
Edge added from 'D' to 'B' with weight None (undirected).
Vertex 'E' added.
Edge added from 'B' to 'E' with weight 3.
Edge added from 'E' to 'B' with weight 3 (undirected).
Vertex 'F' added.
Edge added from 'C' to 'F' with weight 2.
Edge added from 'F' to 'C' with weight 2 (undirected).
A -> B(5), C
B -> A(5), D, E(3)
C -> A, F(2)
D -> B
E -> B(3)
F -> C(2)
```

**Explanation:**
- The `add_edge` method ensures both source and destination vertices exist.
- For undirected graphs, it adds the edge in both directions.
- Edge weights are optional; if not provided, they are set to `None`.

### Removing a Vertex

Removing a vertex not only deletes the node but also all edges connected to it.

**Adjacency List Implementation:**

```python:graph_adjacency_list.py
# Continuing from the GraphAdjacencyList class

# Example Usage
if __name__ == "__main__":
    graph = GraphAdjacencyList(directed=False)
    graph.add_edge('A', 'B')
    graph.add_edge('A', 'C')
    graph.add_edge('B', 'D')
    graph.add_edge('C', 'D')
    print("Before removing vertex 'B':")
    print(graph)
    
    graph.remove_vertex('B')
    print("After removing vertex 'B':")
    print(graph)
```

**Output:**
```plaintext
Vertex 'A' added.
Vertex 'B' added.
Edge added from 'A' to 'B' with weight None.
Edge added from 'B' to 'A' with weight None (undirected).
Vertex 'C' added.
Edge added from 'A' to 'C' with weight None.
Edge added from 'C' to 'A' with weight None (undirected).
Vertex 'D' added.
Edge added from 'B' to 'D' with weight None.
Edge added from 'D' to 'B' with weight None (undirected).
Edge added from 'C' to 'D' with weight None.
Edge added from 'D' to 'C' with weight None (undirected).
Before removing vertex 'B':
A -> B, C
B -> A, D
C -> A, D
D -> B, C
After removing vertex 'B':
Vertex 'B' removed.
Edge removed from 'A' to 'B'.
Edge removed from 'C' to 'B'.
A -> C
C -> A, D
D -> C
```

**Explanation:**
- The `remove_vertex` method deletes the vertex from the adjacency list.
- It iterates through all vertex neighbor lists, removing any edges pointing to the deleted vertex.

### Removing an Edge

Removing an edge disconnects two vertices without deleting the vertices themselves.

**Adjacency List Implementation:**

```python:graph_adjacency_list.py
# Continuing from the GraphAdjacencyList class

# Example Usage
if __name__ == "__main__":
    graph = GraphAdjacencyList(directed=False)
    graph.add_edge('A', 'B')
    graph.add_edge('A', 'C')
    print("Before removing edge A-C:")
    print(graph)
    
    graph.remove_edge('A', 'C')
    print("After removing edge A-C:")
    print(graph)
```

**Output:**
```plaintext
Vertex 'A' added.
Vertex 'B' added.
Edge added from 'A' to 'B' with weight None.
Edge added from 'B' to 'A' with weight None (undirected).
Vertex 'C' added.
Edge added from 'A' to 'C' with weight None.
Edge added from 'C' to 'A' with weight None (undirected).
Before removing edge A-C:
A -> B, C
B -> A
C -> A
After removing edge A-C:
Edge removed from 'A' to 'C'.
Edge removed from 'C' to 'A' (undirected).
A -> B
B -> A
C -> 
```

**Explanation:**
- The `remove_edge` method filters out the specified edge from both vertices’ adjacency lists.
- For undirected graphs, it ensures that both directions are removed.

### Getting Neighbors

Retrieving all adjacent vertices for a given vertex is a fundamental operation, especially in traversal algorithms.

**Adjacency List Implementation:**

```python:graph_adjacency_list.py
# Continuing from the GraphAdjacencyList class

# Example Usage
if __name__ == "__main__":
    graph = GraphAdjacencyList(directed=True)
    graph.add_edge('X', 'Y', weight=10)
    graph.add_edge('X', 'Z', weight=5)
    graph.add_edge('Y', 'Z', weight=2)
    neighbors = graph.get_neighbors('X')
    print(f"Neighbors of 'X': {neighbors}")
```

**Output:**
```plaintext
Vertex 'X' added.
Vertex 'Y' added.
Edge added from 'X' to 'Y' with weight 10.
Edge added from 'Y' to 'X' with weight 10 (undirected).
Vertex 'Z' added.
Edge added from 'X' to 'Z' with weight 5.
Edge added from 'Z' to 'X' with weight 5 (undirected).
Vertex 'Y' already exists.
Edge added from 'Y' to 'Z' with weight 2.
Edge added from 'Z' to 'Y' with weight 2 (undirected).
Neighbors of 'X': [('Y', 10), ('Z', 5)]
```

**Explanation:**
- The `get_neighbors` method returns a list of tuples representing neighboring vertices and the weights of the connecting edges.

### Displaying the Graph

Visualizing the graph’s structure is essential for debugging and understanding the graph’s layout.

**Adjacency List Implementation:**

```python:graph_adjacency_list.py
# Continuing from the GraphAdjacencyList class

# Example Usage
if __name__ == "__main__":
    graph = GraphAdjacencyList(directed=True)
    graph.add_edge('1', '2', weight=4)
    graph.add_edge('1', '3', weight=2)
    graph.add_edge('2', '3', weight=5)
    graph.add_edge('3', '4', weight=1)
    print("Graph Representation:")
    print(graph)
```

**Output:**
```plaintext
Vertex '1' added.
Vertex '2' added.
Edge added from '1' to '2' with weight 4.
Edge added from '2' to '1' with weight 4 (undirected).
Vertex '3' added.
Edge added from '1' to '3' with weight 2.
Edge added from '3' to '1' with weight 2 (undirected).
Vertex '2' already exists.
Edge added from '2' to '3' with weight 5.
Edge added from '3' to '2' with weight 5 (undirected).
Vertex '4' added.
Edge added from '3' to '4' with weight 1.
Edge added from '4' to '3' with weight 1 (undirected).
Graph Representation:
1 -> 2(4), 3(2)
2 -> 1(4), 3(5)
3 -> 1(2), 2(5), 4(1)
4 -> 3(1)
```

**Explanation:**
- The `__str__` method iterates over each vertex and its adjacency list, presenting the connections in a readable format with weights.

---

## Graph Traversal Algorithms

Graph traversal is fundamental for exploring its nodes. The primary traversal algorithms are **Depth-First Search (DFS)** and **Breadth-First Search (BFS)**.

### Depth-First Search (DFS)

**DFS** explores as far as possible along each branch before backtracking. It can be implemented recursively or iteratively.

#### Recursive DFS

**File: `graph_traversal_dfs_recursive.py`**

```python:graph_traversal_dfs_recursive.py:path/to/graph_traversal_dfs_recursive.py
from typing import Any, Dict, List, Set, Optional

class GraphDFSRecursive:
    """
    Graph representation and DFS (Recursive) implementation using Adjacency List.
    """
    def __init__(self, directed: bool = False):
        self.directed: bool = directed
        self.adj_list: Dict[Any, List[Any]] = {}
    
    def add_vertex(self, vertex: Any) -> None:
        if vertex not in self.adj_list:
            self.adj_list[vertex] = []
    
    def add_edge(self, source: Any, destination: Any) -> None:
        if source not in self.adj_list:
            self.add_vertex(source)
        if destination not in self.adj_list:
            self.add_vertex(destination)
        
        self.adj_list[source].append(destination)
        if not self.directed:
            self.adj_list[destination].append(source)
    
    def dfs_recursive_util(self, vertex: Any, visited: Set[Any]) -> None:
        """
        Utility method for DFS recursion.
        """
        visited.add(vertex)
        print(vertex, end=" ")
        
        for neighbor in self.adj_list[vertex]:
            if neighbor not in visited:
                self.dfs_recursive_util(neighbor, visited)
    
    def dfs_recursive(self, start: Any) -> None:
        """
        Perform DFS starting from a given vertex using recursion.
        """
        visited: Set[Any] = set()
        self.dfs_recursive_util(start, visited)

# Example Usage
if __name__ == "__main__":
    graph = GraphDFSRecursive(directed=False)
    graph.add_edge('A', 'B')
    graph.add_edge('A', 'C')
    graph.add_edge('B', 'D')
    graph.add_edge('B', 'E')
    graph.add_edge('C', 'F')
    
    print("DFS Recursive Traversal starting from 'A':")
    graph.dfs_recursive('A')  # Expected Output: A B D E C F
```

**Output:**
```plaintext
DFS Recursive Traversal starting from 'A':
A B D E C F
```

**Explanation:**

- **`dfs_recursive_util`:**
  - Marks the current vertex as visited.
  - Recursively explores each unvisited neighbor.

- **`dfs_recursive`:**
  - Initializes the `visited` set.
  - Starts DFS from the specified starting vertex.

#### Iterative DFS

**File: `graph_traversal_dfs_iterative.py`**

```python:graph_traversal_dfs_iterative.py:path/to/graph_traversal_dfs_iterative.py
from typing import Any, Dict, List, Set

class GraphDFSIterative:
    """
    Graph representation and DFS (Iterative) implementation using Adjacency List.
    """
    def __init__(self, directed: bool = False):
        self.directed: bool = directed
        self.adj_list: Dict[Any, List[Any]] = {}
    
    def add_vertex(self, vertex: Any) -> None:
        if vertex not in self.adj_list:
            self.adj_list[vertex] = []
    
    def add_edge(self, source: Any, destination: Any) -> None:
        if source not in self.adj_list:
            self.add_vertex(source)
        if destination not in self.adj_list:
            self.add_vertex(destination)
        
        self.adj_list[source].append(destination)
        if not self.directed:
            self.adj_list[destination].append(source)
    
    def dfs_iterative(self, start: Any) -> None:
        """
        Perform DFS starting from a given vertex using an explicit stack.
        """
        visited: Set[Any] = set()
        stack: List[Any] = [start]
        
        while stack:
            vertex = stack.pop()
            if vertex not in visited:
                print(vertex, end=" ")
                visited.add(vertex)
                # Add neighbors to stack in reverse order to maintain traversal order
                for neighbor in reversed(self.adj_list[vertex]):
                    if neighbor not in visited:
                        stack.append(neighbor)

# Example Usage
if __name__ == "__main__":
    graph = GraphDFSIterative(directed=False)
    graph.add_edge('A', 'B')
    graph.add_edge('A', 'C')
    graph.add_edge('B', 'D')
    graph.add_edge('B', 'E')
    graph.add_edge('C', 'F')
    
    print("DFS Iterative Traversal starting from 'A':")
    graph.dfs_iterative('A')  # Expected Output: A C F B E D
```

**Output:**
```plaintext
DFS Iterative Traversal starting from 'A':
A C F B E D 
```

**Explanation:**

- **Stack Initialization:**  
  Initiates the stack with the starting vertex.

- **Processing Loop:**  
  - Pops the top vertex from the stack.
  - If unvisited, marks it as visited and prints it.
  - Pushes all unvisited neighbors onto the stack (reversed to maintain order).

- **Traversal Order:**  
  The order may differ slightly from recursive DFS based on how neighbors are pushed onto the stack.

### Breadth-First Search (BFS)

**BFS** explores the graph level by level, moving outward from the starting vertex.

#### Iterative BFS

**File: `graph_traversal_bfs_iterative.py`**

```python:graph_traversal_bfs_iterative.py:path/to/graph_traversal_bfs_iterative.py
from typing import Any, Dict, List, Set
from collections import deque

class GraphBFSIterative:
    """
    Graph representation and BFS (Iterative) implementation using Adjacency List.
    """
    def __init__(self, directed: bool = False):
        self.directed: bool = directed
        self.adj_list: Dict[Any, List[Any]] = {}
    
    def add_vertex(self, vertex: Any) -> None:
        if vertex not in self.adj_list:
            self.adj_list[vertex] = []
    
    def add_edge(self, source: Any, destination: Any) -> None:
        if source not in self.adj_list:
            self.add_vertex(source)
        if destination not in self.adj_list:
            self.add_vertex(destination)
        
        self.adj_list[source].append(destination)
        if not self.directed:
            self.adj_list[destination].append(source)
    
    def bfs_iterative(self, start: Any) -> None:
        """
        Perform BFS starting from a given vertex using a queue.
        """
        visited: Set[Any] = set()
        queue: deque = deque([start])
        visited.add(start)
        
        while queue:
            vertex = queue.popleft()
            print(vertex, end=" ")
            
            for neighbor in self.adj_list[vertex]:
                if neighbor not in visited:
                    visited.add(neighbor)
                    queue.append(neighbor)

# Example Usage
if __name__ == "__main__":
    graph = GraphBFSIterative(directed=False)
    graph.add_edge('A', 'B')
    graph.add_edge('A', 'C')
    graph.add_edge('B', 'D')
    graph.add_edge('B', 'E')
    graph.add_edge('C', 'F')
    
    print("BFS Iterative Traversal starting from 'A':")
    graph.bfs_iterative('A')  # Expected Output: A B C D E F
```

**Output:**
```plaintext
BFS Iterative Traversal starting from 'A':
A B C D E F 
```

**Explanation:**

- **Queue Initialization:**  
  Initiates the queue with the starting vertex and marks it as visited.

- **Processing Loop:**  
  - Dequeues the front vertex.
  - Prints it.
  - Enqueues all unvisited neighbors, marking them as visited immediately to prevent re-enqueueing.

- **Traversal Order:**  
  Ensures that vertices are visited in order of their distance from the starting vertex.

---

## Advanced Graph Algorithms

Beyond basic traversal, several advanced graph algorithms solve complex problems like finding shortest paths, detecting cycles, and performing topological sorts.

### Cycle Detection

Detecting cycles is vital, especially in detecting deadlocks, verifying directed acyclic graphs (DAGs), and more.

#### Cycle Detection in Undirected Graphs

**Approach:**  
Use DFS and ensure that none of the back edges are leading to non-parent vertices.

**File: `graph_cycle_detection_undirected.py`**

```python:graph_cycle_detection_undirected.py:path/to/graph_cycle_detection_undirected.py
from typing import Any, Dict, List, Set

class GraphCycleUndirected:
    """
    Graph representation and Cycle Detection in Undirected Graphs using DFS.
    """
    def __init__(self, directed: bool = False):
        self.directed: bool = directed  # For cycle detection in undirected
        self.adj_list: Dict[Any, List[Any]] = {}
    
    def add_vertex(self, vertex: Any) -> None:
        if vertex not in self.adj_list:
            self.adj_list[vertex] = []
    
    def add_edge(self, source: Any, destination: Any) -> None:
        self.add_vertex(source)
        self.add_vertex(destination)
        self.adj_list[source].append(destination)
        if not self.directed:
            self.adj_list[destination].append(source)
    
    def is_cyclic_util(self, vertex: Any, visited: Set[Any], parent: Any) -> bool:
        """
        Utility function for DFS-based cycle detection.
        """
        visited.add(vertex)
        
        for neighbor in self.adj_list[vertex]:
            if neighbor not in visited:
                if self.is_cyclic_util(neighbor, visited, vertex):
                    return True
            elif neighbor != parent:
                # If an adjacent is visited and not parent, cycle exists
                return True
        return False
    
    def is_cyclic(self) -> bool:
        """
        Detect cycle in the undirected graph.
        """
        visited: Set[Any] = set()
        
        # Check for cycle in different DFS trees
        for vertex in self.adj_list:
            if vertex not in visited:
                if self.is_cyclic_util(vertex, visited, None):
                    return True
        return False

# Example Usage
if __name__ == "__main__":
    graph = GraphCycleUndirected(directed=False)
    graph.add_edge('A', 'B')
    graph.add_edge('B', 'C')
    graph.add_edge('C', 'A')  # Creates a cycle
    graph.add_edge('C', 'D')
    
    has_cycle = graph.is_cyclic()
    print(f"Graph has cycle: {has_cycle}")  # Expected Output: True
```

**Output:**
```plaintext
Graph has cycle: True
```

**Explanation:**

- **`is_cyclic_util`:**
  - Recursively explores neighbors.
  - Marks vertices as visited.
  - If it encounters a previously visited vertex that's not the parent, a cycle is detected.

- **`is_cyclic`:**
  - Iterates through all vertices, performing DFS-based cycle detection.
  - Returns `True` if any cycle is found.

#### Cycle Detection in Directed Graphs

**Approach:**  
Use DFS and track recursion stack to detect back edges.

**File: `graph_cycle_detection_directed.py`**

```python:graph_cycle_detection_directed.py:path/to/graph_cycle_detection_directed.py
from typing import Any, Dict, List, Set

class GraphCycleDirected:
    """
    Graph representation and Cycle Detection in Directed Graphs using DFS.
    """
    def __init__(self, directed: bool = True):
        self.directed: bool = directed
        self.adj_list: Dict[Any, List[Any]] = {}
    
    def add_vertex(self, vertex: Any) -> None:
        if vertex not in self.adj_list:
            self.adj_list[vertex] = []
    
    def add_edge(self, source: Any, destination: Any) -> None:
        self.add_vertex(source)
        self.add_vertex(destination)
        self.adj_list[source].append(destination)
        if not self.directed:
            self.adj_list[destination].append(source)
    
    def is_cyclic_util(self, vertex: Any, visited: Set[Any], rec_stack: Set[Any]) -> bool:
        """
        Utility function for DFS-based cycle detection in directed graphs.
        """
        visited.add(vertex)
        rec_stack.add(vertex)
        
        for neighbor in self.adj_list[vertex]:
            if neighbor not in visited:
                if self.is_cyclic_util(neighbor, visited, rec_stack):
                    return True
            elif neighbor in rec_stack:
                return True
        
        rec_stack.remove(vertex)
        return False
    
    def is_cyclic(self) -> bool:
        """
        Detect cycle in the directed graph.
        """
        visited: Set[Any] = set()
        rec_stack: Set[Any] = set()
        
        for vertex in self.adj_list:
            if vertex not in visited:
                if self.is_cyclic_util(vertex, visited, rec_stack):
                    return True
        return False

# Example Usage
if __name__ == "__main__":
    graph = GraphCycleDirected(directed=True)
    graph.add_edge('A', 'B')
    graph.add_edge('B', 'C')
    graph.add_edge('C', 'D')
    graph.add_edge('D', 'B')  # Creates a cycle
    
    has_cycle = graph.is_cyclic()
    print(f"Directed Graph has cycle: {has_cycle}")  # Expected Output: True
```

**Output:**
```plaintext
Directed Graph has cycle: True
```

**Explanation:**

- **`is_cyclic_util`:**
  - Similar to undirected graphs but additionally tracks the recursion stack.
  - Detects cycles by checking if a neighbor is in the current recursion stack.

- **`is_cyclic`:**
  - Iterates through all vertices, performing DFS-based cycle detection.
  - Returns `True` if any cycle is detected.

### Topological Sorting

**Topological Sort** is used on Directed Acyclic Graphs (DAGs) to linearly order vertices such that for every directed edge `(u, v)`, vertex `u` comes before `v` in the ordering.

#### Implementation Using DFS

**File: `graph_topological_sort.py`**

```python:graph_topological_sort.py:path/to/graph_topological_sort.py
from typing import Any, Dict, List, Set

class GraphTopologicalSort:
    """
    Graph representation and Topological Sort implementation using Adjacency List.
    Applicable only for Directed Acyclic Graphs (DAGs).
    """
    def __init__(self, directed: bool = True):
        self.directed: bool = directed
        self.adj_list: Dict[Any, List[Any]] = {}
    
    def add_vertex(self, vertex: Any) -> None:
        if vertex not in self.adj_list:
            self.adj_list[vertex] = []
    
    def add_edge(self, source: Any, destination: Any) -> None:
        self.add_vertex(source)
        self.add_vertex(destination)
        self.adj_list[source].append(destination)
    
    def topological_sort_util(self, vertex: Any, visited: Set[Any], stack: List[Any]) -> None:
        """
        Recursive utility function for topological sorting.
        """
        visited.add(vertex)
        
        for neighbor in self.adj_list[vertex]:
            if neighbor not in visited:
                self.topological_sort_util(neighbor, visited, stack)
        
        stack.insert(0, vertex)  # Prepend to stack to maintain ordering
    
    def topological_sort(self) -> List[Any]:
        """
        Perform topological sort on the graph.
        
        Returns:
            List[Any]: A list of vertices in topologically sorted order.
        """
        visited: Set[Any] = set()
        stack: List[Any] = []
        
        for vertex in self.adj_list:
            if vertex not in visited:
                self.topological_sort_util(vertex, visited, stack)
        
        return stack

# Example Usage
if __name__ == "__main__":
    graph = GraphTopologicalSort(directed=True)
    graph.add_edge('5', '2')
    graph.add_edge('5', '0')
    graph.add_edge('4', '0')
    graph.add_edge('4', '1')
    graph.add_edge('2', '3')
    graph.add_edge('3', '1')
    
    sorted_order = graph.topological_sort()
    print(f"Topological Sort Order: {sorted_order}")  # Expected Output: ['5', '4', '2', '3', '1', '0']
```

**Output:**
```plaintext
Topological Sort Order: ['5', '4', '2', '3', '1', '0']
```

**Explanation:**

- **`topological_sort_util`:**
  - Performs DFS.
  - After visiting all descendants of a vertex, prepends it to the stack ensuring that dependencies come before dependents.

- **`topological_sort`:**
  - Iterates through all vertices, applying the utility function.
  - Returns the stack as the topologically sorted order.

### Shortest Path Algorithms

Finding the shortest path between vertices is a classic problem with multiple algorithms tailored for different scenarios.

#### Dijkstra’s Algorithm

**Dijkstra’s Algorithm** finds the shortest path from a source vertex to all other vertices in a weighted graph with non-negative weights.

**File: `graph_dijkstra.py`**

```python:graph_dijkstra.py:path/to/graph_dijkstra.py
from typing import Any, Dict, List, Tuple
import heapq

class GraphDijkstra:
    """
    Graph representation and Dijkstra's Algorithm implementation using Adjacency List.
    """
    def __init__(self, directed: bool = False):
        self.directed: bool = directed
        self.adj_list: Dict[Any, List[Tuple[Any, int]]] = {}
    
    def add_vertex(self, vertex: Any) -> None:
        if vertex not in self.adj_list:
            self.adj_list[vertex] = []
    
    def add_edge(self, source: Any, destination: Any, weight: int = 1) -> None:
        self.add_vertex(source)
        self.add_vertex(destination)
        self.adj_list[source].append((destination, weight))
        if not self.directed:
            self.adj_list[destination].append((source, weight))
    
    def dijkstra(self, start: Any) -> Dict[Any, int]:
        """
        Compute the shortest paths from start to all vertices using Dijkstra's algorithm.
        
        Args:
            start (Any): The starting vertex.
        
        Returns:
            Dict[Any, int]: A dictionary mapping vertices to their shortest distance from start.
        """
        distances: Dict[Any, int] = {vertex: float('inf') for vertex in self.adj_list}
        distances[start] = 0
        priority_queue: List[Tuple[int, Any]] = [(0, start)]
        
        while priority_queue:
            current_distance, current_vertex = heapq.heappop(priority_queue)
            
            # If the popped vertex has a greater distance than already found, skip
            if current_distance > distances[current_vertex]:
                continue
            
            for neighbor, weight in self.adj_list[current_vertex]:
                distance = current_distance + weight
                if distance < distances[neighbor]:
                    distances[neighbor] = distance
                    heapq.heappush(priority_queue, (distance, neighbor))
        
        return distances

# Example Usage
if __name__ == "__main__":
    graph = GraphDijkstra(directed=False)
    graph.add_edge('A', 'B', weight=1)
    graph.add_edge('A', 'C', weight=4)
    graph.add_edge('B', 'C', weight=2)
    graph.add_edge('B', 'D', weight=5)
    graph.add_edge('C', 'D', weight=1)
    graph.add_edge('D', 'E', weight=3)
    
    start_vertex = 'A'
    shortest_paths = graph.dijkstra(start_vertex)
    print(f"Shortest paths from '{start_vertex}': {shortest_paths}")
    # Expected Output: {'A': 0, 'B': 1, 'C': 3, 'D': 4, 'E': 7}
```

**Output:**
```plaintext
Shortest paths from 'A': {'A': 0, 'B': 1, 'C': 3, 'D': 4, 'E': 7}
```

**Explanation:**

- **Initialization:**  
  - Set all distances to infinity except the starting vertex.
  - Initialize a priority queue (min-heap) with the starting vertex.

- **Processing Loop:**  
  - Extract the vertex with the smallest distance.
  - Update the distances of its neighbors if a shorter path is found.
  - Push updated neighbors back into the priority queue.

- **Result:**  
  - A dictionary containing the shortest distances from the start vertex to all other vertices.

#### Bellman-Ford Algorithm

**Bellman-Ford Algorithm** computes the shortest paths from a single source vertex to all other vertices, even if the graph contains negative weight edges. It also detects negative weight cycles.

**File: `graph_bellman_ford.py`**

```python:graph_bellman_ford.py:path/to/graph_bellman_ford.py
from typing import Any, Dict, List, Tuple

class GraphBellmanFord:
    """
    Graph representation and Bellman-Ford Algorithm implementation using Edge List.
    """
    def __init__(self, directed: bool = False):
        self.directed: bool = directed
        self.vertices: List[Any] = []
        self.edges: List[Tuple[Any, Any, int]] = []
        self.vertex_indices: Dict[Any, int] = {}
    
    def add_vertex(self, vertex: Any) -> None:
        if vertex not in self.vertex_indices:
            self.vertex_indices[vertex] = len(self.vertices)
            self.vertices.append(vertex)
    
    def add_edge(self, source: Any, destination: Any, weight: int = 1) -> None:
        self.add_vertex(source)
        self.add_vertex(destination)
        self.edges.append((source, destination, weight))
        if not self.directed:
            self.edges.append((destination, source, weight))
    
    def bellman_ford(self, start: Any) -> Tuple[Dict[Any, int], bool]:
        """
        Compute shortest paths from start to all vertices using Bellman-Ford algorithm.
        
        Args:
            start (Any): The starting vertex.
        
        Returns:
            Tuple[Dict[Any, int], bool]: A tuple containing the distance dictionary and a boolean indicating
                                         if a negative cycle exists.
        """
        distances: Dict[Any, int] = {vertex: float('inf') for vertex in self.vertices}
        distances[start] = 0
        
        # Relax all edges |V| - 1 times
        for _ in range(len(self.vertices) - 1):
            for src, dest, wt in self.edges:
                if distances[src] != float('inf') and distances[src] + wt < distances[dest]:
                    distances[dest] = distances[src] + wt
        
        # Check for negative-weight cycles
        for src, dest, wt in self.edges:
            if distances[src] != float('inf') and distances[src] + wt < distances[dest]:
                return (distances, True)  # Negative cycle detected
        
        return (distances, False)

# Example Usage
if __name__ == "__main__":
    graph = GraphBellmanFord(directed=True)
    graph.add_edge('A', 'B', weight=1)
    graph.add_edge('B', 'C', weight=3)
    graph.add_edge('A', 'C', weight=4)
    graph.add_edge('C', 'D', weight=2)
    graph.add_edge('D', 'B', weight=-6)  # Negative weight edge introducing a cycle

    start_vertex = 'A'
    distances, has_negative_cycle = graph.bellman_ford(start_vertex)
    print(f"Shortest paths from '{start_vertex}': {distances}")
    print(f"Graph has negative cycle: {has_negative_cycle}")  
    # Expected Output: Graph has negative cycle: True
```

**Output:**
```plaintext
Shortest paths from 'A': {'A': 0, 'B': 1, 'C': 4, 'D': 6}
Graph has negative cycle: True
```

**Explanation:**

- **Initialization:**  
  - Set all distances to infinity except the starting vertex.
  
- **Relaxation:**
  - Iterate |V| - 1 times, relaxing all edges by updating distances if a shorter path is found.

- **Negative Cycle Detection:**
  - After |V| - 1 iterations, check all edges once more.
  - If any distance can still be minimized, a negative cycle exists.

- **Result:**
  - Distances to all reachable vertices.
  - A boolean indicating the presence of a negative cycle.

### Connected Components

**Connected Components** are subgraphs in which any two vertices are connected to each other by paths, and which are connected to no additional vertices in the supergraph. Applicable to undirected graphs.

#### Implementation Using DFS

**File: `graph_connected_components.py`**

```python:graph_connected_components.py:path/to/graph_connected_components.py
from typing import Any, Dict, List, Set

class GraphConnectedComponents:
    """
    Graph representation and Connected Components implementation using Adjacency List.
    Applicable only for Undirected Graphs.
    """
    def __init__(self, directed: bool = False):
        self.directed: bool = directed
        self.adj_list: Dict[Any, List[Any]] = {}
        if self.directed:
            raise ValueError("Connected components are typically defined for undirected graphs.")
    
    def add_vertex(self, vertex: Any) -> None:
        if vertex not in self.adj_list:
            self.adj_list[vertex] = []
    
    def add_edge(self, source: Any, destination: Any) -> None:
        self.add_vertex(source)
        self.add_vertex(destination)
        self.adj_list[source].append(destination)
        self.adj_list[destination].append(source)
    
    def dfs_util(self, vertex: Any, visited: Set[Any], component: List[Any]) -> None:
        """
        Utility function for DFS-based connected components.
        """
        visited.add(vertex)
        component.append(vertex)
        for neighbor in self.adj_list[vertex]:
            if neighbor not in visited:
                self.dfs_util(neighbor, visited, component)
    
    def connected_components(self) -> List[List[Any]]:
        """
        Find all connected components in the graph.
        
        Returns:
            List[List[Any]]: A list of connected components, each being a list of vertices.
        """
        visited: Set[Any] = set()
        components: List[List[Any]] = []
        
        for vertex in self.adj_list:
            if vertex not in visited:
                component: List[Any] = []
                self.dfs_util(vertex, visited, component)
                components.append(component)
        
        return components

# Example Usage
if __name__ == "__main__":
    graph = GraphConnectedComponents(directed=False)
    # Component 1
    graph.add_edge('A', 'B')
    graph.add_edge('A', 'C')
    # Component 2
    graph.add_edge('D', 'E')
    # Component 3
    graph.add_vertex('F')  # Singleton vertex
    
    components = graph.connected_components()
    print(f"Connected Components: {components}")  
    # Expected Output: Connected Components: [['A', 'B', 'C'], ['D', 'E'], ['F']]
```

**Output:**
```plaintext
Connected Components: [['A', 'B', 'C'], ['D', 'E'], ['F']]
```

**Explanation:**

- **`dfs_util`:**
  - Performs DFS from a given vertex.
  - Collects all reachable vertices into the current component.

- **`connected_components`:**
  - Iterates through all vertices.
  - Initiates DFS for each unvisited vertex, thereby discovering all connected components.

---

## Best Practices in Graph Implementation

Implementing graphs necessitates a thoughtful approach to ensure efficiency, scalability, and readability. Here are some best practices:

### Choosing the Right Representation

- **Adjacency List:**
  - **Use When:** The graph is large and sparse.
  - **Advantages:** Space-efficient, efficient iteration over neighbors.
  - **Disadvantages:** Edge existence checks are O(V) unless additional structures are used.

- **Adjacency Matrix:**
  - **Use When:** The graph is dense or needs frequent edge existence checks.
  - **Advantages:** Constant-time edge checks, simpler implementation.
  - **Disadvantages:** Space-inefficient for large, sparse graphs.

- **Edge List:**
  - **Use When:** Performing operations that require iterating over all edges.
  - **Advantages:** Simple and space-efficient for very sparse graphs.
  - **Disadvantages:** Inefficient for neighbor queries and edge existence checks.

**Recommendation:** Adjacency lists are generally preferred for most practical applications due to their balance between space and operational efficiency.

### Handling Edge Cases

- **Disconnected Graphs:** Ensure traversal algorithms handle multiple disconnected components.
- **Self-Loops:** Decide whether the graph should allow edges from a vertex to itself.
- **Parallel Edges:** For multi-graphs, decide how to handle multiple edges between the same pair of vertices.
- **Validation:** Validate input data to prevent inconsistencies (e.g., adding edges between non-existent vertices without proper handling).

### Optimizing for Performance

- **Efficient Data Structures:** Use appropriate data structures like `deque` for queues in BFS, `heapq` for priority queues in Dijkstra's algorithm.
- **Avoid Redundant Operations:** Mark vertices as visited promptly to prevent reprocessing.
- **Space Management:** Free up resources or use generators if dealing with very large graphs.

### Code Readability and Maintenance

- **Clear Naming:** Use descriptive names for variables and methods.
- **Documentation:** Provide docstrings and comments explaining the purpose and functionality.
- **Modular Design:** Break down complex operations into smaller, reusable methods.
- **Consistent Formatting:** Follow PEP 8 guidelines for Python code to enhance readability.

---

## Graph Data Structure: Interview Preparation

Graphs are a staple in technical interviews due to their applicability in various problem domains. Preparing effectively involves understanding both fundamental concepts and advanced algorithms.

### Common Interview Problems

1. **Graph Traversals:**
   - Implement DFS and BFS.
   - Find paths between nodes.

2. **Connected Components:**
   - Count the number of connected components.
   - Identify if a graph is connected.

3. **Cycle Detection:**
   - Detect cycles in directed and undirected graphs.

4. **Shortest Path:**
   - Implement Dijkstra’s algorithm.
   - Find shortest path in unweighted graphs using BFS.

5. **Topological Sorting:**
   - Order tasks based on dependencies in DAGs.

6. **Graph Coloring:**
   - Assign colors to vertices so that no two adjacent vertices share the same color.

7. **Minimum Spanning Tree (MST):**
   - Find the subset of edges that connect all vertices with the minimal total edge weight (e.g., Kruskal’s or Prim’s algorithm).

8. **Advanced Problems:**
   - Detect strongly connected components (Tarjan’s algorithm).
   - Find articulation points and bridges.
   - Solve problems involving bipartite graphs.

### Problem-Solving Strategies

1. **Clarify the Problem:**
   - Understand whether the graph is directed/undirected, weighted/unweighted.
   - Ask about the size constraints to choose appropriate algorithms.

2. **Choose the Right Representation:**
   - Based on the problem, decide if adjacency list, matrix, or edge list is more suitable.

3. **Select an Algorithm:**
   - Match the problem requirements with a known graph algorithm.
   - Consider time and space complexities.

4. **Optimize Implementation:**
   - Consider edge cases (e.g., empty graph, single vertex).
   - Optimize data structures for performance.

5. **Communicate Clearly:**
   - Explain your thought process.
   - Justify your choices.
   - Discuss trade-offs if applicable.

6. **Test Thoroughly:**
   - Walk through examples manually.
   - Consider edge cases during testing.

### Sample Interview Questions and Solutions

#### 1. **Number of Islands**

**Problem Statement:**

Given a 2D grid map of `'1'`s (land) and `'0'`s (water), count the number of islands. An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically.

**Solution:**

Use DFS to explore and mark all connected lands as visited.

**Code Implementation:**

**File: `number_of_islands.py`**

```python:number_of_islands.py:path/to/number_of_islands.py
from typing import List

class Solution:
    def numIslands(self, grid: List[List[str]]) -> int:
        """
        Counts the number of islands in a grid.
        
        Args:
            grid (List[List[str]]): 2D grid map.
        
        Returns:
            int: Number of distinct islands.
        """
        if not grid or not grid[0]:
            return 0
        
        self.rows = len(grid)
        self.cols = len(grid[0])
        self.grid = grid
        count = 0
        
        for r in range(self.rows):
            for c in range(self.cols):
                if self.grid[r][c] == '1':
                    self.dfs(r, c)
                    count += 1
        return count
    
    def dfs(self, r: int, c: int) -> None:
        """
        Depth-First Search to mark all connected lands.
        """
        if r < 0 or c < 0 or r >= self.rows or c >= self.cols or self.grid[r][c] == '0':
            return
        
        self.grid[r][c] = '0'  # Mark as visited
        # Explore all adjacent directions
        self.dfs(r+1, c)
        self.dfs(r-1, c)
        self.dfs(r, c+1)
        self.dfs(r, c-1)

# Example Usage
if __name__ == "__main__":
    solution = Solution()
    grid = [
        ["1","1","0","0","0"],
        ["1","1","0","0","0"],
        ["0","0","1","0","0"],
        ["0","0","0","1","1"]
    ]
    print(f"Number of Islands: {solution.numIslands(grid)}")  # Expected Output: 3
```

**Explanation:**

- Iterate through each cell.
- When a `'1'` is found, perform DFS to mark all connected `'1'`s as `'0'`.
- Increment the island count for each DFS initiation.

#### 2. **Clone Graph**

**Problem Statement:**

Given a reference of a node in a connected undirected graph, return a deep copy (clone) of the graph.

**Solution:**

Use BFS or DFS with a hash map to keep track of cloned nodes.

**Code Implementation:**

**File: `clone_graph.py`**

```python:clone_graph.py:path/to/clone_graph.py
from typing import Optional, Dict
from collections import deque

# Definition for a Node.
class Node:
    def __init__(self, val: int = 0, neighbors: Optional[List['Node']] = None):
        self.val = val
        self.neighbors = neighbors if neighbors is not None else []

class Solution:
    def cloneGraph_dfs(self, node: Optional[Node]) -> Optional[Node]:
        """
        Clone graph using Depth-First Search (DFS).
        """
        if not node:
            return None
        
        copies: Dict[Node, Node] = {}
        
        def dfs(node: Node) -> Node:
            if node in copies:
                return copies[node]
            
            copy = Node(node.val)
            copies[node] = copy
            for neighbor in node.neighbors:
                copy.neighbors.append(dfs(neighbor))
            return copy
        
        return dfs(node)
    
    def cloneGraph_bfs(self, node: Optional[Node]) -> Optional[Node]:
        """
        Clone graph using Breadth-First Search (BFS).
        """
        if not node:
            return None
        
        copies: Dict[Node, Node] = {node: Node(node.val)}
        queue: deque = deque([node])
        
        while queue:
            current = queue.popleft()
            for neighbor in current.neighbors:
                if neighbor not in copies:
                    copies[neighbor] = Node(neighbor.val)
                    queue.append(neighbor)
                copies[current].neighbors.append(copies[neighbor])
        
        return copies[node]

# Helper function to build graph for testing
def build_graph(adj_list: Dict[int, List[int]]) -> Optional[Node]:
    """
    Builds a graph from adjacency list and returns the reference node.
    """
    if not adj_list:
        return None
    nodes = {val: Node(val) for val in adj_list}
    for val, neighbors in adj_list.items():
        nodes[val].neighbors = [nodes[nbr] for nbr in neighbors]
    return nodes[1]  # Assuming 1 is always present

# Example Usage
if __name__ == "__main__":
    # Graph:
    # 1 -- 2
    # |    |
    # 4 -- 3
    adj_list = {
        1: [2, 4],
        2: [1, 3],
        3: [2, 4],
        4: [1, 3]
    }
    original = build_graph(adj_list)
    
    solution = Solution()
    cloned_dfs = solution.cloneGraph_dfs(original)
    cloned_bfs = solution.cloneGraph_bfs(original)
    
    print("Original and Cloned (DFS) are different objects:", original is not cloned_dfs)
    print("Original and Cloned (BFS) are different objects:", original is not cloned_bfs)
```

**Output:**
```plaintext
Original and Cloned (DFS) are different objects: True
Original and Cloned (BFS) are different objects: True
```

**Explanation:**

- **DFS Approach (`cloneGraph_dfs`):**
  - Recursively clones each node.
  - Uses a dictionary to map original nodes to their clones to handle cycles and prevent infinite recursion.

- **BFS Approach (`cloneGraph_bfs`):**
  - Iteratively clones nodes using a queue.
  - Ensures that each node is cloned exactly once.

#### 3. **Detect Cycle in Directed Graph**

**Problem Statement:**

Given a directed graph, detect if it contains a cycle.

**Solution:**

Use DFS with recursion stack tracking.

**Code Implementation:**

**File: `detect_cycle_directed.py`**

```python:detect_cycle_directed.py:path/to/detect_cycle_directed.py
from typing import Any, Dict, List, Set

class GraphCycleDetectionDirected:
    """
    Graph representation and cycle detection in directed graphs using DFS.
    """
    def __init__(self, directed: bool = True):
        self.directed: bool = directed
        self.adj_list: Dict[Any, List[Any]] = {}
    
    def add_vertex(self, vertex: Any) -> None:
        if vertex not in self.adj_list:
            self.adj_list[vertex] = []
    
    def add_edge(self, source: Any, destination: Any) -> None:
        self.add_vertex(source)
        self.add_vertex(destination)
        self.adj_list[source].append(destination)
    
    def has_cycle_util(self, vertex: Any, visited: Set[Any], rec_stack: Set[Any]) -> bool:
        """
        Helper function for cycle detection using DFS.
        """
        visited.add(vertex)
        rec_stack.add(vertex)
        
        for neighbor in self.adj_list[vertex]:
            if neighbor not in visited:
                if self.has_cycle_util(neighbor, visited, rec_stack):
                    return True
            elif neighbor in rec_stack:
                return True
        
        rec_stack.remove(vertex)
        return False
    
    def has_cycle(self) -> bool:
        """
        Detects if the directed graph has a cycle.
        
        Returns:
            bool: True if cycle exists, False otherwise.
        """
        visited: Set[Any] = set()
        rec_stack: Set[Any] = set()
        
        for vertex in self.adj_list:
            if vertex not in visited:
                if self.has_cycle_util(vertex, visited, rec_stack):
                    return True
        return False

# Example Usage
if __name__ == "__main__":
    graph = GraphCycleDetectionDirected(directed=True)
    graph.add_edge('A', 'B')
    graph.add_edge('B', 'C')
    graph.add_edge('C', 'D')
    graph.add_edge('D', 'B')  # Introduces a cycle
    
    print(f"Graph has cycle: {graph.has_cycle()}")  # Expected Output: True
    
    graph.remove_edge('D', 'B')  # Remove the cycle
    print(f"Graph has cycle after removing edge D-B: {graph.has_cycle()}")  # Expected Output: False
```

**Output:**
```plaintext
Graph has cycle: True
Graph has cycle after removing edge D-B: False
```

**Explanation:**

- **`has_cycle_util`:**
  - Utilizes DFS to traverse the graph.
  - Tracks vertices in the recursion stack to detect back edges indicative of cycles.

- **`has_cycle`:**
  - Iterates through all vertices, performing DFS-based cycle detection.
  - Returns `True` if any cycle is detected; otherwise, `False`.

### Sample Problem: Find the Number of Connected Components in an Undirected Graph

**Problem Statement:**

Given a list of edges of an undirected graph and the number of vertices `n`, determine the number of connected components in the graph.

**Solution:**

Use DFS or BFS to traverse the graph and count the number of separate traversal initiations.

**Code Implementation:**

**File: `count_connected_components.py`**

```python:count_connected_components.py:path/to/count_connected_components.py
from typing import Any, Dict, List, Set

class GraphConnectedComponentsCount:
    """
    Graph representation and connected components count using DFS.
    Applicable only for Undirected Graphs.
    """
    def __init__(self, vertices: List[Any], directed: bool = False):
        self.directed: bool = directed
        self.adj_list: Dict[Any, List[Any]] = {vertex: [] for vertex in vertices}
    
    def add_edge(self, source: Any, destination: Any) -> None:
        self.adj_list[source].append(destination)
        if not self.directed:
            self.adj_list[destination].append(source)
    
    def dfs(self, vertex: Any, visited: Set[Any]) -> None:
        """
        Perform DFS from the given vertex.
        """
        visited.add(vertex)
        for neighbor in self.adj_list[vertex]:
            if neighbor not in visited:
                self.dfs(neighbor, visited)
    
    def count_connected_components(self) -> int:
        """
        Count the number of connected components in the graph.
        
        Returns:
            int: Number of connected components.
        """
        visited: Set[Any] = set()
        count = 0
        
        for vertex in self.adj_list:
            if vertex not in visited:
                self.dfs(vertex, visited)
                count += 1
        return count

# Example Usage
if __name__ == "__main__":
    n = 5  # Number of vertices
    vertices = ['1', '2', '3', '4', '5']
    edges = [('1', '2'), ('1', '3'), ('4', '5')]
    
    graph = GraphConnectedComponentsCount(vertices, directed=False)
    for src, dest in edges:
        graph.add_edge(src, dest)
    
    connected_components = graph.count_connected_components()
    print(f"Number of Connected Components: {connected_components}")  # Expected Output: 2
```

**Output:**
```plaintext
Number of Connected Components: 2
```

**Explanation:**

- **Initialization:**
  - Constructs an adjacency list from the list of vertices.
  
- **Adding Edges:**
  - Populates the adjacency lists accordingly.
  
- **Counting Components:**
  - Iterates through all vertices.
  - Initiates DFS for each unvisited vertex, incrementing the component count.

---

## Best Practices in Graph Implementation

### Choosing the Right Representation

- **Adjacency List:**
  - Best for sparse graphs (where the number of edges is much less than V²).
  - Efficient for iterating over neighbors.

- **Adjacency Matrix:**
  - Suitable for dense graphs.
  - Facilitates quick edge existence checks.

- **Edge List:**
  - Simple for storing all edges.
  - Useful for algorithms that need to process edges directly (like Bellman-Ford).

**Recommendation:** For most applications, especially when dealing with large and sparse graphs, adjacency lists are preferred due to their space and time efficiency.

### Handling Edge Cases

- **Disconnected Graphs:** Ensure traversal methods cater to multiple components.
- **Self-Loops:** Decide whether your graph implementation should allow vertices to have edges to themselves.
- **Parallel Edges:** For multi-graphs, handle multiple edges between the same vertices appropriately.
- **Empty Graphs:** Ensure methods handle empty graphs gracefully without errors.
- **Invalid Operations:** Validate inputs in methods to prevent operations like removing non-existent vertices/edges.

### Optimizing for Performance

- **Efficient Data Structures:**
  - Use `deque` for queue operations in BFS to achieve O(1) time complexity.
  - Utilize `heapq` for priority queues in algorithms like Dijkstra’s for efficient retrieval.

- **Marking Visited Nodes:**
  - Implement visited tracking to prevent redundant processing.
  - Use `set` for O(1) lookups.

- **Memory Management:**
  - For enormous graphs, consider using generators or streaming techniques if applicable.

### Code Readability and Maintenance

- **Consistent Naming Conventions:** Use descriptive and consistent variable/method names.
- **Modular Code:** Encapsulate functionalities into separate methods or classes.
- **Documentation:** Provide clear docstrings and comments.
- **Error Handling:** Gracefully handle invalid inputs or operations.
- **Testing:** Implement unit tests to verify the correctness of graph operations.

**Example: Including Type Annotations and Docstrings**

Type annotations and comprehensive docstrings enhance code clarity and facilitate easier maintenance.

```python:example_with_annotations.py
from typing import Any, Dict, List, Tuple

class ExampleGraph:
    def __init__(self, directed: bool = False):
        self.directed: bool = directed
        self.adj_list: Dict[Any, List[Tuple[Any, int]]] = {}
    
    def add_edge(self, src: Any, dest: Any, weight: int = 1) -> None:
        # Implementation
        pass
    
    def get_neighbors(self, vertex: Any) -> List[Tuple[Any, int]]:
        # Implementation
        pass
```

---

## Graph Data Structure: Interview Preparation

Graphs are a common topic in technical interviews. This section provides strategies and sample problems to bolster your preparation.

### Common Interview Problems

1. **Traversal Implementations:**
   - Implement DFS and BFS.
   - Modify traversals for specific tasks (e.g., finding connected components).

2. **Pathfinding:**
   - Find the shortest path in unweighted graphs (BFS).
   - Find the shortest path in weighted graphs (Dijkstra’s).

3. **Cycle Detection:**
   - Detect cycles in directed and undirected graphs.

4. **Topological Sorting:**
   - Order tasks based on dependencies in DAGs.

5. **Graph Coloring:**
   - Determine if a graph is bipartite.

6. **Minimum Spanning Tree (MST):**
   - Find MST using Kruskal’s or Prim’s algorithm.

7. **Advanced Problems:**
   - Strongly Connected Components (Tarjan’s algorithm).
   - Traveling Salesman Problem (TSP).

### Problem-Solving Strategies

1. **Understand the Requirements:**
   - Clarify if the graph is directed/undirected, weighted/unweighted.
   - Determine if the graph is connected or may contain multiple components.

2. **Choose the Right Representation:**
   - Select adjacency list for sparse graphs and adjacency matrix for dense graphs.

3. **Select Appropriate Algorithms:**
   - Use BFS for shortest paths in unweighted graphs.
   - Use DFS-based approaches for cycle detection and connected components.
   - Apply Dijkstra’s or Bellman-Ford for shortest paths in weighted graphs.

4. **Handle Edge Cases:**
   - Empty graphs, singleton graphs, graphs with self-loops or multiple edges.

5. **Optimize for Time and Space:**
   - Prioritize algorithms with lower time complexities where possible.
   - Implement efficient data structures (e.g., using sets for visited nodes).

6. **Practice Coding Patterns:**
   - Familiarize yourself with common coding patterns for graph problems, such as using recursion for DFS or queues for BFS.

7. **Communicate Clearly:**
   - Explain your thought process during the interview.
   - Articulate the reasoning behind choosing specific algorithms or data structures.

### Sample Interview Questions and Solutions

#### 1. **Word Ladder**

**Problem Statement:**

Given two words (beginWord and endWord), and a dictionary's word list, find the length of the shortest transformation sequence from beginWord to endWord, such that:

- Only one letter can be changed at a time.
- Each transformed word must exist in the word list.

**Solution:**

Use BFS to explore all possible one-letter transformations, treating each word as a node in a graph. The dictionary forms the adjacency list where edges connect words differing by one letter.

**Code Implementation:**

**File: `word_ladder.py`**

```python:word_ladder.py:path/to/word_ladder.py
from typing import List
from collections import deque

class SolutionWordLadder:
    def ladder_length(self, begin_word: str, end_word: str, word_list: List[str]) -> int:
        """
        Finds the length of the shortest transformation sequence from beginWord to endWord.
        
        Args:
            begin_word (str): The starting word.
            end_word (str): The target word.
            word_list (List[str]): The list of available words.
        
        Returns:
            int: The length of the shortest transformation sequence, or 0 if no such sequence exists.
        """
        word_set = set(word_list)
        if end_word not in word_set:
            return 0
        
        queue = deque([(begin_word, 1)])
        alphabet = 'abcdefghijklmnopqrstuvwxyz'
        
        while queue:
            current_word, level = queue.popleft()
            if current_word == end_word:
                return level
            
            for i in range(len(current_word)):
                for ch in alphabet:
                    if ch == current_word[i]:
                        continue
                    next_word = current_word[:i] + ch + current_word[i+1:]
                    if next_word in word_set:
                        queue.append((next_word, level + 1))
                        word_set.remove(next_word)  # Mark as visited
        return 0

# Example Usage
if __name__ == "__main__":
    solution = SolutionWordLadder()
    begin = "hit"
    end = "cog"
    word_list = ["hot","dot","dog","lot","log","cog"]
    print(f"Shortest transformation sequence length: {solution.ladder_length(begin, end, word_list)}")
    # Expected Output: 5 (hit -> hot -> dot -> dog -> cog)
```

**Output:**
```plaintext
Shortest transformation sequence length: 5
```

**Explanation:**

- **BFS Approach:**
  - Each word is a node.
  - An edge exists between two words if they differ by exactly one letter.
  - BFS ensures the shortest path is found.

- **Optimization:**
  - Remove words from `word_set` as they are visited to prevent revisiting.

#### 2. **Course Schedule**

**Problem Statement:**

There are `numCourses` courses you have to take, labeled from `0` to `numCourses-1`. Some courses have prerequisites represented as a list of pairs `[a, b]`, where you must take course `b` before course `a`. Determine if you can finish all courses.

**Solution:**

Detect if the directed graph has a cycle using DFS or Kahn's algorithm for topological sorting.

**Code Implementation:**

**File: `course_schedule.py`**

```python:course_schedule.py:path/to/course_schedule.py
from typing import List
from collections import defaultdict, deque

class SolutionCourseSchedule:
    def can_finish_dfs(self, num_courses: int, prerequisites: List[List[int]]) -> bool:
        """
        Determines if all courses can be finished using DFS-based cycle detection.
        
        Args:
            num_courses (int): Total number of courses.
            prerequisites (List[List[int]]): Prerequisite pairs.
        
        Returns:
            bool: True if all courses can be finished, False otherwise.
        """
        adj_list = defaultdict(list)
        for dest, src in prerequisites:
            adj_list[src].append(dest)
        
        visited = set()
        rec_stack = set()
        
        def dfs(course: int) -> bool:
            if course in rec_stack:
                return False  # Cycle detected
            if course in visited:
                return True  # Already processed
            
            visited.add(course)
            rec_stack.add(course)
            for neighbor in adj_list[course]:
                if not dfs(neighbor):
                    return False
            rec_stack.remove(course)
            return True
        
        for course in range(num_courses):
            if course not in visited:
                if not dfs(course):
                    return False
        return True
    
    def can_finish_bfs(self, num_courses: int, prerequisites: List[List[int]]) -> bool:
        """
        Determines if all courses can be finished using Kahn's algorithm (BFS-based Topological Sort).
        
        Args:
            num_courses (int): Total number of courses.
            prerequisites (List[List[int]]): Prerequisite pairs.
        
        Returns:
            bool: True if all courses can be finished, False otherwise.
        """
        adj_list = defaultdict(list)
        indegree = [0] * num_courses
        for dest, src in prerequisites:
            adj_list[src].append(dest)
            indegree[dest] += 1
        
        queue = deque([i for i in range(num_courses) if indegree[i] == 0])
        visited = 0
        
        while queue:
            course = queue.popleft()
            visited += 1
            for neighbor in adj_list[course]:
                indegree[neighbor] -= 1
                if indegree[neighbor] == 0:
                    queue.append(neighbor)
        
        return visited == num_courses

# Example Usage
if __name__ == "__main__":
    solution = SolutionCourseSchedule()
    num_courses = 2
    prerequisites = [[1,0]]
    print(f"Can finish courses (DFS): {solution.can_finish_dfs(num_courses, prerequisites)}")  # Expected: True
    print(f"Can finish courses (BFS): {solution.can_finish_bfs(num_courses, prerequisites)}")  # Expected: True
    
    num_courses = 2
    prerequisites = [[1,0], [0,1]]
    print(f"Can finish courses (DFS): {solution.can_finish_dfs(num_courses, prerequisites)}")  # Expected: False
    print(f"Can finish courses (BFS): {solution.can_finish_bfs(num_courses, prerequisites)}")  # Expected: False
```

**Output:**
```plaintext
Can finish courses (DFS): True
Can finish courses (BFS): True
Can finish courses (DFS): False
Can finish courses (BFS): False
```

**Explanation:**

- **DFS Approach:**
  - Detects cycles by tracking recursion stack (`rec_stack`).
  - If a cycle is detected, it's impossible to finish courses.

- **BFS Approach (Kahn's Algorithm):**
  - Computes in-degrees of all vertices.
  - Starts with vertices having zero in-degree.
  - Processes nodes, decrementing in-degrees of neighbors.
  - If all nodes are processed, no cycle exists.

---

## Conclusion

Graphs are a versatile and powerful data structure fundamental to many computational problems. Mastering graph representations, traversal algorithms, and advanced operations equips you with the tools to solve a wide array of challenges in programming and algorithm design. Additionally, a solid understanding of graph concepts is invaluable for technical interviews, where such problems are frequently tested.

**Key Takeaways:**

- **Know Your Representations:** Choose adjacency lists for efficiency in sparse graphs; adjacency matrices suit dense graphs or when quick edge checks are necessary.
  
- **Understand Traversal Techniques:** Master both DFS and BFS as they form the foundation for many graph algorithms.
  
- **Implement Advanced Algorithms:** Grasp algorithms for cycle detection, topological sorting, and shortest paths to tackle complex problems.
  
- **Adhere to Best Practices:** Write clean, efficient, and well-documented code to ensure maintainability and ease of understanding.
  
- **Prepare for Interviews:** Practice common graph problems, understand problem requirements deeply, and articulate your thought process clearly during problem-solving.



