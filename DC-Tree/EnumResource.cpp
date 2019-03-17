#include "EnumResource.hpp"

#include <cstdint>

#include <strsafe.h>


BOOL EnumTypesFunc(HMODULE hModule, LPTSTR lpType, LONG lParam)
{
    TCHAR szBuffer[80];
    size_t cbString;
    HRESULT hResult;

    // the resource type may be a string or an unsigned decimal integer
    if (!IS_INTRESOURCE(lpType)) {
        hResult = StringCchPrintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), TEXT("Type: %s\r\n"), lpType);
        if (FAILED(hResult)) {
            return FALSE;
        }
    } else {
        hResult = StringCchPrintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), TEXT("Type: %u\r\n"), LOWORD(lpType));
        if (FAILED(hResult)) {
            return FALSE;
        }
    }

    hResult = StringCchLength(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), &cbString);
    if (FAILED(hResult)) {
        return FALSE;
    }

    OutputDebugString(szBuffer);

    // find the names of all resources of type lpType
    EnumResourceNames(hModule, lpType, (ENUMRESNAMEPROC)EnumNamesFunc, 0);

    return TRUE;
}

// Resource name callback
BOOL EnumNamesFunc(HMODULE hModule, LPCTSTR lpType, LPTSTR lpName, LONG lParam)
{
    TCHAR szBuffer[80];
    size_t cbString;
    HRESULT hResult;

    // the name may be a string or an unsigned decimal integer
    if (!IS_INTRESOURCE(lpName)) {
        hResult = StringCchPrintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), TEXT("\tName: %s\r\n"), lpName);
        if (FAILED(hResult)) {
            return FALSE;
        }
    } else {
        hResult = StringCchPrintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), TEXT("\tName: %u\r\n"), LOWORD(lpName));
        if (FAILED(hResult)) {
            return FALSE;
        }
    }

    hResult = StringCchLength(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), &cbString);
    if (FAILED(hResult)) {
        return FALSE;
    }

    OutputDebugString(szBuffer);

    // find the languages of all resources of type lpType and name lpName
    EnumResourceLanguages(hModule, lpType, lpName, (ENUMRESLANGPROC)EnumLangsFunc, 0);

    return TRUE;
}

// Resource language callback
BOOL EnumLangsFunc(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, WORD wLang, LONG lParam)
{
    HRSRC hResInfo;
    TCHAR szBuffer[80];  // print buffer for info file
    size_t cbString;
    HRESULT hResult;

    hResInfo = FindResourceEx(hModule, lpType, lpName, wLang);
    // write the resource language to the resource information file
    hResult = StringCchPrintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), TEXT("\t\tLanguage: %u\r\n"), (USHORT)wLang);
    if (FAILED(hResult)) {
        return FALSE;
    }

    hResult = StringCchLength(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), &cbString);
    if (FAILED(hResult)) {
        return FALSE;
    }

    // write the resource handle and size to buffer
    hResult = StringCchPrintf(szBuffer,
        sizeof(szBuffer) / sizeof(TCHAR),
        TEXT("\t\thResInfo == %lx,  Size == %lu\r\n\r\n"),
        hResInfo,
        SizeofResource(hModule, hResInfo));
    if (FAILED(hResult)) {
        return FALSE;
    }

    hResult = StringCchLength(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), &cbString);
    if (FAILED(hResult)) {
        return FALSE;
    }

    OutputDebugString(szBuffer);

    return TRUE;
}
