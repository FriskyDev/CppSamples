// exclude rarely-used items from Windows headers
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <d3d11.h>
#include <dcomp.h>

#include <cstdio>
#include <cmath>

#include "App.hpp"
#include "Util.hpp"
#include "ComponentObjectModel.hpp"

ComponentObjectModel com;


#if false
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Direct3D d3d;

    if (FAILED(d3d.status)) {
        printf("error: failed to initialize directX 3D (%ld)\n", d3d.status);
        exit(1);
    }

    DXGraphicInfrastructureDevice dxgi(d3d);
    if (FAILED(dxgi.status)) {
        printf("error: failed to initialize directX graphics infrastructure (%ld)\n", dxgi.status);
        exit(1);
}

    DirectComposition dcomp(dxgi);
}
#else

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    if (SUCCEEDED(com.status)) {
        com.hinst = hInstance;
        {
            App app;
            HRESULT hr = app.Initialize();
            if (SUCCEEDED(hr)) {
                app.RunMessageLoop();
            }
        }
    }

    return EXIT_SUCCESS;
}

#endif