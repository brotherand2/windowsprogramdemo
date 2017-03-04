//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include"resources.h"
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
	HINSTANCE hInst;
	static int is_Selected = ID_BKG_WHITE;//��ǰѡ�е��ǰ׻�ˢ
	int idColor[5] = { WHITE_BRUSH, GRAY_BRUSH, LTGRAY_BRUSH, DKGRAY_BRUSH, BLACK_BRUSH };//���ϵͳ��ˢ
	POINT pt;//�������
	static HMENU menu;//ϵͳ�˵�
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//�õ�����ʵ������
		menu = LoadMenu(hInst,TEXT("POPUPMENU"));//����Դ�ļ���ȡ�˵�
		menu = GetSubMenu(menu, 0);//�õ��Ӳ˵�
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		cxIcon = GetSystemMetrics(SM_CXICON);//�õ�ϵͳͼ���ȡ��߶�
		cyIcon = GetSystemMetrics(SM_CYICON);
		icon = LoadIcon(hInst, TEXT("IDI_ICON"));
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		break;
	case WM_RBUTTONDOWN://����Ҽ�����
		pt.x = LOWORD(lParam);//WM_MBUTTONUP,lParam�����ĵ�λ�����Ŀͻ���x���꣬��λ�ǿͻ���y����
		pt.y = HIWORD(lParam);
		ClientToScreen(hwnd, &pt);//���ͻ�������ת��Ϊ��Ļ����
		TrackPopupMenu(menu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, nullptr);//�����˵�
		//TPM_RIGHTBUTTON�������ô˱�־���û���������Ҽ�ѡ��˵���
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		switch (wmId)
		{
		case ID_FILE_NEW:
		case ID_FILE_OPEN:
		case ID_FILE_SAVE:
		case ID_FILE_SAVEAS:
			MessageBeep(0);
			break;
		case ID_APP_EXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case ID_EDIT_COPY:
		case ID_EDIT_CUT:
		case ID_EDIT_DELETE:
		case ID_EDIT_PASTE:
		case ID_EDIT_UNDO:
			MessageBeep(0);
			break;
		case ID_BKG_BLACK:
		case ID_BKG_DARKGRAY:
		case ID_BKG_GRAY:
		case ID_BKG_LIGHTGRAY:
		case ID_BKG_WHITE:
			CheckMenuItem(menu, is_Selected, MF_UNCHECKED);//ȡ��ԭ�ȵ�����״̬
			is_Selected = wmId;
			CheckMenuItem(menu, is_Selected, MF_CHECKED);//��ѡ��ǰѡ��
			SetClassLong(hwnd, GCL_HBRBACKGROUND, (long)GetStockObject(idColor[wmId - ID_BKG_WHITE]));//����ѡ����ɫΪ����ɫ
			InvalidateRect(hwnd, nullptr, true);//�ػ�
			break;
		case ID_TIMER_BEGIN:
			EnableMenuItem(menu, ID_TIMER_BEGIN, MF_GRAYED);//�ÿ�ʼ�˵�������
			EnableMenuItem(menu, ID_TIMER_END, MF_ENABLED);//�ý�˵�����
			SetTimer(hwnd, 1, 1000, nullptr);//��ʼ��ʱ
			break;
		case ID_TIMER_END:
			EnableMenuItem(menu, ID_TIMER_BEGIN, MF_ENABLED);//�ÿ�ʼ�˵�����
			EnableMenuItem(menu, ID_TIMER_END, MF_GRAYED);//�ý�˵�������
			KillTimer(hwnd, 1);//�رռ�ʱ
			break;
		case ID_APP_HELP:
			MessageBox(nullptr, TEXT("Help not yet implement"),TEXT("help and support"),MB_OK);
			break;
		case ID_APP_ABOUT:
			MessageBox(nullptr, TEXT("Menu Program\n(c)yong hua by,2015"), TEXT("����Menu"), MB_ICONINFORMATION | MB_OK);
		}
		break;
	case WM_TIMER://��ʱ����
		MessageBeep(0);//����һ��
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		for (int i = 0; i < cxClient; i += cxIcon)//�ڿͻ�����ͼ��
		{
			for (int j = 0; j < cyClient; j += cyIcon)
				DrawIcon(hdc, i, j, icon);
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
	wndclass.hIcon = LoadIcon(hInstance, TEXT("IDI_ICON"));//ͼ��
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