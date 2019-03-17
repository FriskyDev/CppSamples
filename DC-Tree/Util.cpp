#include "Util.hpp"

#include <algorithm>

#include <strsafe.h>


std::wstring GetFormattedMessage(LPCWSTR message, ...)
{

    va_list args = nullptr;
    va_start(args, message);

    LPWSTR pBuffer = nullptr;
    FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER, message, 0, 0, (LPWSTR)&pBuffer, 0, &args);

    std::wstring result;
    LocalFree(pBuffer);

    va_end(args);

    return result;
}

std::wstring ErrorMessage(LPCWSTR message, DWORD error /*= 0*/)
{
    if (error == 0) {
        // retrieve the system error message for the last-error code
        error = GetLastError();
    }

    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, nullptr);

    // display the error message and exit the process
    LPVOID lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)message) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("error: %s (%d: %s)"), message, error, lpMsgBuf);

    std::wstring errorMessage = (LPCWSTR)lpDisplayBuf;
    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);

    errorMessage.erase(std::remove(errorMessage.begin(), errorMessage.end(), L'\r'), errorMessage.end());
    errorMessage.erase(std::remove(errorMessage.begin(), errorMessage.end(), L'\n'), errorMessage.end());
    errorMessage += L'\n';

    return errorMessage;
}