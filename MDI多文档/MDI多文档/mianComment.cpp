//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include"resource.h"
using namespace std;
//#ifndef UNICODE 
//typedef  string tstring;
//#else
//typedef  wstring tstring;
//#endif
#ifndef UNICODE //如果没有定义UNICODE ,用tstring替换为string,否则替换为wstring
using  tstring = string;
#else
using  tstring = wstring;
#endif

TCHAR szAppName[]=TEXT("MDIDEMO");
TCHAR szFrameClass[] = TEXT("MdiFrame");
TCHAR szHelloClass[] = TEXT("MdiHelloChild");
TCHAR szRectClass[] = TEXT("MdiRectChild");
HMENU hMenuInit, hMenuHello, hMenuRect;
HMENU hMenuInitWindow, hMenuHelloWindow, hMenuRectWindow;
const int INIT_MENU_POS = 0;
const int HELLO_MENU_POS = 2;
const int RECT_MENU_POS = 1;
const int IDM_FIRST_CHILD = 50000;
typedef struct
{
	unsigned int iColor;
	COLORREF clrText;
}HELLODATA;
typedef struct
{
	short cxClient;
	short cyClient;
}RECTDATA;
int _stdcall CloseEnumWndProc(HWND hwnd, long lParam)
{
	SendMessage(GetParent(hwnd), WM_MDIRESTORE, 0, 0);
	if (!SendMessage(hwnd, WM_QUERYENDSESSION, 0, 0))
		return true;
	SendMessage(GetParent(hwnd), WM_MDIDESTROY, (long)hwnd, 0);
	return true;
}
long _stdcall FrameWndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	static HWND hwndClient;
	CLIENTCREATESTRUCT clientCreate;
	static MDICREATESTRUCT mdiCreate;
	HWND hwndChild;
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		clientCreate.hWindowMenu = hMenuInitWindow;
		clientCreate.idFirstChild = IDM_FIRST_CHILD;
		hwndClient = CreateWindow(TEXT("MDICLIENT"), 0, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)1, hInst, (LPVOID)&clientCreate);
		mdiCreate.cx = CW_USEDEFAULT; 
		mdiCreate.cy = CW_USEDEFAULT;
		mdiCreate.hOwner = hInst;
		mdiCreate.lParam = 0;
		mdiCreate.style = 0;
		mdiCreate.x = CW_USEDEFAULT;
		mdiCreate.y = CW_USEDEFAULT;
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		switch (wmId)
		{
		case IDM_APP_EXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case IDM_FILE_CLOSE:
			hwndChild =(HWND) SendMessage(hwndClient, WM_MDIGETACTIVE, 0, 0);
			if (hwndChild)
			{
				if (!SendMessage(hwndChild, WM_QUERYENDSESSION, 0, 0))
					return 0;
				SendMessage(hwndClient, WM_MDIDESTROY, (long)hwndChild, 0);
			}
			break;
		case IDM_FILE_NEWHELLO:
			mdiCreate.szClass = szHelloClass;
			mdiCreate.szTitle = TEXT("Hello");
			SendMessage(hwndClient, WM_MDICREATE, 0, (long)&mdiCreate);
			break;
		case IDM_FILE_NEWRECT:
			mdiCreate.szClass = szRectClass;
			mdiCreate.szTitle = TEXT("Rectangles");
			SendMessage(hwndClient, WM_MDICREATE, 0, (long)&mdiCreate);
			break;
		case IDM_WINDOW_ARRANGE:
			SendMessage(hwndClient, WM_MDIICONARRANGE, 0, 0);
			break;
		case IDM_WINDOW_CASCADE:
			SendMessage(hwndClient, WM_MDICASCADE, 0, 0);
			break;
		case IDM_WINDOW_TILE:
			SendMessage(hwndClient, WM_MDITILE, 0, 0);
			break;
		case IDM_WINDOW_CLOSEALL:
			EnumChildWindows(hwndClient, CloseEnumWndProc, 0);
			break;
		default:
			hwndChild =(HWND) SendMessage(hwndClient,WM_MDIGETACTIVE, 0, 0);
			if (IsWindow(hwndChild))
				SendMessage(hwndChild, WM_COMMAND, wParam, lParam);
			break;
		}
		break;
	case WM_QUERYENDSESSION:
	case WM_CLOSE:
		SendMessage(hwnd, WM_COMMAND, IDM_WINDOW_CLOSEALL, 0);
		if (GetWindow(hwndClient, GW_CHILD))
			return 0;
		break;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔

		EndPaint(hwnd, &ps);//释放画笔资源
		break;
	case WM_DESTROY://窗口销毁时
		PostQuitMessage(0);//抛出一条返回值为0退出消息
		break;
	}
	return DefFrameProc(hwnd,hwndClient, UMsg, wParam, lParam);//用默认的窗口过程处理其它消息，做些其它处理
}
long _stdcall HelloWndProc(HWND hwnd, unsigned int message, unsigned int wParam, long lParam)
{
	HELLODATA *pHello;
	static HWND hwndClient, hwndFrame;
	RECT rect;
	HDC hdc;
	PAINTSTRUCT ps;
	int wmId, wmEvent;
	static COLORREF colors[]{RGB(0,0,0),RGB(255,0,0), RGB(0,255,0),RGB(0,0,255),RGB(255,255,255)};
	switch (message)
	{
	case WM_CREATE:
		pHello = (HELLODATA*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(HELLODATA));
		pHello->clrText = 0;
		pHello->iColor =IDM_COLOR_BLACK ;
		SetWindowLong(hwnd, 0, (long)pHello);
		hwndClient= GetParent(hwnd);
		hwndFrame = GetParent(hwndClient);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_COLOR_BLACK:
		case IDM_COLOR_BLUE:
		case IDM_COLOR_GREEN:
		case IDM_COLOR_RED:
		case IDM_COLOR_WHITE:
		    pHello = (HELLODATA*)GetWindowLong(hwnd, 0);
			CheckMenuItem(hMenuHello, pHello->iColor, MF_UNCHECKED);
			pHello->iColor = wmId;
			pHello->clrText = colors[wmId - IDM_COLOR_BLACK];
			CheckMenuItem(hMenuHello, pHello->iColor, MF_CHECKED);
			InvalidateRect(hwnd, 0, true);
			break;
		default:
			break;
		}
		break;
	case WM_QUERYENDSESSION:
	case WM_CLOSE:
		if (IDOK != MessageBox(hwnd, TEXT("是否关闭窗口"), TEXT("Hello"),MB_OKCANCEL|MB_ICONQUESTION))
			return 0;
		break;
	case WM_MDIACTIVATE:
		pHello = (HELLODATA*)GetWindowLong(hwnd, 0);
		if (lParam == (long)hwnd)
		{
			SendMessage(hwndClient, WM_MDISETMENU, (unsigned int)hMenuHello, (long)hMenuHelloWindow);
			CheckMenuItem(hMenuHello, pHello->iColor, MF_CHECKED);
		}
		else
		{
			SendMessage(hwndClient, WM_MDISETMENU, (unsigned int)hMenuInit, (long)hMenuInitWindow);
			CheckMenuItem(hMenuHello, pHello->iColor, MF_UNCHECKED);
		}
		DrawMenuBar(hwndFrame);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		pHello = (HELLODATA*)GetWindowLong(hwnd, 0);
		SetTextColor(hdc, pHello->clrText);
		DrawText(hdc, TEXT("Hello World"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		pHello = (HELLODATA*)GetWindowLong(hwnd, 0);
		HeapFree(GetProcessHeap(), 0, pHello);
		break;
	default:
		break;
	}
	return DefMDIChildProc(hwnd, message, wParam, lParam);
}
long _stdcall RectWndProc(HWND hwnd, unsigned int message, unsigned int wParam, long lParam)
{
	RECTDATA *pRect;
	static HWND hwndClient, hwndFrame;
	int left, top, right, bottom, red, green, blue;
	HBRUSH hOldBrush, hBrush;
	HDC hdc;
	switch (message)
	{
	case WM_CREATE:
		pRect = (RECTDATA*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(RECTDATA));
		SetWindowLong(hwnd, 0, (long)pRect);
		hwndClient = GetParent(hwnd);
		hwndFrame = GetParent(hwndClient);
		SetTimer(hwnd, 1, 250, 0);
		srand(GetTickCount());
		break;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED)
		{
			pRect = (RECTDATA*)GetWindowLong(hwnd, 0);
			pRect->cxClient = LOWORD(lParam);
			pRect->cyClient = HIWORD(lParam);
		}
		break;
	case WM_TIMER:
		hdc = GetDC(hwnd);
		pRect = (RECTDATA*)GetWindowLong(hwnd, 0);
		left = rand() % pRect->cxClient;
		right = rand() % pRect->cxClient;
		top = rand() % pRect->cyClient;
		bottom = rand() % pRect->cyClient;
		red = rand()%256;
		green = rand()%256;
		blue= rand()%256;
		hBrush = CreateSolidBrush(RGB(red, green, blue));
		hOldBrush =(HBRUSH) SelectObject(hdc, hBrush);
		Rectangle(hdc, min(left, right), min(top, bottom), max(left, right), max(top, bottom));
		DeleteObject(SelectObject(hdc, hOldBrush));
		ReleaseDC(hwnd, hdc);
		break;
	case WM_MDIACTIVATE:
		if (lParam == (long)hwnd)
			SendMessage(hwndClient, WM_MDISETMENU, (unsigned int)hMenuRect, (long)hMenuRectWindow);
		else
			SendMessage(hwndClient, WM_MDISETMENU, (unsigned int)hMenuInit, (long)hMenuInitWindow);
		DrawMenuBar(hwndFrame);
		break;
	case WM_DESTROY:
		pRect= (RECTDATA*)GetWindowLong(hwnd, 0);
		HeapFree(GetProcessHeap(), 0, pRect);
		break;
	default:
		break;
	}
	return DefMDIChildProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	WNDCLASS wndclass;//窗口类
	wndclass.cbClsExtra = 0;//为类分配的额外空间，可以保存一些地址、变量
	wndclass.cbWndExtra = 0;//为窗口分配的额外空间，可以保存一些地址、变量
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//背景画刷
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//鼠标光标
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);//图标
	wndclass.hInstance = hInstance;//实例名柄，程序的PID，一个整数，根据这个整数，系统可以找到进程的地址
	wndclass.lpfnWndProc = FrameWndProc;//窗口过程
	wndclass.lpszClassName = szFrameClass;//类名
	wndclass.lpszMenuName = nullptr;//菜单
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//哪些改变后要重画，设置水平和垂直改变后重绘
	if (!RegisterClass(&wndclass))//注册窗口类
	{
		MessageBox(nullptr, TEXT("register FrameClass failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	wndclass.lpszClassName = szHelloClass;
	wndclass.lpfnWndProc = HelloWndProc;
	wndclass.cbWndExtra = sizeof(HANDLE);
	if (!RegisterClass(&wndclass))//注册窗口类
	{
		MessageBox(nullptr, TEXT("register HelloClass failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	wndclass.lpszClassName =szRectClass;
	wndclass.lpfnWndProc = RectWndProc;
	if (!RegisterClass(&wndclass))//注册窗口类
	{
		MessageBox(nullptr, TEXT("register RectClass failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	hMenuInit=LoadMenu(hInstance, TEXT("IDR_MENUINIT"));
	hMenuHello = LoadMenu(hInstance, TEXT("IDR_MENUHELLO"));
	hMenuRect = LoadMenu(hInstance, TEXT("IDR_MENURECT"));
	hMenuInitWindow = GetSubMenu(hMenuInit, INIT_MENU_POS);
	hMenuHelloWindow = GetSubMenu(hMenuHello, HELLO_MENU_POS);
	hMenuRectWindow = GetSubMenu(hMenuRect, RECT_MENU_POS);
	HWND hwnd = CreateWindow(szFrameClass, szAppName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,0,
		hMenuInit, nullptr, hInstance, nullptr);//创建窗口
	HWND  hwndClient = GetWindow(hwnd,GW_CHILD);
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	ShowWindow(hwnd, nCmdShow);//显示窗口
	UpdateWindow(hwnd);//进行重绘
	MSG msg;//消息结构
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (GetMessage(&msg, nullptr, 0, 0))//从消息队列获取消息
	{
		if (!TranslateAccelerator(hwnd, hAccel, &msg) &&!TranslateMDISysAccel(hwndClient, &msg))
		{
			TranslateMessage(&msg);//将一些按键消息转换成字符消息放在按键消息后面
			DispatchMessage(&msg);//分发消息，即调用窗口过程进行处理消息
		}
	}
	DestroyMenu(hMenuHello);
	DestroyMenu(hMenuRect);
}