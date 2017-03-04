//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<tchar.h>
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
void DisplayDibHeaders(HWND hwnd, TCHAR *szTitle, TCHAR *szPath)
{
	static TCHAR *szInfoName[] = {TEXT("BITMAPCOREHEADER"),TEXT("BITMAPINFOHEADER"),TEXT("BITMAPV4HEADER"),TEXT("BITMAPV5HEADER")};
	static TCHAR *szCompression[] = {TEXT("BI_RGB"),TEXT("BI_RLE8"),TEXT("BI_RLE4"),TEXT("BI_BITFIELDS"),TEXT("unknown")};
	Printf(hwnd, TEXT("File:%s\r\n\r\n"), szPath);
	BITMAPFILEHEADER *pbmfh=DibLoadImage(hwnd, szPath);
	if (!pbmfh)
		return;
	Printf(hwnd, TEXT("文件头\r\n"));
	Printf(hwnd, TEXT("\t.文件类型=0X%X\r\n"),pbmfh->bfType);
	Printf(hwnd, TEXT("\t.文件大小=%u\r\n"),pbmfh->bfSize);
	Printf(hwnd, TEXT("\t.文件保留1=%u\r\n"),pbmfh->bfReserved1);
	Printf(hwnd, TEXT("\t.文件保留1=%u\r\n"),pbmfh->bfReserved2);
	Printf(hwnd, TEXT("\t.像素位偏移=%u\r\n\r\n"),pbmfh->bfOffBits);
	BITMAPV5HEADER *pbmih = (BITMAPV5HEADER*)(pbmfh + 1);//信息头
	int size;
	switch (pbmih->bV5Size)
	{
	case sizeof(BITMAPCOREHEADER) :
		size = 0;
		break;
	case sizeof(BITMAPINFOHEADER) :
		size = 1;
		break;
	case sizeof(BITMAPV4HEADER) :
		size = 2;
		break;
	case sizeof(BITMAPV5HEADER) :
		size = 3;
		break;
	default:
		size = 4;
		break;
	}
	Printf(hwnd, TEXT("信息头:%s\r\n"), szInfoName[size]);
	if (size == 0)//如果是核心文件头，宽度、高度是WORD要先处理
	{
		BITMAPCOREHEADER *pbmch = (BITMAPCOREHEADER*)pbmfh;
		Printf(hwnd, TEXT("\t.信息头大小=%u\r\n"), pbmch->bcSize);
		Printf(hwnd, TEXT("\t.以像素计的图像宽度=%u\r\n"), pbmch->bcWidth);
		Printf(hwnd, TEXT("\t.以像素计的图像高度=%u\r\n"), pbmch->bcHeight);
		Printf(hwnd, TEXT("\t.平面数=%u\r\n"), pbmch->bcPlanes);
		Printf(hwnd, TEXT("\t.每个像素的位数=%u\r\n"), pbmch->bcBitCount);
		free(pbmfh);
		return;
	}
	//显示信息头
	Printf(hwnd, TEXT("\t.信息头大小=%u\r\n"),pbmih->bV5Size);
	Printf(hwnd, TEXT("\t.以像素计的图像宽度=%u\r\n"),pbmih->bV5Width);
	Printf(hwnd, TEXT("\t.以像素计的图像高度=%u\r\n"),pbmih->bV5Height);
	Printf(hwnd, TEXT("\t.平面数=%u\r\n"),pbmih->bV5Planes);
	Printf(hwnd, TEXT("\t.每个像素的位数=%u\r\n"),pbmih->bV5BitCount);
	Printf(hwnd, TEXT("\t.压缩编码=%s\r\n"), szCompression[min(4, pbmih->bV5Compression)]);
	Printf(hwnd, TEXT("\t.图像的字节数=%u\r\n"), pbmih->bV5SizeImage);
	Printf(hwnd, TEXT("\t.水平分辨率=%u\r\n"), pbmih->bV5XPelsPerMeter);
	Printf(hwnd, TEXT("\t.垂直分辨率=%u\r\n"), pbmih->bV5YPelsPerMeter);
	Printf(hwnd, TEXT("\t.用到的颜色数=%u\r\n"), pbmih->bV5ClrUsed);
	Printf(hwnd, TEXT("\t.重要颜色数=%u\r\n\r\n"), pbmih->bV5ClrImportant);
	if (size == 1)//扩展的信息头
	{
		if (pbmih->bV5Compression == BI_BITFIELDS)
		{
			Printf(hwnd, TEXT("\t.红色遮罩=%x\r\n"),pbmih->bV5RedMask);
			Printf(hwnd, TEXT("\t.绿色遮罩=%x\r\n"),pbmih->bV5GreenMask);
			Printf(hwnd, TEXT("\t.蓝色遮罩=%x\r\n"),pbmih->bV5BlueMask);
		}
		free(pbmfh);
		return;
	}
	Printf(hwnd, TEXT("\t.红色遮罩=%x\r\n"), pbmih->bV5RedMask);
	Printf(hwnd, TEXT("\t.绿色遮罩=%x\r\n"), pbmih->bV5GreenMask);
	Printf(hwnd, TEXT("\t.蓝色遮罩=%x\r\n"), pbmih->bV5BlueMask);
	Printf(hwnd, TEXT("\t.阿尔法遮罩=%x\r\n"), pbmih->bV5AlphaMask);
	Printf(hwnd, TEXT("\t.色彩空间类型=%x\r\n"), pbmih->bV5CSType);
	Printf(hwnd, TEXT("\t.红色伽马值=%x\r\n"), pbmih->bV5GammaRed);
	Printf(hwnd, TEXT("\t.绿色伽马值=%x\r\n"), pbmih->bV5GammaGreen);
	Printf(hwnd, TEXT("\t.蓝色伽马值=%x\r\n"), pbmih->bV5GammaBlue);
	free(pbmfh);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	static HWND hEdit;
	static TCHAR szTitle[MAX_PATH], szPath[MAX_PATH];
	static OPENFILENAME ofn;
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		hEdit = CreateWindow(TEXT("edit"), 0, WS_CHILD |WS_VISIBLE|  ES_MULTILINE |ES_READONLY|  WS_VSCROLL | WS_HSCROLL,
			0, 0, 0, 0, hwnd, (HMENU)1,hInst, 0);
		DibFileInitialize(hwnd, ofn);
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		MoveWindow(hEdit, 0, 0, cxClient, cyClient, true);
		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		switch (wmId)
		{
		case ID_OPEN:
			ofn.lpstrFileTitle = szTitle;
			ofn.lpstrFile = szPath;
			if (GetOpenFileName(&ofn))
				DisplayDibHeaders(hEdit,szTitle,szPath);
			break;
		default:
			break;
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
	while (GetMessage(&msg, nullptr, 0, 0))//从消息队列获取消息
	{
		TranslateMessage(&msg);//将一些按键消息转换成字符消息放在按键消息后面
		DispatchMessage(&msg);//分发消息，即调用窗口过程进行处理消息
	}
}