//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include"resources.h"
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
	HINSTANCE hInst;
	static int is_Selected = ID_BKG_WHITE;//当前选中的是白画刷
	int idColor[5] = { WHITE_BRUSH, GRAY_BRUSH, LTGRAY_BRUSH, DKGRAY_BRUSH, BLACK_BRUSH };//五个系统画刷
	POINT pt;//鼠标坐标
	static HMENU menu;//系统菜单
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//得到窗口实例名柄
		menu = LoadMenu(hInst,TEXT("POPUPMENU"));//从资源文件读取菜单
		menu = GetSubMenu(menu, 0);//得到子菜单
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		cxIcon = GetSystemMetrics(SM_CXICON);//得到系统图标宽度、高度
		cyIcon = GetSystemMetrics(SM_CYICON);
		icon = LoadIcon(hInst, TEXT("IDI_ICON"));
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		break;
	case WM_RBUTTONDOWN://鼠标右键单击
		pt.x = LOWORD(lParam);//WM_MBUTTONUP,lParam参数的低位是鼠标的客户区x坐标，高位是客户区y坐标
		pt.y = HIWORD(lParam);
		ClientToScreen(hwnd, &pt);//将客户区坐标转换为屏幕坐标
		TrackPopupMenu(menu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, nullptr);//弹出菜单
		//TPM_RIGHTBUTTON：若设置此标志，用户能用鼠标右键选择菜单项
		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		switch (wmId)
		{
		case ID_FILE_NEW:
		case ID_FILE_OPEN:
		case ID_FILE_SAVE:
		case ID_FILE_SAVEAS:
			MessageBeep(0);
			break;
		case ID_APP_EXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case ID_EDIT_COPY:
		case ID_EDIT_CUT:
		case ID_EDIT_DELETE:
		case ID_EDIT_PASTE:
		case ID_EDIT_UNDO:
			MessageBeep(0);
			break;
		case ID_BKG_BLACK:
		case ID_BKG_DARKGRAY:
		case ID_BKG_GRAY:
		case ID_BKG_LIGHTGRAY:
		case ID_BKG_WHITE:
			CheckMenuItem(menu, is_Selected, MF_UNCHECKED);//取消原先的先中状态
			is_Selected = wmId;
			CheckMenuItem(menu, is_Selected, MF_CHECKED);//勾选当前选中
			SetClassLong(hwnd, GCL_HBRBACKGROUND, (long)GetStockObject(idColor[wmId - ID_BKG_WHITE]));//设置选中颜色为背景色
			InvalidateRect(hwnd, nullptr, true);//重绘
			break;
		case ID_TIMER_BEGIN:
			EnableMenuItem(menu, ID_TIMER_BEGIN, MF_GRAYED);//让开始菜单不可用
			EnableMenuItem(menu, ID_TIMER_END, MF_ENABLED);//让结菜单可用
			SetTimer(hwnd, 1, 1000, nullptr);//开始计时
			break;
		case ID_TIMER_END:
			EnableMenuItem(menu, ID_TIMER_BEGIN, MF_ENABLED);//让开始菜单可用
			EnableMenuItem(menu, ID_TIMER_END, MF_GRAYED);//让结菜单不可用
			KillTimer(hwnd, 1);//关闭计时
			break;
		case ID_APP_HELP:
			MessageBox(nullptr, TEXT("Help not yet implement"),TEXT("help and support"),MB_OK);
			break;
		case ID_APP_ABOUT:
			MessageBox(nullptr, TEXT("Menu Program\n(c)yong hua by,2015"), TEXT("关于Menu"), MB_ICONINFORMATION | MB_OK);
		}
		break;
	case WM_TIMER://计时到了
		MessageBeep(0);//响铃一次
		break;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		for (int i = 0; i < cxClient; i += cxIcon)//在客户画上图标
		{
			for (int j = 0; j < cyClient; j += cyIcon)
				DrawIcon(hdc, i, j, icon);
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
	wndclass.hIcon = LoadIcon(hInstance, TEXT("IDI_ICON"));//图标
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