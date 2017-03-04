//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<strsafe.h>
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
# define IDM_DEVMODE 1000

void BasicInfo(HDC hdc, HDC hdcInfo, int cxChar, int cyChar)
{
	static struct
	{
		int index;
		TCHAR *szDesc;
	}
	info[] =
	{
		HORZSIZE, TEXT("HORZSIZE 以毫米为单位的显示宽度"),
		VERTSIZE, TEXT("VERTSIZE 以毫米为单位的显示高度"),
		HORZRES, TEXT("HORZRES 以像素为单位的显示宽度 0~65535"),
		VERTRES, TEXT("VERTRES 以像素为单位的显示高度 0~65535"),
		BITSPIXEL, TEXT("BITSPIXEL每个像素颜色位数"),
		PLANES, TEXT("PLANES：颜色位面数"),
		NUMBRUSHES, TEXT("NUMBRUSHES：设备指定画刷数"),
		NUMPENS, TEXT("NUMPENS：设备指定笔数"),
		NUMMARKERS, TEXT("NUMMARKERS      Number of device markers:"),
		NUMFONTS, TEXT("NUMFONTS：设备指定字体数"),
		NUMCOLORS, TEXT("NUMCOLORS：设备颜色表的入口数，如果设备的色深不超过8位像素。对于超过色深的设备返回－1"),
		PDEVICESIZE, TEXT("PDEVICESIZE     Size of device structure:"),
		ASPECTX, TEXT("ASPECTX：用于画线的设备像素的相对宽度"),
		ASPECTY, TEXT("ASPECTY：用于画线的设备像素的相对高度"),
		ASPECTXY, TEXT("ASPECTXY        Relative diagonal of pixel:"),
		LOGPIXELSX, TEXT("LOGPIXELSX 像素/逻辑英寸（水平）"),
		LOGPIXELSY, TEXT("LOGPIXELSY 像素/逻辑英寸（垂直）"),
		SIZEPALETTE, TEXT("SIZEPALETTE     Number of palette entries:"),
		NUMRESERVED, TEXT("NUMRESERVED     Reserved palette entries:"),
		COLORRES, TEXT("COLORRES        Actual color resolution:"),
		PHYSICALWIDTH, TEXT("PHYSICALWIDTH   Printer page pixel width:"),
		PHYSICALHEIGHT, TEXT("PHYSICALHEIGHT  Printer page pixel height:"),
		PHYSICALOFFSETX, TEXT("PHYSICALOFFSETX Printer page x offset:"),
		PHYSICALOFFSETY, TEXT("PHYSICALOFFSETY Printer page y offset:")
	};
	TCHAR szBuffer[1024];
	for (int i = 0; i < sizeof(info) / sizeof(info[0]); i++)
	{
		StringCchPrintf(szBuffer, 1024, TEXT("%-60s%-8d"), info[i].szDesc, GetDeviceCaps(hdcInfo, info[i].index));
		TextOut(hdc, 0, i*cyChar, szBuffer, lstrlen(szBuffer));
	}
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	HDC hdcInfo;
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	unsigned long dwNeeded;//保存打印机的结构数量
	unsigned long dwReturned;//返回的保存打印机结构大小
	static PRINTER_INFO_4 *pinfo4;//打印机信息结构指针
	static int nCurrentDevice = IDM_SCREEN;//当前设备ID
	static int nCurrentInfo = IDM_BASIC;//当前作息菜单ID
	HMENU menu;
	static TCHAR szDeviceName[128];//设备名
	static TCHAR szWindowText[128];//窗口标题
	HANDLE hPrint;
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
	case WM_SETTINGCHANGE:
		menu = GetSubMenu(GetMenu(hwnd), 0);//得到第1个字菜单，设备菜单
		while(GetMenuItemCount(menu) > 1)//如果菜单项目数量大于1
			DeleteMenu(menu, 1, MF_BYPOSITION);//删除第2个菜单
		EnumPrinters(PRINTER_ENUM_LOCAL, nullptr, 4, nullptr, 0, &dwNeeded, &dwReturned);//取得需要保存打印机结构大小
		pinfo4 = (PRINTER_INFO_4*) malloc(dwNeeded);//分配空间保存
		EnumPrinters(PRINTER_ENUM_LOCAL, nullptr, 4,(PBYTE) pinfo4, dwNeeded, &dwNeeded, &dwReturned);//将所在打印机信息保存在pInfo4结构中
		for (int i = 0; i < dwReturned; i++)//将每台打印机的名字加到设备菜单中
			AppendMenu(menu,MF_STRING, i + 1, pinfo4[i].pPrinterName);
		free(pinfo4);
		AppendMenu(menu, MF_SEPARATOR, 0, nullptr);//加入分割线
		AppendMenu(menu, MF_STRING, IDM_DEVMODE, TEXT("properties"));//加入属性菜单项
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		menu = GetMenu(hwnd);
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		if (wmId == IDM_SCREEN || wmId < IDM_DEVMODE)
		{
			CheckMenuItem(menu, nCurrentDevice, MF_UNCHECKED);//取消选中上一次选中的
			nCurrentDevice = wmId;
			CheckMenuItem(menu, wmId, MF_CHECKED);//选中当前选的
		}
		else
		{
			if (wmId == IDM_DEVMODE)
			{
				GetMenuString(menu, nCurrentDevice,szDeviceName, sizeof(szDeviceName) / sizeof(TCHAR), MF_BYCOMMAND);//得当菜单名
				if (OpenPrinter(szDeviceName, &hPrint, nullptr))//打开打印机
				{
					PrinterProperties(hwnd, hPrint);//打开打印机属性对话框
					ClosePrinter(hPrint);
				}
			}
			else
			{
				CheckMenuItem(menu, nCurrentInfo, MF_UNCHECKED);
				nCurrentInfo = wmId;
				CheckMenuItem(menu, wmId, MF_CHECKED);
			}
		}
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_INITMENUPOPUP:
		if (lParam==0)//如果是设备菜单弹出
		EnableMenuItem(GetMenu(hwnd), IDM_DEVMODE, (nCurrentDevice == IDM_SCREEN) ? MF_GRAYED : MF_ENABLED);
		break;
	case WM_PAINT://有区域失效，发生重绘
		lstrcpy(szWindowText, TEXT("设备性能:"));
		if (nCurrentDevice == IDM_SCREEN)
		{
			lstrcat(szWindowText, TEXT("屏幕"));
			hdcInfo = CreateIC(TEXT("DISPLAY"), 0, 0, 0);
		}
		else
		{
			GetMenuString(GetMenu(hwnd), nCurrentDevice, szDeviceName, sizeof(szDeviceName) / sizeof(TCHAR), MF_BYCOMMAND);
			lstrcat(szWindowText, szDeviceName);
			hdcInfo = CreateIC(0, szDeviceName, 0, 0);
		}
		SetWindowText(hwnd, szWindowText);
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		if (hdcInfo)
		{
			switch (nCurrentInfo)
			{
			case IDM_BASIC:
				BasicInfo(hdc, hdcInfo, cxChar, cyChar);
				break;
			case IDM_CURVE:
			case IDM_DEVMODE:
			case IDM_LINE:
			case IDM_OTHER:
			case IDM_POLY:
				break;
			}
			DeleteDC(hdcInfo);
		}
		EndPaint(hwnd, &ps);//释放画笔资源
		break;
	case WM_CLOSE://窗口关闭
		DestroyWindow(hwnd);//销毁窗口
		break;
	case WM_DESTROY://窗口销毁时
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
	wndclass.lpszMenuName =MAKEINTRESOURCE(IDR_MENU1);//菜单
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