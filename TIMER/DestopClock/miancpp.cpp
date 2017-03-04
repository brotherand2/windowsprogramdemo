//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
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
void DisplayDigit(HDC hdc, int number)
{
	static bool sevenSegment[10][7]=
	{
		{1,1,1,0,1,1,1},
		{0,0,1,0,0,1,0},
		{1,1,0,1,0,1,1},
		{1,0,1,1,0,1,1},
		{0,0,1,1,1,1,0},
		{1,0,1,1,1,0,1},
		{1,1,1,1,1,0,1},
		{0,0,1,0,0,1,1},
		{1,1,1,1,1,1,1},
		{1,0,1,1,1,1,1}
	};
	static POINT ptSegment[7][6]=
	{
		{7,6 ,11,2 ,31,2  ,35,6  ,31,10, 11,10},
		{6,7, 10,11,10,31, 6,35,  2,31,  2,11},
		{36,7,40,11,40,31,36,35,32,31,32,11},
		{7,36,11,32,31,32,35,36,31,40,11,40},
		{6,37,10,41,10,61,6,65,2,61,2,41},
		{36,37,40,41,40,61,36,65,32,61,32,41},
		{7,66,11,62,31,62,35,66,31,70,11,70}
	};
	for (int i = 0; i < 7; i++)
		if (sevenSegment[number][i])
			Polygon(hdc, ptSegment[i],6);
}
void DisplayTwoDigit(HDC hdc, int number,bool fSuppress)
{
	if (!fSuppress||(number /10 != 0))
		DisplayDigit(hdc, number /10);
	OffsetWindowOrgEx(hdc, -42, 0, nullptr);
	DisplayDigit(hdc, number % 10);
	OffsetWindowOrgEx(hdc, -42, 0, nullptr);
}
void DisplayColon(HDC hdc)
{
	static POINT colon[2][4] = { 
			{2,21,6,17,10,21,6,25},
			{2,51,6,47,10,51,6,55},
			 };
	Polygon(hdc, colon[0], 4);
	Polygon(hdc, colon[1], 4);
	OffsetWindowOrgEx(hdc, -12, 0, nullptr);
}
void DisplayTime(HDC hdc,bool f24hour,bool fSuppress)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	if (f24hour)
		DisplayTwoDigit(hdc, st.wHour,fSuppress);
	else
		DisplayTwoDigit(hdc, (st.wHour %= 12) ? st.wHour : 12, fSuppress);
	DisplayColon(hdc);
	DisplayTwoDigit(hdc, st.wMinute, false);
	DisplayColon(hdc);
	DisplayTwoDigit(hdc, st.wSecond,false);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int cxClient, cyClient,cxScreen,cyScreen;
	static int cxChar, cxCaps, cyChar;
	static HBRUSH hBrushRed;
	TCHAR szBuffer[2];
	static bool f24hour, fSuppress;
	static HDC hdcScreen;
	RECT rect;
	switch (UMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		hdcScreen = CreateDC("DISPLAY", nullptr, nullptr, nullptr);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		SetTimer(hwnd, 1, 1000, nullptr);
		hBrushRed = CreateSolidBrush(RGB(255, 0, 0));


	case WM_SETTINGCHANGE:
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME,szBuffer, 2);
		f24hour = (szBuffer[0] == '1');
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2);
		fSuppress = (szBuffer[0] == '0');
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_TIMER:
		rect.left = rect.top = 0;
		rect.right = 276;
		rect.left = 72;
		SetMapMode(hdcScreen, MM_ISOTROPIC);
		SetWindowExtEx(hdcScreen, 276, 72, nullptr);
		SetViewportExtEx(hdcScreen, cxScreen, -cyScreen, nullptr);
		SetViewportOrgEx(hdcScreen, cxScreen / 2, cyScreen / 2, nullptr);
		SetWindowOrgEx(hdcScreen, 138, 36, nullptr);
		SelectObject(hdcScreen, hBrushRed);
		SelectObject(hdcScreen, GetStockObject(NULL_PEN));
		Sleep(1000);
		InvalidateRect(nullptr, &rect, true);

		DisplayTime(hdcScreen, f24hour, fSuppress);

		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		cxScreen = GetSystemMetrics(SM_CXSCREEN);
		cyScreen = GetSystemMetrics(SM_CYSCREEN);
		break;
	//case WM_PAINT:
	//	hdc = BeginPaint(hwnd, &ps);
	//	EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hwnd, 1);
		DeleteObject(hBrushRed);
		DeleteDC(hdcScreen);
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = TEXT("zeng");
	wndclass.lpszMenuName = nullptr;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("keyScroll"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 11, 11,
		nullptr, nullptr, hInstance, nullptr);
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	ShowWindow(hwnd, SW_MINIMIZE);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}