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
BOOL Colision;
std::vector<Point> data;

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
	x = 740;
	y = 100;
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

void update (HWND hwnd, HDC hdc)
{	  
 RECT rcClient;
   ::GetClientRect(hwnd, &rcClient);
   int left = rcClient.left;
   int top = rcClient.top;
   int width = rcClient.right - rcClient.left;
   int height = rcClient.bottom - rcClient.top;

   HDC hdcMem = ::CreateCompatibleDC(hdc);
   const int nMemDC = ::SaveDC(hdcMem);

   HBITMAP hBitmap = ::CreateCompatibleBitmap(hdc, width, height);
   ::SelectObject(hdcMem, hBitmap);

   Graphics graphics(hdcMem);
   SolidBrush back(Color(255,255,255));
   graphics.FillRectangle(&back, left, top, width, height);

   Pen black(Color(255, 0, 0, 0), 4);
    Pen blue (Color(255, 0, 0, 255));
    Pen red (Color(255, 255, 0, 0));
	Pen MyPen(Color(150, 0, 0, 0));
	SolidBrush grey(Color(255, 128, 128, 128));

	Image image(L"Crane.png");

	for(int i = 0; i < 3; i++) {
		graphics.DrawRectangle(&MyPen, data[i].X, data[i].Y, 50, 50);
	}
	graphics.FillRectangle(&grey, 0, 630, 1200, 630);
	graphics.DrawEllipse(&MyPen, data[3].X, data[3].Y, 50, 50);
	graphics.DrawLine(&MyPen, 0, 630, 1200, 630);
	graphics.DrawLine(&MyPen, x, 160, x, 160+y);
	graphics.DrawImage(&image, 10, 10);

   RECT rcClip;
   ::GetClipBox(hdc, &rcClip);
   left = rcClip.left;
   top = rcClip.top;
   width = rcClip.right - rcClip.left;
   height = rcClip.bottom - rcClip.top;
   ::BitBlt(hdc, left, top, width, height, hdcMem, left, top, SRCCOPY);

   ::RestoreDC(hdcMem, nMemDC);
   ::DeleteObject(hBitmap);
}

void inputData()
{	
	data.push_back(Point(300, 580));
	for (int i = 1; i < 4; i++){
		data.push_back(Point(300+100*i, 580));
	}
}

int OnCreate(HWND window)
{
	inputData();
	return 0;
}

int CheckPosition(int number) {
	int tab[4];
	int tabi = 0;
	for(int t = 0; t < 4; t++) {
		if((data[number].X >= data[t].X && data[number].X <= data[t].X+50) || (data[number].X+50 >= data[t].X && data[number].X+50 <= data[t].X+50)) {
			if(number != t) {
				tab[tabi] = t;
				tabi++;
			}
		}
	}
	if(tabi > 0) {
		for(int tabin = 0; tabin < tabi; tabin++) {
			if(data[tab[tabin]].Y < 100) return 6;
			else if(data[tab[tabin]].Y < 400) return tab[tabin];
		}
		return tab[0];
	}
	return 5;
}

bool CheckColision(int f) {
	if(data[f].Y-160 == y && data[f].X < x && data[f].X+50 > x) return TRUE;
	else return FALSE;
}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps)
{
	InvalidateRect(hWnd, NULL, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	update(hWnd, hdc);
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
      CW_USEDEFAULT, 0, 800, 750, NULL, NULL, hInstance, NULL);

   OnCreate(hWnd);

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
	int Colisions = 0;
	int ColisionsPosition = 5;
	int ColisionsBottom = 5;
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
					SetTimer(hWnd, TMR_1, 10, 0);
				break;
				case VK_UP:
					SetTimer(hWnd, TMR_2, 10, 0);
				break;
				case VK_RIGHT:
					SetTimer(hWnd, TMR_3, 10, 0);
				break;
				case VK_LEFT:
					SetTimer(hWnd, TMR_4, 10, 0);
				break;
				case VK_SPACE:
					for(int num = 0; num < 4; num++) {
						if(CheckColision(num) == TRUE) {
							ColisionsBottom = CheckPosition(num);
							if(ColisionsBottom < 5) ColisionsPosition = num;
							else ColisionsPosition = 5;
						}
					}
					if(ColisionsBottom != 6) {
						Colision = !Colision;
						if(Colision == FALSE) SetTimer(hWnd, TMR_5, 10, 0);
					}
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
				for(int i = 0; i < 4; i++) {
					if(i != 3 && CheckColision(i) == TRUE && Colision == TRUE) {
						if(data[i].Y < 580) {
							data[i].Y++;
						}
					}
					else if(CheckColision(i) == TRUE && Colision == FALSE) y--;
				}
				if(y+160 >= 580 && Colision == TRUE) y = y;
				else y++;
			break;
			case TMR_2:
				repaintWindow(hWnd, hdc, ps);
				for(int i = 0; i < 3; i++) {
					if(i != 3 && CheckColision(i) == TRUE && Colision == TRUE) {
						if(y > 0) {
							data[i].Y--;
						}
					}
				}
				if(y < 0 && Colision == TRUE) y = y;
				else if(y > 0) y--;
			break;
			case TMR_3:
				repaintWindow(hWnd, hdc, ps);
				for(int i = 0; i < 3; i++) {
					if(CheckColision(i) == TRUE && Colision == TRUE) {
						if(x <= 740){
							data[i].X++;
						}
					}
				}
				if (x <= 740) x++;
			break;
			case TMR_4:
				repaintWindow(hWnd, hdc, ps);
				for(int i = 0; i < 3; i++) {
					if(CheckColision(i) == TRUE && Colision == TRUE) {
						if(x >= 280) {
							data[i].X--;
						}
					}
				}
				if (x >= 280) x--;		
			break;
			case TMR_5:
				repaintWindow(hWnd, hdc, ps);
				if(ColisionsPosition < 5) {
					if(ColisionsBottom < 5) {
						if(data[ColisionsPosition].Y < data[ColisionsBottom].Y) data[ColisionsPosition].Y--;
						else if(data[ColisionsPosition].Y == data[ColisionsBottom].Y) Colisions = 3; 
					}
					else {
						if(data[ColisionsPosition].Y < 580) data[ColisionsPosition].Y--;
						if(data[ColisionsPosition].Y == 580) Colisions = 3;
					}
				}
				if(Colisions == 3) {
					Colisions = 0;
					KillTimer(hWnd, TMR_5);
				}
			break;
			}
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			update(hWnd, hdc);
			EndPaint(hWnd, &ps);
			break;
		case WM_ERASEBKGND:
			return FALSE;
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