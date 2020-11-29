#include <iostream>
#include <windows.h>

#define REG_PATH L"Software\\TestApplication"

int main()
{
    HANDLE hFile = CreateFile(L"D:\\sho.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    
    std::string data = "sometimes i feel like esmeralda";
    WriteFile(hFile, data.c_str(), data.size(), NULL, NULL);
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

    char buf[40];
    DWORD bufSize = sizeof(buf);
    ZeroMemory(&buf, sizeof(buf));
    ReadFile(hFile, buf, 31, NULL, NULL);

    CloseHandle(hFile);

    DeleteFile(L"D:\\sho.txt");

    HKEY result;
    RegOpenKey(HKEY_CURRENT_USER, L"Software\\Python", &result);
    RegCloseKey(result);

    HKEY createResult;
    RegCreateKey(HKEY_CURRENT_USER, REG_PATH, &createResult);
    LPCWSTR lpData = L"test-data";
    RegSetValue(HKEY_CURRENT_USER, REG_PATH, REG_SZ, lpData, sizeof(wchar_t) * (wcslen(lpData) + 1));

    ZeroMemory(&buf, sizeof(buf));
    RegGetValue(HKEY_CURRENT_USER, REG_PATH, NULL, RRF_RT_ANY, NULL, buf, &bufSize);
    RegCloseKey(createResult);
    RegDeleteKey(HKEY_CURRENT_USER, REG_PATH);

    std::cout << "test application marker\n";
}