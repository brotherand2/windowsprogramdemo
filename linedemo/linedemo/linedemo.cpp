#include<Windows.h>
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
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
		SelectObject(hdc, hBrush);
		//SelectObject(hdc, GetStockObject(NULL_PEN));
		//SelectObject(hdc, GetStockObject(NULL_BRUSH));
		GetClientRect(hwnd, &rect);
		cxClient = rect.right;
		cyClient = rect.bottom;
		Rectangle(hdc,cxClient/ 8,cyClient/ 8 , 7* cxClient/ 8, 7*cyClient / 8 );
		MoveToEx(hdc, 0, 0, nullptr);
		LineTo(hdc, cxClient, cyClient);
		MoveToEx(hdc, 0, cyClient,nullptr);
		LineTo(hdc, cxClient, 0);
		Ellipse(hdc, cxClient / 8, cyClient / 8, 7 * cxClient / 8,7*cyClient / 8);
		RoundRect(hdc,1*cxClient/4,1*cyClient/4,3*cxClient/4,3*cyClient/4,cxClient/4,1*cxClient/4);
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
	HWND hwnd = CreateWindow("zeng", "«˙œﬂ≤‚ ‘", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
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