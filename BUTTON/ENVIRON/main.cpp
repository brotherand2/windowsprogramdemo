//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<memory>
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
void FillListBox(HWND hList)
{
	TCHAR *block = GetEnvironmentStrings();
	TCHAR *end,*beg;
	int len;
	while (*block)
	{
		if (*block != '=')
		{
			beg = block;
			while (*block++ != '=')
				continue;
			end = block - 1;
			len = end - beg;
			string var(beg, end);
			SendMessage(hList, LB_ADDSTRING, 0,(long) var.c_str());
		}
		while (*block++ != '\0')
			continue;
	}
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	int wmId, wmEvent;
	static HWND hText, hList;
	HINSTANCE hInst;
	int len;
	TCHAR *varName;
	switch (UMsg)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		hText = CreateWindow(TEXT("static"), TEXT(""), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)1, hInst, nullptr);
		hList = CreateWindow(TEXT("listbox"), TEXT(""), WS_CHILD | WS_VISIBLE |LBS_NOTIFY| WS_VSCROLL | WS_BORDER, cxChar, 3 * cyChar, 28 * cxChar, 10 * cyChar,
			hwnd,(HMENU)2, hInst, nullptr);
		FillListBox(hList);
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		MoveWindow(hText, cxChar, cyChar, cxClient, cyChar, true);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		if (wmId== 2 && wmEvent ==LBN_SELCHANGE)
		{
			int index = SendMessage(hList,LB_GETCURSEL, 0, 0);
			len = SendMessage(hList, LB_GETTEXTLEN, index, 0);
			varName = new TCHAR[len + 1];
			shared_ptr<TCHAR>sp(varName, [](TCHAR *sp){delete[]sp; });
			SendMessage(hList, LB_GETTEXT, index,(long) varName);
			len = GetEnvironmentVariable(varName, nullptr, 0);
			TCHAR *value = new TCHAR[len + 1];
			shared_ptr<TCHAR>sp1(value, [](TCHAR *sp){delete[]sp; });
			GetEnvironmentVariable(varName, value, len + 1);
			SetWindowText(hText, value);
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