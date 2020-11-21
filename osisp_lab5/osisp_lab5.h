#pragma once

#include "resource.h"
#include <string>
#include <vector>
#include "PhonebookLargeLib/phonebook.h"

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
#define IDC_PREVBTN (HMENU)112
#define IDC_NEXTBTN (HMENU)113

const wchar_t* headers[] = {
        { L"telephone" },
        { L"last name" },
        { L"first name" },
        { L"middle name" },
        { L"street" },
        { L"house" },
        { L"flat" }
};

enum appState {
    view, search, none
};

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

HWND CreateListView(HWND parent, int columns);
void UpdateListView(HWND hListView, std::vector<PhonebookRecord*> book);