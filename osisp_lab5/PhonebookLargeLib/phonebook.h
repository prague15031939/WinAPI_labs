#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

#ifdef PHONEBOOKCORE_EXPORTS
#define PHONEBOOKCORE_API __declspec(dllexport)
#else
#define PHONEBOOKCORE_API __declspec(dllimport)
#endif

struct PhonebookRecord
{
	wchar_t telephone[20];
	wchar_t lastName[20];
	wchar_t firstName[20];
	wchar_t middleName[20];
	wchar_t street[20];
	wchar_t house[10];
	wchar_t flat[10];
};

PHONEBOOKCORE_API std::vector<PhonebookRecord*> __cdecl GetPhonebook();

PHONEBOOKCORE_API std::vector<PhonebookRecord*> __cdecl Search(PhonebookRecord searchParam);

PHONEBOOKCORE_API std::vector<PhonebookRecord*> __cdecl GetNext();

PHONEBOOKCORE_API std::vector<PhonebookRecord*> __cdecl SearchNext(PhonebookRecord searchParam);

PHONEBOOKCORE_API void _cdecl ReturnToPrevious();
