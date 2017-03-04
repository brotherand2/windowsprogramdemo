//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<strsafe.h>
#include"resource.h"

using namespace std;
//#ifndef UNICODE 
//typedef  string tstring;
//#else
//typedef  wstring tstring;
//#endif
#ifndef UNICODE 
using  tstring = string;
#else
using  tstring = wstring;
#endif
void FindWindowSize(int &x, int &y)
{
	HDC hdc = CreateIC(TEXT("DISPLAY"), nullptr, nullptr, nullptr);
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	x = tm.tmAveCharWidth * 12 + GetSystemMetrics(SM_CXBORDER)*2;
	y = tm.tmHeight * 2 + GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYBORDER)*2;
	DeleteDC(hdc);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static HDC hdcScreen;
	TEXTMETRIC tm;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	static COLORREF cr, crlast;
	RECT rect;
	POINT pt;
	TCHAR szBuffer[128];
	switch (UMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		hdcScreen = CreateDC(TEXT("DISPLAY"), nullptr, nullptr, nullptr);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		SetTimer(hwnd, 1, 100, nullptr);
		//SetMenu(hwnd,TEXT("IDR_MENU1"));
		break;
	case WM_TIMER:
		GetCursorPos(&pt);
		cr = GetPixel(hdcScreen, pt.x, pt.y);
		if (cr != crlast)
		{
			crlast = cr;
			InvalidateRect(hwnd, nullptr, true);
		}
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		StringCchPrintf(szBuffer, 128, TEXT("  %2X %2X %2X  "), GetRValue(cr), GetGValue(cr), GetBValue(cr));
		DrawText(hdc, szBuffer, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		DeleteObject(hdcScreen);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = TEXT("zeng");
	//wndclass.lpszMenuName = nullptr;
	wndclass.lpszMenuName = TEXT("IDR_MENU1");
	//wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONMY1));
	if (wndclass.hIconSm == NULL)
	{
		MessageBox(nullptr, TEXT("icon failed"), TEXT("error"), MB_ICONINFORMATION);

	}
	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	int x, y;
	//WS_BORDER|WS_OVERLAPPED|WS_CAPTION|
	FindWindowSize(x, y);
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("keyScroll"),WS_SYSMENU ,
		CW_USEDEFAULT,CW_USEDEFAULT, x, y, 
		nullptr, nullptr, hInstance, nullptr);
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}