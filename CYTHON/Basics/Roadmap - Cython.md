
# Roadmap for Learning Cython

Cython is a programming language that blends Python with C/C++ capabilities, allowing you to write Python code that can be compiled to highly efficient C extensions. Here's a structured roadmap to master Cython:

## Phase 1: Getting Started
1. **Understanding the Basics**
   - What is Cython and why use it
   - Installing Cython and setting up your environment
   - The compilation process

2. **First Steps**
   - Writing your first Cython module
   - Understanding `.pyx` and `.pxd` files
   - Basic setup.py configuration

## Phase 2: Core Concepts
1. **Static Typing**
   - C type declarations
   - Type conversion between Python and C
   - Performance gains from static typing

2. **Memory Management**
   - C memory allocation
   - Working with pointers safely
   - Memory views

3. **Function Optimization**
   - Cython function types (Python, C, and cpdef functions)
   - Using C functions from external libraries
   - Optimizing loops and algorithms

## Phase 3: Advanced Features
1. **Object-Oriented Programming**
   - Extension types (cdef classes)
   - Inheritance and polymorphism
   - Special methods

2. **Parallelism and Concurrency**
   - The GIL and thread safety
   - Parallel loops with prange
   - Releasing the GIL

3. **Integration Capabilities**
   - Working with NumPy arrays
   - C++ integration
   - Interfacing with external libraries

## Phase 4: Mastery and Best Practices
1. **Performance Tuning**
   - Profiling Cython code
   - Using annotated HTML output
   - Removing Python overhead

2. **Project Architecture**
   - Organizing complex Cython projects
   - Packaging and distribution
   - Continuous integration

3. **Advanced Applications**
   - Domain-specific optimizations
   - Creating Python extensions in pure C
   - Debugging complex Cython applications
---


# Phase 1: Getting Started with Cython

## Understanding the Basics

### What is Cython?

Cython is a programming language that makes writing C extensions for Python as easy as writing Python itself. It combines the ease of Python with the speed of C, allowing you to:

1. Accelerate Python code by adding static type declarations
2. Easily wrap external C/C++ libraries for Python use
3. Implement performance-critical code in a more C-like language while maintaining Python compatibility

### Why Use Cython?

- **Performance**: Python code can be significantly accelerated (10-1000x speedups in some cases)
- **Integration**: Seamless connection between Python and C/C++ ecosystems
- **Gradual optimization**: Start with regular Python code and optimize incrementally
- **Type safety**: Add optional static typing for more robust code

### Installing Cython

Cython can be installed using pip:

```bash
pip install cython
```

For development, you'll also need a C compiler:
- **Windows**: Microsoft Visual C++ or MinGW
- **macOS**: Xcode Command Line Tools
- **Linux**: GCC or Clang (usually pre-installed)

## First Steps with Cython

### Cython Modes

Cython offers two primary modes of operation:

1. **Pure Python Mode**: Write regular Python code in `.py` files with Cython-specific annotations
2. **Cython Mode**: Write code in `.pyx` files using Cython's extended Python syntax with C-like features

Let's explore both approaches:

### Example 1: Pure Python Mode

First, let's create a simple example in pure Python to compute Fibonacci numbers:

```python
# fib_pure_python.py
def fibonacci(n):
    """Compute the nth Fibonacci number."""
    a, b = 0, 1
    for i in range(n):
        a, b = b, a + b
    return a
```

Now, let's convert this to Cython's pure Python mode:

```python
# fib_py_cython.py
# cython: language_level=3
import cython

@cython.cfunc
def _fib_core(n: cython.int) -> cython.int:
    """C-optimized core function."""
    a, b = 0, 1
    for i in range(n):
        a, b = b, a + b
    return a

def fibonacci(n):
    """Python-callable wrapper."""
    if n < 0:
        raise ValueError("n must be non-negative")
    return _fib_core(n)
```

Key features in this example:
- The `# cython: language_level=3` directive specifies the Python compatibility level
- `@cython.cfunc` creates a C-level function (not callable from Python directly)
- Type annotations with `cython.int` enable C-level integer operations

### Example 2: Cython Mode (`.pyx` file)

Here's the same example using a `.pyx` file with Cython's native syntax:

```cython
# fib_cython.pyx
cdef int _fib_core(int n):
    """C-optimized core function."""
    cdef int i, a=0, b=1
    for i in range(n):
        a, b = b, a + b
    return a

def fibonacci(n):
    """Python-callable wrapper."""
    if n < 0:
        raise ValueError("n must be non-negative")
    return _fib_core(n)
```

Key differences:
- `cdef` keyword declares C functions and variables
- We explicitly declare all variable types for maximum performance
- More explicit control over what's exposed to Python

### Building with setup.py

To use our Cython code, we need to compile it to a C extension module. The most common approach is using `setup.py`:

```python
# setup.py
from setuptools import setup, Extension
from Cython.Build import cythonize

# For pure Python mode
pure_python_extension = Extension(
    name="fib_py_cython",
    sources=["fib_py_cython.py"],
)

# For Cython mode (.pyx file)
cython_extension = Extension(
    name="fib_cython",
    sources=["fib_cython.pyx"],
)

setup(
    name="fibonacci_example",
    # Explicitly tell setuptools *not* to discover packages/modules

    packages=[],

    py_modules=[],
    ext_modules=cythonize([pure_python_extension, cython_extension], annotate = True),
)
```

Build the extensions with:

```bash
python setup.py build_ext --inplace
```

This generates shared library files (`.so` on Linux/macOS, `.pyd` on Windows) that can be imported like regular Python modules.

### Understanding setup.py for Cython

Let's break down what `setup.py` actually does:

1. `Extension` objects define the source files and compilation options
2. `cythonize()` transforms Cython code into C code
3. `setup()` compiles the C code into binary extensions

The compilation process:
1. Cython translates `.pyx` or annotated `.py` files to `.c` files
2. The C compiler compiles these files into machine code
3. The linker creates a shared library that Python can import

### Comparing Performance

Let's create a simple benchmark to compare our implementations:

```python
# benchmark.py
import time
import fib_pure_python
import fib_py_cython
import fib_cython

def benchmark(func, n, iterations=100):
    start = time.time()
    for _ in range(iterations):
        result = func(n)
    elapsed = time.time() - start
    return elapsed, result

n = 30
iterations = 100000

print(f"Computing fibonacci({n}) {iterations} times:")

elapsed, result = benchmark(fib_pure_python.fibonacci, n, iterations)
print(f"Pure Python:     {elapsed:.4f}s with result {result}")

elapsed, result = benchmark(fib_py_cython.fibonacci, n, iterations)
print(f"Python-Cython:   {elapsed:.4f}s with result {result}")

elapsed, result = benchmark(fib_cython.fibonacci, n, iterations)
print(f"Full Cython:     {elapsed:.4f}s with result {result}")
```

Typical results show significant speed improvements with Cython, especially in the full Cython mode.

## More Advanced Example: Array Processing

Let's look at a more practical example where Cython really shines - array processing:

### Pure Python Implementation

```python
# array_sum_python.py
def array_sum(arr):
    """Calculate sum of squares for array elements."""
    total = 0.0
    for i in range(len(arr)):
        total += arr[i] * arr[i]
    return total
```

### Pure Python Mode with Cython

```python
# array_sum_py_cython.py
# cython: language_level=3
import cython
from cython.cimports import numpy as cnp
import numpy as np

@cython.ccall
def array_sum(arr: cython.double[:]):
    """Calculate sum of squares for array elements using memoryviews."""
    cdef:
        double total = 0.0
        int i, n = arr.shape[0]
        
    for i in range(n):
        total += arr[i] * arr[i]
        
    return total
```

### Full Cython Implementation

```cython
# array_sum_cython.pyx
import numpy as np
cimport numpy as cnp

# Initialize NumPy C API
cnp.import_array()

def array_sum(cnp.ndarray[cnp.double_t, ndim=1] arr):
    """Calculate sum of squares for array elements."""
    cdef:
        double total = 0.0
        int i, n = arr.shape[0]
        double* data = <double*>arr.data
        
    for i in range(n):
        total += data[i] * data[i]
        
    return total
```

### Direct Pointer Access in Cython (Maximum Performance)

```cython
# array_sum_pointer.pyx
import numpy as np
cimport numpy as cnp

# Initialize NumPy C API
cnp.import_array()

def array_sum(cnp.ndarray[cnp.double_t, ndim=1] arr):
    """Calculate sum of squares with direct pointer access."""
    cdef:
        double total = 0.0
        int i, n = arr.shape[0]
        double* data = <double*>arr.data
        
    # Direct pointer arithmetic for maximum speed
    for i in range(n):
        total += data[i] * data[i]
        
    return total
```

## Working with External C Libraries

One of Cython's great strengths is easily wrapping C libraries. Here's a simplified example using the C standard library:

```cython
# math_wrapper.pyx
from libc.math cimport sqrt, pow

def euclidean_distance(double x1, double y1, double x2, double y2):
    """Calculate Euclidean distance using C's math library."""
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2))
```

## Advantages and Disadvantages of Different Cython Modes

### Pure Python Mode
**Advantages:**
- No special file extensions needed (`.py` files)
- Works directly with Python tooling (type checkers, linters, etc.)
- Easier migration path from Python
- Can be conditionally compiled (can run as plain Python if Cython unavailable)

**Disadvantages:**
- Limited access to some Cython features
- Often slightly slower than full Cython mode
- More verbose type annotations
- Restricted ability to use C pointer semantics

### Full Cython Mode
**Advantages:**
- Cleaner syntax for C-level code
- Maximum performance potential
- Complete access to all Cython features
- Better compile-time error checking
- Easier integration with external C libraries

**Disadvantages:**
- Requires special file extensions (`.pyx`, `.pxd`)
- Less compatible with standard Python tools
- Steeper learning curve
- Must be compiled (can't run as plain Python)

## Tips for Effective Cythonization

1. **Start with profiling**: Identify bottlenecks in your Python code before Cythonizing
2. **Incremental approach**: Convert small parts at a time, test and benchmark as you go
3. **Add type declarations strategically**: Focus on loop variables and function parameters first
4. **Use memory views or typed NumPy arrays** for array operations
5. **Watch for Python interactions**: Minimize calls to Python functions from Cython-optimized code

## Understanding Generated C Code

Cython can generate annotated HTML to help visualize how your code is being translated to C:

```bash
cython -a fib_cython.pyx
```

This creates `fib_cython.html` which shows your code with color-coding indicating Python interaction levels:
- Yellow highlighting shows Python interaction (slower)
- White background indicates pure C code (faster)

## Next Steps

After mastering these basics, you might want to explore:
- Defining custom extension types (Cython classes)
- Parallel computing with `prange`
- More advanced NumPy integration
- C++ interoperability


# Diving Deeper into Cython with Working Examples

Let's explore more advanced Cython concepts with practical examples and use cases.

## 1. Defining Custom Extension Types (Cython Classes)

Extension types in Cython provide a way to create classes that work like Python classes but have the efficiency of C structs.

### Example: Creating a 2D Point Class

```cython
# point.pyx
cdef class Point:
    # Declare C attributes (not accessible from Python unless exposed)
    cdef double _x, _y
    
    def __init__(self, double x=0.0, double y=0.0):
        self._x = x
        self._y = y
    
    # Property accessors to expose attributes to Python
    @property
    def x(self):
        return self._x
    
    @x.setter
    def x(self, double value):
        self._x = value
    
    @property
    def y(self):
        return self._y
    
    @y.setter
    def y(self, double value):
        self._y = value
    
    # Regular Python method
    def __repr__(self):
        return f"Point({self._x}, {self._y})"
    
    # C method (not callable from Python)
    cdef double _distance_squared(self, Point other):
        cdef double dx = self._x - other._x
        cdef double dy = self._y - other._y
        return dx*dx + dy*dy
    
    # Python-callable method that uses C method
    def distance_to(self, Point other):
        from libc.math cimport sqrt
        return sqrt(self._distance_squared(other))
    
    # Fast in-place operations with C implementation
    cpdef void move(self, double dx, double dy):
        """Move point by dx, dy"""
        self._x += dx
        self._y += dy
```

**Usage:**

```python
# test_point.py
from point import Point

p1 = Point(1.0, 2.0)
p2 = Point(4.0, 6.0)

print(f"Distance between {p1} and {p2}: {p1.distance_to(p2)}")

p1.move(2.0, 3.0)
print(f"After moving: {p1}")
```

**Key features:**
- `cdef class` creates a Python-compatible class with C-level attributes
- Private C-only methods with `cdef`
- Hybrid methods with `cpdef` callable from both Python and C
- Properties to expose C attributes to Python safely

## 2. Parallel Computing with `prange`

Cython provides easy parallelization with OpenMP through the `prange` function.

### Example: Parallel Array Processing

```cython
# parallel_sum.pyx
import numpy as np
cimport numpy as cnp
from cython.parallel import prange

def parallel_sum_of_squares(cnp.ndarray[cnp.double_t, ndim=1] arr):
    """Calculate sum of squares using parallel processing."""
    cdef:
        int i
        int n = arr.shape[0]
        double total = 0.0
        double* data = <double*>arr.data
    
    # Use OpenMP for parallelization
    # The GIL must be released for true parallelism
    with nogil:
        # Parallel for loop
        for i in prange(n, schedule='static'):
            total += data[i] * data[i]
    
    return total
```

To compile with OpenMP support, modify `setup.py`:

```python
# setup.py for parallel example
from setuptools import setup, Extension
from Cython.Build import cythonize
import numpy as np

# Set OpenMP flags based on platform
import platform
import os

compile_args = []
link_args = []

if platform.system() == "Windows":
    compile_args = ['/openmp']
else:  # Linux, macOS
    compile_args = ['-fopenmp']
    link_args = ['-fopenmp']

extensions = [
    Extension("parallel_sum", 
              ["parallel_sum.pyx"],
              extra_compile_args=compile_args,
              extra_link_args=link_args,
              include_dirs=[np.get_include()])
]

setup(
    ext_modules=cythonize(extensions)
)
```

**Benchmark:**

```python
# test_parallel.py
import numpy as np
import time
from parallel_sum import parallel_sum_of_squares

# Create test data
size = 10_000_000
data = np.random.random(size)

# Test parallel implementation
start = time.time()
result_parallel = parallel_sum_of_squares(data)
elapsed_parallel = time.time() - start
print(f"Parallel sum: {result_parallel:.2f} in {elapsed_parallel:.4f} seconds")

# Compare with numpy (also optimized but single-threaded)
start = time.time()
result_numpy = np.sum(data * data)
elapsed_numpy = time.time() - start
print(f"NumPy sum: {result_numpy:.2f} in {elapsed_numpy:.4f} seconds")

# Verify results match
print(f"Results match: {abs(result_parallel - result_numpy) < 1e-10}")
```

**Key features:**
- `prange` divides the loop across available CPU cores
- `nogil` releases Python's Global Interpreter Lock for true parallelism
- `schedule='static'` determines how work is divided (alternatives: 'dynamic', 'guided')

## 3. Advanced NumPy Integration

### Example: Custom Image Processing Filter

```cython
# image_filter.pyx
import numpy as np
cimport numpy as cnp
from cython.parallel import prange
from libc.math cimport exp

# Import NumPy C API
cnp.import_array()

def apply_gaussian_blur(cnp.ndarray[cnp.uint8_t, ndim=3] image, double sigma=1.0):
    """
    Apply a Gaussian blur to an RGB image.
    
    Parameters:
    - image: RGB image as numpy array with shape (height, width, 3)
    - sigma: Blur standard deviation
    
    Returns:
    - Blurred image
    """
    cdef:
        int height = image.shape[0]
        int width = image.shape[1]
        int y, x, ky, kx, channel
        int kernel_size = int(6 * sigma) + 1  # Ensure odd size
        int half_kernel = kernel_size // 2
        double kernel_sum, kernel_value, pixel_value
        int ny, nx
        
    # Create kernel
    cdef cnp.ndarray[cnp.double_t, ndim=2] kernel = np.zeros((kernel_size, kernel_size), dtype=np.double)
    kernel_sum = 0.0
    
    # Calculate Gaussian kernel
    for y in range(kernel_size):
        for x in range(kernel_size):
            ky = y - half_kernel
            kx = x - half_kernel
            # 2D Gaussian function
            kernel_value = exp(-(kx*kx + ky*ky) / (2 * sigma * sigma))
            kernel[y, x] = kernel_value
            kernel_sum += kernel_value
    
    # Normalize kernel
    for y in range(kernel_size):
        for x in range(kernel_size):
            kernel[y, x] /= kernel_sum
    
    # Create output image
    cdef cnp.ndarray[cnp.uint8_t, ndim=3] output = np.zeros_like(image)
    
    # Apply filter
    with nogil:
        for y in prange(height, schedule='guided'):
            for x in range(width):
                for channel in range(3):  # RGB channels
                    pixel_value = 0.0
                    
                    # Apply kernel
                    for ky in range(kernel_size):
                        ny = y + ky - half_kernel
                        # Handle boundary conditions (clamping)
                        if ny < 0:
                            ny = 0
                        elif ny >= height:
                            ny = height - 1
                        
                        for kx in range(kernel_size):
                            nx = x + kx - half_kernel
                            # Handle boundary conditions (clamping)
                            if nx < 0:
                                nx = 0
                            elif nx >= width:
                                nx = width - 1
                            
                            pixel_value += kernel[ky, kx] * image[ny, nx, channel]
                    
                    # Clamp to valid uint8 range
                    if pixel_value < 0:
                        output[y, x, channel] = 0
                    elif pixel_value > 255:
                        output[y, x, channel] = 255
                    else:
                        output[y, x, channel] = <cnp.uint8_t>pixel_value
    
    return output
```

**Usage:**

```python
# test_image_filter.py
import numpy as np
from PIL import Image
import time
from image_filter import apply_gaussian_blur
import matplotlib.pyplot as plt

# Load an image
img = np.array(Image.open("sample_image.jpg"))

# Apply our Cython blur
start = time.time()
blurred_img = apply_gaussian_blur(img, sigma=2.0)
elapsed = time.time() - start
print(f"Cython blur completed in {elapsed:.3f} seconds")

# Display results
plt.figure(figsize=(12, 6))
plt.subplot(1, 2, 1)
plt.imshow(img)
plt.title("Original Image")
plt.subplot(1, 2, 2)
plt.imshow(blurred_img)
plt.title(f"Cython Blur (sigma=2.0, {elapsed:.3f}s)")
plt.tight_layout()
plt.show()
```

**Key features:**
- Typed 3D NumPy arrays for image data
- Parallel processing with `prange`
- Direct access to array data with boundary checking
- Use of C math library with `libc.math cimport exp`

## 4. C++ Interoperability

Cython can integrate with C++ classes and libraries, enabling you to wrap them for Python use.

### Example: Wrapping a C++ Vector Class

First, create a C++ header for a simple Vector class:

```cpp
// vector.h
#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <cmath>

class Vector {
private:
    double x, y, z;

public:
    // Constructor
    Vector(double x=0, double y=0, double z=0) : x(x), y(y), z(z) {}
    
    // Copy constructor
    Vector(const Vector& other) : x(other.x), y(other.y), z(other.z) {}
    
    // Getters
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    
    // Setters
    void setX(double value) { x = value; }
    void setY(double value) { y = value; }
    void setZ(double value) { z = value; }
    
    // Vector operations
    double magnitude() const {
        return std::sqrt(x*x + y*y + z*z);
    }
    
    Vector normalize() const {
        double mag = magnitude();
        if (mag > 0) {
            return Vector(x/mag, y/mag, z/mag);
        }
        return Vector();
    }
    
    Vector add(const Vector& other) const {
        return Vector(x + other.x, y + other.y, z + other.z);
    }
    
    Vector subtract(const Vector& other) const {
        return Vector(x - other.x, y - other.y, z - other.z);
    }
    
    double dot(const Vector& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    Vector cross(const Vector& other) const {
        return Vector(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    void print() const {
        std::cout << "Vector(" << x << ", " << y << ", " << z << ")" << std::endl;
    }
};

#endif
```

Now let's create a Cython wrapper using `.pxd` and `.pyx` files:

**vector.pxd** (Cython definition file for the C++ class):
```cython
# vector.pxd
cdef extern from "vector.h":
    cdef cppclass Vector:
        Vector() except +
        Vector(double x, double y, double z) except +
        Vector(const Vector&) except +
        
        double getX() const
        double getY() const
        double getZ() const
        
        void setX(double)
        void setY(double)
        void setZ(double)
        
        double magnitude() const
        Vector normalize() const
        Vector add(const Vector&) const
        Vector subtract(const Vector&) const
        double dot(const Vector&) const
        Vector cross(const Vector&) const
        void print() const
```

**py_vector.pyx** (Python wrapper for the C++ class):
```cython
# py_vector.pyx
# distutils: language = c++

from vector cimport Vector

cdef class PyVector:
    """Python wrapper around the C++ Vector class."""
    
    # Hold a C++ Vector instance
    cdef Vector c_vector
    
    def __init__(self, double x=0, double y=0, double z=0):
        self.c_vector = Vector(x, y, z)
    
    # Properties to access x, y, z
    @property
    def x(self):
        return self.c_vector.getX()
    
    @x.setter
    def x(self, double value):
        self.c_vector.setX(value)
    
    @property
    def y(self):
        return self.c_vector.getY()
    
    @y.setter
    def y(self, double value):
        self.c_vector.setY(value)
    
    @property
    def z(self):
        return self.c_vector.getZ()
    
    @z.setter
    def z(self, double value):
        self.c_vector.setZ(value)
    
    # Vector operations
    def magnitude(self):
        return self.c_vector.magnitude()
    
    def normalize(self):
        cdef Vector result = self.c_vector.normalize()
        return PyVector(result.getX(), result.getY(), result.getZ())
    
    def __add__(self, PyVector other):
        cdef Vector result = self.c_vector.add(other.c_vector)
        return PyVector(result.getX(), result.getY(), result.getZ())
    
    def __sub__(self, PyVector other):
        cdef Vector result = self.c_vector.subtract(other.c_vector)
        return PyVector(result.getX(), result.getY(), result.getZ())
    
    def dot(self, PyVector other):
        return self.c_vector.dot(other.c_vector)
    
    def cross(self, PyVector other):
        cdef Vector result = self.c_vector.cross(other.c_vector)
        return PyVector(result.getX(), result.getY(), result.getZ())
    
    def __repr__(self):
        return f"PyVector({self.x}, {self.y}, {self.z})"
    
    # Debug method directly calls the C++ print
    def debug_print(self):
        self.c_vector.print()
```

**setup.py** for C++ compilation:
```python
# setup.py for C++ example
from setuptools import setup, Extension
from Cython.Build import cythonize

# For C++ support
ext_modules = [
    Extension(
        "py_vector",
        sources=["py_vector.pyx"],
        language="c++",  # Compile as C++
        include_dirs=["."],  # Include current directory for vector.h
    )
]

setup(
    ext_modules=cythonize(ext_modules)
)
```

**Usage:**

```python
# test_vector.py
from py_vector import PyVector
import time
import math

# Create some vectors
v1 = PyVector(1.0, 2.0, 3.0)
v2 = PyVector(4.0, 5.0, 6.0)

print(f"v1 = {v1}")
print(f"v2 = {v2}")

# Test operations
print(f"v1 + v2 = {v1 + v2}")
print(f"v1 - v2 = {v1 - v2}")
print(f"v1 · v2 = {v1.dot(v2)}")
print(f"v1 × v2 = {v1.cross(v2)}")
print(f"||v1|| = {v1.magnitude()}")
print(f"v1_norm = {v1.normalize()}")

# Performance test
def test_performance(n=1_000_000):
    start = time.time()
    
    # Test vector operations in loop
    result = PyVector()
    for _ in range(n):
        tmp = v1.cross(v2)
        result = result + tmp.normalize()
    
    elapsed = time.time() - start
    print(f"Performed {n} vector operations in {elapsed:.4f} seconds")
    print(f"Result: {result}")
    return elapsed

# Python equivalent for comparison
class PythonVector:
    def __init__(self, x=0, y=0, z=0):
        self.x = x
        self.y = y
        self.z = z
    
    def magnitude(self):
        return math.sqrt(self.x**2 + self.y**2 + self.z**2)
    
    def normalize(self):
        mag = self.magnitude()
        if mag > 0:
            return PythonVector(self.x/mag, self.y/mag, self.z/mag)
        return PythonVector()
    
    def __add__(self, other):
        return PythonVector(self.x + other.x, self.y + other.y, self.z + other.z)
    
    def __sub__(self, other):
        return PythonVector(self.x - other.x, self.y - other.y, self.z - other.z)
    
    def dot(self, other):
        return self.x * other.x + self.y * other.y + self.z * other.z
    
    def cross(self, other):
        return PythonVector(
            self.y * other.z - self.z * other.y,
            self.z * other.x - self.x * other.z,
            self.x * other.y - self.y * other.x
        )
    
    def __repr__(self):
        return f"PythonVector({self.x}, {self.y}, {self.z})"

def test_python_performance(n=1_000_000):
    py_v1 = PythonVector(1.0, 2.0, 3.0)
    py_v2 = PythonVector(4.0, 5.0, 6.0)
    
    start = time.time()
    
    # Test vector operations in loop
    result = PythonVector()
    for _ in range(n):
        tmp = py_v1.cross(py_v2)
        result = result + tmp.normalize()
    
    elapsed = time.time() - start
    print(f"Python: Performed {n} vector operations in {elapsed:.4f} seconds")
    print(f"Result: {result}")
    return elapsed

# Run performance tests
cython_time = test_performance(1_000_000)
python_time = test_python_performance(1_000_000)
print(f"Speedup: {python_time / cython_time:.1f}x")
```

**Key features:**
- Declaration of C++ class interface in `.pxd` file
- Python wrapper with Pythonic syntax in `.pyx` file
- Operator overloading for natural Python syntax
- The `distutils: language = c++` directive tells Cython this is C++ code
- Exception handling with `except +`

## 5. Shared Memory and GIL-Free Operations

This example demonstrates how to work with shared memory and perform operations without the GIL (Global Interpreter Lock).

```cython
# shared_array.pyx
import numpy as np
cimport numpy as cnp
from libc.stdlib cimport malloc, free
from cython.parallel import prange, parallel

# Create structure for shared array data
cdef struct SharedArray:
    double* data
    int size

# Function to increment values in shared array WITHOUT GIL
cdef void increment_values_nogil(SharedArray* array, double value) nogil:
    cdef int i
    
    for i in range(array.size):
        array.data[i] += value

# Function to process array in parallel WITHOUT GIL
cpdef void parallel_process(cnp.ndarray[cnp.double_t, ndim=1] input_array,
                          cnp.ndarray[cnp.double_t, ndim=1] output_array,
                          double value):
    """
    Process array in parallel without GIL, applying a complex operation.
    """
    cdef:
        int i, n = input_array.shape[0]
        double* input_data = <double*>input_array.data
        double* output_data = <double*>output_array.data
        SharedArray shared_array
    
    # Set up shared array structure
    shared_array.data = <double*>malloc(n * sizeof(double))
    shared_array.size = n
    
    # Copy input data to shared array
    for i in range(n):
        shared_array.data[i] = input_data[i]
    
    # Release GIL and process in parallel
    with nogil, parallel():
        # First increment values
        increment_values_nogil(&shared_array, value)
        
        # Then apply a computation to each element in parallel
        for i in prange(n, schedule='dynamic'):
            output_data[i] = shared_array.data[i] * shared_array.data[i]
    
    # Free shared memory
    free(shared_array.data)

# Python-callable function to test parallel processing
def process_array(cnp.ndarray[cnp.double_t, ndim=1] array, double value):
    """
    Process array with parallel operations.
    """
    cdef cnp.ndarray[cnp.double_t, ndim=1] result = np.zeros_like(array)
    parallel_process(array, result, value)
    return result
```

**Usage:**

```python
# test_shared_array.py
import numpy as np
import time
import matplotlib.pyplot as plt
from shared_array import process_array

# Create test array
size = 50_000_000  # 50 million elements
data = np.random.random(size)

# Process array with different values
results = []
times = []
values = [0.5, 1.0, 2.0, 5.0]

for value in values:
    start = time.time()
    result = process_array(data, value)
    elapsed = time.time() - start
    
    results.append(result)
    times.append(elapsed)
    
    # Calculate mean values for verification
    mean_input = np.mean(data)
    mean_output = np.mean(result)
    
    print(f"Value = {value}, Time = {elapsed:.4f}s")
    print(f"Input mean: {mean_input:.4f}, Output mean: {mean_output:.4f}")
    print(f"Expected output mean: {((mean_input+value)**2):.4f}")
    print()

# Plot performance
plt.figure(figsize=(10, 5))
plt.bar(values, times)
plt.xlabel('Value Added')
plt.ylabel('Processing Time (s)')
plt.title('Parallel Processing Performance')
plt.grid(True, alpha=0.3)
plt.show()
```

**Key features:**
- Use of C structures with `cdef struct`
- Manual memory management with `malloc` and `free`
- Functions declared with `nogil` that can run without Python's GIL
- Parallel processing with `parallel()` and `prange()`
- Direct pointer access to NumPy array data

## 6. Memory Views and Efficient Data Structures

This example shows how to use Cython's memory views and efficient data structures:

```cython
# efficient_graph.pyx
import numpy as np
cimport numpy as cnp
from libc.stdlib cimport malloc, free, realloc
from libc.string cimport memset

# Define a graph node structure
cdef struct Node:
    int id
    int* neighbors  # Dynamic array of neighbor IDs
    int neighbor_count
    int capacity     # Allocated capacity for neighbors

# Define a graph structure
cdef struct Graph:
    Node* nodes
    int node_count

# Initialize a node
cdef void init_node(Node* node, int id, int initial_capacity=4) nogil:
    node.id = id
    node.neighbor_count = 0
    node.capacity = initial_capacity
    node.neighbors = <int*>malloc(initial_capacity * sizeof(int))

# Add a neighbor to a node
cdef void add_neighbor(Node* node, int neighbor_id) nogil:
    cdef int i
    
    # Check if neighbor already exists
    for i in range(node.neighbor_count):
        if node.neighbors[i] == neighbor_id:
            return  # Already a neighbor
    
    # Check if we need to resize the neighbors array
    if node.neighbor_count >= node.capacity:
        node.capacity *= 2
        node.neighbors = <int*>realloc(node.neighbors, node.capacity * sizeof(int))
    
    # Add the neighbor
    node.neighbors[node.neighbor_count] = neighbor_id
    node.neighbor_count += 1

# Create a graph from an edge list
cdef Graph* create_graph(int[:, :] edges, int node_count) nogil:
    cdef:
        Graph* graph = <Graph*>malloc(sizeof(Graph))
        int i, u, v
        int edge_count = edges.shape[0]
    
    # Allocate and initialize nodes
    graph.node_count = node_count
    graph.nodes = <Node*>malloc(node_count * sizeof(Node))
    
    for i in range(node_count):
        init_node(&graph.nodes[i], i)
    
    # Add edges
    for i in range(edge_count):
        u = edges[i, 0]
        v = edges[i, 1]
        
        # Add bidirectional edges
        add_neighbor(&graph.nodes[u], v)
        add_neighbor(&graph.nodes[v], u)
    
    return graph

# Free graph memory
cdef void free_graph(Graph* graph) nogil:
    cdef int i
    
    for i in range(graph.node_count):
        free(graph.nodes[i].neighbors)
    
    free(graph.nodes)
    free(graph)

# Breadth-first search
cdef int* bfs(Graph* graph, int start, int* visited_count) nogil:
    cdef:
        int* queue = <int*>malloc(graph.node_count * sizeof(int))
        int* visited = <int*>malloc(graph.node_count * sizeof(int))
        int* visit_order = <int*>malloc(graph.node_count * sizeof(int))
        int front = 0, rear = 0
        int current, neighbor_idx, neighbor_id
        int visit_idx = 0
    
    # Initialize visited array
    memset(visited, 0, graph.node_count * sizeof(int))
    
    # Start BFS from the start node
    queue[rear] = start
    rear += 1
    visited[start] = 1
    visit_order[visit_idx] = start
    visit_idx += 1
    
    while front < rear:
        current = queue[front]
        front += 1
        
        # Visit all neighbors
        for neighbor_idx in range(graph.nodes[current].neighbor_count):
            neighbor_id = graph.nodes[current].neighbors[neighbor_idx]
            
            if not visited[neighbor_id]:
                visited[neighbor_id] = 1
                queue[rear] = neighbor_id
                rear += 1
                visit_order[visit_idx] = neighbor_id
                visit_idx += 1
    
    # Clean up
    free(queue)
    free(visited)
    
    # Set the count of visited nodes
    visited_count[0] = visit_idx
    
    return visit_order

# Python interface for creating a graph and running BFS
def py_create_graph_and_bfs(cnp.ndarray[cnp.int_t, ndim=2] edges, int node_count, int start_node):
    """
    Create a graph from edges and run BFS from start_node.
    
    Parameters:
    - edges: Nx2 array of edges (u, v)
    - node_count: Number of nodes in the graph
    - start_node: Starting node for BFS
    
    Returns:
    - List of nodes in BFS visit order
    """
    cdef:
        Graph* graph
        int visited_count = 0
        int* visit_order
    
    # Create the graph
    with nogil:
        graph = create_graph(edges, node_count)
    
    # Run BFS
    with nogil:
        visit_order = bfs(graph, start_node, &visited_count)
    
    # Convert result to Python list
    result = [visit_order[i] for i in range(visited_count)]
    
    # Clean up
    with nogil:
        free(visit_order)
        free_graph(graph)
    
    return result
```

**Usage:**

```python
# test_graph.py
import numpy as np
import time
import networkx as nx
import matplotlib.pyplot as plt
from efficient_graph import py_create_graph_and_bfs

# Create a test graph - a grid graph
def create_grid_graph(n, m):
    """Create an nxm grid graph edges."""
    edges = []
    
    for i in range(n):
        for j in range(m):
            node = i * m + j
            
            # Connect to right neighbor
            if j < m - 1:
                edges.append([node, node + 1])
            
            # Connect to bottom neighbor
            if i < n - 1:
                edges.append([node, node + m])
    
    return np.array(edges, dtype=np.int32)

# Create a medium-sized grid graph
n, m = 100, 100
node_count = n * m
edges = create_grid_graph(n, m)
start_node = 0

print(f"Grid graph: {n}x{m} with {node_count} nodes and {len(edges)} edges")

# Test Cython implementation
start = time.time()
bfs_order = py_create_graph_and_bfs(edges, node_count, start_node)
cython_time = time.time() - start

print(f"Cython BFS completed in {cython_time:.4f} seconds")
print(f"Visited {len(bfs_order)} nodes, first 10: {bfs_order[:10]}")

# Compare with NetworkX
G = nx.Graph()
for edge in edges:
    G.add_edge(edge[0], edge[1])

start = time.time()
nx_bfs_order = list(nx.bfs_tree(G, source=start_node).nodes())
nx_time = time.time() - start

print(f"NetworkX BFS completed in {nx_time:.4f} seconds")
print(f"Visited {len(nx_bfs_order)} nodes, first 10: {nx_bfs_order[:10]}")

print(f"Speedup: {nx_time / cython_time:.1f}x")

# Visualize a small portion of the graph
if n <= 10 and m <= 10:
    # Plot the full graph
    subgraph = G
else:
    # Extract a subgraph for visualization
    subgraph = G.subgraph(bfs_order[:100])

pos = nx.spring_layout(subgraph)
plt.figure(figsize=(10, 8))
nx.draw(subgraph, pos, node_color='lightblue', 
        node_size=500, with_labels=True, 
        font_weight='bold', edge_color='gray')
plt.title("Graph Visualization (First 100 nodes from BFS)")
plt.show()
```

**Key features:**
- Custom C structures for graph representation
- Dynamic memory management with `malloc`, `realloc`, and `free`
- Use of efficient memory views for array access
- Implementation of breadth-first search algorithm
- Manual reference counting and memory management
- Inter-operation with Python libraries (NetworkX)

## 7. Using External C Libraries with Cython

Cython excels at integrating with external C libraries. Here's an example using SQLite:

```cython
# sqlite_wrapper.pyx
from libc.stdlib cimport malloc, free
from libc.string cimport strcpy, strlen

# Import SQLite C API
cdef extern from "sqlite3.h":
    # Type definitions
    ctypedef struct sqlite3:
        pass
    
    ctypedef struct sqlite3_stmt:
        pass
    
    # Constants
    int SQLITE_OK
    int SQLITE_ROW
    int SQLITE_DONE
    
    # Functions
    int sqlite3_open(const char *filename, sqlite3 **ppDb)
    int sqlite3_close(sqlite3 *db)
    int sqlite3_exec(sqlite3 *db, const char *sql, 
                    int (*callback)(void*,int,char**,char**), 
                    void *arg, char **errmsg)
    int sqlite3_prepare_v2(sqlite3 *db, const char *zSql, 
                          int nByte, sqlite3_stmt **ppStmt, 
                          const char **pzTail)
    int sqlite3_step(sqlite3_stmt *pStmt)
    int sqlite3_finalize(sqlite3_stmt *pStmt)
    int sqlite3_column_count(sqlite3_stmt *pStmt)
    const char *sqlite3_column_name(sqlite3_stmt *pStmt, int N)
    const char *sqlite3_column_text(sqlite3_stmt *pStmt, int iCol)
    int sqlite3_column_int(sqlite3_stmt *pStmt, int iCol)
    double sqlite3_column_double(sqlite3_stmt *pStmt, int iCol)
    const char *sqlite3_errmsg(sqlite3 *db)

# Python wrapper class for SQLite database
cdef class Database:
    cdef sqlite3 *db
    cdef bint is_open
    
    def __cinit__(self, filename):
        self.is_open = False
        
        # Convert filename to bytes if it's a string
        cdef bytes py_bytes
        if isinstance(filename, str):
            py_bytes = filename.encode('utf-8')
        else:
            py_bytes = filename
        
        # Open the database
        if sqlite3_open(py_bytes, &self.db) != SQLITE_OK:
            error_msg = sqlite3_errmsg(self.db)
            sqlite3_close(self.db)
            raise RuntimeError(f"Failed to open database: {error_msg.decode('utf-8')}")
        
        self.is_open = True
    
    def __dealloc__(self):
        if self.is_open:
            sqlite3_close(self.db)
    
    def execute(self, query):
        """Execute a query without returning results."""
        cdef:
            bytes py_query
            char *error_msg = NULL
            int result
        
        if not self.is_open:
            raise RuntimeError("Database is not open")
        
        # Convert query to bytes
        if isinstance(query, str):
            py_query = query.encode('utf-8')
        else:
            py_query = query
        
        # Execute the query
        result = sqlite3_exec(self.db, py_query, NULL, NULL, &error_msg)
        
        if result != SQLITE_OK:
            error_str = error_msg.decode('utf-8') if error_msg != NULL else "Unknown error"
            raise RuntimeError(f"SQLite error: {error_str}")
    
    def query(self, query):
        """Execute a query and return results as a list of dictionaries."""
        cdef:
            bytes py_query
            sqlite3_stmt *stmt
            int result, col_count, i
            dict row
            list rows = []
            str col_name
            object col_value
        
        if not self.is_open:
            raise RuntimeError("Database is not open")
        
        # Convert query to bytes
        if isinstance(query, str):
            py_query = query.encode('utf-8')
        else:
            py_query = query
        
        # Prepare the statement
        result = sqlite3_prepare_v2(self.db, py_query, -1, &stmt, NULL)
        if result != SQLITE_OK:
            error_msg = sqlite3_errmsg(self.db)
            raise RuntimeError(f"Failed to prepare query: {error_msg.decode('utf-8')}")
        
        try:
            # Get column count
            col_count = sqlite3_column_count(stmt)
            
            # Get column names
            column_names = []
            for i in range(col_count):
                col_name = sqlite3_column_name(stmt, i).decode('utf-8')
                column_names.append(col_name)
            
            # Execute the query and fetch results
            while True:
                result = sqlite3_step(stmt)
                
                if result == SQLITE_ROW:
                    # Process a row
                    row = {}
                    for i in range(col_count):
                        # Get column value based on type
                        # For simplicity, we're only handling text, integers, and floats
                        # A real implementation would handle all SQLite types
                        col_value_text = sqlite3_column_text(stmt, i)
                        if col_value_text != NULL:
                            col_value = col_value_text.decode('utf-8')
                            
                            # Try to convert to int or float if possible
                            try:
                                col_value = int(col_value)
                            except ValueError:
                                try:
                                    col_value = float(col_value)
                                except ValueError:
                                    pass
                        else:
                            col_value = None
                        
                        row[column_names[i]] = col_value
                    
                    rows.append(row)
                
                elif result == SQLITE_DONE:
                    # No more rows
                    break
                
                else:
                    # Error
                    error_msg = sqlite3_errmsg(self.db)
                    raise RuntimeError(f"SQLite error: {error_msg.decode('utf-8')}")
            
            return rows
        
        finally:
            # Always finalize the statement
            sqlite3_finalize(stmt)
```

**Usage example:**

```python
# test_sqlite.py
import os
from sqlite_wrapper import Database
import time
import matplotlib.pyplot as plt

# Create test database
DB_PATH = "test_database.db"

# Remove old database if it exists
if os.path.exists(DB_PATH):
    os.remove(DB_PATH)

# Create and populate database
db = Database(DB_PATH)

# Create schema
db.execute("""
CREATE TABLE users (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    email TEXT NOT NULL,
    age INTEGER,
    created_at TEXT
);
""")

# Insert test data
print("Inserting test data...")
start = time.time()

for i in range(1000):
    db.execute(f"""
    INSERT INTO users (name, email, age, created_at)
    VALUES ('User {i}', 'user{i}@example.com', {20 + (i % 60)}, datetime('now'));
    """)

insert_time = time.time() - start
print(f"Inserted 1000 records in {insert_time:.4f} seconds")

# Query data
print("\nQuerying all users...")
start = time.time()
results = db.query("SELECT * FROM users LIMIT 10")
query_time = time.time() - start
print(f"Query executed in {query_time:.4f} seconds")
print(f"First 5 results: {results[:5]}")

# Aggregation query
print("\nRunning aggregation query...")
start = time.time()
age_groups = db.query("""
SELECT 
    (age / 10) * 10 as age_group,
    COUNT(*) as count
FROM users
GROUP BY age_group
ORDER BY age_group;
""")
agg_time = time.time() - start
print(f"Aggregation executed in {agg_time:.4f} seconds")
print(f"Age distribution: {age_groups}")

# Plot age distribution
ages = [row['age_group'] for row in age_groups]
counts = [row['count'] for row in age_groups]

plt.figure(figsize=(10, 6))
plt.bar(ages, counts, width=8)
plt.xlabel('Age Group')
plt.ylabel('Number of Users')
plt.title('User Age Distribution')
plt.xticks(ages)
plt.grid(True, alpha=0.3)
plt.show()
```

**Key features:**
- Declaration of C functions and structures from SQLite using `cdef extern`
- Memory management with `__cinit__` and `__dealloc__`
- Type conversion between Python and C types
- Error handling with SQLite error codes
- Integration with external C library without writing any C code

## 8. Optimization of Mathematical Algorithms

Cython is particularly effective for optimizing mathematical algorithms. Here's an implementation of a fast prime number generator:

```cython
# primes.pyx
import numpy as np
cimport numpy as cnp
from libc.stdlib cimport malloc, free
from libc.math cimport sqrt, floor
from cython.parallel import prange

def sieve_of_eratosthenes(int n):
    """
    Generate all prime numbers up to n using Sieve of Eratosthenes.
    
    This is a pure Python implementation using NumPy for comparison.
    """
    # Create a boolean array for all numbers up to n
    is_prime = np.ones(n+1, dtype=bool)
    is_prime[0] = is_prime[1] = False
    
    # Mark multiples of each prime as non-prime
    for i in range(2, int(np.sqrt(n)) + 1):
        if is_prime[i]:
            is_prime[i*i:n+1:i] = False
    
    # Return prime numbers
    return np.where(is_prime)[0]

cdef int* fast_sieve(int n, int* count) nogil:
    """
    C implementation of Sieve of Eratosthenes.
    Returns a pointer to an array of primes and sets count to the number of primes.
    """
    cdef:
        char* is_prime = <char*>malloc((n+1) * sizeof(char))
        int i, j, p
        int sqrt_n = <int>floor(sqrt(n))
        int* primes
        int primes_count = 0
    
    # Initialize all as potential primes
    for i in range(n+1):
        is_prime[i] = 1
    
    # 0 and 1 are not prime
    is_prime[0] = is_prime[1] = 0
    
    # Mark non-primes
    for i in range(2, sqrt_n + 1):
        if is_prime[i]:
            # Mark all multiples of i as non-prime
            j = i * i
            while j <= n:
                is_prime[j] = 0
                j += i
    
    # Count primes
    for i in range(2, n+1):
        if is_prime[i]:
            primes_count += 1
    
    # Allocate memory for primes
    primes = <int*>malloc(primes_count * sizeof(int))
    
    # Fill primes array
    j = 0
    for i in range(2, n+1):
        if is_prime[i]:
            primes[j] = i
            j += 1
    
    # Free temporary array
    free(is_prime)
    
    # Set output count
    count[0] = primes_count
    
    return primes

cdef int* segmented_sieve(int n, int* count) nogil:
    """
    Optimized implementation using segmented sieve.
    Good for very large values of n.
    """
    cdef:
        int sqrt_n = <int>floor(sqrt(n))
        int segment_size = max(sqrt_n, 1024)
        int* small_primes
        int small_primes_count = 0
        char* segment
        int low, high, i, j, p, idx
        int* primes
        int primes_count = 0
        int* prime_multiples
    
    # Get small primes up to sqrt(n)
    small_primes = fast_sieve(sqrt_n, &small_primes_count)
    
    # Allocate initial memory for primes (we'll realloc if needed)
    primes = <int*>malloc(max(n//10, 1000) * sizeof(int))
    
    # Copy small primes to output array
    for i in range(small_primes_count):
        primes[primes_count] = small_primes[i]
        primes_count += 1
    
    # Allocate segment array
    segment = <char*>malloc(segment_size * sizeof(char))
    
    # Allocate array to track multiples of small primes
    prime_multiples = <int*>malloc(small_primes_count * sizeof(int))
    
    # Sieve segments
    low = sqrt_n + 1
    while low <= n:
        high = min(low + segment_size - 1, n)
        
        # Initialize segment
        for i in range(high - low + 1):
            segment[i] = 1
        
        # Initialize prime multiples
        for i in range(small_primes_count):
            p = small_primes[i]
            j = (low // p) * p
            if j < low:
                j += p
            prime_multiples[i] = j
        
        # Sieve segment
        for i in range(small_primes_count):
            p = small_primes[i]
            j = prime_multiples[i]
            
            while j <= high:
                segment[j - low] = 0
                j += p
        
        # Collect primes from segment
        for i in range(high - low + 1):
            if segment[i]:
                primes[primes_count] = low + i
                primes_count += 1
        
        # Move to next segment
        low += segment_size
    
    # Free temporary memory
    free(small_primes)
    free(segment)
    free(prime_multiples)
    
    # Set output count
    count[0] = primes_count
    
    return primes

def py_get_primes(int n, method='segmented'):
    """
    Python-callable function to get primes up to n.
    
    Parameters:
    - n: Upper limit
    - method: 'simple', 'fast', or 'segmented'
    
    Returns:
    - NumPy array of primes
    """
    if method == 'simple':
        return sieve_of_eratosthenes(n)
    
    cdef:
        int* primes
        int count = 0
        cnp.ndarray[cnp.int32_t, ndim=1] result
    
    # Use appropriate C method
    if method == 'fast':
        primes = fast_sieve(n, &count)
    else:  # segmented
        primes = segmented_sieve(n, &count)
    
    # Convert to NumPy array
    result = np.zeros(count, dtype=np.int32)
    for i in range(count):
        result[i] = primes[i]
    
    # Free C array
    free(primes)
    
    return result

def prime_factorization(int n):
    """
    Find the prime factorization of n.
    Returns a list of (prime, exponent) tuples.
    """
    cdef:
        cnp.ndarray[cnp.int32_t, ndim=1] primes
        int p, exp
        int i = 0
        list factors = []
    
    if n <= 1:
        return []
    
    # Get primes up to sqrt(n)
    primes = py_get_primes(<int>sqrt(n) + 1)
    
    # Try dividing by each prime
    while i < len(primes) and n > 1:
        p = primes[i]
        
        # Count how many times n is divisible by p
        exp = 0
        while n % p == 0:
            n //= p
            exp += 1
        
        if exp > 0:
            factors.append((p, exp))
        
        i += 1
    
    # If n is still > 1, it's a prime factor itself
    if n > 1:
        factors.append((n, 1))
    
    return factors
```

**Usage example:**

```python
# test_primes.py
import time
import numpy as np
import matplotlib.pyplot as plt
from primes import py_get_primes, prime_factorization

# Compare different methods
def benchmark_methods(n, iterations=3):
    methods = ['simple', 'fast', 'segmented']
    times = {m: 0 for m in methods}
    counts = {m: 0 for m in methods}
    
    for method in methods:
        print(f"Testing {method} method...")
        total_time = 0
        
        for _ in range(iterations):
            start = time.time()
            primes = py_get_primes(n, method=method)
            method_time = time.time() - start
            total_time += method_time
            
            print(f"  Run: {method_time:.4f}s, found {len(primes)} primes")
        
        avg_time = total_time / iterations
        times[method] = avg_time
        counts[method] = len(primes)
        print(f"  Average: {avg_time:.4f}s")
        print(f"  First 10 primes: {primes[:10]}")
        print(f"  Last 10 primes: {primes[-10:]}")
        print()
    
    return times, counts

# Test prime factorization
def test_factorization():
    test_numbers = [
        12, 100, 1234, 9876, 123456, 7654321, 2**31 - 1  # Mersenne prime
    ]
    
    for n in test_numbers:
        start = time.time()
        factors = prime_factorization(n)
        factorization_time = time.time() - start
        
        # Verify the factorization
        product = 1
        for p, exp in factors:
            product *= p ** exp
        
        print(f"Prime factorization of {n}:")
        print(f"  Factors: {factors}")
        print(f"  Verification: {product == n} ({product})")
        print(f"  Time: {factorization_time:.6f}s")
        print()

# Run benchmarks
n = 10_000_000  # 10 million
print(f"Finding primes up to {n:,}...")
times, counts = benchmark_methods(n)

# Plot results
plt.figure(figsize=(10, 6))
methods = list(times.keys())
method_times = [times[m] for m in methods]

plt.bar(methods, method_times)
plt.ylabel('Time (seconds)')
plt.title(f'Prime Number Generation Performance (up to {n:,})')
plt.grid(True, alpha=0.3)

# Add count labels
for i, m in enumerate(methods):
    plt.text(i, times[m] + 0.05, f"{counts[m]:,} primes", 
             ha='center', va='bottom', fontweight='bold')

plt.tight_layout()
plt.show()

# Test prime factorization
test_factorization()
```

**Key features:**
- Multiple algorithm implementations for comparison
- Direct memory management for maximum performance
- Function pointers and callbacks
- Manual memory management
- Segmented sieve algorithm for handling large ranges
- C functions with `nogil` for maximum performance

## 9. Writing Cython Extension Modules for Python Packages

Here's how to structure a Cython extension module that can be installed as a Python package:

**Project structure:**
```
mymath/
├── mymath/
│   ├── __init__.py
│   ├── _core.pyx
│   ├── _vector.pyx
│   ├── _matrix.pyx
│   └── utils.py
├── setup.py
├── pyproject.toml
└── README.md
```

**`mymath/__init__.py`**:
```python
"""
MyMath - High-performance math utilities implemented in Cython
"""

from ._core import factorial, fibonacci
from ._vector import Vector, dot_product, cross_product
from ._matrix import Matrix, identity, inverse

__version__ = '0.1.0'
```

**`mymath/_core.pyx`**:
```cython
# cython: boundscheck=False, wraparound=False, initializedcheck=False

from libc.math cimport pow

def factorial(int n):
    """
    Calculate the factorial of n using an optimized implementation.
    
    Args:
        n: Non-negative integer
        
    Returns:
        n! (n factorial)
    """
    if n < 0:
        raise ValueError("Factorial is only defined for non-negative integers")
    
    cdef:
        int i
        double result = 1.0
    
    for i in range(2, n+1):
        result *= i
    
    return result

def fibonacci(int n):
    """
    Calculate the nth Fibonacci number.
    
    Args:
        n: Non-negative integer
        
    Returns:
        The nth Fibonacci number
    """
    if n < 0:
        raise ValueError("Fibonacci is only defined for non-negative integers")
    
    cdef:
        int i
        double a=0, b=1, temp
    
    if n == 0:
        return a
    
    for i in range(1, n):
        temp = a + b
        a = b
        b = temp
    
    return b

cpdef double log_base(double x, double base):
    """
    Calculate the logarithm of x with given base.
    
    Args:
        x: Positive number
        base: Positive number, not equal to 1
        
    Returns:
        Logarithm of x with base
    """
    from libc.math cimport log
    
    if x <= 0:
        raise ValueError("Logarithm is only defined for positive numbers")
    if base <= 0 or base == 1:
        raise ValueError("Base must be positive and not equal to 1")
    
    return log(x) / log(base)
```

**`mymath/_vector.pyx`**:
```cython
# cython: boundscheck=False, wraparound=False, initializedcheck=False
import numpy as np
cimport numpy as cnp

# Initialize NumPy C API
cnp.import_array()

cdef class Vector:
    """
    A 3D vector class with optimized operations.
    """
    cdef:
        public double x, y, z
    
    def __init__(self, double x=0, double y=0, double z=0):
        self.x = x
        self.y = y
        self.z = z
    
    def __repr__(self):
        return f"Vector({self.x}, {self.y}, {self.z})"
    
    def __add__(Vector self, Vector other):
        return Vector(self.x + other.x, self.y + other.y, self.z + other.z)
    
    def __sub__(Vector self, Vector other):
        return Vector(self.x - other.x, self.y - other.y, self.z - other.z)
    
    def __mul__(self, double scalar):
        return Vector(self.x * scalar, self.y * scalar, self.z * scalar)
    
    def __rmul__(self, double scalar):
        return self * scalar
    
    def __truediv__(self, double scalar):
        if scalar == 0:
            raise ZeroDivisionError("Division by zero")
        return Vector(self.x / scalar, self.y / scalar, self.z / scalar)
    
    def dot(Vector self, Vector other):
        """Calculate dot product with another vector."""
        return self.x * other.x + self.y * other.y + self.z * other.z
    
    def cross(Vector self, Vector other):
        """Calculate cross product with another vector."""
        return Vector(
            self.y * other.z - self.z * other.y,
            self.z * other.x - self.x * other.z,
            self.x * other.y - self.y * other.x
        )
    
    def magnitude(self):
        """Calculate the magnitude (length) of the vector."""
        from libc.math cimport sqrt
        return sqrt(self.x * self.x + self.y * self.y + self.z * self.z)
    
    def normalize(self):
        """Return a normalized version of the vector."""
        cdef double mag = self.magnitude()
        if mag == 0:
            raise ValueError("Cannot normalize zero vector")
        return self / mag
    
    def to_array(self):
        """Convert to NumPy array."""
        return np.array([self.x, self.y, self.z])
    
    @classmethod
    def from_array(cls, array):
        """Create vector from a sequence."""
        return cls(array[0], array[1], array[2])

def dot_product(cnp.ndarray[cnp.double_t, ndim=1] a, 
                cnp.ndarray[cnp.double_t, ndim=1] b):
    """
    Calculate dot product of two 1D arrays.
    """
    cdef:
        int i, n
        double result = 0.0
    
    # Check dimensions
    if a.shape[0] != b.shape[0]:
        raise ValueError("Arrays must have the same length")
    
    n = a.shape[0]
    
    # Calculate dot product
    for i in range(n):
        result += a[i] * b[i]
    
    return result

def cross_product(cnp.ndarray[cnp.double_t, ndim=1] a, 
                  cnp.ndarray[cnp.double_t, ndim=1] b):
    """
    Calculate cross product of two 3D vectors.
    """
    cdef:
        cnp.ndarray[cnp.double_t, ndim=1] result
    
    # Check dimensions
    if a.shape[0] != 3 or b.shape[0] != 3:
        raise ValueError("Arrays must have length 3 for cross product")
    
    # Calculate cross product
    result = np.zeros(3, dtype=np.double)
    result[0] = a[1] * b[2] - a[2] * b[1]
    result[1] = a[2] * b[0] - a[0] * b[2]
    result[2] = a[0] * b[1] - a[1] * b[0]
    
    return result
```

**`setup.py`**:
```python
from setuptools import setup, Extension, find_packages
from Cython.Build import cythonize
import numpy

# Define extensions
extensions = [
    Extension(
        "mymath._core",
        ["mymath/_core.pyx"],
    ),
    Extension(
        "mymath._vector",
        ["mymath/_vector.pyx"],
        include_dirs=[numpy.get_include()],
    ),
    Extension(
        "mymath._matrix",
        ["mymath/_matrix.pyx"],
        include_dirs=[numpy.get_include()],
    ),
]

# Setup configuration
setup(
    name="mymath",
    version="0.1.0",
    description="High-performance math utilities implemented in Cython",
    author="Your Name",
    author_email="your.email@example.com",
    url="https://github.com/yourusername/mymath",
    packages=find_packages(),
    ext_modules=cythonize(extensions, compiler_directives={
        "language_level": 3,
        "boundscheck": False,
        "wraparound": False,
        "initializedcheck": False,
    }),
    install_requires=[
        "numpy>=1.20.0",
    ],
    python_requires=">=3.7",
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Cython",
        "Topic :: Scientific/Engineering :: Mathematics",
    ],
)
```

**`pyproject.toml`**:
```toml
[build-system]
requires = ["setuptools>=42", "wheel", "Cython>=0.29.21", "numpy>=1.20.0"]
build-backend = "setuptools.build_meta"
```

**Installation and usage:**

```bash
# Install in development mode
pip install -e .
```

```python
# Example usage (continued)
import numpy as np
from mymath import factorial, fibonacci, Vector
import time

# Test factorial
print(f"factorial(10) = {factorial(10)}")

# Test fibonacci
print(f"fibonacci(20) = {fibonacci(20)}")

# Test vector operations
v1 = Vector(1, 2, 3)
v2 = Vector(4, 5, 6)

print(f"v1 = {v1}")
print(f"v2 = {v2}")
print(f"v1 + v2 = {v1 + v2}")
print(f"v1 · v2 = {v1.dot(v2)}")
print(f"v1 × v2 = {v1.cross(v2)}")
print(f"|v1| = {v1.magnitude()}")
print(f"v1_norm = {v1.normalize()}")

# Performance test
def benchmark_vector_operations(n=1_000_000):
    start = time.time()
    
    result = Vector()
    for _ in range(n):
        # Mix of operations
        temp = v1.cross(v2)
        result = result + temp.normalize()
    
    elapsed = time.time() - start
    print(f"Performed {n:,} vector operations in {elapsed:.4f} seconds")
    return elapsed

benchmark_vector_operations()
```

## 10. Debugging Cython Code

Debugging Cython code requires special consideration since the compiled code doesn't map directly to your source. Here's how to add effective debugging capabilities:

```cython
# debug_example.pyx
# cython: linetrace=True
# distutils: define_macros=CYTHON_TRACE_NOGIL=1

import time
from cython.parallel import prange
import cython

# Regular function with debug prints
def complex_calculation(double x, double y, int iterations=1000):
    """Calculate a complex mathematical function with debugging."""
    cdef:
        int i
        double result = 0.0
        double intermediate
    
    print(f"Starting calculation with x={x}, y={y}, iterations={iterations}")
    
    for i in range(iterations):
        intermediate = (x * i + y) / (i + 1)
        result += intermediate
        
        # Conditional debugging for suspicious values
        if intermediate > 1000 or intermediate < -1000:
            print(f"Warning: Large intermediate value at i={i}: {intermediate}")
    
    print(f"Calculation completed with result={result}")
    return result

# Function with Cython assertions
@cython.boundscheck(False)  # Turn off bounds checking for performance
@cython.wraparound(False)   # Turn off negative indexing for performance
def array_processing(double[:] data not None):
    """Process an array with assertions for debugging."""
    cdef:
        int i, n = data.shape[0]
        double[:] result = cython.view.array((n,), dtype=float)
    
    for i in range(n):
        # Debug assertions that get removed in optimized builds
        cython.declare(compiled=True)  # Only executed in Cython
        if cython.compiled:
            assert i >= 0 and i < n, f"Index {i} out of bounds for array of size {n}"
        
        result[i] = data[i] * data[i]
    
    return result

# Function with Python traceback
def risky_division(double[:] numerators, double[:] denominators):
    """Division function with traceback for debugging."""
    cdef:
        int i, n
        double[:] result
    
    if numerators.shape[0] != denominators.shape[0]:
        raise ValueError("Arrays must have the same length")
    
    n = numerators.shape[0]
    result = cython.view.array((n,), dtype=float)
    
    for i in range(n):
        if denominators[i] == 0:
            # This will show full traceback with line numbers
            raise ZeroDivisionError(f"Division by zero at index {i}")
        
        result[i] = numerators[i] / denominators[i]
    
    return result

# Parallel function with nogil
def parallel_sum(double[:, :] matrix):
    """Calculate row sums in parallel with GIL released."""
    cdef:
        int i, j, rows = matrix.shape[0], cols = matrix.shape[1]
        double[:] result = cython.view.array((rows,), dtype=float)
    
    # Despite nogil, we can still get debug info with linetrace enabled
    with nogil:
        for i in prange(rows):
            for j in range(cols):
                result[i] += matrix[i, j]
    
    return result

# Function to demonstrate debugging with the 'gdb' debugger
def binary_search(int[:] arr, int target):
    """Binary search algorithm that can be debugged with GDB."""
    cdef:
        int low = 0
        int high = arr.shape[0] - 1
        int mid
    
    while low <= high:
        mid = (low + high) // 2
        
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            low = mid + 1
        else:
            high = mid - 1
    
    return -1  # Target not found
```

**Debugging demonstration script:**

```python
# test_debugging.py
import numpy as np
from debug_example import (
    complex_calculation, array_processing, 
    risky_division, parallel_sum, binary_search
)

def test_complex_calculation():
    print("\n=== Testing complex_calculation ===")
    try:
        # Normal case
        result = complex_calculation(2.0, 3.0, 100)
        print(f"Result: {result}\n")
        
        # Edge case that generates warnings
        result = complex_calculation(1000.0, -500.0, 50)
        print(f"Result: {result}\n")
    except Exception as e:
        print(f"Error: {e}\n")

def test_array_processing():
    print("\n=== Testing array_processing ===")
    try:
        data = np.array([1.0, 2.0, 3.0, 4.0, 5.0])
        result = array_processing(data)
        print(f"Input: {data}")
        print(f"Result: {np.asarray(result)}\n")
    except Exception as e:
        print(f"Error: {e}\n")

def test_risky_division():
    print("\n=== Testing risky_division ===")
    try:
        numerators = np.array([1.0, 4.0, 9.0, 16.0])
        denominators = np.array([1.0, 2.0, 0.0, 4.0])  # Zero at index 2
        
        print(f"Numerators: {numerators}")
        print(f"Denominators: {denominators}")
        
        result = risky_division(numerators, denominators)
        print(f"Result: {np.asarray(result)}\n")
    except Exception as e:
        import traceback
        print(f"Error: {e}")
        traceback.print_exc()
        print()

def test_parallel_sum():
    print("\n=== Testing parallel_sum ===")
    try:
        # Create a test matrix
        matrix = np.array([
            [1.0, 2.0, 3.0],
            [4.0, 5.0, 6.0],
            [7.0, 8.0, 9.0]
        ])
        
        print(f"Matrix:\n{matrix}")
        
        result = parallel_sum(matrix)
        print(f"Row sums: {np.asarray(result)}")
        print(f"Expected: {np.sum(matrix, axis=1)}\n")
    except Exception as e:
        print(f"Error: {e}\n")

def test_binary_search():
    print("\n=== Testing binary_search ===")
    try:
        # Create a sorted array
        arr = np.array([1, 3, 5, 7, 9, 11, 13, 15], dtype=np.int32)
        
        print(f"Array: {arr}")
        
        # Test several cases
        targets = [1, 7, 15, 6]
        for target in targets:
            index = binary_search(arr, target)
            if index != -1:
                print(f"Found {target} at index {index}")
            else:
                print(f"{target} not found in array")
        print()
    except Exception as e:
        print(f"Error: {e}\n")

if __name__ == "__main__":
    test_complex_calculation()
    test_array_processing()
    test_risky_division()
    test_parallel_sum()
    test_binary_search()
```

**Key debugging features:**
1. Enable line tracing with `# cython: linetrace=True`
2. Add strategic print statements for basic debugging
3. Use assertions that can be conditionally compiled
4. Leverage Python exceptions with full tracebacks
5. Enable line tracing for nogil sections with `CYTHON_TRACE_NOGIL=1`
6. Use GDB for low-level debugging if needed

## 11. Cython and Python Compatibility: Specialized Functionality

Here's an example of integrating with Python data structures while maintaining high performance:

```cython
# py_compat.pyx
import cython
from cpython.dict cimport PyDict_Keys, PyDict_GetItem, PyDict_SetItem
from cpython.list cimport PyList_GetItem, PyList_SetItem, PyList_Size
from cpython.ref cimport PyObject
from cpython.object cimport PyObject_GetItem, PyObject_SetItem

# Fast dictionary operations
def count_word_frequency(text):
    """
    Count word frequency in a text using optimized dictionary access.
    
    This demonstrates fast Python object manipulation from Cython.
    """
    cdef:
        dict word_count = {}
        list words = text.lower().split()
        int i, n = len(words)
        str word
        PyObject* py_word
        PyObject* count_obj
        long count
    
    for i in range(n):
        word = words[i]
        
        # Fast dictionary lookup and update using CPython C API
        py_word = <PyObject*>word
        count_obj = PyDict_GetItem(word_count, py_word)
        
        if count_obj is NULL:
            # Word not found in dict, add with count 1
            PyDict_SetItem(word_count, py_word, 1)
        else:
            # Word found, increment count
            count = <long>count_obj + 1
            PyDict_SetItem(word_count, py_word, count)
    
    return word_count

# Efficient generator function
def fibonacci_generator(int n):
    """
    Generate Fibonacci numbers up to n.
    
    This demonstrates efficient Python yield statements in Cython.
    """
    cdef:
        int i
        long a = 0, b = 1, temp
    
    if n >= 1:
        yield a
    if n >= 2:
        yield b
    
    for i in range(2, n):
        temp = a + b
        a = b
        b = temp
        yield b

# Using Python decorators with Cython functions
def timer_decorator(func):
    """Simple timer decorator."""
    import time
    def wrapper(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        elapsed = time.time() - start
        print(f"{func.__name__} took {elapsed:.4f} seconds")
        return result
    return wrapper

@timer_decorator
def optimized_calculation(double[:] data, int iterations=1000):
    """
    Demonstration of using Python decorators with Cython functions.
    """
    cdef:
        int i, j
        double total = 0.0
    
    for i in range(iterations):
        for j in range(data.shape[0]):
            total += data[j] * (i + 1)
    
    return total

# Context manager in Cython
@cython.cclass
class TimedBlock:
    """
    Context manager for timing code blocks.
    """
    cdef:
        double start_time
        str block_name
    
    def __init__(self, block_name):
        self.block_name = block_name
    
    def __enter__(self):
        import time
        self.start_time = time.time()
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        import time
        elapsed = time.time() - self.start_time
        print(f"{self.block_name} completed in {elapsed:.4f} seconds")
        return False  # Don't suppress exceptions
```

**Usage example:**

```python
# test_py_compat.py
import numpy as np
from py_compat import (
    count_word_frequency, fibonacci_generator, 
    optimized_calculation, TimedBlock
)

# Test word frequency counter
def test_word_counter():
    print("\n=== Testing Word Frequency Counter ===")
    text = """
    Python is an interpreted high-level general-purpose programming language. 
    Python's design philosophy emphasizes code readability with its notable use 
    of significant indentation. Its language constructs as well as its 
    object-oriented approach aim to help programmers write clear, logical code 
    for small and large-scale projects.
    """
    
    with TimedBlock("Word frequency count"):
        frequencies = count_word_frequency(text)
    
    # Print most common words
    print("Most frequent words:")
    for word, count in sorted(frequencies.items(), key=lambda x: x[1], reverse=True)[:10]:
        print(f"  {word}: {count}")
    print()

# Test Fibonacci generator
def test_fibonacci_generator():
    print("\n=== Testing Fibonacci Generator ===")
    n = 20
    
    print(f"First {n} Fibonacci numbers:")
    for i, fib in enumerate(fibonacci_generator(n)):
        print(f"  Fibonacci({i}): {fib}")
    print()

# Test optimized calculation with decorator
def test_optimized_calculation():
    print("\n=== Testing Optimized Calculation ===")
    
    # Create test data
    data = np.random.random(10000)
    
    # Run calculation (will be timed by decorator)
    result = optimized_calculation(data, iterations=100)
    print(f"Calculation result: {result}")
    print()

# Test context manager
def test_context_manager():
    print("\n=== Testing Context Manager ===")
    
    # Use the context manager for timing
    with TimedBlock("Array multiplication"):
        a = np.random.random((1000, 1000))
        b = np.random.random((1000, 1000))
        c = a @ b
        sum_result = np.sum(c)
    
    print(f"Matrix multiplication sum: {sum_result}")
    print()

if __name__ == "__main__":
    test_word_counter()
    test_fibonacci_generator()
    test_optimized_calculation()
    test_context_manager()
```

**Key Python compatibility features:**
1. CPython C API access via `cimports`
2. Generator functions with `yield`
3. Python decorators on Cython functions
4. Context managers using `__enter__` and `__exit__`
5. Mixing Python objects with Cython optimizations

## Conclusion

This comprehensive guide has covered various advanced aspects of Cython with practical, working examples. Cython offers a powerful way to optimize Python code while maintaining compatibility with the Python ecosystem.

The key benefits demonstrated include:

1. **Performance Optimization**: Orders of magnitude faster than equivalent Python code
2. **Integration Capabilities**: Seamless interoperability with C, C++, and Python
3. **Gradual Adoption**: Ability to optimize specific bottlenecks while keeping most code as Python
4. **Memory Efficiency**: Direct control over memory layout and management
5. **Parallel Processing**: Easy multithreading with the GIL released

You can use these examples as a foundation for your own Cython projects. Remember to follow these best practices:

1. Profile first to identify bottlenecks
2. Start with simple type declarations before adding more complex optimizations
3. Use memory views for efficient array operations
4. Release the GIL for CPU-bound parallel code
5. Leverage compiler directives like `boundscheck=False` for performance-critical sections
6. Add proper error handling and validation
7. Enable Cython's annotation to visualize Python interaction


---
---

# Phase 2: Core Concepts in Cython

## 1. Static Typing

Static typing is the heart of Cython's performance gains. By declaring variable types, you enable C-level operations that bypass Python's dynamic machinery.

### Basic Type Declarations

```cython
# basic_typing.pyx
def untyped_sum(n):
    """Python function with no type information."""
    result = 0
    for i in range(n):
        result += i
    return result

def typed_sum(int n):
    """Function with typed parameter but untyped local variables."""
    result = 0
    for i in range(n):
        result += i
    return result

def fully_typed_sum(int n):
    """Fully typed function with C-level operations."""
    cdef int i, result = 0
    for i in range(n):
        result += i
    return result
```

Let's benchmark these functions to see the impact:

```python
# test_typing.py
import time
import matplotlib.pyplot as plt
from basic_typing import untyped_sum, typed_sum, fully_typed_sum

def benchmark(func, n, iterations=100):
    start = time.time()
    result = None
    for _ in range(iterations):
        result = func(n)
    elapsed = time.time() - start
    return elapsed, result

n = 1000000
iterations = 100

functions = [
    ("Untyped", untyped_sum),
    ("Typed Parameter", typed_sum),
    ("Fully Typed", fully_typed_sum)
]

results = []
for name, func in functions:
    time_taken, result = benchmark(func, n, iterations)
    results.append((name, time_taken))
    print(f"{name}: {time_taken:.6f} seconds, result = {result}")

# Plot results
names = [r[0] for r in results]
times = [r[1] for r in results]

plt.figure(figsize=(10, 5))
plt.bar(names, times)
plt.title(f"Performance comparison for calculating sum of integers up to {n}")
plt.ylabel("Time (seconds)")
plt.grid(axis='y', alpha=0.3)
plt.show()
```

### Available C Types in Cython

```cython
# c_types.pyx
# Integer types
cdef char a = 127  # 8-bit signed integer (-128 to 127)
cdef unsigned char b = 255  # 8-bit unsigned (0 to 255)
cdef short c = 32767  # 16-bit signed
cdef unsigned short d = 65535  # 16-bit unsigned
cdef int e = 2147483647  # 32-bit signed
cdef unsigned int f = 4294967295  # 32-bit unsigned
cdef long g = 9223372036854775807  # Usually 64-bit on 64-bit systems
cdef unsigned long h = 18446744073709551615  # Unsigned long
cdef long long i = 9223372036854775807  # 64-bit signed
cdef unsigned long long j = 18446744073709551615  # 64-bit unsigned

# Floating point types
cdef float k = 3.14  # 32-bit float
cdef double l = 3.141592653589793  # 64-bit double
cdef long double m = 3.141592653589793238462  # Extended precision

# Boolean type
cdef bint n = True  # C boolean (maps to int in C, but accepts/returns Python bool)

# Character string type (not Python string)
cdef char* o = b"hello"  # C string (null-terminated array of bytes)

# Complex numbers
cdef float complex p = 1.0 + 2.0j  # Complex float
cdef double complex q = 1.0 + 2.0j  # Complex double

# Size type
cdef size_t r = 1000  # Unsigned integer for sizes (from C's stddef.h)

# Function to demonstrate type checking and conversion
def demonstrate_types():
    print(f"char: {a}")
    print(f"unsigned char: {b}")
    print(f"short: {c}")
    print(f"unsigned short: {d}")
    print(f"int: {e}")
    print(f"unsigned int: {f}")
    print(f"long: {g}")
    print(f"unsigned long: {h}")
    print(f"long long: {i}")
    print(f"unsigned long long: {j}")
    print(f"float: {k}")
    print(f"double: {l}")
    print(f"long double: {m}")
    print(f"bint: {n}")
    # Need to decode bytes for proper printing
    print(f"char*: {o.decode('utf-8')}")
    print(f"float complex: {p}")
    print(f"double complex: {q}")
    print(f"size_t: {r}")
```

### Type Conversion Between Python and C

```cython
# type_conversion.pyx
from libc.stdlib cimport malloc, free
from cpython.ref cimport PyObject

# Python to C conversion
def py_to_c_conversion(obj):
    """Demonstrate conversions from Python objects to C types."""
    cdef:
        int i
        double d
        bint b
        char* s
        list py_list
        int* c_array
        int array_length, idx
    
    # Integer conversion
    i = obj if isinstance(obj, int) else 0
    print(f"Python object {obj} converted to C int: {i}")
    
    # Float conversion
    d = obj if isinstance(obj, (int, float)) else 0.0
    print(f"Python object {obj} converted to C double: {d}")
    
    # Boolean conversion
    b = bool(obj)
    print(f"Python object {obj} converted to C bint: {b}")
    
    # String conversion (safe way using bytes)
    if isinstance(obj, str):
        py_bytes = obj.encode('utf-8')
        # Here we're getting a temporary char* that's managed by Python
        s = py_bytes
        print(f"Python string '{obj}' converted to C char*: {s.decode('utf-8')}")
    
    # List to C array conversion
    if isinstance(obj, list) and all(isinstance(x, int) for x in obj):
        py_list = obj
        array_length = len(py_list)
        
        # Allocate C array
        c_array = <int*>malloc(array_length * sizeof(int))
        
        # Copy data
        for idx in range(array_length):
            c_array[idx] = py_list[idx]
        
        # Print the array
        print("Python list converted to C array:")
        for idx in range(array_length):
            print(f"  c_array[{idx}] = {c_array[idx]}")
        
        # Free memory
        free(c_array)

# C to Python conversion
def c_to_py_conversion():
    """Demonstrate conversions from C types to Python objects."""
    cdef:
        int i = 42
        double d = 3.14159
        bint b = 1
        char* s = b"Hello, world!"
        int c_array[5]
        list py_list
        int idx
    
    # Initialize C array
    for idx in range(5):
        c_array[idx] = idx * 10
    
    # Convert C int to Python int
    py_int = i
    print(f"C int {i} converted to Python int: {py_int}, type: {type(py_int)}")
    
    # Convert C double to Python float
    py_float = d
    print(f"C double {d} converted to Python float: {py_float}, type: {type(py_float)}")
    
    # Convert C bint to Python bool
    py_bool = b
    print(f"C bint {b} converted to Python bool: {py_bool}, type: {type(py_bool)}")
    
    # Convert C char* to Python str
    py_str = s.decode('utf-8')
    print(f"C char* '{s.decode('utf-8')}' converted to Python str: '{py_str}', type: {type(py_str)}")
    
    # Convert C array to Python list
    py_list = []
    for idx in range(5):
        py_list.append(c_array[idx])
    
    print(f"C array converted to Python list: {py_list}, type: {type(py_list)}")
```

### Performance Benchmark Comparing Static Typing

Let's benchmark a complete algorithm with different levels of typing:

```cython
# matrix_multiply.pyx
import numpy as np
cimport numpy as cnp

# Untyped Python version
def matrix_multiply_python(A, B):
    """Matrix multiplication in pure Python."""
    n = len(A)
    m = len(B[0])
    p = len(B)
    C = [[0 for _ in range(m)] for _ in range(n)]
    
    for i in range(n):
        for j in range(m):
            for k in range(p):
                C[i][j] += A[i][k] * B[k][j]
    
    return C

# Partially typed Cython version
def matrix_multiply_partial(A, B):
    """Matrix multiplication with partial typing."""
    cdef int n = len(A)
    cdef int m = len(B[0])
    cdef int p = len(B)
    cdef int i, j, k
    
    C = [[0 for _ in range(m)] for _ in range(n)]
    
    for i in range(n):
        for j in range(m):
            for k in range(p):
                C[i][j] += A[i][k] * B[k][j]
    
    return C

# Fully typed Cython version with memory views
def matrix_multiply_typed(double[:, :] A, double[:, :] B):
    """Matrix multiplication with full typing using memory views."""
    cdef int n = A.shape[0]
    cdef int p = A.shape[1]
    cdef int m = B.shape[1]
    
    # Check dimensions
    if p != B.shape[0]:
        raise ValueError("Incompatible matrix dimensions")
    
    # Create result matrix
    cdef double[:, :] C = np.zeros((n, m), dtype=np.double)
    
    cdef int i, j, k
    cdef double temp
    
    for i in range(n):
        for j in range(m):
            temp = 0.0
            for k in range(p):
                temp += A[i, k] * B[k, j]
            C[i, j] = temp
    
    return np.asarray(C)

# NumPy version for comparison
def matrix_multiply_numpy(A, B):
    """Matrix multiplication using NumPy."""
    return np.dot(A, B)
```

Benchmark script:

```python
# test_matrix_multiply.py
import time
import numpy as np
import matplotlib.pyplot as plt
from matrix_multiply import (
    matrix_multiply_python,
    matrix_multiply_partial,
    matrix_multiply_typed,
    matrix_multiply_numpy
)

def benchmark(func, *args, iterations=3):
    times = []
    for _ in range(iterations):
        start = time.time()
        result = func(*args)
        elapsed = time.time() - start
        times.append(elapsed)
    
    return min(times), result  # Return the best time

def test_matrices(size=100):
    # Create random matrices
    A_list = [[np.random.random() for _ in range(size)] for _ in range(size)]
    B_list = [[np.random.random() for _ in range(size)] for _ in range(size)]
    
    A_np = np.random.random((size, size))
    B_np = np.random.random((size, size))
    
    results = []
    
    # Python version
    print(f"Testing Python version ({size}x{size} matrices)...")
    if size <= 100:  # Skip for large matrices, too slow
        py_time, py_result = benchmark(matrix_multiply_python, A_list, B_list)
        results.append(("Python", py_time))
        print(f"  Time: {py_time:.6f} seconds")
    else:
        print("  Skipped (too slow for large matrices)")
        results.append(("Python", None))
    
    # Partially typed version
    print(f"Testing partially typed version ({size}x{size} matrices)...")
    if size <= 200:  # Skip for very large matrices
        partial_time, partial_result = benchmark(matrix_multiply_partial, A_list, B_list)
        results.append(("Partial Typing", partial_time))
        print(f"  Time: {partial_time:.6f} seconds")
    else:
        print("  Skipped (too slow for large matrices)")
        results.append(("Partial Typing", None))
    
    # Fully typed version
    print(f"Testing fully typed version ({size}x{size} matrices)...")
    typed_time, typed_result = benchmark(matrix_multiply_typed, A_np, B_np)
    results.append(("Full Typing", typed_time))
    print(f"  Time: {typed_time:.6f} seconds")
    
    # NumPy version
    print(f"Testing NumPy version ({size}x{size} matrices)...")
    numpy_time, numpy_result = benchmark(matrix_multiply_numpy, A_np, B_np)
    results.append(("NumPy", numpy_time))
    print(f"  Time: {numpy_time:.6f} seconds")
    
    # Verify results
    if size <= 100:
        numpy_py_result = np.array(py_result)
        print(f"Python result matches NumPy: {np.allclose(numpy_py_result, numpy_result)}")
    
    if size <= 200:
        numpy_partial_result = np.array(partial_result)
        print(f"Partial typing result matches NumPy: {np.allclose(numpy_partial_result, numpy_result)}")
    
    print(f"Full typing result matches NumPy: {np.allclose(typed_result, numpy_result)}")
    
    # Plot results
    names = [r[0] for r in results]
    times = [r[1] if r[1] is not None else 0 for r in results]
    
    plt.figure(figsize=(10, 6))
    bars = plt.bar(names, times)
    plt.title(f"Matrix Multiplication Performance ({size}x{size})")
    plt.ylabel("Time (seconds)")
    
    # Add value labels
    for bar in bars:
        height = bar.get_height()
        if height > 0:
            plt.text(bar.get_x() + bar.get_width()/2., height,
                    f'{height:.6f}',
                    ha='center', va='bottom', rotation=0)
    
    plt.grid(axis='y', alpha=0.3)
    plt.show()

# Test with different matrix sizes
test_matrices(50)  # Small matrices
test_matrices(200)  # Medium matrices
test_matrices(500)  # Large matrices (Python and partial typing will be skipped)
```

## 2. Memory Management

Understanding and controlling memory is crucial for Cython performance.

### Manual Memory Allocation

```cython
# memory_mgmt.pyx
from libc.stdlib cimport malloc, calloc, realloc, free
from libc.string cimport memcpy, memset
import numpy as np

def demonstrate_malloc():
    """
    Demonstrate manual memory allocation using malloc.
    
    malloc allocates uninitialized memory.
    """
    cdef:
        int* int_array
        int i, n = 10
    
    # Allocate memory for 10 integers
    int_array = <int*>malloc(n * sizeof(int))
    
    # Check for allocation failure
    if int_array == NULL:
        raise MemoryError("Failed to allocate memory")
    
    try:
        # Initialize the memory (malloc doesn't initialize it)
        for i in range(n):
            int_array[i] = i * 100
        
        # Print the values
        print("Values in malloc'd array:")
        for i in range(n):
            print(f"  int_array[{i}] = {int_array[i]}")
    
    finally:
        # Always free the memory
        free(int_array)

def demonstrate_calloc():
    """
    Demonstrate memory allocation using calloc.
    
    calloc allocates memory and initializes it to zero.
    """
    cdef:
        double* double_array
        int i, n = 10
    
    # Allocate and zero-initialize memory for 10 doubles
    double_array = <double*>calloc(n, sizeof(double))
    
    # Check for allocation failure
    if double_array == NULL:
        raise MemoryError("Failed to allocate memory")
    
    try:
        # Print the values (should all be 0.0)
        print("Values in calloc'd array (before assignment):")
        for i in range(n):
            print(f"  double_array[{i}] = {double_array[i]}")
        
        # Assign new values
        for i in range(n):
            double_array[i] = i * 3.14
        
        # Print the values again
        print("Values in calloc'd array (after assignment):")
        for i in range(n):
            print(f"  double_array[{i}] = {double_array[i]}")
    
    finally:
        # Always free the memory
        free(double_array)

def demonstrate_realloc():
    """
    Demonstrate memory reallocation using realloc.
    
    realloc changes the size of a previously allocated memory block.
    """
    cdef:
        int* int_array
        int i, old_size = 5, new_size = 10
    
    # Initial allocation
    int_array = <int*>malloc(old_size * sizeof(int))
    
    # Check for allocation failure
    if int_array == NULL:
        raise MemoryError("Failed to allocate memory")
    
    try:
        # Initialize the memory
        for i in range(old_size):
            int_array[i] = i * 100
        
        print("Values in original array:")
        for i in range(old_size):
            print(f"  int_array[{i}] = {int_array[i]}")
        
        # Resize the array
        int_array = <int*>realloc(int_array, new_size * sizeof(int))
        
        # Check for reallocation failure
        if int_array == NULL:
            raise MemoryError("Failed to reallocate memory")
        
        # Initialize the new elements
        for i in range(old_size, new_size):
            int_array[i] = i * 100
        
        print("Values in resized array:")
        for i in range(new_size):
            print(f"  int_array[{i}] = {int_array[i]}")
    
    finally:
        # Always free the memory
        free(int_array)

def demonstrate_memset_memcpy():
    """
    Demonstrate memory operations using memset and memcpy.
    
    memset: Fill memory with a specific byte value
    memcpy: Copy memory from one location to another
    """
    cdef:
        int* source_array
        int* dest_array
        int i, n = 10
    
    # Allocate source array
    source_array = <int*>malloc(n * sizeof(int))
    if source_array == NULL:
        raise MemoryError("Failed to allocate source array")
    
    # Allocate destination array
    dest_array = <int*>malloc(n * sizeof(int))
    if dest_array == NULL:
        free(source_array)  # Don't forget to free already allocated memory
        raise MemoryError("Failed to allocate destination array")
    
    try:
        # Initialize source array
        for i in range(n):
            source_array[i] = i * 100
        
        # Set all bytes in destination array to 0 using memset
        memset(dest_array, 0, n * sizeof(int))
        
        print("Destination array after memset:")
        for i in range(n):
            print(f"  dest_array[{i}] = {dest_array[i]}")
        
        # Copy contents from source to destination using memcpy
        memcpy(dest_array, source_array, n * sizeof(int))
        
        print("Destination array after memcpy:")
        for i in range(n):
            print(f"  dest_array[{i}] = {dest_array[i]}")
    
    finally:
        # Always free all allocated memory
        free(source_array)
        free(dest_array)

def create_numpy_from_c_array():
    """
    Create a NumPy array from a C array.
    
    This demonstrates bridging between C memory management and NumPy.
    """
    cdef:
        double* c_array
        int i, n = 1000
        double[:] view
    
    # Allocate memory
    c_array = <double*>malloc(n * sizeof(double))
    if c_array == NULL:
        raise MemoryError("Failed to allocate memory")
    
    try:
        # Initialize with some values
        for i in range(n):
            c_array[i] = i * 0.01
        
        # Create a memory view of the C array
        view = <double[:n]>c_array
        
        # Convert to a NumPy array (this makes a copy)
        numpy_array = np.array(view)
        
        print(f"Created NumPy array from C array:")
        print(f"  Shape: {numpy_array.shape}")
        print(f"  Data type: {numpy_array.dtype}")
        print(f"  First 5 elements: {numpy_array[:5]}")
        print(f"  Last 5 elements: {numpy_array[-5:]}")
        
        return numpy_array
    
    finally:
        # Free the original C array
        free(c_array)
```

### Memory Views

Memory views provide a safe and efficient way to work with memory in Cython:

```cython
# memoryviews.pyx
import numpy as np
cimport numpy as cnp

def process_1d_array(double[:] arr):
    """Process a 1D array using memory views."""
    cdef:
        int i, n = arr.shape[0]
        double[:] result = np.zeros(n, dtype=np.double)
    
    for i in range(n):
        result[i] = arr[i] * arr[i]
    
    return np.asarray(result)

def process_2d_array(double[:, :] arr):
    """Process a 2D array using memory views."""
    cdef:
        int i, j
        int rows = arr.shape[0]
        int cols = arr.shape[1]
        double[:, :] result = np.zeros((rows, cols), dtype=np.double)
    
    for i in range(rows):
        for j in range(cols):
            result[i, j] = arr[i, j] * 2.0
    
    return np.asarray(result)

def process_3d_array(double[:, :, :] arr):
    """Process a 3D array using memory views."""
    cdef:
        int i, j, k
        int dim1 = arr.shape[0]
        int dim2 = arr.shape[1]
        int dim3 = arr.shape[2]
        double[:, :, :] result = np.zeros((dim1, dim2, dim3), dtype=np.double)
    
    for i in range(dim1):
        for j in range(dim2):
            for k in range(dim3):
                result[i, j, k] = arr[i, j, k] + i + j + k
    
    return np.asarray(result)

def use_strided_view(double[:, ::1] arr):
    """
    Process a 2D array using a contiguous memory view for columns.
    
    The ::1 indicates that this dimension is contiguous in memory.
    """
    cdef:
        int i, j
        int rows = arr.shape[0]
        int cols = arr.shape[1]
        double total = 0.0
    
    for j in range(cols):
        for i in range(rows):
            total += arr[i, j]
    
    return total

def use_sliced_view(double[:] arr, int start, int end):
    """Work with a slice of a memory view."""
    cdef:
        int i
        double total = 0.0
        double[:] sliced = arr[start:end]
    
    # Process the slice
    for i in range(sliced.shape[0]):
        total += sliced[i]
    
    return total

def readonly_view_example(double[:] arr not None):
    """
    Demonstrate a readonly memory view.
    
    Note: By default, memory views are read-write.
    """
    cdef:
        int i, n = arr.shape[0]
        double total = 0.0
    
    # Read from the array
    for i in range(n):
        total += arr[i]
    
    # The following would cause a compile-time error if using a readonly view
    # arr[0] = 42.0  
    
    return total

def direct_pointer_access(double[:] arr):
    """
    Use direct pointer access for maximum speed.
    
    This is more dangerous but can be faster than accessing through memory views.
    """
    cdef:
        int i, n = arr.shape[0]
        double* data = &arr[0]  # Get a pointer to the first element
        double total = 0.0
    
    # Use pointer arithmetic for faster access
    for i in range(n):
        total += data[i]
    
    return total
```

### Memory Management Test and Benchmark

```python
# test_memory.py
import numpy as np
import time
import matplotlib.pyplot as plt
from memory_mgmt import (
    demonstrate_malloc, demonstrate_calloc,
    demonstrate_realloc, demonstrate_memset_memcpy,
    create_numpy_from_c_array
)
from memoryviews import (
    process_1d_array, process_2d_array, process_3d_array,
    use_strided_view, use_sliced_view, readonly_view_example,
    direct_pointer_access
)

def test_memory_management():
    print("\n=== Testing Basic Memory Management ===")
    demonstrate_malloc()
    print()
    demonstrate_calloc()
    print()
    demonstrate_realloc()
    print()
    demonstrate_memset_memcpy()
    print()
    numpy_array = create_numpy_from_c_array()
    print()

def test_memory_views():
    print("\n=== Testing Memory Views ===")
    
    # Create test arrays
    array_1d = np.random.random(1000)
    array_2d = np.random.random((100, 100))
    array_3d = np.random.random((10, 10, 10))
    
    # Test 1D array processing
    result_1d = process_1d_array(array_1d)
    print(f"1D array processing: input[0:5] = {array_1d[0:5]}")
    print(f"                      result[0:5] = {result_1d[0:5]}")
    print()
    
    # Test 2D array processing
    result_2d = process_2d_array(array_2d)
    print(f"2D array processing: input[0,0:5] = {array_2d[0,0:5]}")
    print(f"                      result[0,0:5] = {result_2d[0,0:5]}")
    print()
    
    # Test 3D array processing
    result_3d = process_3d_array(array_3d)
    print(f"3D array shape: {array_3d.shape} -> {result_3d.shape}")
    print()
    
    # Test strided view
    total_strided = use_strided_view(array_2d)
    print(f"Strided view total: {total_strided}")
    print(f"NumPy sum for comparison: {np.sum(array_2d)}")
    print()
    
    # Test sliced view
    start, end = 200, 700
    total_sliced = use_sliced_view(array_1d, start, end)
    print(f"Sliced view total [{start}:{end}]: {total_sliced}")
    print(f"NumPy slice sum for comparison: {np.sum(array_1d[start:end])}")
    print()
    
    # Test readonly view
    total_readonly = readonly_view_example(array_1d)
    print(f"Readonly view total: {total_readonly}")
    print()
    
    # Test direct pointer access
    total_pointer = direct_pointer_access(array_1d)
    print(f"Direct pointer access total: {total_pointer}")
    print(f"NumPy sum for comparison: {np.sum(array_1d)}")
    print()

def benchmark_memory_view_vs_numpy():
    print("\n=== Benchmarking Memory Views vs. NumPy ===")
    
    sizes = [100, 1000, 10000, 100000, 1000000]
    cython_times = []
    numpy_times = []
    pointer_times = []
    
    for size in sizes:
        # Create a large 1D array
        array_1d = np.random.random(size)
        
        # Benchmark Cython memory view
        start = time.time()
        _ = process_1d_array(array_1d)
        cython_time = time.time() - start
        cython_times.append(cython_time)
        
        # Benchmark NumPy
        start = time.time()
        _ = array_1d * array_1d  # Element-wise multiplication
        numpy_time = time.time() - start
        numpy_times.append(numpy_time)
        
        # Benchmark direct pointer access
        start = time.time()
        _ = direct_pointer_access(array_1d)
        pointer_time = time.time() - start
        pointer_times.append(pointer_time)
        
        print(f"Size: {size:,}")
        print(f"  Cython memory view: {cython_time:.6f} seconds")
        print(f"  NumPy vectorized:   {numpy_time:.6f} seconds")
        print(f"  Direct pointer:     {pointer_time:.6f} seconds")
        print()
    
    # Plot results
    plt.figure(figsize=(12, 6))
    plt.plot(sizes, cython_times, 'o-', label='Cython Memory View')
    plt.plot(sizes, numpy_times, 's-', label='NumPy Vectorized')
    plt.plot(sizes, pointer_times, '^-', label='Direct Pointer')
    
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Array Size')
    plt.ylabel('Time (seconds)')
    plt.title('Performance Comparison: Memory Views vs. NumPy vs. Direct Pointer')
    plt.legend()
    plt.grid(True, which="both", ls="--", alpha=0.3)
    plt.show()

if __name__ == "__main__":
    test_memory_management()
    test_memory_views()
    benchmark_memory_view_vs_numpy()
```

## 3. Function Optimization

Cython offers multiple function types with different performance characteristics and use cases.

### Function Types in Cython

```cython
# function_types.pyx
cimport cython
from libc.math cimport sqrt, sin, cos, exp
import time

# Python function (accessible from Python)
def py_function(x, y):
    """
    Regular Python function with Python objects.
    
    This is callable from Python and can accept any Python objects.
    """
    return x * y

# Typed Python function (accessible from Python)
def typed_function(double x, double y):
    """
    Python function with typed arguments.
    
    This is callable from Python but expects specific types.
    """
    return x * y

# C function (not accessible from Python)
cdef double c_function(double x, double y) nogil:
    """
    Pure C function.
    
    This is not callable from Python but can be called from other Cython code.
    The nogil declaration means it can run without the Python GIL.
    """
    return x * y

# Hybrid function (accessible from both Python and C)
cpdef double hybrid_function(double x, double y) nogil:
    """
    Hybrid Python/C function.
    
    This generates both a C function and a Python wrapper, making it
    callable from both Python and other Cython code.
    """
    return x * y

# Python function that calls C function
def call_c_function(double x, double y):
    """
    Python-callable function that internally uses a C function.
    
    This demonstrates how to use C functions within Python-callable functions.
    """
    return c_function(x, y)

# Complex mathematical function in pure Python
def py_complex_math(double x, double y, int iterations=1000):
    """Complex mathematical function in pure Python."""
    result = 0.0
    for i in range(iterations):
        result += (x**2 * sin(y) + y**2 * cos(x)) / (i + 1)
    return result

# Complex mathematical function in C
cdef double c_complex_math(double x, double y, int iterations=1000) nogil:
    """Complex mathematical function in C."""
    cdef:
        double result = 0.0
        int i
    
    for i in range(iterations):
        result += (x*x * sin(y) + y*y * cos(x)) / (i + 1.0)
    return result

# Python wrapper for the C function
def call_c_complex_math(double x, double y, int iterations=1000):
    """Python-callable wrapper for the C complex math function."""
    return c_complex_math(x, y, iterations)

# Benchmark functions
def benchmark_functions():
    """Benchmark different function types."""
    cdef:
        double x = 1.5
        double y = 2.5
        int iterations = 10000000
        int i
        double result
        double start_time
    
    # Benchmark Python function
    start_time = time.time()
    for i in range(iterations):
        result = py_function(x, y)
    py_time = time.time() - start_time
    
    # Benchmark typed Python function
    start_time = time.time()
    for i in range(iterations):
        result = typed_function(x, y)
    typed_time = time.time() - start_time
    
    # Benchmark C function
    start_time = time.time()
    with nogil:
        for i in range(iterations):
            result = c_function(x, y)
    c_time = time.time() - start_time
    
    # Benchmark hybrid function
    start_time = time.time()
    with nogil:
        for i in range(iterations):
            result = hybrid_function(x, y)
    hybrid_time = time.time() - start_time
    
    # Benchmark Python function that calls C function
    start_time = time.time()
    for i in range(iterations):
        result = call_c_function(x, y)
    call_c_time = time.time() - start_time
    
    return {
        "Python function": py_time,
        "Typed Python function": typed_time,
        "C function": c_time,
        "Hybrid function": hybrid_time,
        "Python calling C function": call_c_time
    }

# Benchmark complex math functions
def benchmark_complex_math():
    """Benchmark complex mathematical functions."""
    cdef:
        double x = 1.5
        double y = 2.5
        int iterations = 1000  # Number of terms in the math series
        int repeats = 10000    # Number of times to repeat the function call
        int i
        double result
        double start_time
    
    # Benchmark Python implementation
    start_time = time.time()
    for i in range(repeats):
        result = py_complex_math(x, y, iterations)
    py_time = time.time() - start_time
    
    # Benchmark C implementation
    start_time = time.time()
    for i in range(repeats):
        result = call_c_complex_math(x, y, iterations)
    c_time = time.time() - start_time
    
    return {
        "Python complex math": py_time,
        "C complex math": c_time
    }
```

### Using External C Functions

```cython
# external_c.pyx
from libc.stdlib cimport rand, srand, RAND_MAX
from libc.math cimport sin, cos, tan, exp, log, sqrt, fabs
from libc.time cimport time as c_time

# Initialize random seed
def initialize_random_seed():
    """Initialize the C random number generator with the current time."""
    srand(c_time(NULL))

# Get a random number between 0 and 1
def random_uniform():
    """
    Get a uniform random number between 0 and 1 using C's rand() function.
    
    This is not for cryptographic purposes, just for demonstration.
    """
    return rand() / float(RAND_MAX)

# Demonstrate C math functions
def c_math_demo(double x):
    """Demonstrate various C math functions from libc.math."""
    results = {
        "sin": sin(x),
        "cos": cos(x),
        "tan": tan(x),
        "exp": exp(x),
        "log": log(x) if x > 0 else float('nan'),
        "sqrt": sqrt(x) if x >= 0 else float('nan'),
        "fabs": fabs(x)
    }
    return results

# Use external C functions for vector operations
def vector_magnitude(double x, double y, double z):
    """Calculate the magnitude of a 3D vector using C's sqrt function."""
    return sqrt(x*x + y*y + z*z)

def vector_normalize(double x, double y, double z):
    """Normalize a 3D vector using C math functions."""
    cdef:
        double mag = vector_magnitude(x, y, z)
        double nx, ny, nz
    
    if mag > 0:
        nx = x / mag
        ny = y / mag
        nz = z / mag
        return (nx, ny, nz)
    else:
        return (0.0, 0.0, 0.0)

# Monte Carlo estimation of pi using C's random generator
def monte_carlo_pi(int n):
    """
    Estimate π using Monte Carlo method with C's random number generator.
    
    This works by randomly placing points in a square and counting
    how many fall inside the inscribed circle.
    """
    cdef:
        int i, count = 0
        double x, y, distance_squared
    
    for i in range(n):
        # Generate random point in unit square
        x = random_uniform()
        y = random_uniform()
        
        # Calculate distance from origin
        distance_squared = x*x + y*y
        
        # Check if point is inside circle
        if distance_squared <= 1.0:
            count += 1
    
    # ratio of points in circle to total points ≈ π/4
    return 4.0 * count / n
```

### Function Optimization Test Suite

```python
# test_functions.py
import time
import numpy as np
import matplotlib.pyplot as plt
from function_types import benchmark_functions, benchmark_complex_math
from external_c import (
    initialize_random_seed, random_uniform, c_math_demo,
    vector_magnitude, vector_normalize, monte_carlo_pi
)

def test_function_types():
    print("\n=== Benchmarking Function Types ===")
    results = benchmark_functions()
    
    # Print results
    for name, time_taken in results.items():
        print(f"{name}: {time_taken:.6f} seconds")
    
    # Plot results
    plt.figure(figsize=(10, 6))
    plt.bar(results.keys(), results.values())
    plt.title("Function Type Performance Comparison")
    plt.ylabel("Time (seconds)")
    plt.xticks(rotation=45, ha='right')
    plt.tight_layout()
    plt.grid(axis='y', alpha=0.3)
    plt.show()
    
    # Benchmark complex math
    print("\n=== Benchmarking Complex Math Functions ===")
    complex_results = benchmark_complex_math()
    
    for name, time_taken in complex_results.items():
        print(f"{name}: {time_taken:.6f} seconds")
    
    # Calculate speedup
    if "Python complex math" in complex_results and "C complex math" in complex_results:
        speedup = complex_results["Python complex math"] / complex_results["C complex math"]
        print(f"C implementation is {speedup:.2f}x faster")
    
    # Plot complex math results
    plt.figure(figsize=(8, 6))
    plt.bar(complex_results.keys(), complex_results.values())
    plt.title("Complex Math Performance Comparison")
    plt.ylabel("Time (seconds)")
    plt.grid(axis='y', alpha=0.3)
    plt.show()

def test_external_c_functions():
    print("\n=== Testing External C Functions ===")
    
    # Initialize random seed
    initialize_random_seed()
    
    # Test random uniform
    randoms = [random_uniform() for _ in range(10)]
    print(f"10 random numbers: {randoms}")
    
    # Test C math functions
    x = 0.5
    math_results = c_math_demo(x)
    print(f"Math functions for x = {x}:")
    for func, result in math_results.items():
        print(f"  {func}({x}) = {result}")
    
    # Test vector functions
    x, y, z = 3.0, 4.0, 5.0
    mag = vector_magnitude(x, y, z)
    nx, ny, nz = vector_normalize(x, y, z)
    print(f"Vector ({x}, {y}, {z}):")
    print(f"  Magnitude: {mag}")
    print(f"  Normalized: ({nx}, {ny}, {nz})")
    print(f"  Verification: magnitude of normalized vector = {vector_magnitude(nx, ny, nz)}")
    
    # Test Monte Carlo pi estimation
    iterations = [1000, 10000, 100000, 1000000]
    pi_estimates = []
    times = []
    
    for n in iterations:
        start = time.time()
        pi_est = monte_carlo_pi(n)
        elapsed = time.time() - start
        
        pi_estimates.append(pi_est)
        times.append(elapsed)
        
        error = 100 * abs(pi_est - np.pi) / np.pi
        print(f"Monte Carlo π with n={n:,}: {pi_est:.6f} (error: {error:.4f}%, time: {elapsed:.6f}s)")
    
    # Plot Monte Carlo convergence
    plt.figure(figsize=(10, 6))
    plt.semilogx(iterations, pi_estimates, 'o-', label='Estimated π')
    plt.axhline(y=np.pi, color='r', linestyle='-', label='Actual π')
    plt.xlabel('Number of Points')
    plt.ylabel('Estimated π')
    plt.title('Monte Carlo π Estimation Convergence')
    plt.grid(True, which="both", ls="--", alpha=0.3)
    plt.legend()
    plt.show()
    
    # Plot time complexity
    plt.figure(figsize=(10, 6))
    plt.loglog(iterations, times, 'o-')
    plt.xlabel('Number of Points')
    plt.ylabel('Time (seconds)')
    plt.title('Monte Carlo π Estimation Time Complexity')
    plt.grid(True, which="both", ls="--", alpha=0.3)
    plt.show()

def optimize_loop_example():
    """
    Demonstrate loop optimization techniques.
    """
    print("\n=== Loop Optimization Techniques ===")
    
    # Create a large array
    n = 10000000  # 10 million elements
    data = np.random.random(n)
    
    # Technique 1: Basic loop in Python
    def python_loop(arr):
        total = 0.0
        for x in arr:
            total += x
        return total
    
    # Technique 2: Basic loop in Cython with typed variables
    from function_types import typed_loop_sum
    
    # Technique 3: Loop with GIL released
    from function_types import nogil_loop_sum
    
    # Technique 4: NumPy sum
    def numpy_sum(arr):
        return np.sum(arr)
    
    # Benchmark all techniques
    techniques = [
        ("Python Loop", python_loop),
        ("Typed Cython Loop", typed_loop_sum),
        ("NoGIL Cython Loop", nogil_loop_sum),
        ("NumPy Sum", numpy_sum)
    ]
    
    results = {}
    for name, func in techniques:
        start = time.time()
        result = func(data)
        elapsed = time.time() - start
        results[name] = elapsed
        print(f"{name}: {elapsed:.6f} seconds, sum = {result:.6f}")
    
    # Plot results
    plt.figure(figsize=(10, 6))
    plt.bar(results.keys(), results.values())
    plt.title("Loop Optimization Techniques Comparison")
    plt.ylabel("Time (seconds)")
    plt.xticks(rotation=45, ha='right')
    plt.tight_layout()
    plt.grid(axis='y', alpha=0.3)
    plt.show()

if __name__ == "__main__":
    test_function_types()
    test_external_c_functions()
    optimize_loop_example()
```

Add this function to `function_types.pyx` for the loop optimization example:

```cython
# Add to function_types.pyx
def typed_loop_sum(double[:] arr):
    """Sum array elements using a typed loop."""
    cdef:
        int i, n = arr.shape[0]
        double total = 0.0
    
    for i in range(n):
        total += arr[i]
    
    return total

def nogil_loop_sum(double[:] arr):
    """Sum array elements using a loop with GIL released."""
    cdef:
        int i, n = arr.shape[0]
        double total = 0.0
    
    with nogil:
        for i in range(n):
            total += arr[i]
    
    return total
```

## Combining It All: Real-World Example

Let's see how to combine static typing, memory management, and function optimization in a real-world example - a fast image processing library:

```cython
# fast_image.pyx
import numpy as np
cimport numpy as cnp
from libc.stdlib cimport malloc, free
from libc.math cimport sqrt, exp
from cython.parallel import prange

# Initialize NumPy C API
cnp.import_array()

# Define image filter types
cpdef enum FilterType:
    BLUR = 0
    SHARPEN = 1
    EDGE_DETECT = 2
    EMBOSS = 3

# Kernel structure
cdef struct Kernel:
    double* data
    int size  # Kernel is assumed to be square (size x size)

# Create Gaussian blur kernel
cdef Kernel create_gaussian_kernel(int size, double sigma) nogil:
    """Create a Gaussian blur kernel."""
    cdef:
        int i, j, center = size // 2
        double x, y, sum_val = 0.0
        Kernel kernel
    
    # Allocate memory for kernel
    kernel.data = <double*>malloc(size * size * sizeof(double))
    kernel.size = size
    
    # Compute Gaussian kernel values
    for i in range(size):
        for j in range(size):
            x = i - center
            y = j - center
            kernel.data[i * size + j] = exp(-(x*x + y*y) / (2 * sigma * sigma))
            sum_val += kernel.data[i * size + j]
    
    # Normalize kernel
    for i in range(size * size):
        kernel.data[i] /= sum_val
    
    return kernel

# Create edge detection kernel
cdef Kernel create_edge_kernel() nogil:
    """Create an edge detection kernel (Sobel)."""
    cdef:
        Kernel kernel
        int size = 3
    
    kernel.data = <double*>malloc(size * size * sizeof(double))
    kernel.size = size
    
    # Sobel operator
    kernel.data[0] = -1; kernel.data[1] = 0; kernel.data[2] = 1
    kernel.data[3] = -2; kernel.data[4] = 0; kernel.data[5] = 2
    kernel.data[6] = -1; kernel.data[7] = 0; kernel.data[8] = 1
    
    return kernel

# Create sharpen kernel
cdef Kernel create_sharpen_kernel() nogil:
    """Create a sharpening kernel."""
    cdef:
        Kernel kernel
        int size = 3
    
    kernel.data = <double*>malloc(size * size * sizeof(double))
    kernel.size = size
    
    # Sharpening kernel
    kernel.data[0] = 0;  kernel.data[1] = -1; kernel.data[2] = 0
    kernel.data[3] = -1; kernel.data[4] = 5;  kernel.data[5] = -1
    kernel.data[6] = 0;  kernel.data[7] = -1; kernel.data[8] = 0
    
    return kernel

# Create emboss kernel
cdef Kernel create_emboss_kernel() nogil:
    """Create an embossing kernel."""
    cdef:
        Kernel kernel
        int size = 3
    
    kernel.data = <double*>malloc(size * size * sizeof(double))
    kernel.size = size
    
    # Emboss kernel
    kernel.data[0] = -2; kernel.data[1] = -1; kernel.data[2] = 0
    kernel.data[3] = -1; kernel.data[4] = 1;  kernel.data[5] = 1
    kernel.data[6] = 0;  kernel.data[7] = 1;  kernel.data[8] = 2
    
    return kernel

# Free kernel memory
cdef void free_kernel(Kernel kernel) nogil:
    """Free kernel memory."""
    free(kernel.data)

# Apply kernel to image
cdef void apply_kernel_to_image(unsigned char[:, :] input_image, 
                              unsigned char[:, :] output_image,
                              Kernel kernel) nogil:
    """Apply a kernel to an image (grayscale)."""
    cdef:
        int rows = input_image.shape[0]
        int cols = input_image.shape[1]
        int kernel_size = kernel.size
        int kernel_radius = kernel_size // 2
        int i, j, ki, kj, i_offset, j_offset
        double sum_val, pixel_val
    
    # Apply kernel to each pixel
    for i in range(kernel_radius, rows - kernel_radius):
        for j in range(kernel_radius, cols - kernel_radius):
            sum_val = 0.0
            
            # Apply kernel
            for ki in range(kernel_size):
                i_offset = i + ki - kernel_radius
                for kj in range(kernel_size):
                    j_offset = j + kj - kernel_radius
                    sum_val += input_image[i_offset, j_offset] * kernel.data[ki * kernel_size + kj]
            
            # Clamp value to 0-255 range
            if sum_val < 0:
                pixel_val = 0
            elif sum_val > 255:
                pixel_val = 255
            else:
                pixel_val = sum_val
            
            output_image[i, j] = <unsigned char>pixel_val

# Apply filter to grayscale image
def apply_filter(unsigned char[:, :] image, FilterType filter_type):
    """
    Apply a filter to a grayscale image.
    
    Parameters:
        image: Input grayscale image (2D array of unsigned char)
        filter_type: Type of filter to apply (enum value)
    
    Returns:
        Filtered image as NumPy array
    """
    cdef:
        int rows = image.shape[0]
        int cols = image.shape[1]
        unsigned char[:, :] output = np.zeros((rows, cols), dtype=np.uint8)
        Kernel kernel
    
    # Create appropriate kernel based on filter type
    if filter_type == FilterType.BLUR:
        kernel = create_gaussian_kernel(5, 1.0)
    elif filter_type == FilterType.SHARPEN:
        kernel = create_sharpen_kernel()
    elif filter_type == FilterType.EDGE_DETECT:
        kernel = create_edge_kernel()
    elif filter_type == FilterType.EMBOSS:
        kernel = create_emboss_kernel()
    else:
        raise ValueError("Unknown filter type")
    
    # Apply kernel to image
    apply_kernel_to_image(image, output, kernel)
    
    # Free kernel memory
    free_kernel(kernel)
    
    # Return as NumPy array
    return np.asarray(output)

# Apply filter to color image (RGB)
def apply_filter_rgb(unsigned char[:, :, :] image, FilterType filter_type):
    """
    Apply a filter to an RGB color image.
    
    Parameters:
        image: Input RGB image (3D array of unsigned char)
        filter_type: Type of filter to apply (enum value)
    
    Returns:
        Filtered image as NumPy array
    """
    cdef:
        int rows = image.shape[0]
        int cols = image.shape[1]
        int channels = image.shape[2]
        unsigned char[:, :, :] output = np.zeros((rows, cols, channels), dtype=np.uint8)
        Kernel kernel
        int i, c
    
    # Create appropriate kernel based on filter type
    if filter_type == FilterType.BLUR:
        kernel = create_gaussian_kernel(5, 1.0)
    elif filter_type == FilterType.SHARPEN:
        kernel = create_sharpen_kernel()
    elif filter_type == FilterType.EDGE_DETECT:
        kernel = create_edge_kernel()
    elif filter_type == FilterType.EMBOSS:
        kernel = create_emboss_kernel()
    else:
        raise ValueError("Unknown filter type")
    
    # Apply kernel to each channel
    for c in range(channels):
        # Create views for input and output channels
        cdef:
            unsigned char[:, :] channel_input = image[:, :, c]
            unsigned char[:, :] channel_output = output[:, :, c]
        
        # Apply kernel to channel
        apply_kernel_to_image(channel_input, channel_output, kernel)
    
    # Free kernel memory
    free_kernel(kernel)
    
    # Return as NumPy array
    return np.asarray(output)

# Parallel implementation for color images
def apply_filter_rgb_parallel(unsigned char[:, :, :] image, FilterType filter_type):
    """
    Apply a filter to an RGB color image using parallel processing.
    
    Parameters:
        image: Input RGB image (3D array of unsigned char)
        filter_type: Type of filter to apply (enum value)
    
    Returns:
        Filtered image as NumPy array
    """
    cdef:
        int rows = image.shape[0]
        int cols = image.shape[1]
        int channels = image.shape[2]
        unsigned char[:, :, :] output = np.zeros((rows, cols, channels), dtype=np.uint8)
        int kernel_size, kernel_radius
        int i, j, c, ki, kj, i_offset, j_offset
        double sum_val, pixel_val
    
    # Determine kernel and parameters
    if filter_type == FilterType.BLUR:
        kernel_size = 5
    else:
        kernel_size = 3
    
    kernel_radius = kernel_size // 2
    
    # Use appropriate kernel based on filter type
    cdef:
        Kernel kernel
    
    if filter_type == FilterType.BLUR:
        kernel = create_gaussian_kernel(kernel_size, 1.0)
    elif filter_type == FilterType.SHARPEN:
        kernel = create_sharpen_kernel()
    elif filter_type == FilterType.EDGE_DETECT:
        kernel = create_edge_kernel()
    elif filter_type == FilterType.EMBOSS:
        kernel = create_emboss_kernel()
    else:
        raise ValueError("Unknown filter type")
    
    # Parallel application of kernel to all channels and pixels
    with nogil:
        for c in prange(channels, schedule='static'):
            for i in range(kernel_radius, rows - kernel_radius):
                for j in range(kernel_radius, cols - kernel_radius):
                    sum_val = 0.0
                    
                    # Apply kernel
                    for ki in range(kernel_size):
                        i_offset = i + ki - kernel_radius
                        for kj in range(kernel_size):
                            j_offset = j + kj - kernel_radius
                            sum_val += image[i_offset, j_offset, c] * kernel.data[ki * kernel_size + kj]
                    
                    # Clamp value to 0-255 range
                    if sum_val < 0:
                        pixel_val = 0
                    elif sum_val > 255:
                        pixel_val = 255
                    else:
                        pixel_val = sum_val
                    
                    output[i, j, c] = <unsigned char>pixel_val
    
    # Free kernel memory
    free_kernel(kernel)
    
    # Return as NumPy array
    return np.asarray(output)
```

Testing the image processing library:

```python
# test_image_processing.py
import time
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image
from fast_image import apply_filter, apply_filter_rgb, apply_filter_rgb_parallel, FilterType

def load_test_image(path, grayscale=False):
    """Load a test image."""
    if grayscale:
        return np.array(Image.open(path).convert('L'), dtype=np.uint8)
    else:
        return np.array(Image.open(path), dtype=np.uint8)

def benchmark_filters(image_path):
    """Benchmark different filters and implementations."""
    # Load color image
    print("\n=== Benchmarking Image Filters ===")
    print(f"Loading image: {image_path}")
    color_image = load_test_image(image_path)
    
    # Prepare filter types and labels
    filter_types = [
        (FilterType.BLUR, "Gaussian Blur"),
        (FilterType.SHARPEN, "Sharpen"),
        (FilterType.EDGE_DETECT, "Edge Detection"),
        (FilterType.EMBOSS, "Emboss")
    ]
    
    # Compare sequential vs parallel implementations
    for filter_type, filter_name in filter_types:
        print(f"\nApplying {filter_name} filter:")
        
        # Sequential implementation
        start_time = time.time()
        sequential_result = apply_filter_rgb(color_image, filter_type)
        sequential_time = time.time() - start_time
        print(f"  Sequential: {sequential_time:.4f} seconds")
        
        # Parallel implementation
        start_time = time.time()
        parallel_result = apply_filter_rgb_parallel(color_image, filter_type)
        parallel_time = time.time() - start_time
        print(f"  Parallel:   {parallel_time:.4f} seconds")
        
        # Calculate speedup
        speedup = sequential_time / parallel_time
        print(f"  Speedup:    {speedup:.2f}x")
        
        # Check if results match
        match = np.array_equal(sequential_result, parallel_result)
        print(f"  Results match: {match}")
        
        # Display the results
        plt.figure(figsize=(15, 5))
        
        plt.subplot(1, 3, 1)
        plt.imshow(color_image)
        plt.title('Original Image')
        plt.axis('off')
        
        plt.subplot(1, 3, 2)
        plt.imshow(sequential_result)
        plt.title(f'Sequential {filter_name}\n({sequential_time:.4f}s)')
        plt.axis('off')
        
        plt.subplot(1, 3, 3)
        plt.imshow(parallel_result)
        plt.title(f'Parallel {filter_name}\n({parallel_time:.4f}s, {speedup:.2f}x faster)')
        plt.axis('off')
        
        plt.tight_layout()
        plt.show()

if __name__ == "__main__":
    # You'll need to provide a path to a test image
    image_path = "test_image.jpg"  
    benchmark_filters(image_path)
```

This comprehensive example demonstrates:
1. Static typing with C types (`unsigned char`, `double`)
2. Memory management with `malloc` and `free`
3. Function types (`cdef`, `cpdef`, and regular Python functions)
4. Parallel processing with `prange` and `nogil`
5. Memory views for efficient array access
6. Algorithm optimization with C-level operations

The resulting image processing library provides significant performance benefits over equivalent Python implementations.


---
---

# Phase 3: Advanced Features in Cython

Let's explore the advanced features of Cython using clear, concise examples with both Pure Python mode and traditional Cython mode.

## 1. Object-Oriented Programming

### Extension Types (cdef classes)

Extension types allow you to create Python classes with C-level attributes and methods, combining Python's flexibility with C's performance.

#### Example: 3D Point Class

**Traditional Cython Mode (`point.pyx`):**

```cython
# point.pyx
cdef class Point3D:
    # C attributes (private by default)
    cdef double _x, _y, _z
    
    def __init__(self, double x=0.0, double y=0.0, double z=0.0):
        self._x = x
        self._y = y
        self._z = z
    
    # Python properties to access C attributes
    @property
    def x(self):
        return self._x
    
    @x.setter
    def x(self, double value):
        self._x = value
    
    @property
    def y(self):
        return self._y
    
    @y.setter
    def y(self, double value):
        self._y = value
    
    @property
    def z(self):
        return self._z
    
    @z.setter
    def z(self, double value):
        self._z = value
    
    # Regular Python method
    def __repr__(self):
        return f"Point3D({self._x}, {self._y}, {self._z})"
    
    # C method (fast but not callable from Python)
    cdef double _distance_squared(self, Point3D other):
        return (self._x - other._x) ** 2 + (self._y - other._y) ** 2 + (self._z - other._z) ** 2
    
    # Public method that uses the C method
    def distance_to(self, Point3D other):
        """Calculate Euclidean distance to another point."""
        from libc.math cimport sqrt
        return sqrt(self._distance_squared(other))
    
    # hybrid method (callable from both Python and Cython)
    cpdef Point3D add(self, Point3D other):
        """Add another point to this one."""
        return Point3D(self._x + other._x, self._y + other._y, self._z + other._z)
```

**Pure Python Mode (`point_pure.py`):**

```python
# point_pure.py
# cython: language_level=3
import cython

@cython.cclass
class Point3D:
    # C attributes with types
    _x: cython.double
    _y: cython.double
    _z: cython.double
    
    def __init__(self, x=0.0, y=0.0, z=0.0):
        self._x = x
        self._y = y
        self._z = z
    
    # Python properties to access C attributes
    @property
    def x(self):
        return self._x
    
    @x.setter
    def x(self, value):
        self._x = value
    
    @property
    def y(self):
        return self._y
    
    @y.setter
    def y(self, value):
        self._y = value
    
    @property
    def z(self):
        return self._z
    
    @z.setter
    def z(self, value):
        self._z = value
    
    # Regular Python method
    def __repr__(self):
        return f"Point3D({self._x}, {self._y}, {self._z})"
    
    # C method (not callable from Python)
    @cython.cfunc
    @cython.returns(cython.double)
    def _distance_squared(self, other: "Point3D") -> cython.double:
        return (self._x - other._x) ** 2 + (self._y - other._y) ** 2 + (self._z - other._z) ** 2
    
    # Public method that uses the C method
    def distance_to(self, other: "Point3D") -> float:
        """Calculate Euclidean distance to another point."""
        import math
        return math.sqrt(self._distance_squared(other))
    
    # Hybrid method
    @cython.ccall
    def add(self, other: "Point3D") -> "Point3D":
        """Add another point to this one."""
        return Point3D(self._x + other._x, self._y + other._y, self._z + other._z)
```

**Usage:**

```python
# test_point.py
# For Cython mode
from point import Point3D as CythonPoint
# For Pure Python mode
from point_pure import Point3D as PythonPoint
import time

def benchmark_points(PointClass, name, iterations=1000000):
    # Create points
    p1 = PointClass(1.0, 2.0, 3.0)
    p2 = PointClass(4.0, 5.0, 6.0)
    
    print(f"Testing {name} implementation:")
    print(f"p1 = {p1}")
    print(f"p2 = {p2}")
    
    # Distance calculation
    start = time.time()
    for _ in range(iterations):
        dist = p1.distance_to(p2)
    elapsed = time.time() - start
    print(f"Distance: {dist:.4f}")
    print(f"Time for {iterations} distance calculations: {elapsed:.4f}s")
    
    # Addition
    start = time.time()
    for _ in range(iterations):
        result = p1.add(p2)
    elapsed = time.time() - start
    print(f"Addition result: {result}")
    print(f"Time for {iterations} additions: {elapsed:.4f}s")
    print()

# Run benchmarks
benchmark_points(CythonPoint, "Cython")
benchmark_points(PythonPoint, "Pure Python")
```

### Inheritance and Special Methods

**Traditional Cython Mode (`shapes.pyx`):**

```cython
# shapes.pyx
from libc.math cimport sin, cos, M_PI

# Base shape class
cdef class Shape:
    cdef:
        double _x, _y  # Position
        str _name
    
    def __init__(self, double x=0.0, double y=0.0, str name="Shape"):
        self._x = x
        self._y = y
        self._name = name
    
    @property
    def position(self):
        return (self._x, self._y)
    
    @position.setter
    def position(self, tuple pos):
        self._x, self._y = pos
    
    @property
    def name(self):
        return self._name
    
    # Virtual method - to be overridden by subclasses
    cpdef double area(self):
        return 0.0
    
    def __repr__(self):
        return f"{self._name} at ({self._x}, {self._y})"
    
    def __richcmp__(self, other, int op):
        # op: 0=<, 1<=, 2==, 3!=, 4>, 5>=
        if not isinstance(other, Shape):
            return NotImplemented
        
        if op == 2:  # ==
            return (self._x == (<Shape>other)._x and 
                    self._y == (<Shape>other)._y and 
                    self._name == (<Shape>other)._name)
        elif op == 3:  # !=
            return not (self._x == (<Shape>other)._x and 
                       self._y == (<Shape>other)._y and 
                       self._name == (<Shape>other)._name)
        else:
            return NotImplemented

# Circle subclass
cdef class Circle(Shape):
    cdef double _radius
    
    def __init__(self, double x=0.0, double y=0.0, double radius=1.0):
        super().__init__(x, y, "Circle")
        self._radius = radius
    
    @property
    def radius(self):
        return self._radius
    
    @radius.setter
    def radius(self, double value):
        if value <= 0:
            raise ValueError("Radius must be positive")
        self._radius = value
    
    cpdef double area(self):
        return M_PI * self._radius * self._radius
    
    def __repr__(self):
        return f"Circle(radius={self._radius}) at ({self._x}, {self._y})"

# Rectangle subclass
cdef class Rectangle(Shape):
    cdef:
        double _width, _height
    
    def __init__(self, double x=0.0, double y=0.0, 
                 double width=1.0, double height=1.0):
        super().__init__(x, y, "Rectangle")
        self._width = width
        self._height = height
    
    @property
    def width(self):
        return self._width
    
    @width.setter
    def width(self, double value):
        if value <= 0:
            raise ValueError("Width must be positive")
        self._width = value
    
    @property
    def height(self):
        return self._height
    
    @height.setter
    def height(self, double value):
        if value <= 0:
            raise ValueError("Height must be positive")
        self._height = value
    
    cpdef double area(self):
        return self._width * self._height
    
    def __repr__(self):
        return f"Rectangle({self._width}×{self._height}) at ({self._x}, {self._y})"
```

**Pure Python Mode (`shapes_pure.py`):**

```python
# shapes_pure.py
# cython: language_level=3
import cython
import math

# Base shape class
@cython.cclass
class Shape:
    # C attributes
    _x: cython.double
    _y: cython.double
    _name: str
    
    def __init__(self, x=0.0, y=0.0, name="Shape"):
        self._x = x
        self._y = y
        self._name = name
    
    @property
    def position(self):
        return (self._x, self._y)
    
    @position.setter
    def position(self, pos):
        self._x, self._y = pos
    
    @property
    def name(self):
        return self._name
    
    # Virtual method - to be overridden by subclasses
    @cython.ccall
    def area(self) -> cython.double:
        return 0.0
    
    def __repr__(self):
        return f"{self._name} at ({self._x}, {self._y})"
    
    def __eq__(self, other):
        if not isinstance(other, Shape):
            return NotImplemented
        return (self._x == other._x and 
                self._y == other._y and 
                self._name == other._name)
    
    def __ne__(self, other):
        return not (self == other)

# Circle subclass
@cython.cclass
class Circle(Shape):
    _radius: cython.double
    
    def __init__(self, x=0.0, y=0.0, radius=1.0):
        super().__init__(x, y, "Circle")
        self._radius = radius
    
    @property
    def radius(self):
        return self._radius
    
    @radius.setter
    def radius(self, value):
        if value <= 0:
            raise ValueError("Radius must be positive")
        self._radius = value
    
    @cython.ccall
    def area(self) -> cython.double:
        return math.pi * self._radius * self._radius
    
    def __repr__(self):
        return f"Circle(radius={self._radius}) at ({self._x}, {self._y})"

# Rectangle subclass
@cython.cclass
class Rectangle(Shape):
    _width: cython.double
    _height: cython.double
    
    def __init__(self, x=0.0, y=0.0, width=1.0, height=1.0):
        super().__init__(x, y, "Rectangle")
        self._width = width
        self._height = height
    
    @property
    def width(self):
        return self._width
    
    @width.setter
    def width(self, value):
        if value <= 0:
            raise ValueError("Width must be positive")
        self._width = value
    
    @property
    def height(self):
        return self._height
    
    @height.setter
    def height(self, value):
        if value <= 0:
            raise ValueError("Height must be positive")
        self._height = value
    
    @cython.ccall
    def area(self) -> cython.double:
        return self._width * self._height
    
    def __repr__(self):
        return f"Rectangle({self._width}×{self._height}) at ({self._x}, {self._y})"
```

**Usage:**

```python
# test_shapes.py
import time
import random

# Import both implementations
from shapes import Shape as CShape, Circle as CCircle, Rectangle as CRectangle
from shapes_pure import Shape as PShape, Circle as PCircle, Rectangle as PRectangle

def test_shapes(ShapeClass, CircleClass, RectangleClass, name, iterations=100000):
    print(f"Testing {name} implementation:")
    
    # Create shapes
    circle = CircleClass(1.0, 2.0, 5.0)
    rectangle = RectangleClass(3.0, 4.0, 10.0, 20.0)
    
    print(f"Circle: {circle}")
    print(f"Rectangle: {rectangle}")
    print(f"Circle area: {circle.area()}")
    print(f"Rectangle area: {rectangle.area()}")
    
    # Benchmark area calculations
    start = time.time()
    total_area = 0.0
    
    for _ in range(iterations):
        if random.random() < 0.5:
            total_area += circle.area()
        else:
            total_area += rectangle.area()
    
    elapsed = time.time() - start
    print(f"Time for {iterations} area calculations: {elapsed:.4f}s")
    print()

# Test both implementations
test_shapes(CShape, CCircle, CRectangle, "Cython")
test_shapes(PShape, PCircle, PRectangle, "Pure Python")
```

## 2. Parallelism and Concurrency

### Parallel Loops with prange

**Traditional Cython Mode (`parallel.pyx`):**

```cython
# parallel.pyx
import numpy as np
cimport numpy as cnp
from cython.parallel import prange

def parallel_sum(double[:] arr):
    """Sum array elements in parallel."""
    cdef:
        int i
        double total = 0.0
    
    for i in prange(arr.shape[0], nogil=True):
        total += arr[i]
    
    return total

def parallel_vector_add(double[:] a, double[:] b, double[:] result):
    """Add two vectors element-wise in parallel."""
    cdef:
        int i, n = a.shape[0]
    
    # Check array sizes
    if a.shape[0] != b.shape[0] or a.shape[0] != result.shape[0]:
        raise ValueError("Arrays must be the same size")
    
    # Parallel addition
    for i in prange(n, nogil=True):
        result[i] = a[i] + b[i]

def parallel_matrix_vector(double[:, :] A, double[:] x, double[:] result):
    """Multiply matrix by vector in parallel."""
    cdef:
        int i, j, rows, cols
    
    rows = A.shape[0]
    cols = A.shape[1]
    
    # Check dimensions
    if cols != x.shape[0] or rows != result.shape[0]:
        raise ValueError("Incompatible dimensions")
    
    # Reset result vector
    for i in range(rows):
        result[i] = 0.0
    
    # Parallel matrix-vector multiplication
    for i in prange(rows, nogil=True):
        for j in range(cols):
            result[i] += A[i, j] * x[j]
```

**Pure Python Mode (`parallel_pure.py`):**

```python
# parallel_pure.py
# cython: language_level=3
import cython
import numpy as np
from cython.parallel import prange

@cython.nogil
@cython.cfunc
def parallel_sum(arr: cython.double[:]) -> cython.double:
    """Sum array elements in parallel."""
    total: cython.double = 0.0
    
    for i in prange(arr.shape[0]):
        total += arr[i]
    
    return total

def py_parallel_sum(arr):
    """Python-callable wrapper for parallel_sum."""
    return parallel_sum(arr)

@cython.nogil
def parallel_vector_add(a: cython.double[:], b: cython.double[:], result: cython.double[:]):
    """Add two vectors element-wise in parallel."""
    n: cython.int = a.shape[0]
    
    # Parallel addition
    for i in prange(n):
        result[i] = a[i] + b[i]

def py_parallel_vector_add(a, b):
    """Python-callable function for vector addition."""
    if len(a) != len(b):
        raise ValueError("Arrays must be the same size")
    
    result = np.zeros_like(a)
    parallel_vector_add(a, b, result)
    return result

@cython.nogil
def parallel_matrix_vector(A: cython.double[:, :], x: cython.double[:], result: cython.double[:]):
    """Multiply matrix by vector in parallel."""
    rows: cython.int = A.shape[0]
    cols: cython.int = A.shape[1]
    
    # Initialize result vector to zero
    for i in range(rows):
        result[i] = 0.0
    
    # Parallel matrix-vector multiplication
    for i in prange(rows):
        for j in range(cols):
            result[i] += A[i, j] * x[j]

def py_parallel_matrix_vector(A, x):
    """Python-callable function for matrix-vector multiplication."""
    if A.shape[1] != x.shape[0]:
        raise ValueError("Incompatible dimensions")
    
    result = np.zeros(A.shape[0])
    parallel_matrix_vector(A, x, result)
    return result
```

**Usage:**

```python
# test_parallel.py
import numpy as np
import time
import matplotlib.pyplot as plt

# Cython mode
import parallel

# Pure Python mode
import parallel_pure

def benchmark_parallel_operations(size=1000000, runs=5):
    # Create large test arrays
    a = np.random.random(size)
    b = np.random.random(size)
    
    # Small matrix for matrix-vector multiplication
    matrix_size = min(size, 1000)  # Use smaller size for matrix tests
    A = np.random.random((matrix_size, matrix_size))
    x = np.random.random(matrix_size)
    
    print(f"Benchmarking with array size: {size:,}")
    
    # Functions to test
    functions = [
        ("NumPy Sum", lambda: np.sum(a)),
        ("Cython Parallel Sum", lambda: parallel.parallel_sum(a)),
        ("Pure Python Parallel Sum", lambda: parallel_pure.py_parallel_sum(a)),
        ("NumPy Vector Add", lambda: a + b),
        ("Cython Parallel Add", lambda: {
            result = np.zeros_like(a)
            parallel.parallel_vector_add(a, b, result)
            return result
        }),
        ("Pure Python Parallel Add", lambda: parallel_pure.py_parallel_vector_add(a, b)),
        ("NumPy Matrix-Vector", lambda: A @ x),
        ("Cython Matrix-Vector", lambda: {
            result = np.zeros(A.shape[0])
            parallel.parallel_matrix_vector(A, x, result)
            return result
        }),
        ("Pure Python Matrix-Vector", lambda: parallel_pure.py_parallel_matrix_vector(A, x))
    ]
    
    results = {}
    
    # Run benchmarks
    for name, func in functions:
        # Warmup
        func()
        
        # Timed runs
        times = []
        for _ in range(runs):
            start = time.time()
            func()
            times.append(time.time() - start)
        
        # Record best time
        results[name] = min(times)
        print(f"{name}: {min(times):.6f} seconds")
    
    # Visualize results
    plt.figure(figsize=(12, 6))
    
    # Group results by operation
    sum_results = {k: v for k, v in results.items() if "Sum" in k}
    add_results = {k: v for k, v in results.items() if "Add" in k}
    mv_results = {k: v for k, v in results.items() if "Matrix-Vector" in k}
    
    # Plot each group
    plt.subplot(131)
    plt.bar(sum_results.keys(), sum_results.values())
    plt.title("Array Sum")
    plt.xticks(rotation=45)
    
    plt.subplot(132)
    plt.bar(add_results.keys(), add_results.values())
    plt.title("Vector Addition")
    plt.xticks(rotation=45)
    
    plt.subplot(133)
    plt.bar(mv_results.keys(), mv_results.values())
    plt.title("Matrix-Vector Multiplication")
    plt.xticks(rotation=45)
    
    plt.tight_layout()
    plt.show()

benchmark_parallel_operations()
```

### Releasing the GIL

**Traditional Cython Mode (`nogil.pyx`):**

```cython
# nogil.pyx
from libc.math cimport sin, cos, sqrt, fabs
from cython.parallel import prange
import numpy as np

# Function that runs without the GIL
cdef double compute_without_gil(double x, double y, int iterations) nogil:
    """Perform intensive computation without the GIL."""
    cdef:
        int i
        double result = 0.0
    
    for i in range(iterations):
        result += sin(x) * cos(y) / (i + 1)
        x += 0.0001
        y += 0.0001
    
    return result

# Python-callable wrapper
def py_compute(double x, double y, int iterations):
    """Python-callable wrapper for compute_without_gil."""
    return compute_without_gil(x, y, iterations)

# Function that releases the GIL temporarily
def release_gil_example(double[:] data, int iterations):
    """Process data by releasing the GIL temporarily."""
    cdef:
        int i, n = data.shape[0]
        double total = 0.0
    
    # This section runs without the GIL
    with nogil:
        for i in range(n):
            total += compute_without_gil(data[i], data[i] * 2, iterations)
    
    return total

# Parallel processing without the GIL
def parallel_process_nogil(double[:] data, int iterations):
    """Process data in parallel without the GIL."""
    cdef:
        int i, n = data.shape[0]
        double total = 0.0
    
    # Parallel loop without the GIL
    with nogil:
        for i in prange(n):
            total += compute_without_gil(data[i], data[i] * 2, iterations)
    
    return total
```

**Pure Python Mode (`nogil_pure.py`):**

```python
# nogil_pure.py
# cython: language_level=3
import cython
import math
from cython.parallel import prange
import numpy as np

# Function that runs without the GIL
@cython.cfunc
@cython.nogil
@cython.returns(cython.double)
def compute_without_gil(x: cython.double, y: cython.double, iterations: cython.int) -> cython.double:
    """Perform intensive computation without the GIL."""
    result: cython.double = 0.0
    
    for i in range(iterations):
        result += math.sin(x) * math.cos(y) / (i + 1)
        x += 0.0001
        y += 0.0001
    
    return result

# Python-callable wrapper
def py_compute(x, y, iterations):
    """Python-callable wrapper for compute_without_gil."""
    return compute_without_gil(x, y, iterations)

# Function that releases the GIL temporarily
def release_gil_example(data, iterations):
    """Process data by releasing the GIL temporarily."""
    n = len(data)
    total = 0.0
    
    # This section runs without the GIL
    with cython.nogil:
        for i in range(n):
            total += compute_without_gil(data[i], data[i] * 2, iterations)
    
    return total

# Parallel processing without the GIL
def parallel_process_nogil(data, iterations):
    """Process data in parallel without the GIL."""
    n = len(data)
    total = 0.0
    
    # Parallel loop without the GIL
    with cython.nogil:
        for i in prange(n):
            total += compute_without_gil(data[i], data[i] * 2, iterations)
    
    return total
```

**Usage:**

```python
# test_nogil.py
import time
import numpy as np
import threading
import matplotlib.pyplot as plt

# Import Cython implementations
import nogil
import nogil_pure

def test_nogil_function():
    """Test basic nogil function."""
    # Parameters for testing
    x, y = 1.5, 2.5
    iterations = 1000000
    
    # Test both implementations
    start = time.time()
    result1 = nogil.py_compute(x, y, iterations)
    cython_time = time.time() - start
    
    start = time.time()
    result2 = nogil_pure.py_compute(x, y, iterations)
    pure_time = time.time() - start
    
    print(f"Cython result: {result1:.8f} in {cython_time:.6f} seconds")
    print(f"Pure Python result: {result2:.8f} in {pure_time:.6f} seconds")
    print(f"Results match: {abs(result1 - result2) < 1e-10}")
    print()

def test_nogil_threading():
    """Test nogil with threading."""
    data = np.random.random(10000)
    iterations = 10000
    threads = 4
    
    # Test functions
    functions = [
        ("Cython - Release GIL", nogil.release_gil_example),
        ("Cython - Parallel NoGIL", nogil.parallel_process_nogil),
        ("Pure Python - Release GIL", nogil_pure.release_gil_example),
        ("Pure Python - Parallel NoGIL", nogil_pure.parallel_process_nogil)
    ]
    
    for name, func in functions:
        # Single-threaded execution
        start = time.time()
        result_single = func(data, iterations)
        single_time = time.time() - start
        
        # Multi-threaded execution using Python's threading
        results = [None] * threads
        thread_list = []
        
        # Create threads that all call the same function
        start = time.time()
        for i in range(threads):
            chunk_size = len(data) // threads
            start_idx = i * chunk_size
            end_idx = start_idx + chunk_size if i < threads - 1 else len(data)
            chunk = data[start_idx:end_idx]
            
            def thread_func(idx, data_chunk):
                results[idx] = func(data_chunk, iterations)
            
            thread = threading.Thread(target=thread_func, args=(i, chunk))
            thread_list.append(thread)
            thread.start()
        
        # Wait for all threads to finish
        for thread in thread_list:
            thread.join()
        
        # Sum results from all threads
        result_multi = sum(results)
        multi_time = time.time() - start
        
        # Calculate speedup
        speedup = single_time / multi_time
        
        print(f"{name}:")
        print(f"  Single-threaded: {single_time:.6f} seconds")
        print(f"  Multi-threaded ({threads} threads): {multi_time:.6f} seconds")
        print(f"  Speedup: {speedup:.2f}x")
        print()

if __name__ == "__main__":
    test_nogil_function()
    test_nogil_threading()
```

## 3. Integration Capabilities

### Working with NumPy Arrays

**Traditional Cython Mode (`numpy_ops.pyx`):**

```cython
# numpy_ops.pyx
import numpy as np
cimport numpy as cnp

# Ensure NumPy is initialized
cnp.import_array()

def array_stats(cnp.ndarray[cnp.double_t, ndim=1] arr):
    """Calculate basic statistics for a NumPy array."""
    cdef:
        int i, n = arr.shape[0]
        double total = 0.0
        double mean, var = 0.0
        double min_val = arr[0]
        double max_val = arr[0]
    
    # Calculate sum, min, max
    for i in range(n):
        total += arr[i]
        if arr[i] < min_val:
            min_val = arr[i]
        if arr[i] > max_val:
            max_val = arr[i]
    
    # Calculate mean
    mean = total / n
    
    # Calculate variance
    for i in range(n):
        var += (arr[i] - mean) ** 2
    var /= n
    
    return {
        'mean': mean,
        'variance': var,
        'min': min_val,
        'max': max_val,
        'sum': total,
        'count': n
    }

def weighted_sum(
    cnp.ndarray[cnp.double_t, ndim=1] values,
    cnp.ndarray[cnp.double_t, ndim=1] weights
):
    """Calculate weighted sum of an array."""
    cdef:
        int i, n = values.shape[0]
        double total = 0.0
    
    # Check dimensions
    if values.shape[0] != weights.shape[0]:
        raise ValueError("Arrays must have the same length")
    
    # Calculate weighted sum
    for i in range(n):
        total += values[i] * weights[i]
    
    return total

def row_means(cnp.ndarray[cnp.double_t, ndim=2] arr):
    """Calculate mean of each row in a 2D array."""
    cdef:
        int i, j
        int rows = arr.shape[0]
        int cols = arr.shape[1]
        cnp.ndarray[cnp.double_t, ndim=1] means = np.zeros(rows, dtype=np.double)
    
    for i in range(rows):
        for j in range(cols):
            means[i] += arr[i, j]
        means[i] /= cols
    
    return means

def convolve_1d(
    cnp.ndarray[cnp.double_t, ndim=1] arr,
    cnp.ndarray[cnp.double_t, ndim=1] kernel
):
    """Apply 1D convolution with a kernel."""
    cdef:
        int i, j
        int n = arr.shape[0]
        int k = kernel.shape[0]
        int result_size = n - k + 1
        cnp.ndarray[cnp.double_t, ndim=1] result = np.zeros(result_size, dtype=np.double)
    
    # Apply convolution
    for i in range(result_size):
        for j in range(k):
            result[i] += arr[i + j] * kernel[j]
    
    return result
```

**Pure Python Mode (`numpy_ops_pure.py`):**

```python
# numpy_ops_pure.py
# cython: language_level=3
import cython
import numpy as np

def array_stats(arr: cython.double[:]):
    """Calculate basic statistics for a NumPy array."""
    n: cython.int = arr.shape[0]
    total: cython.double = 0.0
    min_val: cython.double = arr[0]
    max_val: cython.double = arr[0]
    
    # Calculate sum, min, max
    for i in range(n):
        total += arr[i]
        if arr[i] < min_val:
            min_val = arr[i]
        if arr[i] > max_val:
            max_val = arr[i]
    
    # Calculate mean
    mean: cython.double = total / n
    
    # Calculate variance
    var: cython.double = 0.0
    for i in range(n):
        var += (arr[i] - mean) ** 2
    var /= n
    
    return {
        'mean': mean,
        'variance': var,
        'min': min_val,
        'max': max_val,
        'sum': total,
        'count': n
    }

def weighted_sum(values: cython.double[:], weights: cython.double[:]):
    """Calculate weighted sum of an array."""
    n: cython.int = values.shape[0]
    total: cython.double = 0.0
    
    # Check dimensions
    if values.shape[0] != weights.shape[0]:
        raise ValueError("Arrays must have the same length")
    
    # Calculate weighted sum
    for i in range(n):
        total += values[i] * weights[i]
    
    return total

def row_means(arr: cython.double[:, :]):
    """Calculate mean of each row in a 2D array."""
    rows: cython.int = arr.shape[0]
    cols: cython.int = arr.shape[1]
    means = np.zeros(rows, dtype=np.float64)
    
    for i in range(rows):
        for j in range(cols):
            means[i] += arr[i, j]
        means[i] /= cols
    
    return means

def convolve_1d(arr: cython.double[:], kernel: cython.double[:]):
    """Apply 1D convolution with a kernel."""
    n: cython.int = arr.shape[0]
    k: cython.int = kernel.shape[0]
    result_size: cython.int = n - k + 1
    result = np.zeros(result_size, dtype=np.float64)
    
    # Apply convolution
    for i in range(result_size):
        for j in range(k):
            result[i] += arr[i + j] * kernel[j]
    
    return result
```

**Usage:**

```python
# test_numpy_ops.py
import numpy as np
import time
import matplotlib.pyplot as plt

# Import both implementations
import numpy_ops
import numpy_ops_pure

def benchmark_numpy_operations(size=1000000):
    """Benchmark NumPy operations."""
    # Create test data
    data = np.random.random(size)
    weights = np.random.random(size)
    
    # Create 2D array for row means
    rows = min(size, 10000)  # Use smaller size for 2D operations
    cols = 100
    array_2d = np.random.random((rows, cols))
    
    # Create data for convolution
    conv_size = min(size, 100000)
    kernel_size = 10
    array_1d = np.random.random(conv_size)
    kernel = np.random.random(kernel_size)
    
    # Functions to test
    functions = [
        ("Stats - Cython", lambda: numpy_ops.array_stats(data[:10000])),
        ("Stats - Pure Python", lambda: numpy_ops_pure.array_stats(data[:10000])),
        ("Stats - NumPy", lambda: {
            'mean': np.mean(data[:10000]),
            'variance': np.var(data[:10000]),
            'min': np.min(data[:10000]),
            'max': np.max(data[:10000]),
            'sum': np.sum(data[:10000]),
            'count': len(data[:10000])
        }),
        ("Weighted Sum - Cython", lambda: numpy_ops.weighted_sum(data[:10000], weights[:10000])),
        ("Weighted Sum - Pure Python", lambda: numpy_ops_pure.weighted_sum(data[:10000], weights[:10000])),
        ("Weighted Sum - NumPy", lambda: np.sum(data[:10000] * weights[:10000])),
        ("Row Means - Cython", lambda: numpy_ops.row_means(array_2d)),
        ("Row Means - Pure Python", lambda: numpy_ops_pure.row_means(array_2d)),
        ("Row Means - NumPy", lambda: np.mean(array_2d, axis=1)),
        ("Convolution - Cython", lambda: numpy_ops.convolve_1d(array_1d, kernel)),
        ("Convolution - Pure Python", lambda: numpy_ops_pure.convolve_1d(array_1d, kernel)),
        ("Convolution - NumPy", lambda: np.convolve(array_1d, kernel, mode='valid'))
    ]
    
    results = {}
    
    # Run benchmarks
    for name, func in functions:
        # Warmup
        func()
        
        # Timed run
        start = time.time()
        func()
        elapsed = time.time() - start
        
        results[name] = elapsed
        print(f"{name}: {elapsed:.6f} seconds")
    
    # Plot results
    plt.figure(figsize=(12, 6))
    
    # Group results by operation
    stats_results = {k: v for k, v in results.items() if "Stats" in k}
    weight_results = {k: v for k, v in results.items() if "Weighted" in k}
    row_results = {k: v for k, v in results.items() if "Row" in k}
    conv_results = {k: v for k, v in results.items() if "Convolution" in k}
    
    # Plot each group
    plt.subplot(221)
    plt.bar(stats_results.keys(), stats_results.values())
    plt.title("Array Stats")
    plt.xticks(rotation=45, ha='right')
    
    plt.subplot(222)
    plt.bar(weight_results.keys(), weight_results.values())
    plt.title("Weighted Sum")
    plt.xticks(rotation=45, ha='right')
    
    plt.subplot(223)
    plt.bar(row_results.keys(), row_results.values())
    plt.title("Row Means")
    plt.xticks(rotation=45, ha='right')
    
    plt.subplot(224)
    plt.bar(conv_results.keys(), conv_results.values())
    plt.title("Convolution")
    plt.xticks(rotation=45, ha='right')
    
    plt.tight_layout()
    plt.show()

# Run benchmarks
benchmark_numpy_operations()
```

### C++ Integration

**C++ Class Header (`vector3d.h`):**

```cpp
// vector3d.h
#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
#include <iostream>

class Vector3D {
private:
    double x, y, z;

public:
    // Constructors
    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
    
    // Copy constructor
    Vector3D(const Vector3D& other) : x(other.x), y(other.y), z(other.z) {}
    
    // Accessors
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    
    // Mutators
    void setX(double value) { x = value; }
    void setY(double value) { y = value; }
    void setZ(double value) { z = value; }
    
    // Operations
    double magnitude() const {
        return std::sqrt(x*x + y*y + z*z);
    }
    
    Vector3D normalize() const {
        double mag = magnitude();
        if (mag > 0) {
            return Vector3D(x/mag, y/mag, z/mag);
        }
        return Vector3D();
    }
    
    Vector3D add(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3D subtract(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }
    
    double dot(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    Vector3D cross(const Vector3D& other) const {
        return Vector3D(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    Vector3D scale(double factor) const {
        return Vector3D(x * factor, y * factor, z * factor);
    }
    
    void print() const {
        std::cout << "Vector3D(" << x << ", " << y << ", " << z << ")" << std::endl;
    }
};

#endif // VECTOR3D_H
```

**Traditional Cython Mode (`cpp_wrapper.pyx`):**

```cython
# cpp_wrapper.pyx
# distutils: language = c++

# Import C++ class definition
cdef extern from "vector3d.h":
    cdef cppclass Vector3D:
        Vector3D() except +
        Vector3D(double x, double y, double z) except +
        Vector3D(const Vector3D&) except +
        
        double getX() const
        double getY() const
        double getZ() const
        
        void setX(double)
        void setY(double)
        void setZ(double)
        
        double magnitude() const
        Vector3D normalize() const
        Vector3D add(const Vector3D&) const
        Vector3D subtract(const Vector3D&) const
        double dot(const Vector3D&) const
        Vector3D cross(const Vector3D&) const
        Vector3D scale(double) const
        void print() const

# Python wrapper class
cdef class PyVector3D:
    """Python wrapper for C++ Vector3D class."""
    cdef Vector3D* thisptr
    
    def __cinit__(self, double x=0, double y=0, double z=0):
        self.thisptr = new Vector3D(x, y, z)
    
    def __dealloc__(self):
        del self.thisptr
    
    # Properties
    @property
    def x(self):
        return self.thisptr.getX()
    
    @x.setter
    def x(self, double value):
        self.thisptr.setX(value)
    
    @property
    def y(self):
        return self.thisptr.getY()
    
    @y.setter
    def y(self, double value):
        self.thisptr.setY(value)
    
    @property
    def z(self):
        return self.thisptr.getZ()
    
    @z.setter
    def z(self, double value):
        self.thisptr.setZ(value)
    
    # Methods
    def magnitude(self):
        return self.thisptr.magnitude()
    
    def normalize(self):
        cdef Vector3D result = self.thisptr.normalize()
        return PyVector3D(result.getX(), result.getY(), result.getZ())
    
    def __add__(self, PyVector3D other):
        cdef Vector3D result = self.thisptr.add(other.thisptr[0])
        return PyVector3D(result.getX(), result.getY(), result.getZ())
    
    def __sub__(self, PyVector3D other):
        cdef Vector3D result = self.thisptr.subtract(other.thisptr[0])
        return PyVector3D(result.getX(), result.getY(), result.getZ())
    
    def dot(self, PyVector3D other):
        return self.thisptr.dot(other.thisptr[0])
    
    def cross(self, PyVector3D other):
        cdef Vector3D result = self.thisptr.cross(other.thisptr[0])
        return PyVector3D(result.getX(), result.getY(), result.getZ())
    
    def scale(self, double factor):
        cdef Vector3D result = self.thisptr.scale(factor)
        return PyVector3D(result.getX(), result.getY(), result.getZ())
    
    def print_vector(self):
        self.thisptr.print()
    
    def __repr__(self):
        return f"PyVector3D({self.x}, {self.y}, {self.z})"
```

Setup file for C++ integration:

```python
# setup.py for C++ integration
from setuptools import setup, Extension
from Cython.Build import cythonize

ext_modules = [
    Extension(
        "cpp_wrapper",
        ["cpp_wrapper.pyx"],
        language="c++",
    ),
]

setup(
    name="Cython C++ Example",
    ext_modules=cythonize(ext_modules),
)
```

**Usage:**

```python
# test_cpp_wrapper.py
import time
import numpy as np
import matplotlib.pyplot as plt
from cpp_wrapper import PyVector3D

def test_vector_operations():
    """Test C++ vector operations."""
    # Create vectors
    v1 = PyVector3D(1.0, 2.0, 3.0)
    v2 = PyVector3D(4.0, 5.0, 6.0)
    
    print(f"v1 = {v1}")
    print(f"v2 = {v2}")
    
    # Test properties
    print(f"v1.x = {v1.x}, v1.y = {v1.y}, v1.z = {v1.z}")
    
    # Change property
    v1.x = 10.0
    print(f"After changing x: v1 = {v1}")
    
    # Test methods
    print(f"Magnitude of v1: {v1.magnitude()}")
    print(f"Normalized v1: {v1.normalize()}")
    print(f"v1 + v2 = {v1 + v2}")
    print(f"v1 - v2 = {v1 - v2}")
    print(f"v1 · v2 = {v1.dot(v2)}")
    print(f"v1 × v2 = {v1.cross(v2)}")
    print(f"2 * v1 = {v1.scale(2.0)}")
    
    # Use C++ print method
    print("C++ print method output:")
    v1.print_vector()

def benchmark_vector_operations(iterations=1000000):
    """Benchmark vector operations."""
    # Create vectors
    v1 = PyVector3D(1.0, 2.0, 3.0)
    v2 = PyVector3D(4.0, 5.0, 6.0)
    
    # Operations to benchmark
    operations = {
        "Magnitude": lambda: v1.magnitude(),
        "Normalize": lambda: v1.normalize(),
        "Addition": lambda: v1 + v2,
        "Subtraction": lambda: v1 - v2,
        "Dot Product": lambda: v1.dot(v2),
        "Cross Product": lambda: v1.cross(v2),
        "Scaling": lambda: v1.scale(2.0)
    }
    
    # Pure Python implementation for comparison
    class PythonVector3D:
        def __init__(self, x=0, y=0, z=0):
            self.x = x
            self.y = y
            self.z = z
        
        def magnitude(self):
            return (self.x**2 + self.y**2 + self.z**2)**0.5
        
        def normalize(self):
            mag = self.magnitude()
            if mag > 0:
                return PythonVector3D(self.x/mag, self.y/mag, self.z/mag)
            return PythonVector3D()
        
        def __add__(self, other):
            return PythonVector3D(self.x + other.x, self.y + other.y, self.z + other.z)
        
        def __sub__(self, other):
            return PythonVector3D(self.x - other.x, self.y - other.y, self.z - other.z)
        
        def dot(self, other):
            return self.x * other.x + self.y * other.y + self.z * other.z
        
        def cross(self, other):
            return PythonVector3D(
                self.y * other.z - self.z * other.y,
                self.z * other.x - self.x * other.z,
                self.x * other.y - self.y * other.x
            )
        
        def scale(self, factor):
            return PythonVector3D(self.x * factor, self.y * factor, self.z * factor)
        
        def __repr__(self):
            return f"PythonVector3D({self.x}, {self.y}, {self.z})"
    
    # Create Python vectors
    py_v1 = PythonVector3D(1.0, 2.0, 3.0)
    py_v2 = PythonVector3D(4.0, 5.0, 6.0)
    
    # Python operations
    python_operations = {
        "Magnitude": lambda: py_v1.magnitude(),
        "Normalize": lambda: py_v1.normalize(),
        "Addition": lambda: py_v1 + py_v2,
        "Subtraction": lambda: py_v1 - py_v2,
        "Dot Product": lambda: py_v1.dot(py_v2),
        "Cross Product": lambda: py_v1.cross(py_v2),
        "Scaling": lambda: py_v1.scale(2.0)
    }
    
    # NumPy operations for comparison
    np_v1 = np.array([1.0, 2.0, 3.0])
    np_v2 = np.array([4.0, 5.0, 6.0])
    
    numpy_operations = {
        "Magnitude": lambda: np.linalg.norm(np_v1),
        "Normalize": lambda: np_v1 / np.linalg.norm(np_v1),
        "Addition": lambda: np_v1 + np_v2,
        "Subtraction": lambda: np_v1 - np_v2,
        "Dot Product": lambda: np.dot(np_v1, np_v2),
        "Cross Product": lambda: np.cross(np_v1, np_v2),
        "Scaling": lambda: 2.0 * np_v1
    }
    
    # Benchmark all operations
    cpp_results = {}
    python_results = {}
    numpy_results = {}
    
    print("\nBenchmarking vector operations:")
    for op_name in operations:
        # C++ implementation
        start = time.time()
        for _ in range(iterations):
            operations[op_name]()
        cpp_time = time.time() - start
        cpp_results[op_name] = cpp_time
        
        # Python implementation
        start = time.time()
        for _ in range(iterations):
            python_operations[op_name]()
        python_time = time.time() - start
        python_results[op_name] = python_time
        
        # NumPy implementation
        start = time.time()
        for _ in range(iterations):
            numpy_operations[op_name]()
        numpy_time = time.time() - start
        numpy_results[op_name] = numpy_time
        
        # Report results
        print(f"{op_name}:")
        print(f"  C++:    {cpp_time:.6f} seconds")
        print(f"  Python: {python_time:.6f} seconds")
        print(f"  NumPy:  {numpy_time:.6f} seconds")
        print(f"  Speedup over Python: {python_time/cpp_time:.2f}x")
        print(f"  Speedup over NumPy:  {numpy_time/cpp_time:.2f}x")
        print()
    
    # Visualize results
    plt.figure(figsize=(12, 6))
    
    # Prepare data for grouped bar chart
    op_names = list(operations.keys())
    cpp_times = [cpp_results[op] for op in op_names]
    python_times = [python_results[op] for op in op_names]
    numpy_times = [numpy_results[op] for op in op_names]
    
    # Setup bar positions
    x = np.arange(len(op_names))
    width = 0.25
    
    # Create bars
    plt.bar(x - width, cpp_times, width, label='C++')
    plt.bar(x, python_times, width, label='Python')
    plt.bar(x + width, numpy_times, width, label='NumPy')
    
    # Add labels and title
    plt.xlabel('Operation')
    plt.ylabel('Time (seconds)')
    plt.title('Vector Operations Performance Comparison')
    plt.xticks(x, op_names, rotation=45)
    plt.legend()
    
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    test_vector_operations()
    benchmark_vector_operations()
```

### Interfacing with External Libraries

This example shows how to interface with the standard C math library:

**Traditional Cython Mode (`math_funcs.pyx`):**

```cython
# math_funcs.pyx
from libc.math cimport sin, cos, tan, exp, log, sqrt, pow, fabs
from libc.stdlib cimport rand, srand
from libc.time cimport time as c_time

# Initialize random seed
def init_random():
    srand(c_time(NULL))

# Get random number between 0 and 1
def random():
    return rand() / 2147483647.0  # RAND_MAX on many systems

# Trigonometric functions
def math_sin(double x):
    return sin(x)

def math_cos(double x):
    return cos(x)

def math_tan(double x):
    return tan(x)

# Exponential and logarithmic functions
def math_exp(double x):
    return exp(x)

def math_log(double x):
    if x <= 0:
        raise ValueError("Input must be positive")
    return log(x)

# Power and square root
def math_sqrt(double x):
    if x < 0:
        raise ValueError("Cannot take square root of negative number")
    return sqrt(x)

def math_pow(double x, double y):
    return pow(x, y)

# Absolute value
def math_abs(double x):
    return fabs(x)

# Complex calculation
def complex_calculation(double x, double y, int iterations=1000):
    """Perform a complex calculation using C math functions."""
    cdef:
        int i
        double result = 0.0
    
    for i in range(iterations):
        result += sin(x) * cos(y) / (i + 1) + pow(x, 2) * exp(-fabs(y) / (i + 1))
    
    return result
```

**Pure Python Mode (`math_funcs_pure.py`):**

```python
# math_funcs_pure.py
# cython: language_level=3
import cython
import time

# Import C math functions
from libc.math cimport sin, cos, tan, exp, log, sqrt, pow, fabs
from libc.stdlib cimport rand, srand
from libc.time cimport time as c_time

# Initialize random seed
def init_random():
    srand(c_time(NULL))

# Get random number between 0 and 1
def random():
    return rand() / 2147483647.0  # RAND_MAX on many systems

# Trigonometric functions
def math_sin(x: cython.double) -> cython.double:
    return sin(x)

def math_cos(x: cython.double) -> cython.double:
    return cos(x)

def math_tan(x: cython.double) -> cython.double:
    return tan(x)

# Exponential and logarithmic functions
def math_exp(x: cython.double) -> cython.double:
    return exp(x)

def math_log(x: cython.double) -> cython.double:
    if x <= 0:
        raise ValueError("Input must be positive")
    return log(x)

# Power and square root
def math_sqrt(x: cython.double) -> cython.double:
    if x < 0:
        raise ValueError("Cannot take square root of negative number")
    return sqrt(x)

def math_pow(x: cython.double, y: cython.double) -> cython.double:
    return pow(x, y)

# Absolute value
def math_abs(x: cython.double) -> cython.double:
    return fabs(x)

# Complex calculation
def complex_calculation(x: cython.double, y: cython.double, iterations: cython.int = 1000) -> cython.double:
    """Perform a complex calculation using C math functions."""
    result: cython.double = 0.0
    
    for i in range(iterations):
        result += sin(x) * cos(y) / (i + 1) + pow(x, 2) * exp(-fabs(y) / (i + 1))
    
    return result
```

**Usage:**

```python
# test_math_funcs.py
import time
import math
import numpy as np
import matplotlib.pyplot as plt

# Import both implementations
import math_funcs
import math_funcs_pure

def test_math_functions():
    """Test C math functions."""
    # Initialize random seed
    math_funcs.init_random()
    
    # Test with various inputs
    x = 1.5
    y = 2.5
    
    # C functions from Cython
    print(f"C Math Functions (x={x}, y={y}):")
    print(f"sin(x) = {math_funcs.math_sin(x)}")
    print(f"cos(x) = {math_funcs.math_cos(x)}")
    print(f"tan(x) = {math_funcs.math_tan(x)}")
    print(f"exp(x) = {math_funcs.math_exp(x)}")
    print(f"log(x) = {math_funcs.math_log(x)}")
    print(f"sqrt(x) = {math_funcs.math_sqrt(x)}")
    print(f"pow(x, y) = {math_funcs.math_pow(x, y)}")
    print(f"abs(-x) = {math_funcs.math_abs(-x)}")
    print()
    
    # Generate 5 random numbers
    print("Random numbers:")
    for _ in range(5):
        print(f"  {math_funcs.random()}")
    print()
    
    # Test complex calculation
    iterations = 1000
    result = math_funcs.complex_calculation(x, y, iterations)
    print(f"Complex calculation result ({iterations} iterations): {result}")

def benchmark_math_functions():
    """Benchmark math functions."""
    # Parameters for testing
    x = 1.5
    y = 2.5
    iterations = 1000
    repeats = 100000
    
    # Functions to benchmark
    functions = [
        ("sin - Cython", lambda: math_funcs.math_sin(x)),
        ("sin - Pure Python", lambda: math_funcs_pure.math_sin(x)),
        ("sin - Python Math", lambda: math.sin(x)),
        ("sin - NumPy", lambda: np.sin(x)),
        
        ("exp - Cython", lambda: math_funcs.math_exp(x)),
        ("exp - Pure Python", lambda: math_funcs_pure.math_exp(x)),
        ("exp - Python Math", lambda: math.exp(x)),
        ("exp - NumPy", lambda: np.exp(x)),
        
        ("pow - Cython", lambda: math_funcs.math_pow(x, y)),
        ("pow - Pure Python", lambda: math_funcs_pure.math_pow(x, y)),
        ("pow - Python Math", lambda: math.pow(x, y)),
        ("pow - NumPy", lambda: np.power(x, y)),
        
        ("Complex - Cython", lambda: math_funcs.complex_calculation(x, y, iterations)),
        ("Complex - Pure Python", lambda: math_funcs_pure.complex_calculation(x, y, iterations)),
    ]
    
    results = {}
    
    # Run benchmarks
    print("\nBenchmarking math functions:")
    for name, func in functions:
        start = time.time()
        for _ in range(repeats):
            func()
        elapsed = time.time() - start
        results[name] = elapsed
        print(f"{name}: {elapsed:.6f} seconds")
    
    # Visualize results
    plt.figure(figsize=(12, 8))
    
    # Group results by function
    sin_results = {k: v for k, v in results.items() if "sin" in k.lower()}
    exp_results = {k: v for k, v in results.items() if "exp" in k.lower()}
    pow_results = {k: v for k, v in results.items() if "pow" in k.lower()}
    complex_results = {k: v for k, v in results.items() if "complex" in k.lower()}
    
    # Plot each group
    plt.subplot(221)
    plt.bar(sin_results.keys(), sin_results.values())
    plt.title("sin(x)")
    plt.xticks(rotation=45)
    
    plt.subplot(222)
    plt.bar(exp_results.keys(), exp_results.values())
    plt.title("exp(x)")
    plt.xticks(rotation=45)
    
    plt.subplot(223)
    plt.bar(pow_results.keys(), pow_results.values())
    plt.title("pow(x, y)")
    plt.xticks(rotation=45)
    
    plt.subplot(224)
    plt.bar(complex_results.keys(), complex_results.values())
    plt.title("Complex Calculation")
    plt.xticks(rotation=45)
    
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    test_math_functions()
    benchmark_math_functions()
```

## Summary of Phase 3 Advanced Features

Let's summarize the key concepts we've covered in Phase 3:

### 1. Object-Oriented Programming with Cython

- **Extension Types (`cdef class`)**: Allow you to create efficient, statically-typed Python classes
- **C-level attributes**: Store data directly as C types for efficiency
- **Property accessors**: Provide controlled access to C attributes
- **Method types**:
  - Regular Python methods: Callable from Python but slower
  - C methods (`cdef`): Fast but only callable from Cython code
  - Hybrid methods (`cpdef`): Both Python-callable and Cython-optimized
- **Inheritance**: Extension types can inherit from other extension types
- **Special methods**: Implement Python protocols (`__add__`, `__richcmp__`, etc.)

### 2. Parallelism and Concurrency

- **GIL (Global Interpreter Lock)**: Normally restricts Python to one thread at a time
- **Releasing the GIL**: Use `with nogil:` to allow true parallelism
- **Parallel loops with `prange`**: Automatic parallelization of loops
- **Thread safety considerations**: Must be careful with shared data when the GIL is released
- **Performance scaling**: Great for CPU-bound tasks on multicore systems

### 3. Integration Capabilities

- **NumPy Integration**:
  - Type-specific Cython declarations for NumPy arrays
  - Memory views for efficient array access
  - Optimization of array operations
  - Interoperability with NumPy's C API

- **C++ Integration**:
  - Declaring C++ classes in Cython
  - Creating Python wrapper classes
  - Memory management with constructors/destructors
  - Handling C++ exceptions
  - Method overloading and operator overloading

- **External Library Integration**:
  - Using C standard library functions
  - Interfacing with system libraries
  - Improving performance with optimized external implementations
  - Error handling and memory management

## Common Patterns and Best Practices

1. **Choose the right approach**:
   - Pure Python mode: Best for compatibility and ease of development
   - Traditional Cython mode: Best for maximum performance and C/C++ integration

2. **Type declarations**:
   - Add static types to function parameters first
   - Add types to local variables in performance-critical loops
   - Use memory views for array operations
   - Add typing gradually, starting with bottlenecks

3. **Object-oriented programming**:
   - Use `cdef class` for performance-critical classes
   - Expose C attributes through property accessors
   - Implement special methods for pythonic behavior
   - Consider a mix of Python methods and C-level methods

4. **Parallelism**:
   - Release the GIL for CPU-bound work
   - Use `prange` for simple loop parallelization
   - Avoid shared mutable state when running without the GIL
   - Use the correct scheduling strategy for the workload

5. **Integration**:
   - For NumPy: Use `cimport numpy` and typed views
   - For C++: Declare classes with `cdef extern from`
   - For C libraries: Use `cimport` from appropriate modules

6. **Error handling**:
   - Always check return values from C functions
   - Properly manage memory (free what you allocate)
   - Use Cython's C++ integration for exception handling

These examples demonstrate how Cython bridges the gap between Python and C/C++, offering great flexibility in how you approach optimization. Whether you need full control with traditional Cython syntax or prefer the Python-like approach of Pure Python mode, Cython provides powerful tools to accelerate your code while maintaining Python's ease of use.



---
---

# Phase 4: Mastery and Best Practices in Cython

## 1. Performance Tuning

### Profiling Cython Code

**Traditional Cython Mode (`profile_example.pyx`):**

```cython
# profile_example.pyx
# cython: profile=True
# cython: linetrace=True
# distutils: define_macros=CYTHON_TRACE=1

import numpy as np
cimport numpy as cnp
from libc.math cimport sqrt, pow

def slow_distance(cnp.ndarray[cnp.double_t, ndim=2] points):
    """Calculate pairwise distances between points (unoptimized)."""
    cdef:
        int i, j, n = points.shape[0]
        cnp.ndarray[cnp.double_t, ndim=2] distances = np.zeros((n, n), dtype=np.double)
    
    for i in range(n):
        for j in range(n):
            # Inefficient: redundant calculations and sqrt per iteration
            distances[i, j] = sqrt(
                pow(points[i, 0] - points[j, 0], 2) +
                pow(points[i, 1] - points[j, 1], 2) +
                pow(points[i, 2] - points[j, 2], 2)
            )
    
    return distances

def optimized_distance(cnp.ndarray[cnp.double_t, ndim=2] points):
    """Calculate pairwise distances between points (optimized)."""
    cdef:
        int i, j, n = points.shape[0]
        cnp.ndarray[cnp.double_t, ndim=2] distances = np.zeros((n, n), dtype=np.double)
        double dx, dy, dz
    
    for i in range(n):
        # Avoid redundancy: set diagonal to 0 and compute only half
        for j in range(i+1, n):
            dx = points[i, 0] - points[j, 0]
            dy = points[i, 1] - points[j, 1]
            dz = points[i, 2] - points[j, 2]
            
            # Compute once and reuse for symmetric matrix
            distances[i, j] = sqrt(dx*dx + dy*dy + dz*dz)
            distances[j, i] = distances[i, j]
    
    return distances
```

**Pure Python Mode (`profile_example_pure.py`):**

```python
# profile_example_pure.py
# cython: profile=True
# cython: language_level=3
import cython
import numpy as np

def slow_distance(points):
    """Calculate pairwise distances between points (unoptimized)."""
    n = points.shape[0]
    distances = np.zeros((n, n), dtype=np.float64)
    
    for i in range(n):
        for j in range(n):
            # Inefficient: redundant calculations
            distances[i, j] = np.sqrt(
                (points[i, 0] - points[j, 0])**2 +
                (points[i, 1] - points[j, 1])**2 +
                (points[i, 2] - points[j, 2])**2
            )
    
    return distances

def optimized_distance(points):
    """Calculate pairwise distances between points (optimized)."""
    n = points.shape[0]
    distances = np.zeros((n, n), dtype=np.float64)
    
    for i in range(n):
        # Avoid redundancy: compute only half and mirror
        for j in range(i+1, n):
            dx = points[i, 0] - points[j, 0]
            dy = points[i, 1] - points[j, 1]
            dz = points[i, 2] - points[j, 2]
            
            # Compute once and reuse
            distances[i, j] = np.sqrt(dx*dx + dy*dy + dz*dz)
            distances[j, i] = distances[i, j]
    
    return distances
```

**Profiling Script:**

```python
# profile_test.py
import numpy as np
import time
import cProfile
import pstats
from pstats import SortKey

# Import functions to profile
from profile_example import slow_distance, optimized_distance
# from profile_example_pure import slow_distance, optimized_distance  # Uncomment to test pure version

def profile_functions():
    # Generate random points
    np.random.seed(42)
    points = np.random.random((100, 3))
    
    # Profile slow function
    print("Profiling slow_distance...")
    cProfile.runctx('slow_distance(points)', globals(), locals(), 'slow_prof.prof')
    
    # Profile optimized function
    print("Profiling optimized_distance...")
    cProfile.runctx('optimized_distance(points)', globals(), locals(), 'opt_prof.prof')
    
    # Load and print stats
    print("\nSlow function stats:")
    s = pstats.Stats('slow_prof.prof')
    s.strip_dirs().sort_stats(SortKey.TIME).print_stats(10)
    
    print("\nOptimized function stats:")
    s = pstats.Stats('opt_prof.prof')
    s.strip_dirs().sort_stats(SortKey.TIME).print_stats(10)
    
    # Time comparison
    start = time.time()
    slow_distance(points)
    slow_time = time.time() - start
    
    start = time.time()
    optimized_distance(points)
    opt_time = time.time() - start
    
    print(f"\nTime comparison:")
    print(f"Slow version: {slow_time:.6f} seconds")
    print(f"Optimized version: {opt_time:.6f} seconds")
    print(f"Speedup: {slow_time / opt_time:.2f}x")

if __name__ == "__main__":
    profile_functions()
```

### Using Annotated HTML Output

To generate annotated HTML showing Python interaction:

```bash
cython -a profile_example.pyx
# Open profile_example.html in a browser
```

Example script to automate the process:

```python
# generate_annotations.py
import os
import subprocess
import webbrowser

def generate_html_annotation(filename):
    """Generate annotated HTML for a Cython file."""
    if not filename.endswith(('.pyx', '.py')):
        print(f"Error: {filename} is not a .pyx or .py file")
        return
    
    # Run Cython with annotation enabled
    cmd = ['cython', '-a', filename]
    print(f"Running: {' '.join(cmd)}")
    subprocess.run(cmd, check=True)
    
    # Get the HTML filename
    html_filename = os.path.splitext(filename)[0] + '.html'
    
    if os.path.exists(html_filename):
        print(f"Generated HTML annotation: {html_filename}")
        # Open in browser
        webbrowser.open(f'file://{os.path.abspath(html_filename)}')
    else:
        print(f"Error: Failed to generate HTML annotation")

if __name__ == "__main__":
    # Generate HTML for both examples
    generate_html_annotation('profile_example.pyx')
    generate_html_annotation('profile_example_pure.py')
```

## 2. Project Architecture

### Organizing Complex Cython Projects

**Project Structure Example:**

```
myproject/
├── setup.py                # Main setup file
├── pyproject.toml          # Build system requirements
├── myproject/
│   ├── __init__.py         # Package initialization
│   ├── _version.py         # Version information
│   ├── core/
│   │   ├── __init__.py
│   │   ├── _core.pyx       # Core functionality in Cython
│   │   ├── _core.pxd       # Declarations for sharing
│   │   └── utils.py        # Python utilities
│   ├── io/
│   │   ├── __init__.py
│   │   ├── _readers.pyx    # File readers in Cython
│   │   └── writers.py      # File writers in Python
│   └── algorithms/
│       ├── __init__.py
│       ├── _fast_algo.pyx  # Fast algorithms in Cython
│       └── helpers.py      # Helper functions in Python
└── tests/
    ├── test_core.py
    ├── test_io.py
    └── test_algorithms.py
```

**Example `setup.py`:**

```python
# setup.py
from setuptools import setup, find_packages, Extension
from Cython.Build import cythonize
import numpy as np

# Define extensions
extensions = [
    Extension(
        "myproject.core._core",
        ["myproject/core/_core.pyx"],
        include_dirs=[np.get_include()],
    ),
    Extension(
        "myproject.io._readers",
        ["myproject/io/_readers.pyx"],
    ),
    Extension(
        "myproject.algorithms._fast_algo",
        ["myproject/algorithms/_fast_algo.pyx"],
        include_dirs=[np.get_include()],
    ),
]

# Cython compiler directives
cython_directives = {
    'language_level': 3,
    'boundscheck': False,
    'wraparound': False,
    'initializedcheck': False,
    'cdivision': True,
}

setup(
    name="myproject",
    version="0.1.0",
    packages=find_packages(),
    ext_modules=cythonize(extensions, compiler_directives=cython_directives),
    install_requires=[
        "numpy>=1.20.0",
    ],
    python_requires=">=3.7",
)
```

**Example `pyproject.toml`:**

```toml
[build-system]
requires = ["setuptools>=42", "wheel", "Cython>=0.29.21", "numpy>=1.20.0"]
build-backend = "setuptools.build_meta"
```

### Sharing Declarations Between Cython Modules

**Implementation File (`_core.pyx`):**

```cython
# myproject/core/_core.pyx
import numpy as np
cimport numpy as cnp

from myproject.core._core cimport Point, distance, THRESHOLD

# Define constant from .pxd file
cdef double THRESHOLD = 1e-6

# Implement the Point class declared in .pxd
cdef class Point:
    def __init__(self, double x=0.0, double y=0.0, double z=0.0):
        self.x = x
        self.y = y
        self.z = z
    
    cpdef double distance_to(self, Point other):
        """Calculate distance to another point."""
        return distance(self, other)
    
    def __repr__(self):
        return f"Point({self.x}, {self.y}, {self.z})"

# Implement the distance function declared in .pxd
cdef double distance(Point p1, Point p2) nogil:
    """Calculate distance between two points."""
    cdef:
        double dx = p1.x - p2.x
        double dy = p1.y - p2.y
        double dz = p1.z - p2.z
    
    return sqrt(dx*dx + dy*dy + dz*dz)

# Python-callable wrapper
def py_create_point(double x, double y, double z):
    """Create a Point object."""
    return Point(x, y, z)

def py_distance(Point p1, Point p2):
    """Calculate distance between two points."""
    return distance(p1, p2)
```

**Declaration File (`_core.pxd`):**

```cython
# myproject/core/_core.pxd
from libc.math cimport sqrt

# Constant declaration
cdef double THRESHOLD

# Class declaration
cdef class Point:
    # Public attributes
    cdef public double x, y, z
    
    # Method declarations
    cpdef double distance_to(self, Point other)

# Function declaration
cdef double distance(Point p1, Point p2) nogil
```

**Additional Module Using Declarations (`_fast_algo.pyx`):**

```cython
# myproject/algorithms/_fast_algo.pyx
import numpy as np
cimport numpy as cnp

# Import declarations from core
from myproject.core._core cimport Point, distance, THRESHOLD

def nearest_neighbors(cnp.ndarray[cnp.double_t, ndim=2] points_array, int k=1):
    """Find k-nearest neighbors for each point."""
    cdef:
        int i, j, n = points_array.shape[0]
        list points = []
        list neighbors = []
        Point p1, p2
        double dist
    
    # Convert array to Point objects
    for i in range(n):
        points.append(Point(
            points_array[i, 0],
            points_array[i, 1],
            points_array[i, 2]
        ))
    
    # Find neighbors for each point
    for i in range(n):
        p1 = points[i]
        
        # Calculate all distances
        dists = []
        for j in range(n):
            if i != j:
                p2 = points[j]
                dist = distance(p1, p2)
                dists.append((dist, j))
        
        # Sort by distance and get k nearest
        dists.sort()
        neighbors.append([points[idx] for _, idx in dists[:k]])
    
    return neighbors

def filter_threshold(cnp.ndarray[cnp.double_t, ndim=2] points_array):
    """Filter points that are closer than THRESHOLD."""
    cdef:
        int i, j, n = points_array.shape[0]
        list points = []
        list result = []
        Point p1, p2
        bint too_close
    
    # Convert array to Point objects
    for i in range(n):
        points.append(Point(
            points_array[i, 0],
            points_array[i, 1],
            points_array[i, 2]
        ))
    
    # Filter points
    for i in range(n):
        p1 = points[i]
        too_close = False
        
        for j in range(i):
            p2 = points[j]
            if distance(p1, p2) < THRESHOLD:
                too_close = True
                break
        
        if not too_close:
            result.append(p1)
    
    return result
```

## 3. Advanced Applications

### Using Compiler Directives

**Using Directives in Source (`directives_example.pyx`):**

```cython
# directives_example.pyx
# cython: boundscheck=False, wraparound=False, cdivision=True

import numpy as np
cimport numpy as cnp
from libc.math cimport sqrt

def apply_filter(cnp.ndarray[cnp.double_t, ndim=2] image, 
                cnp.ndarray[cnp.double_t, ndim=2] kernel):
    """Apply a 2D filter to an image."""
    cdef:
        int i, j, ki, kj
        int height = image.shape[0]
        int width = image.shape[1]
        int k_height = kernel.shape[0]
        int k_width = kernel.shape[1]
        int k_radius_y = k_height // 2
        int k_radius_x = k_width // 2
        cnp.ndarray[cnp.double_t, ndim=2] result = np.zeros_like(image)
        double sum_val
    
    # Loop with compiler directives in effect
    for i in range(k_radius_y, height - k_radius_y):
        for j in range(k_radius_x, width - k_radius_x):
            sum_val = 0.0
            
            # Apply kernel
            for ki in range(k_height):
                for kj in range(k_width):
                    sum_val += (
                        image[i + ki - k_radius_y, j + kj - k_radius_x] * 
                        kernel[ki, kj]
                    )
            
            result[i, j] = sum_val
    
    return result

# Block with different directives
with cython.boundscheck(True), cython.wraparound(True):
    def safe_filter(cnp.ndarray[cnp.double_t, ndim=2] image, 
                  cnp.ndarray[cnp.double_t, ndim=2] kernel):
        """Apply a 2D filter with bounds checking."""
        cdef:
            int i, j, ki, kj
            int height = image.shape[0]
            int width = image.shape[1]
            int k_height = kernel.shape[0]
            int k_width = kernel.shape[1]
            int k_radius_y = k_height // 2
            int k_radius_x = k_width // 2
            cnp.ndarray[cnp.double_t, ndim=2] result = np.zeros_like(image)
            double sum_val
        
        # Loop with boundscheck and wraparound enabled
        for i in range(k_radius_y, height - k_radius_y):
            for j in range(k_radius_x, width - k_radius_x):
                sum_val = 0.0
                
                # Apply kernel
                for ki in range(k_height):
                    for kj in range(k_width):
                        sum_val += (
                            image[i + ki - k_radius_y, j + kj - k_radius_x] * 
                            kernel[ki, kj]
                        )
                
                result[i, j] = sum_val
        
        return result
```

**Using Directives in Pure Python Mode (`directives_example_pure.py`):**

```python
# directives_example_pure.py
# cython: language_level=3
import cython
import numpy as np

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.cdivision(True)
def apply_filter(image, kernel):
    """Apply a 2D filter to an image."""
    height = image.shape[0]
    width = image.shape[1]
    k_height = kernel.shape[0]
    k_width = kernel.shape[1]
    k_radius_y = k_height // 2
    k_radius_x = k_width // 2
    result = np.zeros_like(image)
    
    # Loop with compiler directives in effect
    for i in range(k_radius_y, height - k_radius_y):
        for j in range(k_radius_x, width - k_radius_x):
            sum_val = 0.0
            
            # Apply kernel
            for ki in range(k_height):
                for kj in range(k_width):
                    sum_val += (
                        image[i + ki - k_radius_y, j + kj - k_radius_x] * 
                        kernel[ki, kj]
                    )
            
            result[i, j] = sum_val
    
    return result

@cython.boundscheck(True)
@cython.wraparound(True)
def safe_filter(image, kernel):
    """Apply a 2D filter with bounds checking."""
    height = image.shape[0]
    width = image.shape[1]
    k_height = kernel.shape[0]
    k_width = kernel.shape[1]
    k_radius_y = k_height // 2
    k_radius_x = k_width // 2
    result = np.zeros_like(image)
    
    # Loop with boundscheck and wraparound enabled
    for i in range(k_radius_y, height - k_radius_y):
        for j in range(k_radius_x, width - k_radius_x):
            sum_val = 0.0
            
            # Apply kernel
            for ki in range(k_height):
                for kj in range(k_width):
                    sum_val += (
                        image[i + ki - k_radius_y, j + kj - k_radius_x] * 
                        kernel[ki, kj]
                    )
            
            result[i, j] = sum_val
    
    return result
```

### Creating Python Extensions in Pure C

Here's a simplified example of creating a Python extension module using Cython to generate C code:

**Cython Source (`pure_c_extension.pyx`):**

```cython
# pure_c_extension.pyx
"""Module that will be compiled to pure C."""
# distutils: language=c

from libc.stdlib cimport malloc, free
from libc.string cimport memcpy, memset

cdef struct Array:
    double* data
    int size

cdef Array* create_array(int size) nogil:
    """Create a new array struct."""
    cdef Array* arr = <Array*>malloc(sizeof(Array))
    
    if arr == NULL:
        return NULL
    
    arr.size = size
    arr.data = <double*>malloc(size * sizeof(double))
    
    if arr.data == NULL:
        free(arr)
        return NULL
    
    # Initialize to zeros
    memset(arr.data, 0, size * sizeof(double))
    
    return arr

cdef void free_array(Array* arr) nogil:
    """Free array memory."""
    if arr != NULL:
        if arr.data != NULL:
            free(arr.data)
        free(arr)

def sum_of_squares(double[:] data):
    """Calculate sum of squares using a C array."""
    cdef:
        int i, n = data.shape[0]
        Array* arr
        double result = 0.0
    
    # Create C array
    arr = create_array(n)
    if arr == NULL:
        raise MemoryError("Failed to allocate memory")
    
    try:
        # Copy data and square each element
        for i in range(n):
            arr.data[i] = data[i] * data[i]
        
        # Sum the squared values
        for i in range(n):
            result += arr.data[i]
        
        return result
    finally:
        # Always free the memory
        free_array(arr)
```

**Setup Script for Pure C Extension:**

```python
# setup_pure_c.py
from setuptools import setup, Extension
from Cython.Build import cythonize

ext_modules = [
    Extension(
        "pure_c_extension",
        ["pure_c_extension.pyx"],
        language="c",  # Generate C, not C++
    )
]

setup(
    name="PureCExtension",
    ext_modules=cythonize(ext_modules, language_level=3),
)
```

### Debugging Complex Cython Applications

**Cython Source with Debug Features (`debug_example.pyx`):**

```cython
# debug_example.pyx
# cython: language_level=3
# cython: linetrace=True
# distutils: define_macros=CYTHON_TRACE=1,CYTHON_TRACE_NOGIL=1

import numpy as np
cimport numpy as cnp
from libc.math cimport sqrt, log, exp
import cython

# Debug logging
def debug_log(str message):
    """Print a debug message."""
    print(f"DEBUG: {message}")

# Function with assertions
def process_data(cnp.ndarray[cnp.double_t, ndim=1] data):
    """Process data with assertions for debugging."""
    cdef:
        int i, n = data.shape[0]
        double total = 0.0
        double mean, stddev
    
    # Check input
    assert data.ndim == 1, "Data must be 1-dimensional"
    assert n > 0, "Data array cannot be empty"
    
    # Calculate sum with checking
    debug_log(f"Processing array of length {n}")
    for i in range(n):
        # Check for NaN/Inf
        assert cython.isfinite(data[i]), f"Invalid value at index {i}: {data[i]}"
        total += data[i]
    
    # Calculate mean
    mean = total / n
    debug_log(f"Mean value: {mean}")
    
    # Calculate standard deviation
    total = 0.0
    for i in range(n):
        total += (data[i] - mean) ** 2
    
    stddev = sqrt(total / n)
    debug_log(f"Standard deviation: {stddev}")
    
    # Normalize data (conditional debug)
    if stddev < 1e-10:
        debug_log("WARNING: Very small standard deviation, normalization may be unstable")
    
    return (data - mean) / max(stddev, 1e-10)

# Function that might cause errors
def risky_function(cnp.ndarray[cnp.double_t, ndim=1] data):
    """Function that might cause errors, with debug information."""
    cdef:
        int i, n = data.shape[0]
        double result = 0.0
    
    debug_log(f"Entering risky function with {n} elements")
    
    try:
        for i in range(n):
            if data[i] == 0:
                debug_log(f"Division by zero at index {i}")
                raise ZeroDivisionError(f"Division by zero at index {i}")
            
            if data[i] < 0:
                debug_log(f"Negative value at index {i}: {data[i]}")
                raise ValueError(f"Cannot calculate log of negative value: {data[i]}")
            
            result += log(data[i])
        
        debug_log("Successfully processed all data")
        return result
    except Exception as e:
        debug_log(f"Error in risky_function: {str(e)}")
        raise
```

**Debugging Script:**

```python
# test_debugging.py
import numpy as np
from debug_example import process_data, risky_function

def test_debug_features():
    """Test the debugging features."""
    print("\n=== Testing debug features ===")
    
    # Create test data
    np.random.seed(42)
    normal_data = np.random.normal(5.0, 2.0, 1000)
    bad_data = np.array([1.0, 2.0, 0.0, 4.0, 5.0])
    negative_data = np.array([1.0, 2.0, -3.0, 4.0, 5.0])
    
    # Test process_data with normal data
    print("\nTesting process_data with normal data:")
    try:
        result = process_data(normal_data)
        print(f"Processed successfully, first 5 results: {result[:5]}")
    except Exception as e:
        print(f"Error: {e}")
    
    # Test risky_function with normal data
    print("\nTesting risky_function with normal data:")
    try:
        result = risky_function(np.abs(normal_data[:10]))
        print(f"Processed successfully, result: {result}")
    except Exception as e:
        print(f"Error: {e}")
    
    # Test risky_function with zero value
    print("\nTesting risky_function with zero value:")
    try:
        result = risky_function(bad_data)
        print(f"Processed successfully, result: {result}")
    except Exception as e:
        print(f"Error: {e}")
    
    # Test risky_function with negative value
    print("\nTesting risky_function with negative value:")
    try:
        result = risky_function(negative_data)
        print(f"Processed successfully, result: {result}")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    test_debug_features()
```

## Summary of Phase 4 Best Practices

### Performance Tuning

1. **Profiling Cython Code**
   - Always profile before optimizing
   - Use `cProfile` for function-level profiling
   - Enable Cython profiling with `# cython: profile=True`
   - Focus on the bottlenecks identified by profiling

2. **Using Annotated HTML Output**
   - Generate with `cython -a file.pyx`
   - Yellow highlights indicate Python interaction (slower)
   - White background indicates pure C code (faster)
   - Use to identify where Python overhead exists

3. **Optimization Strategies**
   - Avoid redundant calculations
   - Minimize Python interactions
   - Use algorithm improvements (e.g., symmetric calculations)
   - Consider memory layout and access patterns

### Project Architecture

1. **Organizing Cython Projects**
   - Use `.pyx` for implementation files
   - Use `.pxd` for declarations shared between modules
   - Organize by functionality (core, io, algorithms, etc.)
   - Mix Python and Cython modules as appropriate

2. **Setup.py Configuration**
   - Use `cythonize()` with compiler directives
   - Group related extensions
   - Specify include directories for external dependencies
   - Define global compiler directives for consistency

3. **Managing Shared Declarations**
   - Define shared constants, functions and classes in `.pxd` files
   - Import declarations with `cimport`
   - Implement in corresponding `.pyx` files
   - Use for cross-module optimization

### Advanced Applications

1. **Compiler Directives**
   - File-level: `# cython: directive=value`
   - Block-level: `with cython.directive(value):`
   - Function-level: `@cython.directive(value)`
   - Common directives:
     - `boundscheck`: Disable bounds checking for speed
     - `wraparound`: Disable negative indexing
     - `cdivision`: Disable division-by-zero checking
     - `initializedcheck`: Disable memoryview initialized checking

2. **Pure C Extensions**
   - Use `# distutils: language=c` for C (not C++)
   - Implement with C data structures
   - Manage memory carefully (malloc/free)
   - Create clean Python interfaces

3. **Debugging Techniques**
   - Enable line tracing with `# cython: linetrace=True`
   - Use assertions to catch errors early
   - Add debug logging functions
   - Use try/except for error handling
   - Create Python-friendly error messages

By mastering these Phase 4 concepts, you can create efficient, maintainable, and robust Cython code that performs at near-C speeds while maintaining Python's ease of use and integration capabilities.

---
