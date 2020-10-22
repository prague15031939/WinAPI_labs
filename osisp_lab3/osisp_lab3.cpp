#include <iostream>
#include <windows.h>

using namespace std;

CRITICAL_SECTION criticalSection;

int main()
{
    InitializeCriticalSectionAndSpinCount(&criticalSection, 100000);
    
    HANDLE fileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CRITICAL_SECTION), TEXT("SHARED_MEM"));
    LPCSTR buffer = (LPCSTR)MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CRITICAL_SECTION));
    CopyMemory((PVOID)buffer, &criticalSection, sizeof(criticalSection));

    STARTUPINFO info1;
    STARTUPINFO info2;
    PROCESS_INFORMATION process1;
    PROCESS_INFORMATION process2;

    ZeroMemory(&info1, sizeof(info1));
    info1.cb = sizeof(info1);
    ZeroMemory(&process1, sizeof(process1));

    ZeroMemory(&info2, sizeof(info2));
    info2.cb = sizeof(info2);
    ZeroMemory(&process2, sizeof(process2));

    wstring filePath1(L"D:\\Works C++\\5 сем\\WinAPI_labs\\osisp_lab3\\Debug\\TestProcess1.exe");
    wstring filePath2(L"D:\\Works C++\\5 сем\\WinAPI_labs\\osisp_lab3\\Debug\\TestProcess2.exe");

    CreateProcess(NULL, &filePath1[0], NULL, NULL, FALSE, 0, NULL, NULL, &info1, &process1);
    CreateProcess(NULL, &filePath2[0], NULL, NULL, FALSE, 0, NULL, NULL, &info2, &process2);

    WaitForSingleObject(process1.hProcess, INFINITE);
    WaitForSingleObject(process2.hProcess, INFINITE);

    CloseHandle(process1.hThread);
    CloseHandle(process1.hProcess);
    CloseHandle(process2.hThread);
    CloseHandle(process2.hProcess);

    char ch;
    std::cin >> ch;

    UnmapViewOfFile(buffer);
    CloseHandle(fileMapping);
    DeleteCriticalSection(&criticalSection);
}

