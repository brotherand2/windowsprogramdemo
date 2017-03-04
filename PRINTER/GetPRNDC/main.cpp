//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<strsafe.h>
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
# define IDM_DEVMODE 1000

void BasicInfo(HDC hdc, HDC hdcInfo, int cxChar, int cyChar)
{
	static struct
	{
		int index;
		TCHAR *szDesc;
	}
	info[] =
	{
		HORZSIZE, TEXT("HORZSIZE �Ժ���Ϊ��λ����ʾ���"),
		VERTSIZE, TEXT("VERTSIZE �Ժ���Ϊ��λ����ʾ�߶�"),
		HORZRES, TEXT("HORZRES ������Ϊ��λ����ʾ��� 0~65535"),
		VERTRES, TEXT("VERTRES ������Ϊ��λ����ʾ�߶� 0~65535"),
		BITSPIXEL, TEXT("BITSPIXELÿ��������ɫλ��"),
		PLANES, TEXT("PLANES����ɫλ����"),
		NUMBRUSHES, TEXT("NUMBRUSHES���豸ָ����ˢ��"),
		NUMPENS, TEXT("NUMPENS���豸ָ������"),
		NUMMARKERS, TEXT("NUMMARKERS      Number of device markers:"),
		NUMFONTS, TEXT("NUMFONTS���豸ָ��������"),
		NUMCOLORS, TEXT("NUMCOLORS���豸��ɫ��������������豸��ɫ�����8λ���ء����ڳ���ɫ����豸���أ�1"),
		PDEVICESIZE, TEXT("PDEVICESIZE     Size of device structure:"),
		ASPECTX, TEXT("ASPECTX�����ڻ��ߵ��豸���ص���Կ��"),
		ASPECTY, TEXT("ASPECTY�����ڻ��ߵ��豸���ص���Ը߶�"),
		ASPECTXY, TEXT("ASPECTXY        Relative diagonal of pixel:"),
		LOGPIXELSX, TEXT("LOGPIXELSX ����/�߼�Ӣ�磨ˮƽ��"),
		LOGPIXELSY, TEXT("LOGPIXELSY ����/�߼�Ӣ�磨��ֱ��"),
		SIZEPALETTE, TEXT("SIZEPALETTE     Number of palette entries:"),
		NUMRESERVED, TEXT("NUMRESERVED     Reserved palette entries:"),
		COLORRES, TEXT("COLORRES        Actual color resolution:"),
		PHYSICALWIDTH, TEXT("PHYSICALWIDTH   Printer page pixel width:"),
		PHYSICALHEIGHT, TEXT("PHYSICALHEIGHT  Printer page pixel height:"),
		PHYSICALOFFSETX, TEXT("PHYSICALOFFSETX Printer page x offset:"),
		PHYSICALOFFSETY, TEXT("PHYSICALOFFSETY Printer page y offset:")
	};
	TCHAR szBuffer[1024];
	for (int i = 0; i < sizeof(info) / sizeof(info[0]); i++)
	{
		StringCchPrintf(szBuffer, 1024, TEXT("%-60s%-8d"), info[i].szDesc, GetDeviceCaps(hdcInfo, info[i].index));
		TextOut(hdc, 0, i*cyChar, szBuffer, lstrlen(szBuffer));
	}
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	HDC hdcInfo;
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	unsigned long dwNeeded;//�����ӡ���Ľṹ����
	unsigned long dwReturned;//���صı����ӡ���ṹ��С
	static PRINTER_INFO_4 *pinfo4;//��ӡ����Ϣ�ṹָ��
	static int nCurrentDevice = IDM_SCREEN;//��ǰ�豸ID
	static int nCurrentInfo = IDM_BASIC;//��ǰ��Ϣ�˵�ID
	HMENU menu;
	static TCHAR szDeviceName[128];//�豸��
	static TCHAR szWindowText[128];//���ڱ���
	HANDLE hPrint;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
	case WM_SETTINGCHANGE:
		menu = GetSubMenu(GetMenu(hwnd), 0);//�õ���1���ֲ˵����豸�˵�
		while(GetMenuItemCount(menu) > 1)//����˵���Ŀ��������1
			DeleteMenu(menu, 1, MF_BYPOSITION);//ɾ����2���˵�
		EnumPrinters(PRINTER_ENUM_LOCAL, nullptr, 4, nullptr, 0, &dwNeeded, &dwReturned);//ȡ����Ҫ�����ӡ���ṹ��С
		pinfo4 = (PRINTER_INFO_4*) malloc(dwNeeded);//����ռ䱣��
		EnumPrinters(PRINTER_ENUM_LOCAL, nullptr, 4,(PBYTE) pinfo4, dwNeeded, &dwNeeded, &dwReturned);//�����ڴ�ӡ����Ϣ������pInfo4�ṹ��
		for (int i = 0; i < dwReturned; i++)//��ÿ̨��ӡ�������ּӵ��豸�˵���
			AppendMenu(menu,MF_STRING, i + 1, pinfo4[i].pPrinterName);
		free(pinfo4);
		AppendMenu(menu, MF_SEPARATOR, 0, nullptr);//����ָ���
		AppendMenu(menu, MF_STRING, IDM_DEVMODE, TEXT("properties"));//�������Բ˵���
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		menu = GetMenu(hwnd);
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		if (wmId == IDM_SCREEN || wmId < IDM_DEVMODE)
		{
			CheckMenuItem(menu, nCurrentDevice, MF_UNCHECKED);//ȡ��ѡ����һ��ѡ�е�
			nCurrentDevice = wmId;
			CheckMenuItem(menu, wmId, MF_CHECKED);//ѡ�е�ǰѡ��
		}
		else
		{
			if (wmId == IDM_DEVMODE)
			{
				GetMenuString(menu, nCurrentDevice,szDeviceName, sizeof(szDeviceName) / sizeof(TCHAR), MF_BYCOMMAND);//�õ��˵���
				if (OpenPrinter(szDeviceName, &hPrint, nullptr))//�򿪴�ӡ��
				{
					PrinterProperties(hwnd, hPrint);//�򿪴�ӡ�����ԶԻ���
					ClosePrinter(hPrint);
				}
			}
			else
			{
				CheckMenuItem(menu, nCurrentInfo, MF_UNCHECKED);
				nCurrentInfo = wmId;
				CheckMenuItem(menu, wmId, MF_CHECKED);
			}
		}
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_INITMENUPOPUP:
		if (lParam==0)//������豸�˵�����
		EnableMenuItem(GetMenu(hwnd), IDM_DEVMODE, (nCurrentDevice == IDM_SCREEN) ? MF_GRAYED : MF_ENABLED);
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		lstrcpy(szWindowText, TEXT("�豸����:"));
		if (nCurrentDevice == IDM_SCREEN)
		{
			lstrcat(szWindowText, TEXT("��Ļ"));
			hdcInfo = CreateIC(TEXT("DISPLAY"), 0, 0, 0);
		}
		else
		{
			GetMenuString(GetMenu(hwnd), nCurrentDevice, szDeviceName, sizeof(szDeviceName) / sizeof(TCHAR), MF_BYCOMMAND);
			lstrcat(szWindowText, szDeviceName);
			hdcInfo = CreateIC(0, szDeviceName, 0, 0);
		}
		SetWindowText(hwnd, szWindowText);
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		if (hdcInfo)
		{
			switch (nCurrentInfo)
			{
			case IDM_BASIC:
				BasicInfo(hdc, hdcInfo, cxChar, cyChar);
				break;
			case IDM_CURVE:
			case IDM_DEVMODE:
			case IDM_LINE:
			case IDM_OTHER:
			case IDM_POLY:
				break;
			}
			DeleteDC(hdcInfo);
		}
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