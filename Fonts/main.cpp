#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <cstdlib>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static struct
    {
        int idStockFont;
        const WCHAR* szStockFont;
    } stockFont[] = {
            OEM_FIXED_FONT, L"OEM_FIXED_FONT",
            ANSI_FIXED_FONT, L"ANSI_FIXED_FONT",
            ANSI_VAR_FONT, L"ANSI_VAR_FONT",
            SYSTEM_FONT, L"SYSTEM_FONT",
            DEVICE_DEFAULT_FONT, L"DEVICE_DEFAULT_FONT",
            SYSTEM_FIXED_FONT, L"SYSTEM_FIXED_FONT",
            DEFAULT_GUI_FONT, L"DEFAULT_GUI_FONT",

    };

    static int iFont;
    static int cFonts = sizeof(stockFont) / sizeof(stockFont[0]);
    HDC hDC;
    int i, x, y, cxGrid, cyGrid;
    PAINTSTRUCT ps{};
    WCHAR szFaceName[LF_FACESIZE], szBuffer[LF_FACESIZE + 64];
    TEXTMETRIC tm{};

    switch (message) {
        case WM_CREATE:
            SetScrollRange(hWnd, SB_VERT, 0, cFonts - 1, TRUE);
            return 0;

        case WM_DISPLAYCHANGE:
            InvalidateRect(hWnd, nullptr, TRUE);
            return 0;

        case WM_VSCROLL:
            switch (LOWORD(wParam)) {
                case SB_TOP: iFont = 0; break;
                case SB_BOTTOM: iFont = cFonts - 1; break;
                case SB_LINEUP:
                case SB_PAGEUP:iFont -= 1; break;
                case SB_LINEDOWN:
                case SB_PAGEDOWN: iFont += 1; break;
                case SB_THUMBPOSITION: iFont = HIWORD(wParam);
            }
            iFont = max(0, min(cFonts - 1, iFont));
            SetScrollPos(hWnd, SB_VERT, iFont, TRUE);
            InvalidateRect(hWnd, nullptr, TRUE);

        case WM_KEYDOWN:
            switch (wParam) {
                case VK_HOME: SendMessage(hWnd, WM_VSCROLL, SB_TOP, 0); break;
                case VK_END: SendMessage(hWnd, WM_VSCROLL, SB_BOTTOM, 0); break;
                case VK_PRIOR:
                case VK_LEFT:
                case VK_UP: SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, 0); break;
                case VK_NEXT:
                case VK_RIGHT:
                case VK_DOWN: SendMessage(hWnd, WM_VSCROLL, SB_PAGEDOWN, 0); break;
            }
            return 0;

        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);

            SelectObject(hDC, GetStockObject(stockFont[iFont].idStockFont));
            GetTextFace(hDC, LF_FACESIZE, szFaceName);
            cxGrid = max(3 * tm.tmAveCharWidth, 2 * tm.tmMaxCharWidth);
            cyGrid = tm.tmHeight + 3;

            TextOut(hDC, 0, 0, szBuffer,
                wsprintf(szBuffer, L" %s: Face name = %s, CharSet = %i", stockFont[iFont].szStockFont, szFaceName, tm.tmCharSet));

            SetTextAlign(hDC, TA_TOP | TA_CENTER);

            for (i = 0; i < 17; i++) {
                MoveToEx(hDC, (i + 2)*cxGrid, 2 * cyGrid, nullptr);
                LineTo(hDC, (i + 2)* cxGrid, 19 * cyGrid);

                MoveToEx(hDC, cxGrid, (i + 3) * cyGrid, nullptr);
                LineTo(hDC, 18 * cxGrid, (i + 3) * cyGrid);
            }

            for (i = 0; i < 16; i++) {
                TextOut(hDC, (2 * i + 5)*cxGrid / 2, 2 * cyGrid + 2, szBuffer,
                    wsprintf(szBuffer, L"%X-", i));
                TextOut(hDC, 3 * cxGrid / 2, (i + 3) * cyGrid + 2, szBuffer,
                    wsprintf(szBuffer, L"-%X", i));
            }

            for (y = 0; y < 16; y++) {
                for (x = 0; x < 16; x++) {
                    TextOut(hDC, (2 * x + 5)*cxGrid / 2, (y + 3)*cyGrid + 2, szBuffer,
                        wsprintf(szBuffer, L"%c", 16 * x + y));
                }
            }

            EndPaint(hWnd, &ps);

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static WCHAR szAppName[] = L"Fonts";

    WNDCLASS wndclass{};
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = nullptr;
    wndclass.lpszClassName = szAppName;
    if (!RegisterClass(&wndclass)) {
        MessageBox(nullptr, L"Program requires NT or better", szAppName, MB_ICONERROR);
        return 0;
    }


    HWND hWnd = CreateWindow(szAppName, L"Stock Fonts", WS_OVERLAPPEDWINDOW | WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return EXIT_SUCCESS;
}
