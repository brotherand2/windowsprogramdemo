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
WNDPROC oldProc[3];
static int idFocus;
long _stdcall ScrollProc(HWND hwnd, unsigned int message, unsigned int wParam, long lParam)
{
	int id = GetWindowLong(hwnd, GWL_ID);
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_TAB)
			SetFocus(GetDlgItem(GetParent(hwnd), (GetKeyState(VK_SHIFT) < 0) ? (id + 2) % 3 : (id + 1) % 3));
		break;
	case WM_SETFOCUS:
		idFocus = id;
		break;
	}
	return CallWindowProc(oldProc[id], hwnd, message, wParam, lParam);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	int wmID, wmEvent;
	static HWND hScroll[3], hLable[3], hValue[3], hRect;
	static RECT rect;
	COLORREF colors[3] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255) };
	static HBRUSH hBrush[3],hBrushStatic;
	HINSTANCE hInst;
	TCHAR *colorText[3] = { TEXT("Red"), TEXT("Green"), TEXT("Blue") };
	static int color[3];
	TCHAR szBuffer[128];
	switch (UMsg)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		hRect = CreateWindow(TEXT("static"), TEXT(""), WS_CHILD | WS_VISIBLE|SS_WHITERECT,
			0, 0, 0, 0, hwnd, (HMENU)9, hInst, nullptr);

		for (int i = 0; i < 3; i++)
		{
			hScroll[i] = CreateWindow(TEXT("scrollbar"), TEXT(""), WS_CHILD | WS_VISIBLE|WS_TABSTOP |SBS_VERT,
			0, 0, 0, 0, hwnd, (HMENU)i, hInst, nullptr);
			hLable[i] = CreateWindow(TEXT("static"), colorText[i], WS_CHILD | WS_VISIBLE|SS_CENTER,
				0, 0, 0, 0, hwnd, (HMENU)(3 + i), hInst, nullptr);
			hValue[i] = CreateWindow(TEXT("static"), TEXT("0"), WS_CHILD | WS_VISIBLE|SS_CENTER,
				0, 0, 0, 0, hwnd, (HMENU)(6 + i), hInst, nullptr);
			hBrush[i] = CreateSolidBrush(colors[i]);
			SetScrollRange(hScroll[i], SB_CTL, 0, 255, true);
			SetScrollPos(hScroll[i], SB_CTL, 0, true);
			oldProc[i] = (WNDPROC)SetWindowLong(hScroll[i], GWL_WNDPROC,(long)ScrollProc);
		}
		hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
		break;
	case WM_SYSCOLORCHANGE:
		DeleteObject(hBrushStatic);
		hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		for (int i = 0; i < 3; i++)
		{
			MoveWindow(hScroll[i], (1+2*i)*cxClient / 14, 2 * cyChar, cxClient / 14, cyClient - 4 * cyChar, true);
			MoveWindow(hLable[i], (1 + 2 * i)*cxClient / 14, cyChar, cxClient / 14, cyChar, true);
			MoveWindow(hValue[i], (1 + 2 * i)*cxClient / 14, cyClient - 2 * cyChar, cxClient / 14, cyChar, true);
		}
		MoveWindow(hRect, 0, 0, cxClient / 2, cyClient, true);
		rect.left = cxClient / 2;
		rect.right = cxClient;
		rect.top = 0;
		rect.bottom = cyClient;
		SetFocus(hwnd);
		break;
	case WM_VSCROLL:
		wmID = GetWindowLong((HWND)lParam, GWL_ID);
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			color[wmID] = max(0, color[wmID] - 1);
			break;
		case SB_LINEDOWN:
			color[wmID] = min(255, color[wmID] + 1);
			break;
		case SB_PAGEUP:
			color[wmID] = max(0, color[wmID] - 15);
			break;
		case SB_PAGEDOWN:
			color[wmID] = min(255, color[wmID] + 15);
			break;
		case SB_TOP:
			color[wmID] = 0;
			break;
		case SB_BOTTOM:
			color[wmID] = 255;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			color[wmID] = HIWORD(wParam);
			break;
		}
		SetScrollPos((HWND)lParam, SB_CTL, color[wmID], true);
		StringCchPrintf(szBuffer, 128, TEXT("%d"), color[wmID]);
		SetWindowText(hValue[wmID], szBuffer);
		DeleteObject((HBRUSH)SetClassLong(hwnd, GCL_HBRBACKGROUND,(long) CreateSolidBrush(RGB(color[0], color[1], color[2]))));
		InvalidateRect(hwnd, &rect, true);
		break;
	case WM_COMMAND:
		wmID = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		idFocus = wmID;
		switch (wmID)
		{
			
		}
		break;
	case WM_SETFOCUS:
		SetFocus(hScroll[idFocus]);
		break;
	case WM_CTLCOLORSTATIC:
		wmID = GetWindowLong((HWND)lParam, GWL_ID);
		if (wmID >= 3 && wmID <= 8)
		{
			SetTextColor((HDC)wParam, colors[wmID%3]);
			SetBkColor((HDC)wParam, GetSysColor(COLOR_BTNHIGHLIGHT));
			return (long)hBrushStatic;	
		}
		break;
	case WM_CTLCOLORSCROLLBAR:
		wmID = GetWindowLong((HWND)lParam, GWL_ID);
		return (long)hBrush[wmID];
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
		DeleteObject((HBRUSH)SetClassLong(hwnd, GCL_HBRBACKGROUND,(long) GetStockObject(WHITE_BRUSH)));
		for (int i = 0; i < 3; i++)
			DeleteObject(hBrush[i]);
		DeleteObject(hBrushStatic);
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = CreateSolidBrush(0);
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
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("ÓÂ»ª"), WS_OVERLAPPEDWINDOW,
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