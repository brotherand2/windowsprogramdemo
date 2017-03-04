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
void DrawRect(HWND hwnd, const POINT &beg, const POINT &end)
{
	HDC hdc = GetDC(hwnd);
	SetROP2(hdc, R2_NOT);
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, beg.x, beg.y, end.x, end.y);
	ReleaseDC(hwnd, hdc);

}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	static POINT beg, end, boxBeg, boxEnd;
	static bool fBlocking, fValidBox;
	TCHAR szBuffer[128];
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
	case WM_LBUTTONDOWN:
		SetCursor(LoadCursor(nullptr, IDC_CROSS));
		end.x = beg.x = LOWORD(lParam);
		end.y = beg.y = HIWORD(lParam);
		fBlocking = true;
		SetCapture(hwnd);
		break;
	case WM_RBUTTONDOWN:
		hdc = GetDC(hwnd);

		end.x = LOWORD(lParam);
		end.y = HIWORD(lParam);
		DrawRect(hwnd, beg, end);
		ReleaseDC(hwnd, hdc);

		break;
	case WM_CHAR:
		if (fBlocking&&wParam == '\x1B')
		{
			DrawRect(hwnd, beg, end);
			SetCursor(LoadCursor(nullptr, IDC_ARROW));
			fValidBox = true;
			fBlocking = false;
		}
		//MessageBox(hwnd, TEXT("char"), TEXT(""), MB_OK);
		break;

	case WM_LBUTTONUP:
		if (fBlocking)
		{
			SetCursor(LoadCursor(nullptr, IDC_ARROW));
			boxBeg = beg;
			boxEnd = end;
			ReleaseCapture();

			fBlocking = false;
			fValidBox = true;
			InvalidateRect(hwnd, nullptr, true);
		}
		break;
	case WM_MOUSEMOVE:
		if (fBlocking)
		{
			DrawRect(hwnd, beg, end);
			end.x = (short)LOWORD(lParam);
			end.y = (short)HIWORD(lParam);
			DrawRect(hwnd, beg, end);
			//sprintf_s(szBuffer, TEXT("(%d,%d)"), end.x,end.y);
			//hdc = GetDC(hwnd);
			//TextOut(hdc, 400, 400, szBuffer, lstrlen(szBuffer));
			//ReleaseDC(hwnd, hdc);
		}
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		//if (fBlocking)
		//Rectangle(hdc, beg.x, beg.y, end.x, end.y);
		if (fValidBox)
			Rectangle(hdc, boxBeg.x, boxBeg.y, boxEnd.x, boxEnd.y);
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
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}