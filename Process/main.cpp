
#define WIN32_LEAN_AND_MEAN
// exclude rarely-used items from Windows headers

#include <Windows.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include <strsafe.h>


template<typename ... Args>
std::wstring string_format(const std::wstring& format, Args ... args)
{
    // extra space for '\0'
    size_t size = swprintf(nullptr, 0, format.c_str(), args ...) + 1;
    std::unique_ptr<wchar_t[]> buf(new wchar_t[size]);
    swprintf(buf.get(), size, format.c_str(), args ...);
    // we don't want the '\0' inside
    return std::wstring(buf.get(), buf.get() + size - 1);
}

std::wstring ErrorMessage(DWORD error = 0)
{
    if (error == 0) {
        error = GetLastError();
    }

    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, nullptr);

    std::wstring errorMessage = (LPCWSTR)lpMsgBuf;
    LocalFree(lpMsgBuf);

    //errorMessage.erase(std::remove(errorMessage.begin(), errorMessage.end(), L'\r'), errorMessage.end());
    //errorMessage.erase(std::remove(errorMessage.begin(), errorMessage.end(), L'\n'), errorMessage.end());

    return errorMessage;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    const wchar_t szApplicationName[] = L"C:\\Windows\\System32\\cmd.exe";
    wchar_t szCommandLine[MAX_PATH]{ L"" };
    wchar_t szAppTitle[] = L"Sample Application";
    STARTUPINFO startupInfo{};
    startupInfo.lpTitle = szAppTitle;
    PROCESS_INFORMATION processInfo{};

    BOOL r = CreateProcess(szApplicationName, szCommandLine, nullptr, nullptr, false, CREATE_NEW_CONSOLE, nullptr, nullptr, &startupInfo, &processInfo);
    if (!r) {
        auto msg = ErrorMessage();
        MessageBox(nullptr, msg.c_str(), szAppTitle, MB_ICONERROR);
        return EXIT_FAILURE;
    }

    auto msg = string_format(L"Created --> Process(%d) Thread(%d)\n", processInfo.dwProcessId, processInfo.dwThreadId);
    OutputDebugString(msg.c_str());

    OutputDebugString(L"Waiting for process to exit...\n");
    WaitForSingleObject(processInfo.hProcess, INFINITE);

    OutputDebugString(L"Process completed\n");


    return EXIT_SUCCESS;
}
