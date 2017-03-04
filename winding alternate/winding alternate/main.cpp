#include<Windows.h>
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int  wParam, long lParam)
{
	static POINT apt[] = { 10, 70, 50, 70, 50, 10, 90, 10, 90, 50, 30, 50, 30, 90, 70, 90, 70, 30, 10, 30 };
	static int cxClient, cyClient;
	PAINTSTRUCT ps;
	HDC hdc;
	POINT temp[10];
	HBRUSH hBrush,oldBrush;
	switch (UMsg)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBrush = CreateSolidBrush(RGB(0, 255, 0));
		oldBrush =(HBRUSH) SelectObject(hdc, hBrush);
		SetPolyFillMode(hdc, ALTERNATE);
		for (int i = 0; i < 10; i++)
		{
			temp[i].x = apt[i].x*cxClient / 200;
			temp[i].y = apt[i].y*cyClient / 100;
		}
		Polygon(hdc, temp, 10);
		SetPolyFillMode(hdc, WINDING);
		//SelectObject(hdc, GetStockObject(NULL_PEN));
		//SelectObject(hdc, GetStockObject(NULL_BRUSH));
		for (int i = 0; i < 10; i++)
			temp[i].x += cxClient / 2;
		Polygon(hdc, temp, 10);
		DeleteObject(SelectObject(hdc, oldBrush));

		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);
}
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *szCmdCommand, int nCmdShow)
{
	HBRUSH brush = CreateSolidBrush(RGB(111, 25, 135));
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = brush;
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