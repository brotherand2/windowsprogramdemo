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
int const DIVISIONS = 5;
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	static int cxBlock, cyBlock;
	static bool fState[DIVISIONS][DIVISIONS];
	int x, y;
	POINT point;
	switch (UMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		cxBlock = cxClient / DIVISIONS;
		cyBlock = cyClient / DIVISIONS;
		break;
	case WM_KEYDOWN:
		GetCursorPos(&point);
		ScreenToClient(hwnd, &point);
		x = point.x / cxBlock;
		y = point.y / cyBlock;
		x = max(0, min(DIVISIONS - 1, x));
		y = max(0, min(DIVISIONS - 1, y));
		switch (wParam)
		{
		case VK_LEFT:
			x--;
			break;
		case VK_RIGHT:
			x++;
			break;
		case VK_UP:
			y--;
			break;
		case VK_DOWN:
			y++;
			break;
		case VK_HOME:
			x = y = 0;
			break;
		case VK_END:
			x = y = DIVISIONS - 1;
			break;
		case VK_SPACE:
		case VK_RETURN:
			SendMessage(hwnd, WM_LBUTTONDOWN, VK_LBUTTON,y*cyBlock<<16|x*cxBlock);
			break;
		}
		x = (x + DIVISIONS) % DIVISIONS;
		y = (y + DIVISIONS) % DIVISIONS;
		point.x = x*cxBlock + cxBlock / 2;
		point.y = y*cyBlock + cyBlock / 2;
		ClientToScreen(hwnd, &point);
		SetCursorPos(point.x, point.y);
		break;
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam) / cxBlock;
		y = HIWORD(lParam) / cyBlock;
		if (x < DIVISIONS&&y < DIVISIONS)
		{
			fState[x][y] ^= 1;
		}
		else
			MessageBeep(0);
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		for (int i = 0; i < DIVISIONS; i++)
		{
			for (int j = 0; j < DIVISIONS; j++)
			{
				Rectangle(hdc, cxBlock*i, cyBlock*j, cxBlock*(i + 1), cyBlock*(j + 1));
				if (fState[i][j])
				{
					MoveToEx(hdc, cxBlock*i, cyBlock*j, nullptr);
					LineTo(hdc, cxBlock*(i + 1), cyBlock*(j + 1));
					MoveToEx(hdc, cxBlock*i, cyBlock*(j + 1), nullptr);
					LineTo(hdc, cxBlock*(i + 1), cyBlock*j);
				}
			}
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