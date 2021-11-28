// Processes.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Processes.h"

#define MAX_LOADSTRING                  100
#define CMD_BUTTON_NOTEPAD              1001
#define CMD_BUTTON_WEBSITE              1002
#define CMD_BUTTON_CALC                 1003
#define CMD_BUTTON_STOP_NOTEPAD         1004
#define CMD_BUTTON_STOP_WEBSITE         1005
#define CMD_BUTTON_STOP_CALC            1006

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND butNotepad;
HWND butWebsite;
HWND butCalc;
HWND stopNotepad;
HWND stopWebsite;
HWND stopCalc;
HWND list;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI ThreadProcNotepad(LPVOID params);
DWORD WINAPI ThreadProcWebsite(LPVOID params);
DWORD WINAPI ThreadProcCalc(LPVOID params);
DWORD WINAPI  ButtonNotepad();
DWORD WINAPI  ButtonWebsite();
DWORD WINAPI  ButtonCalc();
void StopThreadNotepad();
void StopThreadWebsite();
void StopThreadCalc();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROCESSES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROCESSES));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROCESSES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROCESSES);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

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

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        list = CreateWindowW(L"Listbox", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
            200, 30, 300, 400, hWnd, NULL, hInst, NULL);
        butNotepad = CreateWindowW(L"Button", L"Notepad", WS_VISIBLE | WS_CHILD | WS_BORDER,
            10, 30, 75, 23, hWnd, (HMENU)CMD_BUTTON_NOTEPAD, hInst, NULL);
        stopNotepad = CreateWindowW(L"Button", L"STOP", WS_VISIBLE | WS_CHILD | WS_BORDER,
            95, 30, 75, 23, hWnd, (HMENU)CMD_BUTTON_STOP_NOTEPAD, hInst, NULL);
        butCalc = CreateWindowW(L"Button", L"Calc", WS_VISIBLE | WS_CHILD | WS_BORDER,
            10, 60, 75, 23, hWnd, (HMENU)CMD_BUTTON_CALC, hInst, NULL);
        stopWebsite = CreateWindowW(L"Button", L"STOP", WS_VISIBLE | WS_CHILD | WS_BORDER,
            95, 60, 75, 23, hWnd, (HMENU)CMD_BUTTON_STOP_CALC, hInst, NULL);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case CMD_BUTTON_NOTEPAD:
                ButtonNotepad();
                break;
            case CMD_BUTTON_WEBSITE:
                break;
            case CMD_BUTTON_CALC:
                ButtonCalc();
                break;
            case CMD_BUTTON_STOP_NOTEPAD:
                StopThreadNotepad();
                break;
            case CMD_BUTTON_STOP_CALC:
                StopThreadCalc();
                break;
            case CMD_BUTTON_STOP_WEBSITE:

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
            // TODO: Add any drawing code that uses hdc here...
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

BOOL processNotepad = FALSE;
BOOL processWebsite = FALSE;
BOOL processCalc = FALSE;

STARTUPINFO sinfoNotepad;
STARTUPINFO sinfoWebsite;
STARTUPINFO sinfoCalc;
PROCESS_INFORMATION pinfoNotepad;
PROCESS_INFORMATION pinfoWebsite;
PROCESS_INFORMATION pinfoCalc;

// Message handler for about box.
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

DWORD WINAPI ThreadProcNotepad(LPVOID params) {
    PROCESS_INFORMATION pinfoNotepad = (PROCESS_INFORMATION&)params;
    SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"Notepad works!");
    WaitForSingleObject(pinfoNotepad.hProcess, INFINITE);

    return 0;
}

DWORD WINAPI  ButtonNotepad() {
    ZeroMemory(&sinfoNotepad, sizeof(STARTUPINFO));
    ZeroMemory(&pinfoNotepad, sizeof(PROCESS_INFORMATION));

    if (processNotepad == FALSE)
    {
        if (CreateProcessW(L"C:\\Windows\\notepad.exe", NULL, NULL, NULL, TRUE, 0, NULL, NULL, &sinfoNotepad, &pinfoNotepad)) {

            processNotepad = TRUE;
            CreateThread(NULL, 0, ThreadProcNotepad, &pinfoNotepad, 0, NULL);

        }
        else
        {
            SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"Creating Notepad process error!");
        }
    }
    else {
        SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"Creating Notepad process error! Already exist!");
    }
    return 0;
}

void StopThreadNotepad() {
    if (processNotepad)
    {
        TerminateProcess(pinfoNotepad.hProcess, 0);
        CloseHandle(pinfoNotepad.hThread);
        CloseHandle(pinfoNotepad.hProcess);
        SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"Notepad stopped!");
        processNotepad = FALSE;

    }
    else {
        SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"Notepad process doesn't exist!");
    }
}

DWORD WINAPI ThreadProcCalc(LPVOID params) {
    PROCESS_INFORMATION pinfoCalc = (PROCESS_INFORMATION&)params;
    SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"Calc works!");
    WaitForSingleObject(pinfoCalc.hProcess, INFINITE);

    return 0;
}

DWORD WINAPI ButtonCalc() {
    ZeroMemory(&sinfoCalc, sizeof(STARTUPINFO));
    ZeroMemory(&pinfoCalc, sizeof(PROCESS_INFORMATION));
    if (processCalc == FALSE)
    {
        if (CreateProcessW(L"C:\\Windows\\WinSxS\\wow64_microsoft-windows-calc_31bf3856ad364e35_10.0.19041.1_none_6a03b910ee7a4073\\calc.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL,
            &sinfoCalc, &pinfoCalc))
        {
            processCalc = TRUE;
            CreateThread(NULL, 0, ThreadProcCalc, &pinfoCalc, 0, NULL);
        }
        else
        {
            SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"Creating Calc process error!");
        }
    }
    else {
        SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"Creating Calc process error! Already exist!");
    }
    return 0;
}

void StopThreadCalc() {
    if (processCalc)
    {
        TerminateProcess(pinfoCalc.hProcess, 0);
        CloseHandle(pinfoCalc.hThread);
        CloseHandle(pinfoCalc.hProcess);
        SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"Calc stopped!");
        processCalc = FALSE;

    }
    else {
        SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"Calc process doesn't exist!");
    }
}

