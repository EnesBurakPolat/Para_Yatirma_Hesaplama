#include <windows.h>
#include <string>
#include <sstream>
#include <iomanip> // for fixed and setprecision

#define IDC_RATE_EDIT 101
#define IDC_WITHDRAWAL_EDIT 102
#define IDC_RESULT_LABEL 103
#define IDC_INTEREST_LABEL 105
#define IDC_CALCULATE_BUTTON 104

// g++ -o Para_Yatirma_Hesaplama.exe Para_Yatirma_Hesaplama.cpp -mwindows

using namespace std;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hRateEdit, hWithdrawalEdit, hResultLabel, hInterestLabel, hCalculateButton;

    switch (uMsg) {
        case WM_CREATE:
            CreateWindowW(L"STATIC", L"Faiz Orani (%):", WS_VISIBLE | WS_CHILD, 50, 50, 120, 20, hwnd, NULL, NULL, NULL);
            hRateEdit = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 180, 50, 100, 20, hwnd, (HMENU)IDC_RATE_EDIT, NULL, NULL);

            CreateWindowW(L"STATIC", L"Cekilecek Miktar:", WS_VISIBLE | WS_CHILD, 50, 80, 120, 20, hwnd, NULL, NULL, NULL);
            hWithdrawalEdit = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 180, 80, 100, 20, hwnd, (HMENU)IDC_WITHDRAWAL_EDIT, NULL, NULL);

            CreateWindowW(L"STATIC", L"Yatirilmasi Gereken Miktar:", WS_VISIBLE | WS_CHILD, 50, 110, 100, 35, hwnd, NULL, NULL, NULL);
            hResultLabel = CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 210, 110, 100, 35, hwnd, (HMENU)IDC_RESULT_LABEL, NULL, NULL);

            // Faiz miktarını gösterecek yeni label
            CreateWindowW(L"STATIC", L"Alinan Faiz:", WS_VISIBLE | WS_CHILD, 50, 150, 120, 30, hwnd, NULL, NULL, NULL);
            hInterestLabel = CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 180, 150, 100, 32, hwnd, (HMENU)IDC_INTEREST_LABEL, NULL, NULL);

            hCalculateButton = CreateWindowW(L"BUTTON", L"Hesapla", WS_VISIBLE | WS_CHILD, 150, 190, 100, 32, hwnd, (HMENU)IDC_CALCULATE_BUTTON, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_CALCULATE_BUTTON) {
                wchar_t rateText[20], withdrawalText[20];
                GetWindowTextW(hRateEdit, rateText, 20);
                GetWindowTextW(hWithdrawalEdit, withdrawalText, 20);

                double rate = _wtof(rateText) / 100;
                double withdrawal = _wtof(withdrawalText);

                double requiredAmount = withdrawal / (1 + rate);
                double interestAmount = withdrawal - requiredAmount;

                wostringstream resultStr, interestStr;
                resultStr << L"Gerekli Miktar: " << fixed << setprecision(2) << requiredAmount << L" TL";
                interestStr << L"Alinan Faiz: " << fixed << setprecision(2) << interestAmount << L" TL";

                SetWindowTextW(hResultLabel, resultStr.str().c_str());
                SetWindowTextW(hInterestLabel, interestStr.str().c_str());
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        WS_EX_CLIENTEDGE, // Optional: Provides a border
        CLASS_NAME,
        L"Para_Yatirma_Hesaplama",
        WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_SIZEBOX), // Remove maximize and resize box
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
