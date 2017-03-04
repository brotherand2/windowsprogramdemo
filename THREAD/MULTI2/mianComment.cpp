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
const int ID_TIMER = 1;
int cyChar;
typedef struct
{
	int cxClinet;
	int cyClient;
	HWND hwnd;
	int cyChar;
	bool bKill;
}PARAMS;
void CheckBottom(HWND hwnd, int cyClient, int &iLine)
{
	if (iLine*cyChar + cyChar > cyClient)
	{
		iLine = 0;
		InvalidateRect(hwnd, 0, true);
	}
}
void Thread1(PVOID pvoid)
{
	HDC hdc;
	static int iNum = 0, iLine = -1;
	TCHAR szBuffer[32];
	PARAMS *param = (PARAMS*)pvoid;
	while (!param->bKill)
	{
		hdc = GetDC(param->hwnd);
		iLine++;
		iNum++;
		if (iNum < 0)
			iNum = 0;
		CheckBottom(param->hwnd, param->cyClient, iLine);
		TextOut(hdc, 0, iLine*cyChar, szBuffer, wsprintf(szBuffer, TEXT("%d"), iNum));
		ReleaseDC(param->hwnd, hdc);
	}
	_endthread();
}
void Thread2(PVOID pvoid)
{
	HDC hdc;
	static int iNum = 1, iLine = -1;
	TCHAR  szBuffer[32];
	int iSqrt;
	int i = 0;

	PARAMS *param = (PARAMS*)pvoid;
	while (!param->bKill)
	{
		hdc = GetDC(param->hwnd);
		iLine++;
		CheckBottom(param->hwnd, param->cyClient, iLine);
		if (iNum < 0)
			iNum = 0;
		do
		{
			iNum++;
			iSqrt = (int)sqrt(iNum);
			for (i = 2; i <= iSqrt; i++)
				if (iNum%i == 0)
					break;
		} while (i <= iSqrt);
		TextOut(hdc, 0, iLine*cyChar, szBuffer, wsprintf(szBuffer, TEXT("%d"), iNum));
		ReleaseDC(param->hwnd, hdc);

	}
	_endthread();
}
void Thread3(PVOID pvoid)
{
	HDC hdc;
	static int iNum = 0, next = 1, temp, iLine = -1;
	TCHAR  szBuffer[32];

	PARAMS *param = (PARAMS*)pvoid;
	while (!param->bKill)
	{
		hdc = GetDC(param->hwnd);
		iLine++;
		if (iNum < 0)
		{
			iNum = 0;
			next = 1;
		}
		CheckBottom(param->hwnd, param->cyClient, iLine);
		TextOut(hdc, 0, iLine*cyChar, szBuffer, wsprintf(szBuffer, TEXT("%d"), iNum));
		temp = next;
		next = iNum + next;
		iNum = temp;
		ReleaseDC(param->hwnd, hdc);

	}
	_endthread();

}
void Thread4(PVOID pvoid)
{
	HDC hdc;
	static int iNum = 0, iLine = 0;

	PARAMS *param = (PARAMS*)pvoid;
	while (!param->bKill)
	{
		hdc = GetDC(param->hwnd);
		InvalidateRect(param->hwnd, 0, true);
		UpdateWindow(param->hwnd);
		iNum = rand() % (max(1, min(param->cxClinet, param->cyClient)));
		Ellipse(hdc, (param->cxClinet - iNum) / 2, (param->cyClient - iNum) / 2, (param->cxClinet + iNum) / 2, (param->cyClient + iNum) / 2);
		ReleaseDC(param->hwnd, hdc);

	}
	_endthread();

}

long _stdcall Child1Proc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	static int cyClient,cxClient;
	static PARAMS param;
	switch (UMsg)
	{
	case WM_CREATE:
		param.bKill = false;
		param.hwnd = hwnd;
		param.cyChar =HIWORD( GetDialogBaseUnits());
		_beginthread(Thread1, 0, &param);
	case WM_SIZE:
		param.cxClinet = LOWORD(lParam);
		param.cyClient = HIWORD(lParam);
		break;
	case WM_TIMER:
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, UMsg, wParam, lParam);//用默认的窗口过程处理其它消息，做些其它处理
}
long _stdcall Child2Proc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	static int cyClient, cxClient;
	static PARAMS param;
	switch (UMsg)
	{
	case WM_CREATE:
		param.bKill = false;
		param.hwnd = hwnd;
		param.cyChar = HIWORD(GetDialogBaseUnits());
		_beginthread(Thread2, 0, &param);
	case WM_SIZE:
		param.cxClinet = LOWORD(lParam);
		param.cyClient = HIWORD(lParam);
		break;
	case WM_TIMER:
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//用默认的窗口过程处理其它消息，做些其它处理
}
long _stdcall Child3Proc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	static int cyClient, cxClient;
	static PARAMS param;
	switch (UMsg)
	{
	case WM_CREATE:
		param.bKill = false;
		param.hwnd = hwnd;
		param.cyChar = HIWORD(GetDialogBaseUnits());
		_beginthread(Thread3, 0, &param);
	case WM_SIZE:
		param.cxClinet = LOWORD(lParam);
		param.cyClient = HIWORD(lParam);
		break;
	case WM_TIMER:
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//用默认的窗口过程处理其它消息，做些其它处理
}
long _stdcall Child4Proc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	static int cyClient, cxClient;
	static PARAMS param;
	switch (UMsg)
	{
	case WM_CREATE:
		param.bKill = false;
		param.hwnd = hwnd;
		param.cyChar = HIWORD(GetDialogBaseUnits());
		_beginthread(Thread4, 0, &param);
	case WM_SIZE:
		param.cxClinet = LOWORD(lParam);
		param.cyClient = HIWORD(lParam);
		break;
	case WM_TIMER:
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//用默认的窗口过程处理其它消息，做些其它处理
}

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	static HWND hwndChild[4];
	WNDPROC proc[4]{Child1Proc,Child2Proc,Child3Proc,Child4Proc };
	static TCHAR *szClass[] = {TEXT("child1"),TEXT("child2"),TEXT("child3"),TEXT("child4")};
	WNDCLASS wndclass;
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.hCursor = 0;
		wndclass.hIcon = 0;
		wndclass.hInstance = hInst;
		wndclass.lpszMenuName = 0;
		wndclass.style = CS_VREDRAW | CS_HREDRAW;
		for (int i = 0; i < 4; i++)
		{
			wndclass.lpfnWndProc = proc[i];
			wndclass.lpszClassName = szClass[i];
			RegisterClass(&wndclass);
			hwndChild[i] = CreateWindow(szClass[i], 0, WS_CHILDWINDOW|WS_BORDER|WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)i, hInst, 0);
		}
		break;
	case WM_CHAR:
		if (wParam == '\x1B')
			DestroyWindow(hwnd);
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		for (int i = 0; i < 4; i++)
			MoveWindow(hwndChild[i], (i % 2) ? cxClient / 2 :0 , i <2 ? 0 : cyClient / 2, cxClient / 2, cyClient / 2,true);
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
		KillTimer(hwnd, ID_TIMER);
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