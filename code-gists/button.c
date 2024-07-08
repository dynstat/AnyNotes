#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

// Global variables
bool running = true;
HWND hEdit;

// Function declaration for the running loop
DWORD WINAPI RunningLoop(LPVOID lpParam);

// Window procedure to handle events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Function to append text to the edit control
void AppendText(HWND hEdit, const char* newText);

int main() {
    // Register the window class
    const char CLASS_NAME[]  = "Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        "Sample Window",                // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,

        NULL,       // Parent window
        NULL,       // Menu
        GetModuleHandle(NULL), // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);

    // Create a thread for the running loop
    DWORD dwThreadId;
    HANDLE hThread = CreateThread(
        NULL,              // Default security attributes
        0,                 // Default stack size
        RunningLoop,       // Thread function
        NULL,              // Thread function argument
        0,                 // Default creation flags
        &dwThreadId        // Receive thread identifier
    );

    if (hThread == NULL) {
        return 1;
    }

    // Run the message loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup
    running = false;  // Signal the running loop to stop
    WaitForSingleObject(hThread, INFINITE);  // Wait for the thread to finish
    CloseHandle(hThread);  // Close the thread handle

    return 0;
}

DWORD WINAPI RunningLoop(LPVOID lpParam) {
    while (running) {
        AppendText(hEdit, "running\n");
        Sleep(1000);  // Wait for 1 second
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        // Create the edit control
        hEdit = CreateWindowEx(
            WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
            0, 0, 500, 400,
            hwnd, (HMENU) 1, GetModuleHandle(NULL), NULL
        );
        if (hEdit == NULL) {
            MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);
        }
        return 0;

    case WM_SIZE:
        // Resize the edit control when the window is resized
        MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
        if (wParam == SIZE_MINIMIZED) {
            AppendText(hEdit, "Minimize button clicked\n");
        } else if (wParam == SIZE_MAXIMIZED) {
            AppendText(hEdit, "Maximize button clicked\n");
        } else if (wParam == SIZE_RESTORED) {
            AppendText(hEdit, "Window restored\n");
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AppendText(HWND hEdit, const char* newText) {
    // Get the current text length
    int len = GetWindowTextLength(hEdit);

    // Move the caret to the end of the text
    SendMessage(hEdit, EM_SETSEL, (WPARAM)len, (LPARAM)len);

    // Insert the new text
    SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)newText);
}
