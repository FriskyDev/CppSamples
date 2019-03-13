#include "DirectComposition.h"


Direct3D::Direct3D()
{
    // Create the D3D device object. The D3D11_CREATE_DEVICE_BGRA_SUPPORT
    // flag enables rendering on surfaces using Direct2D.
    status = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &ptr,
        &featureLevelSupported,
        nullptr);
}

Direct3D::~Direct3D()
{
    status = E_FAIL;
}

DXGraphicInfrastructureDevice::DXGraphicInfrastructureDevice(const Direct3D& d3d)
{
    if (SUCCEEDED(d3d.status)) {
        status = d3d.ptr->QueryInterface(&ptr);
    }
}

DirectComposition::DirectComposition(const DXGraphicInfrastructureDevice& dxgi)
{
    if (SUCCEEDED(dxgi.status)) {
        // create the directcomposition device object
        status = DCompositionCreateDevice(dxgi.ptr, __uuidof(IDCompositionDevice), reinterpret_cast<void**>(&ptr));
    }
}

CompositionTarget::CompositionTarget(const DirectComposition& dcomp, HWND hwnd)
{
    status = dcomp.ptr->CreateTargetForHwnd(hwnd, TRUE, &ptr);
}

Visual::Visual(const DirectComposition& dcomp)
{
    status = dcomp.ptr->CreateVisual(&ptr);
}


HRESULT CreateGDIRenderedDCompSurface(HBITMAP hBitmap, const DirectComposition& dcomp, IDCompositionSurface **ppSurface)
{
    if (ppSurface == nullptr) {
        return E_INVALIDARG;
    }

    // get information about the bitmap
    BITMAP bmp{};
    int bmpSize = GetObject(hBitmap, sizeof(BITMAP), &bmp);

    int bitmapWidth = 0;
    int bitmapHeight = 0;
    IDCompositionSurface* pDCSurface = nullptr;
    HRESULT hr = bmpSize ? S_OK : E_FAIL;
    if (SUCCEEDED(hr)) {
        // save the bitmap dimensions
        bitmapWidth = bmp.bmWidth;
        bitmapHeight = bmp.bmHeight;

        // create a DirectComposition-compatible surface that is the same size as the bitmap
        // (the DXGI_FORMAT_B8G8R8A8_UNORM flag is required for rendering on the surface using GDI via GetDC)
        hr = dcomp.ptr->CreateSurface(bitmapWidth, bitmapHeight, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ALPHA_MODE_IGNORE, &pDCSurface);
    }

    IDXGISurface1* pDXGISurface = nullptr;
    POINT pointOffset = { };
    if (SUCCEEDED(hr)) {
        // begin rendering to the surface
        hr = pDCSurface->BeginDraw(nullptr, __uuidof(IDXGISurface1), reinterpret_cast<void**>(&pDXGISurface), &pointOffset);
    }

    HDC hSurfaceDC = nullptr;
    if (SUCCEEDED(hr)) {
        // get the device context (DC) for the surface
        hr = pDXGISurface->GetDC(FALSE, &hSurfaceDC);
    }

    HDC hBitmapDC = nullptr;
    if (SUCCEEDED(hr)) {
        // create a compatible dc and select the surface into the dc
        hBitmapDC = CreateCompatibleDC(hSurfaceDC);
        if (hBitmapDC != nullptr) {
            HBITMAP hBitmapOld = (HBITMAP)SelectObject(hBitmapDC, hBitmap);
            BitBlt(hSurfaceDC, pointOffset.x, pointOffset.y, bitmapWidth, bitmapHeight, hBitmapDC, 0, 0, SRCCOPY);

            if (hBitmapOld) {
                SelectObject(hBitmapDC, hBitmapOld);
            }
            DeleteDC(hBitmapDC);
        }

        pDXGISurface->ReleaseDC(nullptr);
    }

    // end the rendering
    pDCSurface->EndDraw();
    *ppSurface = pDCSurface;

    // call an application-defined macro to free the surface pointer
    SafeRelease(&pDXGISurface);

    return hr;
}
