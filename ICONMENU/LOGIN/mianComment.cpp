//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
# define IDS_APP_NAME 1
#define IDS_APP_CAPTION 2
# define ID_USERNAME_EDIT 3
# define ID_USERPASSWORD_EDIT 4
# define ID_USERNAME_LABEL 5
# define ID_USERPASSWORD_LABEL 6
# define ID_BTN_LOGIN 7
# define ID_BTN_CANCEL 8
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
	static HWND hUserNameEdit;//�û����༭��
	static HWND hUserPasswordEdit;//����༭��
	static HWND hUserNameLabel;//�û�����ǩ
	static HWND hUserPasswordLabel;//�����ǩ
	static HWND hBtnLogin;//��¼��ť
	static HWND hBtnCancel;//ȡ����ť

	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		hUserNameEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE|WS_BORDER, 0, 0, 0, 0,
			hwnd, (HMENU)ID_USERNAME_EDIT, hInst, nullptr);
		hUserPasswordEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE|WS_BORDER|ES_PASSWORD, 0, 0, 0, 0,
			hwnd, (HMENU)ID_USERPASSWORD_EDIT, hInst, nullptr);
		hUserNameLabel = CreateWindow(TEXT("static"), TEXT("�������û���"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
			hwnd, (HMENU)ID_USERNAME_LABEL, hInst, nullptr);
		hUserPasswordLabel = CreateWindow(TEXT("static"), TEXT("����������"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
			hwnd, (HMENU)ID_USERPASSWORD_LABEL, hInst, nullptr);
		hBtnLogin= CreateWindow(TEXT("button"), TEXT("��¼"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
			hwnd, (HMENU)ID_BTN_LOGIN, hInst, nullptr);
		hBtnCancel = CreateWindow(TEXT("button"), TEXT("ȡ��"), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
			hwnd, (HMENU)ID_BTN_CANCEL, hInst, nullptr);
		cxChar = HIWORD(GetDialogBaseUnits());//���Ի�������ĸ�ĸ߶���Ϊ�ؼ���һ��ˮƽ��Ԫ
		cyChar = LOWORD(GetDialogBaseUnits());//���Ի�������ĸ�Ŀ����Ϊ�ؼ���һ����ֱ��Ԫ

		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		MoveWindow(hUserNameLabel, 3 * cxChar, 3 * cyChar, 20 * cxChar, 3 * cyChar, true);
		MoveWindow(hUserNameEdit, 3 * cxChar,6 * cyChar, 20 * cxChar, 3 * cyChar, true);
		MoveWindow(hUserPasswordLabel, 3 * cxChar,9 * cyChar, 20 * cxChar, 3 * cyChar, true);
		MoveWindow(hUserPasswordEdit, 3 * cxChar, 12 * cyChar, 20 * cxChar, 3 * cyChar, true);
		MoveWindow(hBtnLogin, 3 * cxChar, 16 * cyChar,6 * cxChar, 3 * cyChar, true);
		MoveWindow(hBtnCancel, 10 * cxChar,16 * cyChar, 6 * cxChar, 3 * cyChar, true);
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
		PostQuitMessage(0);//�׳�һ������ֵΪ0�˳���Ϣ
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//��Ĭ�ϵĴ��ڹ��̴���������Ϣ����Щ��������
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	TCHAR szAppName[64], szCaptionName[64];
	LoadString(hInstance,IDS_APP_NAME ,szAppName,64 );
	LoadString(hInstance, IDS_APP_CAPTION, szCaptionName, 64);
	WNDCLASS wndclass;//������
	wndclass.cbClsExtra = 0;//Ϊ�����Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.cbWndExtra = 0;//Ϊ���ڷ���Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.hbrBackground =CreateSolidBrush(GetSysColor(COLOR_BTNFACE));//������ˢ
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//�����
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);//ͼ��
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
	int cxChar = HIWORD(GetDialogBaseUnits());//���Ի�������ĸ�ĸ߶���Ϊ�ؼ���һ��ˮƽ��Ԫ
	int cyChar = LOWORD(GetDialogBaseUnits());//���Ի�������ĸ�Ŀ����Ϊ�ؼ���һ����ֱ��Ԫ

	HWND hwnd = CreateWindow(szAppName, szCaptionName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,25*cxChar,25*cyChar,
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