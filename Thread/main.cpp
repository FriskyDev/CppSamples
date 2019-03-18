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
std::wstring string_format(const std::wstring& format, Args ... args)
{
    size_t size = swprintf(nullptr, 0, format.c_str(), args ...) + 1;
    std::unique_ptr<wchar_t[]> buf(new wchar_t[size]);
    swprintf(buf.get(), size, format.c_str(), args ...);
    return std::wstring(buf.get(), buf.get() + size - 1);
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

void WINAPI FiberFunc(LPVOID lpParameter)
{
    UNREFERENCED_PARAMETER(lpParameter);

    // does nothing and ends
    OutputDebugString(L"Hello from the fiber\n");

    SwitchToFiber(g_fiber);
}

HANDLE ghMutex;


DWORD WINAPI WriteToDatabase(LPVOID lpParam)
{
    // lpParam not used in this example
    UNREFERENCED_PARAMETER(lpParam);

    DWORD dwCount = 0;
    DWORD dwWaitResult{};

    // request ownership of mutex
    while (dwCount < 20) {
        dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);

        switch (dwWaitResult) {
            case WAIT_OBJECT_0:
            {
                // the thread got ownership of the mutex
                try {
                    // TODO: write to the database
                    auto msg = string_format(L"Thread %d writing to database...\n", GetCurrentThreadId());
                    OutputDebugString(msg.c_str());
                    dwCount++;
                }
                catch (...) {}

                // release ownership of the mutex object
                if (!ReleaseMutex(ghMutex)) {
                    // handle error
                }
            }
            break;

            case WAIT_ABANDONED:
                // the thread got ownership of an abandoned mutex
                // the database is in an indeterminate state
                return FALSE;
        }
    }
    return TRUE;
}

#define THREADCOUNT 20

void SampleThreadsWrittingToDabase()
{
    HANDLE aThread[THREADCOUNT];
    DWORD ThreadID;

    // Create a mutex with no initial owner

    ghMutex = CreateMutex(
        nullptr,           // default security attributes
        FALSE,             // initially not owned
        nullptr);          // unnamed mutex
    if (ghMutex == nullptr) {
        auto msg = string_format(L"CreateMutex error: %d\n", GetLastError());
        OutputDebugString(msg.c_str());
        return;
    }

    // create worker threads
    for (int i = 0; i < THREADCOUNT; i++) {
        aThread[i] = CreateThread(
            nullptr,    // default security attributes
            0,          // default stack size
            (LPTHREAD_START_ROUTINE)WriteToDatabase,
            nullptr,    // no thread function arguments
            0,          // default creation flags
            &ThreadID); // receive thread identifier
        if (aThread[i] == nullptr) {
            auto msg = string_format(L"CreateThread error: %d\n", GetLastError());
            OutputDebugString(msg.c_str());
            return;
        }
    }

    // wait for all threads to terminate
    WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, INFINITE);

    // close thread and mutex handles
    for (int i = 0; i < THREADCOUNT; i++) {
        CloseHandle(aThread[i]);
    }

    CloseHandle(ghMutex);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();
    {
        auto msg = string_format(L"Process: %d (0x%08X)  Thread: %d (0x%08X)\n", pid, pid, tid, tid);
        OutputDebugString(msg.c_str());
    }

    HANDLE th = GetCurrentThread();

    CONTEXT ctx{};
    BOOL result = GetThreadContext(th, &ctx);
    if (!result) {
        auto err = GetLastError();
        auto errMsg = GetFormatedMessage(err);
        auto msg = string_format(L"GetThreadContext() returned 0x%08X (%d): %s\n", err, err, errMsg.c_str());
        MessageBox(nullptr, msg.c_str(), L"Thread", MB_ICONERROR);
        return EXIT_FAILURE;
    }

    //WOW64_CONTEXT ctx32{};
    //result = Wow64GetThreadContext(th, &ctx32);
    //if (!result) {
    //    auto err = GetLastError();
    //    auto errMsg = GetFormatedMessage(err);
    //    auto msg = string_format(L"Wow64GetThreadContext() returned 0x%08X (%d): %s\n\nMust not be a 32-bit application on a 64-bit OS.", err, err, errMsg.c_str());
    //    MessageBox(nullptr, msg.c_str(), "Thread", MB_ICONERROR);
    //    return EXIT_FAILURE;
    //}

    g_fiber = ConvertThreadToFiber(nullptr);
    if (!g_fiber) {
        auto err = GetLastError();
        auto errMsg = GetFormatedMessage(err);
        auto msg = string_format(L"ConvertThreadToFiber() returned 0x%08X (%d): %s\n", err, err, errMsg.c_str());
        MessageBox(nullptr, msg.c_str(), L"Thread", MB_ICONERROR);
        return EXIT_FAILURE;
    }

    auto child = CreateFiber(0, FiberFunc, nullptr);
    if (!child) {
        auto err = GetLastError();
        auto errMsg = GetFormatedMessage(err);
        auto msg = string_format(L"CreateFiber() returned 0x%08X (%d): %s\n", err, err, errMsg.c_str());
        MessageBox(nullptr, msg.c_str(), L"Thread", MB_ICONERROR);
        return EXIT_FAILURE;
    }

    SwitchToFiber(child);

    DeleteFiber(child);
    OutputDebugString(L"Fiber ended and deleted\n");

    OutputDebugString(L"Now showing simulated writes to database...\n");
    SampleThreadsWrittingToDabase();

    return EXIT_SUCCESS;
}
