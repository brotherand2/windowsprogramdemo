//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<vector>
#include<strsafe.h>
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

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar,xCaret,yCaret,xBuffer,yBuffer;
	static vector<tstring>buffer;
	MSG msg;
	static long count=0,kill=0;
	TCHAR temp[128];
	int oldSize = 0;
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
		xBuffer = cxClient / cxChar;
		yBuffer = cyClient / cyChar;
		buffer.resize(yBuffer);
		for (int i = 0; i < buffer.size(); i++)
			buffer.at(i) =tstring(xBuffer,TEXT(' '));
		//xCaret = yCaret = 0;
		//if (hwnd = GetFocus())
		//SetCaretPos(xCaret*cxChar, yCaret*cyChar);
		break;
	case WM_SETFOCUS:
		//hdc = GetDC(hwnd);
		CreateCaret(hwnd, nullptr, cxChar, cyChar);
		SetCaretPos(xCaret*cxChar, yCaret*cyChar);
		ShowCaret(hwnd);

		count++;
		//TextOut(hdc, cxClient / 2, cyClient / 2, temp, lstrlen(temp));
		InvalidateRect(hwnd, nullptr, true);
		//MessageBox(nullptr, TEXT("I have the focus"), TEXT("look here"), MB_ICONEXCLAMATION);
		//ReleaseDC(hwnd, hdc);
		break;
	case WM_KILLFOCUS:
		kill++;
		DestroyCaret();
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_KEYDOWN:
		msg.hwnd = hwnd;
		msg.lParam = lParam;
		msg.wParam = wParam;
		msg.message = UMsg;
		TranslateMessage(&msg);
		switch (wParam)
		{
		case VK_HOME:
			xCaret = 0;
			break;
		case VK_END:
			xCaret = xBuffer - 1;
			break;
		case VK_PRIOR:
			yCaret = 0;
			break;
		case VK_NEXT:
			yCaret = yBuffer - 1;
			break;
		case VK_LEFT:
			xCaret = max(0, xCaret - 1);
			break;
		case VK_RIGHT:
			xCaret = min(xBuffer - 1, xCaret + 1);
			break;
		case VK_UP:
			yCaret = max(0, yCaret - 1);
			break;
		case VK_DOWN:
			yCaret = min(yBuffer - 1, yCaret + 1);
			break;
		case VK_DELETE:
			hdc = GetDC(hwnd);
			HideCaret(hwnd);
			for (int x = xCaret; x < xBuffer-1; x++)
				buffer[yCaret][x] = buffer[yCaret][x + 1];
			buffer[yCaret][xBuffer - 1] = ' ';
			TextOut(hdc, xCaret*cxChar, yCaret*cyChar, &buffer[yCaret][xCaret], xBuffer - xCaret);
			ShowCaret(hwnd);
			ReleaseDC(hwnd, hdc);
			break;
		}
		SetCaretPos(xCaret*cxChar, yCaret*cyChar);
		//InvalidateRect(hwnd, nullptr, true);

		break;
	case WM_CHAR:
		switch (wParam)
		{
		case '\b':
			if (xCaret > 0)
			{
				xCaret--;
				SendMessage(hwnd, WM_KEYDOWN, VK_DELETE, 0);
			}
			break;
		case '\t':
			for (int i = 0; i < 8; i++)
				SendMessage(hwnd, WM_CHAR, ' ', 0);
			break;
		case '\r':
			xCaret = 0;
			if (++yCaret == yBuffer)
				yCaret = 0;
			break;
		case '\n':
			if (++yCaret == yBuffer)
				yCaret = 0;
			break;
		case '\x1B':
			for (int i = 0; i < buffer.size(); i++)
				for (int j = 0; j < xBuffer; j++)
					buffer[i][j] = ' ';
			xCaret = yCaret = 0;
			InvalidateRect(hwnd, nullptr, true);
			break;
		default:
			HideCaret(hwnd);
			hdc = GetDC(hwnd);
			buffer[yCaret][xCaret] = (TCHAR)wParam;
			TextOut(hdc, xCaret*cxChar, yCaret*cyChar, &buffer[yCaret][xCaret], 1);

			if (++xCaret == xBuffer)
			{
				xCaret = 0;
				if (++yCaret == yBuffer)
					yCaret = 0;
			}
			ReleaseDC(hwnd, hdc);
			break;
		}
		ShowCaret(hwnd);
		SetCaretPos(xCaret*cxChar, yCaret*cyChar);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetViewportExtEx(hdc, cxChar, cyChar,nullptr);
		SetWindowExtEx(hdc, 1, 1,nullptr);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		for (int i = 0; i < buffer.size(); i++)
			TextOut(hdc, 0, i, buffer[i].c_str(), buffer[i].size());
		////StringCchPrintf(temp, 128, TEXT("focus=%d"), count);

		////TextOut(hdc, cxClient / 2, cyClient / 2, temp, lstrlen(temp));
		////StringCchPrintf(temp, 128, TEXT("kill=%d"),kill);
		////TextOut(hdc, cxClient / 2, cyClient / 2 + 40, temp, lstrlen(temp));

		//StringCchPrintf(temp, 128, TEXT("x=%d,y=%d"),xCaret,yCaret);

		//TextOut(hdc, 11,11, temp, lstrlen(temp));


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
	wndclass.lpszClassName = TEXT("zengyonghua");
	wndclass.lpszMenuName = nullptr;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(TEXT("zengyonghua"), TEXT("keyScroll"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);
	HWND hwnd1 = CreateWindow(TEXT("zengyonghua"), TEXT("keyScroll2"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);

	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	ShowWindow(hwnd1, nCmdShow);
	UpdateWindow(hwnd1);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		DispatchMessage(&msg);
	}
}