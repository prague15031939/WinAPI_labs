#include <iostream>
#include <windows.h>

int main()
{
    HANDLE hFile = CreateFile(L"D:\\sho.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    
    std::string data = "sometimes i feel like esmeralda";
    WriteFile(hFile, data.c_str(), data.size(), NULL, NULL);
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

    char buf[40];
    ZeroMemory(&buf, sizeof(buf));
    ReadFile(hFile, buf, 31, NULL, NULL);

    CloseHandle(hFile);

    DeleteFile(L"D:\\sho.txt");

    std::cout << "test application marker\n";
}