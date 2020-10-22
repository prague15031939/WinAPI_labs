#include <iostream>
#include <windows.h>

using namespace std;

LPCRITICAL_SECTION criticalSection;

int main() {

    HANDLE fileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, TEXT("SHARED_MEM"));

    criticalSection = (LPCRITICAL_SECTION)MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CRITICAL_SECTION));

    while (true) {
        if (TryEnterCriticalSection(criticalSection)) {
            std::cout << "TestProcess2, id = " << GetProcessId(GetCurrentProcess()) << std::endl;
            for (int i = 0; i < 10000; i++)
                std::cout << 2;
            std::cout << std::endl;
            LeaveCriticalSection(criticalSection);
            break;
        }
    }

    UnmapViewOfFile(criticalSection);
    CloseHandle(fileMapping);
    DeleteCriticalSection(criticalSection);

    return 0;
}
