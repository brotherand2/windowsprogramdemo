//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<random>
#include<memory>
#include<vector>
#include<ctime>
#include<strsafe.h>
using namespace std;
//#ifndef UNICODE 
//typedef  string tstring;
//#else
//typedef  wstring tstring;
//#endif
#ifndef UNICODE 
using  tstring =string;
#else
using  tstring=wstring ;
#endif
tstring data = TEXT("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
typedef struct systemmetric
{
	int index;
	tstring label;
	tstring constant;
}Systemmetric;
using  randStr = shared_ptr < Systemmetric > ;
randStr randData(int &count)
{
	default_random_engine e;
	e.seed(time(0));
	uniform_int_distribution<int> uid(20,40);
	uniform_int_distribution<int> uid1(100, 200);
	uniform_int_distribution<int> uid2(0, 51);
	count = uid1(e);
	randStr metric(new Systemmetric[count], [](Systemmetric *s){delete[]s; });
	TCHAR temp[41];
	for (int i = 0; i < count; i++)
	{
		int len1 = uid(e);
		for (int j = 0; j < len1; j++)
			temp[j] = data[uid2(e)];
		temp[len1] = TEXT('\0');
		(metric.get() + i)->label = temp;
		int len2 = uid(e);
		for (int j = 0; j < len2; j++)
			temp[j] = data[uid2(e)];
		temp[len2] = TEXT('\0');
		(metric.get() + i)->constant = temp;
		(metric.get() + i)->index = uid1(e);
	}
	return metric;
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int iVertPos, iHorzPos;
	static int cxClient, cyClient,iMaxWidth;
	static int cxChar, cxCaps, cyChar;
	static randStr metric;
	static int count;
	int beginPaint, endPaint;
	SCROLLINFO si;
	tstring temp;
	TCHAR buffer[12];
	switch (UMsg)
	{
	case WM_VSCROLL:
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &si);
		iVertPos = si.nPos;
		switch (LOWORD(wParam))
		{
		case SB_TOP:
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;
		case SB_LINEUP:
			si.nPos -= 1;
			break;
		case SB_LINEDOWN:
			si.nPos += 1;
			break;
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;
		}
		SetScrollInfo(hwnd, SB_VERT, &si, true);
		GetScrollInfo(hwnd, SB_VERT, &si);
		if (iVertPos != si.nPos)
		{
			ScrollWindow(hwnd, 0, cyChar*(iVertPos - si.nPos), nullptr, nullptr);
			//UpdateWindow(hwnd);
		}
		break;
	case WM_HSCROLL:
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos = si.nPos;
		switch (LOWORD(wParam))
		{
		case SB_LEFT:
			si.nPos = si.nMin;
			break;
		case SB_RIGHT:
			si.nPos = si.nMax;
			break;
		case SB_LINELEFT:
			si.nPos -= 1;
			break;
		case SB_LINERIGHT:
			si.nPos += 1;
			break;
		case SB_PAGELEFT:
			si.nPos -= si.nPage;
			break;
		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;
		case SB_THUMBPOSITION:
			si.nPos = si.nTrackPos;
			break;
		}
		SetScrollInfo(hwnd, SB_HORZ, &si, true);
		GetScrollInfo(hwnd, SB_HORZ, &si);
		if (iHorzPos != si.nPos)
		{
			ScrollWindow(hwnd, cxChar*(iHorzPos - si.nPos), 0, nullptr, nullptr);
		}
		break;
	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_LEFT:
			SendMessage(hwnd, WM_HSCROLL, SB_LINELEFT, 0);
			break;
		case VK_RIGHT:
			SendMessage(hwnd, WM_HSCROLL, SB_LINERIGHT, 0);
			break;
		case VK_UP:
			SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
			break;
		case VK_DOWN:
			SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
			break;
		case VK_PRIOR:
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0);
			break;
		case VK_NEXT:
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0);
			break;
		case VK_HOME:
			SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0);
			break;
		case VK_END:
			SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0);
			break;
		}
		break;
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		iMaxWidth = 42 * cxCaps + 80 * cxChar;
		ReleaseDC(hwnd, hdc);
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		metric = randData(count);
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = count - 1;
		si.nPage = cyClient / cyChar;
		SetScrollInfo(hwnd, SB_VERT, &si, true);
		si.nMax = iMaxWidth - 1;
		si.nPage = cxClient / cxChar;
		SetScrollInfo(hwnd, SB_HORZ, &si, true);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &si);
		iVertPos = si.nPos;
		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos = si.nPos;
		beginPaint = max(0, iVertPos + ps.rcPaint.top/cyChar);
		endPaint = min(count - 1, iVertPos + ps.rcPaint.bottom/cyChar);
		for (int i = beginPaint; i <= endPaint; i++)
		{
			int x = cxChar*(-iHorzPos);
			int y = cyChar*(i - iVertPos);
			temp = (metric.get() + i)->label;
			TextOut(hdc, x, y, temp.c_str(), temp.size());
			temp = (metric.get() + i)->constant;
			TextOut(hdc, x + 42 * cxCaps, y, temp.c_str(), temp.size());
			SetTextAlign(hdc, TA_TOP | TA_RIGHT);
			StringCchPrintf(buffer, 12, TEXT("%d"),(metric.get()+i)->index);
			TextOut(hdc, x + iMaxWidth, y, buffer, lstrlen(buffer));
			SetTextAlign(hdc, TA_TOP | TA_LEFT);
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
		CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
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