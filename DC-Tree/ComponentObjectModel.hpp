#pragma once

// exclude rarely-used items from Windows headers
#define WIN32_LEAN_AND_MEAN

#include <combaseapi.h>


struct ComponentObjectModel
{
    HRESULT status;
    HINSTANCE hinst;

    ComponentObjectModel();
    ~ComponentObjectModel();
};
