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
#ifndef UNICODE //���û�ж���UNICODE ,��tstring�滻Ϊstring,�����滻Ϊwstring
using  tstring = string;
#else
using  tstring = wstring;
#endif
void GetLargestDiaplayMode(int &cxBitmap, int &cyBitmap)
{
	DEVMODE dm;
	cxBitmap = cyBitmap = 0;
	int iModeNum = 0;
	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(dm);
	while (EnumDisplaySettings(0, iModeNum++, &dm))
	{
		cxBitmap = max(cxBitmap, dm.dmPelsWidth);
		cyBitmap = max(cyBitmap, dm.dmPelsHeight);
	}
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static HDC hdcMem;
	static HBITMAP hBtimap;
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static int cxBitMap, cyBitMap;
	static bool bLBbuttonDown=false;
	static bool bRBbuttonDown=false;
	static int cxMouse, cyMouse;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
		GetLargestDiaplayMode(cxBitMap, cyBitMap);
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		hdc=GetDC(hwnd);
		hdcMem = CreateCompatibleDC(hdc);
		hBtimap = CreateCompatibleBitmap(hdc, cxBitMap, cyBitMap);
		SelectObject(hdcMem, hBtimap);
		PatBlt(hdcMem, 0, 0, cxBitMap, cyBitMap, WHITENESS);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		if (!bRBbuttonDown)
			SetCapture(hwnd);
		cxMouse = LOWORD(lParam);
		cyMouse = HIWORD(lParam);
		bLBbuttonDown = true;
		break;
	case WM_LBUTTONUP:
		if (bLBbuttonDown)
			SetCapture(false);
		bLBbuttonDown = false;
		break;
	case WM_RBUTTONDOWN:
		if (!bLBbuttonDown)
			SetCapture(hwnd);
		cxMouse = LOWORD(lParam);
		cyMouse = HIWORD(lParam);
		bRBbuttonDown = true;
		break;
	case WM_RBUTTONUP:
		if (bRBbuttonDown)
			SetCapture(false);
		bRBbuttonDown = false;
		break;
	case WM_MOUSEMOVE:
		if (!bLBbuttonDown&&!bRBbuttonDown)
			return 0;
		hdc = GetDC(hwnd);
		SelectObject(hdc, GetStockObject(bLBbuttonDown ? BLACK_PEN : WHITE_PEN));
		SelectObject(hdcMem, GetStockObject(bLBbuttonDown ? BLACK_PEN : WHITE_PEN));
		MoveToEx(hdc, cxMouse, cyMouse, 0);
		MoveToEx(hdcMem, cxMouse, cyMouse, 0);

		cxMouse =(short) LOWORD(lParam);
		cyMouse = (short)HIWORD(lParam);
		LineTo(hdc, cxMouse, cyMouse);
		LineTo(hdcMem, cxMouse, cyMouse);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����

		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������ʱ
		DeleteDC(hdcMem);
		DeleteObject(hBtimap);
		PostQuitMessage(0);//�׳�һ������ֵΪ0�˳���Ϣ
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//��Ĭ�ϵĴ��ڹ��̴���������Ϣ����Щ��������
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	WNDCLASS wndclass;//������
	wndclass.cbClsExtra = 0;//Ϊ�����Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.cbWndExtra = 0;//Ϊ���ڷ���Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//������ˢ
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//�����
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);//ͼ��
	wndclass.hInstance = hInstance;//ʵ�������������PID��һ���������������������ϵͳ�����ҵ����̵ĵ�ַ
	wndclass.lpfnWndProc = WndProc;//���ڹ���
	wndclass.lpszClassName = TEXT("zeng");//����
	wndclass.lpszMenuName = nullptr;//�˵�
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//��Щ�ı��Ҫ�ػ�������ˮƽ�ʹ�ֱ�ı���ػ�
	if (!RegisterClass(&wndclass))//ע�ᴰ����
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("�»�"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);//��������
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	ShowWindow(hwnd, nCmdShow);//��ʾ����
	UpdateWindow(hwnd);//�����ػ�
	MSG msg;//��Ϣ�ṹ
	while (GetMessage(&msg, nullptr, 0, 0))//����Ϣ���л�ȡ��Ϣ
	{
		TranslateMessage(&msg);//��һЩ������Ϣת�����ַ���Ϣ���ڰ�����Ϣ����
		DispatchMessage(&msg);//�ַ���Ϣ�������ô��ڹ��̽��д�����Ϣ
	}
}