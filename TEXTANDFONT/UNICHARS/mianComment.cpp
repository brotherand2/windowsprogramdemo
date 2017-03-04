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

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static HFONT hFont;
	static LOGFONT lf;
	static CHOOSEFONT cf;
	static unsigned int iPage = 0;
	TEXTMETRIC tm;
	int cxLabel;
	TCHAR szBuffer[128];
	SIZE size;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		hdc = GetDC(hwnd);
		lstrcpy(lf.lfFaceName, TEXT("Lucida Sans Unicode"));
		lf.lfHeight = -GetDeviceCaps(hdc, LOGPIXELSX) / 6;
		cf.lStructSize = sizeof(cf);
		cf.hwndOwner = hwnd;
		cf.lpLogFont = &lf;
		cf.Flags = CF_INITTOLOGFONTSTRUCT;
		SetScrollRange(hwnd, SB_VERT, 0, 256 * 16 - 1, false);
		SetScrollPos(hwnd, SB_VERT, 0, true);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			iPage--;
			break;
		case SB_LINEDOWN:
			iPage++;
			break;
		case SB_PAGEDOWN:
			iPage += 16;
			break;
		case SB_PAGEUP:
			iPage -= 16;
			break;
		case SB_TOP:
			iPage = 0;
			break;
		case SB_BOTTOM:
			iPage = 16 * 256 - 1;
			break;
		case SB_THUMBPOSITION:
			iPage = HIWORD(wParam);
			break;
		default:
			break;
		}
		iPage = max(0, min(256 * 16 - 1, iPage));
		SetScrollPos(hwnd, SB_VERT, iPage, true);
		InvalidateRect(hwnd, 0, true);
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		switch (wmId)
		{
		case ID_FONT:
			if (ChooseFont(&cf))
				InvalidateRect(hwnd, 0, true);
			break;
		}
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		SelectObject(hdc, CreateFontIndirect(&lf));
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmMaxCharWidth;
		cyChar = tm.tmHeight+tm.tmExternalLeading;
		cxLabel = 0;
		for (int i = 0; i < 16; i++)
		{
			wsprintf(szBuffer, TEXT("000%x_: "), i);
			GetTextExtentPoint32(hdc, szBuffer, lstrlen(szBuffer), &size);
			cxLabel = max(cxLabel, size.cx);
		}
		for (int y = 0; y < 16; y++)
		{
			wsprintf(szBuffer, TEXT("%04x_: "), 16*iPage+y);
			TextOut(hdc, 0, y*cyChar, szBuffer, lstrlen(szBuffer));
			for (int x = 0; x < 16; x++)
			{
				wchar_t ch = 256 * iPage + y * 16 + x;
				TextOutW(hdc, cxLabel + x*cxChar, y*cyChar, &ch, 1);
			}
		}
		DeleteObject((HFONT)SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������ʱ
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
	while (GetMessage(&msg, nullptr, 0, 0))//����Ϣ���л�ȡ��Ϣ
	{
		TranslateMessage(&msg);//��һЩ������Ϣת�����ַ���Ϣ���ڰ�����Ϣ����
		DispatchMessage(&msg);//�ַ���Ϣ�������ô��ڹ��̽��д�����Ϣ
	}
}