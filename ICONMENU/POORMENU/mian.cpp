//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
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
const int ID_SYS_HELP = 1;
const int ID_SYS_ABOUT = 2;
const int ID_SYS_REMOVE = 3;
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
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		cxIcon = GetSystemMetrics(SM_CXICON);//得到系统图标宽度、高度
		cyIcon = GetSystemMetrics(SM_CYICON);
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//得到窗口实例名柄
		icon=LoadIcon(hInst, TEXT("IDI_ICON"));
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		break;
	case WM_SYSCOMMAND://在系统菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		switch (wmId)
		{
		case ID_SYS_ABOUT:
			MessageBox(nullptr, TEXT("Menu Program\n(c)yong hua by,2015"), TEXT("关于Menu"), MB_ICONINFORMATION | MB_OK);

			break;
		case ID_SYS_HELP:
			MessageBox(nullptr, TEXT("Help not yet implement"), TEXT("help and support"), MB_OK);

			break;
		case ID_SYS_REMOVE:
			GetSystemMenu(hwnd, true);//第2个参数设为true会将增加的系统菜单去掉
			break;
		}
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
	wndclass.hCursor = LoadCursor(hInstance,TEXT("Cursor"));//鼠标光标
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
	HMENU hMenu = GetSystemMenu(hwnd, false);//得到系统菜单，第2 个参数设为false表示修改系统菜单
	AppendMenu(hMenu, MF_STRING,ID_SYS_ABOUT,TEXT("About...") );//增加1个菜单，MF_STRING：指定菜单项是一个正文字符串；参数lpNewltem指向该字符串。
	AppendMenu(hMenu, MF_STRING, ID_SYS_HELP, TEXT("Help..."));
	AppendMenu(hMenu, MF_STRING, ID_SYS_REMOVE, TEXT("Remove Additions"));
	//HWND hDestop = GetDesktopWindow();//得到桌面窗口句柄
	//HMENU hSysMenu = GetSystemMenu(hDestop, false);//得到系统菜单，第2 个参数设为false表示修改系统菜单
	//AppendMenu(hSysMenu, MF_STRING, 4, TEXT("About..."));//增加1个菜单，MF_STRING：指定菜单项是一个正文字符串；参数lpNewltem指向该字符串。
	//AppendMenu(hSysMenu, MF_STRING, 5, TEXT("Help..."));
	//AppendMenu(hSysMenu, MF_STRING, 6, TEXT("Remove Additions"));

	ShowWindow(hwnd, nCmdShow);//显示窗口
	UpdateWindow(hwnd);//进行重绘
	MSG msg;//消息结构
	while (GetMessage(&msg, nullptr, 0, 0))//从消息队列获取消息
	{
		TranslateMessage(&msg);//将一些按键消息转换成字符消息放在按键消息后面
		DispatchMessage(&msg);//分发消息，即调用窗口过程进行处理消息
	}
}