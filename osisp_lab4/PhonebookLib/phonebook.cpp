#include "pch.h"
#include "phonebook.h"
#include "index.h"

#include <algorithm>

static CRITICAL_SECTION criticalSection;
static std::vector<PhonebookRecord*> phoneBook;

Index<wchar_t[20]> *telephoneIndex, *lastNameIndex, *firstNameIndex, *middleNameIndex, *streetIndex;
Index<unsigned int>* houseIndex, * flatIndex;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

PHONEBOOKCORE_API std::vector<PhonebookRecord*> __cdecl GetPhonebook()
{
    return phoneBook;
}

PHONEBOOKCORE_API std::vector<PhonebookRecord*> __cdecl Search(PhonebookRecord searchParam)
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
    if (searchParam.house != 0) {
        result = Intersect(result, houseIndex->Search(houseIndex->root, searchParam.house));
        isEmpty = false;
    }
    if (searchParam.flat != 0) {
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
    streetIndex = new Index<wchar_t[20]>(phoneBook, offsetof(PhonebookRecord, street));
    houseIndex = new Index<unsigned int>(phoneBook, offsetof(PhonebookRecord, house));
    flatIndex = new Index<unsigned int>(phoneBook, offsetof(PhonebookRecord, flat));
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

