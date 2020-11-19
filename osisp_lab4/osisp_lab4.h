#pragma once

#include "resource.h"

#define IDC_LISTVIEW (HMENU)101
#define IDC_SEARCHBTN (HMENU)102
#define IDC_LNEDIT (HMENU)103
#define IDC_TELEDIT (HMENU)104
#define IDC_STREDIT (HMENU)106

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
void UpdateListView(HWND hListView);
