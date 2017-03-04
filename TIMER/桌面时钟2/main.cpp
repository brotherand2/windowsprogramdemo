#include<Windows.h>
#include<string>
#include<vector>
#include<strsafe.h>
using namespace std;
#ifndef UNICODE 
using  tstring = string;
#else
using  tstring = wstring;
#endif
static int cxClient, cyClient;
static HBRUSH hBrushRed;
HWND hWND;
HRGN hRgn = CreateRectRgn(0,0,30,30);
static bool sevenSegment[10][7] =
{
	{ 1, 1, 1, 0, 1, 1, 1 },
	{ 0, 0, 1, 0, 0, 1, 0 },
	{ 1, 0, 1, 1, 1, 0, 1 },
	{ 1, 0, 1, 1, 0, 1, 1 },
	{ 0, 1, 1, 1, 0, 1, 0 },
	{ 1, 1,0, 1,0, 1, 1 },
	{ 1, 1, 0, 1, 1, 1, 1 },//6
	{ 1, 0, 1, 0, 0, 1, 0 },
	{ 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 0, 1, 1 }
};
static POINT ptSegment[7][6] =
{
	{ 7, 6, 11, 2, 31, 2, 35, 6, 31, 10, 11, 10 },
	{ 6, 7, 10, 11, 10, 31, 6, 35, 2, 31, 2, 11 },
	{ 36, 7, 40, 11, 40, 31, 36, 35, 32, 31, 32, 11 },
	{ 7, 36, 11, 32, 31, 32, 35, 36, 31, 40, 11, 40 },
	{ 6, 37, 10, 41, 10, 61, 6, 65, 2, 61, 2, 41 },
	{ 36, 37, 40, 41, 40, 61, 36, 65, 32, 61, 32, 41 },
	{ 7, 66, 11, 62, 31, 62, 35, 66, 31, 70, 11, 70 }
};
static POINT colon[2][4] = {
		{ 2, 21, 6, 17, 10, 21, 6, 25 },
		{ 2, 51, 6, 47, 10, 51, 6, 55 },
};
vector<tstring>allTitle;
tstring temp;
POINT copyPoint[7][6];
POINT copyColon[2][4];
void DisplayDigit(HDC hdc, int number)
{
	for (int i = 0; i < 7; i++)
		if (sevenSegment[number][i])
		{
		HRGN temp = CreatePolygonRgn(copyPoint[i], 6, WINDING);
			CombineRgn(hRgn, temp, hRgn, RGN_OR);
			DeleteObject(temp);
		}
}
void OffsetPoint(HDC hdc, int num, int colon)
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			copyPoint[i][j].x = ptSegment[i][j].x + num * 42 + colon * 12;
			copyPoint[i][j].y = ptSegment[i][j].y;
		}
	}
}
void OffsetColon(HDC hdc, int num, int col)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			copyColon[i][j].x = colon[i][j].x + num * 42 + col * 12;
			copyColon[i][j].y = colon[i][j].y;
		}
	}
}
void DisplayTwoDigit(HDC hdc, int number, bool fSuppress,int row,int col)
{
	OffsetPoint(hdc, row, col);
	if (!fSuppress || (number / 10 != 0))
		DisplayDigit(hdc, number / 10);
	OffsetPoint(hdc, row+1,col);
	DisplayDigit(hdc, number % 10);
}
void DisplayColon(HDC hdc)
{
	HRGN rgn1 = CreatePolygonRgn(copyColon[0], 4, WINDING);
	HRGN rgn2 = CreatePolygonRgn(copyColon[1], 4, WINDING);
	CombineRgn(rgn1, rgn1, rgn2, RGN_OR);
	CombineRgn(hRgn, rgn1,hRgn,RGN_OR);
	DeleteObject(rgn1);
	DeleteObject(rgn2);
}
void DisplayTime( HWND hwnd,HDC hdc, bool f24hour, bool fSuppress)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	if (f24hour)
		DisplayTwoDigit(hdc, st.wHour, fSuppress,0,0);
	else
		DisplayTwoDigit(hdc, (st.wHour %= 12) ? st.wHour : 12, fSuppress,1,0);
	OffsetColon(hdc, 2, 0);
	DisplayColon(hdc);
	DisplayTwoDigit(hdc, st.wMinute, false,2,1);
	OffsetColon(hdc, 4, 1);
	DisplayColon(hdc);
	DisplayTwoDigit(hdc, st.wSecond, false,4,2);
	SetWindowOrgEx(hdc, 138, 36, nullptr);
}
void SetIsotropirc(HDC hdc)
{
	SelectObject(hdc, hBrushRed);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	SetMapMode(hdc, MM_ISOTROPIC);
	SetWindowExtEx(hdc, 276, 72, nullptr);
	SetViewportExtEx(hdc, cxClient, cyClient, nullptr);
	SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, nullptr);
	SetWindowOrgEx(hdc, 138, 36, nullptr);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int cxChar, cxCaps, cyChar;
	TCHAR szBuffer[128];
	static bool f24hour, fSuppress;
	static SYSTEMTIME prev;
	SYSTEMTIME st;
	static NOTIFYICONDATA nid;
	TCHAR info[256] = TEXT("你洗澡了吗？");
	long style;
	static HWND hDestop;//桌面句柄
	static int cxScreen, cyScreen;//屏幕宽度、高度
	static HINSTANCE hInst;
	HRGN rgnTest;
	switch (UMsg)
	{
	case WM_CREATE:
		hWND = hwnd;
		nid.cbSize = sizeof(nid);
		nid.hIcon = LoadIcon(nullptr, IDI_INFORMATION);
		nid.hWnd = hwnd;
		nid.uID = 1;
		nid.uTimeout = 2000;
		nid.dwInfoFlags = NIIF_INFO;
		StringCchCopy(nid.szInfo, 256, info);
		StringCchCopy(nid.szInfoTitle, 64, TEXT("我说:"));
		StringCchCopy(nid.szTip, 128, TEXT("时间显示了"));
		nid.uFlags = NIF_ICON | NIF_MESSAGE ;
		nid.uCallbackMessage = WM_USER;
		Shell_NotifyIcon(NIM_ADD, &nid);
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		SetTimer(hwnd, 1, 1000, nullptr);
		hBrushRed = CreateSolidBrush(RGB(255, 0, 0));
		hWND = hwnd;
		cxScreen = GetSystemMetrics(SM_CXSCREEN);////屏幕宽度、高度
		cyScreen = GetSystemMetrics(SM_CYSCREEN);
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
	case WM_SETTINGCHANGE:
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, 2);
		f24hour = (szBuffer[0] == '1');
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2);
		fSuppress = (szBuffer[0] == '0');
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_USER:
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			SendMessage(hwnd, WM_DESTROY, wParam, lParam);
			break;
		case WM_LBUTTONDOWN:
			ShowWindow(hwnd, SW_NORMAL);
			SetForegroundWindow(hwnd);
			break;
		}
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			InvalidateRect(hwnd, 0, true);
			break;
		}
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DeleteObject(hRgn);
		hRgn = CreateRectRgn(0, 0, 1, 1);
		//rgnTest = CreateRectRgn(110, 110, 331, 331);
		//FillRgn(hdc, rgnTest, hBrushRed);
		//SetWindowRgn(hwnd,rgnTest,true);
		//CombineRgn(rgnTest, rgnTest, hRgn, RGN_OR);
		SetIsotropirc(hdc);

		DisplayTime(hwnd,hdc, f24hour, fSuppress);
		SetBkMode(hdc, TRANSPARENT);

		//FillRgn(hdc,hRgn, hBrushRed);


		EndPaint(hwnd, &ps);
		SetWindowRgn(hwnd,hRgn,true);

		return 0;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hwnd, 1);
		KillTimer(hwnd, 2);
		DeleteObject(hBrushRed);
		Shell_NotifyIcon(NIM_DELETE, &nid);
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = CreateSolidBrush(RGB(0,255,0));
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
	HWND hwnd = CreateWindow(TEXT("zeng"), 0,WS_POPUP,
		122, 122, 700,700,
		nullptr, nullptr, hInstance, nullptr);

	//HWND hwnd = CreateWindowEx(WS_EX_LAYERED|WS_EX_TOPMOST|WS_EX_TOOLWINDOW,TEXT("zeng"), 0, WS_POPUP|WS_CLIPSIBLINGS|WS_VISIBLE,
	//	422, 122, 200,200,
	//	0,nullptr, hInstance, nullptr);
	//SetLayeredWindowAttributes(hwnd, RGB(0,255, 0),55,LWA_ALPHA);
	hWND = hwnd;
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	//ShowWindow(hwnd1, SW_SHOWNORMAL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}