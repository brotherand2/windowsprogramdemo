//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<memory>
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
const int ID_LISTBOXLEFT = 1;
const int ID_LISTBOXMIDDLE = 2;
const int ID_LISTBOXRIGHT = 3;
const int ID_MOVERIGHT1 = 4;
const int ID_MOVERIGHT2 = 5;
const int ID_ADDTHREESTRINGLEFT = 6;
const int ID_DELETESTRING = 7;
const int ID_RESET = 8;
const int ID_SETTHREE = 9;
const int ID_UNSET = 10;
const int ID_ADDTHREESTRINGRIGHT = 11;
const int ID_INSERTPOSTHREE = 12;
const int ID_SETTHREEANDSIX = 13;
const int ID_UNSETTHREEANDSIX = 14;
const int ID_SETALL = 15;

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static HWND hListBoxLeft, hListBoxMiddle, hListBoxRight;
	int index;//����
	int len;//�ַ�������
	static TCHAR *p;//�ַ���ָ��
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		hListBoxLeft = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY|LBS_SORT
			, cxChar, cyChar,18*cxChar+GetSystemMetrics(SM_CXVSCROLL), 9*cyChar, hwnd, (HMENU)ID_LISTBOXLEFT, hInst, nullptr);
		hListBoxMiddle = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY|LBS_MULTIPLESEL
			,33*cxChar, cyChar, 18 * cxChar + GetSystemMetrics(SM_CXVSCROLL), 9 * cyChar, hwnd, (HMENU)ID_LISTBOXLEFT, hInst, nullptr);
		hListBoxRight = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY
			, 65*cxChar, cyChar, 18 * cxChar + GetSystemMetrics(SM_CXVSCROLL), 9 * cyChar, hwnd, (HMENU)ID_LISTBOXLEFT, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT(">"), WS_CHILD | WS_VISIBLE,24*cxChar,4*cyChar, 6*cxChar,2*cyChar,
			hwnd, (HMENU)ID_MOVERIGHT1, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT(">"), WS_CHILD | WS_VISIBLE,56*cxChar, 4*cyChar,6*cxChar,2*cyChar,
			hwnd, (HMENU)ID_MOVERIGHT2, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("����3��"), WS_CHILD | WS_VISIBLE,cxChar,11*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_ADDTHREESTRINGLEFT, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("ɾ����3��"), WS_CHILD | WS_VISIBLE,cxChar,14*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_DELETESTRING, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("����"), WS_CHILD | WS_VISIBLE,cxChar,17*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_RESET, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("ѡ�е�3��"), WS_CHILD | WS_VISIBLE,cxChar,20*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_SETTHREE, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("ȡ��ѡ�е�"), WS_CHILD | WS_VISIBLE,cxChar,23*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_UNSET, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("����3��"), WS_CHILD | WS_VISIBLE,33*cxChar,11*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_ADDTHREESTRINGRIGHT, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("�ڵ�3��λ�ò���"), WS_CHILD | WS_VISIBLE,33*cxChar,14*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_INSERTPOSTHREE, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("ѡ�е�3�͵�6��"), WS_CHILD | WS_VISIBLE,33*cxChar,17*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_SETTHREEANDSIX, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("ȡ��ѡ�е�3�͵�6��"), WS_CHILD | WS_VISIBLE,33*cxChar,20*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_UNSETTHREEANDSIX, hInst, nullptr);
		CreateWindow(TEXT("button"), TEXT("ȫ��ѡ��"), WS_CHILD | WS_VISIBLE,33*cxChar,23*cyChar,20*cxChar,2*cyChar,
			hwnd, (HMENU)ID_SETALL, hInst, nullptr);
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
		case ID_MOVERIGHT1://���б��1��ѡ�ĵ��Ƶ��б��2
			index = SendMessage(hListBoxLeft, LB_GETCURSEL, 0, 0);
			if (index == LB_ERR)//�����ǰû��ѡ��
				return 0;
			len = SendMessage(hListBoxLeft, LB_GETTEXTLEN, index, 0);//��ȡѡ�е��ַ�������
			p = new TCHAR[len + 1];//��̬����len+1���ַ��ռ�
			{
				shared_ptr<TCHAR>sp(p, [](TCHAR *p){delete[]p;});//������Ŀռ佻����ָ�����
				SendMessage(hListBoxLeft, LB_GETTEXT, index, (long)p);//��ȡѡ�е��ַ���
				SendMessage(hListBoxMiddle, LB_ADDSTRING, -1, (long)p);//���ַ��������2 ���б��
				SendMessage(hListBoxLeft, LB_DELETESTRING, index, 0);//���б����ɾ��ѡ�е��ַ���

			}
			return 0;
		case ID_MOVERIGHT2://���б��2��ѡ�ĵ��Ƶ��б��3
		{
			int count = SendMessage(hListBoxMiddle, LB_GETCOUNT, 0, 0);//��ȡ�б��2ѡ�еĸ���
			for (int i = count - 1; i >= 0; i--)
			{
			    int isSelect = SendMessage(hListBoxMiddle, LB_GETSEL, i, 0);//�жϵ�ǰ�����Ƿ�ѡ��
				if (isSelect != 0)//��ǰ����ѡ����
				{
					len = SendMessage(hListBoxMiddle, LB_GETTEXTLEN, i, 0);//��ȡѡ���ַ����ĳ���
					p = new TCHAR[len + 1];//��̬����ռ�
					shared_ptr<TCHAR>sp(p, [](TCHAR *p){delete[]p; });//������Ŀռ佻����ָ�����
					SendMessage(hListBoxMiddle, LB_GETTEXT, i, long(p));//��ȡѡ�е��ַ���
					SendMessage(hListBoxRight, LB_ADDSTRING, 0, long(p));//����ȡ���ַ��������б�3
					SendMessage(hListBoxMiddle, LB_DELETESTRING, i, 0);//��ѡ�е��ַ���ɾ��
				}
			}
		}
			return 0;
		case ID_ADDTHREESTRINGLEFT://���б��������3���ַ���
			SendMessage(hListBoxLeft, LB_ADDSTRING, 0, (long)TEXT("hello"));
			SendMessage(hListBoxLeft, LB_ADDSTRING, 0, (long)TEXT("windows"));
			SendMessage(hListBoxLeft, LB_ADDSTRING, 0, (long)TEXT("world"));
			return 0;
		case ID_DELETESTRING://ɾ����3��
			SendMessage(hListBoxLeft, LB_DELETESTRING, 2, 0);
			return 0;
		case ID_RESET://����Ϊ��
			SendMessage(hListBoxLeft, LB_RESETCONTENT, 0, 0);
			return 0;
		case ID_SETTHREE://ѡ�е�3��
			SendMessage(hListBoxLeft, LB_SETCURSEL, 2, 0);
			return 0;
		case ID_UNSET:
			index = SendMessage(hListBoxLeft, LB_GETCURSEL, 0, 0);//���ѡ�е�����
			if (index != LB_ERR)//���ѡ���ˣ�ȡ��ѡ��
				SendMessage(hListBoxLeft, LB_SETCURSEL, -1, 0);
			return 0;
		case ID_ADDTHREESTRINGRIGHT://�ڵ�2���б�����3���ַ���
			SendMessage(hListBoxMiddle, LB_ADDSTRING, -1, long(TEXT("one")));
			SendMessage(hListBoxMiddle, LB_ADDSTRING, -1, long(TEXT("two")));
			SendMessage(hListBoxMiddle, LB_ADDSTRING, -1, long(TEXT("three")));
			return 0;
		case ID_INSERTPOSTHREE:
			SendMessage(hListBoxMiddle, LB_INSERTSTRING, 2, long(TEXT("nihao")));//�ڵ�2���б���3��λ�ò�����ַ���
			return 0;
		case ID_SETTHREEANDSIX://ѡ�е�3���͵�6���ַ���
			SendMessage(hListBoxMiddle, LB_SETSEL,1, 2);
			SendMessage(hListBoxMiddle, LB_SETSEL,1, 5);
			return 0;
		case ID_UNSETTHREEANDSIX://ȡ��ѡ�е�3���͵�6���ַ���
			SendMessage(hListBoxMiddle, LB_SETSEL,0, 2);
			SendMessage(hListBoxMiddle, LB_SETSEL,0, 5);
			return 0;
		case ID_SETALL:
			SendMessage(hListBoxMiddle, LB_SETSEL, 1, -1);
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
	wndclass.hbrBackground =CreateSolidBrush(GetSysColor(COLOR_BTNFACE));//������ˢ
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