#include<memory>
#include<string>

#include<Windows.h>
using namespace std;
#include"resource.h"
static int width, heigh;//截图的宽、高
static RECT clipRect;//剪切区域的矩形
static bool bFullScreen = false;//是否全屏
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
	SetWindowPos(hwnd, HWND_TOPMOST,-3,-3, cxScreen+3, cyScreen+3, SWP_SHOWWINDOW);//让窗口显示全屏，置顶，(0,0)会使对话框窗口不能在屏幕左上角-3才显示正常，可能是边框长度3
	bFullScreen = true;//是全屏
}
void RecoveryWindow(HWND hwnd, HMENU hMenu)//恢复窗口原来的大小位置
{
	SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);//恢复窗口样式
	SetMenu(hwnd, hMenu);//添加菜单
	SetWindowPos(hwnd, HWND_NOTOPMOST,122, 122,width+140,heigh+180, SWP_SHOWWINDOW);//设置窗口大小，为截图大小加上120，留60像素作为截图与边框间间距
	bFullScreen = false;//不是全屏
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
int _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
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
	static HBITMAP blockBitmap;//保存截图的位图
	BITMAP bitmap;//位图结构
	static int cxScreen, cyScreen;//屏幕宽度、高度
	static RECT windowRect;//窗口的坐标，当窗口全屏时保存原窗口
	static HBRUSH redBrush;//红色画刷，画截图矩形的边框的颜色
	static HMENU hMenu;//窗口菜单
	switch (UMsg)
	{
	case WM_INITDIALOG://窗口被创建
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		cxScreen = GetSystemMetrics(SM_CXSCREEN);//屏幕宽度、高度
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
		if (bNewBlcok&&hBitmap)//如果开始截图并且全屏位图不为空
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
		case VK_SPACE://按空格进行全屏
			if (bFullScreen)//如果已是全屏
				break;//退出
			GetWindowRect(hwnd, &windowRect);
			if (hBitmap)//如果位图句柄不为空
			{
				DeleteObject(hBitmap);//销毁位图
				hBitmap = 0;
			}
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
			RecoveryWindow(hwnd,hMenu);//恢复原来的窗口
			HDC hdcClip = CreateCompatibleDC(hdc);//创建剪切区域的画笔
			if (blockBitmap)//如果上一次的截图位图未删除
			{
				DeleteObject(blockBitmap);//删除
				blockBitmap = 0;
			}
			blockBitmap = CreateCompatibleBitmap(hdc, width, heigh);//创建兼容的画笔
			SelectObject(hdcClip, blockBitmap);//将截图位图放入
			BitBlt(hdcClip, 0, 0, width, heigh, hdcMem, clipRect.left, clipRect.top, SRCCOPY);//将截图绘制在截图位图
			SetCursor(LoadCursor(0, IDC_ARROW));//恢复鼠标箭头
			ReleaseCapture();//释放鼠标的捕捉
			ReleaseDC(hwnd, hdc);
			DeleteDC(hdcMem);
			DeleteDC(hdcClip);
			ShowWindow(hwnd,SW_NORMAL);//显示窗口
			SetForegroundWindow(hwnd);//窗口置顶
			InvalidateRect(hwnd, 0, true);//在窗口区域显示截图
			if (!blockBitmap)
				break;//没有要拷贝的位图
			copyBitmap = CopyBitmap(blockBitmap);//拷贝位图
			OpenClipboard(hwnd);//打开剪贴板
			EmptyClipboard();//将剪贴板清空
			SetClipboardData(CF_BITMAP, copyBitmap);//将位图句柄放到剪贴板
			CloseClipboard();//关闭剪贴板
		}
		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		switch (wmId)
		{
		case ID_40001:
			bNewBlcok = true;//开始截图
			ShowWindow(hwnd, SW_HIDE);//隐藏窗口
			if (hBitmap)//如果位图不为空
			{
				DeleteObject(hBitmap);//将以前位图删除
				hBitmap = 0;
			}
			GetDestopBK(hDestop, hBitmap, cxScreen, cyScreen);//得到窗口位图
			GetWindowRect(hwnd, &windowRect);//获得窗口坐标，在截完图后恢复
			ShowFullScreen(hwnd, cxScreen, cyScreen);//让窗口全屏
			InvalidateRect(hwnd, 0, true);//显示整个屏幕截图
			break;
		case IDCANCEL://按下ESC键
			RecoveryWindow(hwnd, hMenu);
			break;
		}
		break;
	case WM_PAINT://有区域失效，发生重绘
	{
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		HBITMAP tempBitmap = 0;//昨时位图，保存要显示的位图
		tempBitmap = (bFullScreen) ? hBitmap : blockBitmap;//如果是全屏，用全屏位图hBitmap,否则用截图位图blockBitmap
		if (tempBitmap)//位图不空
		{
			hdcMem = CreateCompatibleDC(0);//获得与屏幕兼容的画笔
			BITMAP bm;//位图结构
			GetObject(tempBitmap, sizeof(BITMAP), &bm);//获得位图信息
			SelectObject(hdcMem, tempBitmap);//将位图选入画笔
			if (bFullScreen)//全屏
			BitBlt(hdc,0,0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);//在窗口上显示截图
			else
			BitBlt(hdc,60,60, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);//在窗口上显示截图
			DeleteObject(hdcMem);
		}
	}
		EndPaint(hwnd, &ps);//释放画笔资源
		break;
	case WM_CLOSE://窗口关闭
		if (hBitmap)
			DeleteObject(hBitmap);//删除位图
		if (blockBitmap)//如果上一次的截图位图未删除
			DeleteObject(blockBitmap);//删除
		DeleteObject(redBrush);//删除画刷
		EndDialog(hwnd, 0);
		break;
	}
	return false;
}
int _stdcall DialogProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)
{
	int wmID = LOWORD(wParam);//控件ID
	static HWND hListLeft, hListMiddle, hListRight; 
	int index;
	TCHAR szBuffer[256];
	TCHAR *pText;
	int len;
	int count;
	switch (message)
	{
	case WM_INITDIALOG:
		hListLeft = GetDlgItem(hDlg, IDC_LISTLEFT);
		hListMiddle = GetDlgItem(hDlg, IDC_LISTMIDDLE);
		hListRight = GetDlgItem(hDlg, IDC_LISTRIGHT);

		return false;
	case WM_COMMAND:
		switch (wmID)
		{
			case IDC_MOVERIGHT1:
				index = SendMessage(hListLeft, LB_GETCURSEL, 0,0);
				if (index != LB_ERR)
				{
					len = SendMessage(hListLeft, LB_GETTEXTLEN, index, 0);
					pText = new TCHAR[len + 1];
					shared_ptr<TCHAR>sp(pText, [](TCHAR *p){delete[]p; });
					SendMessage(hListLeft, LB_GETTEXT, index, (long)pText);
					SendMessage(hListMiddle, LB_INSERTSTRING, -1, (long)pText);
					SendMessage(hListLeft, LB_DELETESTRING, index, 0);
				}
				break;
			case IDC_MOVERIGHT2:
				count = SendMessage(hListMiddle, LB_GETCOUNT, 0, 0);
				if (count)
				{
					for (int index = count - 1; index >= 0; index--)
					{
						if (SendMessage(hListMiddle, LB_GETSEL, index, 0))
						{
							len = SendMessage(hListMiddle, LB_GETTEXTLEN, index, 0);
							pText = new TCHAR[len + 1];
							shared_ptr<TCHAR>sp(pText, [](TCHAR *p){delete[]p; });
							SendMessage(hListMiddle, LB_GETTEXT, index, (long)pText);
							SendMessage(hListRight, LB_INSERTSTRING, -1, (long)pText);
							SendMessage(hListMiddle, LB_DELETESTRING, index, 0);
						}
					}
				}
			case IDC_INSERTTHREELEFT:
				SendMessage(hListLeft, LB_ADDSTRING, 0, (long)TEXT("hello"));
				SendMessage(hListLeft, LB_ADDSTRING, 0, (long)TEXT("world"));
				SendMessage(hListLeft, LB_ADDSTRING, 0, (long)TEXT("program"));
				break;
			case IDC_INSERTTHREERIGHT:
				SendMessage(hListMiddle, LB_INSERTSTRING, -1, (long)TEXT("one"));
				SendMessage(hListMiddle, LB_INSERTSTRING, -1, (long)TEXT("two"));
				SendMessage(hListMiddle, LB_INSERTSTRING, -1, (long)TEXT("three"));
				break;
			case IDC_DELETETHIRD:
				SendMessage(hListLeft, LB_DELETESTRING, 2, 0);
				break;
			case IDC_INSERTTHIRD:
				SendMessage(hListMiddle, LB_INSERTSTRING, 2, (long)TEXT("nihao"));
				break;
			case IDC_SETTHIRDANDSIXTH:
				SendMessage(hListMiddle, LB_SETSEL,1, 2);
				SendMessage(hListMiddle, LB_SETSEL,1, 5);
				break;
			case IDC_UNSETTHIRDANDSIXTH:
				SendMessage(hListMiddle, LB_SETSEL, 0 ,2);
				SendMessage(hListMiddle, LB_SETSEL,  0,5);
				break;
			case IDC_SELECTALL:
				SendMessage(hListMiddle, LB_SETSEL, 1, -1);
				break;
			case IDC_RESET:
				SendMessage(hListLeft, LB_RESETCONTENT, 0, 0);
				break;
			case IDC_SELECTTHIRD:
				SendMessage(hListLeft, LB_SETCURSEL, 2,0);
				break;
			case IDC_UNSET:
				index = SendMessage(hListLeft, LB_GETCURSEL, 0, 0);
				if (index != LB_ERR)
					SendMessage(hListLeft, LB_SETCURSEL, -1, 0);
				break;

		}
		return true;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return false;
	}
	return false;
}
WNDPROC oldProc;
long _stdcall StaticProc(HWND hDlg, UINT message, UINT wParam, long lParam)//
{
	HDC hdc, hdcMem;//画笔
	static HBITMAP hBitmap;//位图句柄
	BITMAP bm;//位图结构
	static int cxClient, cyClient;//客户区宽度、高度
	PAINTSTRUCT ps;//画笔结构，保留画笔作息

	switch (message)
	{
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);//获得窗口画笔

		hBitmap = LoadBitmap((HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_BITMAP1));//加载位图
		hdcMem = CreateCompatibleDC(hdc);//创建内存画笔
		SelectObject(hdcMem, hBitmap);//将位图选入画笔
		GetObject(hBitmap, sizeof(BITMAP), &bm);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
		//FillRect(hdc,)
		EndPaint(hDlg, &ps);//释放画笔资源

		break;
	case WM_CLOSE:
		DeleteObject(hBitmap);
		break;
	}
	return CallWindowProc(oldProc, hDlg, message, wParam, lParam);
}
int _stdcall DlgFileBatProc(HWND hDlg, UINT message, UINT wParam, long lParam)//文件批处理对话框过程
{
	static HWND hStaticPhoto;//显示图片区域的static控件句柄
	switch (message)
	{
	case WM_INITDIALOG://对话框初始化
		//显示图片区域的static控件句柄初始化
		hStaticPhoto = GetDlgItem(hDlg, IDC_BITMAP);//得到句柄
		oldProc = (WNDPROC)SetWindowLong(hStaticPhoto, GWL_WNDPROC,(long) StaticProc);
		break;
	case WM_PAINT:

		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	}
	return false;
}
int WinMain(HINSTANCE hInstance, HINSTANCE hPreHinstance, char *szCmdCommand, int nShow)
{

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG3), nullptr,DlgFileBatProc);

}