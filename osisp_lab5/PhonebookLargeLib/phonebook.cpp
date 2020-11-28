#include "pch.h"
#include "phonebook.h"

#include <stack>

TCHAR currentDirectory[1024];

HANDLE hSrcFile;
DWORD dwFileSize;
HANDLE hMapObject;
LPVOID lpvFilePointer;
DWORD offset;

std::vector<DWORD> cachedOffsets;

SYSTEM_INFO sysinfo;

LPVOID SetFilePointer(HANDLE hMap, LPVOID prevFilePointer, DWORD offset);
std::wstring ReadLine(DWORD* offset);
wchar_t ReadSymbol(DWORD* offset);
PhonebookRecord* ParseLine(std::wstring line);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        GetSystemInfo(&sysinfo);
        GetCurrentDirectory(sizeof(currentDirectory), currentDirectory);

        hSrcFile = CreateFile(((std::wstring)currentDirectory + L"\\phonebook.db").c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
        dwFileSize = GetFileSize(hSrcFile, NULL);
        hMapObject = CreateFileMapping(hSrcFile, NULL, PAGE_READONLY, 0, 0, L"PhonebookLarge");
        lpvFilePointer = SetFilePointer(hMapObject, NULL, 0);
        offset = 0;
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void _cdecl ReturnToPrevious() {
    if (cachedOffsets.size() == 0)
        offset = 0;
    else if (cachedOffsets.size() == 1) {
        cachedOffsets.pop_back();
        offset = 0;
    }
    else {
        cachedOffsets.pop_back();
        offset = cachedOffsets.back();
        cachedOffsets.pop_back();
    }
    lpvFilePointer = SetFilePointer(hMapObject, lpvFilePointer, offset);
}

std::vector<PhonebookRecord*> __cdecl GetPhonebook()
{
    offset = 0;
    cachedOffsets.clear();
    return GetNext();
}

std::vector<PhonebookRecord*> __cdecl GetNext() 
{
    std::vector<PhonebookRecord*> result;
    std::wstring line;

    if (offset < dwFileSize)
        cachedOffsets.push_back(offset);

    for (int i = 0; i < 20; i++) {
        if (offset < dwFileSize) {
            line.clear();
            line = ReadLine(&offset);
            PhonebookRecord* temp = ParseLine(line);
            result.push_back(temp);
        }
        else
            break;
    }

    return result;
}

std::vector<PhonebookRecord*> __cdecl Search(PhonebookRecord searchParam) 
{
    offset = 0;
    cachedOffsets.clear();
    return SearchNext(searchParam);
}

std::vector<PhonebookRecord*> __cdecl SearchNext(PhonebookRecord searchParam)
{
    std::vector<PhonebookRecord*> result;
    std::wstring line;

    if (offset < dwFileSize)
        cachedOffsets.push_back(offset);

    while (offset < dwFileSize) {
        line.clear();
        line = ReadLine(&offset);
        PhonebookRecord* temp = ParseLine(line);

        if (!((std::wstring)searchParam.telephone).empty() && (std::wstring)temp->telephone != (std::wstring)searchParam.telephone ||
            !((std::wstring)searchParam.firstName).empty() && (std::wstring)temp->firstName != (std::wstring)searchParam.firstName ||
            !((std::wstring)searchParam.lastName).empty() && (std::wstring)temp->lastName != (std::wstring)searchParam.lastName ||
            !((std::wstring)searchParam.middleName).empty() && (std::wstring)temp->middleName != (std::wstring)searchParam.middleName ||
            !((std::wstring)searchParam.street).empty() && (std::wstring)temp->street != (std::wstring)searchParam.street ||
            !((std::wstring)searchParam.house).empty() && (std::wstring)temp->house != (std::wstring)searchParam.house ||
            !((std::wstring)searchParam.flat).empty() && (std::wstring)temp->flat != (std::wstring)searchParam.flat) continue;

        result.push_back(temp);
        if (result.size() == 20) break;
    }

    return result;
}

LPVOID SetFilePointer(HANDLE hMap, LPVOID prevFilePointer, DWORD offset)
{
    LPVOID result;
    if (prevFilePointer != NULL)
        UnmapViewOfFile(prevFilePointer);

    result = MapViewOfFile(hMap, FILE_MAP_READ, 0, offset, sysinfo.dwAllocationGranularity);
    if (result == NULL) 
        result = MapViewOfFile(hMap, FILE_MAP_READ, 0, offset, 0);
    return result;
}

BYTE ReadByte(DWORD offset) {
    if (offset % 65536 == 0)
        lpvFilePointer = SetFilePointer(hMapObject, lpvFilePointer, offset);
    return *(BYTE*)((DWORD)lpvFilePointer + (offset % 65536));
}

wchar_t ReadSymbol(DWORD* offset)
{
    short first = ReadByte(*offset);

    *offset += 1;
    if ((first >> 6) == 3) {
        short second = ReadByte(*offset);
        *offset += 1;
        if ((second >> 6) == 2) {
            return ((first & 0x003F) << 6) + (second & 0x003F);
        }
    }
    return first;
}

std::wstring ReadLine(DWORD* offset)
{
    wchar_t symbol;
    std::wstring result = L"";

    do {
        symbol = ReadSymbol(offset);
        result += symbol;
    } while (symbol != L'\n');

    return result;
}

PhonebookRecord* ParseLine(std::wstring line)
{
    PhonebookRecord* item = new PhonebookRecord();
    ZeroMemory(item, sizeof(PhonebookRecord));
    std::vector<std::wstring> tokens;

    int pos = 0;
    std::wstring token;
    while ((pos = line.find(L";")) != std::string::npos) {
        token = line.substr(0, pos);
        if (token != L"")
            tokens.push_back(token);
        else
            tokens.push_back(NULL);
        line.erase(0, pos + 1);
    }

    std::copy(std::begin(tokens[0]), std::end(tokens[0]), std::begin(item->telephone));
    std::copy(std::begin(tokens[1]), std::end(tokens[1]), std::begin(item->lastName));
    std::copy(std::begin(tokens[2]), std::end(tokens[2]), std::begin(item->firstName));
    std::copy(std::begin(tokens[3]), std::end(tokens[3]), std::begin(item->middleName));
    std::copy(std::begin(tokens[4]), std::end(tokens[4]), std::begin(item->street));
    std::copy(std::begin(tokens[5]), std::end(tokens[5]), std::begin(item->house));
    std::copy(std::begin(tokens[6]), std::end(tokens[6]), std::begin(item->flat));

    return item;
}

