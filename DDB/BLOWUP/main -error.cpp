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
static int width, heigh;//截图的宽、高
static RECT clipRect;//剪切区域的矩形
void GetDestopBK(HWND hDestop, HBITMAP &hBitmap, int cxScreen, int cyScreen)//获得窗口背景，选入位图
{
	HDC hdcMem = CreateCompatibleDC(0);//获得与屏幕兼容的画笔
	HDC  hdcScr = GetDCEx(hDestop, 0, DCX_LOCKWINDOWUPDATE);//获得屏幕的画笔,即使屏幕锁住也能刷新
	hBitmap = CreateCompatibleBitmap(hdcScr, cxScreen, cyScreen);//创建与屏幕大小兼容的位图
	SelectObject(hdcMem, hBitmap);//将位图选入画笔
	BitBlt(hdcMem, 0, 0, cxScreen, cyScreen, hdcScr, 0, 0, SRCCOPY);//将屏幕的位图拷贝到内容上的位图
	DeleteDC(hdcMem);
	ReleaseDC(hDestop, hdcScr);
}

void ShowFullScreen(HWND hwnd, int cxScreen, int cyScreen)//让窗口全屏
{
	SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);//设置窗口样式为弹出式窗口，就是去掉系统菜单、标题栏、边框等，也可用WS_BORDER样式
	SetMenu(hwnd, 0);//去掉菜单
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, cxScreen, cyScreen, SWP_SHOWWINDOW);//让窗口显示全屏，置顶
	clipRect.left = clipRect.top = 0;//在屏幕显示在左上角
	width = cxScreen;//显示的宽度、高度
	heigh = cyScreen;
}
void RecoveryWindow(HWND hwnd, RECT windowRect, HMENU hMenu)//恢复窗口原来的大小位置
{
	SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);//恢复窗口样式
	SetMenu(hwnd, hMenu);//添加菜单
	SetWindowPos(hwnd, HWND_NOTOPMOST, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, SWP_SHOWWINDOW);//恢复原来位置
}
HBITMAP CopyBitmap(const HBITMAP &hBitmap)//位图拷贝
{
	HBITMAP copyBitmap;
	HDC hdcSrc = CreateCompatibleDC(0);//获得内存画笔
	HDC hdcDst = CreateCompatibleDC(0);//获得内存画笔
	SelectObject(hdcSrc, hBitmap);//将位图选入内存画笔
	BITMAP bm;//位图结构
	GetObject(hBitmap, sizeof(BITMAP), &bm);//获得要拷贝的位图信息
	copyBitmap = CreateCompatibleBitmap(hdcSrc, bm.bmWidth, bm.bmHeight);//创建要拷贝的位图
	SelectObject(hdcDst, copyBitmap);//将位图选入内存画笔
	BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);//将位图拷贝到另一副位图
	DeleteObject(hdcSrc);
	DeleteObject(hdcDst);
	return copyBitmap;
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	static HDC hdc;//画笔
	static HDC hdcMem;//内存画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	static POINT ptBeg, ptEnd;//截图起点、终点
	static bool  bBlock = false;//是否开始画矩形块
	static bool bNewBlcok = false;//是否开始截图
	static HWND hDestop;//桌面句柄
	static HBITMAP hBitmap;//位图句柄
	static HBITMAP copyBitmap;//拷贝的位图，当进行复制、剪裁时，将截图拷贝一份到剪贴板
	static HBITMAP clipBitmap;//从剪贴板中获得的位图句柄
	BITMAP bitmap;//位图结构
	static int cxScreen, cyScreen;//屏幕宽度、高度
	static RECT windowRect;//窗口的坐标，当窗口全屏时保存原窗口
	static HBRUSH redBrush;//红色画刷，画截图矩形的边框的颜色
	static HMENU hMenu;//窗口菜单
	HANDLE handle;//从剪贴板返回的句柄
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		cxScreen = GetSystemMetrics(SM_CXSCREEN);////屏幕宽度、高度
		cyScreen = GetSystemMetrics(SM_CYSCREEN);
		redBrush = CreateSolidBrush(RGB(255, 0, 0));//红色画刷，画截图矩形的边框的颜色
		hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));//加载菜单
		SetMenu(hwnd, hMenu);//设置菜单
		hDestop = GetDesktopWindow();//桌面句柄
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		if (bNewBlcok&&hBitmap)//如果开始截图并且位图不为空
		{
			bBlock = true;//开始画矩形块
			SetCapture(hwnd);//捕捉鼠标
			SetCursor(LoadCursor(0, IDC_CROSS));//将光标设为十字箭头，表示在截图中
			ptBeg.x = LOWORD(lParam);//获得截图的起始坐标
			ptBeg.y = HIWORD(lParam);
			ptEnd = ptBeg;
			hdc = GetDC(hwnd);//获得窗口画笔
			hdcMem = CreateCompatibleDC(hdc);//获得的画笔
			SelectObject(hdcMem, hBitmap);//将屏幕位图选入画笔
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE://按ESC键退出全屏
			RecoveryWindow(hwnd, windowRect, hMenu);
			break;
		case VK_SPACE://按空格进行全屏
			GetWindowRect(hwnd, &windowRect);
			GetDestopBK(hDestop, hBitmap, cxScreen, cyScreen);//得到窗口位图
			ShowFullScreen(hwnd, cxScreen, cyScreen);//全屏显示
			InvalidateRect(hwnd, 0, true);//在窗口绘制桌面背景
			break;
		}
		break;
	case WM_MOUSEMOVE:
		if (bBlock)//如果已经画矩形区域
		{
			BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY);//将先前画的矩形擦除	
			ptEnd.x = (short)LOWORD(lParam);//终点坐标
			ptEnd.y = (short)HIWORD(lParam);
			if (ptBeg.x < ptEnd.x)//将模坐标最小的作为矩形的起点
			{
				clipRect.left = ptBeg.x;
				clipRect.right = ptEnd.x;
			}
			else
			{
				clipRect.left = ptEnd.x;
				clipRect.right = ptBeg.x;
			}
			if (ptBeg.y < ptEnd.y)//将模坐标最小的作为矩形的终点
			{
				clipRect.top = ptBeg.y;
				clipRect.bottom = ptEnd.y;
			}
			else
			{
				clipRect.top = ptEnd.y;
				clipRect.bottom = ptBeg.y;
			}
			FrameRect(hdc, &clipRect, redBrush);//用红色边框画出矩形区域
		}
		break;
	case WM_RBUTTONDOWN://鼠标右键，取消本次截图，重次截图
		bBlock = false;//还未划矩形块
		BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY);//将先前画的矩形擦除	
		SetCursor(LoadCursor(0, IDC_ARROW));//恢复鼠标箭头
		ReleaseDC(hwnd, hdc);
		DeleteDC(hdcMem);
		break;
	case WM_LBUTTONUP:
		if (bBlock)
		{
			bBlock = bNewBlcok = false;//截图结束
			width = abs(ptEnd.x - ptBeg.x);//截图的宽度，高度
			heigh = abs(ptEnd.y - ptBeg.y);
			RecoveryWindow(hwnd, windowRect, hMenu);//恢复原来的窗口
			BitBlt(hdc, 0, 0, width, heigh, hdcMem, clipRect.left, clipRect.top, SRCCOPY);//将截图绘制在窗口
			SetCursor(LoadCursor(0, IDC_ARROW));//恢复鼠标箭头
			ReleaseCapture();//释放鼠标的捕捉
			ReleaseDC(hwnd, hdc);
			DeleteDC(hdcMem);
			InvalidateRect(hwnd, 0, true);//在窗口区域显示截图
		}
		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		switch (wmId)
		{
		case IDM_EDIT_COPY:
		case IDM_EDIT_CUT:
			if (!hBitmap)
				break;//没有要拷贝的位图
			copyBitmap = CopyBitmap(hBitmap);//拷贝位图
			OpenClipboard(hwnd);//打开剪贴板
			EmptyClipboard();//将剪贴板清空
			SetClipboardData(CF_BITMAP, copyBitmap);//将位图句柄放到剪贴板
			CloseClipboard();//关闭剪贴板
			if (wmId == IDM_EDIT_COPY)//如果是拷贝的话，退出switch，否则fall through 到下面的清空窗口的内容
				break;
		case IDM_EDIT_DELETE:
			if (hBitmap)//如果位图句柄不为空
			{
				DeleteObject(hBitmap);//销毁位图
				hBitmap = 0;
				InvalidateRect(hwnd, 0, true);//重新显示窗口
			}
			break;
		case IDM_EIDT_PASTE:
			if (hBitmap)//如果位图不为空
			{
				DeleteObject(hBitmap);//删除
				hBitmap = 0;
			}
			OpenClipboard(hwnd);//打开剪贴板
			clipBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);//剪贴板将位图句柄
			if (clipBitmap)//如果剪贴板上有位图句柄
			{
				hBitmap = CopyBitmap(clipBitmap);//拷贝位图
				BITMAP bm;//位图结构
				GetObject(hBitmap, sizeof(BITMAP), &bm);//获得要粘贴的位图信息
				width = bm.bmWidth;//获得剪贴板上位图的宽度高度
				heigh = bm.bmHeight;
				clipRect.left = clipRect.top = 0;//设置起点为位图上左上角
			}
			CloseClipboard();//关闭剪贴板
			InvalidateRect(hwnd, 0, true);//重新显示窗口
			break;
		case IDM_FILENEW:
			bNewBlcok = true;//开始截图
			if (hBitmap)//如果位图不为空
			{
				DeleteObject(hBitmap);//将以前位图删除
				hBitmap = 0;
			}
			GetDestopBK(hDestop, hBitmap, cxScreen, cyScreen);//得到窗口位图
			GetWindowRect(hwnd, &windowRect);//获得窗口坐标，在截完图后恢复
			ShowFullScreen(hwnd, cxScreen, cyScreen);//让窗口全屏
			InvalidateRect(hwnd, 0, true);//显示整个屏幕截图
		}
		break;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		if (hBitmap)
		{
			hdcMem = CreateCompatibleDC(0);//获得与屏幕兼容的画笔
			SelectObject(hdcMem, hBitmap);//将位图选入画笔
			BitBlt(hdc, 0, 0, width, heigh, hdcMem, clipRect.left, clipRect.top, SRCCOPY);//在窗口上显示截图
			DeleteObject(hdcMem);
		}
		EndPaint(hwnd, &ps);//释放画笔资源
		break;
	case WM_CLOSE://窗口关闭
		DestroyWindow(hwnd);//销毁窗口
		break;
	case WM_DESTROY://窗口销毁时
		if (hBitmap)
			DeleteObject(hBitmap);//删除位图
		DeleteObject(redBrush);//删除画刷
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
	wndclass.lpszMenuName = 0;//菜单
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