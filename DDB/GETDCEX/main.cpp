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

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	static HDC hdc, hdcMem;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	static HBITMAP hBitmap;
	static int cxRadius, cyRadius, cxMove, cyMove, cxTotal, cyTotal, cxCenter, cyCenter;
	static HBRUSH hBrush;
	RECT rect;
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		SetTimer(hwnd, 1, 1, 0);
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		cxRadius = cyRadius = min(cxClient, cyClient) / 16;
		cxMove = cyMove = max(1, cxRadius / 2);
		cxTotal = cyTotal = (cxRadius + cxMove) * 2;
		hdc = GetDC(hwnd);
		hdcMem = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, cxTotal, cyTotal);
		SelectObject(hdcMem, hBitmap);
		Rectangle(hdcMem, -1, -1, cxTotal + 1, cyTotal + 1);
		hBrush = CreateHatchBrush(HS_DIAGCROSS, 0);
		SetBkColor(hdcMem, RGB(255, 0, 255));
		SelectObject(hdcMem, hBrush);
		Ellipse(hdcMem, cxMove, cyMove, cxTotal - cxMove, cyTotal - cyMove);
		cxCenter = cxClient / 2;
		cyCenter = cyClient / 2;
		DeleteObject(hBitmap);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_TIMER:
		cxCenter += cxMove;
		cyCenter += cyMove;
		GetClientRect(hwnd, &rect);
		hdc = GetDCEx(hwnd,CreateRectRgnIndirect(&rect),DCX_LOCKWINDOWUPDATE);
		BitBlt(hdc, cxCenter - cxTotal / 2, cyCenter - cyTotal / 2, cxTotal, cyTotal, hdcMem, 0, 0, SRCCOPY);
		if (cxCenter + cxRadius >= cxClient || cxCenter - cxRadius <= 0)
			cxMove = -cxMove;
		if (cyCenter + cyRadius >= cyClient || cyCenter - cyRadius <= 0)
			cyMove = -cyMove;
		ReleaseDC(hwnd, hdc);
		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作

		break;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		BitBlt(hdc, cxCenter - cxTotal / 2, cyCenter - cyTotal / 2, cxTotal, cyTotal, hdcMem, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);//释放画笔资源
		break;
	case WM_CLOSE://窗口关闭
		DestroyWindow(hwnd);//销毁窗口
		break;
	case WM_DESTROY://窗口销毁时
		DeleteDC(hdcMem);
		DeleteObject(hBrush);
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
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("getdcex"), WS_OVERLAPPEDWINDOW,
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