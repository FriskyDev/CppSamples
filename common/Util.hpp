#pragma once

#include <chrono>
#include <cstdint>
#include <string>

#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif


template<typename ... Args>
std::wstring string_format(const std::wstring& format, Args ... args)
{
    // first run the string format just to get the size, and allocate space for the result
    size_t size = swprintf(nullptr, 0, format.c_str(), args ...) + 1;
    std::unique_ptr<wchar_t[]> buf(new wchar_t[size]);

    // now run it for real, and return the result
    swprintf(buf.get(), size, format.c_str(), args ...);
    return std::wstring(buf.get(), buf.get() + size - 1);
}

// Returns the error code as a formatted message.
std::tstring get_system_error_message(uint32_t err = 0);


class Timer
{
public:
    void start();
    void stop();
    double elapsedMilliseconds();
    double elapsedSeconds();

private:
    std::chrono::time_point<std::chrono::system_clock> start_;
    std::chrono::time_point<std::chrono::system_clock> end_;
    bool isRunning_ = false;
};