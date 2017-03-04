#include<Windows.h>
#include<ctime>
static int cxClient, cyClient;

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int  wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (UMsg)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

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
void DrawRect(HWND hwnd)
{
	HDC hdc;
	HBRUSH hBrush;
	RECT rect;
	hdc = GetDC(hwnd);
	if (cxClient == 0 || 0 == cyClient)
		return;
	SetRect(&rect, rand() % cxClient, rand() % cyClient, rand() % cxClient, rand() % cyClient);
	hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
	FillRect(hdc, &rect, hBrush);
	ReleaseDC(hwnd, hdc);
	DeleteObject(hBrush);
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
	srand(time(0));
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, true))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			DrawRect(hwnd);
	}
}