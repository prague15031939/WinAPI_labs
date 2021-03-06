﻿#include <iostream>
#include <windows.h>

STARTUPINFO processStartupInfo;
PROCESS_INFORMATION processInfo;

int main()
{
    std::wstring processPath = L"D:\\Works C++\\5 сем\\WinAPI_labs\\osisp_lab6\\Debug\\TestApplication.exe";
    std::wstring dllPath = L"D:\\Works C++\\5 сем\\WinAPI_labs\\osisp_lab6\\Debug\\InjectionLib.dll";

    CreateProcess(processPath.c_str(), NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &processStartupInfo, &processInfo);

    void* loadLibraryW = GetProcAddress(GetModuleHandle(L"Kernel32"), "LoadLibraryW");
    LPVOID lpvMemory = VirtualAllocEx(processInfo.hProcess, NULL, dllPath.size() * sizeof(wchar_t) + 1, MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(processInfo.hProcess, lpvMemory, dllPath.c_str(), dllPath.size() * sizeof(wchar_t) + 1, NULL);

    HANDLE hRemoteThread = CreateRemoteThread(processInfo.hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)loadLibraryW, lpvMemory, NULL, NULL);
    WaitForSingleObject(hRemoteThread, INFINITE);
    CloseHandle(hRemoteThread);

    ResumeThread(processInfo.hThread);
    WaitForSingleObject(processInfo.hProcess, INFINITE);

    VirtualFreeEx(processInfo.hProcess, lpvMemory, 0, MEM_RELEASE);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    std::cout << "main marker\n";
}
