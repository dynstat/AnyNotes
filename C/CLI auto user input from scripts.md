```python:run_cli.py
import subprocess

def run_cli_with_inputs(cli_path, inputs):
    # Start the CLI application
    process = subprocess.Popen(cli_path, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

    for i, user_input in enumerate(inputs):
        # Read the output from the CLI tool
        output = process.stdout.read()
        
        # Print the last 10 lines of the output
        output_lines = output.splitlines()
        print("\n".join(output_lines[-10:]))

        # Send the input to the CLI tool
        process.stdin.write(user_input + '\n')
        process.stdin.flush()

    # Wait for the process to complete
    process.wait()

# Example usage
cli_path = "path/to/your/cli_tool.exe"
inputs = ["input1", "input2", "input3"]
run_cli_with_inputs(cli_path, inputs)
```

### Explanation

1. **Importing subprocess**: The `subprocess` module is used to spawn new processes, connect to their input/output/error pipes, and obtain their return codes.

2. **Starting the CLI application**:
    ```python
    process = subprocess.Popen(cli_path, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    ```
    - `Popen` is used to start the CLI application.
    - `stdin=subprocess.PIPE` allows us to send input to the process.
    - `stdout=subprocess.PIPE` allows us to read the output from the process.
    - `stderr=subprocess.PIPE` allows us to read the error output from the process.
    - `text=True` ensures that the input and output are treated as text strings.

3. **Reading and printing the last 10 lines of output**:
    ```python
    output = process.stdout.read()
    output_lines = output.splitlines()
    print("\n".join(output_lines[-10:]))
    ```
    - `process.stdout.read()` reads the output from the CLI tool.
    - `output.splitlines()` splits the output into individual lines.
    - `print("\n".join(output_lines[-10:]))` prints the last 10 lines of the output.

4. **Sending input to the CLI tool**:
    ```python
    process.stdin.write(user_input + '\n')
    process.stdin.flush()
    ```
    - `process.stdin.write(user_input + '\n')` writes the user input to the process.
    - `process.stdin.flush()` ensures that the input is sent immediately.

5. **Waiting for the process to complete**:
    ```python
    process.wait()
    ```
    - `process.wait()` waits for the process to complete before the script continues.

### Example Usage
- `cli_path` is the path to your CLI tool executable.
- `inputs` is a list of inputs that you want to provide to the CLI tool.

This script will start the CLI tool, provide the inputs one by one, and print the last 10 lines of the output before each input.

### C Language Example

To achieve similar functionality in C, you can use the `popen` and `pclose` functions for executing a command and interacting with it. Here's a simple example:

```c:run_cli.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fp;
    char path[1035];
    char *inputs[3] = {"input1", "input2", "input3"};
    int i;

    // Open the command for reading.
    fp = popen("path/to/your/cli_tool.exe", "w");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    for (i = 0; i < 3; i++) {
        // Write input to the CLI tool
        fprintf(fp, "%s\n", inputs[i]);
        fflush(fp);
    }

    // Close the process
    pclose(fp);

    return 0;
}
```

### Explanation for C Code
1. **Opening the CLI tool**: `popen` is used to execute the CLI tool, and it opens a pipe that you can write to.
2. **Writing inputs**: Using `fprintf` to send inputs to the CLI tool.
3. **Closing the process**: `pclose` closes the pipe and ends the process.

### Batch Script Example

For a Batch script on Windows, you can use a similar approach:

```batch:run_cli.bat
@echo off
setlocal

set "cli_path=path\to\your\cli_tool.exe"

(
    echo input1
    echo input2
    echo input3
) | %cli_path%
```

### Explanation for Batch Script
- **Pipe (`|`)**: This sends the `echo`ed inputs directly into your CLI tool.
- **Parentheses**: Used to group multiple commands so their combined output can be piped to the CLI tool.

### Bash Script Example

For a Bash script on Unix-like systems:

```bash:run_cli.sh
#!/bin/bash

cli_path="/path/to/your/cli_tool"

{
    echo "input1"
    echo "input2"
    echo "input3"
} | $cli_path
```

### Explanation for Bash Script
- **Braces (`{}`)**: Similar to the Batch script, this groups commands so that all their outputs can be piped into the CLI tool.
- **Echo**: Sends each input to the CLI tool.

These examples provide basic functionality to interact with a CLI tool by sending predefined inputs. Adjust paths and inputs as necessary for your specific use case.