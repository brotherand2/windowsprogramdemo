//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<memory>
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
const int ID_LISTBOXLEFT = 1;
const int ID_LISTBOXMIDDLE = 2;
const int ID_LISTBOXRIGHT = 3;
const int ID_MOVERIGHT1 = 4;
const int ID_MOVERIGHT2 = 5;
const int ID_ADDTHREESTRINGLEFT = 6;
const int ID_DELETESTRING = 7;
const int ID_RESET = 8;
const int ID_SETTHREE = 9;
const int ID_UNSET = 10;
const int ID_ADDTHREESTRINGRIGHT = 11;
const int ID_INSERTPOSTHREE = 12;
const int ID_SETTHREEANDSIX = 13;
const int ID_UNSETTHREEANDSIX = 14;
const int ID_SETALL = 15;

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	static HWND hListBoxLeft, hListBoxMiddle, hListBoxRight;
	int index;//索引
	int len;//字符串长度
	static TCHAR *p;//字符串指针
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		hListBoxLeft = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY|LBS_SORT
			, cxChar, cyChar,18*cxChar+GetSystemMetrics(SM_CXVSCROLL), 9*cyChar, hwnd, (HMENU)ID_LISTBOXLEFT, hInst, nullptr);
		hListBoxMiddle = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY|LBS_MULTIPLESEL
			,33*cxChar, cyChar, 18 * cxChar + GetSystemMetrics(SM_CXVSCROLL), 9 * cyChar, hwnd, (HMENU)ID_LISTBOXLEFT, hInst, nullptr);
		hListBoxRight = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY
			, 65*cxChar, cyChar, 18 * cxChar + GetSystemMetrics(SM_CXVSCROLL), 9 * cyChar, hwnd, (HMENU)ID_LISTBOXLEFT, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT(">"), WS_CHILD | WS_VISIBLE,24*cxChar,4*cyChar, 6*cxChar,2*cyChar,
			hwnd, (HMENU)ID_MOVERIGHT1, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT(">"), WS_CHILD | WS_VISIBLE,56*cxChar, 4*cyChar,6*cxChar,2*cyChar,
			hwnd, (HMENU)ID_MOVERIGHT2, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("增加3个"), WS_CHILD | WS_VISIBLE,cxChar,11*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_ADDTHREESTRINGLEFT, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("删除第3个"), WS_CHILD | WS_VISIBLE,cxChar,14*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_DELETESTRING, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("重置"), WS_CHILD | WS_VISIBLE,cxChar,17*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_RESET, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("选中第3个"), WS_CHILD | WS_VISIBLE,cxChar,20*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_SETTHREE, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("取消选中的"), WS_CHILD | WS_VISIBLE,cxChar,23*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_UNSET, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("增加3个"), WS_CHILD | WS_VISIBLE,33*cxChar,11*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_ADDTHREESTRINGRIGHT, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("在第3个位置插入"), WS_CHILD | WS_VISIBLE,33*cxChar,14*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_INSERTPOSTHREE, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("选中第3和第6个"), WS_CHILD | WS_VISIBLE,33*cxChar,17*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_SETTHREEANDSIX, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("取消选中第3和第6个"), WS_CHILD | WS_VISIBLE,33*cxChar,20*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_UNSETTHREEANDSIX, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("全部选中"), WS_CHILD | WS_VISIBLE,33*cxChar,23*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_SETALL, hInst, nullptr);
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
		case ID_MOVERIGHT1://从列表框1中选的的移到列表框2
			index = SendMessage(hListBoxLeft, LB_GETCURSEL, 0, 0);
			if (index == LB_ERR)//如果当前没有选中
				return 0;
			len = SendMessage(hListBoxLeft, LB_GETTEXTLEN, index, 0);//获取选中的字符串长度
			p = new TCHAR[len + 1];//动态分配len+1个字符空间
			{
				shared_ptr<TCHAR>sp(p, [](TCHAR *p){delete[]p;});//将分配的空间交智能指针管理
				SendMessage(hListBoxLeft, LB_GETTEXT, index, (long)p);//获取选中的字符串
				SendMessage(hListBoxMiddle, LB_ADDSTRING, -1, (long)p);//将字符串插入第2 个列表框
				SendMessage(hListBoxLeft, LB_DELETESTRING, index, 0);//从列表框中删除选中的字符串

			}
			return 0;
		case ID_MOVERIGHT2://从列表框2中选的的移到列表框3
		{
			int count = SendMessage(hListBoxMiddle, LB_GETCOUNT, 0, 0);//获取列表框2选中的个数
			for (int i = count - 1; i >= 0; i--)
			{
			    int isSelect = SendMessage(hListBoxMiddle, LB_GETSEL, i, 0);//判断当前索引是否选中
				if (isSelect != 0)//当前索引选中了
				{
					len = SendMessage(hListBoxMiddle, LB_GETTEXTLEN, i, 0);//获取选中字符串的长度
					p = new TCHAR[len + 1];//动态分配空间
					shared_ptr<TCHAR>sp(p, [](TCHAR *p){delete[]p; });//将分配的空间交智能指针管理
					SendMessage(hListBoxMiddle, LB_GETTEXT, i, long(p));//获取选中的字符串
					SendMessage(hListBoxRight, LB_ADDSTRING, 0, long(p));//将获取的字符串插入列表3
					SendMessage(hListBoxMiddle, LB_DELETESTRING, i, 0);//将选中的字符串删除
				}
			}
		}
			return 0;
		case ID_ADDTHREESTRINGLEFT://在列表框中增加3个字符串
			SendMessage(hListBoxLeft, LB_ADDSTRING, 0, (long)TEXT("hello"));
			SendMessage(hListBoxLeft, LB_ADDSTRING, 0, (long)TEXT("windows"));
			SendMessage(hListBoxLeft, LB_ADDSTRING, 0, (long)TEXT("world"));
			return 0;
		case ID_DELETESTRING://删除第3个
			SendMessage(hListBoxLeft, LB_DELETESTRING, 2, 0);
			return 0;
		case ID_RESET://重置为空
			SendMessage(hListBoxLeft, LB_RESETCONTENT, 0, 0);
			return 0;
		case ID_SETTHREE://选中第3个
			SendMessage(hListBoxLeft, LB_SETCURSEL, 2, 0);
			return 0;
		case ID_UNSET:
			index = SendMessage(hListBoxLeft, LB_GETCURSEL, 0, 0);//获得选中的索引
			if (index != LB_ERR)//如果选中了，取消选中
				SendMessage(hListBoxLeft, LB_SETCURSEL, -1, 0);
			return 0;
		case ID_ADDTHREESTRINGRIGHT://在第2个列表框插入3个字符串
			SendMessage(hListBoxMiddle, LB_ADDSTRING, -1, long(TEXT("one")));
			SendMessage(hListBoxMiddle, LB_ADDSTRING, -1, long(TEXT("two")));
			SendMessage(hListBoxMiddle, LB_ADDSTRING, -1, long(TEXT("three")));
			return 0;
		case ID_INSERTPOSTHREE:
			SendMessage(hListBoxMiddle, LB_INSERTSTRING, 2, long(TEXT("nihao")));//在第2个列表框第3个位置插入个字符串
			return 0;
		case ID_SETTHREEANDSIX://选中第3个和第6个字符串
			SendMessage(hListBoxMiddle, LB_SETSEL,1, 2);
			SendMessage(hListBoxMiddle, LB_SETSEL,1, 5);
			return 0;
		case ID_UNSETTHREEANDSIX://取消选中第3个和第6个字符串
			SendMessage(hListBoxMiddle, LB_SETSEL,0, 2);
			SendMessage(hListBoxMiddle, LB_SETSEL,0, 5);
			return 0;
		case ID_SETALL:
			SendMessage(hListBoxMiddle, LB_SETSEL, 1, -1);
			return 0;
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
	wndclass.hbrBackground =CreateSolidBrush(GetSysColor(COLOR_BTNFACE));//背景画刷
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