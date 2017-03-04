//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
using namespace std;
//#ifndef UNICODE 
//typedef  string tstring;
//#else
//typedef  wstring tstring;
//#endif
#ifndef UNICODE 
using  tstring = string;
#else
using  tstring = wstring;
#endif
const int  ID_TEXT = 1;
const int  ID_LIST = 2;
WNDPROC oldProc;
long _stdcall ListProc(HWND hwnd, unsigned int message, unsigned int wParam, long lParam)//重写列表框窗口过程
{
	if (message == WM_KEYDOWN&&wParam == VK_RETURN)//按下回车键
		SendMessage(GetParent(hwnd), WM_COMMAND, MAKELONG(ID_LIST, LBN_DBLCLK), (long)hwnd);//设置等同于双击的效果
	return CallWindowProc(oldProc, hwnd, message, wParam, lParam);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;		
	HDC hdc;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	int wmId, wmEvent;
	static HWND hText, hList;
	HINSTANCE hInst;
	TCHAR path[MAX_PATH];
	static TCHAR szBuffer[MAX_PATH];//保留文件名
	static RECT rect;//显示文本的区域
	HANDLE hFile;
	switch (UMsg)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
		hText = CreateWindow(TEXT("static"), TEXT(""), WS_CHILD | WS_VISIBLE,0,0,0,0,
			hwnd, (HMENU)ID_TEXT, hInst, nullptr);
		hList = CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("listbox"), TEXT(""), WS_CHILD | WS_VISIBLE | LBS_NOTIFY | LBS_SORT | WS_VSCROLL, cxChar, 3 * cyChar, 30 * cxChar, 10 * cyChar,
			hwnd,(HMENU)ID_LIST,hInst,nullptr);
		SendMessage(hList, LB_DIR, DDL_ARCHIVE | DDL_DIRECTORY | DDL_DRIVES | DDL_EXCLUSIVE |
			DDL_HIDDEN | DDL_POSTMSGS | DDL_READONLY | DDL_READWRITE | DDL_SYSTEM,(long) TEXT("*.*"));
		
		GetCurrentDirectory(MAX_PATH, path);
		SetWindowText(hText, path);
		rect.left = 35 * cxChar;//从列表框的右边移2个字符
		rect.top = 3 * cyChar;//高度与列表框一样
		oldProc = (WNDPROC)SetWindowLong(hList, GWL_WNDPROC,(long)ListProc);//设置列表框新的窗口过程，返回旧的窗口过程
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		rect.right = cxClient;
		rect.bottom = cyClient;
		MoveWindow(hText, cxChar, cyChar, cxClient - cxChar, cyChar, true);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		if (wmId == ID_LIST&&wmEvent == LBN_DBLCLK)
		{
			int index = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (index == LB_ERR)
				break;
			SendMessage(hList, LB_GETTEXT, index, (long)szBuffer);
			GetCurrentDirectory(MAX_PATH, path);//获得当前目录
			hFile = CreateFile(szBuffer, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, nullptr);
			if (hFile != INVALID_HANDLE_VALUE)//当前选中的是个文件
			{
				CloseHandle(hFile);//关闭名柄
				tstring str(path);//str当前路径
				if (path[lstrlen(path )- 1] != '\\')//如果是类似c:\则后面不再加\,如果是类似c\a则在后面加1个
					str += TEXT("\\");
				str+= szBuffer;//加上当前文件名
				SetWindowText(hText, str.c_str());//显示文件的路径
				InvalidateRect(hwnd, nullptr, true);//进入WM_PAINT消息，进行显示一部分文件内容
			}
			else//否则打开的是目录,当然也可能是一些不共享文件，已被打开，锁住了，不能被读取
			{
				WIN32_FIND_DATA wfd;
				if (FindFirstFile(szBuffer, &wfd) != INVALID_HANDLE_VALUE && (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)
				{
					MessageBeep(0);
					MessageBox(nullptr, TEXT("该文件已被打开，不共享，已被锁定，只有关闭后才能打开"),TEXT( "警告!!"), MB_ICONWARNING);
					break;//如果该文件存在，并且不是目录，说明是一些不共享文件，已被打开，锁住了，不能被读取

				}
				//要么文件不存在，是驱动器，要么文件存在，是目录
				szBuffer[lstrlen(szBuffer) - 1] = '\0';//将最后1个字符]置为'\0'
				if (!SetCurrentDirectory(szBuffer + 1))//如果是子目录，则像[file],将从第2个字符开始的字符串设为当前目录
				{
					//设置不成功，说明选中的是根目录，如[-c-],[-d-]
					szBuffer[3] = ':';//将目录如[-c-]之类的根目录设为[-c:\后跟'\0'
					szBuffer[4] = '\\';
					szBuffer[5] = '\0';
					SetCurrentDirectory(szBuffer + 2);//将每3个字符开始的字符串设为根目录
				}
				GetCurrentDirectory(MAX_PATH, path);//获取当前目录
				SetWindowText(hText, path);//显示新目录
				SendMessage(hList, LB_RESETCONTENT, 0, 0);//将列表框的内容清空
				SendMessage(hList, LB_DIR, DDL_ARCHIVE | DDL_DIRECTORY | DDL_DRIVES | DDL_EXCLUSIVE |
					DDL_HIDDEN | DDL_POSTMSGS | DDL_READONLY | DDL_READWRITE | DDL_SYSTEM, (long)TEXT("*.*"));
				//在列表框列出新目录的所有属性的文件
			}
		}
		break;
	case WM_SETFOCUS:
		SetFocus(hList);//设置列表框获得焦点
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hFile = CreateFile(szBuffer, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);//读取文件
		if (hFile != INVALID_HANDLE_VALUE)//打开成功
		{
			TCHAR temp[8192];//最多只读取8K字节内容
			unsigned long i;
			ReadFile(hFile, temp, 8192, &i, nullptr);
			SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));//设置等宽字体
			SetTextColor(hdc, GetSysColor(COLOR_BTNTEXT));//设置画笔字体为文本字体颜色
			SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));//设置画笔字体为按钮背景颜色
			DrawText(hdc, temp, i, &rect, DT_WORDBREAK | DT_EXPANDTABS );//
			//DT_WORDBREAK	断开字。当一行中的字符将会延伸到由lpRect指定的矩形的边框时，此行自动地在字之间断开。一个回车一换行也能使行折断。
			//DT_EXPANDTABS	扩展制表符，每个制表符的缺省字符数是8,如果没加这个属性，所有制表符会变成小矩形黑点
			
			CloseHandle(hFile);//释放文件
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = TEXT("zeng");
	wndclass.lpszMenuName = nullptr;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("勇华"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}