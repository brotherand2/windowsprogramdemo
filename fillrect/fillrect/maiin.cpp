#include<Windows.h>
#include<ctime>
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int  wParam, long lParam)
{
	static int cxClient, cyClient;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	HBRUSH hBrush;
	switch (UMsg)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		srand(time(0));
		SetRect(&rect, rand() % 100, rand() % 100, 100+rand() % 100, 100+rand() % 100);
		hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		FillRect(hdc, &rect, hBrush);
		SetRect(&rect, 200+rand() % 100, rand() % 100, 300 + rand() % 100, 100 + rand() % 100);
		hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		FrameRect(hdc, &rect, hBrush);
	
		for (int i = 0; i < 5; i++)
		{
			OffsetRect(&rect, 50, 50);
			FrameRect(hdc, &rect, hBrush);
		}
		hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		for (int i = 1; i <= 5; i++)
		{
			InflateRect(&rect, i * 10, i * 10);
			FrameRect(hdc, &rect, hBrush);
		}
		SetRect(&rect, 200 + rand() % 100,200+ rand() % 100, 300 + rand() % 100, 300 + rand() % 100);
		InvertRect(hdc,&rect);
		DeleteObject(hBrush);
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