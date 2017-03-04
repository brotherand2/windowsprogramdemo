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
int iCurColor = IDC_WHITE;//��ǰѡ�е���ɫID
int iCurShape = IDC_RECT;////ѡ�е���״ID
void PaintWindow(HWND hwnd, int id, int shape)//����ָ������״����ɫ�ڿͻ�����
{
	static COLORREF crColor[8] =
	{
		RGB(0, 0, 0),//��ɫ
		RGB(0, 0, 255),//��ɫ
		RGB(0, 255, 0),//��ɫ
		RGB(0, 255, 255),//��ɫ
		RGB(255, 0, 0),//��ɫ
		RGB(255, 0, 255),//�ۺ�ɫ
		RGB(255, 255, 0),//��ɫ
		RGB(255, 255, 255)//��ɫ
	};
	HDC hdc = GetDC(hwnd);
	RECT rect;
	HBRUSH hBrush = CreateSolidBrush(crColor[id - IDC_BLACK]);//���ָ����ɫ�Ļ�ˢ��IDC_BLACK��8����ɫID����С
	GetClientRect(hwnd, &rect);//��ÿͻ��Ĵ�С
	HRGN rgn;
	if (shape == IDC_RECT)//�����״�Ǿ���
		rgn = CreateRectRgnIndirect(&rect);//������������
	else
		rgn = CreateEllipticRgnIndirect(&rect);//������Բ������
	FillRgn(hdc, rgn, hBrush);//��ָ���Ļ�ˢ��ָ������
	DeleteObject(hBrush);
	DeleteObject(rgn);
	ReleaseDC(hwnd, hdc);
}
void PaintTheBlock(HWND hPaint, int id, int shape)//����ָ������״����ɫ���Ӵ��ڿͻ�����
{
	InvalidateRect(hPaint, nullptr, true);//���Ӵ��������ͻ���ʧЧ�����ñ���ɫ����֮ǰ����ͼ��
	UpdateWindow(hPaint);//�����Ӵ��ڣ����Ӵ��ڿͻ�����Ч�����������Ի���ʧЧ�ػ��ʱ���Ӵ�����Ϊ�ͻ�����Ч�����ᷢ���ػ棬���������ᱻ����
	PaintWindow(hPaint, id, shape);//����ָ������״����ɫ���Ӵ��ڿͻ�����
}
int _stdcall DialogProc(HWND hDlg, unsigned int message, unsigned int wParam, long lParam)
{
	static int SelectedColorID=iCurColor;//ѡ�е���ɫID
    static int SelectedShapeID=iCurShape;//ѡ�е���״ID
	static HWND hPaint;//�Ի����е�һ���Ӵ��ڣ����������ָ����ɫ��ָ����״
	switch (message)
	{
		case WM_INITDIALOG://�����Ի���ĳ�ʼ����Ϣ
			SendMessage(GetDlgItem(hDlg,iCurColor), BM_SETCHECK, 1, 0);//ѡ��Ĭ�ϵ�ID
			SendMessage(GetDlgItem(hDlg,iCurShape), BM_SETCHECK, 1, 0);//ѡ��Ĭ�ϵ�ID
			hPaint = GetDlgItem(hDlg, IDC_PAINT);//����Ӵ��ھ��
			return true;
		case WM_PAINT:
			PaintTheBlock(hPaint, iCurColor, iCurShape);
			return false;//����false�����ϵͳ�ĶԻ�����̣��ػ����������簴ť��ͼ��
		case WM_COMMAND://�Ի�����ؼ��Ĳ���
		{
			int wmId = LOWORD(wParam);//�ؼ�ID
			int wmEvent = HIWORD(wParam);//�ؼ��ϵĲ���
			switch (wmId)
			{
			case IDOK:
				iCurColor = SelectedColorID;//�����µ���ɫ����״
				iCurShape = SelectedShapeID;
				InvalidateRect(hPaint, nullptr, true);//�öԻ����Ӵ����ػ�
				InvalidateRect(GetParent(hDlg), nullptr, true);//�ÿͻ����ػ�
				EndDialog(hDlg, 0);//�ͷŶԻ�����Դ
				return true;
			case IDCANCEL:
				EndDialog(hDlg, 0);//�ͷŶԻ�����Դ
				return true;
			case IDC_BLACK://ѡ ����ɫ��ť
			case IDC_BLUE:
			case IDC_CYAN:
			case IDC_YELLOW:
			case IDC_RED:
			case IDC_WHITE:
			case IDC_GREEN:
			case IDC_PINK:
				//if (wmId <= IDC_WHITE&&wmId >= IDC_BLACK)//IDC_BALCK��IDC_WHITE��8����ɫID�����������ģ��������ɫ��ť
				//{
				//	SendMessage(GetDlgItem(hDlg, SelectedID), BM_SETCHECK, 0, 0);//�����ԭѡ�е�ѡ��ť��ȡ��
				//    SendMessage(GetDlgItem(hDlg, wmId), BM_SETCHECK, 1, 0);//�����ѡ���µ�ѡ��ť
				//	SelectedID = wmId;//���õ�ǰ��ѡ��ť��ID
				//}
				//SendMessage(GetDlgItem(hDlg, wmId), BM_SETCHECK, 1, 0);//�����ѡ�е�ѡ��ť
				CheckRadioButton(hDlg, IDC_BLACK, IDC_WHITE, wmId);//IDC_BALCK��IDC_WHITE��8����ɫID�����������ģ��������ɫ��ť,ֻѡ��idΪwmId�İ�ť������ȡ��ѡ��
				SelectedColorID = wmId;//���õ�ǰ��ѡ��ť��ID
				PaintTheBlock(hPaint, SelectedColorID,SelectedShapeID);//���Ӵ�����ʾ���Ƶ�ͼ��
				return true;
			case IDC_RECT:
			case IDC_ELLIPSE://ѡ����״��ť
				CheckRadioButton(hDlg, IDC_RECT, IDC_ELLIPSE, wmId);
				SelectedShapeID = wmId;//���õ�ǰ��ѡ��ť��ID
				PaintTheBlock(hPaint, SelectedColorID, SelectedShapeID);//���Ӵ�����ʾ���Ƶ�ͼ��
				return true;
			}
		}
	}
	return false;//���û�д�������0,�����Ĭ�϶Ի�����̣���0�����ˣ�������Ĭ�ϵ�
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
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
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MYDIALOG), hwnd, DialogProc);
			break;
		}
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���

		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		PaintWindow(hwnd, iCurColor, iCurShape);//����ָ������״����ɫ�ڿͻ�����
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
	wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));//ͼ��
	wndclass.hInstance = hInstance;//ʵ�������������PID��һ���������������������ϵͳ�����ҵ����̵ĵ�ַ
	wndclass.lpfnWndProc = WndProc;//���ڹ���
	wndclass.lpszClassName = TEXT("zeng");//����
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);//�˵�
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