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
HWND hDialogModeLess;//��ģ̬�Ի���

int _stdcall DialogProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)//�Ի��򴰿ڹ���
{
	int wmId = LOWORD(wParam);//�ؼ�ID
	int wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
	static int iColor[3];
	int index;//����
	HWND hCtrl;
	HWND hParent;//�����ھ��
	switch (message)
	{
	//case WM_COMMAND:

	//	return false;
	case WM_INITDIALOG:
		for (int id = IDC_SCROLLRED; id <= IDC_SCROLLBLUE; id++)//3��������ID������ȴIDC_SCROLLRED��С
		{
			hCtrl = GetDlgItem(hDlg, id);//��õ�ǰ���������
			SetScrollRange(hCtrl, SB_CTL,0, 255, false);//���ù�������Χ
			SetScrollPos(hCtrl, SB_CTL, 0, false);//���ù�����λ��
		}
		return false;//�õ�1��TABSTOP��ý���
	case WM_VSCROLL:
		hCtrl = (HWND)lParam;
		hParent = GetParent(hDlg);
		index = GetDlgCtrlID(hCtrl)-IDC_SCROLLRED;//3��������ID������ȴIDC_SCROLLRED��С
		switch (wmId)
		{
		case SB_PAGEDOWN:
			iColor[index] += 15;
		case SB_LINEDOWN:
			iColor[index] = min(255, iColor[index] + 1);
			break;
		case SB_PAGEUP:
			iColor[index] -= 15;
		case SB_LINEUP:
			iColor[index] = max(0, iColor[index] -1);
			break;
		case SB_TOP:
			iColor[index] = 0;
			break;
		case SB_BOTTOM:
			iColor[index] = 255;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			iColor[index] = wmEvent;
			break;
		}
		SetScrollPos((HWND)lParam, SB_CTL, iColor[index], true);
		SetDlgItemInt(hDlg, GetWindowLong(hCtrl, GWL_ID)+3, iColor[index], true);//�������ɫֵID����Ӧ������ID��3
		DeleteObject((HGDIOBJ)SetClassLong(hParent, GCL_HBRBACKGROUND,(long)CreateSolidBrush(RGB(iColor[0], iColor[1], iColor[2]))));
		InvalidateRect(hParent,nullptr,true);
		return true;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return false;
	}
	return false;
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		hDialogModeLess = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DialogProc);//��showwindow()�󴴽����öԻ����ý���
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		SetFocus(hDialogModeLess);
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����

		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���

		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������ʱ
		DeleteObject((HGDIOBJ)SetClassLong(hwnd, GCL_HBRBACKGROUND, (long)GetStockObject(WHITE_BRUSH)));
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
		if (hDialogModeLess == 0 || !IsDialogMessage(hDialogModeLess, &msg))//����޴���䣬��TAB�������ڿؼ����л�����
		{
			TranslateMessage(&msg);//��һЩ������Ϣת�����ַ���Ϣ���ڰ�����Ϣ����
			DispatchMessage(&msg);//�ַ���Ϣ�������ô��ڹ��̽��д�����Ϣ
		}
	}
}