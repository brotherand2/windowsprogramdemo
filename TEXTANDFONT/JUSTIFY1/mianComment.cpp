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
void DrawRuler(HDC hdc, POINT pt)
{
	SaveDC(hdc);
	static int justify[16] = {360,72,144,72,216,72,144,72,288,72,144,72,216,72,144,72};
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, 1440, 1440, 0);
	SetViewportExtEx(hdc, GetDeviceCaps(hdc, LOGPIXELSX), GetDeviceCaps(hdc, LOGPIXELSY), 0);
	SetWindowOrgEx(hdc, -720, -720, 0);
	DPtoLP(hdc, &pt, 1);
	pt.x = pt.x - 360;
	MoveToEx(hdc, 0, -360, 0);
	LineTo(hdc, pt.x, -360);
	MoveToEx(hdc, -360, 0, 0);
	LineTo(hdc, -360, pt.y);
	for (int i = 0, j = 0; i < pt.x; i += 1440 / 16, j++)
	{
		MoveToEx(hdc, i, -360, 0);
		LineTo(hdc, i, -360 - justify[j % 16]);
	}
	for (int i = 0, j = 0; j < pt.y; i +=1440 / 16, j++)
	{
		MoveToEx(hdc, -360, i, 0);
		LineTo(hdc, -360 - justify[j % 16], i);
	}
	RestoreDC(hdc, -1);
}
void Justify(HDC hdc, TCHAR *pText, RECT rect, int Align)
{
	if (!pText||*pText=='\0')
		return;
	TCHAR *pBegin, *pEnd;
	SIZE size;
	int xStart, yStart = rect.top;
	while (*pText != '\0')
	{
		int space = 0;
		while (*pText == ' ')
			pText++;
		pBegin = pText;
		do
		{
			pEnd = pText;
			while (*pText!='\0'&&*pText++ != ' ')
				;
			if (*pText == '\0')
				break;
			space++;
			GetTextExtentPoint32(hdc, pBegin, pText - pBegin - 1, &size);
		} while (size.cx < rect.right - rect.left);
		space--;
		while (*(pEnd - 1) == ' ')
		{
			pEnd--;
			space--;
		}
		if (*pText == '\0')
		{
			GetTextExtentPoint32(hdc, pBegin, pText - pBegin - 1, &size);
			if (size.cx < rect.right - rect.left)
			   pEnd = pText;
		}
		GetTextExtentPoint32(hdc, pBegin, pEnd-pBegin, &size);
		switch (Align)
		{
		case ID_ALIGN_LEFT:
			xStart = rect.left;
			break;
		case ID_ALIGN_RIGHT:
			xStart = rect.right - size.cx;
			break;
		case ID_ALIGN_CENTER:
			xStart = (rect.left + rect.right - size.cx) / 2;
			break;
		case ID_ALIGN_JUSTIFIED:
			if (*pText != '\0'&&space > 0)
				SetTextJustification(hdc, rect.right - rect.left - size.cx, space);
			xStart = rect.left;
		default:
			break;
		}
		TextOut(hdc, xStart, yStart, pBegin, pEnd - pBegin);
		SetTextJustification(hdc, 0, 0);
		yStart += size.cy;
		pText = pEnd;
	}
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	POINT pt;
	static TCHAR *pText;
	HANDLE hFile;
	unsigned long dwBytesRead,dwFileSize,dwHighSize;
	static HFONT hFont;
	static CHOOSEFONT cf;
	static LOGFONT lf;
	RECT rect;
	static int align = ID_ALIGN_LEFT;
	static DOCINFO di = { sizeof(DOCINFO), TEXT("JUSTIFY1") };
	static PRINTDLG pd = { sizeof(PRINTDLG) };
	static HMENU hMenu;
	bool bSuccess;
	int cxPage, cyPage;
	int iSaveFontHeigh;
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		hMenu = GetMenu(hwnd);
		GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);
		cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_EFFECTS;
		cf.hwndOwner = hwnd;
		cf.lpLogFont = &lf;
		cf.lStructSize = sizeof(cf);
		hFile = CreateFile(TEXT("text.txt"), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hwnd, TEXT("找不到text.txt"), 0, 0);
			return 0;
		}
		dwFileSize = GetFileSize(hFile, &dwHighSize);
		pText =(TCHAR*) malloc(dwFileSize);
		ReadFile(hFile, pText, dwFileSize, &dwBytesRead, 0);
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
		case ID_PRINT:
			pd.Flags = PD_RETURNDC;
			if (!PrintDlg(&pd))
				return 0;
			hdc = pd.hDC;
			if (!hdc)
				MessageBox(hwnd, TEXT("can not cant dc"), 0, 0);
			bSuccess = false;
			rect.left = GetDeviceCaps(hdc, LOGPIXELSX);
			rect.top = GetDeviceCaps(hdc, LOGPIXELSY);
			rect.right = GetDeviceCaps(hdc, PHYSICALWIDTH) - rect.left;
			rect.bottom = GetDeviceCaps(hdc, PHYSICALHEIGHT) - rect.top;
			if (StartDoc(hdc, &di) > 0 && StartPage(hdc) > 0)
			{
				iSaveFontHeigh = lf.lfHeight;
				lf.lfHeight = -(cf.iPointSize*GetDeviceCaps(hdc, LOGPIXELSY)) / 720;
				SelectObject(hdc, CreateFontIndirect(&lf));
				lf.lfHeight = iSaveFontHeigh;
				SetTextColor(hdc, cf.rgbColors);
				Justify(hdc, pText, rect, align);
				if (EndPage(hdc) > 0)
				{
					bSuccess = true;
					EndDoc(hdc);
				}
			}
			DeleteDC(hdc);
			if (!bSuccess)
				MessageBox(hwnd, TEXT("打印失败"), 0, 0);
			break;
		case ID_FONT:
			if (ChooseFont(&cf))
				InvalidateRect(hwnd, 0, true);
			break;
		case ID_ALIGN_CENTER:
		case ID_ALIGN_JUSTIFIED:
		case ID_ALIGN_LEFT:
		case ID_ALIGN_RIGHT:
			CheckMenuItem(hMenu, align, MF_UNCHECKED);
			align = wmId;
			CheckMenuItem(hMenu, align, MF_CHECKED);
			InvalidateRect(hwnd, 0, true);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		pt.x = cxClient;
		pt.y = cyClient;
		DrawRuler(hdc,pt);
		GetClientRect(hwnd, &rect);
		rect.left = GetDeviceCaps(hdc,LOGPIXELSX) / 2;
		rect.top = GetDeviceCaps(hdc, LOGPIXELSY) / 2;
		rect.right -= GetDeviceCaps(hdc, LOGPIXELSX) / 4;
		SelectObject(hdc, CreateFontIndirect(&lf));
		SetTextColor(hdc, cf.rgbColors);
		Justify(hdc, pText, rect, align);
		DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
		EndPaint(hwnd, &ps);//释放画笔资源
		break;
	case WM_CLOSE://窗口关闭
		DestroyWindow(hwnd);//销毁窗口
		break;
	case WM_DESTROY://窗口销毁时
		if (pText)
			free(pText);
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
