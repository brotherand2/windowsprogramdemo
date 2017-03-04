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
const double TWOPI = 2*3.1415926;
static int cxClient, cyClient;

void SetIsotropic(HDC hdc)
{
	SetMapMode(hdc, MM_ISOTROPIC);
	SetWindowExtEx(hdc, 1000, 1000, nullptr);
	SetViewportExtEx(hdc, cxClient / 2, -cyClient / 2, nullptr);
	SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, nullptr);
	//SetWindowExtEx(hdc, 1000, 1000, nullptr);
}
void RotatePoint(POINT pt[], int n, int angle)
{
	POINT temp;
	for (int i = 0; i < n; i++)
	{
		temp.x = pt[i].x*cos(TWOPI*angle/360) + pt[i].y*sin(TWOPI*angle/360);
		temp.y = pt[i].y*cos(TWOPI*angle/360) -pt[i].x*sin(TWOPI*angle/360);
		pt[i] = temp;
	}
}
void DrawClock(HDC hdc)
{
	static POINT pt[4];
	SetIsotropic(hdc);
	for (int i = 0; i < 360; i += 6)
	{
		pt[0].x = 0;
		pt[0].y = 900;
		RotatePoint(pt, 1, i);
		pt[2].x = pt[2].y = (i % 5)?33:100;
		pt[0].x -= pt[2].x/2;
		pt[0].y -= pt[2].y/2;
		pt[1].x = pt[0].x + pt[2].x;
		pt[1].y = pt[0].y + pt[2].y;
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		Ellipse(hdc, pt[0].x, pt[0].y, pt[1].x, pt[1].y);
	}
}
void DrawHand(HDC hdc, bool fchange,SYSTEMTIME &st)
{
	static POINT apt[3][5]=
	{
		{ -100, 0, 0, 600, 100, 0, 0, -150,-100,0 },//
		{ -50,0,0,800,50,0,0,-200,-50,0 },
		{0,0,0,0,0,0,0,800}
	};
	POINT temp[3][5];
	int Angle[3];
	Angle[0] = (st.wHour % 12) * 30 + st.wMinute / 2;
	Angle[1] = st.wMinute * 6;
	Angle[2] = st.wSecond * 6;
	
	memcpy(temp, apt, sizeof(apt));
	for (int i=fchange?0:2; i < 3; i++)
	{
		RotatePoint(temp[i], 5, Angle[i]);
		Polyline(hdc, temp[i],5);
	}
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int cxChar, cxCaps, cyChar;
	static SYSTEMTIME st, stPre;
	bool fchange;
	switch (UMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		SetTimer(hwnd, 1, 1000, nullptr);
		ReleaseDC(hwnd, hdc);
		GetLocalTime(&st);
		stPre = st;
		break;
	case WM_TIMER:
		hdc = GetDC(hwnd);
		GetLocalTime(&st);
		DrawClock(hdc);
		fchange = st.wHour != stPre.wHour || st.wMinute != stPre.wMinute;
		SelectObject(hdc,GetStockObject(WHITE_PEN));
		DrawHand(hdc,fchange,stPre);
		SelectObject(hdc, GetStockObject(BLACK_PEN));
		DrawHand(hdc, true,st);
		stPre = st;
		ReleaseDC(hwnd,hdc);
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawClock(hdc);
		DrawHand(hdc,true,st);
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hwnd, 1);
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