#include <cstring>
#include <cstdint>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" {
    EXPORT int process_apdu(const uint8_t* command, int command_len, uint8_t* response, int* response_len) {
        // This is a dummy implementation for testing purposes
        // In a real scenario, this would process the APDU command and generate a response

        // For this example, we'll just echo back the command with a status word appended
        if (*response_len < command_len + 2) {
            return -1; // Not enough space in the response buffer
        }

        std::memcpy(response, command, command_len);
        response[command_len] = 0x90; // SW1: Success
        response[command_len + 1] = 0x00; // SW2: No further qualification

        *response_len = command_len + 2;

        return 0; // Success
    }
}