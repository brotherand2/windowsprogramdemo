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
HWND hDlgModeLess=0;
bool bAbort = false;
void PageGDICalls(HDC hdc, int cxPage, int cyPage)
{
	static TCHAR szText[] = TEXT("Hello ,Printer");
	Rectangle(hdc, 0, 0, cxPage, cyPage);
	MoveToEx(hdc, 0, 0, nullptr);
	LineTo(hdc, cxPage, cyPage);
	MoveToEx(hdc, cxPage, 0, nullptr);
	LineTo(hdc, 0, cyPage);
	SaveDC(hdc);
	SetMapMode(hdc, MM_ISOTROPIC);
	SetWindowExtEx(hdc, 1000, 1000, nullptr);
	SetViewportExtEx(hdc, cxPage / 2, -cyPage / 2, 0);
	SetViewportOrgEx(hdc, cxPage / 2, cyPage / 2, 0);
	Ellipse(hdc, -500, 500, 500, -500);
	SetTextAlign(hdc, TA_CENTER);
	TextOut(hdc, 0, 0, szText, lstrlen(szText));
	RestoreDC(hdc, -1);
}
HDC GetPrinterDC()
{
	HDC hdc;
	unsigned long dwNeeded, dwReturned;
	PRINTER_INFO_4 *pinfo4;
	EnumPrinters(PRINTER_ENUM_LOCAL, 0, 4, 0, 0, &dwNeeded, &dwReturned);
	pinfo4 = (PRINTER_INFO_4*)malloc(dwNeeded);
	EnumPrinters(PRINTER_ENUM_LOCAL, 0, 4, (PBYTE)pinfo4, dwNeeded, &dwNeeded, &dwReturned);
	hdc = CreateDC(0, pinfo4[0].pPrinterName, 0, 0);
	free(pinfo4);
	return hdc;
}
int _stdcall AbortProc(HDC hdc,int iCode)
{
	MSG msg;
	while(!bAbort&& PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (!hDlgModeLess || !IsDialogMessage(hDlgModeLess,&msg))
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Sleep(8000);

	MessageBox(0, TEXT("PRINT PAGE "), 0, 0);
	return !bAbort;
}
int _stdcall DlgProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)
{
	TCHAR szCaption[128];
	switch (message)
	{

	case WM_INITDIALOG:
		GetWindowText(GetParent(hDlg), szCaption, 128);
		SetWindowText(hDlg, szCaption);
		EnableMenuItem(GetSystemMenu(hDlg, false), SC_CLOSE, MF_GRAYED);
		return true;
	case WM_COMMAND:
		bAbort = true;
		EnableWindow(GetParent(hDlg), true);
		DestroyWindow(hDlg);
		hDlgModeLess = 0;
		return true;
	}
	return false;
}
bool MyPrintPage(HWND hwnd,HINSTANCE hInst)
{
	hDlgModeLess = CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DlgProc);

	HDC hdc = GetPrinterDC();
	if (!hdc)
		return false;
	static DOCINFO di;
	memset(&di, 0, sizeof(di));
	di.cbSize = sizeof(di);
	di.lpszDocName = TEXT("PRINT2:");
	int xPage = GetDeviceCaps(hdc, HORZRES);
	int yPage = GetDeviceCaps(hdc, VERTRES);
	EnableWindow(hwnd, false);
	SetAbortProc(hdc, AbortProc);

	if (StartDoc(hdc, &di))
	{
		if (StartPage(hdc))
		{
			PageGDICalls(hdc, xPage, yPage);
			if (EndPage(hdc))
				EndDoc(hdc);
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
	EnableWindow(hwnd, true);
	DeleteDC(hdc);
	return true;
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	HMENU hMenu;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		hMenu = GetSystemMenu(hwnd, false);
		AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
		AppendMenu(hMenu, MF_STRING, 1, TEXT("&Print"));
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		break;
	case WM_SYSCOMMAND:
		if (wParam == 1)
			if (!MyPrintPage(hwnd,hInst))
				MessageBox(hwnd, TEXT("can not Print page"), 0, 0 | MB_ICONEXCLAMATION);
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����

		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		PageGDICalls(hdc, cxClient, cyClient);
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