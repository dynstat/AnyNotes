### Minutes of Meeting (MOM) Format

**Title of the Meeting:** [Meeting Title]  
**Date:** [Date]  
**Time:** [Start Time] - [End Time]  
**Location:** [Meeting Location or Virtual Meeting Platform]  
**Attendees:** [List of Attendees]  
**Absentees:** [List of Absentees]

#### Agenda:

1. [Agenda Item 1]
2. [Agenda Item 2]
3. [Agenda Item 3]
   - And so on...

#### Discussions:

1. **[Agenda Item 1]**
   - [Summary of the discussion]
   - [Any decisions made or action items]
2. **[Agenda Item 2]**
   - [Summary of the discussion]
   - [Any decisions made or action items]
3. **[Agenda Item 3]**
   - [Summary of the discussion]
   - [Any decisions made or action items]
   - And so on...

#### Action Items:

- **[Action Item 1]**
  - [Responsible Person]
  - [Deadline]
- **[Action Item 2]**
  - [Responsible Person]
  - [Deadline]
- And so on...

#### Next Meeting:

**Date:** [Next Meeting Date]  
**Time:** [Next Meeting Time]  
**Location:** [Next Meeting Location or Platform]

#### Additional Notes:

- [Any other important notes or information]

This format ensures that all critical aspects of the meeting are documented, making it easier for attendees and other stakeholders to follow up on action items and decisions.

T

### 1. Mark Functions for Export

In your DLL project, you need to specify which functions are to be exported. This is done using the `__declspec(dllexport)` keyword before the function declaration. This tells the compiler and linker that this function should be available outside the DLL.

Here's an example:

```c
// ExampleDLL.h

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) int add(int a, int b);
__declspec(dllexport) void displayMessage();

#ifdef __cplusplus
}
#endif
```

### 2. Implement the Functions

Implement these functions in a corresponding `.c` file.

```c
// ExampleDLL.c

#include "ExampleDLL.h"
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

void displayMessage() {
    printf("Hello from DLL!\n");
}
```

### 3. Compile the DLL

Using your development environment (like Visual Studio Code with a C/C++ compiler setup), compile these files into a DLL. Ensure your project settings are configured to output a DLL.

### 4. Use the DLL in Another Project

To use the exported functions in another project, you need to do the following:

#### a. Declare the Functions in the Other Project

You can either include the header file if it's accessible, or declare the functions explicitly:

```c
// OtherProject.c

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllimport) int add(int a, int b);
__declspec(dllimport) void displayMessage();

#ifdef __cplusplus
}
#endif
```

#### b. Link Against the DLL

When compiling your project, you need to link against the generated DLL. This can typically be done by adding the DLL to your project's linker settings or placing the DLL in a directory that's part of the system's PATH.

#### c. Copy the DLL

Ensure the DLL is in the same directory as the executable of your new project or in any directory included in the system PATH.

### 5. Run the Other Project

Run your application. It should now be able to call the `add` and `displayMessage` functions defined and exported from your DLL.

### Additional Tips

- **Use `extern "C"`**: This prevents C++ name mangling if you're using a C++ compiler.
- **Error Handling**: Check for errors related to loading the DLL or finding the functions.
- **Versioning**: Manage versions of your DLL carefully to avoid compatibility issues.

To compile and link a DLL and use it in another project across multiple platforms, you'll need to use different tools and commands depending on the operating system. Below, I'll outline the process for both Windows and Linux, as these are the most common platforms for such tasks.

### Windows

#### Compiling the DLL

1. **Create the DLL**:
   Use `cl.exe` (MSVC compiler) to compile the DLL. You need to ensure that your environment variables are set up for command line compilation (e.g., using Visual Studio's Developer Command Prompt).
   
   ```bash
   cl /c /EHsc /D_USRDLL /D_WINDLL ExampleDLL.c
   ```
   
   - `/c`: Compiles without linking.
   - `/EHsc`: Specifies the exception handling model.
   - `/D_USRDLL`: Defines `_USRDLL`, which is used to switch between `__declspec(dllexport)` and `__declspec(dllimport)`.
   - `/D_WINDLL`: Defines `_WINDLL`, which is necessary for building DLLs.

2. **Link the DLL**:
   Use `link.exe` to create the DLL from the compiled object file.
   
   ```bash
   link /DLL /OUT:ExampleDLL.dll ExampleDLL.obj
   ```
   
   - `/DLL`: Specifies that a DLL should be created.
   - `/OUT`: Specifies the output file name.

#### Using the DLL in Another Project

1. **Include the Header**:
   Make sure the header file is accessible to the project, or declare the functions using `__declspec(dllimport)`.

2. **Link with the DLL**:
   When compiling the application that uses the DLL, ensure the linker can find the DLL. You might need to specify the path to the DLL or ensure it's in a directory listed in your PATH environment variable.
   
   ```bash
   cl /EHsc OtherProject.c /link /LIBPATH:path_to_dll_directory ExampleDLL.lib
   ```
   
   - `/LIBPATH`: Specifies the path to the directory that contains `ExampleDLL.lib`.

### Linux

#### Compiling the DLL (Shared Library in Linux)

1. **Create the Shared Library**:
   Use `gcc` to compile and link the shared library.
   
   ```bash
   gcc -fPIC -shared -o libexample.so ExampleDLL.c
   ```
   
   - `-fPIC`: Generates position-independent code.
   - `-shared`: Creates a shared library.

#### Using the Shared Library in Another Project

1. **Include the Header**:
   Ensure the header file is accessible, or declare the functions explicitly in your source file.

2. **Compile and Link with the Shared Library**:
   When compiling the application that uses the shared library, specify the path to the library.
   
   ```bash
   gcc OtherProject.c -Lpath_to_library_directory -lexample -o OtherProject
   ```
   
   - `-L`: Specifies the path to the directory that contains `libexample.so`.
   - `-l`: Links with `libexample.so` (note that the `lib` prefix and `.so` suffix are omitted).

3. **Set the Library Path**:
   Before running your application, make sure the shared library can be found by setting the `LD_LIBRARY_PATH` environment variable.
   
   ```bash
   export LD_LIBRARY_PATH=path_to_library_directory:$LD_LIBRARY_PATH
   ```

### Cross-Platform Considerations

- **Build Scripts**: Consider using build systems like CMake to handle differences in compilation and linking commands across platforms.
- **Continuous Integration**: Set up CI pipelines to build and test your code on multiple platforms to catch platform-specific issues early.

These commands and steps should help you compile and use DLLs/shared libraries across different platforms effectively.



# Difference between .lib and .dll

A concise comparison between `.lib` and `.dll` files in tabular form:

| Feature          | `.lib` (Static Library)                                                  | `.dll` (Dynamic Link Library)                                         |
| ---------------- | ------------------------------------------------------------------------ | --------------------------------------------------------------------- |
| **Definition**   | A collection of object files linked into the executable at compile time. | A collection of functions and resources loaded at runtime.            |
| **Linking**      | Linked at compile time.                                                  | Linked at runtime.                                                    |
| **File Size**    | Increases the size of the executable.                                    | Does not increase the size of the executable.                         |
| **Memory Usage** | Code is loaded into memory when the executable is loaded.                | Code is loaded into memory only when needed.                          |
| **Sharing**      | Cannot be shared between multiple programs.                              | Can be shared between multiple programs.                              |
| **Updates**      | Requires recompilation of the executable for updates.                    | Can be updated independently of the executable.                       |
| **Distribution** | Only the executable needs to be distributed.                             | Both the executable and the DLL need to be distributed.               |
| **Dependency**   | No runtime dependency on external files.                                 | Requires the DLL to be present at runtime.                            |
| **Usage**        | Typically used for libraries that are not expected to change frequently. | Used for libraries that may be updated or shared across applications. |
| **Example**      | `mylib.lib`                                                              | [mydll.dll](file:///d%3A/dll_to_dll/mydll.dll#1%2C1-1%2C1)            |

### Detailed Explanation

- **Static Library (.lib)**:
  
  - **Linking**: The code from the static library is copied into the executable at compile time.
  - **File Size**: The executable file size increases because it contains the library code.
  - **Memory Usage**: The entire library code is loaded into memory when the executable is loaded.
  - **Sharing**: Static libraries cannot be shared between multiple programs; each program gets its own copy of the library code.
  - **Updates**: If the library code changes, the executable must be recompiled.
  - **Distribution**: Only the executable needs to be distributed, as it contains all the necessary code.

- **Dynamic Link Library (.dll)**:
  
  - **Linking**: The code from the DLL is not copied into the executable. Instead, the executable contains references to the DLL, which is loaded at runtime.
  - **File Size**: The executable file size remains smaller because it does not contain the library code.
  - **Memory Usage**: The DLL code is loaded into memory only when needed, which can save memory if the DLL is not used immediately.
  - **Sharing**: DLLs can be shared between multiple programs, allowing for code reuse and reduced memory usage.
  - **Updates**: DLLs can be updated independently of the executable. As long as the interface (function signatures) remains the same, the executable does not need to be recompiled.
  - **Distribution**: Both the executable and the DLL need to be distributed. The executable depends on the presence of the DLL at runtime.

### Example Usage

- **Static Library**:
  
  ```c
  // mylib.h
  int add(int a, int b);
  
  // mylib.c
  int add(int a, int b) {
      return a + b;
  }
  
  // main.c
  #include "mylib.h"
  int main() {
      int result = add(3, 4);
      return 0;
  }
  ```

- **Dynamic Link Library**:
  
  ```c
  // mydll.h
  #ifdef MYDLL_EXPORTS
  #define MYDLL_API __declspec(dllexport)
  #else
  #define MYDLL_API __declspec(dllimport)
  #endif
  
  MYDLL_API int add(int a, int b);
  
  // mydll.c
  #include "mydll.h"
  MYDLL_API int add(int a, int b) {
      return a + b;
  }
  
  // main.c
  #include <windows.h>
  #include <stdio.h>
  
  typedef int (*AddFunc)(int, int);
  
  int main() {
      HINSTANCE hinstLib = LoadLibrary(TEXT("mydll.dll"));
      if (hinstLib != NULL) {
          AddFunc add = (AddFunc) GetProcAddress(hinstLib, "add");
          if (add != NULL) {
              int result = add(3, 4);
              printf("Result: %d\n", result);
          }
          FreeLibrary(hinstLib);
      }
      return 0;
  }
  ```

In summary, `.lib` files are used for static linking, where the library code is included in the executable at compile time, while `.dll` files are used for dynamic linking, where the library code is loaded at runtime.
