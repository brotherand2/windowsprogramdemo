//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include"resource.h"
#include"strlib.h"
#pragma comment(lib,"strlib.lib")
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
const int STATE_ENTER = 0;
const int STATE_DELETE = 1;
int iState = 0;
typedef struct
{
	HDC hdc;
	int cxChar;
	int cyChar;
	int xStart, yStart;
	int xInc, yInc;
	int xMax, yMax;
}CBPARAM;
unsigned int iDataChange;
int _stdcall DlgProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	static TCHAR *szMessage[] = { TEXT("�����ַ���:"), TEXT("ɾ���ַ���:") };
	TCHAR szBuffer[MAX_STRINGS+1];
	switch (UMsg)
	{
	case WM_INITDIALOG:
		SetWindowText(hwnd, szMessage[iState]);
		SetDlgItemText(hwnd, IDC_TIP, szMessage[iState]);
		SendDlgItemMessage(hwnd, IDC_EDIT1, EM_LIMITTEXT, MAX_LENGTH, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_EDIT1:
			if (HIWORD(wParam)==EN_MAXTEXT)
				MessageBox(hwnd, TEXT("�ַ������ַ��������ܳ���63��"), 0, 0);
			break;
		case IDC_OK:
			GetDlgItemText(hwnd, IDC_EDIT1, szBuffer, MAX_STRINGS);
			if (iState ==STATE_ENTER)
			{
				if (AddString(szBuffer))
					PostMessage(HWND_BROADCAST, iDataChange, 0, 0);
				else
					MessageBeep(0);
			}
			else
			{
				if (DeleteString(szBuffer))
					PostMessage(HWND_BROADCAST, iDataChange, 0, 0);
				else
					MessageBeep(0);
			}	
		case IDC_CANCEL:
			EndDialog(hwnd, 0);
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default:
		break;
	}
	return false;
}
BOOL _stdcall GetStrCallBack(TCHAR *str,void *pvoid)
{
	CBPARAM *param = (CBPARAM*)pvoid;
	TextOut(param->hdc, param->xStart, param->yStart, str, lstrlen(str));
	if ((param->yStart += param->yInc) > param->yMax)
	{
		param->yStart = param->cyChar;
		if ((param->xStart += param->xInc) > param->xMax)
			return FALSE;
	}
	return TRUE;
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static CBPARAM param;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		param.cxChar = cxChar;
		param.cyChar = cyChar;
		param.xInc = cxChar*MAX_LENGTH;
		param.yInc = cyChar;
		iDataChange = RegisterWindowMessage(TEXT("StringDataChange"));
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		param.xMax = cxClient + param.xInc;
		param.yMax = cyClient - cyChar;
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		switch (wmId)
		{
		case ID_INPUT:
			iState = STATE_ENTER;
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DlgProc);
			break;
		case ID_DELETE:
			iState = STATE_DELETE;
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DlgProc);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		param.hdc = hdc;
		param.xStart = cxChar;
		param.yStart = cyChar;
		GetString(GetStrCallBack, &param);
		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������ʱ
		PostQuitMessage(0);//�׳�һ������ֵΪ0�˳���Ϣ
		break;
	default:
		if (UMsg == iDataChange)
			InvalidateRect(hwnd, 0, true);
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