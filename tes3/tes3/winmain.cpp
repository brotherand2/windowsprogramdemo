#include<Windows.h>
#include<memory>
#include<random>
#include<string>
#include<ctime>
using namespace std;
typedef struct systemetric
{
	int index;
	string label;
	string content;
}Systemetric;
using metric = shared_ptr < Systemetric > ;
string data = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
metric randomData(int &count)
{
	default_random_engine e;
	e.seed(time(0));
	uniform_int_distribution<int> uid(5, 20);
	uniform_int_distribution <int>uid1(100, 200);
	uniform_int_distribution<int>uid2(0, 51);
	count = uid1(e);
	int len1 = uid(e);
	int len2 = uid(e);
	char str[21];
	metric mt(new Systemetric[count], [](Systemetric *s){delete[]s; });
	for (int j = 0; j < count; j++)
	{
		for (int i = 0; i < len1; i++)
			str[i] = data[uid2(e)];
		str[len1] = '\0';
		(mt.get() + j)->label = str;
		for (int i = 0; i < len2; i++)
			str[i] = data[uid2(e)];
		str[len2] = '\0';
		(mt.get() + j)->content = str;
		(mt.get() + j)->index = uid1(e);
	}
	return mt;
}
long _stdcall WndProc(HWND hwnd, unsigned  int UMsg, unsigned int wParam, long lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	static int cxChar, cyChar, cxCaps,iVScrollPos=0,iHScrollPos=0,cxClient,cyClient;
	static int count=0;
	string temp;
	static metric mt;
	char buffer[21];
	int iMaxWidth = 22 * cxCaps + 40 * cxChar - 1;
	switch (UMsg)
	{
	case WM_CREATE:
		hdc=GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);

		break;
	case WM_SIZE:
		cyClient = HIWORD(lParam);
		cxClient = LOWORD(lParam);
		mt=randomData(count);
		SetScrollRange(hwnd, SB_VERT, 0, count - 1,false);
		SetScrollRange(hwnd, SB_HORZ, 0, iMaxWidth/cxChar, true);
		break;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			iVScrollPos -= 1;
			break;
		case SB_LINEDOWN:
			iVScrollPos += 1;
			break;
		case SB_PAGEUP:
			iVScrollPos -= cyClient / cyChar;
			break;
		case SB_PAGEDOWN:
			iVScrollPos += cyClient / cyChar;
			break;
		case SB_THUMBTRACK:
			iVScrollPos = HIWORD(wParam);
			break;
		case SB_TOP:
			iVScrollPos = 0;
			break;
		case SB_BOTTOM:
			iVScrollPos = count - 1;
			break;
		}
		iVScrollPos = max(0, min(iVScrollPos, count - 1));
		if (iVScrollPos != GetScrollPos(hwnd, SB_VERT))
		{
			SetScrollPos(hwnd, SB_VERT,iVScrollPos, true);
			InvalidateRect(hwnd, nullptr, true);
		}
		break;
	case WM_HSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LEFT:
			iHScrollPos = 0;
			break;
		case SB_RIGHT:
			iHScrollPos = iMaxWidth/cxChar;
			break;
		case SB_LINELEFT:
			iHScrollPos -= 1;
			break;
		case SB_LINERIGHT:
			iHScrollPos += 1;
			break;
		case SB_PAGELEFT:
			iHScrollPos -= cxClient / cxChar;
			break;
		case SB_PAGERIGHT:
			iHScrollPos += cxClient / cxChar;
			break;
		case SB_THUMBPOSITION:
			iHScrollPos = HIWORD(wParam);
			break;
		}
		iHScrollPos = max(0, min(iMaxWidth/cxChar-1, iHScrollPos));
		if (iHScrollPos != GetScrollPos(hwnd, SB_HORZ))
		{
			SetScrollPos(hwnd, SB_HORZ, iHScrollPos, true);
			InvalidateRect(hwnd, nullptr, true);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < count; i++)
		{
			int y = (i - iVScrollPos)*cyChar;
			int x = iHScrollPos*cxChar;
			temp = (mt.get() + i)->label;
			TextOut(hdc, 0-x, y, temp.c_str(), temp.size());
			temp = (mt.get() + i)->content;
			TextOut(hdc, 22 * cxCaps-x, y, temp.c_str(), temp.size());
			SetTextAlign(hdc, TA_TOP | TA_RIGHT);
			TextOut(hdc, 22 * cxCaps + 40 * cxChar-x, y,buffer, wsprintf(buffer, "%d", (mt.get() + i)->index));
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
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *szCmdLine, int iCmdShow)
{
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(nullptr, IDC_APPSTARTING);
	wndclass.hIcon = LoadIcon(nullptr, IDI_ERROR);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = "zeng";
	wndclass.lpszMenuName = nullptr;
	wndclass.style =CS_VREDRAW|CS_HREDRAW;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(nullptr, "register failed", "error", MB_ICONSTOP);
		return 0;
	}
	HWND hwnd = CreateWindow("zeng", "ÔøÓÂ»ª¹ö¶¯Ìõ", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, 0);
	if (!hwnd)
	{
		MessageBox(nullptr, "create window failed", "error", MB_ICONSTOP);
		return 0;
	}
	ShowWindow(hwnd, SHOW_FULLSCREEN);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}