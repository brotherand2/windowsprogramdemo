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
#define WM_SOCKET_NOTIFY (WM_USER+1)
int  _stdcall ServerProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam);
void EditPrintf(HWND hEdit, TCHAR *szFormat, ...);
void ChangeLocalTime(HWND hEdit, const SYSTEMTIME &stOld, const SYSTEMTIME &stNew);
static bool upDateTime = false;
void ShowReturnTime(HWND hEdit, unsigned long ulTime)
{
	SYSTEMTIME stNew;
	SYSTEMTIME stOld;
	FILETIME ft;
	LARGE_INTEGER li;
	TCHAR szTime[64], szDate[64];
	GetLocalTime(&stOld);
	stNew.wYear = 1900;
	stNew.wMonth = stNew.wDay = 1;
	stNew.wHour = stNew.wMinute = stNew.wSecond = stNew.wMilliseconds = 0;
	SystemTimeToFileTime(&stNew, &ft);//将1900年1月1日系统时间转换为1601年1月1日到1900年1月1日的文件时间，以100ns为单位
	li = *(LARGE_INTEGER*)&ft;
	li.QuadPart += (LONGLONG)10000000 * ulTime;//再加上1900到今天的文件时间
	ft = *(FILETIME*)&li;//1601年到现在的文件时间
	FileTimeToLocalFileTime(&ft, &ft);//将UTC文件时间转换成本地文件时间
	FileTimeToSystemTime(&ft, &stNew);//将文件时间转换为系统时间
	GetDateFormat(LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE | DATE_SHORTDATE, &stNew, 0, szDate, sizeof(szDate) / sizeof(TCHAR));
	GetTimeFormat(LOCALE_USER_DEFAULT, 0, &stNew, 0, szTime, sizeof(szTime) / sizeof(TCHAR));
	EditPrintf(hEdit, TEXT("\r\n服务器上返回的新时间 %s  %s %d\r\n"), szDate, szTime,stNew.wMilliseconds);
	if (upDateTime)
		ChangeLocalTime(hEdit, stOld, stNew);
}
void ChangeLocalTime(HWND hEdit, const SYSTEMTIME &stOld, const SYSTEMTIME &stNew)
{
	TCHAR szOldTime[64], szOldDate[64];
	TCHAR szTime[64], szDate[64];
	if (SetLocalTime(&stNew))
	{
		GetDateFormat(LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE | DATE_SHORTDATE, &stNew, 0, szDate, sizeof(szDate) / sizeof(TCHAR));
		GetTimeFormat(LOCALE_USER_DEFAULT, 0, &stNew, 0, szTime, sizeof(szTime) / sizeof(TCHAR));
		GetDateFormat(LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE | DATE_SHORTDATE, &stOld, 0, szOldDate, sizeof(szOldDate) / sizeof(TCHAR));
		GetTimeFormat(LOCALE_USER_DEFAULT, 0, &stOld, 0, szOldTime, sizeof(szOldTime) / sizeof(TCHAR));
		EditPrintf(hEdit, TEXT("系统当前时间成功改变，从\r\n\t%s %s.%d to\r\n\t%s %s.%d\r\n"),
			szOldDate, szOldTime, stOld.wMilliseconds, szDate, szTime, stNew.wMilliseconds);
	}
	else
		EditPrintf(hEdit, TEXT("无法修改本地时间，必需以管理员身份运行"));
}
void EditPrintf(HWND hEdit, TCHAR *szFormat, ...)
{
	TCHAR szBuffer[1024];
	va_list pArgList;
	va_start(pArgList, szFormat);
	wvsprintf(szBuffer,szFormat, pArgList);
	va_end(pArgList);
	SendMessage(hEdit, EM_SETSEL, -2, -1);
	SendMessage(hEdit, EM_REPLACESEL, false, (long)szBuffer);
	SendMessage(hEdit,EM_SCROLLCARET, 0, 0);
}
int  _stdcall DlgProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;
	static TCHAR szIPAddress[64] = TEXT("129.6.15.30");
	int iError;
	WSADATA wsaData;
	static sockaddr_in  sa;//保存套接字IP地址与端口号结构
	static HWND hEdit,hButton;
	static SOCKET sock;
	const int ID_TIMER = 1;
	const int ID_TIMERWAIT = 2;
	const int IDC_CANCEL = 2;
	unsigned short wEvent, wError;
	unsigned long ulTime;
	switch (UMsg)
	{
	case WM_INITDIALOG:
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
		hEdit = GetDlgItem(hwnd, IDC_EDIT1);
		hButton = GetDlgItem(hwnd, IDC_SHOWSERVERTIME);
		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		switch (wmId)
		{
		case IDC_SELECTSERVER:
			DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, ServerProc, (long)szIPAddress);
			break;
		case IDC_UPDATETIME:
			upDateTime = true;
		case IDC_SHOWSERVERTIME:
			iError = WSAStartup(MAKELONG(2, 0), &wsaData);//启动2.0版本的套接字
			if (iError)
			{
				EditPrintf(hEdit, TEXT("套接字初始化失败，启动不成功 #error %d\r\n"), iError);
				return true;
			}
			EditPrintf(hEdit, TEXT("套接字 %s 版本启动成功\r\n"), wsaData.szDescription);
			sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建TCP/IP流套接字
			if (sock == INVALID_SOCKET)
			{
				EditPrintf(hEdit, TEXT("套接字创建失败 %d\r\n"), WSAGetLastError());
				WSACleanup();
				return true;
			}
			EditPrintf(hEdit, TEXT("套接字 %d创建\r\n"), sock);
			if (SOCKET_ERROR == WSAAsyncSelect(sock, hwnd, WM_SOCKET_NOTIFY, FD_CONNECT | FD_READ))//将套接字的连接和读取由阻塞式变为非阻塞式
			{//并把连接和读取的消息发送到自定义消息WM_SOCKET_NOTIFY
				EditPrintf(hEdit, TEXT("套接字的连接和读取由阻塞式变为非阻塞式 错误#%d\r\n"), WSAGetLastError());
				closesocket(sock);
				WSACleanup();
				return true;
			}
			//初始化端口号和IP地址
			sa.sin_addr.S_un.S_addr = inet_addr(szIPAddress);//将IP地址从点数格式转换成无符号长整型
			sa.sin_family = AF_INET;
			sa.sin_port = htons(IPPORT_TIMESERVER);//将本地端口号32转换成网络端口号
			connect(sock, (sockaddr*)&sa, sizeof(sa));//连接服务器
			if (WSAEWOULDBLOCK != (iError = WSAGetLastError()))//等于WSAEWOULDBLOCK说明非阻塞式connect已用于非阻塞式
			{
				EditPrintf(hEdit, TEXT("连接失败 错误#%d\r\n"), WSAGetLastError());
				closesocket(sock);
				WSACleanup();
				return true;
			}
			EditPrintf(hEdit, TEXT("连接 %s ..."), szIPAddress);
			SetTimer(hwnd, ID_TIMER, 1000, 0);
			SetTimer(hwnd, ID_TIMERWAIT, 60000, 0);
			SetWindowText(hButton, TEXT("取消"));
			SetWindowLong(hButton, GWL_ID, IDC_CANCEL);
			break;
		case IDC_CANCEL:
			KillTimer(hwnd, ID_TIMER);
			WSACleanup();
			closesocket(sock);
			sock = 0;
			upDateTime = false;
			SetWindowText(hButton, TEXT("连接服务器"));
			SetWindowLong(hButton, GWL_ID,IDC_SHOWSERVERTIME);
			EditPrintf(hEdit, TEXT("\r\n套接字关闭\r\n"));
			break;
		default:
			break;
		}
		break;
	case WM_SOCKET_NOTIFY:
		wError = WSAGETSELECTERROR(lParam);
		wEvent = WSAGETSELECTEVENT(lParam);
		switch (wEvent)
		{
		case FD_CONNECT:
			if (wError)
			{
				EditPrintf(hEdit, TEXT("\r\n连接失败 错误#%d\r\n"), wError);
				SendMessage(hwnd, WM_COMMAND, IDC_CANCEL, 0);
				return true;
			}
			EditPrintf(hEdit, TEXT("\r\n成功连接上 %s\r\n"), szIPAddress);
			recv(sock, (char*)&ulTime, 4, MSG_PEEK);//发送命令让TCP/IP协议开始接收数据
			EditPrintf(hEdit, TEXT("正在等待接收..."));
			break;
		case FD_READ:
			KillTimer(hwnd, ID_TIMER);
			if (wError)
			{
				EditPrintf(hEdit, TEXT("读取失败 #错误%d\r\n"), wError);
				SendMessage(hwnd, WM_COMMAND, IDC_CANCEL, 0);
				return true;
			}
			recv(sock, (char*)&ulTime, 4, 0);//读取接收缓冲区内容
			ulTime = ntohl(ulTime);//将网络时间转换成本地时间
			EditPrintf(hEdit, TEXT("\r\n当前接收的时间是自1990年1月1日经历的时间 %u 秒"), ulTime);
			ShowReturnTime(hEdit, ulTime);
			SendMessage(hwnd, WM_COMMAND, IDC_CANCEL, 0);
			break;
		default:
			break;
		}
		break;
	case WM_TIMER:
		if (wParam==ID_TIMER)
		EditPrintf(hEdit, TEXT("."));
		else
		{
			EditPrintf(hEdit, TEXT("\r\n等待超时,结束连接.\r\n"));
			SendMessage(hwnd, WM_COMMAND, IDC_CANCEL, 0);
		}
		break;
	case WM_CLOSE:
		if (sock)
			SendMessage(hwnd, WM_COMMAND, IDC_CANCEL, 0);
		DestroyWindow(hwnd);
		break;
	default:
		break;
	}
	return false;
}
int  _stdcall ServerProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	static int server=IDC_RADIO1;
	TCHAR szLabel[64];
	static TCHAR *szServer;
	char *context;
	switch (UMsg)
	{
	case WM_INITDIALOG:
		szServer = (TCHAR*)lParam;
		CheckRadioButton(hwnd, IDC_RADIO1, IDC_RADIO10, server);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO1:
		case IDC_RADIO2:
		case IDC_RADIO3:
		case IDC_RADIO4:
		case IDC_RADIO5:
		case IDC_RADIO6:
		case IDC_RADIO7:
		case IDC_RADIO8:
		case IDC_RADIO9:
		case IDC_RADIO10:
			server = LOWORD(wParam);
			break;
		case IDOK:
			GetDlgItemText(hwnd, server, szLabel, sizeof(szLabel) / sizeof(TCHAR));
			strtok_s(szLabel, "(",&context);
			lstrcpy(szServer, strtok_s(0, ")",&context));
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default:
		break;
	}
	return false;
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
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
	wndclass.lpszMenuName = nullptr;//菜单
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//哪些改变后要重画，设置水平和垂直改变后重绘
	if (!RegisterClass(&wndclass))//注册窗口类
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("勇华"), WS_OVERLAPPEDWINDOW&(~(WS_MAXIMIZEBOX | WS_THICKFRAME)),
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);//创建窗口
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
		return 0;
	}
	HWND hwndModeLess = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DlgProc);

	RECT rect;
	GetClientRect(hwndModeLess, &rect);
	AdjustWindowRect(&rect, WS_CAPTION | WS_BORDER, false);
	SetWindowPos(hwnd, 0, 0, 0, rect.right-rect.left, rect.bottom-rect.top, SWP_NOMOVE);
	ShowWindow(hwnd, nCmdShow);//显示窗口
	UpdateWindow(hwnd);//进行重绘
	ShowWindow(hwndModeLess, SW_SHOW);

	MSG msg;//消息结构
	while (GetMessage(&msg, nullptr, 0, 0))//从消息队列获取消息
	{
		if (hwndModeLess == 0 || !IsDialogMessage(hwndModeLess, &msg))
		{
			TranslateMessage(&msg);//将一些按键消息转换成字符消息放在按键消息后面
			DispatchMessage(&msg);//分发消息，即调用窗口过程进行处理消息
		}
	}
}