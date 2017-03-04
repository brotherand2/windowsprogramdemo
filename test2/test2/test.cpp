#include"test.h"
typedef struct sysmetric
{
	int index;
	string szLabel;
	string szDesc;
}Sysmetric;
using metrics = shared_ptr <Sysmetric> ;
metrics randomData(int &count)
{
	engine.seed(time(0));
	char szLabel[21];
	char szDesc[21];
	count = uid1(engine);
	metrics sysmetrics(new Sysmetric[count], [](Sysmetric *s){delete[]s; });
	for (int i = 0; i < count; i++)
	{
		(sysmetrics.get() + i)->index = uid1(engine);
		int len1 = uid(engine);
		int len2 = uid(engine);
		for (int j = 0; j < len1; j++)
			szLabel[j] = letter[uid2(engine)];
		szLabel[len1] = '\0';
		for (int j = 0; j < len2; j++)
			szDesc[j] = letter[uid2(engine)]; 
		szDesc[len2] = '\0';
		(sysmetrics.get() + i)->szDesc = szDesc;
		(sysmetrics.get() + i)->szLabel = szLabel;

	}
	return sysmetrics;
}
void show(metrics &temp,int counts)
{
	for (int i = 0; i < counts; i++)
		cout << "index:" << (temp.get() + i)->index << "    " << (temp.get() + i)->szDesc << "   "\
		<< (temp.get() + i)->szLabel << endl;
}
long _stdcall WndProc(HWND hwnd, unsigned int uMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	HDC hdc;
	RECT rect;
	char buffer[20];
	char mouse[20] ="你按下了鼠标";
	metrics temp;
	int count;
	static int cxChar, cyChar, cxCaps;
	string str;
	switch (uMsg)
	{

	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		TextOut(hdc, 200, 300, mouse, strlen(mouse));
		ReleaseDC(hwnd,hdc);
		break;
	case WM_SIZE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		break;
	case WM_CHAR:
		hdc = GetDC(hwnd);
		sprintf(buffer, "your enter %c", wParam);
		TextOut(hdc, 100, 200, buffer, strlen(buffer));
		//MessageBox(nullptr,buffer , "char message", MB_OK);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		temp = randomData(count);
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < count; i++)
		{
			str = (temp.get() + i)->szDesc;
			TextOut(hdc, 0, cyChar*i, str.c_str(), str.length());
			str = (temp.get() + i)->szLabel;
			TextOut(hdc, 22*cxCaps, cyChar*i, str.c_str(), str.length());
			SetTextAlign(hdc, TA_TOP | TA_RIGHT);
			TextOut(hdc, 22* cxCaps + 40 * cxChar,cyChar*i ,buffer, sprintf(buffer, "%d", (temp.get() + i)->index));
			SetTextAlign(hdc, TA_TOP | TA_LEFT);
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
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
int  WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, char* szCmdLine, int iCmdShow)
{//hinstance 类似于进程PID，szcomline =string terminated with zero commond line
	//icmdshow=int commond show
	int count;
	metrics m_t=randomData(count);
	show(m_t,count);
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;//count of byte class extra
	wndclass.cbWndExtra = 0;//cout of window extra
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//handle brush background
	wndclass.hCursor = LoadCursor(nullptr, IDC_APPSTARTING);//identifier cursor
	wndclass.hIcon = LoadIcon(nullptr, IDI_ASTERISK);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = "zeng";
	wndclass.lpszMenuName = nullptr;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.hIconSm = nullptr;
	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(nullptr, "register failed", "error", MB_ICONERROR);
		return false;
	}
	HWND hwnd = CreateWindowEx(0, "zeng", "曾勇华", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
	if (!hwnd)
		MessageBox(nullptr, "create window failed", "error", MB_ICONERROR);
	else
	{
		ShowWindow(hwnd, iCmdShow);
		UpdateWindow(hwnd);
		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	int choice = MessageBox(nullptr, "title", "content", MB_YESNOCANCEL);
	if (choice == IDYES)
		MessageBox(nullptr, "your choice ok", "content", MB_OK);
	else
		MessageBox(nullptr, "your choice cancel", "content", MB_OK);

};
