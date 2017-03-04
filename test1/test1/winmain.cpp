#include<windows.h>
#include<stdio.h>

struct
{
	int index;
	char *label;
	char *desc;
}
devcaps[]=
{
	HORZSIZE, "HORZSIZE", "用毫米米表示宽度:",
	VERTSIZE, "VERTSIZE", "用毫米表示高度",
	HORZRES, "HORZRES", "用像素表示宽度",
	VERTRES, "VERTRES", "用像素表示高度",
	BITSPIXEL, "BITSPIXEL", "每个像素包含的颜色位数",
	PLANES, "PLANES", "颜色平面数量",
	NUMBRUSHES, "NUMBRUSHES", "设备画刷数量",
	NUMFONTS, "NUMFONTS", "设备画笔数量",
	NUMMARKERS, "NUMMARKERS", "设备标记数量",
	NUMCOLORS, "NUMCOLORS", "设备颜色数量",
	PDEVICESIZE, "PDEVICESIZE", "设备结构大小",
	ASPECTX, "ASPECTX", "每个像素的相对宽度",
	ASPECTY, "ASPECTY", "每个像素的相对高度",
	ASPECTXY, "ASPECTXY", "每个像素的相对对角线",
	LOGPIXELSX, "LOGPIXELSX", "每英尺的水平像素",
	LOGPIXELSY, "LOGPIXELSY", "每英尺的垂直像素",
	SIZEPALETTE, "SIZEPALETTE", "调色板大小",
	NUMRESERVED, "NUMRESERVED", "调色板保留数量",
	COLORRES, "COLORRES", "实际颜色数量"
};
const int numlines = sizeof(devcaps) / sizeof(devcaps[0]);
long _stdcall WndProc(HWND hwnd, unsigned int uMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	char *s = "textout";
	TEXTMETRIC tm;
	char buffer[22];
	static int cxChar, cxCaps, cyChar, cxClient, cyClient;
	switch (uMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cxClient = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < numlines; i++)
		{
			COLORREF oldColor=SetTextColor(hdc, RGB(255, 0, 0));
			COLORREF oldBkColor = SetBkColor(hdc, RGB(168, 221, 78));
			TextOut(hdc, 0, i*cyChar, devcaps[i].label, lstrlen(devcaps[i].label));
			SetTextColor(hdc, oldColor);
			TextOut(hdc, 22 * cxCaps, i*cyChar, devcaps[i].desc, lstrlen(devcaps[i].desc));
			SetTextAlign(hdc, TA_TOP | TA_RIGHT);
			SetBkColor(hdc, oldBkColor);
			TextOut(hdc, 22 * cxCaps + 40*cxChar,i*cyChar, buffer, wsprintf(buffer, "%d", GetDeviceCaps(hdc, devcaps[i].index)));
			SetTextAlign(hdc, TA_TOP | TA_LEFT);
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 100, 500, s, strlen(s));
		EndPaint(hwnd,&ps);
		break;
	case WM_CHAR:
		hdc = GetDC(hwnd);
		char buffer[20];
		sprintf_s(buffer, "your enter %c", wParam);
		TextOut(hdc, 100, 200, buffer, strlen(buffer));
		//MessageBox(nullptr,buffer , "char message", MB_OK);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	//case WM_PAINT:
	//	hdc = BeginPaint(hwnd, &ps);
	//	
	//	GetClientRect(hwnd,&rect);
	//	DrawText(hdc, "this is zeng", -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	//	EndPaint(hwnd, &ps); 
	//	break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, char* szCmdLine, int iCmdShow)
{//hinstance 类似于进程PID，szcomline =string terminated with zero commond line
	//icmdshow=int commond show
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
	HWND hwnd = CreateWindowEx(0, "zeng", "曾勇华", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
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
	if (choice==IDYES)
		MessageBox(nullptr, "your choice ok", "content", MB_OK);
	else
		MessageBox(nullptr, "your choice cancel", "content", MB_OK);

};
