// gcc -o tray.exe systray.c -mwindows -luser32 -lshell32 -lgdi32
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>

// Global variables
NOTIFYICONDATA nid;
HWND hWndConsole;
HMENU hPopupMenu;
BOOL running = TRUE;
HWND hWndHidden;
BOOL isMinimized = FALSE;

// Function declarations
DWORD WINAPI GuiThread(LPVOID lpParam);
void MinimizeToTray();
void RestoreFromTray();
void ExitApplication();
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void SetupTrayIcon(HWND hWnd);
void SetupTrayMenu();

int main() {
    // Allocate a console for output
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    // Get the console window handle and show it
    hWndConsole = GetConsoleWindow();
    ShowWindow(hWndConsole, SW_HIDE);

    // Create a separate GUI thread
    CreateThread(NULL, 0, GuiThread, NULL, 0, NULL);

    // Main application loop
    while (running) {
        printf("Running...\n");
        Sleep(1000); // Simulate work
    }

    // Cleanup
    FreeConsole();
    return 0;
}

DWORD WINAPI GuiThread(LPVOID lpParam) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "TrayMinimizerClass";
    RegisterClass(&wc);

    // Create a hidden window to process messages
    hWndHidden = CreateWindow(wc.lpszClassName, "Hidden Window", WS_OVERLAPPEDWINDOW,
                             0, 0, 0, 0, NULL, NULL, hInstance, NULL);
    
    if (!hWndHidden) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    SetupTrayIcon(hWndHidden);
    SetupTrayMenu();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


void SetupTrayIcon(HWND hWnd) {
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_USER + 1;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    strcpy(nid.szTip, "Console App");
    Shell_NotifyIcon(NIM_ADD, &nid);
}

void SetupTrayMenu() {
    hPopupMenu = CreatePopupMenu();
    // Add menu items where InsertMenu each parameter means:
    //InPopupMEnu - handle to the menu to be changed
    //uPosition - position at which to insert the new menu item
    //uFlags - options that control the appearance and behavior of the new menu item
    //uIDNewItem - menu item identifier
    //lpNewItem - pointer to a null-terminated string or a handle to the drop-down menu or submenu
    InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, WM_USER + 2, "Open");
    InsertMenu(hPopupMenu, 1, MF_BYPOSITION | MF_STRING, WM_USER + 3, "Exit");
}

void MinimizeToTray() {
    ShowWindow(hWndConsole, SW_RESTORE);
    ShowWindow(hWndConsole, SW_HIDE);
    isMinimized = TRUE;
}

void RestoreFromTray() {
    ShowWindow(hWndConsole, SW_RESTORE);
    SetForegroundWindow(hWndConsole);
    isMinimized = FALSE;
}

void ExitApplication() {
    Shell_NotifyIcon(NIM_DELETE, &nid);
    running = FALSE;
    PostQuitMessage(0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_USER + 1:
            if (lParam == WM_RBUTTONDOWN) {
                POINT curPoint;
                GetCursorPos(&curPoint);
                SetForegroundWindow(hWnd);

                UINT clicked = TrackPopupMenu(hPopupMenu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, hWnd, NULL);
                SendMessage(hWnd, WM_NULL, 0, 0);
                if (clicked == WM_USER + 2) {
                    printf("Open CLICKED\n");
                    RestoreFromTray();
                } else if (clicked == WM_USER + 3) {
                    printf("Exit CLICKED\n");
                    ExitApplication();
                }
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            if (message == WM_SYSCOMMAND && wParam == SC_MINIMIZE) {
                printf("Minimize CLICKED\n");
                MinimizeToTray();
                return 0;
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}