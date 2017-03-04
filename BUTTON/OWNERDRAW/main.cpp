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
int const ID_SMALL = 1;
int const ID_LARGER = 2;
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	static HWND hwndSmaller, hwndLarger;
	static int BTN_WIDTH, BTN_HEIGHT;
	LPDRAWITEMSTRUCT lpds;
	int cx, cy;
	RECT rect;
	POINT pt_small[] = {3,1,5,1,4,3,7,3,7,5,5,4,5,7,3,7,4,5,1,5,1,3,3,4};
	POINT pt_larger[] = {5,3,3,3,4,1,5,5,5,3,7,4,3,5,5,5,4,7,3,3,3,5,1,4};
	int aiCounts[] = { 3, 3, 3, 3 };
	int id, event;
	switch (UMsg)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		hwndSmaller = CreateWindow(TEXT("button"), TEXT(""),
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 0, 0, hwnd, (HMENU)ID_SMALL,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), nullptr);
		hwndLarger = CreateWindow(TEXT("button"), TEXT(""),
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 0, 0,
			hwnd, (HMENU)ID_LARGER, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), nullptr);
		BTN_WIDTH = 8 * cxChar;
		BTN_HEIGHT = 4 * cyChar;
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		MoveWindow(hwndSmaller, cxClient / 2 - 3 * BTN_WIDTH / 2, cyClient / 2 - BTN_HEIGHT / 2, BTN_WIDTH, BTN_HEIGHT,true);
		MoveWindow(hwndLarger, cxClient / 2 + BTN_WIDTH / 2, cyClient / 2 - BTN_HEIGHT / 2, BTN_WIDTH, BTN_HEIGHT, true);

		break;
	case WM_DRAWITEM:
		lpds = (LPDRAWITEMSTRUCT)lParam;
		rect = lpds->rcItem;
		
		FillRect(lpds->hDC, &rect, (HBRUSH)GetStockObject( WHITE_BRUSH));
		FrameRect(lpds->hDC, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		cx = rect.right;
		cy = rect.bottom;
		SetMapMode(lpds->hDC, MM_ANISOTROPIC);
		SetWindowExtEx(lpds->hDC, 8, 8, nullptr);
		SetViewportExtEx(lpds->hDC, cx, cy, nullptr);
		SelectObject(lpds->hDC, GetStockObject(BLACK_BRUSH));

		switch (lpds->CtlID)
		{
		case ID_SMALL:
			PolyPolygon(lpds->hDC, pt_small, aiCounts, 4);
			break;
		case ID_LARGER:
			PolyPolygon(lpds->hDC, pt_larger, aiCounts, 4);
			break;
		}
		SelectObject(lpds->hDC, GetStockObject(WHITE_BRUSH));
		if (lpds->itemState&ODS_SELECTED)
			InvertRect(lpds->hDC, &rect);
		if (lpds->itemState&ODS_FOCUS)
		{
			rect.left=rect.top = 1;
			rect.right = rect.bottom = 7;
			DrawFocusRect(lpds->hDC, &rect);
		}
		break;
	case WM_COMMAND:
		GetWindowRect(hwnd, &rect);
		id = LOWORD(wParam);
		event = HIWORD(wParam);
		switch (id)
		{
		case ID_SMALL:
			rect.left += cxClient / 20;
			rect.right -= cxClient / 20;
			rect.top+= cyClient / 20;
			rect.bottom -= cyClient / 20;
			break;
		case ID_LARGER:
			rect.left -= cxClient / 20;
			rect.right += cxClient / 20;
			rect.top -= cyClient / 20;
			rect.bottom += cyClient / 20;
			break;
		}
		MoveWindow(hwnd, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, true);
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