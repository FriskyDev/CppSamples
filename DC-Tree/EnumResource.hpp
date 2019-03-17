#pragma once

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used items from Windows headers

#include <Windows.h>


BOOL EnumTypesFunc(HMODULE hModule, LPTSTR lpType, LONG lParam);
BOOL EnumNamesFunc(HMODULE hModule, LPCTSTR lpType, LPTSTR lpName, LONG lParam);
BOOL EnumLangsFunc(HMODULE hModule, LPCTSTR lpType, LPCTSTR lpName, WORD wLang, LONG lParam);
