//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<cmath>
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
	static int cxIcon, cyIcon;//ϵͳͼ���ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HICON icon;//ͼ����
	static HINSTANCE hInst;
	TCHAR szError[64],szBuffer[64];
	static TCHAR *text;//ָ���ȡ���ı�
	TCHAR *temp;
	HRSRC src;
	HGLOBAL hGlobal;
	static int lines;//����
	static HWND hScroll;//�������ؼ�
	static int xScroll;//ϵͳ���������
	static int iVScrollPos;//��ֱ������λ��
	RECT rect;//����Ҫ�ػ������
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		cxIcon = GetSystemMetrics(SM_CXICON);//�õ�ϵͳͼ���ȡ��߶�
		cyIcon = GetSystemMetrics(SM_CYICON);
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//�õ�����ʵ������
		icon=LoadIcon(hInst, TEXT("IDI_ICON"));
		 src=FindResource(hInst, TEXT("POEPOEMTEXT"), TEXT("TEXT"));//�ҵ��Զ���TEXT���͵���Դ
		hGlobal= LoadResource(hInst, src);//������Դ���ڴ�
		temp=text = (char *)LockResource(hGlobal);//������Դ
		while (*temp != '\0')
		{
			if (*temp == '\n')//������β
				lines++;//������1
			++temp;//ָ����һ���ַ�
		}
		hScroll = CreateWindow(TEXT("scrollbar"), TEXT(""), WS_CHILD | WS_VISIBLE | SBS_VERT, 0, 0, 0, 0,
			hwnd, (HMENU)1, hInst, nullptr);
		SetScrollRange(hScroll, SB_CTL, 0, lines - 1, false);
		SetScrollPos(hScroll, SB_CTL, 0, false);
		xScroll = GetSystemMetrics(SM_CXVSCROLL);////�õ�ϵͳ��ֱ���������
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		MoveWindow(hScroll, cxClient - xScroll, 0, xScroll, cyClient, true);//���ù�������Сλ��
		break;
	case WM_VSCROLL://��ֱ������Ϣ
		switch (LOWORD(wParam))//�������ϲ���
		{
		case SB_LINEUP://�����ƶ�
			iVScrollPos -= 1;
			break;
		case SB_LINEDOWN://�����ƶ�
			iVScrollPos += 1;
			break;
		case SB_PAGEUP://���Ϸ�ҳ
			iVScrollPos -= cyClient / cyChar;
			break;
		case SB_PAGEDOWN://���·�ҳ
			iVScrollPos += cyClient / cyChar;
			break;
		case SB_THUMBTRACK://�϶� 
			iVScrollPos = HIWORD(wParam);
			break;
		case SB_TOP://�ص�����
			iVScrollPos = 0;
			break;
		case SB_BOTTOM://�ص��ײ�
			iVScrollPos = lines - 1;
			break;
		}
		iVScrollPos = max(0, min(iVScrollPos, lines - 1));//�ù�������������Χ
		if (iVScrollPos != GetScrollPos(hScroll, SB_CTL))//������λ�ñ仯��
		{
			SetScrollPos(hScroll, SB_CTL, iVScrollPos, true);//���ù�������λ��
			InvalidateRect(hwnd, nullptr, true);//�ػ�ͻ�������������
		}
		break;

	case WM_SETFOCUS://�����ڻ�ý���ʱ
		SetFocus(hScroll);//�ù�������ý���
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		rect = ps.rcPaint;
		rect.top -= cyChar*iVScrollPos;//���������Ϲ���iVScrollPos�У����0��iVScrollPos-1���ڿͻ�����
		DrawText(hdc, text, -1, &rect, DT_WORDBREAK);
		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_CLOSE://���ڹر�
		LoadString(hInst, IDS_ERROR, szError, sizeof(szBuffer)/sizeof(TCHAR));
		GetWindowText(hwnd, szBuffer, 64);
		MessageBox(nullptr, szError, szBuffer, MB_ICONWARNING);
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
	TCHAR szAppName[64], szCaptionName[64];
	LoadString(hInstance, IDS_APPNAME, szAppName, sizeof(szAppName)/sizeof(TCHAR));//����Դ�ļ���ȡ�ַ�������Ϊ��������
	LoadString(hInstance, IDS_CAPTIONNAME, szCaptionName, sizeof(szCaptionName) / sizeof(TCHAR));//����Դ�ļ���ȡ�ַ�������Ϊ���ڱ�������
	WNDCLASS wndclass;//������
	wndclass.cbClsExtra = 0;//Ϊ�����Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.cbWndExtra = 0;//Ϊ���ڷ���Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//������ˢ
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//�����
	wndclass.hIcon = LoadIcon(hInstance, TEXT("IDI_ICON"));//ͼ��
	wndclass.hInstance = hInstance;//ʵ�������������PID��һ���������������������ϵͳ�����ҵ����̵ĵ�ַ
	wndclass.lpfnWndProc = WndProc;//���ڹ���
	wndclass.lpszClassName = szAppName;//����
	wndclass.lpszMenuName = nullptr;//�˵�
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//��Щ�ı��Ҫ�ػ�������ˮƽ�ʹ�ֱ�ı���ػ�
	if (!RegisterClass(&wndclass))//ע�ᴰ����
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(szAppName,szCaptionName, WS_OVERLAPPEDWINDOW,
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