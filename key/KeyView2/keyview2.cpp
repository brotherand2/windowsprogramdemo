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
	static int cxClient, cyClient, col;
	static int cxChar, cxCaps, cyChar;
	static TCHAR szTop[] = TEXT("Message         Key      Char         Repeat Scan Ext ALT Prev Tran");
	static TCHAR szUnd[] = TEXT("_______         ___      ____         ______ ____ ___ ___ ____ ____");
	static TCHAR *szFormat[2] = {
		TEXT("%-16s%3d %-17s%c%6d%5d%4s%4s%5s%5s"),
		TEXT("%-16s         0X%04X%s%c %10d%5d%4s%4s%5s%5s")
	};
	static TCHAR *szMessage[] = {
		TEXT("WM_KEYDOWN"),
		TEXT("WM_KEYUP"),
		TEXT("WM_CHAR"),
		TEXT("DEADCHAR"),
		TEXT("WM_SYSKEYDOWN"),
		TEXT("WM_SYSKEYUP"),
		TEXT("WM_SYSCHAR"),
		TEXT("WM_SYSDEADCHAR")
	};
	static MSG msg;
	static vector<MSG>vec;
	static TCHAR  szYes[] = TEXT("yes");
	static TCHAR  szNo[] = TEXT("no");
	static TCHAR  szUp[] = TEXT("up");
	static TCHAR  szDown[] = TEXT("down");
	int iType;
	static unsigned long dwCharSet;
	TCHAR szBuffer[128], szKeyName[128];
	switch (UMsg)
	{
	case WM_INPUTLANGCHANGE:
		dwCharSet = wParam;
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:
		msg.hwnd = hwnd;
		msg.lParam = lParam;
		msg.wParam = wParam;
		msg.message = UMsg;
		vec.push_back(msg);
		TranslateMessage(&msg);
		InvalidateRect(hwnd, nullptr, true);
		if (UMsg == WM_SYSKEYDOWN)
		{
			if (wParam == VK_F4)
				return 0;
			if (wParam == VK_F5)
				wParam = VK_F4;
		}
		if (UMsg == WM_KEYDOWN)
		{
			if (GetKeyState(VK_CONTROL) < 0 && GetKeyState(VK_MENU) < 0)
				MessageBox(hwnd, TEXT("YOU enter ctrl and alt"), TEXT("keydown"), MB_ICONHAND);
		}
		break;
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
		col = cyClient / cyChar;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SaveDC(hdc);
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, 1, 1, nullptr);
		SetViewportExtEx(hdc, cxChar, cyChar, nullptr);
		SetBkMode(hdc, TRANSPARENT);
		//SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, nullptr));
		TextOut(hdc, 0, 0, szTop, lstrlen(szTop));
		TextOut(hdc, 0, 0, szUnd, lstrlen(szUnd));
		if (vec.size() < col)
		{
			for (int i = 0; i < vec.size(); i++)
			{
				iType = vec[i].message == WM_CHAR ||
					vec[i].message == WM_DEADCHAR ||
					vec[i].message == WM_SYSCHAR ||
					vec[i].message == WM_SYSDEADCHAR;
				GetKeyNameText(vec[i].lParam, szKeyName, sizeof(szKeyName) / sizeof(TCHAR));
				StringCchPrintf(szBuffer, 128,
					szFormat[iType],
					szMessage[vec[i].message - WM_KEYFIRST],
					vec[i].wParam,
					iType ? TEXT(" ") : szKeyName,
					(TCHAR)(iType ? vec[i].wParam : ' '),
					LOWORD(vec[i].lParam),
					HIWORD(vec[i].lParam) & 0xff,
					(0X01000000 & vec[i].lParam) ? szYes : szNo,
					(0X20000000 & vec[i].lParam) ? szYes : szNo,
					(0X40000000 & vec[i].lParam) ? szDown : szUp,
					(0X80000000 & vec[i].lParam) ? szUp : szDown
					);
				TextOut(hdc, 0, i + 1, szBuffer, lstrlen(szBuffer));

			}
		}
		else
		{
			for (int i = vec.size() - col + 1, j = 0; i < vec.size(); i++, j++)
			{
				iType = vec[i].message == WM_CHAR ||
					vec[i].message == WM_DEADCHAR ||
					vec[i].message == WM_SYSCHAR ||
					vec[i].message == WM_SYSDEADCHAR;
				GetKeyNameText(vec[i].lParam, szKeyName, sizeof(szKeyName) / sizeof(TCHAR));
				StringCchPrintf(szBuffer, 128,
					szFormat[iType],
					szMessage[vec[i].message - WM_KEYFIRST],
					vec[i].wParam,
					iType ? TEXT(" ") : szKeyName,
					(TCHAR)(iType ? vec[i].wParam : ' '),
					LOWORD(vec[i].lParam),
					HIWORD(vec[i].lParam) & 0xff,
					(0X01000000 & vec[i].lParam) ? szYes : szNo,
					(0X20000000 & vec[i].lParam) ? szYes : szNo,
					(0X40000000 & vec[i].lParam) ? szDown : szUp,
					(0X80000000 & vec[i].lParam) ? szUp : szDown
					);
				TextOut(hdc, 0, j + 1, szBuffer, lstrlen(szBuffer));
			}
		}
		RestoreDC(hdc, -1);
		DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
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