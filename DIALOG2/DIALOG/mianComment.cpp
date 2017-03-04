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
int iCurColor = IDC_WHITE;//当前选中的颜色ID
int iCurShape = IDC_RECT;////选中的形状ID
void PaintWindow(HWND hwnd, int id, int shape)//根据指定的形状和颜色在客户区画
{
	static COLORREF crColor[8] =
	{
		RGB(0, 0, 0),//黑色
		RGB(0, 0, 255),//蓝色
		RGB(0, 255, 0),//绿色
		RGB(0, 255, 255),//青色
		RGB(255, 0, 0),//红色
		RGB(255, 0, 255),//粉红色
		RGB(255, 255, 0),//黄色
		RGB(255, 255, 255)//白色
	};
	HDC hdc = GetDC(hwnd);
	RECT rect;
	HBRUSH hBrush = CreateSolidBrush(crColor[id - IDC_BLACK]);//获得指定颜色的画刷，IDC_BLACK是8种颜色ID中最小
	GetClientRect(hwnd, &rect);//获得客户的大小
	HRGN rgn;
	if (shape == IDC_RECT)//如果形状是矩形
		rgn = CreateRectRgnIndirect(&rect);//创建矩形区域
	else
		rgn = CreateEllipticRgnIndirect(&rect);//创建椭圆形区域
	FillRgn(hdc, rgn, hBrush);//用指定的画刷在指定区域画
	DeleteObject(hBrush);
	DeleteObject(rgn);
	ReleaseDC(hwnd, hdc);
}
void PaintTheBlock(HWND hPaint, int id, int shape)//根据指定的形状和颜色在子窗口客户区画
{
	InvalidateRect(hPaint, nullptr, true);//让子窗口整个客户区失效，即用背景色擦除之前画的图形
	UpdateWindow(hPaint);//更新子窗口，让子窗口客户区有效，这样，当对话框失效重绘的时候，子窗口有为客户区有效而不会发生重绘，即背景不会被擦除
	PaintWindow(hPaint, id, shape);//根据指定的形状和颜色在子窗口客户区画
}
int _stdcall DialogProc(HWND hDlg, unsigned int message, unsigned int wParam, long lParam)
{
	static int SelectedColorID=iCurColor;//选中的颜色ID
    static int SelectedShapeID=iCurShape;//选中的形状ID
	static HWND hPaint;//对话框中的一个子窗口，在里面绘制指定颜色的指定形状
	switch (message)
	{
		case WM_INITDIALOG://创建对话框的初始化消息
			SendMessage(GetDlgItem(hDlg,iCurColor), BM_SETCHECK, 1, 0);//选中默认的ID
			SendMessage(GetDlgItem(hDlg,iCurShape), BM_SETCHECK, 1, 0);//选中默认的ID
			hPaint = GetDlgItem(hDlg, IDC_PAINT);//获得子窗口句柄
			return true;
		case WM_PAINT:
			PaintTheBlock(hPaint, iCurColor, iCurShape);
			return false;//返回false会调用系统的对话框过程，重绘其它区域，如按钮、图标
		case WM_COMMAND://对话框里控件的操作
		{
			int wmId = LOWORD(wParam);//控件ID
			int wmEvent = HIWORD(wParam);//控件上的操作
			switch (wmId)
			{
			case IDOK:
				iCurColor = SelectedColorID;//设置新的颜色和形状
				iCurShape = SelectedShapeID;
				InvalidateRect(hPaint, nullptr, true);//让对话框子窗口重画
				InvalidateRect(GetParent(hDlg), nullptr, true);//让客户区重画
				EndDialog(hDlg, 0);//释放对话框资源
				return true;
			case IDCANCEL:
				EndDialog(hDlg, 0);//释放对话框资源
				return true;
			case IDC_BLACK://选 中颜色按钮
			case IDC_BLUE:
			case IDC_CYAN:
			case IDC_YELLOW:
			case IDC_RED:
			case IDC_WHITE:
			case IDC_GREEN:
			case IDC_PINK:
				//if (wmId <= IDC_WHITE&&wmId >= IDC_BLACK)//IDC_BALCK到IDC_WHITE的8个颜色ID是连续递增的，如果是颜色按钮
				//{
				//	SendMessage(GetDlgItem(hDlg, SelectedID), BM_SETCHECK, 0, 0);//点击后原选中单选按钮被取消
				//    SendMessage(GetDlgItem(hDlg, wmId), BM_SETCHECK, 1, 0);//点击后选中新单选按钮
				//	SelectedID = wmId;//设置当前单选按钮的ID
				//}
				//SendMessage(GetDlgItem(hDlg, wmId), BM_SETCHECK, 1, 0);//点击后选中单选按钮
				CheckRadioButton(hDlg, IDC_BLACK, IDC_WHITE, wmId);//IDC_BALCK到IDC_WHITE的8个颜色ID是连续递增的，如果是颜色按钮,只选中id为wmId的按钮，其它取消选中
				SelectedColorID = wmId;//设置当前单选按钮的ID
				PaintTheBlock(hPaint, SelectedColorID,SelectedShapeID);//在子窗口显示绘制的图形
				return true;
			case IDC_RECT:
			case IDC_ELLIPSE://选中形状按钮
				CheckRadioButton(hDlg, IDC_RECT, IDC_ELLIPSE, wmId);
				SelectedShapeID = wmId;//设置当前单选按钮的ID
				PaintTheBlock(hPaint, SelectedColorID, SelectedShapeID);//在子窗口显示绘制的图形
				return true;
			}
		}
	}
	return false;//如果没有处理，返回0,会调用默认对话框过程，非0则处理了，不调用默认的
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
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MYDIALOG), hwnd, DialogProc);
			break;
		}
		break;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔

		EndPaint(hwnd, &ps);//释放画笔资源
		PaintWindow(hwnd, iCurColor, iCurShape);//根据指定的形状和颜色在客户区画
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
	wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));//图标
	wndclass.hInstance = hInstance;//实例名柄，程序的PID，一个整数，根据这个整数，系统可以找到进程的地址
	wndclass.lpfnWndProc = WndProc;//窗口过程
	wndclass.lpszClassName = TEXT("zeng");//类名
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);//菜单
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