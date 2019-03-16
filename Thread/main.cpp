#define WINVER 0x0602
#define _WIN32_WINNT 0x0602
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif




template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
    size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

// Returns the error code as a formatted message.
std::tstring GetFormatedMessage(DWORD err = 0)
{
    if (err == 0) {
        err = GetLastError();
    }

    LPTSTR lpMsgBuf{};
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message and exit the process
    std::tstring result = lpMsgBuf;
    LocalFree(lpMsgBuf);
    return result;
}

LPVOID g_fiber;

void FiberFunc(LPVOID lpParameter)
{
    UNREFERENCED_PARAMETER(lpParameter);

    // does nothing and ends
    OutputDebugString(TEXT("Hello from the fiber\n"));

    SwitchToFiber(g_fiber);
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();
    {
        auto msg = string_format(TEXT("Process: %d (0x%08X)  Thread: %d (0x%08X)\n"), pid, pid, tid, tid);
        OutputDebugString(msg.c_str());
    }

    HANDLE th = GetCurrentThread();

    CONTEXT ctx{};
    BOOL result = GetThreadContext(th, &ctx);
    if (!result) {
        auto err = GetLastError();
        auto msg = GetFormatedMessage(err);
        TCHAR buf[128];
        wsprintf(buf, "GetThreadContext() returned 0x%08X (%d):\n", err, err);
        msg = buf + msg;
        MessageBox(nullptr, msg.c_str(), "Thread", MB_ICONERROR);
        return EXIT_FAILURE;
    }

    //WOW64_CONTEXT ctx32{};
    //result = Wow64GetThreadContext(th, &ctx32);
    //if (!result) {
    //    auto err = GetLastError();
    //    auto msg = GetFormatedMessage(err);
    //    TCHAR buf[128];
    //    wsprintf(buf, "Wow64GetThreadContext() returned 0x%08X (%d):\n", err, err);
    //    msg = buf + msg + "\nMust not be a 32-bit application on a 64-bit OS.";
    //    MessageBox(nullptr, msg.c_str(), "Thread", MB_ICONERROR);
    //    return EXIT_FAILURE;
    //}

    g_fiber = ConvertThreadToFiber(nullptr);
    if (!g_fiber) {
        auto err = GetLastError();
        auto msg = GetFormatedMessage(err);
        TCHAR buf[128];
        wsprintf(buf, "ConvertThreadToFiber() returned 0x%08X (%d):\n", err, err);
        msg = buf + msg;
        MessageBox(nullptr, msg.c_str(), "Thread", MB_ICONERROR);
        return EXIT_FAILURE;
    }

    auto child = CreateFiber(0, FiberFunc, nullptr);
    if (!child) {
        auto err = GetLastError();
        auto msg = GetFormatedMessage(err);
        TCHAR buf[128];
        wsprintf(buf, "CreateFiber() returned 0x%08X (%d):\n", err, err);
        msg = buf + msg;
        MessageBox(nullptr, msg.c_str(), "Thread", MB_ICONERROR);
        return EXIT_FAILURE;
    }

    SwitchToFiber(child);

    DeleteFiber(child);

    return EXIT_SUCCESS;
}
