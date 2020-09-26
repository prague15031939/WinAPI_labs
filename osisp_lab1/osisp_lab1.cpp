#include "framework.h"
#include "osisp_lab1.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];           

std::vector<Figure*> FigureVector;
Figure* CurrentFigure;
FigureType CurrentFigureType;
BOOL StartedDrawing;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OSISPLAB1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OSISPLAB1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OSISPLAB1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OSISPLAB1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
        CurrentFigure = new Figure();
        CurrentFigureType = ftRectangle;
        StartedDrawing = false;
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_KEYDOWN:
    {
        SwitchFigureType(hWnd, message, wParam, lParam);
    }
    break;
    case WM_LBUTTONDOWN:
    {
        onClickDown(hWnd, message, wParam, lParam);
        InvalidateRect(hWnd, NULL, true);
    }
    break;
    case WM_LBUTTONUP:
    {
        onClickUp(hWnd, message, wParam, lParam);
        InvalidateRect(hWnd, NULL, true);
    }
    break;
    case WM_MOUSEMOVE:
    {
        onMouseMove(hWnd, message, wParam, lParam);
        InvalidateRect(hWnd, NULL, true);
    }
    break;
    case WM_ERASEBKGND:
    {
        InvalidateRect(hWnd, NULL, FALSE);
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HDC memDC = CreateCompatibleDC(hdc);
        RECT lpRect;
        GetClientRect(hWnd, &lpRect);
        HBITMAP hBM = CreateCompatibleBitmap(hdc, lpRect.right, lpRect.bottom);
        HANDLE hOld = SelectObject(memDC, hBM);
        RECT r;
        SetRect(&r, 0, 0, lpRect.right, lpRect.bottom);
        FillRect(memDC, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));

        for (int i = 0; i < FigureVector.size(); i++) {
            onPaint(memDC, FigureVector[i]);
        }
        if (StartedDrawing) {
            onPaint(memDC, CurrentFigure);
        }

        BitBlt(hdc, 0, 0, lpRect.right, lpRect.bottom, memDC, 0, 0, SRCCOPY);
        SelectObject(memDC, hOld);
        DeleteObject(hBM);
        DeleteDC(memDC);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY: 
    {
        PostQuitMessage(0);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void CreateInstance() {
    switch (CurrentFigureType) {
    case ftRectangle: CurrentFigure = new RectangleFigure(); break;
    case ftEllipse: CurrentFigure = new EllipseFigure(); break;
    }
    CurrentFigure->type = CurrentFigureType;
}

void SwitchFigureType(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
    if (!StartedDrawing) {
        switch (wParam) {
        case 0x45:
            CurrentFigureType = ftEllipse;
            break;
        case 0x52:
            CurrentFigureType = ftRectangle;
            break;
        }
    }
}

void onClickDown(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);

    CreateInstance();
    switch (CurrentFigure->type) {
    case ftRectangle: dynamic_cast<RectangleFigure*>(CurrentFigure)->ClickDown(xPos, yPos, xPos, yPos); break;
    case ftEllipse: dynamic_cast<EllipseFigure*>(CurrentFigure)->ClickDown(xPos, yPos, xPos, yPos); break;
    }

    StartedDrawing = true;
}

void onMouseMove(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (wParam == MK_LBUTTON) {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        switch (CurrentFigure->type) {
        case ftRectangle: dynamic_cast<RectangleFigure*>(CurrentFigure)->MouseMove(xPos, yPos); break;
        case ftEllipse: dynamic_cast<EllipseFigure*>(CurrentFigure)->MouseMove(xPos, yPos); break;
        }
    }
}

void onClickUp(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);

    switch (CurrentFigure->type) {
    case ftRectangle: dynamic_cast<RectangleFigure*>(CurrentFigure)->ClickUp(xPos, yPos, FigureVector); break;
    case ftEllipse: dynamic_cast<EllipseFigure*>(CurrentFigure)->ClickUp(xPos, yPos, FigureVector); break;
    }

    StartedDrawing = false;
}

void onPaint(HDC hdc, Figure* figure) {
    switch (figure->type) {
    case ftRectangle: dynamic_cast<RectangleFigure*>(figure)->Paint(hdc); break;
    case ftEllipse: dynamic_cast<EllipseFigure*>(figure)->Paint(hdc); break;
    }
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
