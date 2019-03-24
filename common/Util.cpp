#include "Util.hpp"

#define WINVER 0x0602
#define _WIN32_WINNT 0x0602
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <Windows.h>


// Returns the error code as a formatted message.
std::tstring get_system_error_message(uint32_t err /*= 0*/)
{
    if (err == 0) {
        err = GetLastError();
    }

    LPTSTR lpMsgBuf{};
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, nullptr);

    // display the error message and exit the process
    std::tstring result = lpMsgBuf;
    LocalFree(lpMsgBuf);
    return result;
}

void Timer::start()
{
    start_ = std::chrono::system_clock::now();
    isRunning_ = true;
}

void Timer::stop()
{
    end_ = std::chrono::system_clock::now();
    isRunning_ = false;
}

double Timer::elapsedMilliseconds()
{
    auto end = isRunning_ ? std::chrono::system_clock::now() : end_;
    return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start_).count();
}

double Timer::elapsedSeconds()
{
    auto end = isRunning_ ? std::chrono::system_clock::now() : end_;
    return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1,1>>>(end - start_).count();
}