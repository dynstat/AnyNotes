
# Understanding Cython with Practical Examples

Cython is a programming language that combines Python with the power and performance of C. It allows you to write Python code with optional static type declarations that get compiled to efficient C extensions, giving you significant performance improvements.

## 1. Getting Started with Cython

### Installation

First, let's install Cython:

```bash
pip install cython
```

### Hello World Example

Let's start with a simple example. Create a file named `hello.pyx`:

```python
# hello.pyx
def say_hello(name):
    print(f"Hello, {name}!")
    
def say_hello_typed(str name):
    print(f"Hello, {name}!")
```

Notice the second function has type declaration `str name`, which is a Cython-specific syntax.

### Setting up with setup.py

Create a `setup.py` file to compile your Cython code:

```python
# setup.py
from setuptools import setup
from Cython.Build import cythonize

setup(
    name="Hello World App",
    ext_modules=cythonize("hello.pyx"),
)
```

Compile the code:

```bash
python setup.py build_ext --inplace
```

This will create a shared library file (e.g., `hello.cp39-win_amd64.pyd` on Windows or `hello.cpython-39-x86_64-linux-gnu.so` on Linux) that you can import in Python.

Use it in Python:

```python
# test.py
import hello

hello.say_hello("World")
hello.say_hello_typed("Cython")
```

## 2. Understanding setup.py in Detail

The `setup.py` file is central to building Cython extensions. Let's break down its components:

```python
from setuptools import setup, Extension
from Cython.Build import cythonize
import numpy as np  # If you need NumPy integration

# For a single file
setup(
    name="MyProject",
    ext_modules=cythonize("mymodule.pyx"),
)

# For multiple files with additional options
extensions = [
    Extension(
        "fast_math",  # Name of the extension (import name)
        ["fast_math.pyx"],  # Source files
        include_dirs=[np.get_include()],  # Include directories (e.g., for NumPy)
        libraries=["m"],  # Link with these libraries
        extra_compile_args=["-O3"],  # Compiler optimizations
    ),
    Extension(
        "utils", 
        ["utils.pyx"],
    )
]

setup(
    name="MyComplexProject",
    ext_modules=cythonize(
        extensions,
        compiler_directives={
            "language_level": 3,  # Use Python 3 syntax
            "boundscheck": False,  # Disable bounds checking for better performance
            "wraparound": False,   # Disable negative indexing for better performance
        }
    ),
    install_requires=["numpy"],
)
```

### Key Components of setup.py:

1. **`ext_modules`**: Defines the Cython extension modules to build
2. **`cythonize()`**: Turns Cython code into C and handles compilation
3. **`Extension`**: Specifies compilation details for more complex builds
4. **`compiler_directives`**: Controls Cython compilation behavior

## 3. Static Typing for Performance

Let's create a practical example showing the power of static typing in Cython. We'll implement a function to calculate the Fibonacci sequence:

```python
# fibonacci.pyx

# Python function (no type declarations)
def fib_py(n):
    a, b = 0, 1
    for i in range(n):
        a, b = a + b, a
    return a

# Cython function with type declarations
def fib_cy(int n):
    cdef int a = 0
    cdef int b = 1
    cdef int i
    
    for i in range(n):
        a, b = a + b, a
    return a

# Even more optimized Cython function
cpdef int fib_opt(int n):
    cdef int a = 0
    cdef int b = 1
    cdef int i
    cdef int temp
    
    for i in range(n):
        temp = a
        a = a + b
        b = temp
    return a
```

Setup file:

```python
# setup.py
from setuptools import setup
from Cython.Build import cythonize

setup(
    name="Fibonacci",
    ext_modules=cythonize(
        "fibonacci.pyx",
        compiler_directives={"language_level": 3}
    ),
)
```

Benchmarking script:

```python
# benchmark.py
import time
import fibonacci

def benchmark(func, n, iterations=100):
    start = time.time()
    for _ in range(iterations):
        result = func(n)
    end = time.time()
    return (end - start) / iterations

n = 1000000
py_time = benchmark(fibonacci.fib_py, n, 3)  # Python version will be slow
cy_time = benchmark(fibonacci.fib_cy, n)
opt_time = benchmark(fibonacci.fib_opt, n)

print(f"Python version: {py_time:.6f} seconds per call")
print(f"Cython version: {cy_time:.6f} seconds per call")
print(f"Optimized version: {opt_time:.6f} seconds per call")
print(f"Speedup (Python vs. Optimized): {py_time/opt_time:.1f}x")
```

## 4. Working with C Arrays and Memory

Let's implement a simple array summation function using C arrays for better performance:

```python
# array_utils.pyx
import numpy as np
cimport numpy as np

# Pure Python implementation
def sum_array_py(arr):
    total = 0
    for value in arr:
        total += value
    return total

# Cython with typed NumPy array
def sum_array_np(np.ndarray[np.float64_t, ndim=1] arr):
    cdef double total = 0
    cdef int i
    cdef int n = arr.shape[0]
    
    for i in range(n):
        total += arr[i]
    return total

# Cython with C array (using memory view)
def sum_array_c(double[:] arr):
    cdef double total = 0
    cdef int i
    cdef int n = arr.shape[0]
    
    for i in range(n):
        total += arr[i]
    return total
```

Setup file:

```python
# setup.py
import numpy
from setuptools import setup
from Cython.Build import cythonize

setup(
    name="ArrayUtils",
    ext_modules=cythonize(
        "array_utils.pyx",
        compiler_directives={"language_level": 3}
    ),
    include_dirs=[numpy.get_include()]
)
```

Usage:

```python
# test_arrays.py
import numpy as np
import array_utils

# Create a large array for testing
test_array = np.random.random(10000000)

# Compare the sum functions
python_sum = array_utils.sum_array_py(test_array)
numpy_sum = array_utils.sum_array_np(test_array)
c_sum = array_utils.sum_array_c(test_array)
builtin_sum = np.sum(test_array)

print(f"Python sum: {python_sum}")
print(f"Cython NumPy sum: {numpy_sum}")
print(f"Cython C array sum: {c_sum}")
print(f"NumPy builtin sum: {builtin_sum}")
```

## 5. Creating C Extension Types (cdef classes)

Extension types are Cython's version of classes that compile to C structs:

```python
# point.pyx
cdef class Point:
    # Attributes must be declared
    cdef public double x
    cdef public double y
    cdef double _norm  # Private attribute
    cdef bint _norm_valid  # Boolean flag to track if norm is calculated
    
    def __init__(self, double x=0.0, double y=0.0):
        self.x = x
        self.y = y
        self._norm_valid = False
    
    cpdef double norm(self) except -1:
        """Calculate the Euclidean norm (distance from origin)"""
        if not self._norm_valid:
            self._norm = (self.x**2 + self.y**2)**0.5
            self._norm_valid = True
        return self._norm
    
    def __repr__(self):
        return f"Point({self.x}, {self.y})"
    
    # When attribute changes, norm needs recalculation
    def __setattr__(self, name, value):
        if name in ('x', 'y'):
            self._norm_valid = False
        super().__setattr__(name, value)
    
    # Fast method for point addition
    cpdef Point add(self, Point other):
        return Point(self.x + other.x, self.y + other.y)
```

Setup file:

```python
# setup.py
from setuptools import setup
from Cython.Build import cythonize

setup(
    name="Point",
    ext_modules=cythonize(
        "point.pyx",
        compiler_directives={"language_level": 3}
    ),
)
```

Usage:

```python
# test_point.py
import point

p1 = point.Point(3, 4)
p2 = point.Point(1, 2)

print(f"p1: {p1}")
print(f"p2: {p2}")
print(f"Distance of p1 from origin: {p1.norm()}")

p3 = p1.add(p2)
print(f"p1 + p2 = {p3}")
print(f"Distance of p3 from origin: {p3.norm()}")

# Change attribute and see norm recalculated
p1.x = 0
print(f"After changing p1.x to 0, norm = {p1.norm()}")
```

## 6. Interfacing with C Libraries

One of Cython's strongest features is its ability to easily interface with C libraries. Let's create an example using the C standard library's math functions:

```python
# c_math.pyx
# Import the C math library
cdef extern from "math.h":
    double sin(double x)
    double cos(double x)
    double sqrt(double x)
    double M_PI

# Expose the functions to Python
def py_sin(x):
    return sin(x)

def py_cos(x):
    return cos(x)

def py_sqrt(x):
    return sqrt(x)

# Get the value of π
def get_pi():
    return M_PI

# A function that uses multiple C functions
def compute_distance(double x1, double y1, double x2, double y2):
    cdef double dx = x2 - x1
    cdef double dy = y2 - y1
    return sqrt(dx*dx + dy*dy)

# A more complex example calculating a circular arc length
def arc_length(double radius, double angle_degrees):
    cdef double angle_radians = angle_degrees * M_PI / 180.0
    return radius * angle_radians
```

Setup file:

```python
# setup.py
from setuptools import setup, Extension
from Cython.Build import cythonize

ext_modules = [
    Extension(
        "c_math",
        ["c_math.pyx"],
        libraries=["m"],  # Link against the math library
    )
]

setup(
    name="C Math Wrapper",
    ext_modules=cythonize(
        ext_modules,
        compiler_directives={"language_level": 3}
    ),
)
```

Usage:

```python
# test_c_math.py
import math
import c_math

# Compare with Python's math functions
x = 0.5
print(f"Python sin({x}): {math.sin(x)}")
print(f"Cython sin({x}): {c_math.py_sin(x)}")

print(f"Python π: {math.pi}")
print(f"C math.h π: {c_math.get_pi()}")

# Calculate distance between two points
p1 = (1, 2)
p2 = (4, 6)
print(f"Distance between {p1} and {p2}: {c_math.compute_distance(1, 2, 4, 6)}")

# Calculate arc length
radius = 5
angle = 45  # degrees
print(f"Arc length for radius {radius} and angle {angle}°: {c_math.arc_length(radius, angle)}")
```

## 7. Parallel Processing with OpenMP

Cython can leverage OpenMP for parallel processing:

```python
# parallel.pyx
import numpy as np
cimport numpy as np
from cython.parallel import prange

def parallel_sum(double[:] arr):
    cdef int i
    cdef int n = arr.shape[0]
    cdef double total = 0.0
    
    # Use OpenMP for parallel computation
    # The nogil means we release the Global Interpreter Lock
    # which is necessary for true parallelism
    for i in prange(n, nogil=True):
        total += arr[i]
    
    return total

def parallel_vector_add(double[:] a, double[:] b, double[:] result):
    cdef int i
    cdef int n = a.shape[0]
    
    # Add vectors in parallel
    for i in prange(n, nogil=True):
        result[i] = a[i] + b[i]
```

Setup file with OpenMP flags:

```python
# setup.py
import numpy
from setuptools import setup, Extension
from Cython.Build import cythonize

# Extra compile arguments for enabling OpenMP
# Note: For MSVC on Windows, use '/openmp' instead of '-fopenmp'
ext_modules = [
    Extension(
        "parallel",
        ["parallel.pyx"],
        extra_compile_args=['-fopenmp'],
        extra_link_args=['-fopenmp'],
    )
]

setup(
    name="Parallel",
    ext_modules=cythonize(
        ext_modules,
        compiler_directives={"language_level": 3}
    ),
    include_dirs=[numpy.get_include()]
)
```

Usage:

```python
# test_parallel.py
import numpy as np
import parallel
import time

# Test with large arrays
size = 50000000
a = np.random.random(size)
b = np.random.random(size)
result = np.zeros(size)

# Time the parallel sum
start = time.time()
parallel_sum = parallel.parallel_sum(a)
end = time.time()
print(f"Parallel sum: {parallel_sum}")
print(f"Time taken: {end - start:.4f} seconds")

# Compare with numpy's sum
start = time.time()
numpy_sum = np.sum(a)
end = time.time()
print(f"NumPy sum: {numpy_sum}")
print(f"Time taken: {end - start:.4f} seconds")

# Test vector addition
start = time.time()
parallel.parallel_vector_add(a, b, result)
end = time.time()
print(f"Time for parallel vector addition: {end - start:.4f} seconds")

# Compare with NumPy's vector addition
start = time.time()
numpy_result = a + b
end = time.time()
print(f"Time for NumPy vector addition: {end - start:.4f} seconds")

# Verify the results are the same
print(f"Results match: {np.allclose(result, numpy_result)}")
```

## 8. Wrapping C++ Classes

Cython can also wrap C++ classes:

```python
# rectangle.pyx
# Declare the C++ Rectangle class interface
cdef extern from "rectangle.hpp" namespace "shapes":
    cdef cppclass Rectangle:
        Rectangle() except +
        Rectangle(int w, int h) except +
        int x
        int y
        int width
        int height
        int getArea()
        void setLocation(int x, int y)

# Python wrapper class
cdef class PyRectangle:
    cdef Rectangle *thisptr  # Hold a C++ instance
    
    def __cinit__(self, int width=0, int height=0):
        self.thisptr = new Rectangle(width, height)
    
    def __dealloc__(self):
        del self.thisptr
    
    @property
    def width(self):
        return self.thisptr.width
    
    @width.setter
    def width(self, int value):
        self.thisptr.width = value
    
    @property
    def height(self):
        return self.thisptr.height
    
    @height.setter
    def height(self, int value):
        self.thisptr.height = value
    
    @property
    def x(self):
        return self.thisptr.x
    
    @property
    def y(self):
        return self.thisptr.y
    
    def get_area(self):
        return self.thisptr.getArea()
    
    def set_location(self, int x, int y):
        self.thisptr.setLocation(x, y)
```

C++ header file (rectangle.hpp):

```cpp
// rectangle.hpp
#ifndef RECTANGLE_H
#define RECTANGLE_H

namespace shapes {
    class Rectangle {
    public:
        int x, y, width, height;
        
        Rectangle() : x(0), y(0), width(0), height(0) {}
        
        Rectangle(int w, int h) : x(0), y(0), width(w), height(h) {}
        
        int getArea() {
            return width * height;
        }
        
        void setLocation(int x_, int y_) {
            x = x_;
            y = y_;
        }
    };
}

#endif
```

Setup file for C++ compilation:

```python
# setup.py
from setuptools import setup, Extension
from Cython.Build import cythonize

extensions = [
    Extension(
        "rectangle",
        ["rectangle.pyx"],
        language="c++",  # Enable C++ compilation
    )
]

setup(
    name="Rectangle",
    ext_modules=cythonize(
        extensions,
        compiler_directives={"language_level": 3}
    ),
)
```

Usage:

```python
# test_rectangle.py
import rectangle

# Create a rectangle
rect = rectangle.PyRectangle(10, 5)
print(f"Rectangle dimensions: {rect.width}x{rect.height}")
print(f"Rectangle area: {rect.get_area()}")
print(f"Rectangle position: ({rect.x}, {rect.y})")

# Change the rectangle
rect.width = 15
rect.height = 8
rect.set_location(10, 20)
print(f"Updated rectangle: {rect.width}x{rect.height} at ({rect.x}, {rect.y})")
print(f"Updated area: {rect.get_area()}")
```

## 9. Using Pure Python Mode

Cython also offers a "pure Python" mode where you can write Cython code in `.py` files:

```python
# fast_math.py
# cython: language_level=3
# distutils: language=c

import cython

@cython.cfunc
def _factorial(n: cython.int) -> cython.int:
    """Calculate factorial using C function"""
    result: cython.int = 1
    i: cython.int
    for i in range(2, n + 1):
        result *= i
    return result

@cython.ccall
def factorial(n: cython.int) -> cython.int:
    """Public function to calculate factorial"""
    if n < 0:
        raise ValueError("Factorial not defined for negative numbers")
    if n <= 1:
        return 1
    return _factorial(n)

@cython.ccall
def binomial_coefficient(n: cython.int, k: cython.int) -> cython.int:
    """Calculate binomial coefficient (n choose k)"""
    if k < 0 or k > n:
        return 0
    return _factorial(n) // (_factorial(k) * _factorial(n - k))
```

Setup file:

```python
# setup.py
from setuptools import setup
from Cython.Build import cythonize

setup(
    name="FastMath",
    ext_modules=cythonize(
        "fast_math.py",  # Note .py extension, not .pyx
    ),
)
```

Usage:

```python
# test_pure_python.py
import fast_math
import math
import time

def benchmark(func, n, iterations=100000):
    start = time.time()
    for _ in range(iterations):
        result = func(n)
    end = time.time()
    return (end - start) / iterations

# Test factorial calculations
n = 10
cython_time = benchmark(fast_math.factorial, n)
python_time = benchmark(math.factorial, n)

print(f"Cython factorial({n}) = {fast_math.factorial(n)}")
print(f"Python factorial({n}) = {math.factorial(n)}")
print(f"Cython time: {cython_time:.9f} seconds per call")
print(f"Python time: {python_time:.9f} seconds per call")
print(f"Speedup: {python_time/cython_time:.2f}x")

# Test binomial coefficient
n, k = 20, 10
print(f"Binomial coefficient ({n} choose {k}) = {fast_math.binomial_coefficient(n, k)}")
```

## Conclusion

This guide has covered the core concepts of Cython, showing how to:

1. Create and compile basic Cython modules
2. Configure `setup.py` files for different use cases
3. Leverage static typing for performance
4. Work efficiently with arrays and memory
5. Create extension types (cdef classes)
6. Interface with C libraries
7. Implement parallel processing
8. Wrap C++ classes
9. Use Cython's pure Python mode

Cython's ability to combine Python's ease of use with C's performance makes it an excellent tool for optimizing performance-critical parts of your Python applications.

---
---
