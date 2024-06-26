# Function Pointers using DLL in C

Creating a `registerCallback` function in the DLL that allows the main program to register a callback function. The DLL can then call this registered callback function when needed.

### [mydll.c](file:///d%3A/dll_to_dll/mydll.c#1%2C1-1%2C1)

```c
#include <stdio.h>

// Define a function pointer type
typedef void (*TestFunctionPointer)(int, const char*);

// Global variable to store the callback function pointer
TestFunctionPointer g_callback = NULL;

// Function to register the callback
__declspec(dllexport) void registerCallback(TestFunctionPointer callback) {
    g_callback = callback;
}

// Function to be exported from the DLL
__declspec(dllexport) void dllFunction() {
    int number = 42;
    const char* message = "Hello from DLL";

    // Perform some operations (if any)
    printf("DLL Function: Performing some operations...\n");

    // Call the registered callback function with data
    if (g_callback != NULL) {
        g_callback(number, message);
    } else {
        printf("No callback function registered.\n");
    }
}
```

### `test_fp.c`

```c
#include <stdio.h>
#include <windows.h> // Add this line to include the windows.h header

// Function to be called by the DLL
void testFunction(int number, const char* message) {
    printf("Test Function: Received number = %d, message = %s\n", number, message);
}

int main() {
    // Declare the function pointers
    void (*registerCallback)(void (*)(int, const char*));
    void (*dllFunction)();

    // Load the DLL
    HINSTANCE hinstLib = LoadLibrary(TEXT("mydll.dll"));
    if (hinstLib != NULL) {
        // Get the function addresses
        registerCallback = (void (*)(void (*)(int, const char*))) GetProcAddress(hinstLib, "registerCallback");
        dllFunction = (void (*)()) GetProcAddress(hinstLib, "dllFunction");

        if (registerCallback != NULL && dllFunction != NULL) {
            // Register the callback function
            registerCallback(testFunction);

            // Call the DLL function
            dllFunction();
        } else {
            printf("Could not locate the functions.\n");
        }

        // Free the DLL module
        FreeLibrary(hinstLib);
    } else {
        printf("Could not load the DLL.\n");
    }

    return 0;
}
```

### Explanation

1. **`mydll.c`**:
   
   - Defines a global variable `g_callback` to store the callback function pointer.
   - Implements `registerCallback` to set the `g_callback` variable.
   - Implements `dllFunction` to call the registered callback function if it is set.

2. **`test_fp.c`**:
   
   - Implements `testFunction` which prints the received integer and string.
   - In `main`, loads the DLL and retrieves the addresses of `registerCallback` and `dllFunction`.
   - Registers `testFunction` as the callback using `registerCallback`.
   - Calls `dllFunction` which will then call the registered callback function.

### Compilation

To compile and link these files into a DLL and an executable:

1. Compile `mydll.c` into a DLL:
   
   ```shell
   gcc -shared -o mydll.dll mydll.c
   ```

2. Compile `test_fp.c` and link it with the DLL:
   
   ```shell
   gcc -o test_fp.exe test_fp.c -L. -lmydll
   ```

This setup allows the main program to register a callback function with the DLL, which the DLL can then call when needed.

---

OTHER APPROACH

Sure, I'll provide the necessary code for both [mydll.c](file:///d%3A/dll_to_dll/mydll.c#1%2C1-1%2C1) and [test_fp.c](file:///d%3A/dll_to_dll/test_fp.c#1%2C1-1%2C1) files. The DLL function will call a function in [test_fp.c](file:///d%3A/dll_to_dll/test_fp.c#1%2C1-1%2C1) using a function pointer and pass an integer and a string.

### [mydll.c](file:///d%3A/dll_to_dll/mydll.c#1%2C1-1%2C1)

```c
#include <stdio.h>

// Define a function pointer type
typedef void (*TestFunctionPointer)(int, const char*);

// Function to be exported from the DLL
__declspec(dllexport) void dllFunction(TestFunctionPointer callback) {
    int number = 42;
    const char* message = "Hello from DLL";

    // Perform some operations (if any)
    printf("DLL Function: Performing some operations...\n");

    // Call the callback function with data
    callback(number, message);
}
```

### `test_fp.c`c

```c
#include <stdio.h>

// Function to be called by the DLL
void testFunction(int number, const char* message) {
    printf("Test Function: Received number = %d, message = %s\n", number, message);
}

int main() {
    // Declare the function pointer
    void (*dllFunction)(void (*)(int, const char*));

    // Load the DLL
    HINSTANCE hinstLib = LoadLibrary(TEXT("mydll.dll"));
    if (hinstLib != NULL) {
        // Get the function address
        dllFunction = (void (*)(void (*)(int, const char*))) GetProcAddress(hinstLib, "dllFunction");

        if (dllFunction != NULL) {
            // Call the DLL function with the test function as a callback
            dllFunction(testFunction);
        } else {
            printf("Could not locate the function.\n");
        }

        // Free the DLL module
        FreeLibrary(hinstLib);
    } else {
        printf("Could not load the DLL.\n");
    }

    return 0;
}
```

### Explanation

1. **`mydll.c`**:
   
   - Defines a function pointer type `TestFunctionPointer`.
   - Implements `dllFunction` which takes a function pointer as an argument.
   - Calls the provided function pointer with an integer and a string.

2. **`test_fp.c`**:
   
   - Implements `testFunction` which prints the received integer and string.
   - In `main`, loads the DLL and retrieves the address of `dllFunction`.
   - Calls `dllFunction` with `testFunction` as the callback.

### Compilation

To compile and link these files into a DLL and an executable:

1. Compile `mydll.c` into a DLL:
   
   ```sh
   gcc -shared -o mydll.dll mydll.c
   ```

2. Compile `test_fp.c` and link it with the DLL:
   
   ```sh
   gcc -o test_fp.exe test_fp.c -L. -lmydll
   ```

This setup ensures that the DLL function can call back into the main program using a function pointer, passing along the required data.

### Compile and Link using cl compiler

1. Compile the DLL:

- Open the Developer Command Prompt for Visual Studio.

- Navigate to the directory containing mydll.c.

- Run the following command to compile and create the DLL:
  
  ```sh
  cl /LD mydll.c /Fe:mydll.dll
  ```

2. Compile the Executable:

- Navigate to the directory containing test_fp.c.

- Run the following command to compile the executable:
  
  ```sh
  cl test_fp.c /link mydll.lib
  ```

### Explanation

        `cl /LD mydll.c /Fe:mydll.dll:`

- cl is the command-line compiler.

- /LD tells the compiler to create a DLL.

- mydll.c is the source file.

- /Fe:mydll.dll specifies the output file name for the DLL.
  
  `cl test_fp.c /link mydll.lib:`

- cl is the command-line compiler.

- test_fp.c is the source file.

- /link mydll.lib links the executable with the import library for the DLL.

This will create mydll.dll and test_fp.exe. Running test_fp.exe will load the DLL, register the callback, and call the DLL function, which in turn calls the registered callback function.
