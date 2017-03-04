# include<Windows.h>
# include<stdio.h>
# include<iostream>
# include<time.h>
time_t rawtime;//时间类型，在time.h定义
# pragma comment(lib,"WINMM.LIB")
class point
{
public:
	int x;
	int y;
	 point(int x1, int y1)
	{
		x = x1;
		y = y1;
	}
};
HINSTANCE hi;
HINSTANCE prehi;
class window
{
public:
	WNDCLASS wndclass;
	HWND hwnd;
	MSG msg;
	HINSTANCE hinstance;
	void message()
	{
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	window(HINSTANCE hinstance);
	void myWindow()
	{
		time(&rawtime);//将rawtime地址传过去，获得时间，以秒计
		srand((int)time(NULL));//设置于随机数种子为当前系统时间，每次种子不同，产生的随机
		int x, y, x1, y1;
		x = 20+rand() % 200;
		y= 20 + rand() % 200;
		x1 = 150 + rand() % 500;
		y1 = 150 + rand() % 500;
		hwnd = CreateWindow("hello", "nihao", WS_OVERLAPPEDWINDOW, x, y,x1, y1, NULL, NULL, hinstance, NULL);
		set(hwnd);
	}
	void set(HWND hwnd)
	{
		ShowWindow(hwnd, SW_NORMAL);
		UpdateWindow(hwnd);
	}
};
window *win;
long _stdcall WndProc(HWND hwnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC dc;
	PAINTSTRUCT ps;
	char info[20];
	RECT rect;
	switch (uMsg)
	{
	case WM_CREATE:
		PlaySound("login.wav", nullptr, SND_SYNC | SND_FILENAME);
		break;
	case WM_CLOSE:
		if (IDYES == MessageBox(hwnd, "are you sure to quit!", "you are trying to quit", MB_YESNO))
			DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		//win->myWindow();
		new window(prehi);
		MessageBox(hwnd, "hello word\n", "hello", MB_YESNO);
		break;
	case WM_KEYDOWN:
		dc = GetDC(hwnd);
		strcpy(info, "您按下了键盘");
		TextOut(dc, 100, 100, info, strlen(info));
		break;
	case WM_CHAR:
		dc = GetDC(hwnd);
		//system("cmd");
		system("test");
		//system("cd Debug");
		//system("test");
		//system("pause");
		sprintf(info, "您按的字符是%c", wParam);
		TextOut(dc, 200, 200, info, strlen(info));
		break;
	case WM_PAINT:
		dc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		DrawText(dc, "what can i do for you?", -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		EndPaint(hwnd, &ps);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}
	return 0;
}
window::window(HINSTANCE instanc)
{
	hinstance = instanc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(hinstance, IDC_APPSTARTING);
	wndclass.hIcon = LoadIcon(hinstance, IDI_APPLICATION);
	wndclass.hInstance = hinstance;
	wndclass.lpszClassName = "hello";
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	RegisterClass(&wndclass);
	myWindow();
	//for (int i = 0; i <1; i++)
		//myWindow();
	message();
}
int WinMain(HINSTANCE hinstance, HINSTANCE hPrevHinstance, LPSTR lpCmdLine, int nCmdShow)
{
	//window win(hinstance);
	hi = hinstance;
	prehi = hinstance;
	win = new window(hinstance);
}