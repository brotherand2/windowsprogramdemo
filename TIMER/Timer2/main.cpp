//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<ctime>
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
COLORREF color=RGB(255,255,255);
const int DIVISIONS = 10;
static int x, y;
static int cxBlock, cyBlock;
void _stdcall TimerProc(HWND hwnd, unsigned message, unsigned int nTimerCount, unsigned long dwTime)
{
	color = RGB(rand() % 255, rand()%255, rand() % 255);
	x = rand() % 10;
	y = rand() % 10;
	RECT  rect;
	rect.left = x*cxBlock;
	rect.top = y*cyBlock;
	rect.right = (x + 1)*cxBlock;
	rect.bottom = (y + 1)*cyBlock;
	InvalidateRect(hwnd,&rect,true );
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	RECT rect;
	HBRUSH hBrush, oldBrush;
	switch (UMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		srand(time(0));
		SetTimer(hwnd, 1, 5, TimerProc);
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		cxBlock = cxClient / DIVISIONS;
		cyBlock = cyClient / DIVISIONS;
		break;
	case WM_LBUTTONDOWN:
		hdc = CreateDC(TEXT("DISPLAY"), nullptr, nullptr, nullptr);
		Rectangle(hdc, 0, 0, 100, 100);
		DeleteDC(hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		rect = ps.rcPaint;
		hBrush= CreateSolidBrush(color);
		FillRect(hdc,&rect,hBrush);
		DeleteObject(hBrush);
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hwnd,1);
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
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}