#pragma once

#include <vector>
#include <string>

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
	unsigned int house;
	unsigned int flat;
};

PHONEBOOKCORE_API std::vector<PhonebookRecord*> __cdecl GetPhonebook();

PHONEBOOKCORE_API std::vector<PhonebookRecord*> __cdecl Search(PhonebookRecord searchParam);
