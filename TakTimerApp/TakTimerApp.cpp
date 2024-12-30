#include <windows.h>
#include <sstream>


HWND hHourEdit, hMinuteEdit, hSecondEdit, hStartButton, hTimeDisplay;
bool timerRunning = false;
int hours = 0, minutes = 0, seconds = 0;


void UpdateDisplay(HWND hwnd) {
    std::wstringstream timeStream;
    timeStream << L"Time Remaining: "
        << (hours < 10 ? L"0" : L"") << hours << L":"
        << (minutes < 10 ? L"0" : L"") << minutes << L":"
        << (seconds < 10 ? L"0" : L"") << seconds;

    SetWindowText(hTimeDisplay, timeStream.str().c_str());
}


void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    if (hours == 0 && minutes == 0 && seconds == 0) {
        KillTimer(hwnd, 1);
        MessageBox(hwnd, L"Time's up!", L"Task Timer", MB_OK | MB_ICONINFORMATION);
        timerRunning = false;
        return;
    }

    if (seconds > 0) {
        seconds--;
    }
    else {
        seconds = 59;
        if (minutes > 0) {
            minutes--;
        }
        else {
            minutes = 59;
            if (hours > 0) {
                hours--;
            }
        }
    }
    UpdateDisplay(hwnd);
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        
        CreateWindow(L"STATIC", L"Hours:", WS_VISIBLE | WS_CHILD, 20, 20, 50, 20, hwnd, NULL, NULL, NULL);
        hHourEdit = CreateWindow(L"EDIT", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 80, 20, 50, 20, hwnd, NULL, NULL, NULL);

        CreateWindow(L"STATIC", L"Minutes:", WS_VISIBLE | WS_CHILD, 20, 50, 50, 20, hwnd, NULL, NULL, NULL);
        hMinuteEdit = CreateWindow(L"EDIT", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 80, 50, 50, 20, hwnd, NULL, NULL, NULL);

        CreateWindow(L"STATIC", L"Seconds:", WS_VISIBLE | WS_CHILD, 20, 80, 50, 20, hwnd, NULL, NULL, NULL);
        hSecondEdit = CreateWindow(L"EDIT", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 80, 80, 50, 20, hwnd, NULL, NULL, NULL);

        hStartButton = CreateWindow(L"BUTTON", L"Start Timer", WS_VISIBLE | WS_CHILD, 20, 120, 110, 30, hwnd, (HMENU)1, NULL, NULL);
        hTimeDisplay = CreateWindow(L"STATIC", L"Time Remaining: 00:00:00", WS_VISIBLE | WS_CHILD, 20, 160, 200, 20, hwnd, NULL, NULL, NULL);
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == 1 && !timerRunning) {
           
            wchar_t buffer[10];
            GetWindowText(hHourEdit, buffer, 10);
            hours = _wtoi(buffer);

            GetWindowText(hMinuteEdit, buffer, 10);
            minutes = _wtoi(buffer);

            GetWindowText(hSecondEdit, buffer, 10);
            seconds = _wtoi(buffer);

            if (hours == 0 && minutes == 0 && seconds == 0) {
                MessageBox(hwnd, L"Please enter a valid time.", L"Error", MB_OK | MB_ICONERROR);
                return 0;
            }

            timerRunning = true;
            SetTimer(hwnd, 1, 1000, TimerProc);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Task Timer";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Task Timer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 250,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
