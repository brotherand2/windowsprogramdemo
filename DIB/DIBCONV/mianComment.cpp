//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include"resource.h"
#include"DIBFILE.h"
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
	static int cxClient, cyClient,cxDib,cyDib;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	static OPENFILENAME ofn;
	static BITMAPFILEHEADER *pbmfh;
	static BITMAPINFOHEADER *pbmih;
	static unsigned char *pBits,*bits;
	static HMENU hMenu;
	static TCHAR szTitle[MAX_PATH], szPath[MAX_PATH];
	static HBITMAP hBitmap;
	BITMAP bm;
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		DibFileInitialize(hwnd, ofn);
		hMenu = GetMenu(hwnd);
		EnableMenuItem(hMenu, ID_SAVE, pbmfh ? MF_ENABLED : MF_GRAYED);
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
		case ID_OPEN:
			ofn.lpstrFile = szPath;
			ofn.lpstrFileTitle = szTitle;
			if (GetOpenFileName(&ofn))
			{
				if (pbmfh)
				{
					free(pbmfh);
					pbmfh = 0;
				}
				if (hBitmap)
				{
					DeleteObject(hBitmap);
					hBitmap = 0;
				}

				pbmfh= DibLoadImage(hwnd, szPath);
				EnableMenuItem(hMenu, ID_SAVE, pbmfh ? MF_ENABLED : MF_GRAYED);
				if (pbmfh)
				{
					pbmih = (BITMAPINFOHEADER*)(pbmfh + 1);
					if (pbmih->biSize == sizeof(BITMAPCOREHEADER))
					{
						cxDib = ((BITMAPCOREHEADER*)(pbmih))->bcWidth;
						cyDib = ((BITMAPCOREHEADER*)(pbmih))->bcHeight;
					}
					else
					{
						cxDib =abs( pbmih->biWidth);
						cyDib =abs( pbmih->biHeight);
					}
					pBits = (unsigned char *)pbmfh + pbmfh->bfOffBits;
					hBitmap = CreateDIBSection(0, (BITMAPINFO*)pbmih, DIB_RGB_COLORS, (void**)&bits, 0, 0);
					CopyMemory(bits, pBits, pbmfh->bfSize -pbmfh->bfOffBits);
					InvalidateRect(hwnd, 0, true);
				}
			}
			break;
		case ID_SAVE:
			if (GetSaveFileName(&ofn))
			{
				DibWriteImage(szPath, pbmfh);
			}
			break;
		default:
			break;
		}
		break;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		if (hBitmap)
		{
			GetObject(hBitmap, sizeof(BITMAP), &bm);
			HDC hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmap);
			BitBlt(hdc, (cxClient - bm.bmWidth) / 2, (cyClient - bm.bmHeight) / 2, bm.bmWidth, bm.bmHeight,
				hdcMem, 0, 0, SRCCOPY);
			DeleteDC(hdcMem);
		}
		EndPaint(hwnd, &ps);//释放画笔资源
		break;
	case WM_CLOSE://窗口关闭
		DestroyWindow(hwnd);//销毁窗口
		break;
	case WM_DESTROY://窗口销毁时
		if (pbmfh)
			free(pbmfh);
		if (hBitmap)
		{
			DeleteObject(hBitmap);
			hBitmap = 0;

		}
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