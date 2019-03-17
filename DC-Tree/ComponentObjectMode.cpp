#include "ComponentObjectModel.hpp"


ComponentObjectModel::ComponentObjectModel()
{
    // ignore return and run the program even in the unlikely event that HeapSetInformation fails
    HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);

    status = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}

ComponentObjectModel::~ComponentObjectModel()
{
    CoUninitialize();
}
