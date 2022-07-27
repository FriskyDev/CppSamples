#include "targetver.h"

// exclude rarely-used stuff from windows headers
#define WIN32_LEAN_AND_MEAN
// Windows Header Files
#include <Windows.h>

// c runtime header files
#include <cstdlib>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "resource.h"

#define MAX_LOADSTRING 100

////////
// global Variables:

// current instance
HINSTANCE g_hInst;
// the title bar text
WCHAR g_szTitle[MAX_LOADSTRING];
// the main window class name
WCHAR g_szWindowClass[MAX_LOADSTRING];

int docWidth = 10240;
int docHeight = 7680;
SIZE minScroll{ 100, 100 };
SIZE maxScroll{ 0, 0 };
SIZE currentScroll{ 0, 0 };


////////
// forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
VOID OnPaint(HWND hWnd);
VOID OnSize(HWND hWnd, UINT flag, int width, int height);
VOID OnScroll(HWND hWnd, int bar, int request, int currentPos);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BASICWINDOWS, g_szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // perform application initialization:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BASICWINDOWS));

    // main message loop:
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


inline int GetBarDimension(int bar, SIZE size)
{
    switch (bar) {
        case SB_HORZ: return size.cx;
        case SB_VERT: return size.cy;
        default: throw L"invalid bar";
    }
}


inline void SetBarDimension(int bar, SIZE size, int value)
{
    switch (bar) {
        case SB_HORZ: size.cx = value; break;
        case SB_VERT: size.cy = value; break;
        default: throw L"invalid bar";
    }
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BASICWINDOWS));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_BASICWINDOWS);
    wcex.lpszClassName = g_szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    // store instance handle in our global variable
    g_hInst = hInstance;

    auto styles = WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL;
    HWND hWnd = CreateWindowW(g_szWindowClass, g_szTitle, styles, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId) {
                case IDM_ABOUT:
                    DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        }
        case WM_PAINT:
            OnPaint(hWnd);
            break;
        case WM_SIZE:
        {
            const int width = LOWORD(lParam);
            const int height = HIWORD(lParam);

            OnSize(hWnd, UINT(wParam), width, height);
            break;
        }
        case WM_VSCROLL:
        {
            int request = LOWORD(wParam);
            int currentPos = HIWORD(wParam);

            OnScroll(hWnd, SB_VERT, request, currentPos);
            break;
        }
        case WM_HSCROLL:
        {
            int request = LOWORD(wParam);
            int currentPos = HIWORD(wParam);

            OnScroll(hWnd, SB_HORZ, request, currentPos);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
        case WM_INITDIALOG:
            return (INT_PTR) TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR) TRUE;
            }
            break;
    }
    return static_cast<INT_PTR>(FALSE);
}


VOID OnPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(hWnd, &ps);

    COLORREF bk = 0x00202020;
    HBRUSH brush = CreateSolidBrush(bk);
    FillRect(hDC, &ps.rcPaint, brush);

    COLORREF clr{ 0x000079FF };
    HPEN pen = CreatePen(PS_SOLID, 1, clr);
    auto tmpPen = SelectObject(hDC, pen);
    auto tmpBrush = SelectObject(hDC, brush);

    MoveToEx(hDC, 60, 150, nullptr);
    LineTo(hDC, 100, 150);
    LineTo(hDC, 95, 145);
    LineTo(hDC, 95, 155);
    LineTo(hDC, 100, 150);

    Ellipse(hDC, 100, 100, 200, 200);

    Ellipse(hDC, 250, 100, 350, 200);
    Ellipse(hDC, 255, 105, 345, 195);

    MoveToEx(hDC, 150, 100, nullptr);
    SetArcDirection(hDC, AD_CLOCKWISE);
    ArcTo(hDC, 150, 75, 300, 125, 150, 100, 300, 100);
    LineTo(hDC, 292, 95);
    LineTo(hDC, 299, 90);
    LineTo(hDC, 300, 100);

    HFONT font = CreateFont(14, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));
    auto tmpFont = SelectObject(hDC, font);

    RECT rect{ 200, 50, 250, 100 };
    auto tmpBk = SetBkColor(hDC, bk);
    auto tmpClr = SetTextColor(hDC, clr);
    DrawText(hDC, L" 0, 1 ", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    font = CreateFont(24, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));
    SelectObject(hDC, font);
    rect = { 120, 120, 180, 180 };
    DrawText(hDC, L"S\u2080", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    rect = { 270, 120, 330, 180 };
    DrawText(hDC, L"S\u2081", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    font = CreateFont(28, 0, 0, 0, FW_DONTCARE, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe"));
    SelectObject(hDC, font);

    rect = { 100, 20, 350, 50 };
    DrawText(hDC, L"Finite Automata", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hDC, tmpFont);

    SetTextColor(hDC, tmpClr);
    SetBkColor(hDC, tmpBk);
    SelectObject(hDC, tmpPen);
    SelectObject(hDC, tmpBrush);
    DeleteObject(pen);
    DeleteObject(brush);

    EndPaint(hWnd, &ps);
}


VOID OnSize(HWND hWnd, UINT flag, int width, int height)
{
    // the horizontal scrolling range is defined by (docWidth - client_width)
    // the current horizontal scroll value remains within the horizontal scrolling range
    maxScroll.cx = max(docWidth - width, 0);
    currentScroll.cx = min(currentScroll.cx, maxScroll.cx);
    SCROLLINFO si{};
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = minScroll.cx;
    si.nMax = docWidth;
    si.nPage = width;
    si.nPos = currentScroll.cx;
    SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

    // the vertical scrolling range is defined by (docheight - client_height)
    // the current vertical scroll value remains within the vertical scrolling range
    maxScroll.cy = max(docHeight - height, 0);
    currentScroll.cy = min(currentScroll.cy, maxScroll.cy);
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = minScroll.cy;
    si.nMax = docHeight;
    si.nPage = height;
    si.nPos = currentScroll.cy;
    SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
}


VOID OnScroll(HWND hWnd, int bar, int request, int currentPos)
{
    int xDelta = 0;
    int newPos;

    switch (request) {
        case SB_PAGEUP: newPos = GetBarDimension(bar, currentScroll) - 50; break;
        case SB_PAGEDOWN: newPos = GetBarDimension(bar, currentScroll) + 50; break;
        case SB_LINEUP: newPos = GetBarDimension(bar, currentScroll) - 5; break;
        case SB_LINEDOWN: newPos = GetBarDimension(bar, currentScroll) + 5; break;
        case SB_THUMBPOSITION: newPos = currentPos; break;
        default: newPos = GetBarDimension(bar, currentScroll);
    }

    // new position must be between 0 and the max
    newPos = max(0, newPos);
    newPos = min(GetBarDimension(bar, maxScroll), newPos);

    // if the current position does not change, do not scroll
    if (newPos == GetBarDimension(bar, currentScroll)) {
        return;
    }

    // set the scroll flag to true
    //fScroll = TRUE;

    // determine the amount scrolled (in pixels)
    SIZE delta{};
    SetBarDimension(bar, delta, newPos - GetBarDimension(bar, currentScroll));

    // reset the current scroll position
    SetBarDimension(bar, currentScroll, newPos);

    // scroll the window
    // note: the system repaints most of the client area when ScrollWindowEx is called
    //       however, it is necessary to call UpdateWindow in order to repaint the rectangle of pixels that were invalidated
    ScrollWindowEx(hWnd, -delta.cx, -delta.cy, (CONST RECT*) nullptr, (CONST RECT*) nullptr, (HRGN) nullptr, (PRECT) nullptr, SW_INVALIDATE);
    UpdateWindow(hWnd);

    // reset the scroll bar
    SCROLLINFO si{};
    si.cbSize = sizeof(si);
    si.fMask = SIF_POS;
    si.nPos = GetBarDimension(bar, currentScroll);
    SetScrollInfo(hWnd, bar, &si, TRUE);
}
