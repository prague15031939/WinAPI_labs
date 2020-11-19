#pragma once

#include "resource.h"
#include "PhonebookLib/phonebook.h"

#define IDC_LISTVIEW (HMENU)101
#define IDC_SEARCHBTN (HMENU)102
#define IDC_LNEDIT (HMENU)103
#define IDC_TELEDIT (HMENU)104
#define IDC_STREDIT (HMENU)106
#define IDC_FNEDIT (HMENU)107
#define IDC_MNEDIT (HMENU)108
#define IDC_HEDIT (HMENU)109
#define IDC_FEDIT (HMENU)110
#define IDC_REFRESHBTN (HMENU)111

const wchar_t* headers[] = {
        { L"telephone" },
        { L"last name" },
        { L"first name" },
        { L"middle name" },
        { L"street" },
        { L"house" },
        { L"flat" }
};

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

HWND CreateListView(HWND parent, int columns);
void UpdateListView(HWND hListView, std::vector<PhonebookRecord*> book);
std::wstring GetText(HWND hEdit);