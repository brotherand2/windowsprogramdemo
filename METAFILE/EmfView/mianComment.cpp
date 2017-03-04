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
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	static OPENFILENAME ofn;
	static TCHAR szPath[MAX_PATH], szTitle[MAX_PATH];
	static TCHAR szFilter[] = TEXT("emf文件(*.emf)\0*.emf\0所有文件(*.*)\0*.*\0\0");
	static HENHMETAFILE hemf,hemfCopy;
	static RECT rect;
	static DOCINFO di = { sizeof(di), TEXT("EmfView") };
	static PRINTDLG pd = { sizeof(pd) };
	HDC hdcPrn;
	bool bSuccess;
	int length = 0;
	int i = 0;
	TCHAR szBuffer[256];
	ENHMETAHEADER header;
	int isEnabled;
	static HMENU hMenu;
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		hMenu = GetMenu(hwnd);
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFilter = szFilter;
		ofn.lpstrFile = szPath;
		ofn.lpstrFileTitle = szTitle;
		ofn.nMaxFile = MAX_PATH;
		ofn.nMaxFileTitle = MAX_PATH;
		ofn.nFilterIndex = 1;
		ofn.lpstrDefExt = TEXT("emf");
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		break;
	case WM_INITMENUPOPUP:
		if (hemf)
			isEnabled = MF_ENABLED;
		else
			isEnabled = MF_GRAYED;
		EnableMenuItem(hMenu, IDM_SAVEAS, isEnabled);
		EnableMenuItem(hMenu, ID_PRINT, isEnabled);
		EnableMenuItem(hMenu, IDM_PROPERTIES, isEnabled);
		EnableMenuItem(hMenu, IDM_CUT, isEnabled);
		EnableMenuItem(hMenu, IDM_COPY, isEnabled);
		EnableMenuItem(hMenu, IDM_DELETE, isEnabled);
		EnableMenuItem(hMenu, IDM_PASTE, IsClipboardFormatAvailable(CF_ENHMETAFILE)?MF_ENABLED:MF_GRAYED);
		break;

	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		switch (wmId)
		{
		case IDM_OPEN:
			if (!GetOpenFileName(&ofn))
				return 0;
			if (hemf)
			{
				DeleteEnhMetaFile(hemf);
				hemf = 0;
			}
			hemf = GetEnhMetaFile(szPath);
			if (hemf)
				InvalidateRect(hwnd, 0, true);
			else
				MessageBox(hwnd, TEXT("不是图元文件"), TEXT("文件读取错误"), MB_ICONERROR);
			break;
		case IDM_SAVEAS:
			ofn.Flags = OFN_OVERWRITEPROMPT;
			if(!GetSaveFileName(&ofn))
				return 0;
			hemfCopy = CopyEnhMetaFile(hemf, szPath);
			if (hemfCopy)
			{
				DeleteEnhMetaFile(hemf);
				hemf = hemfCopy;
			}
			else
				MessageBox(hwnd, TEXT("不能保存图元文件"), TEXT("文件保存失败"), MB_ICONERROR);
			break;
		case ID_PRINT:
			pd.Flags = PD_RETURNDC;
			if (!PrintDlg(&pd))
				return 0;
			hdcPrn = pd.hDC;
			if (!hdcPrn)
			{
				MessageBox(hwnd, TEXT("不能获得dc"), TEXT("打印失败"), MB_ICONERROR);
				return 0;
			}
			rect.right = GetDeviceCaps(hdcPrn, HORZRES);
			rect.bottom = GetDeviceCaps(hdcPrn, VERTRES);
			bSuccess = false;
			if (StartDoc(hdcPrn, &di) > 0 && StartPage(hdcPrn) > 0)
			{
				PlayEnhMetaFile(hdcPrn, hemf, &rect);
				if (EndPage(hdcPrn) > 0)
				{
					bSuccess = true;
					EndDoc(hdcPrn);
				}
			}
			if (!bSuccess)
				MessageBox(hwnd, TEXT("打印失败"), TEXT(""), MB_ICONERROR);
			break;
		case IDM_PROPERTIES:
			length = GetEnhMetaFileDescription(hemf, 0, 0);
			GetEnhMetaFileHeader(hemf, sizeof(ENHMETAHEADER),&header);
			i += wsprintf(szBuffer, TEXT("Bounds=(%d,%d)to(%d,%d)pixels\n"), header.rclBounds.left,
				header.rclBounds.top, header.rclBounds.right, header.rclBounds.bottom);
			i += wsprintf(szBuffer + i, TEXT("Frame=(%d,%d)to(%d,%d)mms\n"), header.rclFrame.left,
				header.rclFrame.top, header.rclFrame.right, header.rclFrame.bottom);
			i += wsprintf(szBuffer + i, TEXT("Resolution=(%d,%d)mms=(%d,%d)pixels\n"), header.szlDevice.cx,
				header.szlDevice.cy, header.szlMillimeters.cx, header.szlMillimeters.cy);
			i += wsprintf(szBuffer + i, TEXT("Size=%d,Records=%d,Handles=%d,Palette Entries=%d\n"),
				header.nSize, header.nRecords, header.nHandles, header.nPalEntries);
			if (length)
			{
				i += wsprintf(szBuffer+i, TEXT("Description:"));
				GetEnhMetaFileDescription(hemf, length, szBuffer + i);
				szBuffer[lstrlen(szBuffer)] = '\t';
			}
			MessageBox(hwnd, szBuffer, TEXT("MetaFile Properties"), 0);
			break;
		case ID_EXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case IDM_CUT:
		case IDM_COPY:
			if (!hemf)
				return 0;
			hemfCopy = CopyEnhMetaFile(hemf, 0);
			OpenClipboard(hwnd);
			EmptyClipboard();
			SetClipboardData(CF_ENHMETAFILE, hemfCopy);
			CloseClipboard();
			if (wmId==IDM_COPY)
				break;
		case IDM_DELETE:
			if (hemf)
			{
				DeleteEnhMetaFile(hemf);
				hemf = 0;
				InvalidateRect(hwnd, 0, true);
			}
			break;
		case IDM_PASTE:
			OpenClipboard(hwnd);
			hemfCopy =(HENHMETAFILE) GetClipboardData(CF_ENHMETAFILE);
			if (hemfCopy)
			{
				if (hemf)
				DeleteEnhMetaFile(hemf);
				hemf =CopyEnhMetaFile(hemfCopy,0);
				InvalidateRect(hwnd, 0, true);
			}
			CloseClipboard();
			break;
		case IDM_ABOUT:
			MessageBox(hwnd, TEXT("Metafile Viewer ver1.1\n(c)zeng,13"), TEXT("EmvView"), MB_OK);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		GetClientRect(hwnd, &rect);
		if (hemf)
			PlayEnhMetaFile(hdc, hemf,&rect);
		EndPaint(hwnd, &ps);//释放画笔资源
		break;
	case WM_CLOSE://窗口关闭
		DestroyWindow(hwnd);//销毁窗口
		break;
	case WM_DESTROY://窗口销毁时
		PostQuitMessage(0);//抛出一条返回值为0退出消息
		if (hemf)
			DeleteEnhMetaFile(hemf);
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