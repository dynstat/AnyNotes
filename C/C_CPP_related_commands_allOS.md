Here’s a detailed guide with commands, explanations, and practical examples for compiling .c and .cpp files, creating and using static/dynamic libraries on Windows, Linux, and macOS using GCC, Clang, and MSVC compilers.


---

Part 1: Quick Summary


---

Part 2: Detailed Explanations with Commands


---

1. Compile .c, .cpp, and both together

Linux/macOS with GCC/Clang

# Compile C file
gcc main.c -o main_c_app
# OR using Clang
clang main.c -o main_c_app

# Compile C++ file
g++ main.cpp -o main_cpp_app
# OR using Clang++
clang++ main.cpp -o main_cpp_app

# Compile both together (use g++ or clang++ for linking C++ stdlib)
g++ main.cpp helper.c -o mixed_app

Options Explained:

-o <output>: Output executable name.

g++: Links against C++ standard libraries.

Use extern "C" in C++ if calling C functions to avoid name mangling.


Windows using MSVC (cl)

# Compile C file
cl main.c

# Compile C++ file
cl main.cpp

# Compile both together
cl main.cpp helper.c

> Generates main.exe and main.obj. MSVC auto-handles linking.




---

2. Include a Static Library

Linux/macOS

gcc main.c -L. -lmylib -o app

-L.: Add current directory to library search path.

-lmylib: Links with libmylib.a (omit lib prefix and .a extension).


Windows (MinGW)

gcc main.c -L. -lmylib -o app.exe

> Requires libmylib.a or mylib.lib.



Windows (MSVC)

cl main.c mylib.lib


---

3. Include a Dynamic Library

Linux

gcc main.c -L. -lmylib -o app
export LD_LIBRARY_PATH=.
./app

> Links against libmylib.so. Use LD_LIBRARY_PATH for runtime linking.



macOS

gcc main.c -L. -lmylib -o app
export DYLD_LIBRARY_PATH=.
./app

Windows (MinGW)

gcc main.c -o app.exe -L. -lmylib

> Needs mylib.dll at runtime.



Windows (MSVC)

cl main.c mylib.dll.lib

> Requires .dll and corresponding .lib for linking.




---

4. Create Static and Dynamic Libraries

Static Library

Linux/macOS (GCC/Clang)

gcc -c foo.c         # Creates foo.o
ar rcs libmylib.a foo.o

Windows (MSVC)

cl /c foo.c
lib /OUT:mylib.lib foo.obj

Dynamic Library

Linux

gcc -fPIC -c foo.c
gcc -shared -o libmylib.so foo.o

macOS

gcc -dynamiclib -o libmylib.dylib foo.o

Windows (MinGW)

gcc -shared -o mylib.dll foo.c -Wl,--out-implib,libmylib.a

Windows (MSVC)

cl /LD foo.c

> Generates foo.dll and foo.lib.




---

5. Use Static Library in Another Project

Linux/macOS

gcc main.c -L./libdir -lmylib -o main_app

Place libmylib.a in ./libdir.

Windows (MinGW)

gcc main.c -L./libdir -lmylib -o main.exe

Windows (MSVC)

cl main.c libdir\mylib.lib

> Cross-platform Tip: Keep headers and .lib/.a files organized in /include and /lib.




---

6. Use Dynamic Library in Another Project

Linux

gcc main.c -L./libdir -lmylib -o app
export LD_LIBRARY_PATH=./libdir
./app

macOS

gcc main.c -L./libdir -lmylib -o app
export DYLD_LIBRARY_PATH=./libdir
./app

Windows (MinGW)

gcc main.c -L./libdir -lmylib -o app.exe
copy mylib.dll . && app.exe

Windows (MSVC)

cl main.c libdir\mylib.dll.lib

> Requires .dll to be in the same directory or in PATH.




---

Compiler Differences: GCC vs Clang vs MSVC


---

Practical Example Directory Layout

project/
├── include/
│   └── mylib.h
├── lib/
│   ├── libmylib.a
│   └── libmylib.so / mylib.dll
├── src/
│   └── main.c

Command (Linux/macOS):

gcc src/main.c -Iinclude -Llib -lmylib -o main


---

Resources

GCC Manual

Clang Docs

MSVC Docs



---



