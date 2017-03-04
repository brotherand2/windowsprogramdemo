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
const int ID_PUSHBUTTON = 1;
const int ID_CHECKBOX = 2;
const int ID_RADIOBUTTON = 3;
const int ID_SETWINDOWTEXT = 4;
const int ID_GETWNDOWTEXT = 5;
const int ID_SETCHECK = 6;
const int ID_GETCHECK = 7;
const int ID_SETUNCHECK = 8;
const int ID_SETSHOWHIDE = 9;
const int ID_SETENABLE = 10;
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	int wmId, wmEvent;
	static HINSTANCE hInst;
	TCHAR isSelect[] = TEXT("check box is select");
	TCHAR isNotSelect[] = TEXT("check box is not select");
	TCHAR szBuffer[128];
	switch (UMsg)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		CreateWindow(TEXT("button"), TEXT("PUSHBUTTON"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			5 * cxChar, 10 * cyChar, 20 * cxChar, 2 * cyChar, hwnd, HMENU(ID_PUSHBUTTON),
			hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("CHECKBOX"), WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
			40 * cxChar, 10 * cyChar, 20 * cxChar, 2 * cyChar, hwnd, HMENU(ID_CHECKBOX),
			hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("RADIOBUTTON"), WS_CHILD | WS_VISIBLE |BS_RADIOBUTTON	,
			65 * cxChar, 10 * cyChar, 20 * cxChar, 2 * cyChar, hwnd, HMENU(ID_RADIOBUTTON),
			hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("SetCheck"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			5 * cxChar, 20 * cyChar, 20 * cxChar, 2 * cyChar, hwnd, HMENU(ID_SETCHECK),
			hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("SetWindowText"), WS_CHILD | WS_VISIBLE |BS_PUSHBUTTON,
			40 * cxChar, 20 * cyChar, 20 * cxChar, 2 * cyChar, hwnd, HMENU(ID_SETWINDOWTEXT),
			hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("SetHideShow"), WS_CHILD | WS_VISIBLE |BS_PUSHBUTTON,
			65 * cxChar, 20 * cyChar, 20 * cxChar, 2 * cyChar, hwnd, HMENU(ID_SETSHOWHIDE),
			hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("GetCheck"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			5 * cxChar, 23 * cyChar, 20 * cxChar, 2 * cyChar, hwnd, HMENU(ID_GETCHECK),
			hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("GetWindowText"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			40 * cxChar, 23 * cyChar, 20 * cxChar, 2 * cyChar, hwnd, HMENU(ID_GETWNDOWTEXT),
			hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("ÆôÓÃ\\½ûÓÃ"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			65 * cxChar, 23 * cyChar, 20 * cxChar, 2 * cyChar, hwnd, HMENU(ID_SETENABLE),
			hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("SetUnCheck"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			5 * cxChar, 26 * cyChar, 20 * cxChar, 2 * cyChar, hwnd, HMENU(ID_SETUNCHECK),
			hInst, nullptr);

		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case ID_PUSHBUTTON:
			MessageBeep(3);
			break;
		case ID_CHECKBOX:	
			SendMessage(HWND(lParam), BM_SETCHECK,
				!SendMessage(HWND(lParam), BM_GETCHECK, 0, 0), 0);
			break;
		case ID_RADIOBUTTON:
			SendMessage(HWND(lParam), BM_SETCHECK,
				!SendMessage(HWND(lParam), BM_GETCHECK, 0, 0), 0);
			break;
		case ID_SETCHECK:
			SendMessage(GetDlgItem(hwnd,ID_CHECKBOX), BM_SETCHECK, 1, 0);
			SendMessage(GetDlgItem(hwnd, ID_RADIOBUTTON), BM_SETCHECK, 1, 0);
			break;
		case ID_GETCHECK:
			hdc = GetDC(hwnd);
			SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
			SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
			if (SendMessage(GetDlgItem(hwnd, ID_CHECKBOX), BM_GETCHECK, 0, 0))
			  TextOut(hdc, 5 * cxChar, 5 * cyChar, isSelect, lstrlen(isSelect));
			else
				TextOut(hdc, 5 * cxChar, 5 * cyChar, isNotSelect, lstrlen(isNotSelect));
			ReleaseDC(hwnd, hdc);
			break;
		case ID_SETUNCHECK:
			SendMessage(GetDlgItem(hwnd, ID_CHECKBOX), BM_SETCHECK, 0, 0);
			SendMessage(GetDlgItem(hwnd, ID_RADIOBUTTON), BM_SETCHECK, 0, 0);
			break;
		case ID_SETWINDOWTEXT:
			SetWindowText(GetDlgItem(hwnd, ID_CHECKBOX), TEXT("zengyonghua"));
			SetWindowText(GetDlgItem(hwnd, ID_RADIOBUTTON), TEXT("zengyonghua"));
			SetWindowText(GetDlgItem(hwnd, ID_PUSHBUTTON), TEXT("zengyonghua"));
			break;
		case ID_GETWNDOWTEXT:
			GetWindowText(GetDlgItem(hwnd, ID_PUSHBUTTON), szBuffer, 128);
			hdc = GetDC(hwnd);
			SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
			SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
			TextOut(hdc, 30 * cxChar, 5 * cyChar, szBuffer, lstrlen(szBuffer));
			ReleaseDC(hwnd, hdc);
			break;
		case ID_SETSHOWHIDE:
			if (IsWindowVisible(GetDlgItem(hwnd, ID_PUSHBUTTON)))
				ShowWindow(GetDlgItem(hwnd, ID_PUSHBUTTON), SW_HIDE);
			else
				ShowWindow(GetDlgItem(hwnd, ID_PUSHBUTTON), SW_SHOWNORMAL);
			if (IsWindowVisible(GetDlgItem(hwnd, ID_CHECKBOX)))
				ShowWindow(GetDlgItem(hwnd, ID_CHECKBOX), SW_HIDE);
			else
				ShowWindow(GetDlgItem(hwnd, ID_CHECKBOX), SW_SHOWNORMAL);
			if (IsWindowVisible(GetDlgItem(hwnd, ID_RADIOBUTTON)))
				ShowWindow(GetDlgItem(hwnd,ID_RADIOBUTTON), SW_HIDE);
			else
				ShowWindow(GetDlgItem(hwnd,ID_RADIOBUTTON), SW_SHOWNORMAL);
			break;
		case ID_SETENABLE:
			IsWindowEnabled(GetDlgItem(hwnd, ID_PUSHBUTTON)) ? EnableWindow(GetDlgItem(hwnd, ID_PUSHBUTTON), false) : EnableWindow(GetDlgItem(hwnd, ID_PUSHBUTTON), true);
			IsWindowEnabled(GetDlgItem(hwnd, ID_CHECKBOX)) ? EnableWindow(GetDlgItem(hwnd, ID_CHECKBOX), false) : EnableWindow(GetDlgItem(hwnd, ID_CHECKBOX), true);
			IsWindowEnabled(GetDlgItem(hwnd, ID_RADIOBUTTON)) ? EnableWindow(GetDlgItem(hwnd, ID_RADIOBUTTON), false) : EnableWindow(GetDlgItem(hwnd, ID_RADIOBUTTON), true);

			break;
		}
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
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
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