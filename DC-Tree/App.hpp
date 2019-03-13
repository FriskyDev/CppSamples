#pragma once

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used items from Windows headers

#include <Windows.h>

#include <d3d11.h>
#include <dcomp.h>

#include "ComponentObjectModel.hpp"

extern ComponentObjectModel com;

class App
{
public:
    App();
    ~App();

    HRESULT Initialize();

    void RunMessageLoop();

private:
    HRESULT InitializeDirectCompositionDevice();

    HRESULT CreateResources();
    void DiscardResources();

    HRESULT OnClientClick();

    HRESULT LoadResourceGDIBitmap(PCWSTR resourceName, HBITMAP &hbmp
    );

    HRESULT MyCreateGDIRenderedDCompSurface(HBITMAP hBitmap, IDCompositionSurface **ppSurface);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hwnd;
    HBITMAP m_hBitmap;
    ID3D11Device *m_pD3D11Device;
    IDCompositionDevice *m_pDCompDevice;
    IDCompositionTarget *m_pDCompTarget;
};