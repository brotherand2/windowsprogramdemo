//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif

#include <Windows.h>
//#include <commdlg.h>
#include"resource.h"

#include<string>
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
int DialogProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)
{
	int wmId = LOWORD(wParam);//控件ID
	int wmEvent = HIWORD(wParam);//控件通知码，则操作
	switch (message)
	{
		//case WM_COMMAND:

		//	return false;
	case WM_CLOSE:
		EndDialog(hDlg, 0);//销毁对话框
		return false;
	}
	return false;//如果没有处理，返回0,会调用默认对话框过程，非0则处理了，不调用默认的
}
void InitOpenFileName(OPENFILENAME &ofn,HWND hwnd,TCHAR szFileName[],TCHAR szTitle[])
{
	static TCHAR szFilter[] = TEXT("文本文件(*.txt)\0*.txt\0编程文件(*.c;*.cpp;*.java)\0*.c;*.cpp;*.java\0所有文件(*.*)\0*.*\0\0");//过滤区字符串
	ofn.lStructSize = sizeof(ofn);//结构的大小
	ofn.hwndOwner = hwnd;//指向所有者对话框窗口的句柄。这个成员可以是任意有效窗口句柄，或如果对话框没有所有者它可以为NULL。
	ofn.hInstance = nullptr;
	ofn.lpstrFilter = szFilter;//指向一对以空字符结束的过滤字符串的一个缓冲。缓冲中的最后一个字符串必须以两个NULL字符结束
	ofn.lpstrCustomFilter = nullptr;//指向一个静态缓冲，它包含了一对以空字符结束的过滤器字符串，这个字符串是为了保留用户选择的过滤样式
	ofn.nMaxCustFilter = 0;
	//指定特意为lpstrCustomFilter准备的以TCHARs为单位的缓冲大小。对于ANSI版本，是字节的个数；对于Unicode版本，是字符的个数。这缓冲应该最小在40个字符长。如果lpstrCustomFilter成员是NULL或是指向NULL的字符串，这个成员被忽略
	ofn.nFilterIndex = 2;//显示文件类型的第2个索引，即编程文件
	//指定在文件类型控件中当前选择的过滤器的索引。缓冲指向被lpstrFilter包含的一对定义了的过滤器的字符串。过滤器的第一对字符串的索引值为1，第二对为2，等等
	ofn.lpstrFile = szFileName;//当GetOpenFileName或GetSaveFileName函数返回成功时，这个缓冲包含驱动器，路径，文件名，及所选择的文件的扩展名
	ofn.nMaxFile = MAX_PATH;//指定lpstrFile缓冲的大小
	ofn.lpstrFileTitle = szTitle;//指向接收选择的文件的文件名和扩展名的缓冲（不带路径信息）。这个成员可以是NULL
	ofn.nMaxFileTitle = MAX_PATH;//指定lpstrFileTitle缓冲的大小
	ofn.lpstrInitialDir = TEXT("e:");//指向以空字符结束的字符串，可以在这个字符串中指定初始目录
	ofn.lpstrTitle = TEXT("亲，选一个文件呗");//指向在对话框的标题栏中放置的字符串。如果这个成员是NULL，系统使用默认标题（另存为或打开)
	ofn.Flags = OFN_CREATEPROMPT | OFN_HIDEREADONLY;//位标记的设置，你可以使用来初始化对话框
	//OFN_ALLOWMULTISELECT 指定文件名列表框允许多选,OFN_HIDEREADONLY 隐藏只读复选框。
	// OFN_CREATEPROMPT如果用户指定了一个不存在的文件，这个标记使用对话框能提示用户是否新建这个文件
	ofn.nFileOffset = 0;//返回指定从路径开始到通过lpstrFile指定的文件名字符串基于0的偏移，文件名第1个字母在路径中偏移
	ofn.nFileExtension = 0;//指定从路径开始到通过lpstrFile指定的文件名字符串中扩展名基于0的偏移
	ofn.lpstrDefExt = TEXT("cpp");//指向包含默认扩展名的缓冲
	ofn.lCustData = 0;//指定应用程序定义的数据，这数据是能被lpfnHook成员识别的系统传到的钩子程序
	ofn.lpfnHook = 0;//指向一个钩子程序。除非Flags成员中包含OFN_ENABLEHOOK标记，要么这个成员将被忽略
	ofn.lpTemplateName = 0;//指向一个以空字符结束的字符串，字符串是对话框模板资源的名字，资源保存在能被hInstance成员识别的模块中

}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	TCHAR szFileName[MAX_PATH]=TEXT("");//保存文件路径名包括文件名
	//初始化文件名编辑控件使用的文件名的缓冲。如果不需要初始值，这个缓冲的第一个字符必须是NULL
	TCHAR szTitle[MAX_PATH];//保存文件名不包括路径
	static OPENFILENAME ofn;//用来初始化打开或另存为对话框的信息
	static CHOOSEFONT cf;//字体结构
	static CHOOSECOLOR cc;//颜色结构
	LOGFONT lf;//逻辑字体
	HFONT hFont;//字体句柄
	TCHAR szBuffer[] = TEXT("This a test line 中国人---》综正");//测试用的字符串
	static COLORREF color[16];
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
		case ID_FILE_OPEN://文件打开
			InitOpenFileName(ofn, hwnd, szFileName, szTitle);
			if (GetOpenFileName(&ofn))//创建一个Open公共对话框，使用户指定驱动器、目录和文件名、或使用户打开文件,如果用户指定了一个文件名，点击OK按钮，返回值为非零
				MessageBox(hwnd, szFileName, TEXT("你选择的是:"), MB_OK);
			return 0;
		case ID_FILE_SAVE://文件保存
			InitOpenFileName(ofn, hwnd, szFileName, szTitle);
			ofn.Flags = OFN_OVERWRITEPROMPT;//如果选择的文件已经存在，使用另存为对话框产生一个消息框。用户必须确认是否覆盖这个文件。
			if (GetSaveFileName(&ofn))//创建一个Save公共对话框，以便用户指定驱动器、目录和文件名
				MessageBox(hwnd, szFileName, TEXT("你选择保存的路径是:"), 0);
			return 0;
		case ID_FILE_SAVEAS://文件另存为

			return 0;
		case ID_FILE_COLOR://文件颜色
			cc.lStructSize = sizeof(cc);//指定结构的长度（字节
			cc.Flags = CC_FULLOPEN | CC_RGBINIT ;//一个可以让你初始化颜色对话框的位集。当对话框返回时，它用来这些标识符来标识用户的输入
			//CC_FULLOPEN让对话框显示额外的控件以使用户创建自定义的颜色
			//CC_RGBINIT让对话框默认使用由rgbResult成员指定的颜色
			cc.hInstance = nullptr;
			cc.hwndOwner = hwnd;//拥有对话框的窗口的句柄。该成员可以是任意有效的窗口句柄，或在对话框没有所有者时，可为NULL
			cc.lCustData = 0;//指定应用程序自定义的数据，该数据会被系统发送给钩子程序
			cc.lpfnHook = 0;//指向CCHookProc钩子程序的指针，该钩子可以处理发送给对话框的消息
			cc.lpTemplateName = 0;//指向一个NULL结尾的字符串，该字符串是对话框模板资源的名字
			cc.rgbResult = RGB(255, 0, 0);//如果CC_RGBINIT标识符被设置时，该成员指定了对话框打开时默认的选择颜色
			cc.lpCustColors = color;//指向一个包含16个值的数组，该数组包含了对话框中自定义颜色的红、绿、蓝（RGB）值
			if (ChooseColor(&cc))//该函数创建一个能使用户从中选择颜色的通用颜色对话框
			{
				DeleteObject((HGDIOBJ)SetClassLong(hwnd,GCL_HBRBACKGROUND,(long)CreateSolidBrush(cc.rgbResult)));//将用新的颜色创建新的画刷，放入背景画刷，并删除旧的画刷
				InvalidateRect(hwnd, nullptr, true);//让窗口用背景重绘
			};
			return 0;
		case ID_FILE_FONT://文件字体
			cf.Flags = CF_EFFECTS | CF_NOVERTFONTS;//使对话框显示控件允许用户指定下划线,和文本颜色选项。 如果设置此标志,您可以使用rgbColors成员指定初始文本颜色
			// CF_NOVERTFONTS导致字体对话框列表只面向水平的字体,或未设置则是垂直的
			cf.hDC = nullptr;
			cf.hInstance = nullptr;
			cf.hwndOwner = hwnd;
			cf.iPointSize = 0;//选择字体的大小
			cf.lCustData = 0;
			cf.lpfnHook = 0;
			cf.lpLogFont = &lf;//逻辑字体
			cf.lStructSize = sizeof(cf);
			cf.lpszStyle = 0;//字体风格
			cf.lpTemplateName = 0;
			cf.nFontType = 0;//返回的字体类型
			cf.nSizeMax = 0;//用户可以选择最大点大小
			cf.nSizeMin = 0;// 用户可以选择最小点大小
			cf.rgbColors = RGB(255, 0, 0);//初始文本颜色
			if (ChooseFont(&cf))
			{
				hFont = CreateFontIndirect(&lf);//根据创建字体
				hdc = GetDC(hwnd);
				hFont=(HFONT)SelectObject(hdc, hFont);//将字体选入画笔
				TextOut(hdc, 50, 20, szBuffer, lstrlen(szBuffer));
				hFont = (HFONT)SelectObject(hdc, hFont);//将旧画笔放回去
				DeleteObject(hFont);//删除创建 的画笔
				DeleteDC(hdc);
			}
			return 0;

		case ID_APP_EXIT://程序退出
			SendMessage(hwnd, WM_CLOSE, 0, 0);
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