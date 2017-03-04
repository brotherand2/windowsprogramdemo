//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#define WINVER 0x0500           // ½«´ËÖµ¸ü¸ÄÎªÏàÓ¦µÄÖµ£¬ÒÔÊÊÓÃÓÚ Windows µÄÆäËû°æ±¾¡£  
#define _WIN32_WINNT 0x0501     // ½«´ËÖµ¸ü¸ÄÎªÏàÓ¦µÄÖµ£¬ÒÔÊÊÓÃÓÚ Windows µÄÆäËû°æ±¾¡£  
#define _WIN32_WINDOWS 0x0410 // ½«´ËÖµ¸ü¸ÄÎªÊÊµ±µÄÖµ£¬ÒÔÊÊÓÃÓÚ Windows Me »ò¸ü¸ß°æ±¾¡£  
#define _WIN32_IE 0x0501        // ½«´ËÖµ¸ü¸ÄÎªÏàÓ¦µÄÖµ£¬ÒÔÊÊÓÃÓÚ IE µÄÆäËû°æ±¾¡£  

#include<Windows.h>
#include<string>
#include<vector>
#include<strsafe.h>
#include<ctime>
#include"resource.h"
//#include"winver.h"
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
vector<tstring>file;
int const wallpaperID = 1;
int const tipID = 2;
vector<tstring>allTitle;
void FindFile(const tstring &wallpaper)
{
	WIN32_FIND_DATA fd;
	HANDLE handle=FindFirstFile(wallpaper.c_str(),&fd);
	if (handle!= INVALID_HANDLE_VALUE)
	{
		do
		{
			if (fd.cFileName[0] == '.')
				break;
			file.push_back(fd.cFileName);
		} while (FindNextFile(handle, &fd));
	}
}
int _stdcall EnumWindowProc(HWND hwnd, long lParam)
{
	TCHAR title[256];
	if (IsWindow(hwnd) && IsWindowVisible(hwnd) && !IsIconic(hwnd))
	{
		GetWindowText(hwnd, title, 256);
		if (title[0] != '\0')
		allTitle.push_back(title);
	}
	return true;
}
int _stdcall DlgProc(HWND hwnd, unsigned int message, unsigned wParam, long lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
		return true;
	case WM_COMMAND:
		switch (wParam)
		{
		case IDC_BUTTON1:
			EndDialog(hwnd, 0);
			return true;
		}
		return true;
	}
	return false;
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	static HWND hScreen;
	RECT rect;
	HRGN rgn;
	TCHAR picPath[256];
	TCHAR newPath[256] = TEXT("E:\\BaiduYunDownload\\zhaozu__2.jpg");
	tstring wallpaper = TEXT("E:\\±ÚÖ½\\SogouWP\\Local\\WallPaper\\");
	tstring format = TEXT("*.jpg");
	int size;
	static SCROLLINFO si;
	static int iVertPos, iHorzPos;
	int start, end;
	TCHAR szBuffer[128];
	static int iPerLine,iAccumLine,ulScrollLines;
	static NOTIFYICONDATA nid;
	switch (UMsg)
	{
	case WM_CREATE:
		nid.cbSize = sizeof(nid);
		nid.dwInfoFlags = NIF_ICON;
		nid.hIcon = LoadIcon(((LPCREATESTRUCT)lParam)->hInstance, "IDI_ICON1");
		nid.uFlags = NIF_ICON | NIF_INFO | NIF_MESSAGE|NIF_TIP;
		nid.hWnd = hwnd;
		nid.uID = 0;
		StringCchCopy(nid.szInfo, 256, TEXT("ÏÖÔÚ»»±ÚÖ½ÁË£¬ÔÚ»Øµ½×ÀÃæµÄÊ±ºò£¬Ã¿2Ãë»»Ò»´Î±ÚÖ½"));
		StringCchCopy(nid.szInfoTitle, 64, TEXT("¶Ô×ÅÆÁÄ»ÄÇÃ´¾Ã£¬¸Ã³öÈ¥ÐÝÏ¢ÐÝÏ¢ÁË£¬×¢ÒâÑÛ¾¦"));
		StringCchCopy(nid.szTip, 128, TEXT("ßÍ£¬´ô×Ó¿´Ê²Ã´"));
		nid.uCallbackMessage = WM_USER;
		Shell_NotifyIcon(NIM_ADD, &nid);
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		hScreen = GetDesktopWindow();
		FindFile(wallpaper + format);
		SetTimer(hwnd, wallpaperID, 3000, nullptr);
		SetTimer(hwnd, tipID, 60000*30, nullptr);
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ulScrollLines, 0);
		if (ulScrollLines)
			iPerLine = WHEEL_DELTA / ulScrollLines;
		else
			iPerLine = 0;
		srand(time(0));
		break;
	case WM_MOUSEWHEEL:
		if (ulScrollLines == 0)
			break;
		iAccumLine += (SHORT)HIWORD(wParam);
		while (iAccumLine >= iPerLine)
		{
			iAccumLine -= iPerLine;
			SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, lParam);
		}
		while (iAccumLine <= -iPerLine)
		{
			iAccumLine += iPerLine;
			SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, lParam);
		}
		break;
	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case 1:
			DialogBox((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG1),hwnd, DlgProc);
			break;
		}
	}
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hScreen);
		Rectangle(hdc, 0, 0, 54, 44);
		ReleaseDC(hScreen, hdc);
		break;
	case WM_RBUTTONDOWN:
		InvalidateRect(nullptr, nullptr, true);
		break;
	case WM_TIMER:
		hdc = GetDC(hScreen);
		
		//rect.left = rect.top = 0;
		//rect.right = 54;
		//rect.bottom = 44;
		//rgn = CreateRectRgnIndirect(&rect);
		//SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		//SystemParametersInfo(SPI_GETDESKWALLPAPER, 256, picPath, 0);
		//RedrawWindow(nullptr,nullptr, nullptr,RDW_ERASE|RDW_INVALIDATE|RDW_ALLCHILDREN);
		 ReleaseDC(hScreen, hdc);
		//InvalidateRect(nullptr, nullptr, true);
		 switch (wParam)
		 {
		 case tipID:
			 Shell_NotifyIcon(NIM_MODIFY, &nid);
			 break;
		 case wallpaperID:
			 EnumWindows(EnumWindowProc, 0);
			 size = file.size();

			 if (allTitle.size() < 2&&size>0)
			 {
				SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,(PVOID) (wallpaper+file[rand()%size]).c_str(), 0);
				//TextOut(hdc, 0, 0, picPath, lstrlen(picPath));

			 }
			 allTitle.clear();
			break;
		 }
		break;
	case WM_USER:
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			SendMessage(hwnd, WM_CLOSE, wParam, lParam);
			break;
		case WM_LBUTTONDOWN:
			ShowWindow(hwnd,SW_NORMAL);
			SetForegroundWindow(hwnd);
			break;
		}
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nMin = 0;
		si.nMax = file.size() - 1;
		si.nPage=cyClient / cyChar;
		SetScrollInfo(hwnd, SB_VERT, &si, true);
		si.nMax = 100;
		si.nPage = cxClient / cxChar;
		SetScrollInfo(hwnd, SB_HORZ, &si, true);
		if (IsIconic(hwnd))
			ShowWindow(hwnd, SW_HIDE);
		break;
	case WM_VSCROLL:
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &si);
		iVertPos = si.nPos;
		switch (LOWORD(wParam))
		{
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
		case SB_TOP:
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM:
			si.nPos = si.nMax;
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
		}
		break;
	case WM_HSCROLL:
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos= si.nPos;
		switch (LOWORD(wParam))
		{
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
		case SB_TOP:
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM:
			si.nPos = si.nMax;
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
		switch (wParam)
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
		case VK_HOME:
			SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0);
			break;
		case VK_END:
			SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0);
			break;
		case VK_PRIOR:
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0);
			break;
		case VK_NEXT:
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (file.size() > 0)
		{
			SetMapMode(hdc, MM_ANISOTROPIC);
			SetWindowExtEx(hdc, 1, 1, nullptr);
			SetViewportExtEx(hdc, cxChar, cyChar, nullptr);
			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;
			GetScrollInfo(hwnd, SB_VERT, &si);
			iVertPos = si.nPos;
			GetScrollInfo(hwnd, SB_HORZ, &si);
			iHorzPos = si.nPos;
			start = max(0, min(file.size()-1, iVertPos + ps.rcPaint.top/cyChar));
			end = max(0, min(file.size()-1, iVertPos + ps.rcPaint.bottom/cyChar));
			for (int i = start,j=0; i <= end; i++,j++)
			{
				int x = -iHorzPos;
				int y = i - iVertPos;
				StringCchPrintf(szBuffer, 128, TEXT("%d:    "), i);
				tstring temp = tstring(szBuffer) + file[i];
				TextOut(hdc, x,y, temp.c_str(), temp.size());
			}
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hwnd, wallpaperID);
		KillTimer(hwnd, tipID);
		Shell_NotifyIcon(NIM_DELETE, &nid);
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = TEXT("zeng");
	//wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wndclass.lpszMenuName = TEXT("IDR_MENU1");
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.hIconSm = LoadIcon(hInstance, TEXT("IDI_ICON0"));
	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("keyScroll"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr,nullptr, hInstance, 0);
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