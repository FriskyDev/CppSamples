#pragma once

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used items from Windows headers

#include <Windows.h>


BOOL EnumTypesFunc(
    HMODULE hModule,  // module handle
    LPTSTR lpType,    // address of resource type
    LONG lParam);     // extra parameter, could be used for error checking

BOOL EnumNamesFunc(
    HMODULE hModule,  // module handle
    LPCTSTR lpType,   // address of resource type
    LPTSTR lpName,    // address of resource name
    LONG lParam);     // extra parameter, could be used for error checking

BOOL EnumLangsFunc(
    HMODULE hModule, // module handle
    LPCTSTR lpType,  // address of resource type
    LPCTSTR lpName,  // address of resource name
    WORD wLang,      // resource language
    LONG lParam);    // extra parameter, could be used for error checking
