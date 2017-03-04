#include<Windows.h>
#include<cmath>
const double TWOPI = 2*3.1415926;
double fRadius;
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int  wParam, long lParam)
{
	static int cxClient, cyClient;
	PAINTSTRUCT ps;
	HDC hdc;
	static HRGN hRgnClip;
	HRGN hRgnTemp[6];
	HBRUSH hBrush,oldBrush;
	switch (UMsg)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		hRgnTemp[0] = CreateEllipticRgn(0, cyClient / 3, cxClient / 2, 2 * cyClient / 3);
		hRgnTemp[1] = CreateEllipticRgn(cxClient / 2, cyClient / 3, cxClient, 2 * cyClient / 3);
		hRgnTemp[2] = CreateEllipticRgn(cxClient / 3, 0, 2 * cxClient / 3, cyClient / 2);
		hRgnTemp[3] = CreateEllipticRgn(cxClient / 3, cyClient / 2, 2 * cxClient / 3, cyClient);
		hRgnTemp[4] = CreateEllipticRgn(0, 0, 1, 1);
		hRgnTemp[5] = CreateEllipticRgn(0, 0, 1, 1);
		hRgnClip = CreateEllipticRgn(0, 0, 1, 1);
		CombineRgn(hRgnTemp[4], hRgnTemp[0], hRgnTemp[1], RGN_OR);
		CombineRgn(hRgnTemp[5], hRgnTemp[2], hRgnTemp[3], RGN_OR);
		CombineRgn(hRgnClip, hRgnTemp[4], hRgnTemp[5], RGN_XOR);
		for (int i = 0; i < 5; i++)
			DeleteObject(hRgnTemp[i]);
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		hBrush = CreateSolidBrush(RGB(112, 43, 92));
		FillRgn(hdc, hRgnClip, hBrush);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBrush = CreateSolidBrush(RGB(112, 43, 92));
		oldBrush =(HBRUSH) SelectObject(hdc, hBrush);
		FillRgn(hdc, hRgnClip, hBrush);
		//FrameRgn(hdc, hRgnClip, hBrush,1,1);
		//PaintRgn(hdc, hRgnClip);
		InvertRgn(hdc, hRgnClip);
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, nullptr);
		fRadius = hypot(cxClient, cyClient);
		SelectClipRgn(hdc, hRgnClip);
		for (double fAngle = 0.0; fAngle < TWOPI; fAngle += TWOPI / 360)
		{
			MoveToEx(hdc, 0, 0, nullptr);
			LineTo(hdc, fRadius*cos(fAngle), fRadius*sin(fAngle));
		}
		DeleteObject(SelectObject(hdc, oldBrush));
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