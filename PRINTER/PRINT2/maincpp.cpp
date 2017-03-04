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
HWND hDlgModeLess=0;
bool bAbort = false;
void PageGDICalls(HDC hdc, int cxPage, int cyPage)
{
	static TCHAR szText[] = TEXT("Hello ,Printer");
	Rectangle(hdc, 0, 0, cxPage, cyPage);
	MoveToEx(hdc, 0, 0, nullptr);
	LineTo(hdc, cxPage, cyPage);
	MoveToEx(hdc, cxPage, 0, nullptr);
	LineTo(hdc, 0, cyPage);
	SaveDC(hdc);
	SetMapMode(hdc, MM_ISOTROPIC);
	SetWindowExtEx(hdc, 1000, 1000, nullptr);
	SetViewportExtEx(hdc, cxPage / 2, -cyPage / 2, 0);
	SetViewportOrgEx(hdc, cxPage / 2, cyPage / 2, 0);
	Ellipse(hdc, -500, 500, 500, -500);
	SetTextAlign(hdc, TA_CENTER);
	TextOut(hdc, 0, 0, szText, lstrlen(szText));
	RestoreDC(hdc, -1);
}
HDC GetPrinterDC()
{
	HDC hdc;
	unsigned long dwNeeded, dwReturned;
	PRINTER_INFO_4 *pinfo4;
	EnumPrinters(PRINTER_ENUM_LOCAL, 0, 4, 0, 0, &dwNeeded, &dwReturned);
	pinfo4 = (PRINTER_INFO_4*)malloc(dwNeeded);
	EnumPrinters(PRINTER_ENUM_LOCAL, 0, 4, (PBYTE)pinfo4, dwNeeded, &dwNeeded, &dwReturned);
	hdc = CreateDC(0, pinfo4[0].pPrinterName, 0, 0);
	free(pinfo4);
	return hdc;
}
int _stdcall AbortProc(HDC hdc,int iCode)
{
	MSG msg;
	while(!bAbort&& PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (!hDlgModeLess || !IsDialogMessage(hDlgModeLess,&msg))
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Sleep(8000);

	MessageBox(0, TEXT("PRINT PAGE "), 0, 0);
	return !bAbort;
}
int _stdcall DlgProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)
{
	TCHAR szCaption[128];
	switch (message)
	{

	case WM_INITDIALOG:
		GetWindowText(GetParent(hDlg), szCaption, 128);
		SetWindowText(hDlg, szCaption);
		EnableMenuItem(GetSystemMenu(hDlg, false), SC_CLOSE, MF_GRAYED);
		return true;
	case WM_COMMAND:
		bAbort = true;
		EnableWindow(GetParent(hDlg), true);
		DestroyWindow(hDlg);
		hDlgModeLess = 0;
		return true;
	}
	return false;
}
bool MyPrintPage(HWND hwnd,HINSTANCE hInst)
{
	hDlgModeLess = CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DlgProc);

	HDC hdc = GetPrinterDC();
	if (!hdc)
		return false;
	static DOCINFO di;
	memset(&di, 0, sizeof(di));
	di.cbSize = sizeof(di);
	di.lpszDocName = TEXT("PRINT2:");
	int xPage = GetDeviceCaps(hdc, HORZRES);
	int yPage = GetDeviceCaps(hdc, VERTRES);
	EnableWindow(hwnd, false);
	SetAbortProc(hdc, AbortProc);

	if (StartDoc(hdc, &di))
	{
		if (StartPage(hdc))
		{
			PageGDICalls(hdc, xPage, yPage);
			if (EndPage(hdc))
				EndDoc(hdc);
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
	EnableWindow(hwnd, true);
	DeleteDC(hdc);
	return true;
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	HMENU hMenu;
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		hMenu = GetSystemMenu(hwnd, false);
		AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
		AppendMenu(hMenu, MF_STRING, 1, TEXT("&Print"));
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		break;
	case WM_SYSCOMMAND:
		if (wParam == 1)
			if (!MyPrintPage(hwnd,hInst))
				MessageBox(hwnd, TEXT("can not Print page"), 0, 0 | MB_ICONEXCLAMATION);
		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作

		break;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		PageGDICalls(hdc, cxClient, cyClient);
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