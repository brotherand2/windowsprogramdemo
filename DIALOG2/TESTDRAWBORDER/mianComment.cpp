//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<commctrl.h>

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
int DialogProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)
{
	int wmId = LOWORD(wParam);//�ؼ�ID
	int wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
	switch (message)
	{
		//case WM_COMMAND:

		//	return false;
	case WM_CLOSE:
		EndDialog(hDlg, 0);//���ٶԻ���
		return false;
	}
	return false;//���û�д�������0,�����Ĭ�϶Ի�����̣���0�����ˣ�������Ĭ�ϵ�
}
void DrawBorder(HWND hwnd)
{
	int bx, by;//�߿�ĺ����ꡢ������
	POINT pt{ 0, 0 };
	RECT rect, rect1;
	HRGN hRgn, hRgn1, hRgn2;
	HDC hdc;
	HBRUSH hBlueBrush = CreateSolidBrush(RGB(102, 179, 255));//�༭��ĵ���ɫ�߿��õĻ�ˢ
	hdc = GetWindowDC(hwnd);
	GetWindowRect(hwnd, &rect);//��ô��ھ��ε���Ļ���� 
	ClientToScreen(hwnd, &pt);//���ͻ�������ת������Ļ����

	bx = pt.x - rect.left;//������ͻ��������ꡢ������ľ���
	by = pt.y - rect.top;
	GetClientRect(hwnd, &rect);
	rect.bottom -=26;
	OffsetRect(&rect, bx, by);//���ͻ�������ת���ɴ�������
	hRgn2 = CreateRectRgnIndirect(&rect);
	InflateRect(&rect, -1, -1);//�����ꡢ����������2������
	hRgn1 = CreateRectRgnIndirect(&rect);
	hRgn = CreateRectRgn(0, 0, 1, 1);
	CombineRgn(hRgn, hRgn2, hRgn1, RGN_XOR);
	InvalidateRgn(hwnd, hRgn, true);
	ValidateRgn(hwnd, hRgn);
	FrameRgn(hdc, hRgn, hBlueBrush,1, 1);
	ReleaseDC(hwnd, hdc);
	DeleteObject(hBlueBrush);//ɾ���ɻ�ˢ
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static HWND hEdit;//�༭��
	static HWND hStatus;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL | ES_MULTILINE | ES_NOHIDESEL, 0, 0, 0, 0,
			hwnd, (HMENU)1, hInst, nullptr);//�����б߽�Ķ��б༭��ES_NOHIDESEL�ڱ༭��ʧȥ�����ѡ�е��ı���Ȼ���ָ�����ʾ
		hStatus = CreateWindow(STATUSCLASSNAME, 0, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, hwnd, (HMENU)2, hInst, nullptr);

		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		MoveWindow(hEdit, 2, 2, cxClient-4, cyClient-29, true);//�����༭��СΪ�����ͻ���
		MoveWindow(hStatus, 0, 0, 0,0, true);
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����

		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���

		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		DrawBorder(hwnd);
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