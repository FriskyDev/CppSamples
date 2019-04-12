#include <cstdint>
#include <iostream>
#include <memory>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "../common/Util.hpp"

int main()
{
    using namespace std;

    ULONG length;
    HANDLE curProc = GetCurrentProcess();
    BOOL r = GetSystemCpuSetInformation(nullptr, 0, &length, curProc, 0);

    wcout << L"length=" << length << endl;
    wcout << L"struct=" << sizeof(SYSTEM_CPU_SET_INFORMATION) << endl;
    int num_structs = length / sizeof(SYSTEM_CPU_SET_INFORMATION);
    wcout << L"num_structs=" << num_structs << endl;

    auto cpu_set = make_unique<SYSTEM_CPU_SET_INFORMATION[]>(num_structs);
    r = GetSystemCpuSetInformation(reinterpret_cast<PSYSTEM_CPU_SET_INFORMATION>(cpu_set.get()), length, &length, curProc, 0);
    if (!r) {
        auto msg = get_system_error_message();
        msg = string_format(L"Error: GetSystemCpuSetInformation: %s", msg.c_str());
        wcerr << msg.c_str() << endl;
        exit(1);
    }

    for (int i = 0; i < num_structs; i++) {
        wcout << L"Set: " << i << endl;
        wcout << L"  Size: " << cpu_set[i].Size << endl;
        wcout << L"  Type: " << cpu_set[i].Type << endl;
        if (cpu_set[i].Type != CpuSetInformation) {
            wcout << "Unknown type: skipping..." << endl;
            continue;
        }

        wcout << L"  Id: " << cpu_set[i].CpuSet.Id << endl;
        wcout << L"  LogicalProcessorIndex: " << cpu_set[i].CpuSet.LogicalProcessorIndex << endl;
        wcout << L"  Group: " << cpu_set[i].CpuSet.Group << endl;
        wcout << L"  Parked: " << cpu_set[i].CpuSet.Parked << endl;
        wcout << L"  Allocated: " << cpu_set[i].CpuSet.Allocated << endl;
        wcout << L"  AllocatedToTargetProcess: " << cpu_set[i].CpuSet.AllocatedToTargetProcess << endl;
        wcout << L"  RealTime: " << cpu_set[i].CpuSet.RealTime << endl;
        wcout << L"  SchedulingClass: " << cpu_set[i].CpuSet.SchedulingClass << endl;
        wcout << L"  AllocationTag: " << cpu_set[i].CpuSet.AllocationTag << endl;
    }
}
