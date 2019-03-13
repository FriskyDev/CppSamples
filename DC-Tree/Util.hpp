#pragma once

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used items from Windows headers

#include <Windows.h>

#include <string>

template <typename T>
void SafeRelease(T **ppT)
{
    if (*ppT != nullptr) {
        (*ppT)->Release();
        *ppT = nullptr;
    }
}

template <typename T>
struct ComPtr
{
    T* ptr{};
    HRESULT status{};

    virtual ~ComPtr()
    {
        if (ptr != nullptr) {
            ptr->Release();
            ptr = nullptr;
        }
    }
};

std::wstring GetFormattedMessage(LPCWSTR pMessage, ...);
std::wstring ErrorMessage(LPCWSTR message, DWORD error = 0);
