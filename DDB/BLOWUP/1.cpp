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
void InvertBlock(HWND hwndSrc,HWND hwnd, POINT ptBeg, POINT ptEnd)
{
	HDC hdcScr = GetDCEx(hwndSrc, 0,DCX_CACHE| DCX_LOCKWINDOWUPDATE);
	PatBlt(hdcScr, ptBeg.x, ptBeg.y, ptEnd.x-ptBeg.x, ptEnd.y-ptBeg.y, DSTINVERT);
	ReleaseDC(hwndSrc, hdcScr);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	HDC hdcMem;
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	static POINT ptBeg, ptEnd;
	static bool bCapture=false, bBlock=false;
	static HWND hDestop;
	static HBITMAP hBitmap;
	BITMAP bitmap;
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		hDestop = GetDesktopWindow();
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		if (!bCapture)
		{
			if (LockWindowUpdate(hDestop))
			{
				bCapture = true;
				SetCapture(hwnd);
				SetCursor(LoadCursor(0, IDC_CROSS));
			}
			else
				MessageBeep(0);
		}
		break;
	case WM_RBUTTONDOWN:
		if (bCapture)
		{
			bBlock = true;
			ptBeg.x = LOWORD(lParam);
			ptBeg.y = HIWORD(lParam);
			ClientToScreen(hwnd, &ptBeg);
			ptEnd = ptBeg;
		}
		break;
	case WM_MOUSEMOVE:
		if (bBlock)
		{
			InvertBlock(hDestop, hwnd, ptBeg, ptEnd);
			ptEnd.x = (short)LOWORD(lParam);
			ptEnd.y = (short)HIWORD(lParam);
			ClientToScreen(hwnd, &ptEnd);
			InvertBlock(hDestop, hwnd, ptBeg, ptEnd);
		}
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		if (bBlock)
		{
			InvertBlock(hDestop, hwnd, ptBeg, ptEnd);
			ptEnd.x = (short)LOWORD(lParam);
			ptEnd.y = (short)HIWORD(lParam);
			ClientToScreen(hwnd, &ptEnd);
			if (hBitmap)
				DeleteObject(hBitmap);
			hdc = GetDCEx(hDestop,0,DCX_LOCKWINDOWUPDATE);
			hdcMem = CreateCompatibleDC(hdc);
			int width = abs(ptEnd.x - ptBeg.x);
			int heigh = abs(ptEnd.y - ptBeg.y);
			hBitmap = CreateCompatibleBitmap(hdc, width, heigh);
			SelectObject(hdcMem, hBitmap);
			ptBeg.x = (ptBeg.x < ptEnd.x) ? ptBeg.x : ptEnd.x;
			ptBeg.y = (ptBeg.y < ptEnd.y) ? ptBeg.y : ptEnd.y;

			//StretchBlt(hdcMem, 0, 0, width, heigh, hdc, ptBeg.x, ptBeg.y, width,heigh, SRCCOPY);
			BitBlt(hdcMem, 0, 0, width, heigh, hdc, ptBeg.x, ptBeg.y, SRCCOPY);
			ReleaseDC(hDestop, hdc);
			DeleteDC(hdcMem);
			InvalidateRect(hwnd, 0, true);
		}
		if (bCapture || bBlock)
		{
			bBlock = bCapture = false;
			SetCursor(LoadCursor(0, IDC_ARROW));
			ReleaseCapture();
			LockWindowUpdate(0);
		}
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		switch (wmId)
		{
		case IDM_EDIT_COPY:
		case IDM_EDIT_CUT:
		case IDM_EDIT_DELETE:
		case IDM_EIDT_PASTE:
			MessageBeep(0);
			break;
		}
		break;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		if (hBitmap)
		{
			GetObject(hBitmap, sizeof(BITMAP), &bitmap);
			hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmap);
			BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
			DeleteObject(hdcMem);
		}
		EndPaint(hwnd, &ps);//释放画笔资源
		break;
	case WM_CLOSE://窗口关闭
		DestroyWindow(hwnd);//销毁窗口
		break;
	case WM_DESTROY://窗口销毁时
		if (hBitmap)
			DeleteObject(hBitmap);
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
	wndclass.lpszMenuName =MAKEINTRESOURCE(IDR_MENU1);//菜单
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
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (GetMessage(&msg, nullptr, 0, 0))//从消息队列获取消息
	{
		if (!TranslateAccelerator(hwnd, hAccel, &msg))
		{
			TranslateMessage(&msg);//将一些按键消息转换成字符消息放在按键消息后面
			DispatchMessage(&msg);//分发消息，即调用窗口过程进行处理消息
		}
	}
}