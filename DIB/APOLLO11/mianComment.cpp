//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include"DIBFILE.h"
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

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static BITMAPFILEHEADER *pbmfh[2];
	static BITMAPINFO *pbmih[2];
	static PBYTE pByte[2];
	static unsigned int cxDib[2], cyDib[2];
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		pbmfh[0] = DibLoadImage(hwnd, TEXT("Apollo11.bmp"));
		pbmfh[1] = DibLoadImage(hwnd, TEXT("ApolloTD.bmp"));
		pbmih[0] = (BITMAPINFO*)(pbmfh[0] + 1);
		pbmih[1] = (BITMAPINFO*)(pbmfh[1] + 1);
		pByte[0] = (PBYTE)pbmfh[0] + pbmfh[0]->bfOffBits;
		pByte[1] = (PBYTE)pbmfh[1] + pbmfh[1]->bfOffBits;
		cxDib[0] = pbmih[0]->bmiHeader.biWidth;
		cxDib[1] = pbmih[1]->bmiHeader.biWidth;
		cyDib[0] = abs(pbmih[0]->bmiHeader.biHeight);
		cyDib[1] =abs( pbmih[1]->bmiHeader.biHeight);

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
		SetDIBitsToDevice(hdc, 0,cyClient/4, cxDib[0], cyDib[0], 0, 0, 0, cyDib[0], pByte[0], pbmih[0], DIB_RGB_COLORS);
		SetDIBitsToDevice(hdc, 240,cyClient/4, 80,166, 80,60, 0, cyDib[0], pByte[0], pbmih[0], DIB_RGB_COLORS);
		SetDIBitsToDevice(hdc, 340,cyClient/4, cxDib[1], cyDib[1], 0, 0, 0, cyDib[1], pByte[1], pbmih[1], DIB_RGB_COLORS);
		SetDIBitsToDevice(hdc, 580,cyClient/4, 80,166, 80,60, 0, cyDib[1], pByte[1], pbmih[1], DIB_RGB_COLORS);

		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������ʱ
		PostQuitMessage(0);//�׳�һ������ֵΪ0�˳���Ϣ
		if (pbmfh[0])
			free(pbmfh[0]);
		if (pbmfh[1])
			free(pbmfh[1]);

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