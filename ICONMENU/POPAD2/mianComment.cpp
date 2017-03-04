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
int AskConfirmation()//程序退出时询问是否退出
{
	return	MessageBox(nullptr, TEXT("Really want to quit Notepad?"), TEXT("Quit"), MB_ICONQUESTION|MB_YESNOCANCEL);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HWND hEdit;//编辑框
	static HINSTANCE hInst;//窗口实例句柄

	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE|WS_VSCROLL|ES_MULTILINE, 0, 0, 0, 0,
			hwnd, (HMENU)ID_EDIT, hInst, nullptr);//创建有边界的多行编辑框
		
		return 0;
	case WM_SETFOCUS://窗口获得焦点时
		SetFocus(hEdit);//让编辑框获得焦点
		return 0;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		MoveWindow(hEdit, 0, 0, cxClient, cyClient, true);//调整编辑大小为整个客户区
		return 0;
	case WM_INITMENUPOPUP://菜单弹出时的消息项初始化后才会弹出
		if (LOWORD(lParam) == 1)//如果索引为1，即是弹出编辑菜单
		{//wParam参数是弹出式菜单的句柄，lParam的低位是弹出式菜单的索引
			//如果该菜单是系统菜单，那么高位是1，否则为0
			EnableMenuItem((HMENU)wParam, IDM_EDIT_UNDO, SendMessage(hEdit, EM_CANUNDO, 0, 0) ? MF_ENABLED : MF_GRAYED);
			//先发送一条消息到编辑框，如果可撤销，则撤销菜单项可用，否则，不可用
			EnableMenuItem((HMENU)wParam, IDM_EDIT_PASTE, IsClipboardFormatAvailable(CF_TEXT) ? MF_ENABLED : MF_GRAYED);
			//如果剪贴板有文字，则剪切菜单可用，否则，置为灰色
			long isSelected = SendMessage(hEdit, EM_GETSEL, 0, 0);//获得当前被选文字的开始和结束位置
			int isEnable = (LOWORD(isSelected) == HIWORD(isSelected));//isSelected低位保存着开始位置，高位保存结束位置，如果开始结束位置相等说明未选中
			if (isEnable)//开始结束位置相等，说明未选中文字
				isEnable = MF_GRAYED;
			else
				isEnable = MF_ENABLED;
			EnableMenuItem((HMENU)wParam, IDM_EDIT_COPY, isEnable);
			EnableMenuItem((HMENU)wParam, IDM_EDIT_CUT, isEnable);
			EnableMenuItem((HMENU)wParam, IDM_EDIT_CLEAR, isEnable);
		}
		return 0;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		if (wmId == ID_EDIT&&wmEvent == EN_ERRSPACE || wmEvent == EN_MAXTEXT)//如果编辑框字符超过30000个字符
		{
			MessageBox(nullptr, TEXT("Edit control out of space"), TEXT("Error"), MB_ICONSTOP);
			return 0;
		}
		switch (wmId)
		{
		case IDM_FILE_NEW ://新建文件
		case IDM_FILE_OPEN ://打开文件
		case IDM_FILE_SAVE ://保存文件
		case IDM_FILE_SAVEAS ://另存为
		case IDM_FILE_PAGESET ://页面设置
		case IDM_FILE_PRINT ://打印
			MessageBeep(0);
			return 0;
		case IDM_APP_EXIT ://退出
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;

		case IDM_EDIT_FIND: //查找
		case IDM_EDIT_FINDNEXT://查找下一个
		case IDM_EDIT_REPLACE ://替换
		case IDM_EDIT_GOTO: //转到
		case IDM_EDIT_TIMEDATE://时间/日期
			MessageBeep(0);
			return 0;
		case IDM_EDIT_SELECTALL://全选
			SendMessage(hEdit, EM_SETSEL, 0, -1);//0表示从第1个字符开始，-1表示到最后1个字符
			return 0;//发送消息让编辑框默认的窗口过程处理
		case IDM_EDIT_UNDO :  //撤销
			SendMessage(hEdit, WM_UNDO, 0, 0);//发送消息让编辑框默认的窗口过程处理
			return 0;
		case IDM_EDIT_CUT ://剪切
			SendMessage(hEdit, WM_CUT, 0, 0);//发送消息让编辑框默认的窗口过程处理
			return 0;
		case IDM_EDIT_COPY :  //复制
			SendMessage(hEdit, WM_COPY, 0, 0);//发送消息让编辑框默认的窗口过程处理
			return 0;
		case IDM_EDIT_PASTE:  //粘贴
			SendMessage(hEdit, WM_PASTE, 0, 0);//发送消息让编辑框默认的窗口过程处理
			return 0;
		case IDM_EDIT_CLEAR://删除选中   未处理删除后面
			SendMessage(hEdit, WM_CLEAR, 0, 0);
			return 0;
		case IDM_FORMAT_AUTOLINE://自动换行
		case IDM_FORMAT_FONT://字体
		case IDM_LOOKUP_STATE://状态栏
			MessageBeep(0);
			return 0;
		case IDM_APP_HELP://帮助
			MessageBox(nullptr, TEXT("Notepad Help not yet implement"), TEXT("help and support"), MB_OK);
			return 0;
		case IDM_APP_ABOUT: //关于
			MessageBox(nullptr, TEXT("Notepad Program\n(c)yong hua by,2015"), TEXT("关于Menu"), MB_ICONINFORMATION | MB_OK);
			return 0;
		}
		return 0;
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔

		EndPaint(hwnd, &ps);//释放画笔资源
		return 0;
	case WM_QUERYENDSESSION://关机时询问是否退出，点确定才退出关机，否则会取消关机
		if (IDYES == AskConfirmation())//窗口关闭时询问是否退出，点确定才退出
			return 1;//当结束对话或者一个程序调用系统关闭功能的时候，WM_QUERYENDSESSION消息会被发送给尚未终止的所有窗口。当程序在处理这个消息的时候，如果返回了false（0），那么系统将不结束对话或者关机（注销）
		else
			return 0;
	case WM_CLOSE://窗口关闭
		if (IDYES == AskConfirmation())//窗口关闭时询问是否退出，点确定才退出
		DestroyWindow(hwnd);//销毁窗口
		return 0;
	case WM_DESTROY://窗口销毁时
		PostQuitMessage(0);//抛出一条返回值为0退出消息
		return 0;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//用默认的窗口过程处理其它消息，做些其它处理
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	HACCEL hAccel;//加速键（快捷键）句柄
	WNDCLASS wndclass;//窗口类
	wndclass.cbClsExtra = 0;//为类分配的额外空间，可以保存一些地址、变量
	wndclass.cbWndExtra = 0;//为窗口分配的额外空间，可以保存一些地址、变量
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//背景画刷
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//鼠标光标
	wndclass.hIcon = LoadIcon(hInstance,TEXT("POPAD"));//图标
	wndclass.hInstance = hInstance;//实例名柄，程序的PID，一个整数，根据这个整数，系统可以找到进程的地址
	wndclass.lpfnWndProc = WndProc;//窗口过程
	wndclass.lpszClassName = TEXT("zeng");//类名
	wndclass.lpszMenuName = TEXT("POPADMENU");//菜单
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
	hAccel = LoadAccelerators(hInstance, TEXT("POPAD"));//从资源文件读取加速键句柄
	while (GetMessage(&msg, nullptr, 0, 0))//从消息队列获取消息
	{
		if (!TranslateAccelerator(hwnd, hAccel, &msg))
		{//TranslateAccelerator，函数功能：翻译加速键表。该函数处理菜单命令中的加速键。
			//该函数将一个WM_KEYDOWN或WM_SYSKEYDOWN消息翻译成一个WM_COMMAND或WM_SYSCOMMAND消息（如果在给定的加速键表中有该键的入口），
			//然后将WM_COMMAND或WM_SYSCOMMAND消息直接送到相应的窗口处理过程
			//若TransLateAccelerator返回非零值且消息已被翻译，说明从加速捷表中找到了对应的按键，直接调用窗口过程
			//直到窗口过程处理完消息后才返回，应用程序就不能调用TranslateMessage函数对消息再做处理，若没找到，不翻译，返回0
			TranslateMessage(&msg);//将一些按键消息转换成字符消息放在按键消息后面
			DispatchMessage(&msg);//分发消息，即调用窗口过程进行处理消息
		}
	}
}