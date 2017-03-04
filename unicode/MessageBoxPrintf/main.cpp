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
	TEXTMETRIC tm;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	HPEN hPen, oldPen;
	static HBRUSH hBlueBrush, hRedBrush, hYellowBrush, oldBrush;
	static HPEN hWhitePen,hBluePen;
	static POINT apt[2][4] =
	{
		{-90,-10,-90,-60,-110,-70,-130,-50},
		{ 90, -10, 90, -60, 110, -70, 130, -50 }
	};
	switch (UMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		hBlueBrush = CreateSolidBrush(RGB(0, 159, 232));
		hRedBrush = CreateSolidBrush(RGB(255,0,0));
		hYellowBrush = CreateSolidBrush(RGB(255, 255, 0));
		hWhitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		hBluePen = CreatePen(PS_SOLID, 2, RGB(0, 159, 232));
		ReleaseDC(hwnd, hdc);
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hPen = CreatePen(PS_DOT, 0, RGB(192, 192, 192));
		oldPen =(HPEN) SelectObject(hdc, hPen);
		SetMapMode(hdc, MM_ISOTROPIC);
		SetWindowExtEx(hdc, 400,400, nullptr);
		SetViewportExtEx(hdc, cxClient, -cyClient, nullptr);
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, nullptr);

		MoveToEx(hdc, -cxClient, 0, nullptr);
		LineTo(hdc, cxClient, 0);//∏®÷˙œﬂ
		MoveToEx(hdc, 0, cyClient, nullptr);
		LineTo(hdc, 0, -cyClient);
		SelectObject(hdc, oldPen);

		oldBrush = (HBRUSH)SelectObject(hdc, hBlueBrush);
		Ellipse(hdc, -120, 200, 120, -40);//Õ∑

		SelectObject(hdc, oldBrush);
		Ellipse(hdc, -100, 160, 100, -40);//¡≥
		Ellipse(hdc, -50, 180, 0, 120);//—€æ¶
		Ellipse(hdc, 50, 180, 0, 120);
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		Ellipse(hdc, -20, 160, -5, 140);//—€÷ÈÕ‚∫⁄
		Ellipse(hdc, 20, 160, 5, 140);
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		Ellipse(hdc, -15, 155, -10, 145);//—€÷È¿Ô∞◊
		Ellipse(hdc, 15, 155, 10, 145);
		oldBrush=(HBRUSH)SelectObject(hdc, hRedBrush);
		Ellipse(hdc, -10, 135, 10, 115);//±«◊”
		SelectObject(hdc,oldBrush);
		MoveToEx(hdc, 0, 115, nullptr);//±«œﬂ
		LineTo(hdc, 0, 30);


		Arc(hdc, -70, 120, 70, 30, -60, 50, 60, 50);//◊Ï∞Õ

		MoveToEx(hdc, -70, 115, nullptr);//◊Û∫˙–Î
		LineTo(hdc, -20, 100);
		MoveToEx(hdc, -80, 85, nullptr);
		LineTo(hdc, -20, 85);
		MoveToEx(hdc, -70, 55, nullptr);
		LineTo(hdc, -20, 70);

		MoveToEx(hdc, 70, 115, nullptr);//”“∫˙–Î
		LineTo(hdc, 20, 100);
		MoveToEx(hdc, 80, 85, nullptr);
		LineTo(hdc, 20, 85);
		MoveToEx(hdc, 70, 55, nullptr);
		LineTo(hdc, 20, 70);

		SelectObject(hdc, hBlueBrush);
		Rectangle(hdc, -90, 0, 90, -150);//ª≠…ÌÃÂ

		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		Ellipse(hdc, -70, 20, 70, -120);//ª≠∂«◊”

		oldPen=(HPEN)SelectObject(hdc, hWhitePen);
		Arc(hdc, -70, 20, 70, -120, 70, 10, -70, 10);//œ˚≥˝∂‡”‡µƒª°œﬂ

		SelectObject(hdc, oldPen);
		oldBrush = (HBRUSH)SelectObject(hdc, hRedBrush);
		RoundRect(hdc, -95, 5, 95, -10,20,20);//œÓ»¶
		SelectObject(hdc, oldBrush);

		oldBrush = (HBRUSH)SelectObject(hdc, hYellowBrush);
		Ellipse(hdc,-15,0,15,-30);//¡ÂÓı
		RoundRect(hdc, -15, -10, 15, -15,2,2);//◊∞ Œ1
		SelectObject(hdc, oldBrush);

		oldBrush = (HBRUSH)SelectObject(hdc, hRedBrush);
		Ellipse(hdc,-4,-18,4,-26);//◊∞ Œ2
		MoveToEx(hdc, 0, -26, nullptr);
		LineTo(hdc, 0,-30);
		SelectObject(hdc, oldBrush);


		Pie(hdc, -50, 0, 50, -100, -50, -50, 50, -50);//∞Ÿ±¶¥¸

		Pie(hdc, -20, -130, 20, -170, 20, -150, -20, -150);//Ω≈

		oldPen = (HPEN)SelectObject(hdc, hWhitePen);
		MoveToEx(hdc, 20, -150, nullptr);//–ﬁ Œ
		LineTo(hdc, -20, -150);
		SelectObject(hdc, oldPen);
      
		Ellipse(hdc, -110, -130, -10, -170);//Ω≈’∆
		Ellipse(hdc, 110, -130, 10, -170);//Ω≈’∆

		oldBrush = (HBRUSH)SelectObject(hdc, hBlueBrush);// ÷
		Polygon(hdc, apt[0], 4);
		Polygon(hdc, apt[1], 4);

		SelectObject(hdc, oldBrush);

		Ellipse(hdc, -150, -40, -100, -80);// ÷’∆
		Ellipse(hdc, 150, -40, 100, -80);// ÷’∆

		oldPen = (HPEN)SelectObject(hdc, hBluePen);
		MoveToEx(hdc, -90,-10, nullptr);//–ﬁ Œ ÷
		LineTo(hdc, -90, -50);
		MoveToEx(hdc, 90, -10, nullptr);//–ﬁ Œ ÷
		LineTo(hdc, 90, -50);

		SelectObject(hdc, oldPen);


		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		DeleteObject(hRedBrush);
		DeleteObject(hBlueBrush);
		DeleteObject(hYellowBrush);
		DeleteObject(hWhitePen);
		DeleteObject(hBluePen);
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
		DispatchMessage(&msg);
	}
}