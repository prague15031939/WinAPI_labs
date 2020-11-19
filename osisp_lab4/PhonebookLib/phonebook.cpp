#include "pch.h"
#include "phonebook.h"
#include "index.h"

static std::vector<PhonebookRecord*> phoneBook;
TCHAR currentDirectory[1024];

Index<wchar_t[20]> *telephoneIndex, *lastNameIndex, *firstNameIndex, *middleNameIndex, *streetIndex;
Index<wchar_t[10]> *houseIndex, *flatIndex;

VOID CreateIndex(std::vector<PhonebookRecord*> phoneBook);
std::vector<PhonebookRecord*> Intersect(std::vector<PhonebookRecord*> first, std::vector<PhonebookRecord*> second);
std::vector<PhonebookRecord*> ReadFromFile(std::wstring filePath);
PhonebookRecord* ParseLine(std::wstring line);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        GetCurrentDirectory(sizeof(currentDirectory), currentDirectory);
        phoneBook = ReadFromFile((std::wstring)currentDirectory + L"\\phonebook.db");
        CreateIndex(phoneBook);
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

std::vector<PhonebookRecord*> __cdecl GetPhonebook()
{
    return phoneBook;
}

std::vector<PhonebookRecord*> __cdecl Search(PhonebookRecord searchParam)
{
    bool isEmpty = true;
    std::vector<PhonebookRecord*> result;

    if (!((std::wstring)searchParam.telephone).empty()) {
        result = Intersect(result, telephoneIndex->Search(telephoneIndex->root, searchParam.telephone));
        isEmpty = false;
    }
    if (!((std::wstring)searchParam.lastName).empty()) {
        result = Intersect(result, lastNameIndex->Search(lastNameIndex->root, searchParam.lastName));
        isEmpty = false;
    }
    if (!((std::wstring)searchParam.firstName).empty()) {
        result = Intersect(result, firstNameIndex->Search(firstNameIndex->root, searchParam.firstName));
        isEmpty = false;
    }
    if (!((std::wstring)searchParam.middleName).empty()) {
        result = Intersect(result, middleNameIndex->Search(middleNameIndex->root, searchParam.middleName));
        isEmpty = false;
    }
    if (!((std::wstring)searchParam.street).empty()) {
        result = Intersect(result, streetIndex->Search(streetIndex->root, searchParam.street));
        isEmpty = false;
    }
    if (!((std::wstring)searchParam.house).empty()) {
        result = Intersect(result, houseIndex->Search(houseIndex->root, searchParam.house));
        isEmpty = false;
    }
    if (!((std::wstring)searchParam.flat).empty()) {
        result = Intersect(result, flatIndex->Search(flatIndex->root, searchParam.flat));
        isEmpty = false;
    }

    if (!isEmpty)
        return result;
    else
        return phoneBook;
}

VOID CreateIndex(std::vector<PhonebookRecord*> phoneBook)
{
    telephoneIndex = new Index<wchar_t[20]>(phoneBook, offsetof(PhonebookRecord, telephone));
    lastNameIndex = new Index<wchar_t[20]>(phoneBook, offsetof(PhonebookRecord, lastName));
    firstNameIndex = new Index<wchar_t[20]>(phoneBook, offsetof(PhonebookRecord, firstName));
    middleNameIndex = new Index<wchar_t[20]>(phoneBook, offsetof(PhonebookRecord, middleName));
    streetIndex = new Index<wchar_t[20]>(phoneBook, offsetof(PhonebookRecord, street));
    houseIndex = new Index<wchar_t[10]>(phoneBook, offsetof(PhonebookRecord, house));
    flatIndex = new Index<wchar_t[10]>(phoneBook, offsetof(PhonebookRecord, flat));
}

std::vector<PhonebookRecord*> Intersect(std::vector<PhonebookRecord*> first, std::vector<PhonebookRecord*> second)
{
    std::vector<PhonebookRecord*> result;

    if (!first.empty()) {
        std::sort(first.begin(), first.end());
        std::sort(second.begin(), second.end());

        std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), back_inserter(result));
    }
    else
        result = second;

    return result;
}

std::vector<PhonebookRecord*> ReadFromFile(std::wstring filePath)
{
    std::vector<PhonebookRecord*> result;
    std::wstring line;
    std::wifstream file(filePath);
    if (file.is_open()) {
        while (getline(file, line)) {
            result.push_back(ParseLine(line));
        }
    }

    file.close();
    return result;
}

PhonebookRecord* ParseLine(std::wstring line)
{
    PhonebookRecord *item = new PhonebookRecord();
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

