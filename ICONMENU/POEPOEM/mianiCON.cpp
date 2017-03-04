//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<cmath>
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

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	static int cxIcon, cyIcon;//系统图标宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HICON icon;//图标句柄
	static HINSTANCE hInst;
	TCHAR szError[64],szBuffer[64];
	static TCHAR *text;//指向读取的文本
	TCHAR *temp;
	HRSRC src;
	HGLOBAL hGlobal;
	static int lines;//行数
	static HWND hScroll;//滚动条控件
	static int xScroll;//系统滚动条宽度
	static int iVScrollPos;//垂直滚动条位置
	RECT rect;//保存要重绘的区域
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		cxIcon = GetSystemMetrics(SM_CXICON);//得到系统图标宽度、高度
		cyIcon = GetSystemMetrics(SM_CYICON);
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//得到窗口实例名柄
		icon=LoadIcon(hInst, TEXT("IDI_ICON"));
		 src=FindResource(hInst, TEXT("POEPOEMTEXT"), TEXT("TEXT"));//找到自定义TEXT类型的资源
		hGlobal= LoadResource(hInst, src);//加载资源到内存
		temp=text = (char *)LockResource(hGlobal);//锁定资源
		while (*temp != '\0')
		{
			if (*temp == '\n')//到了行尾
				lines++;//行数加1
			++temp;//指向下一个字符
		}
		hScroll = CreateWindow(TEXT("scrollbar"), TEXT(""), WS_CHILD | WS_VISIBLE | SBS_VERT, 0, 0, 0, 0,
			hwnd, (HMENU)1, hInst, nullptr);
		SetScrollRange(hScroll, SB_CTL, 0, lines - 1, false);
		SetScrollPos(hScroll, SB_CTL, 0, false);
		xScroll = GetSystemMetrics(SM_CXVSCROLL);////得到系统垂直滚动条宽度
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		MoveWindow(hScroll, cxClient - xScroll, 0, xScroll, cyClient, true);//设置滚动条大小位置
		break;
	case WM_VSCROLL://垂直滚动消息
		switch (LOWORD(wParam))//滚动条上操作
		{
		case SB_LINEUP://向上移动
			iVScrollPos -= 1;
			break;
		case SB_LINEDOWN://向下移动
			iVScrollPos += 1;
			break;
		case SB_PAGEUP://向上翻页
			iVScrollPos -= cyClient / cyChar;
			break;
		case SB_PAGEDOWN://向下翻页
			iVScrollPos += cyClient / cyChar;
			break;
		case SB_THUMBTRACK://拖动 
			iVScrollPos = HIWORD(wParam);
			break;
		case SB_TOP://回到顶部
			iVScrollPos = 0;
			break;
		case SB_BOTTOM://回到底部
			iVScrollPos = lines - 1;
			break;
		}
		iVScrollPos = max(0, min(iVScrollPos, lines - 1));//让滚动条不超出范围
		if (iVScrollPos != GetScrollPos(hScroll, SB_CTL))//滚动条位置变化了
		{
			SetScrollPos(hScroll, SB_CTL, iVScrollPos, true);//设置滚动条新位置
			InvalidateRect(hwnd, nullptr, true);//重绘客户区，更新文字
		}
		break;

	case WM_SETFOCUS://当窗口获得焦点时
		SetFocus(hScroll);//让滚动条获得焦点
		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		break;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		rect = ps.rcPaint;
		rect.top -= cyChar*iVScrollPos;//将区域向上滚动iVScrollPos行，则第0到iVScrollPos-1行在客户区外
		DrawText(hdc, text, -1, &rect, DT_WORDBREAK);
		EndPaint(hwnd, &ps);//释放画笔资源
		break;
	case WM_CLOSE://窗口关闭
		LoadString(hInst, IDS_ERROR, szError, sizeof(szBuffer)/sizeof(TCHAR));
		GetWindowText(hwnd, szBuffer, 64);
		MessageBox(nullptr, szError, szBuffer, MB_ICONWARNING);
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
	TCHAR szAppName[64], szCaptionName[64];
	LoadString(hInstance, IDS_APPNAME, szAppName, sizeof(szAppName)/sizeof(TCHAR));//从资源文件读取字符串，作为程序名字
	LoadString(hInstance, IDS_CAPTIONNAME, szCaptionName, sizeof(szCaptionName) / sizeof(TCHAR));//从资源文件读取字符串，作为窗口标题名字
	WNDCLASS wndclass;//窗口类
	wndclass.cbClsExtra = 0;//为类分配的额外空间，可以保存一些地址、变量
	wndclass.cbWndExtra = 0;//为窗口分配的额外空间，可以保存一些地址、变量
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//背景画刷
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//鼠标光标
	wndclass.hIcon = LoadIcon(hInstance, TEXT("IDI_ICON"));//图标
	wndclass.hInstance = hInstance;//实例名柄，程序的PID，一个整数，根据这个整数，系统可以找到进程的地址
	wndclass.lpfnWndProc = WndProc;//窗口过程
	wndclass.lpszClassName = szAppName;//类名
	wndclass.lpszMenuName = nullptr;//菜单
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//哪些改变后要重画，设置水平和垂直改变后重绘
	if (!RegisterClass(&wndclass))//注册窗口类
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(szAppName,szCaptionName, WS_OVERLAPPEDWINDOW,
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