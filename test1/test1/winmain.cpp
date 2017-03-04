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
	HORZSIZE, "HORZSIZE", "�ú����ױ�ʾ���:",
	VERTSIZE, "VERTSIZE", "�ú��ױ�ʾ�߶�",
	HORZRES, "HORZRES", "�����ر�ʾ���",
	VERTRES, "VERTRES", "�����ر�ʾ�߶�",
	BITSPIXEL, "BITSPIXEL", "ÿ�����ذ�������ɫλ��",
	PLANES, "PLANES", "��ɫƽ������",
	NUMBRUSHES, "NUMBRUSHES", "�豸��ˢ����",
	NUMFONTS, "NUMFONTS", "�豸��������",
	NUMMARKERS, "NUMMARKERS", "�豸�������",
	NUMCOLORS, "NUMCOLORS", "�豸��ɫ����",
	PDEVICESIZE, "PDEVICESIZE", "�豸�ṹ��С",
	ASPECTX, "ASPECTX", "ÿ�����ص���Կ��",
	ASPECTY, "ASPECTY", "ÿ�����ص���Ը߶�",
	ASPECTXY, "ASPECTXY", "ÿ�����ص���ԶԽ���",
	LOGPIXELSX, "LOGPIXELSX", "ÿӢ�ߵ�ˮƽ����",
	LOGPIXELSY, "LOGPIXELSY", "ÿӢ�ߵĴ�ֱ����",
	SIZEPALETTE, "SIZEPALETTE", "��ɫ���С",
	NUMRESERVED, "NUMRESERVED", "��ɫ�屣������",
	COLORRES, "COLORRES", "ʵ����ɫ����"
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
{//hinstance �����ڽ���PID��szcomline =string terminated with zero commond line
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
	HWND hwnd = CreateWindowEx(0, "zeng", "���»�", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
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
