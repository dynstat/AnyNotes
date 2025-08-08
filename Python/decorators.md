
## 1. **Simple Function Decorator (No Parameters)**

A basic decorator is just a function that **wraps another function** to modify or extend its behavior.

```python
# Step 1: Define a simple decorator
def my_decorator(func):
    # func → the function we want to wrap
    
    def wrapper():
        # Code before calling the original function
        print("Before the function runs")
        
        func()  # Calling the original function
        
        # Code after calling the original function
        print("After the function runs")
    
    return wrapper  # Returning the wrapper function

# Step 2: Use the decorator
@my_decorator
def say_hello():
    print("Hello World!")

# Step 3: Call the decorated function
say_hello()
```

**Output:**

```
Before the function runs
Hello World!
After the function runs
```

**Use Case:**
Logging, timing, authentication checks, etc.

---

## 2. **Function Decorator That Accepts Arguments in the Target Function**

```python
def debug_decorator(func):
    def wrapper(*args, **kwargs):
        print(f"Calling {func.__name__} with {args} {kwargs}")
        result = func(*args, **kwargs)  # Pass arguments through
        print(f"{func.__name__} returned {result}")
        return result
    return wrapper

@debug_decorator
def add(a, b):
    return a + b

add(5, 3)
```

**Output:**

```
Calling add with (5, 3) {}
add returned 8
```

**Key:**
`*args` and `**kwargs` allow the decorator to work with **any function signature**.

---

## 3. **Parameterized Decorator (Decorator with Its Own Arguments)**

A **decorator factory** returns the actual decorator.

```python
def repeat(times):
    # times → number of repetitions
    def decorator(func):
        def wrapper(*args, **kwargs):
            for i in range(times):
                print(f"Run {i+1}/{times}")
                func(*args, **kwargs)
        return wrapper
    return decorator

@repeat(3)  # Here, repeat(3) returns decorator
def greet(name):
    print(f"Hello, {name}!")

greet("Vivek")
```

**Output:**

```
Run 1/3
Hello, Vivek!
Run 2/3
Hello, Vivek!
Run 3/3
Hello, Vivek!
```

---

## 4. **Preserving Function Metadata with `functools.wraps`**

Without `functools.wraps`, decorators replace the `__name__` and `__doc__` of the original function.

```python
import functools

def safe_decorator(func):
    @functools.wraps(func)  # Keeps name & docstring of original function
    def wrapper(*args, **kwargs):
        return func(*args, **kwargs)
    return wrapper

@safe_decorator
def process():
    """This function processes data."""
    pass

print(process.__name__)  # process
print(process.__doc__)   # This function processes data.
```

---

## 5. **Stacked Decorators (Multiple Decorators)**

Decorators are applied **from bottom to top**.

```python
def bold(func):
    def wrapper():
        return f"<b>{func()}</b>"
    return wrapper

def italic(func):
    def wrapper():
        return f"<i>{func()}</i>"
    return wrapper

@bold
@italic
def text():
    return "Hello"

print(text())  # <b><i>Hello</i></b>
```

**Order:**
`text → italic → bold`

---

## 6. **Decorator for Methods (Instance Methods)**

```python
def method_logger(func):
    def wrapper(self, *args, **kwargs):
        print(f"Method {func.__name__} called with {args}")
        return func(self, *args, **kwargs)
    return wrapper

class Person:
    def __init__(self, name):
        self.name = name

    @method_logger
    def say_hi(self, age):
        print(f"Hi, I'm {self.name}, {age} years old.")

p = Person("Vivek")
p.say_hi(30)
```

---

## 7. **Class-Based Decorator**

When you need **state persistence** across calls, a class can be used as a decorator.

```python
class CallCounter:
    def __init__(self, func):
        self.func = func
        self.count = 0  # State

    def __call__(self, *args, **kwargs):
        self.count += 1
        print(f"Call {self.count}")
        return self.func(*args, **kwargs)

@CallCounter
def hello():
    print("Hello!")

hello()
hello()
```

---

## 8. **Decorator with Parameters + Class Implementation**

```python
class Repeat:
    def __init__(self, times):
        self.times = times

    def __call__(self, func):
        def wrapper(*args, **kwargs):
            for _ in range(self.times):
                func(*args, **kwargs)
        return wrapper

@Repeat(2)
def ping():
    print("Ping!")

ping()
```

---

## 9. **Practical Example: Timing a Function**

```python
import time
import functools

def timing(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        start = time.perf_counter()
        result = func(*args, **kwargs)
        end = time.perf_counter()
        print(f"{func.__name__} took {end - start:.4f} seconds")
        return result
    return wrapper

@timing
def slow_function():
    time.sleep(1)
    print("Done.")

slow_function()
```

---

## 10. **Decorators That Modify Return Values**

```python
def uppercase(func):
    def wrapper(*args, **kwargs):
        return func(*args, **kwargs).upper()
    return wrapper

@uppercase
def get_message():
    return "hello"

print(get_message())  # HELLO
```

---

## 11. **Decorating Built-in / Library Functions (Monkey Patching Style)**

```python
import math

def log_math(func):
    def wrapper(*args, **kwargs):
        print(f"Math function {func.__name__} with {args}")
        return func(*args, **kwargs)
    return wrapper

math.sqrt = log_math(math.sqrt)
math.sqrt(16)
```

---

## 12. **Decorator for Async Functions**

```python
import asyncio
import functools

def async_logger(func):
    @functools.wraps(func)
    async def wrapper(*args, **kwargs):
        print(f"Running async function {func.__name__}")
        return await func(*args, **kwargs)
    return wrapper

@async_logger
async def async_task():
    await asyncio.sleep(1)
    print("Async task done!")

asyncio.run(async_task())
```

---

## **Decorator Types Overview Table**

| Decorator Type     | Purpose                    | Example Use Case                |
| ------------------ | -------------------------- | ------------------------------- |
| Simple Function    | Add extra behavior         | Logging before & after function |
| With Function Args | Works with any function    | Debugging function calls        |
| Parameterized      | Pass settings to decorator | Retry count, repeat             |
| Class-Based        | Maintain state             | Call counters                   |
| Method Decorator   | For instance methods       | Method logging                  |
| Async Decorator    | Works with async functions | Async logging, retries          |
| Stacked            | Combine multiple behaviors | Formatting HTML                 |

---


Alright — now we take the **other scenario**:

* **Decorator has NO parameters**
* **Target function IS parameterized**

This is basically your **`CallCounter`** type case.

---

## **Example: Class Decorator Without Parameters, Function With Parameters**

```python
class CallCounter:
    def __init__(self, func):
        # Here, func IS the target function
        print(f"[__init__] Got function: {func.__name__}")
        self.func = func
        self.count = 0

    def __call__(self, *args, **kwargs):
        # args & kwargs are the parameters passed to the target function
        self.count += 1
        print(f"[__call__] Call #{self.count} with args={args}, kwargs={kwargs}")
        return self.func(*args, **kwargs)


@CallCounter  # No params for decorator
def greet(name, age):  # Target function has parameters
    print(f"Hello {name}, you are {age} years old.")
```

---

### **What Python does internally**

```python
greet = CallCounter(greet)  # __init__(self, func=greet) is called here
```

* Since the decorator has **no `()`**, Python **passes the function directly** into `__init__`.

---

### **Now run it**

```python
greet("Vivek", 30)
greet("Anya", 25)
```

---

### **Output**

```
[__init__] Got function: greet
[__call__] Call #1 with args=('Vivek', 30), kwargs={}
Hello Vivek, you are 30 years old.
[__call__] Call #2 with args=('Anya', 25), kwargs={}
Hello Anya, you are 25 years old.
```

---

## **Parameter Flow Table – No Decorator Params**

| Stage               | Method                            | What is Received                                   |
| ------------------- | --------------------------------- | -------------------------------------------------- |
| Decorating function | `__init__(self, func)`            | The function object itself                         |
| Function execution  | `__call__(self, *args, **kwargs)` | The **runtime parameters** for the target function |

---

## **Execution Diagram**

```
@CallCounter
def greet(name, age):
    ...

      ↓
greet = CallCounter(greet)
        ↑
     __init__ gets function

greet("Vivek", 30)
   ↑
 __call__ gets ("Vivek", 30)
 runs original function
```

---

✅ **Rule of Thumb**:

* If decorator has **no params** → `__init__` gets the **function**.
* If decorator has **params** → `__init__` gets **those params**, and `__call__` gets the **function**.

---
Got it —
you’re asking: **What happens if BOTH**

* the **decorator itself takes parameters**
* the **target function also takes parameters**

…and how `__init__` and `__call__` receive them.

---

## **Step-by-Step Explanation**

Let’s build a case that has **both sets of parameters**.

```python
class Repeat:
    def __init__(self, times):  
        # This is decorator's own parameter
        print(f"[__init__] Got decorator param times={times}")
        self.times = times

    def __call__(self, func):
        # This is called with the target function
        print(f"[__call__] Got function {func.__name__}")

        def wrapper(*args, **kwargs):
            # These are the target function's parameters
            print(f"[wrapper] Got function params args={args}, kwargs={kwargs}")
            for i in range(self.times):
                print(f"Run {i+1}/{self.times}")
                func(*args, **kwargs)
        return wrapper


@Repeat(3)   # Decorator parameter: times=3
def greet(name, age):  # Target function parameters: name, age
    print(f"Hello {name}, you are {age} years old.")
```

---

### **What Python does internally**

```python
temp = Repeat(3)      # Calls __init__(self, times=3)
greet = temp(greet)   # Calls __call__(self, func=greet)
```

---

### **Now run it**

```python
greet("Vivek", 30)
```

---

### **Output**

```
[__init__] Got decorator param times=3
[__call__] Got function greet
[wrapper] Got function params args=('Vivek', 30), kwargs={}
Run 1/3
Hello Vivek, you are 30 years old.
Run 2/3
Hello Vivek, you are 30 years old.
Run 3/3
Hello Vivek, you are 30 years old.
```

---

## **Parameter Flow Table**

| Stage               | Method                     | What is Received                        |
| ------------------- | -------------------------- | --------------------------------------- |
| Decorator creation  | `__init__(self, times)`    | Parameters for **the decorator**        |
| Decorating function | `__call__(self, func)`     | The **function object** being decorated |
| Function execution  | `wrapper(*args, **kwargs)` | Parameters for **the target function**  |

---

## **Execution Diagram**

```
greet = Repeat(3)(greet)
         ↑       ↑
     __init__  __call__
       │         │
  decorator   gets function
 parameters     object
                ↓
          returns wrapper
                ↓
      greet("Vivek", 30)
             │
       wrapper called
             │
   runs func 3 times with ("Vivek", 30)
```

---

## **Key Points**

1. **Decorator parameters** go to `__init__`.
2. **The target function object** goes to `__call__`.
3. **The target function's runtime parameters** go to the wrapper function you return from `__call__`.

---


