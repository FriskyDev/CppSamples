#include "App.hpp"

#include <cmath>

#include "Util.hpp"
#include "EnumResource.hpp"
#include "resource.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

App::App() :
    m_hwnd(nullptr),
    m_hBitmap(nullptr),
    m_pDCompDevice(nullptr),
    m_pDCompTarget(nullptr),
    m_pD3D11Device(nullptr)
{
}

App::~App()
{
    SafeRelease(&m_pDCompDevice);
    SafeRelease(&m_pDCompTarget);
    SafeRelease(&m_pD3D11Device);
}

HRESULT App::Initialize()
{
    // register the window class
    WNDCLASSEX wcex{ sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = App::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(LONG_PTR);
    wcex.hInstance = HINST_THISCOMPONENT;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);;
    wcex.lpszMenuName = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = L"DirectCompDemoApp";

    ATOM atom = RegisterClassEx(&wcex);
    if (atom == 0) {
        auto msg = ErrorMessage(L"registering window class");
        OutputDebugString(msg.c_str());
    }

    // Create the application window.
    //
    // Because the CreateWindow function takes its size in pixels, we
    // obtain the system DPI and use it to scale the window size.
    int dpiX = 0;
    int dpiY = 0;
    HDC hdc = GetDC(nullptr);
    if (hdc) {
        dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
        dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
        ReleaseDC(nullptr, hdc);
    }

    m_hwnd = CreateWindow(
        L"DirectCompDemoApp",
        L"DirectComposition Demo Application",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
        static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
        nullptr,
        nullptr,
        HINST_THISCOMPONENT,
        this
    );

    HRESULT hr = m_hwnd ? S_OK : E_FAIL;
    if (SUCCEEDED(hr)) {
        ShowWindow(m_hwnd, SW_SHOWNORMAL);
        UpdateWindow(m_hwnd);

        // Initialize DirectComposition resources, such as the
        // device object and composition target object.
        hr = InitializeDirectCompositionDevice();
    }

    if (SUCCEEDED(hr)) {
        hr = CreateResources();
    }

    return hr;
}

HRESULT App::InitializeDirectCompositionDevice()
{
    // Create the D3D device object. The D3D11_CREATE_DEVICE_BGRA_SUPPORT
    // flag enables rendering on surfaces using Direct2D.
    D3D_FEATURE_LEVEL featureLevelSupported;
    HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &m_pD3D11Device,
        &featureLevelSupported,
        nullptr);


    // Check the result of calling D3D11CreateDriver.
    IDXGIDevice *pDXGIDevice = nullptr;
    if (SUCCEEDED(hr)) {
        // Create the DXGI device used to create bitmap surfaces.
        hr = m_pD3D11Device->QueryInterface(&pDXGIDevice);
    }

    if (SUCCEEDED(hr)) {
        // Create the DirectComposition device object.
        hr = DCompositionCreateDevice(pDXGIDevice, __uuidof(IDCompositionDevice), reinterpret_cast<void **>(&m_pDCompDevice));
    }
    if (SUCCEEDED(hr)) {
        // Create the composition target object based on the 
        // specified application window.
        hr = m_pDCompDevice->CreateTargetForHwnd(m_hwnd, TRUE, &m_pDCompTarget);
    }

    SafeRelease(&pDXGIDevice);

    return hr;
}

HRESULT App::CreateResources()
{
    //return LoadResourceGDIBitmap(L"Image", m_hBitmap);
    //return LoadResourceGDIBitmap(L"#113", m_hBitmap);
    //return LoadResourceGDIBitmap(L"IDB_LOGO", m_hBitmap);
    //return LoadResourceGDIBitmap(MAKEINTRESOURCE(IDB_LOGO), m_hBitmap);
    return LoadResourceGDIBitmap(MAKEINTRESOURCE(IDB_LOGO), m_hBitmap);
}

void App::DiscardResources()
{
    DeleteObject(m_hBitmap);
}

void App::RunMessageLoop()
{
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

HRESULT App::OnClientClick()
{
    HRESULT hr = S_OK;
    float xOffset = 20; // horizonal position of visual
    float yOffset = 20; // vertical position of visual

    // create a visual object
    IDCompositionVisual *pVisual = nullptr;
    hr = m_pDCompDevice->CreateVisual(&pVisual);

    IDCompositionSurface *pSurface = nullptr;
    if (SUCCEEDED(hr)) {
        // create a composition surface and render a gdi bitmap to the surface
        hr = MyCreateGDIRenderedDCompSurface(m_hBitmap, &pSurface);
    }

    if (SUCCEEDED(hr)) {
        // set the bitmap content of the visual
        hr = pVisual->SetContent(pSurface);
    }

    if (SUCCEEDED(hr)) {
        // Set the horizontal and vertical position of the visual relative
        // to the upper-left corner of the composition target window.
        hr = pVisual->SetOffsetX(xOffset);
        if (SUCCEEDED(hr)) {
            hr = pVisual->SetOffsetY(yOffset);
        }
    }

    if (SUCCEEDED(hr)) {
        // set the visual to be the root of the visual tree
        hr = m_pDCompTarget->SetRoot(pVisual);
    }

    if (SUCCEEDED(hr)) {
        // commit the visual to be composed and displayed
        hr = m_pDCompDevice->Commit();
    }

    // free the visual
    SafeRelease(&pVisual);

    return hr;
}

LRESULT CALLBACK App::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    if (message == WM_CREATE) {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        App *pApp = (App *)pcs->lpCreateParams;

        ::SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            PtrToUlong(pApp)
        );

        result = 1;
    } else {
        App *pApp = reinterpret_cast<App *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));

        bool wasHandled = false;

        if (pApp) {
            switch (message) {
                case WM_LBUTTONDOWN:
                {
                    pApp->OnClientClick();
                }
                wasHandled = true;
                result = 0;
                break;

                case WM_DISPLAYCHANGE:
                {
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
                wasHandled = true;
                result = 0;
                break;

                case WM_DESTROY:
                {
                    PostQuitMessage(0);
                    pApp->DiscardResources();
                }
                wasHandled = true;
                result = 1;
                break;
            }
        }

        if (!wasHandled) {
            result = DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    return result;
}

HRESULT App::LoadResourceGDIBitmap(PCWSTR resourceName, HBITMAP &hbmp)
{
    hbmp = static_cast<HBITMAP>(LoadImage(HINST_THISCOMPONENT, resourceName, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR));
    if (hbmp == nullptr) {
        auto msg = ErrorMessage(L"creating resources");
        MessageBox(nullptr, msg.c_str(), L"Failed to load image", MB_OK);

        EnumResourceTypes(HINST_THISCOMPONENT, (ENUMRESTYPEPROC)EnumTypesFunc,0);

        return E_FAIL;
    }

    return S_OK;
}

HRESULT App::MyCreateGDIRenderedDCompSurface(HBITMAP hBitmap, IDCompositionSurface **ppSurface)
{
    HRESULT hr = S_OK;

    int bitmapWidth = 0;
    int bitmapHeight = 0;
    int bmpSize = 0;
    BITMAP bmp = { };
    HBITMAP hBitmapOld = nullptr;

    HDC hSurfaceDC = nullptr;
    HDC hBitmapDC = nullptr;

    IDXGISurface1 *pDXGISurface = nullptr;
    IDCompositionSurface *pDCSurface = nullptr;
    POINT pointOffset = { };

    if (ppSurface == nullptr)
        return E_INVALIDARG;

    // Get information about the bitmap.
    bmpSize = GetObject(hBitmap, sizeof(BITMAP), &bmp);

    hr = bmpSize ? S_OK : E_FAIL;
    if (SUCCEEDED(hr)) {
        // Save the bitmap dimensions.
        bitmapWidth = bmp.bmWidth;
        bitmapHeight = bmp.bmHeight;

        // Create a DirectComposition-compatible surface that is the same size 
        // as the bitmap. The DXGI_FORMAT_B8G8R8A8_UNORM flag is required for 
        // rendering on the surface using GDI via GetDC.
        hr = m_pDCompDevice->CreateSurface(bitmapWidth, bitmapHeight,
            DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ALPHA_MODE_IGNORE, &pDCSurface);
    }

    if (SUCCEEDED(hr)) {
        // Begin rendering to the surface.
        hr = pDCSurface->BeginDraw(nullptr, __uuidof(IDXGISurface1),
            reinterpret_cast<void**>(&pDXGISurface), &pointOffset);
    }

    if (SUCCEEDED(hr)) {
        // Get the device context (DC) for the surface.
        hr = pDXGISurface->GetDC(FALSE, &hSurfaceDC);
    }

    if (SUCCEEDED(hr)) {
        // Create a compatible DC and select the surface 
        // into the DC.
        hBitmapDC = CreateCompatibleDC(hSurfaceDC);
        if (hBitmapDC != nullptr) {
            hBitmapOld = (HBITMAP)SelectObject(hBitmapDC, hBitmap);
            BitBlt(hSurfaceDC, pointOffset.x, pointOffset.y,
                bitmapWidth, bitmapHeight, hBitmapDC, 0, 0, SRCCOPY);

            if (hBitmapOld) {
                SelectObject(hBitmapDC, hBitmapOld);
            }
            DeleteDC(hBitmapDC);
        }

        pDXGISurface->ReleaseDC(nullptr);
    }

    // End the rendering.
    pDCSurface->EndDraw();
    *ppSurface = pDCSurface;

    // Call an application-defined macro to free the surface pointer.
    SafeRelease(&pDXGISurface);

    return hr;
}