//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include"resource.h"
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
void InvertBlock(HWND hwndSrc,HWND hwnd, POINT ptBeg, POINT ptEnd)
{
	HDC hdcScr = GetDCEx(hwndSrc, 0,DCX_CACHE| DCX_LOCKWINDOWUPDATE);
	PatBlt(hdcScr, ptBeg.x, ptBeg.y, ptEnd.x-ptBeg.x, ptEnd.y-ptBeg.y, DSTINVERT);
	ReleaseDC(hwndSrc, hdcScr);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	HDC hdcMem;
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static POINT ptBeg, ptEnd;
	static bool bCapture=false, bBlock=false;
	static HWND hDestop;
	static HBITMAP hBitmap;
	BITMAP bitmap;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		hDestop = GetDesktopWindow();
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		if (!bCapture)
		{
			if (LockWindowUpdate(hDestop))
			{
				bCapture = true;
				SetCapture(hwnd);
				SetCursor(LoadCursor(0, IDC_CROSS));
			}
			else
				MessageBeep(0);
		}
		break;
	case WM_RBUTTONDOWN:
		if (bCapture)
		{
			bBlock = true;
			ptBeg.x = LOWORD(lParam);
			ptBeg.y = HIWORD(lParam);
			ClientToScreen(hwnd, &ptBeg);
			ptEnd = ptBeg;
		}
		break;
	case WM_MOUSEMOVE:
		if (bBlock)
		{
			InvertBlock(hDestop, hwnd, ptBeg, ptEnd);
			ptEnd.x = (short)LOWORD(lParam);
			ptEnd.y = (short)HIWORD(lParam);
			ClientToScreen(hwnd, &ptEnd);
			InvertBlock(hDestop, hwnd, ptBeg, ptEnd);
		}
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		if (bBlock)
		{
			InvertBlock(hDestop, hwnd, ptBeg, ptEnd);
			ptEnd.x = (short)LOWORD(lParam);
			ptEnd.y = (short)HIWORD(lParam);
			ClientToScreen(hwnd, &ptEnd);
			if (hBitmap)
				DeleteObject(hBitmap);
			hdc = GetDCEx(hDestop,0,DCX_LOCKWINDOWUPDATE);
			hdcMem = CreateCompatibleDC(hdc);
			int width = abs(ptEnd.x - ptBeg.x);
			int heigh = abs(ptEnd.y - ptBeg.y);
			hBitmap = CreateCompatibleBitmap(hdc, width, heigh);
			SelectObject(hdcMem, hBitmap);
			ptBeg.x = (ptBeg.x < ptEnd.x) ? ptBeg.x : ptEnd.x;
			ptBeg.y = (ptBeg.y < ptEnd.y) ? ptBeg.y : ptEnd.y;

			//StretchBlt(hdcMem, 0, 0, width, heigh, hdc, ptBeg.x, ptBeg.y, width,heigh, SRCCOPY);
			BitBlt(hdcMem, 0, 0, width, heigh, hdc, ptBeg.x, ptBeg.y, SRCCOPY);
			ReleaseDC(hDestop, hdc);
			DeleteDC(hdcMem);
			InvalidateRect(hwnd, 0, true);
		}
		if (bCapture || bBlock)
		{
			bBlock = bCapture = false;
			SetCursor(LoadCursor(0, IDC_ARROW));
			ReleaseCapture();
			LockWindowUpdate(0);
		}
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		switch (wmId)
		{
		case IDM_EDIT_COPY:
		case IDM_EDIT_CUT:
		case IDM_EDIT_DELETE:
		case IDM_EIDT_PASTE:
			MessageBeep(0);
			break;
		}
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		if (hBitmap)
		{
			GetObject(hBitmap, sizeof(BITMAP), &bitmap);
			hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmap);
			BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
			DeleteObject(hdcMem);
		}
		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������ʱ
		if (hBitmap)
			DeleteObject(hBitmap);
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
	wndclass.lpszMenuName =MAKEINTRESOURCE(IDR_MENU1);//�˵�
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
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (GetMessage(&msg, nullptr, 0, 0))//����Ϣ���л�ȡ��Ϣ
	{
		if (!TranslateAccelerator(hwnd, hAccel, &msg))
		{
			TranslateMessage(&msg);//��һЩ������Ϣת�����ַ���Ϣ���ڰ�����Ϣ����
			DispatchMessage(&msg);//�ַ���Ϣ�������ô��ڹ��̽��д�����Ϣ
		}
	}
}