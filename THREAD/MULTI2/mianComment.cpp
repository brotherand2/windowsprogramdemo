//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<process.h>
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
const int ID_TIMER = 1;
int cyChar;
typedef struct
{
	int cxClinet;
	int cyClient;
	HWND hwnd;
	int cyChar;
	bool bKill;
}PARAMS;
void CheckBottom(HWND hwnd, int cyClient, int &iLine)
{
	if (iLine*cyChar + cyChar > cyClient)
	{
		iLine = 0;
		InvalidateRect(hwnd, 0, true);
	}
}
void Thread1(PVOID pvoid)
{
	HDC hdc;
	static int iNum = 0, iLine = -1;
	TCHAR szBuffer[32];
	PARAMS *param = (PARAMS*)pvoid;
	while (!param->bKill)
	{
		hdc = GetDC(param->hwnd);
		iLine++;
		iNum++;
		if (iNum < 0)
			iNum = 0;
		CheckBottom(param->hwnd, param->cyClient, iLine);
		TextOut(hdc, 0, iLine*cyChar, szBuffer, wsprintf(szBuffer, TEXT("%d"), iNum));
		ReleaseDC(param->hwnd, hdc);
	}
	_endthread();
}
void Thread2(PVOID pvoid)
{
	HDC hdc;
	static int iNum = 1, iLine = -1;
	TCHAR  szBuffer[32];
	int iSqrt;
	int i = 0;

	PARAMS *param = (PARAMS*)pvoid;
	while (!param->bKill)
	{
		hdc = GetDC(param->hwnd);
		iLine++;
		CheckBottom(param->hwnd, param->cyClient, iLine);
		if (iNum < 0)
			iNum = 0;
		do
		{
			iNum++;
			iSqrt = (int)sqrt(iNum);
			for (i = 2; i <= iSqrt; i++)
				if (iNum%i == 0)
					break;
		} while (i <= iSqrt);
		TextOut(hdc, 0, iLine*cyChar, szBuffer, wsprintf(szBuffer, TEXT("%d"), iNum));
		ReleaseDC(param->hwnd, hdc);

	}
	_endthread();
}
void Thread3(PVOID pvoid)
{
	HDC hdc;
	static int iNum = 0, next = 1, temp, iLine = -1;
	TCHAR  szBuffer[32];

	PARAMS *param = (PARAMS*)pvoid;
	while (!param->bKill)
	{
		hdc = GetDC(param->hwnd);
		iLine++;
		if (iNum < 0)
		{
			iNum = 0;
			next = 1;
		}
		CheckBottom(param->hwnd, param->cyClient, iLine);
		TextOut(hdc, 0, iLine*cyChar, szBuffer, wsprintf(szBuffer, TEXT("%d"), iNum));
		temp = next;
		next = iNum + next;
		iNum = temp;
		ReleaseDC(param->hwnd, hdc);

	}
	_endthread();

}
void Thread4(PVOID pvoid)
{
	HDC hdc;
	static int iNum = 0, iLine = 0;

	PARAMS *param = (PARAMS*)pvoid;
	while (!param->bKill)
	{
		hdc = GetDC(param->hwnd);
		InvalidateRect(param->hwnd, 0, true);
		UpdateWindow(param->hwnd);
		iNum = rand() % (max(1, min(param->cxClinet, param->cyClient)));
		Ellipse(hdc, (param->cxClinet - iNum) / 2, (param->cyClient - iNum) / 2, (param->cxClinet + iNum) / 2, (param->cyClient + iNum) / 2);
		ReleaseDC(param->hwnd, hdc);

	}
	_endthread();

}

long _stdcall Child1Proc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	static int cyClient,cxClient;
	static PARAMS param;
	switch (UMsg)
	{
	case WM_CREATE:
		param.bKill = false;
		param.hwnd = hwnd;
		param.cyChar =HIWORD( GetDialogBaseUnits());
		_beginthread(Thread1, 0, &param);
	case WM_SIZE:
		param.cxClinet = LOWORD(lParam);
		param.cyClient = HIWORD(lParam);
		break;
	case WM_TIMER:
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, UMsg, wParam, lParam);//��Ĭ�ϵĴ��ڹ��̴���������Ϣ����Щ��������
}
long _stdcall Child2Proc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	static int cyClient, cxClient;
	static PARAMS param;
	switch (UMsg)
	{
	case WM_CREATE:
		param.bKill = false;
		param.hwnd = hwnd;
		param.cyChar = HIWORD(GetDialogBaseUnits());
		_beginthread(Thread2, 0, &param);
	case WM_SIZE:
		param.cxClinet = LOWORD(lParam);
		param.cyClient = HIWORD(lParam);
		break;
	case WM_TIMER:
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//��Ĭ�ϵĴ��ڹ��̴���������Ϣ����Щ��������
}
long _stdcall Child3Proc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	static int cyClient, cxClient;
	static PARAMS param;
	switch (UMsg)
	{
	case WM_CREATE:
		param.bKill = false;
		param.hwnd = hwnd;
		param.cyChar = HIWORD(GetDialogBaseUnits());
		_beginthread(Thread3, 0, &param);
	case WM_SIZE:
		param.cxClinet = LOWORD(lParam);
		param.cyClient = HIWORD(lParam);
		break;
	case WM_TIMER:
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//��Ĭ�ϵĴ��ڹ��̴���������Ϣ����Щ��������
}
long _stdcall Child4Proc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	static int cyClient, cxClient;
	static PARAMS param;
	switch (UMsg)
	{
	case WM_CREATE:
		param.bKill = false;
		param.hwnd = hwnd;
		param.cyChar = HIWORD(GetDialogBaseUnits());
		_beginthread(Thread4, 0, &param);
	case WM_SIZE:
		param.cxClinet = LOWORD(lParam);
		param.cyClient = HIWORD(lParam);
		break;
	case WM_TIMER:
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//��Ĭ�ϵĴ��ڹ��̴���������Ϣ����Щ��������
}

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static HWND hwndChild[4];
	WNDPROC proc[4]{Child1Proc,Child2Proc,Child3Proc,Child4Proc };
	static TCHAR *szClass[] = {TEXT("child1"),TEXT("child2"),TEXT("child3"),TEXT("child4")};
	WNDCLASS wndclass;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.hCursor = 0;
		wndclass.hIcon = 0;
		wndclass.hInstance = hInst;
		wndclass.lpszMenuName = 0;
		wndclass.style = CS_VREDRAW | CS_HREDRAW;
		for (int i = 0; i < 4; i++)
		{
			wndclass.lpfnWndProc = proc[i];
			wndclass.lpszClassName = szClass[i];
			RegisterClass(&wndclass);
			hwndChild[i] = CreateWindow(szClass[i], 0, WS_CHILDWINDOW|WS_BORDER|WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)i, hInst, 0);
		}
		break;
	case WM_CHAR:
		if (wParam == '\x1B')
			DestroyWindow(hwnd);
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		for (int i = 0; i < 4; i++)
			MoveWindow(hwndChild[i], (i % 2) ? cxClient / 2 :0 , i <2 ? 0 : cyClient / 2, cxClient / 2, cyClient / 2,true);
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
		KillTimer(hwnd, ID_TIMER);
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