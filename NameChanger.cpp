#include <Windows.h>
#include <string>

HWND g_oldWindowNameInput;
HWND g_newWindowTitleInput;
HWND g_changeButton;
HFONT font; // Declare the font variable here

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            g_oldWindowNameInput = CreateWindow("EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER,
                50, 50, 200, 30, hwnd, NULL, NULL, NULL);

            g_newWindowTitleInput = CreateWindow("EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER,
                50, 100, 200, 30, hwnd, NULL, NULL, NULL);

            g_changeButton = CreateWindow("BUTTON", "Change Window Name", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                50, 150, 150, 30, hwnd, NULL, NULL, NULL);

            // Get the "Segoe UI" font
            font = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                    OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                    DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");

            // Set the font for the input fields and the button
            SendMessage(g_oldWindowNameInput, WM_SETFONT, (WPARAM)font, TRUE);
            SendMessage(g_newWindowTitleInput, WM_SETFONT, (WPARAM)font, TRUE);
            SendMessage(g_changeButton, WM_SETFONT, (WPARAM)font, TRUE);

            return 0;

        case WM_COMMAND:
            if ((HWND)lParam == g_changeButton && HIWORD(wParam) == BN_CLICKED) {
                char oldNameBuffer[256];
                char newNameBuffer[256];
                GetWindowTextA(g_oldWindowNameInput, oldNameBuffer, sizeof(oldNameBuffer));
                GetWindowTextA(g_newWindowTitleInput, newNameBuffer, sizeof(newNameBuffer));

                HWND targetWnd = FindWindowA(NULL, oldNameBuffer);
                if (targetWnd) {
                    SetWindowTextA(targetWnd, newNameBuffer);
                }
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char* windowClassName = "WindowNameChangerApp";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = windowClassName;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        windowClassName,
        "Name Changer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        400, 250,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hwnd) {
        return 1;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
