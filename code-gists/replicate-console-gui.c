// gcc replicate-console-gui.c -o replicate-console-gui -mwindows -luser32 -lgdi32 -luxtheme -lcomctl32 -ldwmapi

#include <richedit.h>
#include <uxtheme.h>
#include <commctrl.h>
#include <dwmapi.h>
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

// Global variables
bool running = true;
HWND hEdit;
HBRUSH hbrBkgnd;
char textBuffer[1024 * 10] = "";

// Function declaration for the running loop
DWORD WINAPI RunningLoop(LPVOID lpParam);

// Window procedure to handle events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Function to append text to the edit control
void AppendText(HWND hEdit, const char *newText);

// Function to load required libraries
void LoadRequiredLibraries()
{
    LoadLibrary("Msftedit.dll");
}

// The entry point for a graphical Windows-based application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    LoadRequiredLibraries();
    // Initialize common controls with the latest visual styles
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);

    // Register the window class
    const char CLASS_NAME[] = "Modern Sample Window Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = CreateSolidBrush(RGB(30, 30, 30)); // Dark background

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW, // Optional window styles
        CLASS_NAME,             // Window class
        "Modern Sample Window", // Window text
        WS_OVERLAPPEDWINDOW,    // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    // Enable dark mode for the window
    BOOL value = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

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
        AppendText(hEdit, "\tMain code is Running\n");
        Sleep(1000); // Wait for 1 second
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30)); // Dark background
        // Create the edit control
        hEdit = CreateWindowEx(
            0, "RICHEDIT50W", "",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
            0, 0, 0, 0,
            hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

        if (hEdit == NULL)
        {
            MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);
        }
        else
        {
            // Set text and background colors, font, and size
            SendMessage(hEdit, EM_SETBKGNDCOLOR, 0, RGB(30, 30, 30));
            CHARFORMAT2 cf;
            ZeroMemory(&cf, sizeof(CHARFORMAT2));
            cf.cbSize = sizeof(CHARFORMAT2);
            cf.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE;
            cf.crTextColor = RGB(173, 216, 230); // Light blue text
            wcscpy_s(cf.szFaceName, LF_FACESIZE, L"Consolas");
            cf.yHeight = 240; // 12 point font (12 * 20)
            SendMessage(hEdit, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
        }
        return 0;

    case WM_SIZE:
        MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
        SetWindowText(hEdit, textBuffer); // Restore the text from the buffer
        if (wParam == SIZE_MINIMIZED)
        {
            AppendText(hEdit, "\tMinimize button clicked\n\n");
        }
        else if (wParam == SIZE_MAXIMIZED)
        {
            AppendText(hEdit, "\tMaximize button clicked\n\n");
        }
        else if (wParam == SIZE_RESTORED)
        {
            // AppendText(hEdit, "\tWindow restored\n\n");
            // Set the entire text content
            // SetWindowText(hEdit, textBuffer);
        }
        return 0;

    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORSTATIC:
        if ((HWND)lParam == hEdit)
        {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, RGB(173, 216, 230)); // Light blue text
            SetBkColor(hdcEdit, RGB(30, 30, 30));      // Dark background
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
    for (int i = 0; newText[i] != '\0' && j < sizeof(formattedText) - 2; i++)
    {
        if (newText[i] == '\n')
        {
            formattedText[j++] = '\r';
            formattedText[j++] = '\n';
        }
        else
        {
            formattedText[j++] = newText[i];
        }
    }
    formattedText[j] = '\0'; // Null-terminate the formatted string

    // Calculate the buffer space left, ensuring not to overwrite the buffer
    size_t bufferSpaceLeft = sizeof(textBuffer) - strlen(textBuffer) - 1;

    // Append the new text to the buffer if there is space
    if (bufferSpaceLeft > strlen(formattedText))
    {
        strncat(textBuffer, formattedText, bufferSpaceLeft);
    }

    // Set the entire text content
    SetWindowText(hEdit, textBuffer);

    // Scroll to the bottom of the edit control
    int len = GetWindowTextLength(hEdit);
    SendMessage(hEdit, EM_SETSEL, len, len);
    SendMessage(hEdit, EM_SCROLLCARET, 0, 0);
}