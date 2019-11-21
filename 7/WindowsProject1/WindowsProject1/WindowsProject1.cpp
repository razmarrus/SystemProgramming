// WindowsProject1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "WindowsProject1.h"


#define MAX_LOADSTRING 100
#define LANG_ENGLISH
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

HWND hEdit;
HWND hListBox1;
HWND hListBox2;

CHAR str_1[] = { "str 1" };
CHAR str_2[] = { "str 2" };
TCHAR buff[1024];


// Отправить объявления функций, включенных в этот модуль кода:
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

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Цикл основного сообщения:
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



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   HWND hButtonAdd = CreateWindowEx(NULL, L"BUTTON", L"ADD", WS_CHILD | WS_VISIBLE, 200, 300, 200, 50, hWnd, HMENU(56), hInst, NULL);
   HWND hButtonClear = CreateWindowEx(NULL, L"BUTTON", L"CLEAR", WS_CHILD | WS_VISIBLE, 400, 300, 200, 50, hWnd, HMENU(57), hInst, NULL);
   HWND hButtonRight = CreateWindowEx(NULL, L"BUTTON", L"TO_RIGHT", WS_CHILD | WS_VISIBLE, 600, 300, 200, 50, hWnd, HMENU(58), hInst, NULL);
   HWND hButtonDelate = CreateWindowEx(NULL, L"BUTTON", L"DELATE", WS_CHILD | WS_VISIBLE, 800, 300, 200, 50, hWnd, HMENU(59), hInst, NULL);



   LPCWSTR message = L"My First Window";
   hListBox1 = CreateWindowEx(NULL, L"listbox", L"box",
	   WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_NOTIFY,
	   300, 100, 200, 100,
	   hWnd, HMENU(60), hInst, NULL);

   hListBox2 = CreateWindowEx(NULL, L"listbox", L"box",
	   WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_NOTIFY,
	   700, 100, 200, 100,
	   hWnd, HMENU(60), hInst, NULL);
   hEdit = CreateWindowEx(NULL, L"EDIT", L"SMTH", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_NOTIFY, 350, 50, 500, 30, hWnd, HMENU(60), hInst, NULL);


   // Добавляем в список несколько строк
   /*char buffer[50] = "qwert wwe";

   SendMessage(hListBox1, LB_ADDSTRING, 0,(LPARAM)(LPSTR)"here is smth");
   SendMessage(hListBox1, LB_ADDSTRING, NULL, (LPARAM)(LPSTR)buffer);

   // Включаем режим перерисовки списка*/


   // Перерисовываем список
   InvalidateRect(hListBox1, NULL, TRUE);
   //LPCWSTR "MESS";


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//static HWND hListBox;

    switch (message)
    {
	case WM_CREATE:
	{
		// Создаем список

		/*LPCWSTR message = L"My First Window";
		hListBox = CreateWindow(message, NULL,
			WS_CHILD | WS_VISIBLE | LBS_STANDARD |
			LBS_WANTKEYBOARDINPUT,
			30, 30, 200, 100,
			hWnd, HMENU(60), hInst, NULL);

		// Отменяем режим перерисовки списка
		SendMessage(hListBox, WM_SETREDRAW, FALSE, 0L);

		// Добавляем в список несколько строк

		SendMessage(hListBox, LB_ADDSTRING, 0,
			(LPARAM)(LPSTR)"Зеленый");

		SendMessage(hListBox, LB_ADDSTRING, 0,
			(LPARAM)(LPSTR)"Красный");

		SendMessage(hListBox, LB_ADDSTRING, 0,
			(LPARAM)(LPSTR)"Розовый");

		SendMessage(hListBox, LB_ADDSTRING, 0,
			(LPARAM)(LPSTR)"Белый");

		// Включаем режим перерисовки списка
		SendMessage(hListBox, WM_SETREDRAW, TRUE, 0L);

		// Перерисовываем список
		InvalidateRect(hListBox, NULL, TRUE);*/


		return 0;
	}
    case WM_COMMAND:

        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
			case 56: //add
			{
				GetWindowText(hEdit, buff, 1024);
				SendMessage(hListBox1, LB_ADDSTRING, 0, (LPARAM)buff);
				SendMessage(hListBox1, LB_DELETESTRING, 0, (LPARAM)buff);
			}
			break;
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
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

// Обработчик сообщений для окна "О программе".
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
