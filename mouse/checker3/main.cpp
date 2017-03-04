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
const int DIVISIONS = 5;
long _stdcall WndChildProc(HWND hwnd, unsigned int message, unsigned int wParam, long lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	TCHAR szBuffer[128];
	switch (message)
	{
	case WM_CREATE:
		SetWindowLong(hwnd, 0, 0);
		SetWindowLong(hwnd, 4, 110);
		break;
	case WM_LBUTTONDOWN:
		//sprintf_s(szBuffer, TEXT("%d"), GetWindowLong(hwnd,4));
		//MessageBox(nullptr, TEXT(""), szBuffer, MB_OK);
		SetWindowLong(hwnd, 0, 1 ^ GetWindowLong(hwnd, 0));
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		if (GetWindowLong(hwnd, 0))
		{
			MoveToEx(hdc, 0, 0, nullptr);
			LineTo(hdc, rect.right, rect.bottom);
			MoveToEx(hdc, 0, rect.bottom, nullptr);
			LineTo(hdc, rect.right, 0);
		}
		EndPaint(hwnd, &ps);
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	static HWND childWnd[DIVISIONS][DIVISIONS];
	TCHAR childName[] = TEXT("chindWindow");
	static int cxBlock, cyBlock;
	switch (UMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		for (int i = 0; i < DIVISIONS; i++)
		{
			for (int j = 0; j < DIVISIONS; ++j)
				childWnd[i][j] = CreateWindow(childName, nullptr, WS_CHILDWINDOW | WS_VISIBLE,
				0, 0, 0, 0, hwnd, HMENU(j << 16 | i),(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE), 0);
		}
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		cxBlock = cxClient / DIVISIONS;
		cyBlock = cyClient / DIVISIONS;
		for (int i = 0; i < DIVISIONS; i++)
		{
			for (int j = 0; j < DIVISIONS; ++j)
				MoveWindow(childWnd[i][j], i*cxBlock, j*cyBlock, cxBlock, cyBlock, true);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
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
	TCHAR childName[] = TEXT("chindWindow");
	wndclass.cbWndExtra = sizeof(long) * 2;
	wndclass.lpfnWndProc = WndChildProc;
	wndclass.lpszClassName = childName;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("keyScroll"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
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
		DispatchMessage(&msg);
	}
}