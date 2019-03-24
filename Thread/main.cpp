#define WINVER 0x0602
#define _WIN32_WINNT 0x0602
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <Windows.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include "../common/Util.hpp"


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

void OutputThreadAndProcessInfo()
{
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
        exit(EXIT_FAILURE);
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
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    OutputThreadAndProcessInfo();

    OutputDebugString(L"Now showing simulated writes to database...\n");
    SampleThreadsWrittingToDabase();

    return EXIT_SUCCESS;
}
