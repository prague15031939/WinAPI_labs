#include "pch.h"
#include <string>
#include <sstream>
#include "D:\Docs\Detours\include\detours.h"

HANDLE hStdOut;

VOID DisplayMessage(std::wstring message);

HANDLE(WINAPI* PCreateFile) (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = CreateFile;

HANDLE WINAPI HookCreateFile(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    DisplayMessage(L"Create file: " + (std::wstring)lpFileName + L"\n");
    return PCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

BOOL(WINAPI* PDeleteFile) (LPCWSTR lpFileName) = DeleteFile;

BOOL WINAPI HookDeleteFile(LPCWSTR lpFileName)
{
    DisplayMessage(L"Delete file: " + (std::wstring)lpFileName + L"\n");
    return PDeleteFile(lpFileName);
}

BOOL(WINAPI* PWriteFile) (HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) = WriteFile;

BOOL WINAPI HookWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
    std::wostringstream wss;
    wss << "Write to file "  << hFile << " " << nNumberOfBytesToWrite << " bytes\n";
    DisplayMessage(wss.str());
    return PWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

BOOL(WINAPI* PReadFile) (HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) = ReadFile;

BOOL WINAPI HookReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) {
    std::wostringstream wss;
    wss << "Read from file " << hFile << " " << nNumberOfBytesToRead << " bytes\n";
    DisplayMessage(wss.str());
    return PReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

        DisableThreadLibraryCalls(hModule);
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DetourAttach(&(PVOID&)PCreateFile, HookCreateFile);
        DetourAttach(&(PVOID&)PDeleteFile, HookDeleteFile);
        DetourAttach(&(PVOID&)PWriteFile, HookWriteFile);
        DetourAttach(&(PVOID&)PReadFile, HookReadFile);

        DetourTransactionCommit();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DetourDetach(&(PVOID&)PCreateFile, HookCreateFile);
        DetourDetach(&(PVOID&)PDeleteFile, HookDeleteFile);
        DetourDetach(&(PVOID&)PWriteFile, HookWriteFile);
        DetourDetach(&(PVOID&)PReadFile, HookReadFile);

        DetourTransactionCommit();

        CloseHandle(hStdOut);
        break;
    }
    return TRUE;
}

VOID DisplayMessage(std::wstring message)
{
    WriteConsole(hStdOut, message.c_str(), message.size(), NULL, NULL);
}
