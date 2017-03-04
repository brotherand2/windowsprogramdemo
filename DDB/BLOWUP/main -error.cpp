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
static int width, heigh;//��ͼ�Ŀ���
static RECT clipRect;//��������ľ���
void GetDestopBK(HWND hDestop, HBITMAP &hBitmap, int cxScreen, int cyScreen)//��ô��ڱ�����ѡ��λͼ
{
	HDC hdcMem = CreateCompatibleDC(0);//�������Ļ���ݵĻ���
	HDC  hdcScr = GetDCEx(hDestop, 0, DCX_LOCKWINDOWUPDATE);//�����Ļ�Ļ���,��ʹ��Ļ��סҲ��ˢ��
	hBitmap = CreateCompatibleBitmap(hdcScr, cxScreen, cyScreen);//��������Ļ��С���ݵ�λͼ
	SelectObject(hdcMem, hBitmap);//��λͼѡ�뻭��
	BitBlt(hdcMem, 0, 0, cxScreen, cyScreen, hdcScr, 0, 0, SRCCOPY);//����Ļ��λͼ�����������ϵ�λͼ
	DeleteDC(hdcMem);
	ReleaseDC(hDestop, hdcScr);
}

void ShowFullScreen(HWND hwnd, int cxScreen, int cyScreen)//�ô���ȫ��
{
	SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);//���ô�����ʽΪ����ʽ���ڣ�����ȥ��ϵͳ�˵������������߿�ȣ�Ҳ����WS_BORDER��ʽ
	SetMenu(hwnd, 0);//ȥ���˵�
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, cxScreen, cyScreen, SWP_SHOWWINDOW);//�ô�����ʾȫ�����ö�
	clipRect.left = clipRect.top = 0;//����Ļ��ʾ�����Ͻ�
	width = cxScreen;//��ʾ�Ŀ�ȡ��߶�
	heigh = cyScreen;
}
void RecoveryWindow(HWND hwnd, RECT windowRect, HMENU hMenu)//�ָ�����ԭ���Ĵ�Сλ��
{
	SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);//�ָ�������ʽ
	SetMenu(hwnd, hMenu);//��Ӳ˵�
	SetWindowPos(hwnd, HWND_NOTOPMOST, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, SWP_SHOWWINDOW);//�ָ�ԭ��λ��
}
HBITMAP CopyBitmap(const HBITMAP &hBitmap)//λͼ����
{
	HBITMAP copyBitmap;
	HDC hdcSrc = CreateCompatibleDC(0);//����ڴ滭��
	HDC hdcDst = CreateCompatibleDC(0);//����ڴ滭��
	SelectObject(hdcSrc, hBitmap);//��λͼѡ���ڴ滭��
	BITMAP bm;//λͼ�ṹ
	GetObject(hBitmap, sizeof(BITMAP), &bm);//���Ҫ������λͼ��Ϣ
	copyBitmap = CreateCompatibleBitmap(hdcSrc, bm.bmWidth, bm.bmHeight);//����Ҫ������λͼ
	SelectObject(hdcDst, copyBitmap);//��λͼѡ���ڴ滭��
	BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);//��λͼ��������һ��λͼ
	DeleteObject(hdcSrc);
	DeleteObject(hdcDst);
	return copyBitmap;
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	static HDC hdc;//����
	static HDC hdcMem;//�ڴ滭��
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static POINT ptBeg, ptEnd;//��ͼ��㡢�յ�
	static bool  bBlock = false;//�Ƿ�ʼ�����ο�
	static bool bNewBlcok = false;//�Ƿ�ʼ��ͼ
	static HWND hDestop;//������
	static HBITMAP hBitmap;//λͼ���
	static HBITMAP copyBitmap;//������λͼ�������и��ơ�����ʱ������ͼ����һ�ݵ�������
	static HBITMAP clipBitmap;//�Ӽ������л�õ�λͼ���
	BITMAP bitmap;//λͼ�ṹ
	static int cxScreen, cyScreen;//��Ļ��ȡ��߶�
	static RECT windowRect;//���ڵ����꣬������ȫ��ʱ����ԭ����
	static HBRUSH redBrush;//��ɫ��ˢ������ͼ���εı߿����ɫ
	static HMENU hMenu;//���ڲ˵�
	HANDLE handle;//�Ӽ����巵�صľ��
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		cxScreen = GetSystemMetrics(SM_CXSCREEN);////��Ļ��ȡ��߶�
		cyScreen = GetSystemMetrics(SM_CYSCREEN);
		redBrush = CreateSolidBrush(RGB(255, 0, 0));//��ɫ��ˢ������ͼ���εı߿����ɫ
		hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));//���ز˵�
		SetMenu(hwnd, hMenu);//���ò˵�
		hDestop = GetDesktopWindow();//������
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		if (bNewBlcok&&hBitmap)//�����ʼ��ͼ����λͼ��Ϊ��
		{
			bBlock = true;//��ʼ�����ο�
			SetCapture(hwnd);//��׽���
			SetCursor(LoadCursor(0, IDC_CROSS));//�������Ϊʮ�ּ�ͷ����ʾ�ڽ�ͼ��
			ptBeg.x = LOWORD(lParam);//��ý�ͼ����ʼ����
			ptBeg.y = HIWORD(lParam);
			ptEnd = ptBeg;
			hdc = GetDC(hwnd);//��ô��ڻ���
			hdcMem = CreateCompatibleDC(hdc);//��õĻ���
			SelectObject(hdcMem, hBitmap);//����Ļλͼѡ�뻭��
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE://��ESC���˳�ȫ��
			RecoveryWindow(hwnd, windowRect, hMenu);
			break;
		case VK_SPACE://���ո����ȫ��
			GetWindowRect(hwnd, &windowRect);
			GetDestopBK(hDestop, hBitmap, cxScreen, cyScreen);//�õ�����λͼ
			ShowFullScreen(hwnd, cxScreen, cyScreen);//ȫ����ʾ
			InvalidateRect(hwnd, 0, true);//�ڴ��ڻ������汳��
			break;
		}
		break;
	case WM_MOUSEMOVE:
		if (bBlock)//����Ѿ�����������
		{
			BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY);//����ǰ���ľ��β���	
			ptEnd.x = (short)LOWORD(lParam);//�յ�����
			ptEnd.y = (short)HIWORD(lParam);
			if (ptBeg.x < ptEnd.x)//��ģ������С����Ϊ���ε����
			{
				clipRect.left = ptBeg.x;
				clipRect.right = ptEnd.x;
			}
			else
			{
				clipRect.left = ptEnd.x;
				clipRect.right = ptBeg.x;
			}
			if (ptBeg.y < ptEnd.y)//��ģ������С����Ϊ���ε��յ�
			{
				clipRect.top = ptBeg.y;
				clipRect.bottom = ptEnd.y;
			}
			else
			{
				clipRect.top = ptEnd.y;
				clipRect.bottom = ptBeg.y;
			}
			FrameRect(hdc, &clipRect, redBrush);//�ú�ɫ�߿򻭳���������
		}
		break;
	case WM_RBUTTONDOWN://����Ҽ���ȡ�����ν�ͼ���شν�ͼ
		bBlock = false;//��δ�����ο�
		BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY);//����ǰ���ľ��β���	
		SetCursor(LoadCursor(0, IDC_ARROW));//�ָ�����ͷ
		ReleaseDC(hwnd, hdc);
		DeleteDC(hdcMem);
		break;
	case WM_LBUTTONUP:
		if (bBlock)
		{
			bBlock = bNewBlcok = false;//��ͼ����
			width = abs(ptEnd.x - ptBeg.x);//��ͼ�Ŀ�ȣ��߶�
			heigh = abs(ptEnd.y - ptBeg.y);
			RecoveryWindow(hwnd, windowRect, hMenu);//�ָ�ԭ���Ĵ���
			BitBlt(hdc, 0, 0, width, heigh, hdcMem, clipRect.left, clipRect.top, SRCCOPY);//����ͼ�����ڴ���
			SetCursor(LoadCursor(0, IDC_ARROW));//�ָ�����ͷ
			ReleaseCapture();//�ͷ����Ĳ�׽
			ReleaseDC(hwnd, hdc);
			DeleteDC(hdcMem);
			InvalidateRect(hwnd, 0, true);//�ڴ���������ʾ��ͼ
		}
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		switch (wmId)
		{
		case IDM_EDIT_COPY:
		case IDM_EDIT_CUT:
			if (!hBitmap)
				break;//û��Ҫ������λͼ
			copyBitmap = CopyBitmap(hBitmap);//����λͼ
			OpenClipboard(hwnd);//�򿪼�����
			EmptyClipboard();//�����������
			SetClipboardData(CF_BITMAP, copyBitmap);//��λͼ����ŵ�������
			CloseClipboard();//�رռ�����
			if (wmId == IDM_EDIT_COPY)//����ǿ����Ļ����˳�switch������fall through ���������մ��ڵ�����
				break;
		case IDM_EDIT_DELETE:
			if (hBitmap)//���λͼ�����Ϊ��
			{
				DeleteObject(hBitmap);//����λͼ
				hBitmap = 0;
				InvalidateRect(hwnd, 0, true);//������ʾ����
			}
			break;
		case IDM_EIDT_PASTE:
			if (hBitmap)//���λͼ��Ϊ��
			{
				DeleteObject(hBitmap);//ɾ��
				hBitmap = 0;
			}
			OpenClipboard(hwnd);//�򿪼�����
			clipBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);//�����彫λͼ���
			if (clipBitmap)//�������������λͼ���
			{
				hBitmap = CopyBitmap(clipBitmap);//����λͼ
				BITMAP bm;//λͼ�ṹ
				GetObject(hBitmap, sizeof(BITMAP), &bm);//���Ҫճ����λͼ��Ϣ
				width = bm.bmWidth;//��ü�������λͼ�Ŀ�ȸ߶�
				heigh = bm.bmHeight;
				clipRect.left = clipRect.top = 0;//�������Ϊλͼ�����Ͻ�
			}
			CloseClipboard();//�رռ�����
			InvalidateRect(hwnd, 0, true);//������ʾ����
			break;
		case IDM_FILENEW:
			bNewBlcok = true;//��ʼ��ͼ
			if (hBitmap)//���λͼ��Ϊ��
			{
				DeleteObject(hBitmap);//����ǰλͼɾ��
				hBitmap = 0;
			}
			GetDestopBK(hDestop, hBitmap, cxScreen, cyScreen);//�õ�����λͼ
			GetWindowRect(hwnd, &windowRect);//��ô������꣬�ڽ���ͼ��ָ�
			ShowFullScreen(hwnd, cxScreen, cyScreen);//�ô���ȫ��
			InvalidateRect(hwnd, 0, true);//��ʾ������Ļ��ͼ
		}
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		if (hBitmap)
		{
			hdcMem = CreateCompatibleDC(0);//�������Ļ���ݵĻ���
			SelectObject(hdcMem, hBitmap);//��λͼѡ�뻭��
			BitBlt(hdc, 0, 0, width, heigh, hdcMem, clipRect.left, clipRect.top, SRCCOPY);//�ڴ�������ʾ��ͼ
			DeleteObject(hdcMem);
		}
		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������ʱ
		if (hBitmap)
			DeleteObject(hBitmap);//ɾ��λͼ
		DeleteObject(redBrush);//ɾ����ˢ
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
	wndclass.lpszMenuName = 0;//�˵�
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
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (GetMessage(&msg, nullptr, 0, 0))//����Ϣ���л�ȡ��Ϣ
	{
		if (!TranslateAccelerator(hwnd, hAccel, &msg))
		{
			TranslateMessage(&msg);//��һЩ������Ϣת�����ַ���Ϣ���ڰ�����Ϣ����
			DispatchMessage(&msg);//�ַ���Ϣ�������ô��ڹ��̽��д�����Ϣ
		}
	}
}