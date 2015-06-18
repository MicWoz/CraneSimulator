// CraneSimulator.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CraneSimulator.h"
#include <vector>
#include <cstdio>

#define MAX_LOADSTRING 100
#define TMR_1 1
#define TMR_2 2
#define TMR_3 3
#define TMR_4 4
#define TMR_5 5

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

INT x;
INT y;
INT Rx;
INT Ry;
BOOL Colision;
int col = 0;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	x = 100;
	y = 100;
	Rx = 400;
	Ry = 400;
	Colision = FALSE;

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;


	GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CRANESIMULATOR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CRANESIMULATOR));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);
	return (int) msg.wParam;
}

void MyOnPaint(HDC hdc)
{
	Graphics graphics(hdc);
    Pen blue (Color(255, 0, 0, 255));
    Pen red (Color(255, 255, 0, 0));

	graphics.DrawRectangle(&red, 0+Rx, 0+Ry, 100, 100);
	graphics.DrawLine(&red, 100+x, 0, 100+x, 0+y);
}

void MyBackgroundPaint(HDC hdc)
{

}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps)
{
	InvalidateRect(hWnd, NULL, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	MyOnPaint(hdc);
	EndPaint(hWnd, &ps);
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CRANESIMULATOR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CRANESIMULATOR);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

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
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

		switch (message)
		{
		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			// Parse the menu selections:
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
			break;
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_DOWN:
					SetTimer(hWnd, TMR_1, 25, 0);
				break;
				case VK_UP:
					SetTimer(hWnd, TMR_2, 25, 0);
				break;
				case VK_RIGHT:
					SetTimer(hWnd, TMR_3, 25, 0);
				break;
				case VK_LEFT:
					SetTimer(hWnd, TMR_4, 25, 0);
				break;
				case VK_SPACE:
					Colision = !Colision;
					if(Colision == FALSE) SetTimer(hWnd, TMR_5, 25, 0);
				break;
			}
			break;
		case WM_KEYUP:
			switch (wParam)
			{
				case VK_DOWN:
					KillTimer(hWnd, TMR_1);
				break;
				case VK_UP:
					KillTimer(hWnd, TMR_2);
				break;
				case VK_RIGHT:
					KillTimer(hWnd, TMR_3);
				break;
				case VK_LEFT:
					KillTimer(hWnd, TMR_4);
				break;
			}
			break;
		case WM_TIMER:
			switch (wParam)
			{
			case TMR_1:
				repaintWindow(hWnd, hdc, ps);
				if(Ry == y && Rx > x && x > Rx-100 && Colision == TRUE) {
					y++;
					Ry++;
				}
				else if(Ry == y && Rx > x && x > Rx-100 && Colision == FALSE) y = y;
				else y++;
			break;
			case TMR_2:
				repaintWindow(hWnd, hdc, ps);
				if(Ry == y && Rx > x && x > Rx-100 && Colision == TRUE) {
					if(y > 0) {
						y--;
						Ry--;
					}
				}
				else y--;
			break;
			case TMR_3:
				repaintWindow(hWnd, hdc, ps);
				if(Ry == y && Rx > x && x > Rx-100 && Colision == TRUE) {
					x++;
					Rx++;
				}
				else x++;
			break;
			case TMR_4:
				repaintWindow(hWnd, hdc, ps);
				if(Ry == y && Rx > x && x > Rx-100 && Colision == TRUE) {
					if(x > 0) {
						x--;
						Rx--;
					}
				}
				else x--;		
			break;
			case TMR_5:
				repaintWindow(hWnd, hdc, ps);
				if(Ry < 400) {
					Ry++;
				}		
			break;
			}
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			MyBackgroundPaint(hdc);
			MyOnPaint(hdc);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	return 0;
}

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