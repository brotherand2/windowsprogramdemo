#include<Windows.h>
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int  wParam, long lParam)
{
	static int cxClient, cyClient;
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH oldBrush, hBrush;
	switch (UMsg)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBrush = CreateHatchBrush(HS_HORIZONTAL, RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		SetBkColor(hdc, RGB(0, 149, 155));
		Rectangle(hdc, cxClient / 7, cyClient / 5, 2 * cxClient / 7, 2 * cyClient / 5);
		hBrush = CreateHatchBrush(HS_VERTICAL, RGB(0, 255, 0));
		SelectObject(hdc, hBrush);
		Rectangle(hdc, 3 * cxClient / 7, cyClient / 5, 4 * cxClient / 7, 2 * cyClient / 5);
		hBrush = CreateHatchBrush(HS_FDIAGONAL, RGB(0, 0, 255));
		SelectObject(hdc, hBrush);
		Rectangle(hdc, 5 * cxClient / 7, cyClient / 5, 6 * cxClient / 7, 2 * cyClient / 5);
		hBrush = CreateHatchBrush(HS_BDIAGONAL,RGB(255,255,0));
		SelectObject(hdc, hBrush);
		Rectangle(hdc, cxClient / 7, 3 * cyClient / 5, 2 * cxClient / 7, 4 * cyClient / 5);
		hBrush = CreateHatchBrush(HS_CROSS, RGB(255, 0, 255));
		SelectObject(hdc, hBrush);
		Rectangle(hdc, 3 * cxClient / 7, 3 * cyClient / 5, 4 * cxClient / 7, 4 * cyClient / 5);
		hBrush = CreateHatchBrush(HS_DIAGCROSS, RGB(0, 255, 255));
		SelectObject(hdc, hBrush);
		Rectangle(hdc, 5 * cxClient / 7, 3 * cyClient / 5, 6 * cxClient/7, 4 * cyClient / 5);
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