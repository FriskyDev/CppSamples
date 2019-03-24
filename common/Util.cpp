#include "Util.hpp"

#define WINVER 0x0602
#define _WIN32_WINNT 0x0602
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <Windows.h>


// Returns the error code as a formatted message.
std::tstring GetFormatedMessage(uint32_t err /*= 0*/)
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
