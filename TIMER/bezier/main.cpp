//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<vector>
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
const int num = 6;
const int size = num * 3 + 1;
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int cxClient, cyClient, cxScreen, cyScreen;
	static int cxChar, cxCaps, cyChar;
	static POINT bezier[size];
	static HPEN hpen;
	int x, y;
	switch (UMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		cxScreen = GetSystemMetrics(SM_CXSCREEN);
		cyScreen = GetSystemMetrics(SM_CYSCREEN);
		SetTimer(hwnd, 1, 100, nullptr);
		srand(time(0));

		hpen = CreatePen(PS_SOLID, 1, RGB(rand() % 255, rand() % 255, rand() % 255));

		break;
	case WM_LBUTTONDOWN:
		hdc = CreateDC(TEXT("DISPLAY"), nullptr, nullptr, nullptr);
		Rectangle(hdc, 0, 0, 54, 44);
		bezier[0].x = bezier[size - 1].x;
		bezier[0].y = bezier[size - 1].x;
		for (int i = 1; i < size; i++)
		{
			bezier[i].x = rand() % cxScreen;
			bezier[i].y = rand() % cyScreen;
		}
		SelectObject(hdc, hpen);
		PolyBezier(hdc, bezier, size);

		DeleteDC( hdc);
		break;
	case WM_TIMER:

		//UpdateWindow(nullptr);
		//RedrawWindow(nullptr, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
		hdc = CreateDC(TEXT("DISPLAY"), nullptr, nullptr, nullptr);
		bezier[0].x = bezier[size - 1].x;
		bezier[0].y = bezier[size - 1].x;
		for (int i = 1; i < size; i++)
		{
			bezier[i].x = rand() % cxScreen;
			bezier[i].y = rand() % cyScreen;
		}
		SelectObject(hdc, hpen);
		InvalidateRect(nullptr, nullptr, true);
		UpdateWindow(nullptr);
		Sleep(10);
		PolyBezier(hdc, bezier, size);
		// x = rand() % cxScreen;
		// y = rand() % cyScreen;
		//Rectangle(hdc,x ,y , x+133, y+133);
		DeleteDC(hdc);
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(nullptr, &ps);

		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hwnd, 1);
		DeleteObject(hpen);

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
		CW_USEDEFAULT, CW_USEDEFAULT, 2, 2,
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