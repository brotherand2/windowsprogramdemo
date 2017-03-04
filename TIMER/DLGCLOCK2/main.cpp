//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<vector>
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
static int cxClient, cyClient;
static HBRUSH hBrushRed;
HWND hWND;
static bool sevenSegment[10][7] =
{
	{ 1, 1, 1, 0, 1, 1, 1 },
	{ 0, 0, 1, 0, 0, 1, 0 },
	{ 1, 1, 0, 1, 0, 1, 1 },
	{ 1, 0, 1, 1, 0, 1, 1 },
	{ 0, 0, 1, 1, 1, 1, 0 },
	{ 1, 0, 1, 1, 1, 0, 1 },
	{ 1, 1, 1, 1, 1, 0, 1 },
	{ 0, 0, 1, 0, 0, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 1, 1, 1, 1, 1 }
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
vector<tstring>allTitle;
tstring temp;
int _stdcall EnumWindowProc(HWND hwnd, long lParam)
{
	TCHAR title[128];
	HDC hdc = GetDC(hWND);
	if (IsWindow(hwnd) && IsWindowVisible(hwnd)&&!IsIconic(hwnd))
	{
		GetWindowText(hwnd, title, 128);
	    //if (lstrcmp(title, TEXT("DLGCLOCK2")));
	    // MessageBox(hwnd, title, TEXT(""), 0);
		//TextOut(hdc, 110, 0, title, lstrlen(title));

		temp+=title + tstring(TEXT(" "));
		if (title[0]!='\0')
		allTitle.push_back(title);
	}
	ReleaseDC(hWND, hdc);
	return true;
}
void DisplayDigit(HDC hdc, int number)
{
	for (int i = 0; i < 7; i++)
		if (sevenSegment[number][i])
			Polygon(hdc, ptSegment[i], 6);
}
void OffsetPoint(HDC hdc,int num,int colon,int number)
{
	POINT copy[7][6];
	RECT rect;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			copy[i][j].x = ptSegment[i][j].x+num * 42 + colon * 12;
			copy[i][j].y = ptSegment[i][j].y;
		}
	}
	rect.left = num * 42 + colon * 12;
	rect.top = 0;
	rect.right = 42 + num * 42 + colon * 12;
	rect.bottom = 72;
	SelectObject(hdc, GetStockObject(NULL_PEN));
	HBRUSH brush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	FillRect(hdc, &rect, brush);
	for (int i = 0; i < 7; i++)
		if (sevenSegment[number][i])
			Polygon(hdc,copy[i], 6);
}
void DisplayTwoDigit(HDC hdc, int number, bool fSuppress)
{
	if (!fSuppress || (number / 10 != 0))
		DisplayDigit(hdc, number / 10);
	OffsetWindowOrgEx(hdc, -42, 0, nullptr);
	DisplayDigit(hdc, number % 10);
	OffsetWindowOrgEx(hdc, -42, 0, nullptr);
}
void DisplayColon(HDC hdc)
{
	static POINT colon[2][4] = {
			{ 2, 21, 6, 17, 10, 21, 6, 25 },
			{ 2, 51, 6, 47, 10, 51, 6, 55 },
	};
	Polygon(hdc, colon[0], 4);
	Polygon(hdc, colon[1], 4);
	OffsetWindowOrgEx(hdc, -12, 0, nullptr);
}
void DisplayTime(HDC hdc, bool f24hour, bool fSuppress)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	if (f24hour)
		DisplayTwoDigit(hdc, st.wHour, fSuppress);
	else
		DisplayTwoDigit(hdc, (st.wHour %= 12) ? st.wHour : 12, fSuppress);
	DisplayColon(hdc);
	DisplayTwoDigit(hdc, st.wMinute, false);
	DisplayColon(hdc);
	DisplayTwoDigit(hdc, st.wSecond, false);
}
void SetIsotropirc(HDC hdc)
{
	SelectObject(hdc, hBrushRed);
	SelectObject(hdc, GetStockObject(NULL_PEN));

	SetMapMode(hdc, MM_ISOTROPIC);
	SetWindowExtEx(hdc, 276, 72, nullptr);
	SetViewportExtEx(hdc, cxClient, -cyClient, nullptr);
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
	TCHAR info[256]=TEXT("张绍鹏你洗澡了吗？");
	long style;
	switch (UMsg)
	{
	case WM_CREATE:
		nid.cbSize = sizeof(nid);
		nid.hIcon = LoadIcon(nullptr, IDI_INFORMATION);
		nid.hWnd = hwnd;
		nid.uID = 1;
		nid.uTimeout = 2000;
		nid.dwInfoFlags =NIIF_INFO;
		StringCchCopy(nid.szInfo, 256, info);
		StringCchCopy(nid.szInfoTitle, 64, TEXT("我说:"));
	
		StringCchCopy(nid.szTip,128, TEXT("时间显示了"));
		nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_INFO|NIF_TIP;
		nid.uCallbackMessage = WM_USER;
		Shell_NotifyIcon(NIM_ADD, &nid);
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		SetTimer(hwnd, 1, 1000, nullptr);
		SetTimer(hwnd, 2, 600000, nullptr);
		hBrushRed = CreateSolidBrush(RGB(255, 0, 0));
		style = GetWindowLong(hwnd, GWL_EXSTYLE);
		hWND = hwnd;
		EnumWindows(EnumWindowProc, 0);
		//SetWindowLong(hwnd, GWL_EXSTYLE,style |WS_EX_TOOLWINDOW);
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
			SendMessage(hwnd, WM_DESTROY,wParam, lParam);
			break;
		case WM_LBUTTONDOWN:
			ShowWindow(hwnd, SW_NORMAL);
			SetForegroundWindow(hwnd);
			break;
		}
		break;
	case WM_TIMER:
		//if (hwnd == GetActiveWindow())
			//MessageBox(nullptr, TEXT("active"), TEXT(""), 0);
		switch (wParam)
		{
			case 1:
			{
				hdc = GetDC(hwnd);
				GetLocalTime(&st);
				SetIsotropirc(hdc);
				if (prev.wHour != st.wHour)
				{
					if (st.wHour % 10 == 0)
					{
						if (f24hour)
						{
							if (!fSuppress || (st.wHour / 10 != 0))
								OffsetPoint(hdc, 0, 0, st.wHour / 10);
							OffsetPoint(hdc, 1, 0, st.wHour % 10);

						}
						else
						{
							st.wHour = (st.wHour %= 12) ? st.wHour : 12;
							if (!fSuppress || (st.wHour / 10 != 0))
								OffsetPoint(hdc, 0, 0, st.wHour / 10);
							OffsetPoint(hdc, 1, 0, st.wHour % 10);
						}
					}
					else
						OffsetPoint(hdc, 1, 0, st.wHour % 10);
				}
				if (prev.wMinute != st.wMinute)
				{
					if (st.wMinute % 10 == 0)
					{
						OffsetPoint(hdc, 2, 1, st.wMinute / 10);
					}
					OffsetPoint(hdc, 3, 1, st.wMinute % 10);
				}
				if (st.wSecond % 10 == 0)
					OffsetPoint(hdc, 4, 2, st.wSecond / 10);
				OffsetPoint(hdc, 5, 2, st.wSecond % 10);
				prev = st;
				ReleaseDC(hwnd, hdc);
			}
				break;
			case 2:
				Shell_NotifyIcon(NIM_MODIFY, &nid);
				//hdc = GetDC(hwnd);
				//TextOut(hdc, 0, 0, temp.c_str(), temp.size());
				//StringCchPrintf(szBuffer, 128, TEXT("size=%d"), allTitle.size());
				//MessageBox(nullptr, szBuffer, TEXT(""), 0);
				//temp =TEXT( "");
				//allTitle.clear();
				//EnumWindows(EnumWindowProc, 0);
				//ReleaseDC(hwnd, hdc);
				break;
		}
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		if (IsIconic(hwnd))
			//MessageBox(nullptr, TEXT("mini"), TEXT(""), MB_OK);
			ShowWindow(hwnd, SW_HIDE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SetIsotropirc(hdc);
		DisplayTime(hdc, f24hour, fSuppress);
		EndPaint(hwnd, &ps);
		break;
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
	HWND hwnd = CreateWindow( TEXT("zeng"), TEXT("dlgclock2"), WS_OVERLAPPEDWINDOW,
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