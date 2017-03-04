#include<Windows.h>
COLORREF  color = RGB(223, 115, 60);
void DrawBezier(HDC hdc, POINT apt[])
{
	PolyBezier(hdc, apt, 7);
	SetTextColor(hdc, RGB(255, 0, 0));
	MoveToEx(hdc, apt[0].x, apt[0].y, nullptr);
	LineTo(hdc, apt[1].x, apt[1].y);
	MoveToEx(hdc, apt[2].x, apt[2].y, nullptr);
	LineTo(hdc, apt[3].x, apt[3].y);
	MoveToEx(hdc, apt[3].x, apt[3].y,nullptr);
	LineTo(hdc, apt[4].x, apt[4].y);
	MoveToEx(hdc, apt[5].x, apt[5].y, nullptr);
	LineTo(hdc, apt[6].x, apt[6].y);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int  wParam, long lParam)
{
	static int cxClient, cyClient;
	static POINT apt[7];
	PAINTSTRUCT ps;
	HDC hdc;
	switch (UMsg)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
		if (wParam&MK_LBUTTON || wParam&MK_RBUTTON)
		{
			hdc = GetDC(hwnd);
			//SelectObject(hdc, GetStockObject(WHITE_PEN));
			HPEN hPen = CreatePen(PS_DOT, 1, RGB(255,255,255));
			HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
			DrawBezier(hdc, apt);//消去上一次画的曲线
			if (wParam&MK_LBUTTON)
			{
				apt[1].x = LOWORD(lParam);
				apt[1].y = HIWORD(lParam);
			}
			if (wParam&MK_RBUTTON)
			{
				apt[2].x = LOWORD(lParam);
				apt[2].y = HIWORD(lParam);
			}
			//SelectObject(hdc, GetStockObject(BLACK_PEN));//恢复画笔颜色
			hPen = CreatePen(PS_DOT, 1, color);
			SelectObject(hdc, hPen);
			DrawBezier(hdc, apt);
			ReleaseDC(hwnd,hdc);
			DeleteObject(hPen);
		}
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		apt[0].x = cxClient / 4;
		apt[0].y = cyClient / 2;
		apt[1].x = cxClient / 2;
		apt[1].y = cyClient/ 4;
		apt[2].x = cxClient / 2;
		apt[2].y = cyClient * 3 / 4;
		apt[3].x = cxClient * 3 / 4;
		apt[3].y = cyClient / 2;
		apt[4].x = cxClient * 7 / 8;
		apt[4].y = cyClient / 4;
		apt[5].x = cxClient * 7 / 8;
		apt[5].y = cyClient * 3 / 4;
		apt[6].x = cxClient;
		apt[6].y = cyClient / 2;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		{
			HPEN hPen = CreatePen(PS_DOT, 1, color);
			HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
			DrawBezier(hdc, apt);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
		}
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
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *szCmdCommand, int nCmdShow)
{
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
	HWND hwnd = CreateWindow("zeng", "bezier曲线", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
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