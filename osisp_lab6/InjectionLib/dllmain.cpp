#include "pch.h"
#include <string>
#include <sstream>
#include "D:\Docs\Detours\include\detours.h"

HANDLE hStdOut;

VOID DisplayMessage(std::wstring message);
std::wstring GetKeyPath(HKEY key);

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

//-----------------------------------------------------------------------------------------------------------------------------------------------------

LSTATUS(WINAPI* PRegCreateKey) (HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult) = RegCreateKey;

LSTATUS WINAPI HookRegCreateKey(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
{
    DisplayMessage(L"Create key " + (std::wstring)lpSubKey + L"\n");
    return PRegCreateKey(hKey, lpSubKey, phkResult);
}

LSTATUS(WINAPI* PRegOpenKey) (HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult) = RegOpenKey;

LSTATUS WINAPI HookRegOpenKey(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
{
    DisplayMessage(L"Open key " + (std::wstring)lpSubKey + L"\n");
    return PRegOpenKey(hKey, lpSubKey, phkResult);
}

LSTATUS(WINAPI* PRegCloseKey) (HKEY hKey) = RegCloseKey;

LSTATUS WINAPI HookRegCloseKey(HKEY hKey)
{
    DisplayMessage(L"Close key " + GetKeyPath(hKey) + L"\n");
    return PRegCloseKey(hKey);
}

LSTATUS(WINAPI* PRegDeleteKey) (HKEY hKey, LPCWSTR lpSubKey) = RegDeleteKey;

LSTATUS WINAPI HookRegDeleteKey(HKEY hKey, LPCWSTR lpSubKey)
{
    DisplayMessage(L"Delete key " + (std::wstring)lpSubKey + L"\n");
    return PRegDeleteKey(hKey, lpSubKey);
}

LSTATUS(WINAPI* PRegGetValue) (HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData) = RegGetValue;

LSTATUS WINAPI HookRegGetValue(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValue, DWORD dwFlags, LPDWORD pdwType, PVOID pvData, LPDWORD pcbData)
{
    DisplayMessage(L"Get value from key " + (std::wstring)lpSubKey + L"\n");
    return PRegGetValue(hKey, lpSubKey, lpValue, dwFlags, pdwType, pvData, pcbData);
}

LSTATUS(WINAPI* PRegSetValue) (HKEY hKey, LPCWSTR lpSubKey, DWORD dwType, LPCWSTR lpData, DWORD cbData) = RegSetValue;

LSTATUS WINAPI HookRegSetValue(HKEY hKey, LPCWSTR lpSubKey, DWORD dwType, LPCWSTR lpData, DWORD cbData)
{
    DisplayMessage(L"Set value " + (std::wstring)lpData + L" to key " + (std::wstring)lpSubKey + L"\n");
    return PRegSetValue(hKey, lpSubKey, dwType, lpData, cbData);
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

        DetourAttach(&(PVOID&)PRegCreateKey, HookRegCreateKey);
        DetourAttach(&(PVOID&)PRegOpenKey, HookRegOpenKey);
        DetourAttach(&(PVOID&)PRegCloseKey, HookRegCloseKey);
        DetourAttach(&(PVOID&)PRegDeleteKey, HookRegDeleteKey);
        DetourAttach(&(PVOID&)PRegSetValue, HookRegSetValue);
        DetourAttach(&(PVOID&)PRegGetValue, HookRegGetValue);

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

        DetourDetach(&(PVOID&)PRegCreateKey, HookRegCreateKey);
        DetourDetach(&(PVOID&)PRegOpenKey, HookRegOpenKey);
        DetourDetach(&(PVOID&)PRegCloseKey, HookRegCloseKey);
        DetourDetach(&(PVOID&)PRegDeleteKey, HookRegDeleteKey);
        DetourDetach(&(PVOID&)PRegSetValue, HookRegSetValue);
        DetourDetach(&(PVOID&)PRegGetValue, HookRegGetValue);

        DetourTransactionCommit();

        CloseHandle(hStdOut);
        break;
    }
    return TRUE;
}

std::wstring GetKeyPath(HKEY key)
{
    std::wstring keyPath;
    if (key != NULL)
    {
        HMODULE dll = LoadLibrary(L"ntdll.dll");
        if (dll != NULL) {
            typedef DWORD(__stdcall* NtQueryKeyType) (HANDLE  KeyHandle, int KeyInformationClass, PVOID  KeyInformation, ULONG  Length, PULONG  ResultLength);

            NtQueryKeyType ntQueryKeyAddress = reinterpret_cast<NtQueryKeyType>(::GetProcAddress(dll, "NtQueryKey"));

            if (ntQueryKeyAddress != NULL) {
                DWORD size = 0;
                DWORD result = 0;
                result = ntQueryKeyAddress(key, 3, 0, 0, &size);
                if (result == ((LONG)0xC0000023L))
                {
                    size = size + 2;
                    wchar_t* buffer = new (std::nothrow) wchar_t[size / sizeof(wchar_t)]; 
                    if (buffer != NULL)
                    {
                        result = ntQueryKeyAddress(key, 3, buffer, size, &size);
                        if (result == ((LONG)0x00000000L))
                        {
                            buffer[size / sizeof(wchar_t)] = L'\0';
                            keyPath = std::wstring(buffer + 2);
                        }

                        delete[] buffer;
                    }
                }
            }

            FreeLibrary(dll);
        }
    }
    return keyPath;
}

VOID DisplayMessage(std::wstring message)
{
    WriteConsole(hStdOut, message.c_str(), message.size(), NULL, NULL);
}
