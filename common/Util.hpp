#pragma once

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
    size_t size = swprintf(nullptr, 0, format.c_str(), args ...) + 1;
    std::unique_ptr<wchar_t[]> buf(new wchar_t[size]);
    swprintf(buf.get(), size, format.c_str(), args ...);
    return std::wstring(buf.get(), buf.get() + size - 1);
}

// Returns the error code as a formatted message.
std::tstring GetFormatedMessage(uint32_t err = 0);
