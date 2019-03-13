#pragma once

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used items from Windows headers

#include <combaseapi.h>


struct ComponentObjectModel
{
    HRESULT status;
    HINSTANCE hinst;

    ComponentObjectModel();
    ~ComponentObjectModel();
};
