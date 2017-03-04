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
struct
{
	int index;
	TCHAR *szDesc;
}
ROP[]=
{
	0xcc0020, TEXT("SRCCOPY"),
	0x0042, TEXT("BLACKNESS"),
	0x1100a6, TEXT("NOTSRCERASE"),
	0x330008, TEXT("NOTSRCCOPY"),
	0x440328, TEXT("SRCERASE"),
	0x550009, TEXT("DSITNVERT"),
	0X5A0049, TEXT("PAINTVERT"),
	0X660046, TEXT("SRCINVERT"),
	0X8800C6, TEXT("SRCAND"),
	0XBB0226, TEXT("MERGEPAINT"),
	0XC000CA, TEXT("MERGECOPY"),
	0XEE0086, TEXT("SRCPAINT"),
	0XF00021, TEXT("PATCOPY"),
	0XFF0062, TEXT("WHITENESS")
};
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	HDC hdcMem;
	static BITMAP bitmap;
	static HBITMAP curBitmap;
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static HBITMAP hBitmap[5] ;
	static int indexBitmap=0;
	static int indexRop=0;
	static int nBitmap;
	static int nRop;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		hBitmap[0] = LoadBitmap(hInst, TEXT("IDB_BITMAPA"));
		hBitmap[1]= LoadBitmap(hInst, TEXT("IDB_BITMAPB")); 
		hBitmap[2] = LoadBitmap(hInst, TEXT("IDB_BITMAPCAR1"));
		hBitmap[3] = LoadBitmap(hInst, TEXT("IDB_BITMAPCAR2"));
		hBitmap[4] = LoadBitmap(hInst, TEXT("IDB_BITMAPCAR3"));
		curBitmap = hBitmap[0];
		nBitmap = sizeof(hBitmap) / sizeof(HBITMAP);
		nRop = sizeof(ROP) / sizeof(ROP[0]);
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem,curBitmap);
		GetObject(curBitmap, sizeof(BITMAP), &bitmap);
		BitBlt(hdc, (cxClient-bitmap.bmWidth)/2,(cyClient-bitmap.bmHeight)/2,bitmap.bmWidth,bitmap.bmHeight, hdcMem, 0, 0,ROP[indexRop].index);
		DeleteDC(hdcMem);
		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			indexBitmap = (indexBitmap + 1+nBitmap)%(nBitmap);
			curBitmap = hBitmap[indexBitmap];
			InvalidateRect(hwnd, 0, true);
			break;
		case VK_DOWN:
			indexBitmap = (indexBitmap - 1+nBitmap) % (nBitmap);
			curBitmap = hBitmap[indexBitmap];
			InvalidateRect(hwnd, 0, true);
			break;
		case VK_LEFT:
			indexRop = (indexRop + 1 + nRop) % nRop;
			SetWindowText(hwnd, ROP[indexRop].szDesc);
			InvalidateRect(hwnd, 0, true);
			break;
		case VK_RIGHT:
			indexRop = (indexRop - 1 + nRop) % nRop;
			SetWindowText(hwnd, ROP[indexRop].szDesc);
			InvalidateRect(hwnd, 0, true);
			break;
		}
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������ʱ
		for (int i = 0; i < nBitmap; i++)
			DeleteObject(hBitmap[i]);
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
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("SRCCOPY"), WS_OVERLAPPEDWINDOW,
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