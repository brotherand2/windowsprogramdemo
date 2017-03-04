//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<strsafe.h>
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
struct
{
	int istyle;
	TCHAR * szText;
}button[]=
{
	BS_PUSHBUTTON, TEXT("BS_PUSHBUTTON"),
	BS_DEFPUSHBUTTON, TEXT("BS_DEFPUSHBUTTON"),
	BS_CHECKBOX, TEXT("BS_CHECKBOX"),
	BS_AUTOCHECKBOX, TEXT("BS_AUTOCHECKBOX"),
	BS_RADIOBUTTON, TEXT("BS_RADIOBUTTON"),
	BS_AUTORADIOBUTTON, TEXT("BS_AUTORADIOBUTTON"),
	BS_3STATE, TEXT("BS_3STATE"),
	BS_AUTO3STATE, TEXT("BS_AUTO3STATE"),
	BS_GROUPBOX, TEXT("BS_GROUPBOX"),
	BS_OWNERDRAW, TEXT("BS_OWNERDRAW")
};
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	static HWND child[10];
	static TCHAR szTop[] = TEXT("message         wParam      lParam");
	static TCHAR szUnd[] = TEXT("_______         ______      ______");
	static TCHAR szFormat[] = TEXT("%-16s%04x-%04x   %04x-%04x");
	static RECT rect;
	TCHAR szBuffer[128];
	switch (UMsg)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());

		for (int i = 0; i < 10; i++)
			child[i] = CreateWindow(TEXT("button"), button[i].szText, WS_CHILD|WS_VISIBLE|button[i].istyle,0, 0, 0, 0,
			hwnd, (HMENU)i, ((LPCREATESTRUCT)(lParam))->hInstance, nullptr);
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		rect.left = 24 * cxChar;
		rect.top = 2 * cyChar;
		rect.right = cxClient;
		rect.bottom = cyClient;
		for (int i = 0; i < 10; i++)
			MoveWindow(child[i], cxChar, (1 + 2 * i)*cyChar, 20 * cxChar, cyChar * 7 / 4,true);
		break;
	case WM_PAINT:
		//InvalidateRect(hwnd, &rect, true);
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc,GetStockObject(SYSTEM_FIXED_FONT));
		TextOut(hdc, 24 * cxChar, cyChar, szTop, lstrlen(szTop));
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc, 24 * cxChar, cyChar, szUnd, lstrlen(szUnd));

		EndPaint(hwnd, &ps);
		break;
	case WM_DRAWITEM:
	case WM_COMMAND:
		hdc = GetDC(hwnd);
		ScrollWindow(hwnd, 0, -cyChar, &rect, &rect);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		StringCchPrintf(szBuffer, 128, szFormat, UMsg == WM_DRAWITEM ? TEXT("WM_DRAWTIEM") : TEXT("WM_COMMAND"),
			HIWORD(wParam), LOWORD(wParam), HIWORD(lParam), LOWORD(lParam));
		TextOut(hdc, 24 * cxChar, cyChar*(rect.bottom/cyChar-1), szBuffer, lstrlen(szBuffer));
		ReleaseDC(hwnd, hdc);
		ValidateRect(hwnd, &rect);
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