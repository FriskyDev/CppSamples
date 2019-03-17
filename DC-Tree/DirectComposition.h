#pragma once

// exclude rarely-used items from Windows headers
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <d3d11.h>
#include <dcomp.h>

#include "Util.hpp"


struct Direct3D : public ComPtr<ID3D11Device>
{
    D3D_FEATURE_LEVEL featureLevelSupported{};

    Direct3D();
    ~Direct3D() override;
};

struct DXGraphicInfrastructureDevice : public ComPtr<IDXGIDevice>
{
    DXGraphicInfrastructureDevice(const Direct3D& d3d);
};

struct DirectComposition : public ComPtr<IDCompositionDevice>
{
    DirectComposition(const DXGraphicInfrastructureDevice& dxgi);
};

struct CompositionTarget : public ComPtr<IDCompositionTarget>
{
    CompositionTarget(const DirectComposition& dcomp, HWND hwnd);
};

struct Visual : public ComPtr<IDCompositionVisual>
{
    Visual(const DirectComposition& dcomp);
};

HRESULT CreateGDIRenderedDCompSurface(HBITMAP hBitmap, const DirectComposition& dcomp, IDCompositionSurface **ppSurface);
