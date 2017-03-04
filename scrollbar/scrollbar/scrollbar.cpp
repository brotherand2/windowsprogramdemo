#include<Windows.h>
#include<memory>
#include<random>
#include<string>
#include<ctime>
using namespace std;
typedef struct systemetric
{
	string label;
	string content;
	int index;
}Systemetric;
using metric = shared_ptr <Systemetric> ;
string data = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
metric randomData(int &count)
{
	default_random_engine e;
	uniform_int_distribution<int> uid(10, 20);
	uniform_int_distribution<int>uid1(100, 200);
	uniform_int_distribution<int>uid2(0, 51);
	e.seed(time(0));
	count = uid1(e);
	metric  mt(new Systemetric[count], [](Systemetric *s){delete[]s; });
	char temp[21];
	int len1, len2;
	for (int i = 0; i < count; i++)
	{
		len1 = uid(e);
		len2 = uid(e);
		for (int j = 0; j < len1; j++)
			temp[j] = data[uid2(e)];
		temp[len1] = '\0';
		(mt.get() + i)->label = temp;
		for (int j = 0; j < len2; j++)
			temp[j] = data[uid2(e)];
		temp[len2] = '\0';
		(mt.get() + i)->content = temp;
		(mt.get() + i)->index = uid1(e);
	}
	return mt;
}

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	static metric mt;
	HDC hdc;
	SCROLLINFO si;
	static int cxChar, cxCaps, cyChar, cxClient, cyClient,iMaxWidth;
	static int count;
	int iVertPos, iHorzPos,iCurPos;
	int beginPaint, endPaint;
	int x, y;
	string temp;
	char buffer[21];
	switch (UMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		iMaxWidth = 22 * cxCaps + 40 * cxChar;
		ReleaseDC(hwnd, hdc);
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		mt=randomData(count);
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		si.nMax = count - 1;
		si.nMin = 0;
		si.nPage = cyClient / cyChar;
		SetScrollInfo(hwnd, SB_VERT, &si, false);
		si.nMax = iMaxWidth / cxChar - 1;
		si.nMin = 0;
		si.nPage = cxClient / cxChar;
		SetScrollInfo(hwnd, SB_HORZ, &si, true);
		break;
	case WM_VSCROLL:
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS|SIF_RANGE|SIF_PAGE;
		GetScrollInfo(hwnd, SB_VERT, &si);
		iCurPos = si.nPos;
		switch (LOWORD(wParam))
		{
		case SB_TOP:
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM:
			si.nPos =si.nMax;
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
			si.nPos = HIWORD(wParam);
			break;
		}
		SetScrollInfo(hwnd, SB_VERT, &si, true);
		GetScrollInfo(hwnd, SB_VERT, &si);
		if (iCurPos != si.nPos)
		{
			ScrollWindow(hwnd, 0, cyChar*(iCurPos - si.nPos), nullptr, nullptr);
			//UpdateWindow(hwnd);
		}
		break;
	case WM_HSCROLL:
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_HORZ, &si);
		iCurPos = si.nPos;
		switch (wParam)
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
			si.nPos = HIWORD(wParam);
			break;
		}
		SetScrollInfo(hwnd, SB_HORZ, &si, true);
		GetScrollInfo(hwnd, SB_HORZ, &si);
		if(iCurPos != si.nPos)
		{
			ScrollWindow(hwnd,cxChar*(iCurPos-si.nPos),0,nullptr,nullptr);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd,SB_VERT,&si);
		iVertPos = si.nPos;
		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos = si.nPos;
		beginPaint = max(0, ps.rcPaint.top/cyChar+iVertPos);
		endPaint = min(count - 1, iVertPos + ps.rcPaint.bottom);
		for (int i = beginPaint; i <= endPaint; i++)
		{
			x =cxChar*( -iHorzPos);
			y = cyChar*(i - iVertPos);
			temp = (mt.get() + i)->label;
			TextOut(hdc, x, y, temp.c_str(), temp.size());
			temp = (mt.get() + i)->content;
			TextOut(hdc, x + 22 * cxCaps, y, temp.c_str(), temp.size());
			SetTextAlign(hdc, TA_TOP | TA_RIGHT);
			TextOut(hdc, x + iMaxWidth, y, buffer, wsprintf(buffer, "%d", (mt.get() + i)->index));
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
int WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, char *szCmdCommand, int iCmdShow)
{
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(nullptr, IDC_HELP);
	wndclass.hIcon = LoadIcon(nullptr, IDI_HAND);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = "zeng";
	wndclass.lpszMenuName = nullptr;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(nullptr, "register failed", "error", MB_ABORTRETRYIGNORE);
		return 0;
	}
	HWND hwnd = CreateWindow("zeng", "¹ö¶¯Ìõ²âÊÔ",WS_OVERLAPPEDWINDOW, 0,
		0, GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN), nullptr, nullptr, hInstance, 0);
	if (!hwnd)
	{
		MessageBox(nullptr, "create failed", "error", MB_ABORTRETRYIGNORE);
		return 0;
	}
	ShowWindow(hwnd,SHOW_ICONWINDOW);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}