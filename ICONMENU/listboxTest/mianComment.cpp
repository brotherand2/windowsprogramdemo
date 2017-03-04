//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
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
const int ID_EDIT_LEFT = 1;
const int ID_EDIT_RIGHT = 2;
const int ID_BTN_CLEAR = 3;
const int ID_BTN_COPY = 4;
const int ID_BTN_PASTE = 5;
const int ID_BTN_SETTEXT = 6;
const int ID_BTN_GETTEXT = 7;

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static HWND hEditLeft, hEditRight, hBtnClear, hBtnPaste, hBtnCopy, hBtnGetText, hBtnSetText;
	TCHAR szBuffer[1024];
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		hEditLeft=CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("edit"), nullptr, WS_VISIBLE | WS_CHILD|ES_MULTILINE|WS_HSCROLL|WS_VSCROLL, 0, 0, 0, 0,
			hwnd, (HMENU)ID_EDIT_LEFT, hInst, nullptr);
		hEditRight = CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("edit"),nullptr,WS_VISIBLE | WS_CHILD | ES_MULTILINE|WS_HSCROLL|WS_VSCROLL, 0, 0, 0, 0,
			hwnd, (HMENU)ID_EDIT_RIGHT, hInst, nullptr);
		hBtnClear=CreateWindow(TEXT("button"), TEXT("�������"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0,
			hwnd, (HMENU)ID_BTN_CLEAR,hInst, nullptr);
		hBtnCopy=CreateWindow(TEXT("button"), TEXT("��������"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0,
			hwnd, (HMENU)ID_BTN_COPY, hInst, nullptr);
		hBtnGetText=CreateWindow(TEXT("button"), TEXT("�õ�����"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0,
			hwnd, (HMENU)ID_BTN_GETTEXT, hInst, nullptr);
		hBtnPaste=CreateWindow(TEXT("button"), TEXT("ճ������"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0,
			hwnd, (HMENU)ID_BTN_PASTE, hInst, nullptr);
		hBtnSetText=CreateWindow(TEXT("button"), TEXT("��������"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0,
			hwnd, (HMENU)ID_BTN_SETTEXT, hInst, nullptr);

		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		MoveWindow(hEditLeft,8*cxChar,4*cyChar,30*cxChar ,10*cyChar , true);
		MoveWindow(hEditRight,50*cxChar ,4*cyChar ,30*cxChar ,10*cyChar, true);
		MoveWindow(hBtnClear,8*cxChar ,17*cyChar ,10*cxChar ,2*cyChar, true);
		MoveWindow(hBtnCopy,8*cxChar ,19*cyChar+4,10*cxChar ,2*cyChar, true);
		MoveWindow(hBtnSetText,8*cxChar ,21*cyChar+8 ,10*cxChar ,2*cyChar, true);
		MoveWindow(hBtnPaste,50 * cxChar, 19 * cyChar + 4, 10 * cxChar, 2 * cyChar, true);
		MoveWindow(hBtnGetText,50 * cxChar, 21 * cyChar + 8, 10 * cxChar, 2 * cyChar, true);

		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		switch (wmId)
		{
		case ID_EDIT_LEFT://��༭��
			switch (wmEvent)
			{
			case EN_UPDATE:
				GetWindowText(hEditLeft, szBuffer, 1024);
				SetWindowText(hEditRight, szBuffer);
				break;
			}
			return 0;
		case ID_EDIT_RIGHT://�ұ༭��
			return 0;
		case ID_BTN_CLEAR://�����ť
			SendMessage(hEditLeft, WM_CLEAR, 0, 0);//���ѡ�е�

			return 0;
		case ID_BTN_COPY://���ư�ť
			SendMessage(hEditLeft, WM_COPY, 0, 0);//���ѡ�е�

			return 0;
		case ID_BTN_GETTEXT://����ı�
			GetWindowText(hEditLeft, szBuffer, 1024);
			SetWindowText(hEditRight, szBuffer);
			return 0;
		case ID_BTN_PASTE://ճ��
			SendMessage(hEditRight, WM_PASTE, 0, 0);//���ѡ�е�

			return 0;
		case ID_BTN_SETTEXT://�����ı�
			SetWindowText(hEditLeft, TEXT("This is a line"));

			return 0;
		}
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
	wndclass.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));//������ˢ
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