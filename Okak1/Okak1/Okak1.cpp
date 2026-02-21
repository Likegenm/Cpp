#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#include <windows.h>
#include <string>
using namespace std;

// Глобальные переменные
HWND hEdit;
wstring inputText;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CREATE: {
        // Создаем поле для ввода текста
        hEdit = CreateWindowExW(
            0, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL,
            50, 50, 700, 200,
            hwnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        // Кнопка для отправки
        CreateWindowExW(
            0, L"BUTTON", L"Отправить",
            WS_CHILD | WS_VISIBLE,
            50, 270, 100, 30,
            hwnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, NULL
        );

        return 0;
    }

    case WM_COMMAND: {
        if (LOWORD(wParam) == 1) { // Нажата кнопка
            // Получаем текст из поля ввода
            int len = GetWindowTextLengthW(hEdit) + 1;
            wchar_t* buffer = new wchar_t[len];
            GetWindowTextW(hEdit, buffer, len);

            inputText = buffer;
            delete[] buffer;

            // Перерисовываем окно
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Заливаем белым фоном
        RECT rect;
        GetClientRect(hwnd, &rect);
        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);

        // Выводим сохраненный текст
        if (!inputText.empty()) {
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(0, 0, 0));
            TextOutW(hdc, 50, 320, inputText.c_str(), inputText.length());
        }

        EndPaint(hwnd, &ps);
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"TextWindow";
    const wchar_t WINDOW_TITLE[] = L"Белый экран с текстом";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 820, 450,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}