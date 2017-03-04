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

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	TCHAR p[] = TEXT("If you want something you've never had, then you've got to do something you've never Done.\nWe Are Responsible for Our Life. And nobody else. Although all success requires the assistance and cooperation of others, our success can never be left to anyone else. Luck is not a strategy.");
	TCHAR pt[] =TEXT("If\tyou\twant\tsomething\tyou've\tnever\thad,\tthen\tyou've got to do something you've never Done.\nWe Are Responsible for Our Life. And nobody else. Although all success requires the assistance and cooperation of others, our success can never be left to anyone else. Luck is not a strategy.");
	TCHAR p2[] = TEXT("Hello world");
	RECT   rect;
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
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����

		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		SetRect(&rect, 10, 10, 310, 210);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		//DrawText(hdc, p2, -1, &rect,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		DrawText(hdc, pt, -1, &rect, DT_WORDBREAK|DT_EXPANDTABS);
		//DrawText(hdc, pt, -1, &rect, DT_NOCLIP);
		SetRect(&rect, 320,10,320+300,210);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		DrawText(hdc, p, -1,&rect,DT_WORDBREAK|DT_RIGHT);

		SetRect(&rect,10,230,10+300,230+200);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

		DrawText(hdc, p, -1,&rect,DT_WORDBREAK|DT_CENTER);
		SetRect(&rect,320,230,320+300,230+40);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

		DrawText(hdc, p, -1,&rect,DT_LEFT|DT_NOCLIP);
		SetRect(&rect,320,280,320+300,280+140);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		DrawText(hdc, p2, -1,&rect,DT_SINGLELINE|DT_LEFT|DT_TOP);
		DrawText(hdc, p2, -1,&rect,DT_SINGLELINE|DT_LEFT|DT_BOTTOM);
		DrawText(hdc, p2, -1,&rect,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		DrawText(hdc, p2, -1,&rect,DT_SINGLELINE|DT_RIGHT|DT_TOP);
		DrawText(hdc, p2, -1,&rect,DT_SINGLELINE|DT_RIGHT|DT_BOTTOM);
		DrawText(hdc, p2, -1,&rect,DT_SINGLELINE|DT_RIGHT|DT_VCENTER);
		DrawText(hdc, p2, -1,&rect,DT_SINGLELINE|DT_CENTER|DT_TOP);
		DrawText(hdc, p2, -1,&rect,DT_SINGLELINE|DT_CENTER|DT_BOTTOM);
		DrawText(hdc, p2, -1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);

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