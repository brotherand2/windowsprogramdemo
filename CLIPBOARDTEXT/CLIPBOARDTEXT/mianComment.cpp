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
#define CF_CHAR CF_TEXT
TCHAR szDefaultText[] = TEXT("Default Text-Ansi Version");
TCHAR szCaptionText[] = TEXT("Clipbroad Text Transfer-Ansi Version");

#else
using  tstring = wstring;
# define  CF_CHAR CF_UNICODETEXT 
TCHAR szDefaultText[] = TEXT("Default Text-Unicode Version");
TCHAR szCaptionText[] = TEXT("Clipbroad Text Transfer-Unicode Version");

#endif

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	bool bEnabel;
	static TCHAR *pText;
	TCHAR *pGlobal;
	HGLOBAL hGlobal;
	RECT rect;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		SendMessage(hwnd, WM_COMMAND, ID_EDIT_RESET, 0);
		break;
	case WM_INITMENUPOPUP:
		EnableMenuItem((HMENU)wParam, ID_EDIT_PASTE, IsClipboardFormatAvailable(CF_CHAR) ? MF_ENABLED : MF_GRAYED);
		bEnabel = pText ? MF_ENABLED : MF_GRAYED;
		EnableMenuItem((HMENU)wParam, ID_EDIT_COPY, bEnabel);
		EnableMenuItem((HMENU)wParam, ID_EDIT_CUT, bEnabel);
		EnableMenuItem((HMENU)wParam, ID_EDIT_DELETE, bEnabel);
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
		case ID_EDIT_COPY:
		case ID_EDIT_CUT:
			if (!pText)
				return 0;
			hGlobal = GlobalAlloc(GHND | GMEM_SHARE, (lstrlen(pText) + 1)*sizeof(TCHAR));
			pGlobal = (TCHAR*)GlobalLock(hGlobal);
			lstrcpy(pGlobal, pText);
			GlobalUnlock(hGlobal);
			OpenClipboard(hwnd);
			EmptyClipboard();
			SetClipboardData(CF_CHAR, hGlobal);
			CloseClipboard();
			if (wmId == ID_EDIT_COPY)
				break;
		case ID_EDIT_DELETE:
			if (pText)
			{
				free(pText);
				pText = nullptr;
			}
			InvalidateRect(hwnd, nullptr, true);
			break;
		case ID_EDIT_PASTE:
			OpenClipboard(hwnd);
			hGlobal = GetClipboardData(CF_CHAR);
			if (hGlobal)
			{
				pGlobal =(TCHAR *) GlobalLock(hGlobal);
				if (pText)
				{
					free(pText);
					pText = nullptr;
				}
				pText =(TCHAR*) malloc(GlobalSize(hGlobal));
				lstrcpy(pText, pGlobal);
				InvalidateRect(hwnd, nullptr, true);
			}
			CloseClipboard();
			break;
		case ID_EDIT_RESET:
			if (pText)
			{
				free(pText);
				pText = nullptr;
			}
			pText = (TCHAR*)malloc((lstrlen(szDefaultText) + 1)*sizeof(TCHAR));
			lstrcpy(pText, szDefaultText);
			InvalidateRect(hwnd, nullptr, true);
			break;
		}
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		GetClientRect(hwnd, &rect);
		if (pText)
			DrawText(hdc, pText, -1, &rect, DT_WORDBREAK|DT_EXPANDTABS);
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
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);//�˵�
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//��Щ�ı��Ҫ�ػ�������ˮƽ�ʹ�ֱ�ı���ػ�
	if (!RegisterClass(&wndclass))//ע�ᴰ����
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(TEXT("zeng"), szCaptionText, WS_OVERLAPPEDWINDOW,
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