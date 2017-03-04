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
HWND hDialogModeLess;//非模态对话框

int _stdcall DialogProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)//对话框窗口过程
{
	int wmId = LOWORD(wParam);//控件ID
	int wmEvent = HIWORD(wParam);//控件通知码，则操作
	static int iColor[3];
	int index;//索引
	HWND hCtrl;
	HWND hParent;//父窗口句柄
	switch (message)
	{
	//case WM_COMMAND:

	//	return false;
	case WM_INITDIALOG:
		for (int id = IDC_SCROLLRED; id <= IDC_SCROLLBLUE; id++)//3个滚动条ID连续，却IDC_SCROLLRED最小
		{
			hCtrl = GetDlgItem(hDlg, id);//获得当前滚动条句柄
			SetScrollRange(hCtrl, SB_CTL,0, 255, false);//设置滚动条范围
			SetScrollPos(hCtrl, SB_CTL, 0, false);//设置滚动条位置
		}
		return false;//让第1个TABSTOP获得焦点
	case WM_VSCROLL:
		hCtrl = (HWND)lParam;
		hParent = GetParent(hDlg);
		index = GetDlgCtrlID(hCtrl)-IDC_SCROLLRED;//3个滚动条ID连续，却IDC_SCROLLRED最小
		switch (wmId)
		{
		case SB_PAGEDOWN:
			iColor[index] += 15;
		case SB_LINEDOWN:
			iColor[index] = min(255, iColor[index] + 1);
			break;
		case SB_PAGEUP:
			iColor[index] -= 15;
		case SB_LINEUP:
			iColor[index] = max(0, iColor[index] -1);
			break;
		case SB_TOP:
			iColor[index] = 0;
			break;
		case SB_BOTTOM:
			iColor[index] = 255;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			iColor[index] = wmEvent;
			break;
		}
		SetScrollPos((HWND)lParam, SB_CTL, iColor[index], true);
		SetDlgItemInt(hDlg, GetWindowLong(hCtrl, GWL_ID)+3, iColor[index], true);//下面的颜色值ID比相应滚动条ID大3
		DeleteObject((HGDIOBJ)SetClassLong(hParent, GCL_HBRBACKGROUND,(long)CreateSolidBrush(RGB(iColor[0], iColor[1], iColor[2]))));
		InvalidateRect(hParent,nullptr,true);
		return true;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return false;
	}
	return false;
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		hDialogModeLess = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DialogProc);//在showwindow()后创建能让对话框获得焦点
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		SetFocus(hDialogModeLess);
		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作

		break;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔

		EndPaint(hwnd, &ps);//释放画笔资源
		break;
	case WM_CLOSE://窗口关闭
		DestroyWindow(hwnd);//销毁窗口
		break;
	case WM_DESTROY://窗口销毁时
		DeleteObject((HGDIOBJ)SetClassLong(hwnd, GCL_HBRBACKGROUND, (long)GetStockObject(WHITE_BRUSH)));
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
		if (hDialogModeLess == 0 || !IsDialogMessage(hDialogModeLess, &msg))//如果无此语句，按TAB键不会在控件间切换焦点
		{
			TranslateMessage(&msg);//将一些按键消息转换成字符消息放在按键消息后面
			DispatchMessage(&msg);//分发消息，即调用窗口过程进行处理消息
		}
	}
}