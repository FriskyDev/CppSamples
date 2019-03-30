#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdlib>

#include "../common/Util.hpp"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    auto h = ::CreateFile(L"does_not_exist", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) {
        auto err = get_system_error_message();
        auto msg = string_format(L"Error: failed to open file: %s", err.c_str());
        MessageBox(nullptr, msg.c_str(), L"CreateFile", MB_ICONINFORMATION);
    }

    return EXIT_SUCCESS;
}
