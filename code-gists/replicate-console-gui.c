// cl button.c /link User32.lib Gdi32.lib

#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Global variables
bool running = true;
HWND hEdit;
HBRUSH hbrBkgnd;                 // Brush for the background color
char textBuffer[1024 * 10] = ""; // Buffer to store the text

// Function declaration for the running loop
DWORD WINAPI RunningLoop(LPVOID lpParam);

// Window procedure to handle events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Function to append text to the edit control
void AppendText(HWND hEdit, const char *newText);

// The entry point for a graphical Windows-based application
// WINAPI is a macro that defines the calling convention for the function
// HINSTANCE hInstance: Handle to the current instance of the application
// HINSTANCE hPrevInstance: Handle to the previous instance of the application (always NULL in Win32 applications)
// LPSTR lpCmdLine: Command line for the application, excluding the program name
// int nCmdShow: Controls how the window is to be shown (e.g., minimized, maximized, etc.)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register the window class
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 255)); // Set background color to black

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                   // Optional window styles
        CLASS_NAME,          // Window class
        "Sample Window",     // Window text
        WS_OVERLAPPEDWINDOW, // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Create a thread for the running loop
    HANDLE hThread = CreateThread(NULL, 0, RunningLoop, NULL, 0, NULL);

    if (hThread == NULL)
    {
        return 1;
    }

    // Run the message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup
    running = false;                        // Signal the running loop to stop
    WaitForSingleObject(hThread, INFINITE); // Wait for the thread to finish
    CloseHandle(hThread);                   // Close the thread handle

    return 0;
}

DWORD WINAPI RunningLoop(LPVOID lpParam)
{
    while (running)
    {
        AppendText(hEdit, "running\n");
        Sleep(1000); // Wait for 1 second
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        hbrBkgnd = CreateSolidBrush(RGB(0, 0, 0)); // Create black brush
        // Create the edit control
        hEdit = CreateWindowEx(
            0, "EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
            0, 0, 500, 400,
            hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
        if (hEdit == NULL)
        {
            MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);
        }
        return 0;

    case WM_SIZE:
        // Resize the edit control when the window is resized
        MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
        SetWindowText(hEdit, textBuffer); // Restore the text from the buffer
        if (wParam == SIZE_MINIMIZED)
        {
            AppendText(hEdit, "Minimize button clicked\n");
        }
        else if (wParam == SIZE_MAXIMIZED)
        {
            AppendText(hEdit, "Maximize button clicked\n");
        }
        else if (wParam == SIZE_RESTORED)
        {
            // AppendText(hEdit, "Window restored\n");
            // Set the entire text content
            // SetWindowText(hEdit, textBuffer);
        }
        return 0;

    case WM_CTLCOLOREDIT:
        if ((HWND)lParam == hEdit)
        {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, RGB(255, 255, 255)); // Set text color to white
            SetBkColor(hdcEdit, RGB(0, 0, 0));         // Set background color to black
            return (LRESULT)hbrBkgnd;
        }
        break;

    case WM_DESTROY:
        DeleteObject(hbrBkgnd); // Delete the background brush
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AppendText(HWND hEdit, const char *newText)
{
    // First, convert \n to \r\n in the input text
    char formattedText[2048]; // Ensure this buffer is large enough for the formatted text
    int j = 0;
    for (int i = 0; newText[i] != '\0' && j < sizeof(formattedText) - 2; i++) {
        if (newText[i] == '\n') {
            formattedText[j++] = '\r';
            formattedText[j++] = '\n';
        } else {
            formattedText[j++] = newText[i];
        }
    }
    formattedText[j] = '\0'; // Null-terminate the formatted string

    // Calculate the buffer space left, ensuring not to overwrite the buffer
    size_t bufferSpaceLeft = sizeof(textBuffer) - strlen(textBuffer) - 1;

    // Append the new text to the buffer if there is space
    if (bufferSpaceLeft > strlen(formattedText)) {
        strncat(textBuffer, formattedText, bufferSpaceLeft);
    }

    // Set the entire text content
    SetWindowText(hEdit, textBuffer);

    // Scroll to the bottom of the edit control
    int len = GetWindowTextLength(hEdit);
    SendMessage(hEdit, EM_SETSEL, len, len);
    SendMessage(hEdit, EM_SCROLLCARET, 0, 0);
}