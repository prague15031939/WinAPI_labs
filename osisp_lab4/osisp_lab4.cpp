#include "framework.h"
#include <commctrl.h>
#include "osisp_lab4.h"

#include <string>

#pragma comment(lib, "comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100

HWND hWnd;
HFONT hFont;
HWND hListView;
HWND hBtnSearch;
HWND hLnEdit, hFnEdit, hMnEdit, hTelEdit, hStrEdit, hHEdit, hFEdit;

std::vector<PhonebookRecord*> phonebook;

HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof icc;
    icc.dwICC = ICC_COOL_CLASSES | ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icc);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OSISPLAB4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OSISPLAB4));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OSISPLAB4));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OSISPLAB4);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        hListView = CreateListView(hWnd, 7);
        hBtnSearch = CreateWindow(WC_BUTTON, TEXT("Search"), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | BS_CENTER,
            925, 495, 200, 23, hWnd, IDC_SEARCHBTN, hInst, NULL);
        HWND hLabel1 = CreateWindow(L"static", L"last_name", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 250, 470, 80, 20, hWnd, (HMENU)(501), (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
        hLnEdit = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            250, 495, 200, 23, hWnd, IDC_LNEDIT, hInst, NULL);
        HWND hLabel2 = CreateWindow(L"static", L"telephone", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 475, 470, 80, 20, hWnd, (HMENU)(501), (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
        hTelEdit = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            475, 495, 200, 23, hWnd, IDC_TELEDIT, hInst, NULL);
        HWND hLabel3 = CreateWindow(L"static", L"street", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 700, 470, 50, 20, hWnd, (HMENU)(501), (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
        hStrEdit = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            700, 495, 200, 23, hWnd, IDC_STREDIT, hInst, NULL);
        hBtnSearch = CreateWindow(WC_BUTTON, TEXT("Refresh"), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | BS_CENTER,
            25, 495, 200, 23, hWnd, IDC_REFRESHBTN, hInst, NULL);

        HWND hLabel4 = CreateWindow(L"static", L"first_name", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 25, 540, 80, 20, hWnd, (HMENU)(501), (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
        hFnEdit = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            25, 565, 200, 23, hWnd, IDC_FNEDIT, hInst, NULL);
        HWND hLabel5 = CreateWindow(L"static", L"middle_name", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 250, 540, 100, 20, hWnd, (HMENU)(501), (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
        hMnEdit = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            250, 565, 200, 23, hWnd, IDC_MNEDIT, hInst, NULL);
        HWND hLabel6 = CreateWindow(L"static", L"house", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 475, 540, 50, 20, hWnd, (HMENU)(501), (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
        hHEdit = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            475, 565, 200, 23, hWnd, IDC_HEDIT, hInst, NULL);
        HWND hLabel7 = CreateWindow(L"static", L"flat", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 700, 540, 50, 20, hWnd, (HMENU)(501), (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
        hFEdit = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            700, 565, 200, 23, hWnd, IDC_FEDIT, hInst, NULL);

        break;
    }
    case WM_COMMAND:
    {
        switch (HIWORD(wParam)) {
        case BN_CLICKED:
            switch (LOWORD(wParam)) {
            case (int)IDC_REFRESHBTN:
            {
                UpdateListView(hListView, GetPhonebook());
                break;
            }
            case (int)IDC_SEARCHBTN:
                PhonebookRecord searchParam;
                ZeroMemory(&searchParam, sizeof(searchParam));
                std::wstring text = GetText(hFnEdit);
                std::copy(std::begin(text), std::end(text), std::begin(searchParam.firstName));

                text = GetText(hLnEdit);
                std::copy(std::begin(text), std::end(text), std::begin(searchParam.lastName));

                text = GetText(hMnEdit);
                std::copy(std::begin(text), std::end(text), std::begin(searchParam.middleName));

                text = GetText(hTelEdit);
                std::copy(std::begin(text), std::end(text), std::begin(searchParam.telephone));

                text = GetText(hStrEdit);
                std::copy(std::begin(text), std::end(text), std::begin(searchParam.street));

                text = GetText(hHEdit);
                std::copy(std::begin(text), std::end(text), std::begin(searchParam.house));

                text = GetText(hFEdit);
                std::copy(std::begin(text), std::end(text), std::begin(searchParam.flat));

                UpdateListView(hListView, Search(searchParam));
                break;
            }
            break;
        }

        switch (LOWORD(wParam))
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            hFont = CreateFont(12, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Times New Roman"));
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_NOTIFY:
    {
        NMLVDISPINFO* plvdi;

        switch (((LPNMHDR)lParam)->code)
        {
        case LVN_GETDISPINFO:

            plvdi = (NMLVDISPINFO*)lParam;

            switch (plvdi->item.iSubItem)
            {
            case 0:
                plvdi->item.pszText = phonebook[plvdi->item.iItem]->telephone;
                break;

            case 1:
                plvdi->item.pszText = phonebook[plvdi->item.iItem]->lastName;
                break;

            case 2:
                plvdi->item.pszText = phonebook[plvdi->item.iItem]->firstName;
                break;
            case 3:
                plvdi->item.pszText = phonebook[plvdi->item.iItem]->middleName;
                break;

            case 4:
                plvdi->item.pszText = phonebook[plvdi->item.iItem]->street;
                break;

            case 5:
                plvdi->item.pszText = phonebook[plvdi->item.iItem]->house;
                break;
            case 6:
                plvdi->item.pszText = phonebook[plvdi->item.iItem]->flat;
                break;


            default:
                break;
            }

            break;
        }
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

HWND CreateListView(HWND parent, int columns) {
    RECT rcClient;
    GetClientRect(parent, &rcClient);

    HWND hWndListView = CreateWindow(WC_LISTVIEW, L"", WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS | WS_EX_CLIENTEDGE, 0, rcClient.top,
        rcClient.right - rcClient.left, rcClient.bottom - rcClient.top - 80, parent, IDC_LISTVIEW, hInst, NULL);

    LVCOLUMN c = { 0 };
    c.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

    for (int i = 0; i < columns; i++) {
        c.fmt = LVCFMT_LEFT;
        c.pszText = (LPWSTR)headers[i];
        c.iSubItem = i;
        c.cx = 100;
        ListView_InsertColumn(hWndListView, i, &c);
    }
    return hWndListView;
}

void UpdateListView(HWND hListView, std::vector<PhonebookRecord*> book) {
    ListView_DeleteAllItems(hListView);
    LVITEM lvI;

    lvI.mask = LVIF_TEXT;
    lvI.pszText = LPSTR_TEXTCALLBACK;
    lvI.iSubItem = 0;

    phonebook = book;

    for (int i = 0; i < book.size(); i++) {
        lvI.iItem = i;
        ListView_InsertItem(hListView, &lvI);
    }
}

std::wstring GetText(HWND hEdit)
{
    WCHAR buffer[512];
    GetWindowText(hEdit, buffer, sizeof(buffer));
    return buffer;
}