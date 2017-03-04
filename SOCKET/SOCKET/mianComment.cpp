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
#define WM_SOCKET_NOTIFY (WM_USER+1)
int  _stdcall ServerProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam);
void EditPrintf(HWND hEdit, TCHAR *szFormat, ...);
void ChangeLocalTime(HWND hEdit, const SYSTEMTIME &stOld, const SYSTEMTIME &stNew);
static bool upDateTime = false;
void ShowReturnTime(HWND hEdit, unsigned long ulTime)
{
	SYSTEMTIME stNew;
	SYSTEMTIME stOld;
	FILETIME ft;
	LARGE_INTEGER li;
	TCHAR szTime[64], szDate[64];
	GetLocalTime(&stOld);
	stNew.wYear = 1900;
	stNew.wMonth = stNew.wDay = 1;
	stNew.wHour = stNew.wMinute = stNew.wSecond = stNew.wMilliseconds = 0;
	SystemTimeToFileTime(&stNew, &ft);//��1900��1��1��ϵͳʱ��ת��Ϊ1601��1��1�յ�1900��1��1�յ��ļ�ʱ�䣬��100nsΪ��λ
	li = *(LARGE_INTEGER*)&ft;
	li.QuadPart += (LONGLONG)10000000 * ulTime;//�ټ���1900��������ļ�ʱ��
	ft = *(FILETIME*)&li;//1601�굽���ڵ��ļ�ʱ��
	FileTimeToLocalFileTime(&ft, &ft);//��UTC�ļ�ʱ��ת���ɱ����ļ�ʱ��
	FileTimeToSystemTime(&ft, &stNew);//���ļ�ʱ��ת��Ϊϵͳʱ��
	GetDateFormat(LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE | DATE_SHORTDATE, &stNew, 0, szDate, sizeof(szDate) / sizeof(TCHAR));
	GetTimeFormat(LOCALE_USER_DEFAULT, 0, &stNew, 0, szTime, sizeof(szTime) / sizeof(TCHAR));
	EditPrintf(hEdit, TEXT("\r\n�������Ϸ��ص���ʱ�� %s  %s %d\r\n"), szDate, szTime,stNew.wMilliseconds);
	if (upDateTime)
		ChangeLocalTime(hEdit, stOld, stNew);
}
void ChangeLocalTime(HWND hEdit, const SYSTEMTIME &stOld, const SYSTEMTIME &stNew)
{
	TCHAR szOldTime[64], szOldDate[64];
	TCHAR szTime[64], szDate[64];
	if (SetLocalTime(&stNew))
	{
		GetDateFormat(LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE | DATE_SHORTDATE, &stNew, 0, szDate, sizeof(szDate) / sizeof(TCHAR));
		GetTimeFormat(LOCALE_USER_DEFAULT, 0, &stNew, 0, szTime, sizeof(szTime) / sizeof(TCHAR));
		GetDateFormat(LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE | DATE_SHORTDATE, &stOld, 0, szOldDate, sizeof(szOldDate) / sizeof(TCHAR));
		GetTimeFormat(LOCALE_USER_DEFAULT, 0, &stOld, 0, szOldTime, sizeof(szOldTime) / sizeof(TCHAR));
		EditPrintf(hEdit, TEXT("ϵͳ��ǰʱ��ɹ��ı䣬��\r\n\t%s %s.%d to\r\n\t%s %s.%d\r\n"),
			szOldDate, szOldTime, stOld.wMilliseconds, szDate, szTime, stNew.wMilliseconds);
	}
	else
		EditPrintf(hEdit, TEXT("�޷��޸ı���ʱ�䣬�����Թ���Ա�������"));
}
void EditPrintf(HWND hEdit, TCHAR *szFormat, ...)
{
	TCHAR szBuffer[1024];
	va_list pArgList;
	va_start(pArgList, szFormat);
	wvsprintf(szBuffer,szFormat, pArgList);
	va_end(pArgList);
	SendMessage(hEdit, EM_SETSEL, -2, -1);
	SendMessage(hEdit, EM_REPLACESEL, false, (long)szBuffer);
	SendMessage(hEdit,EM_SCROLLCARET, 0, 0);
}
int  _stdcall DlgProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;
	static TCHAR szIPAddress[64] = TEXT("129.6.15.30");
	int iError;
	WSADATA wsaData;
	static sockaddr_in  sa;//�����׽���IP��ַ��˿ںŽṹ
	static HWND hEdit,hButton;
	static SOCKET sock;
	const int ID_TIMER = 1;
	const int ID_TIMERWAIT = 2;
	const int IDC_CANCEL = 2;
	unsigned short wEvent, wError;
	unsigned long ulTime;
	switch (UMsg)
	{
	case WM_INITDIALOG:
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
		hEdit = GetDlgItem(hwnd, IDC_EDIT1);
		hButton = GetDlgItem(hwnd, IDC_SHOWSERVERTIME);
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		switch (wmId)
		{
		case IDC_SELECTSERVER:
			DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, ServerProc, (long)szIPAddress);
			break;
		case IDC_UPDATETIME:
			upDateTime = true;
		case IDC_SHOWSERVERTIME:
			iError = WSAStartup(MAKELONG(2, 0), &wsaData);//����2.0�汾���׽���
			if (iError)
			{
				EditPrintf(hEdit, TEXT("�׽��ֳ�ʼ��ʧ�ܣ��������ɹ� #error %d\r\n"), iError);
				return true;
			}
			EditPrintf(hEdit, TEXT("�׽��� %s �汾�����ɹ�\r\n"), wsaData.szDescription);
			sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//����TCP/IP���׽���
			if (sock == INVALID_SOCKET)
			{
				EditPrintf(hEdit, TEXT("�׽��ִ���ʧ�� %d\r\n"), WSAGetLastError());
				WSACleanup();
				return true;
			}
			EditPrintf(hEdit, TEXT("�׽��� %d����\r\n"), sock);
			if (SOCKET_ERROR == WSAAsyncSelect(sock, hwnd, WM_SOCKET_NOTIFY, FD_CONNECT | FD_READ))//���׽��ֵ����ӺͶ�ȡ������ʽ��Ϊ������ʽ
			{//�������ӺͶ�ȡ����Ϣ���͵��Զ�����ϢWM_SOCKET_NOTIFY
				EditPrintf(hEdit, TEXT("�׽��ֵ����ӺͶ�ȡ������ʽ��Ϊ������ʽ ����#%d\r\n"), WSAGetLastError());
				closesocket(sock);
				WSACleanup();
				return true;
			}
			//��ʼ���˿ںź�IP��ַ
			sa.sin_addr.S_un.S_addr = inet_addr(szIPAddress);//��IP��ַ�ӵ�����ʽת�����޷��ų�����
			sa.sin_family = AF_INET;
			sa.sin_port = htons(IPPORT_TIMESERVER);//�����ض˿ں�32ת��������˿ں�
			connect(sock, (sockaddr*)&sa, sizeof(sa));//���ӷ�����
			if (WSAEWOULDBLOCK != (iError = WSAGetLastError()))//����WSAEWOULDBLOCK˵��������ʽconnect�����ڷ�����ʽ
			{
				EditPrintf(hEdit, TEXT("����ʧ�� ����#%d\r\n"), WSAGetLastError());
				closesocket(sock);
				WSACleanup();
				return true;
			}
			EditPrintf(hEdit, TEXT("���� %s ..."), szIPAddress);
			SetTimer(hwnd, ID_TIMER, 1000, 0);
			SetTimer(hwnd, ID_TIMERWAIT, 60000, 0);
			SetWindowText(hButton, TEXT("ȡ��"));
			SetWindowLong(hButton, GWL_ID, IDC_CANCEL);
			break;
		case IDC_CANCEL:
			KillTimer(hwnd, ID_TIMER);
			WSACleanup();
			closesocket(sock);
			sock = 0;
			upDateTime = false;
			SetWindowText(hButton, TEXT("���ӷ�����"));
			SetWindowLong(hButton, GWL_ID,IDC_SHOWSERVERTIME);
			EditPrintf(hEdit, TEXT("\r\n�׽��ֹر�\r\n"));
			break;
		default:
			break;
		}
		break;
	case WM_SOCKET_NOTIFY:
		wError = WSAGETSELECTERROR(lParam);
		wEvent = WSAGETSELECTEVENT(lParam);
		switch (wEvent)
		{
		case FD_CONNECT:
			if (wError)
			{
				EditPrintf(hEdit, TEXT("\r\n����ʧ�� ����#%d\r\n"), wError);
				SendMessage(hwnd, WM_COMMAND, IDC_CANCEL, 0);
				return true;
			}
			EditPrintf(hEdit, TEXT("\r\n�ɹ������� %s\r\n"), szIPAddress);
			recv(sock, (char*)&ulTime, 4, MSG_PEEK);//����������TCP/IPЭ�鿪ʼ��������
			EditPrintf(hEdit, TEXT("���ڵȴ�����..."));
			break;
		case FD_READ:
			KillTimer(hwnd, ID_TIMER);
			if (wError)
			{
				EditPrintf(hEdit, TEXT("��ȡʧ�� #����%d\r\n"), wError);
				SendMessage(hwnd, WM_COMMAND, IDC_CANCEL, 0);
				return true;
			}
			recv(sock, (char*)&ulTime, 4, 0);//��ȡ���ջ���������
			ulTime = ntohl(ulTime);//������ʱ��ת���ɱ���ʱ��
			EditPrintf(hEdit, TEXT("\r\n��ǰ���յ�ʱ������1990��1��1�վ�����ʱ�� %u ��"), ulTime);
			ShowReturnTime(hEdit, ulTime);
			SendMessage(hwnd, WM_COMMAND, IDC_CANCEL, 0);
			break;
		default:
			break;
		}
		break;
	case WM_TIMER:
		if (wParam==ID_TIMER)
		EditPrintf(hEdit, TEXT("."));
		else
		{
			EditPrintf(hEdit, TEXT("\r\n�ȴ���ʱ,��������.\r\n"));
			SendMessage(hwnd, WM_COMMAND, IDC_CANCEL, 0);
		}
		break;
	case WM_CLOSE:
		if (sock)
			SendMessage(hwnd, WM_COMMAND, IDC_CANCEL, 0);
		DestroyWindow(hwnd);
		break;
	default:
		break;
	}
	return false;
}
int  _stdcall ServerProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	static int server=IDC_RADIO1;
	TCHAR szLabel[64];
	static TCHAR *szServer;
	char *context;
	switch (UMsg)
	{
	case WM_INITDIALOG:
		szServer = (TCHAR*)lParam;
		CheckRadioButton(hwnd, IDC_RADIO1, IDC_RADIO10, server);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO1:
		case IDC_RADIO2:
		case IDC_RADIO3:
		case IDC_RADIO4:
		case IDC_RADIO5:
		case IDC_RADIO6:
		case IDC_RADIO7:
		case IDC_RADIO8:
		case IDC_RADIO9:
		case IDC_RADIO10:
			server = LOWORD(wParam);
			break;
		case IDOK:
			GetDlgItemText(hwnd, server, szLabel, sizeof(szLabel) / sizeof(TCHAR));
			strtok_s(szLabel, "(",&context);
			lstrcpy(szServer, strtok_s(0, ")",&context));
		case IDCANCEL:
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
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	static HINSTANCE hInst;//����ʵ�����
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���

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
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("�»�"), WS_OVERLAPPEDWINDOW&(~(WS_MAXIMIZEBOX | WS_THICKFRAME)),
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);//��������
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
		return 0;
	}
	HWND hwndModeLess = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DlgProc);

	RECT rect;
	GetClientRect(hwndModeLess, &rect);
	AdjustWindowRect(&rect, WS_CAPTION | WS_BORDER, false);
	SetWindowPos(hwnd, 0, 0, 0, rect.right-rect.left, rect.bottom-rect.top, SWP_NOMOVE);
	ShowWindow(hwnd, nCmdShow);//��ʾ����
	UpdateWindow(hwnd);//�����ػ�
	ShowWindow(hwndModeLess, SW_SHOW);

	MSG msg;//��Ϣ�ṹ
	while (GetMessage(&msg, nullptr, 0, 0))//����Ϣ���л�ȡ��Ϣ
	{
		if (hwndModeLess == 0 || !IsDialogMessage(hwndModeLess, &msg))
		{
			TranslateMessage(&msg);//��һЩ������Ϣת�����ַ���Ϣ���ڰ�����Ϣ����
			DispatchMessage(&msg);//�ַ���Ϣ�������ô��ڹ��̽��д�����Ϣ
		}
	}
}