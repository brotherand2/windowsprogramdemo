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
const int ID_EDIT_LEFT = 1;
const int ID_EDIT_RIGHT = 2;
const int ID_BTN_CLEAR = 3;
const int ID_BTN_COPY = 4;
const int ID_BTN_PASTE = 5;
const int ID_BTN_SETTEXT = 6;
const int ID_BTN_GETTEXT = 7;

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	static HWND hEditLeft, hEditRight, hBtnClear, hBtnPaste, hBtnCopy, hBtnGetText, hBtnSetText;
	TCHAR szBuffer[1024];
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		hEditLeft=CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("edit"), nullptr, WS_VISIBLE | WS_CHILD|ES_MULTILINE|WS_HSCROLL|WS_VSCROLL, 0, 0, 0, 0,
			hwnd, (HMENU)ID_EDIT_LEFT, hInst, nullptr);
		hEditRight = CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("edit"),nullptr,WS_VISIBLE | WS_CHILD | ES_MULTILINE|WS_HSCROLL|WS_VSCROLL, 0, 0, 0, 0,
			hwnd, (HMENU)ID_EDIT_RIGHT, hInst, nullptr);
		hBtnClear=CreateWindow(TEXT("button"), TEXT("清除文字"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0,
			hwnd, (HMENU)ID_BTN_CLEAR,hInst, nullptr);
		hBtnCopy=CreateWindow(TEXT("button"), TEXT("复制文字"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0,
			hwnd, (HMENU)ID_BTN_COPY, hInst, nullptr);
		hBtnGetText=CreateWindow(TEXT("button"), TEXT("得到文字"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0,
			hwnd, (HMENU)ID_BTN_GETTEXT, hInst, nullptr);
		hBtnPaste=CreateWindow(TEXT("button"), TEXT("粘贴文字"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0,
			hwnd, (HMENU)ID_BTN_PASTE, hInst, nullptr);
		hBtnSetText=CreateWindow(TEXT("button"), TEXT("重置文字"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0,
			hwnd, (HMENU)ID_BTN_SETTEXT, hInst, nullptr);

		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		MoveWindow(hEditLeft,8*cxChar,4*cyChar,30*cxChar ,10*cyChar , true);
		MoveWindow(hEditRight,50*cxChar ,4*cyChar ,30*cxChar ,10*cyChar, true);
		MoveWindow(hBtnClear,8*cxChar ,17*cyChar ,10*cxChar ,2*cyChar, true);
		MoveWindow(hBtnCopy,8*cxChar ,19*cyChar+4,10*cxChar ,2*cyChar, true);
		MoveWindow(hBtnSetText,8*cxChar ,21*cyChar+8 ,10*cxChar ,2*cyChar, true);
		MoveWindow(hBtnPaste,50 * cxChar, 19 * cyChar + 4, 10 * cxChar, 2 * cyChar, true);
		MoveWindow(hBtnGetText,50 * cxChar, 21 * cyChar + 8, 10 * cxChar, 2 * cyChar, true);

		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		switch (wmId)
		{
		case ID_EDIT_LEFT://左编辑框
			switch (wmEvent)
			{
			case EN_UPDATE:
				GetWindowText(hEditLeft, szBuffer, 1024);
				SetWindowText(hEditRight, szBuffer);
				break;
			}
			return 0;
		case ID_EDIT_RIGHT://右编辑框
			return 0;
		case ID_BTN_CLEAR://清除按钮
			SendMessage(hEditLeft, WM_CLEAR, 0, 0);//清除选中的

			return 0;
		case ID_BTN_COPY://复制按钮
			SendMessage(hEditLeft, WM_COPY, 0, 0);//清除选中的

			return 0;
		case ID_BTN_GETTEXT://获得文本
			GetWindowText(hEditLeft, szBuffer, 1024);
			SetWindowText(hEditRight, szBuffer);
			return 0;
		case ID_BTN_PASTE://粘贴
			SendMessage(hEditRight, WM_PASTE, 0, 0);//清除选中的

			return 0;
		case ID_BTN_SETTEXT://重置文本
			SetWindowText(hEditLeft, TEXT("This is a line"));

			return 0;
		}
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
	WNDCLASS wndclass;//窗口类
	wndclass.cbClsExtra = 0;//为类分配的额外空间，可以保存一些地址、变量
	wndclass.cbWndExtra = 0;//为窗口分配的额外空间，可以保存一些地址、变量
	wndclass.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));//背景画刷
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