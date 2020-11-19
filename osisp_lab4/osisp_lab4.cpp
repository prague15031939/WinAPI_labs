#include "framework.h"
#include <commctrl.h>
#include "osisp_lab4.h"

#pragma comment(lib, "comctl32.lib")

#define MAX_LOADSTRING 100

HWND hWnd;
HFONT hFont;
HWND hListView;
HWND hBtnSearch;
HWND hLnEdit;
HWND hTelEdit;
HWND hStrEdit;

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
            25, 495, 200, 23, hWnd, IDC_SEARCHBTN, hInst, NULL);
        hLnEdit = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            250, 495, 200, 23, hWnd, IDC_LNEDIT, hInst, NULL);
        hTelEdit = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            475, 495, 200, 23, hWnd, IDC_TELEDIT, hInst, NULL);
        hStrEdit = CreateWindow(WC_EDIT, TEXT(""), WS_CHILD | WS_OVERLAPPED | WS_EX_CLIENTEDGE | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            700, 495, 200, 23, hWnd, IDC_STREDIT, hInst, NULL);
        UpdateListView(hListView);
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            
            switch (wmId)
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
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

HWND CreateListView(HWND parent, int columns) {
    RECT rcClient;
    GetClientRect(parent, &rcClient);

    HWND hWndListView = CreateWindow(WC_LISTVIEW, L"", WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS | WS_EX_CLIENTEDGE, 0, rcClient.top,
        rcClient.right - rcClient.left, rcClient.bottom - rcClient.top - 50, parent, IDC_LISTVIEW, hInst, NULL);

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

void UpdateListView(HWND hListView) {
    ListView_DeleteAllItems(hListView);
    LVITEM item;
    ZeroMemory(&item, sizeof(LVITEM));
    item.mask = LVIF_TEXT | LVIF_STATE;
    for (int i = 0; i < 10; i++) {
        item.iItem = i;
        item.iSubItem = 0;
        item.pszText = LPWSTR(L"sho");
        ListView_InsertItem(hListView, &item);
        ListView_SetItemText(hListView, i, 1, LPWSTR(L"sho1"));
        ListView_SetItemText(hListView, i, 2, LPWSTR(L"sho2"));
        ListView_SetItemText(hListView, i, 3, LPWSTR(L"sho3"));
        ListView_SetItemText(hListView, i, 4, LPWSTR(L"sho4"));
        ListView_SetItemText(hListView, i, 5, LPWSTR(L"sho5"));
        ListView_SetItemText(hListView, i, 6, LPWSTR(L"sho6"));
    }
}