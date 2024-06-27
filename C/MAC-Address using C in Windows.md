# MAC-Address using C in Windows

To use return type, the function needs to take pointers to the variables where the MAC address and its length will be stored. Here's how you can do it:c

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256
#define MAC_ADDRESS_SIZE 13

void get_mac_address(char *mac_address, int *length) {
    char command[256];
    char buffer[BUFFER_SIZE];
    FILE *pipe;

    // Construct the command to retrieve the MAC address
    snprintf(command, sizeof(command), "getmac /fo csv /nh");

    // Open a pipe to execute the command
    pipe = _popen(command, "r");
    if (pipe == NULL) {
        printf("Failed to open pipe.\n");
        *length = 0;
        return;
    }

    // Read the output of the command
    if (fgets(buffer, BUFFER_SIZE, pipe) != NULL) {
        // Remove trailing newline character
        buffer[strcspn(buffer, "\n")] = '\0';

        // Split the output by comma to extract the MAC address
        char *token = strtok(buffer, ",");
        if (token != NULL) {
            // Remove quotes from the MAC address
            if (token[0] == '"') {
                token++;
                token[strlen(token) - 1] = '\0';
            }

            // Remove hyphens from the MAC address
            char *hyphen = strchr(token, '-');
            while (hyphen != NULL) {
                memmove(hyphen, hyphen + 1, strlen(hyphen));
                hyphen = strchr(token, '-');
            }

            // Copy the MAC address to the provided variable
            strncpy(mac_address, token, MAC_ADDRESS_SIZE);
            mac_address[MAC_ADDRESS_SIZE - 1] = '\0';
            *length = strlen(mac_address);

            printf("MAC Address: %s\n", mac_address);
        }
    }

    // Close the pipe
    _pclose(pipe);
}

int main() {
    char mac_address[MAC_ADDRESS_SIZE];
    int length;
    get_mac_address(mac_address, &length);
    printf("MAC Address Length: %d\n", length);

    return 0;
}
```

### Explanation of Changes:

1. **Function Signature**: Changed the `get_mac_address` function to take `char *mac_address` and `int *length` as parameters.
   
   ```c
   void get_mac_address(char *mac_address, int *length) {
       // ...
   }
   ```

2. **Set Length via Pointer**: Set the length using the pointer provided as an argument.
   
   ```c
   *length = strlen(mac_address);
   ```

3. **Handle Pipe Failure**: Set the length to 0 if the pipe fails to open.
   
   ```c
   if (pipe == NULL) {
       printf("Failed to open pipe.\n");
       *length = 0;
       return;
   }
   ```

4. **Update `main` Function**: Updated the `main` function to call `get_mac_address` with the appropriate arguments and print the length.
   
   ```c
   int main() {
       char mac_address[MAC_ADDRESS_SIZE];
       int length;
       get_mac_address(mac_address, &length);
       printf("MAC Address Length: %d\n", length);
       return 0;
   }
   ```
