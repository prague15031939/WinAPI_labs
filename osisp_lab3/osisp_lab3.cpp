#include <iostream>
#include "CustomCriticalSection.h"

CUSTOM_CIRTICAL_SECTION* criticalSection = new CUSTOM_CIRTICAL_SECTION();
CRITICAL_SECTION cs;

DWORD WINAPI ThreadProc1(LPVOID lpParam) {
    //EnterCustomCriticalSection(criticalSection);
    EnterCriticalSection(&cs);
    for (int i = 0; i < 10000; i++)
        std::cout << 1;
    std::cout << std::endl;
    LeaveCriticalSection(&cs);
    //LeaveCustomCriticalSection(criticalSection);
    return 0;
}

DWORD WINAPI ThreadProc2(LPVOID lpParam) {
    //EnterCustomCriticalSection(criticalSection);
    EnterCriticalSection(&cs);
    for (int i = 0; i < 10000; i++)
        std::cout << 2;
    std::cout << std::endl;
    LeaveCriticalSection(&cs);
    //LeaveCustomCriticalSection(criticalSection);
    return 0;
}

int main()
{
    //InitializeCustomCriticalSectionAndSpinCount(criticalSection, 1000);
    InitializeCriticalSection(&cs);

    CreateThread(NULL, 0, ThreadProc2, NULL, 0, NULL);
    CreateThread(NULL, 0, ThreadProc1, NULL, 0, NULL);

    char ch;
    std::cin >> ch;
}

