//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<process.h>
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
# define WM_CALC_DONE (WM_USER+1)
# define WM_CALC_ABORT (WM_USER+2)
typedef struct
{
	HWND hwnd;
	int bCont;
	HANDLE hEvent;
}PARAMS;
	const int repeat = 5000;

void MyThread(PVOID pvoid)
{
	volatile PARAMS *param = (PARAMS*)pvoid;
	double test = 1.0;
	int sTime = GetCurrentTime();
	int i;
	while (true)
	{
		WaitForSingleObject(param->hEvent,INFINITE);
		for ( i= 0; i < repeat&&param->bCont; i++)
		{
			test=atan(tan(exp(log(sqrt(test*test)))))+1.0;
			Sleep(1);
		}
		if (i == repeat)
		{
			int lTime = GetCurrentTime() - sTime;
			SendMessage(param->hwnd, WM_CALC_DONE, 0, lTime);
		}
		else
			SendMessage(param->hwnd, WM_CALC_ABORT, 0,0);
	}
	_endthread();
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	static PARAMS param;
	static TCHAR *szMessage[] = {TEXT("准备(鼠标左键按下开始计算)"),TEXT("计算中（鼠标右键按下停止计算)"),TEXT("重复%d次花费%d msec")};
	static int iState;
	TCHAR szBuffer[128];
	const int ready = 0, working = 1, done = 2;
	static int lTime;
	RECT rect;
	static HANDLE hEvent;
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		hEvent = CreateEvent(0, 0, 0, 0);
		param.hwnd = hwnd;
		param.bCont = false;
		param.hEvent = hEvent;
		_beginthread(MyThread, 0, &param);

		break;
	case WM_LBUTTONDOWN:
		if (iState == working)
		{
			MessageBeep(0);
			return 0;
		}

		iState = working;
		param.bCont = true;
		SetEvent(hEvent);
		InvalidateRect(hwnd, 0, true);
		break;
	case WM_RBUTTONDOWN:
		param.bCont = false;
		InvalidateRect(hwnd, 0, true);
		break;
	case WM_CALC_ABORT:
		iState = ready;
		InvalidateRect(hwnd, 0, true);
		break;
	case WM_CALC_DONE:
		lTime = lParam;
		iState = done;
		InvalidateRect(hwnd, 0, true);
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作

		break;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		wsprintf(szBuffer, szMessage[iState - ready], repeat, lTime);
		GetClientRect(hwnd, &rect);
		DrawText(hdc, szBuffer, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
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