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
const int ID_HELP = 1;
const int ID_FILE = 2;
const int ID_FONTCOUR = 3;
const int ID_FONTARI = 4;
const int ID_FONTTIME = 5;

HBITMAP StretchBitMap(const HBITMAP &bitmap1)
{
	BITMAP bm1, bm2;
	HDC hdcMem1, hdcMem2;
	HDC hdc = CreateIC(TEXT("DISPLAY"), 0, 0, 0);
	int cxChar =LOWORD( GetDialogBaseUnits());
	int cyChar =HIWORD(GetDialogBaseUnits());
	hdcMem1 = CreateCompatibleDC(hdc);
	hdcMem2 = CreateCompatibleDC(hdc);
	GetObject(bitmap1, sizeof(BITMAP), &bm1);
	bm2 = bm1;
	bm2.bmWidth *= 2;
	bm2.bmHeight *=2;
	bm2.bmWidthBytes = ((bm2.bmWidth + 15) / 16) * 2;
	HBITMAP bitmap2 = CreateBitmapIndirect(&bm2);
	SelectObject(hdcMem1, bitmap1);
	SelectObject(hdcMem2, bitmap2);
	StretchBlt(hdcMem2, 0, 0, bm2.bmWidth, bm2.bmHeight, hdcMem1, 0, 0, bm1.bmWidth, bm1.bmHeight, SRCCOPY);
	DeleteDC(hdc);
	DeleteDC(hdcMem1);
	DeleteDC(hdcMem2);
	return bitmap2;
}
void AddHelpToSys(HINSTANCE hInst,HWND hwnd)
{
	HMENU hMenu = GetSystemMenu(hwnd, false);
	AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	HBITMAP bitmap1 = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1));//TEXT("IDB_BITMAPMY")
	HBITMAP bitmap2 = StretchBitMap(bitmap1);
	AppendMenu(hMenu, MF_BITMAP, ID_HELP,(PTSTR)(long)bitmap2);
	DeleteObject(bitmap1);
}
HBITMAP CreateBitmapFont(int i)
{
	static TCHAR *szFaceName[]{TEXT("Courier New"), TEXT("Arial"), TEXT("Times New Roman")};
	HBITMAP hBitmap;
	HDC hdc = CreateIC(TEXT("DISPLAY"), 0, 0, 0);
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HFONT hFont = CreateFont(2*tm.tmHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, szFaceName[i]);
	SIZE size;
	HFONT oldFont=(HFONT) SelectObject(hdcMem, hFont);
	GetTextExtentPoint32(hdcMem, szFaceName[i], lstrlen(szFaceName[i]), &size);
	//hBitmap = CreateCompatibleBitmap(hdc, size.cx, size.cy);
	hBitmap = CreateBitmap(size.cx, size.cy, 1, 1, 0);
	SelectObject(hdcMem, hBitmap);
	TextOut(hdcMem, 0, 0, szFaceName[i], lstrlen(szFaceName[i]));
	DeleteObject(SelectObject(hdcMem, oldFont));
	DeleteDC(hdc);
	DeleteDC(hdcMem);
	DeleteObject(hFont);
	return hBitmap;
}
HMENU CreateMyMenu(HINSTANCE hInst)
{
	HMENU hMenu = CreateMenu();
	HMENU hMenuPop;
	HBITMAP hBitmap1, hBitmap2;
	hBitmap1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
	hBitmap2 = StretchBitMap(hBitmap1);
	DeleteObject(hBitmap1);
    hMenuPop = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
	AppendMenu(hMenu, MF_BITMAP | MF_POPUP,(int)hMenuPop, (TCHAR *)hBitmap2);
	hBitmap1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	hBitmap2 = StretchBitMap(hBitmap1);
	DeleteObject(hBitmap1);
	hMenuPop = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU2));
	AppendMenu(hMenu, MF_BITMAP | MF_POPUP, (int)hMenuPop, (TCHAR *)hBitmap2);
	hMenuPop = CreateMenu();
	for (int i = 0; i < 3; i++)
	{
		hBitmap1 = CreateBitmapFont( i);
		AppendMenu(hMenuPop, MF_BITMAP, ID_FONTCOUR + i, (TCHAR *)hBitmap1);
	}
	hBitmap1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
	hBitmap2 = StretchBitMap(hBitmap1);
	DeleteObject(hBitmap1);
	AppendMenu(hMenu, MF_BITMAP | MF_POPUP, (int)hMenuPop, (TCHAR *)hBitmap2);

	return hMenu;
}
void DeleteAllBitmaps(HWND hwnd)
{
	HMENU hMenu;
	int i;
	MENUITEMINFO mi{ sizeof(MENUITEMINFO), MIIM_SUBMENU|MIIM_TYPE};
	hMenu = GetSystemMenu(hwnd, false);
	GetMenuItemInfo(hMenu, ID_HELP, false, &mi);//false是指出ID_HELP是ID，不是位置
	DeleteObject((HBITMAP)mi.dwTypeData);

	hMenu= GetMenu(hwnd);
	for (int i = 0; i < 3; i++)
	{
		GetMenuItemInfo(hMenu,ID_FONTCOUR+i, false, &mi);
		DeleteObject((HBITMAP)mi.dwTypeData);
	}

	for (int i = 0; i < 3; i++)
	{
		GetMenuItemInfo(hMenu, i, true, &mi);
		DeleteObject((HBITMAP)mi.dwTypeData);
	}

}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc,hdcMem;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	static HBITMAP bitmap1;
	static HMENU hMenu;
	static int idCurFont = ID_FONTCOUR;
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		AddHelpToSys(hInst, hwnd);
		hMenu = CreateMyMenu(hInst);
		SetMenu(hwnd, hMenu);
		CheckMenuItem(hMenu, idCurFont, MF_CHECKED);
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
		case ID_FONTARI:
		case ID_FONTCOUR:
		case ID_FONTTIME:
			CheckMenuItem(hMenu, idCurFont, MF_UNCHECKED);
			idCurFont = wmId;
			CheckMenuItem(hMenu, idCurFont, MF_CHECKED);
			break;
		}
		break;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		bitmap1= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, bitmap1);
		BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
		DeleteDC(hdcMem);
		EndPaint(hwnd, &ps);//释放画笔资源
		break;
	case WM_CLOSE://窗口关闭
		DestroyWindow(hwnd);//销毁窗口
		break;
	case WM_DESTROY://窗口销毁时
		DeleteObject(bitmap1);
		DeleteAllBitmaps(hwnd);
		PostQuitMessage(0);//抛出一条返回值为0退出消息
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//用默认的窗口过程处理其它消息，做些其它处理
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
	wndclass.lpfnWndProc = WndProc;//窗口过程
	wndclass.lpszClassName = TEXT("zeng");//类名
	wndclass.lpszMenuName = nullptr;//菜单
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//哪些改变后要重画，设置水平和垂直改变后重绘
	if (!RegisterClass(&wndclass))//注册窗口类
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("勇华"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);//创建窗口
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	ShowWindow(hwnd, nCmdShow);//显示窗口
	UpdateWindow(hwnd);//进行重绘
	MSG msg;//消息结构
	while (GetMessage(&msg, nullptr, 0, 0))//从消息队列获取消息
	{
		TranslateMessage(&msg);//将一些按键消息转换成字符消息放在按键消息后面
		DispatchMessage(&msg);//分发消息，即调用窗口过程进行处理消息
	}
}