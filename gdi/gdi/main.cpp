#include<Windows.h>
#include<vector>
using namespace std;
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int  wParam, long lParam)
{
	static int cxClient, cyClient;
	HDC hdc;
	RECT rect;
	POINT apt[5] = { 100, 100, 200, 100, 200, 200, 100, 200, 100, 100 };
	POINT apt1[15] = { 700, 100, 740, 100, 730, 125, 740, 100, 750, 50, 760, 100,
		760, 100, 770, 125, 800, 100, 770, 125, 780, 200, 750, 150, 720, 200, 730, 125, 750, 150 };
	POINT apt2[5] = { 700, 300,900, 300,750,400,800,250,850,400 };
	int aiCounts[5] = { 3, 3, 3, 3, 3 };
	vector<POINT>point;
	POINT nPoint,pp[500];
	static HPEN hPen,oldPen;
	static LOGPEN logpen;
	HPEN pen1;
	LOGPEN logpen1;
	switch (UMsg)
	{
	case WM_CREATE:
		logpen.lopnColor = RGB(255, 0, 0);
		logpen.lopnStyle = PS_SOLID;
		logpen.lopnWidth.x = 4;
		hPen = CreatePenIndirect(&logpen);
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		pen1 = (HPEN)CreatePen(PS_DASH, 1, RGB(211, 11, 11));
		oldPen=(HPEN)SelectObject(hdc, pen1);
		SetBkColor(hdc, RGB(0, 122, 0));
		//SetBkMode(hdc, TRANSPARENT);
		//SetROP2(hdc, R2_NOTCOPYPEN);
		//SetROP2(hdc, R2_BLACK);
		//SetROP2(hdc, R2_NOT);

		for (int i = 100; i < 500; i+=10)
			for (int j = 100; j < 500; j += 10)
			{
			nPoint.x = i;
			nPoint.y = j;
			point.push_back(nPoint);
			}
		for (auto p:point)
			SetPixel(hdc, p.x, p.y, RGB(255, 0, 0));
		GetClientRect(hwnd, &rect);
		for (int x = 0; x < rect.right; x +=100)
		{
			MoveToEx(hdc, x, 0, nullptr);
			LineTo(hdc, x, rect.bottom);
		}
		for (int y = 0; y < rect.bottom; y += 100)
		{
			MoveToEx(hdc, 0, y, nullptr);
			LineTo(hdc, rect.right, y);
		}
		ReleaseDC(hwnd, hdc);
		SelectObject(hdc, oldPen);
		DeleteObject(pen1);
		break;
	case  WM_RBUTTONDOWN:
		hdc = GetDC(hwnd);
		//MoveToEx(hdc, apt[0].x, apt[0].y, nullptr);
		//for (int i = 1; i < 5; i++)
		//	LineTo(hdc, apt[i].x, apt[i].y);
		oldPen=(HPEN)SelectObject(hdc, hPen);
		Polyline(hdc, apt, 5);
		Rectangle(hdc, 200, 300, 400, 400);
		Ellipse(hdc, 200, 300, 400, 400);
		Rectangle(hdc, 400, 200, 600, 300);
		RoundRect(hdc, 400, 200, 600, 300, 200/4, 200/4);
		Rectangle(hdc, 400, 50, 600, 150);
		Arc(hdc, 400, 50, 600, 150, 500, 50, 400, 100);
		Chord(hdc, 400, 50, 600, 150, 600, 130, 530, 50);
		Pie(hdc, 400, 50, 600, 150,  400, 120,580, 150);
		//Polygon(hdc, apt1, 5);
		PolyPolygon(hdc, apt1, aiCounts, 5);
		SetPolyFillMode(hdc,WINDING);

		Polygon(hdc, apt2, 5);
		ReleaseDC(hwnd, hdc);
		SelectObject(hdc, oldPen);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		DeleteObject(hPen);
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);
}
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *szCmdCommand, int nCmdShow)
{
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wndclass.hCursor = LoadCursor(nullptr, IDC_IBEAM);
	wndclass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = "zeng";
	wndclass.lpszMenuName = nullptr;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(nullptr, "register failed", "error", MB_APPLMODAL);
		return 0;
	}
	HWND hwnd = CreateWindow("zeng", "ÕýÏÒÇúÏß", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
	if (!hwnd)
	{
		MessageBox(nullptr, "create failed", "error", MB_APPLMODAL);
		return 0;
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