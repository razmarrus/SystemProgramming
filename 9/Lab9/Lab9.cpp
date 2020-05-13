#include "framework.h"
#include "Lab9.h"
#define MAX_LOADSTRING 100
#define MOV_STEP 10
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HANDLE bmp[4];
int currBmp = 0;
int currentX = 0, currentY = 0, targX = 0, targY = 0;
bool reverse = true;
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
    LoadStringW(hInstance, IDC_LAB9, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB9));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_LAB9));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB9);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
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
    bmp[0] = LoadImage(NULL, L"pic1.bmp", IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE);
    bmp[1] = LoadImage(NULL, L"pic2.bmp", IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE);
    bmp[2] = LoadImage(NULL, L"pic3.bmp", IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE);
    bmp[3] = LoadImage(NULL, L"pic4.bmp", IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
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
    case WM_LBUTTONDOWN:
    {
        targX = LOWORD(lParam) - 30;
        targY = HIWORD(lParam) - 30;
        SetTimer(hWnd, NULL, 100, NULL);
    }
    break;
    case WM_TIMER:
    {
        bool needRedraw = false;
        if (abs(currentX - targX) >= MOV_STEP) {
            needRedraw = true;
            if (targX > currentX) {
                currentX += MOV_STEP;
                reverse = false;
            }
            else {
                currentX -= MOV_STEP;
                reverse = true;
            }
        }
        if (abs(currentY - targY) >= MOV_STEP) {
            needRedraw = true;
            if (targY > currentY)
                currentY += MOV_STEP;
            else
                currentY -= MOV_STEP;
        }
        if (needRedraw) {
            currBmp = (currBmp + 1) % 4;
            InvalidateRect(hWnd, NULL, true);
            SetTimer(hWnd, NULL, 100, NULL);
        }
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HDC hCompatibleDC;
        hCompatibleDC = CreateCompatibleDC(hdc);
        SelectObject(hCompatibleDC, bmp[currBmp]);
        if (reverse)
            StretchBlt(hdc, currentX, currentY, 100, 100, hCompatibleDC, 100, 0, -100, 100, SRCCOPY);
        else
            StretchBlt(hdc, currentX, currentY, 100, 100, hCompatibleDC, 0, 0, 100, 100, SRCCOPY);
        DeleteDC(hCompatibleDC);
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
