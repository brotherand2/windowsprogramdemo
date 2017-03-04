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

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	int wmId, wmEvent;
	static HWND hRadioButton[3];
	static HWND hGroup;
	static HWND hChecked;
	TCHAR *light[3] = { TEXT("red"), TEXT("green"), TEXT("blue") };
	COLORREF color[3] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255) };
	static HBRUSH hBrush;
	switch (UMsg)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		for (int i = 0; i < 3; i++)
			hRadioButton[i] = CreateWindow(TEXT("button"), light[i], WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
			0, 0, 0, 0, hwnd, (HMENU)i, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);
		hGroup = CreateWindow(TEXT("button"), TEXT("ºìÂÌµÆ"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			0, 0, 0, 0, hwnd, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, nullptr);
		SendMessage(hRadioButton[0], BM_SETCHECK, 1, 0);
		hChecked = hRadioButton[0];
		hBrush = CreateSolidBrush(color[0]);
		DeleteObject((HBRUSH)SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)hBrush));
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		for (int i = 0; i < 3; i++)
			MoveWindow(hRadioButton[i],cxChar/2, (2 +2*i)* cyChar, 20 * cxChar,3 * cyChar / 2,true);
		MoveWindow(hGroup, cxChar/4, 0, 21 * cxChar, 8 * cyChar,true);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam); 
		SendMessage(hChecked, BM_SETCHECK, 0, 0);
		hChecked = hRadioButton[wmId];
		hBrush = CreateSolidBrush(color[wmId]);
		DeleteObject((HBRUSH)SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)hBrush));
		InvalidateRect(hwnd, nullptr, true);
		SendMessage(hChecked, BM_SETCHECK, 1, 0);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		DeleteObject(hBrush);
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
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("ÓÂ»ª"), WS_OVERLAPPEDWINDOW,
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