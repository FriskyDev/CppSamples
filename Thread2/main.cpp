
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>

#include "../common/Util.hpp"

namespace chuckeng {
    class CriticalSection final
    {
    public:
        CriticalSection()
        {
            OutputDebugString(L"> Creating\n");

            //InitializeCriticalSection(&cs_);
            InitializeCriticalSectionAndSpinCount(&cs_, 4000);
        }

        ~CriticalSection()
        {
            OutputDebugString(L"< Deleting\n");
            DeleteCriticalSection(&cs_);
        }

        void Enter()
        {
            EnterCriticalSection(&cs_);
            OutputDebugString(L"Entering\n");
        }

        void Leave()
        {
            OutputDebugString(L"Leaving\n");
            LeaveCriticalSection(&cs_);
        }

    private:
        CRITICAL_SECTION cs_;
    };

    class Lock final
    {
    public:
        Lock(CriticalSection& cs) :
            cs_(cs)
        {
            cs_.Enter();
        }

        ~Lock()
        {
            cs_.Leave();
        }

    private:
        CriticalSection& cs_;
    };
}


using namespace chuckeng;

const int MAX_TIMES = 100000;

int g_nIndex = 0;
DWORD g_dwTimes[MAX_TIMES];


DWORD WINAPI FirstThread(PVOID pvParam)
{
    UNREFERENCED_PARAMETER(pvParam);

    auto cs = reinterpret_cast<CriticalSection*>(pvParam);
    while (g_nIndex < MAX_TIMES) {
        Lock lock(*cs);
        g_dwTimes[g_nIndex] = GetTickCount();
        g_nIndex++;
    }

    return 0;
}


DWORD WINAPI SecondThread(PVOID pvParam)
{
    UNREFERENCED_PARAMETER(pvParam);

    auto cs = reinterpret_cast<CriticalSection*>(pvParam);
    while (g_nIndex < MAX_TIMES) {
        Lock lock(*cs);
        g_nIndex++;
        g_dwTimes[g_nIndex] = GetTickCount();
    }

    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    CriticalSection cs;
    const int THREADCOUNT = 2;
    HANDLE threads[THREADCOUNT]{};
    try {
        Timer timer;
        timer.start();
        // create worker threads
        LPTHREAD_START_ROUTINE threadStarts[] = { FirstThread, SecondThread, };
        for (int i = 0; i < THREADCOUNT; i++) {
            DWORD tid;
            threads[i] = CreateThread(nullptr, 0, threadStarts[i], &cs, 0, &tid);
            if (threads[i] == nullptr) {
                auto msg = string_format(L"CreateThread error: (%d) ", GetLastError());
                msg += get_system_error_message();
                msg += L"\n";
                OutputDebugString(msg.c_str());
                return EXIT_FAILURE;
            } else {
                auto msg = string_format(L"Created thread: %u\n", tid);
                OutputDebugString(msg.c_str());
            }
        }

        // wait for all threads to terminate
        WaitForMultipleObjects(THREADCOUNT, threads, TRUE, INFINITE);
        timer.stop();
        {
            auto msg = string_format(L"Took: %f ms\n", timer.elapsedMilliseconds());
            OutputDebugString(msg.c_str());
        }
    }
    catch (...) {
        OutputDebugString(L"Unhandled exception thrown\n");
    }

    // close thread and mutex handles
    for (int i = 0; i < THREADCOUNT; i++) {
        CloseHandle(threads[i]);
    }

    exit(EXIT_SUCCESS);
}
