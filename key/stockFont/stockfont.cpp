//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
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
static struct
{
	int idStockFont;
	TCHAR *szStockFont;
}
stockFont[]=
{
	OEM_FIXED_FONT, TEXT("OEM_FIXED_FONT"),
	ANSI_FIXED_FONT, TEXT("ANSI_FIXED_FONT"),
	ANSI_VAR_FONT, TEXT("ANSI_VAR_FONT"),
	SYSTEM_FONT, TEXT("SYSTEM_FONT"),
	SYSTEM_FIXED_FONT,TEXT("SYSTEM_FIXED_FONT"),
	DEFAULT_GUI_FONT, TEXT("DEFAULT_GUI_FONT"),
};
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar,xGrid,yGrid;
	static int cFonts,iVertPos;
	TCHAR szBuffer[128],szFaceName[128],szID[128];
	switch (UMsg)
	{
	case WM_DISPLAYCHANGE:
		InvalidateRect(hwnd, nullptr, true);
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		cFonts = sizeof(stockFont) / sizeof(stockFont[0]);
		SetScrollRange(hwnd, SB_VERT, 0, cFonts - 1, true);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_TOP:
			iVertPos = 0;
			break;
		case SB_BOTTOM:
			iVertPos = cFonts - 1;
			break;
		case SB_LINEUP:
		case SB_PAGEUP:
			iVertPos -=1;
			break;
		case SB_LINEDOWN:
		case SB_PAGEDOWN:
			iVertPos +=1;
			break;
		case SB_THUMBTRACK:
			iVertPos = HIWORD(wParam);
			break;
		}
		iVertPos = max(0, min(cFonts - 1, iVertPos));
		SetScrollPos(hwnd, SB_VERT, iVertPos, true);
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
		case VK_PRIOR:
			SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
			break;
		case VK_DOWN:
		case VK_NEXT:
			SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
			break;
		case VK_HOME:
			SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0);
			break;
		case VK_END:
			SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0);
			break;
		}
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, GetStockObject(stockFont[iVertPos].idStockFont));
		GetTextFace(hdc, 128, szFaceName);
		GetTextMetrics(hdc, &tm);
		StringCchPrintf(szBuffer, 128, TEXT("%s:Face Name =%s,CharSet=%d"), stockFont[iVertPos].szStockFont, szFaceName, tm.tmCharSet);
		TextOut(hdc, 0, 0, szBuffer, lstrlen(szBuffer));
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		xGrid = cxChar *4;
		yGrid = cyChar+3;
		for (int i = 0; i < 17; i++)
		{
			MoveToEx(hdc, (2 + i)*xGrid, 2 * yGrid, nullptr);//½ôÏß
			LineTo(hdc, (2 + i)*xGrid, 19 * yGrid);
			MoveToEx(hdc, 1 * xGrid, (3 + i)*yGrid, nullptr);//ºáÏß
			LineTo(hdc, 18 * xGrid, (3 + i)*yGrid);
		}
		for (int i = 0; i < 16; i++)
		{
			StringCchPrintf(szID, 128, TEXT("%X-"), i);
			TextOut(hdc, (2 + i)*xGrid + cxChar, 2 * yGrid + 2 , szID, lstrlen(szID));
			StringCchPrintf(szID, 128, TEXT("-%X"), i);
			TextOut(hdc, xGrid + cxChar, (3 + i)*yGrid + 2, szID, lstrlen(szID));
		}
		for (int y = 0; y < 16; y++)
		{
			for (int x = 0; x< 16; x++)
			{
				StringCchPrintf(szID, 128, TEXT("%c"), x * 16 + y);
				TextOut(hdc, (2 + x)*xGrid + cxChar, (3 + y)*yGrid + 2, szID, lstrlen(szID));
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