//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
# define IDS_APP_NAME 1
#define IDS_APP_CAPTION 2
# define ID_USERNAME_EDIT 3
# define ID_USERPASSWORD_EDIT 4
# define ID_USERNAME_LABEL 5
# define ID_USERPASSWORD_LABEL 6
# define ID_BTN_LOGIN 7
# define ID_BTN_CANCEL 8
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
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	static HWND hUserNameEdit;//用户名编辑框
	static HWND hUserPasswordEdit;//密码编辑框
	static HWND hUserNameLabel;//用户名标签
	static HWND hUserPasswordLabel;//密码标签
	static HWND hBtnLogin;//登录按钮
	static HWND hBtnCancel;//取消按钮

	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		hUserNameEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE|WS_BORDER, 0, 0, 0, 0,
			hwnd, (HMENU)ID_USERNAME_EDIT, hInst, nullptr);
		hUserPasswordEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE|WS_BORDER|ES_PASSWORD, 0, 0, 0, 0,
			hwnd, (HMENU)ID_USERPASSWORD_EDIT, hInst, nullptr);
		hUserNameLabel = CreateWindow(TEXT("static"), TEXT("请输入用户名"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
			hwnd, (HMENU)ID_USERNAME_LABEL, hInst, nullptr);
		hUserPasswordLabel = CreateWindow(TEXT("static"), TEXT("请输入密码"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
			hwnd, (HMENU)ID_USERPASSWORD_LABEL, hInst, nullptr);
		hBtnLogin= CreateWindow(TEXT("button"), TEXT("登录"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
			hwnd, (HMENU)ID_BTN_LOGIN, hInst, nullptr);
		hBtnCancel = CreateWindow(TEXT("button"), TEXT("取消"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
			hwnd, (HMENU)ID_BTN_CANCEL, hInst, nullptr);
		cxChar = HIWORD(GetDialogBaseUnits());//将对话框中字母的高度作为控件的一个水平单元
		cyChar = LOWORD(GetDialogBaseUnits());//将对话框中字母的宽度作为控件的一个垂直单元

		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		MoveWindow(hUserNameLabel, 3 * cxChar, 3 * cyChar, 20 * cxChar, 3 * cyChar, true);
		MoveWindow(hUserNameEdit, 3 * cxChar,6 * cyChar, 20 * cxChar, 3 * cyChar, true);
		MoveWindow(hUserPasswordLabel, 3 * cxChar,9 * cyChar, 20 * cxChar, 3 * cyChar, true);
		MoveWindow(hUserPasswordEdit, 3 * cxChar, 12 * cyChar, 20 * cxChar, 3 * cyChar, true);
		MoveWindow(hBtnLogin, 3 * cxChar, 16 * cyChar,6 * cxChar, 3 * cyChar, true);
		MoveWindow(hBtnCancel, 10 * cxChar,16 * cyChar, 6 * cxChar, 3 * cyChar, true);
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
		PostQuitMessage(0);//抛出一条返回值为0退出消息
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//用默认的窗口过程处理其它消息，做些其它处理
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	TCHAR szAppName[64], szCaptionName[64];
	LoadString(hInstance,IDS_APP_NAME ,szAppName,64 );
	LoadString(hInstance, IDS_APP_CAPTION, szCaptionName, 64);
	WNDCLASS wndclass;//窗口类
	wndclass.cbClsExtra = 0;//为类分配的额外空间，可以保存一些地址、变量
	wndclass.cbWndExtra = 0;//为窗口分配的额外空间，可以保存一些地址、变量
	wndclass.hbrBackground =CreateSolidBrush(GetSysColor(COLOR_BTNFACE));//背景画刷
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//鼠标光标
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);//图标
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
	int cxChar = HIWORD(GetDialogBaseUnits());//将对话框中字母的高度作为控件的一个水平单元
	int cyChar = LOWORD(GetDialogBaseUnits());//将对话框中字母的宽度作为控件的一个垂直单元

	HWND hwnd = CreateWindow(szAppName, szCaptionName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,25*cxChar,25*cyChar,
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