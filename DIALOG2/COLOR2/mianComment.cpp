//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<strsafe.h>
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
#pragma comment(lib,"winmm.lib")//加载winmm.lib库，播放声音文件playsound要用到
void ShowNumber(HWND hwnd, int num)
{
	TCHAR temp[20];
	StringCchPrintf(temp, 20, TEXT("%X"), num);//将数转换成16进制
	SetDlgItemText(hwnd, IDC_RESULT,temp);//在显示静态文本区显示数字
}
int Calulate(int first, int operation, int second)//进行计算，参数分别是第1个数，操作符，第2个数
{
	switch (operation)
	{
	case '+':return first + second;
	case '-':return first - second;
	case '*':return first* second;
	case '/':return second?first / second:MAXDWORD;//如果除数为0，显示结果FFFFFFFF
	case '%':return second ? first % second : MAXDWORD;//如果除数为0，显示结果FFFFFFFF
	case '=':return second;
	case '&':return first & second;
	case '|':return first |second;
	case '^':return first ^second;
	case '>':return first >>second;
	case '<':return first <<second;
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
	HWND hButton;
	static int number;//当前按下的数字
	static int firstNum;//第1个按下的数字
	static bool bNewNum=true;//是否是新按下的数
	static int iOperation = '=';//操作符,初始化为'='，输入第1个操作符后进行=号运算，以后每次输入一个操作符，用前一个操作符计算前一次的运算
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
	case WM_KEYDOWN://按下按键
		if (wParam != VK_LEFT&&wParam != VK_DELETE)
			break;//如果不是左方向键或者不是删除键（清空），很可能是字符键，则不处理，
		//如果是方向键（对应删除一个数字）或Del键（对应清空），因为都不是字符消息，所以采用fall through到char 消息
		if (wParam == VK_LEFT)//左方向键
			wParam = VK_BACK;//转换成回退消息
	case WM_CHAR:
		if (isalpha(wParam))//如果是字母按键
			wParam=toupper(wParam);//将字母转换成大写字母
		if (wParam == VK_RETURN)//如果按下回车键，转换成=号键
			wParam = '=';
		if (wParam == 0x1b)
			wParam = VK_DELETE;//如果按下ESC键，转换成删除键（清空）
		if (hButton = GetDlgItem(hwnd, wParam))//如果按下键的ASCII字符在计算器中有对应的键
		{
			SendMessage(hButton, BM_SETSTATE, 1, 0);//让按钮按下
			//SND_FILENAME，表示第1个参数指定了WAVE文件名，SND_SYNC同步播放声音，在播放完后PlaySound函数才返回
			Sleep(100);//保持按下状态100毫秒
			SendMessage(hButton, BM_SETSTATE, 0, 0);//让按钮松开
		}
		else
		{
			PlaySound(TEXT("error.wav"), nullptr, SND_SYNC | SND_FILENAME);//播放按错键时的声音
			break;
		}
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		SetFocus(hwnd);//在控件上点击后，主窗口失去焦点，不能再接收键盘消息，让主窗口重获焦点
		if (wmId == ID_ABOUT)//菜单里只有1个菜单项，如果点击了菜单
		{
			break;
		}
		PlaySound(TEXT("push.wav"), nullptr, SND_SYNC | SND_FILENAME);//播放按下时的声音
		if(wmId== VK_BACK)//按下回退键，删除一个数字
		   ShowNumber(hwnd, number /= 16);//除以16相当于右移一位，删除最后一个数字
		else
		{
			if (wmId == VK_DELETE)//按下删除键，当作清除键
				ShowNumber(hwnd, number = 0);
			else
			{
				if (isxdigit(wmId))//如果按键是十六进制数字
				{
					if (bNewNum)//因为按下操作符后紧接着按新的数字，所以bNewNum这时为true，现按下了一个数字后，置为false
					{
						firstNum = number;//将第2个数赋给第1个数
						number = 0;//第2个数初始化为0
						bNewNum = false;//第二次按数字的时候应为false
					}
					int theUnit = wParam - (isdigit(wParam) ? '0' : 'A' - 10);//个位数，如果是数字字符则减去字符‘0’得到整数，否则减去'A‘后再加上10
					//FFFFFFFF/16=FFFFFFF，FFFFFFF*16+最大个位数等于FFFFFFFF
					if (number <MAXDWORD / 16)//如果小于FFFFFFF，那么加上最大数F也不会溢出
					{
						number = number * 16 + theUnit;//将先前的数乘以16，再加上个位数
						ShowNumber(hwnd, number);
					}
					else//大于等于32位的最大数FFFFFFFF的1/16（即FFFFFFF），这时*16+个位数就可能溢出数太大，有可能溢出
					{
						if (theUnit == 0 && number == MAXDWORD / 16)
						{//如果number刚好是FFFFFFF，最后一位是0，则加上最大的F也不会溢出
							number = number * 16 + theUnit;//将先前的数乘以16，再加上个位数
							ShowNumber(hwnd, number);
						}
						else//如果number刚好是FFFFFFF，最后一位不是0，加上F就会溢出
						{//或者number已是8位数，再增加1位变成9位就溢出了
							MessageBox(hwnd, TEXT("前七位最好不要等于FFFFFFF\n如果前七位等于FFFFFFF，则最后一位只能是0，否则可能会溢出\n而且不能输入9位数最大数为FFFFFFFF"), TEXT("Warning"), MB_OK | MB_ICONWARNING);
						}
					}
				}
				else//是操作符
				{
					if (!bNewNum)//如果前一次输入的数字，现输入操作符
					{//输入第1个操作符后进行=号运算，以后每次输入一个操作符，用前一个操作符计算前一次的运算
						number=Calulate(firstNum,iOperation, number);//进行前一次的操作符计算,计算后的结果保留在第2 个操作数上
						ShowNumber(hwnd,number);//显示计算结果
					}
					bNewNum = true;//按下字符键后，接下来输入的数字就是新数字
					iOperation = wmId;//保留当前操作符，在下一次输入操作符时和使用
				}
			}
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
	static TCHAR szAppName[]=TEXT("IDD_DIALOG");
	WNDCLASS wndclass;//窗口类
	wndclass.cbClsExtra = 0;//为类分配的额外空间，可以保存一些地址、变量
	wndclass.cbWndExtra =DLGWINDOWEXTRA	;//注册自己的对话框过程必须在窗口额外空间分配30个字节
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);//背景画刷
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//鼠标光标
	wndclass.hIcon = LoadIcon(hInstance,TEXT("MYICON"));//图标
	wndclass.hInstance = hInstance;//实例名柄，程序的PID，一个整数，根据这个整数，系统可以找到进程的地址
	wndclass.lpfnWndProc = WndProc;//窗口过程
	wndclass.lpszClassName = TEXT("zeng"); //类名
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);//菜单
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//哪些改变后要重画，设置水平和垂直改变后重绘
	if (!RegisterClass(&wndclass))//注册窗口类
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	MSG msg;//消息结构
	//HWND hwnd = CreateDialog(hInstance,TEXT(IDD_DIALOG), 0, nullptr);
	HWND hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, nullptr);

	ShowWindow(hwnd, nCmdShow);
	while (GetMessage(&msg, nullptr, 0, 0))//从消息队列获取消息
	{
		TranslateMessage(&msg);//将一些按键消息转换成字符消息放在按键消息后面
		DispatchMessage(&msg);//分发消息，即调用窗口过程进行处理消息
	}
}