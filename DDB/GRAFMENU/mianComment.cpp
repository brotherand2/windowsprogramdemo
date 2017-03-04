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
const int ID_HELP = 1;
const int ID_FILE = 2;
const int ID_FONTCOUR = 3;
const int ID_FONTARI = 4;
const int ID_FONTTIME = 5;

HBITMAP StretchBitMap(const HBITMAP &bitmap1)
{
	BITMAP bm1, bm2;
	HDC hdcMem1, hdcMem2;
	HDC hdc = CreateIC(TEXT("DISPLAY"), 0, 0, 0);
	int cxChar =LOWORD( GetDialogBaseUnits());
	int cyChar =HIWORD(GetDialogBaseUnits());
	hdcMem1 = CreateCompatibleDC(hdc);
	hdcMem2 = CreateCompatibleDC(hdc);
	GetObject(bitmap1, sizeof(BITMAP), &bm1);
	bm2 = bm1;
	bm2.bmWidth *= 2;
	bm2.bmHeight *=2;
	bm2.bmWidthBytes = ((bm2.bmWidth + 15) / 16) * 2;
	HBITMAP bitmap2 = CreateBitmapIndirect(&bm2);
	SelectObject(hdcMem1, bitmap1);
	SelectObject(hdcMem2, bitmap2);
	StretchBlt(hdcMem2, 0, 0, bm2.bmWidth, bm2.bmHeight, hdcMem1, 0, 0, bm1.bmWidth, bm1.bmHeight, SRCCOPY);
	DeleteDC(hdc);
	DeleteDC(hdcMem1);
	DeleteDC(hdcMem2);
	return bitmap2;
}
void AddHelpToSys(HINSTANCE hInst,HWND hwnd)
{
	HMENU hMenu = GetSystemMenu(hwnd, false);
	AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	HBITMAP bitmap1 = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1));//TEXT("IDB_BITMAPMY")
	HBITMAP bitmap2 = StretchBitMap(bitmap1);
	AppendMenu(hMenu, MF_BITMAP, ID_HELP,(PTSTR)(long)bitmap2);
	DeleteObject(bitmap1);
}
HBITMAP CreateBitmapFont(int i)
{
	static TCHAR *szFaceName[]{TEXT("Courier New"), TEXT("Arial"), TEXT("Times New Roman")};
	HBITMAP hBitmap;
	HDC hdc = CreateIC(TEXT("DISPLAY"), 0, 0, 0);
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HFONT hFont = CreateFont(2*tm.tmHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, szFaceName[i]);
	SIZE size;
	HFONT oldFont=(HFONT) SelectObject(hdcMem, hFont);
	GetTextExtentPoint32(hdcMem, szFaceName[i], lstrlen(szFaceName[i]), &size);
	//hBitmap = CreateCompatibleBitmap(hdc, size.cx, size.cy);
	hBitmap = CreateBitmap(size.cx, size.cy, 1, 1, 0);
	SelectObject(hdcMem, hBitmap);
	TextOut(hdcMem, 0, 0, szFaceName[i], lstrlen(szFaceName[i]));
	DeleteObject(SelectObject(hdcMem, oldFont));
	DeleteDC(hdc);
	DeleteDC(hdcMem);
	DeleteObject(hFont);
	return hBitmap;
}
HMENU CreateMyMenu(HINSTANCE hInst)
{
	HMENU hMenu = CreateMenu();
	HMENU hMenuPop;
	HBITMAP hBitmap1, hBitmap2;
	hBitmap1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
	hBitmap2 = StretchBitMap(hBitmap1);
	DeleteObject(hBitmap1);
    hMenuPop = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
	AppendMenu(hMenu, MF_BITMAP | MF_POPUP,(int)hMenuPop, (TCHAR *)hBitmap2);
	hBitmap1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	hBitmap2 = StretchBitMap(hBitmap1);
	DeleteObject(hBitmap1);
	hMenuPop = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU2));
	AppendMenu(hMenu, MF_BITMAP | MF_POPUP, (int)hMenuPop, (TCHAR *)hBitmap2);
	hMenuPop = CreateMenu();
	for (int i = 0; i < 3; i++)
	{
		hBitmap1 = CreateBitmapFont( i);
		AppendMenu(hMenuPop, MF_BITMAP, ID_FONTCOUR + i, (TCHAR *)hBitmap1);
	}
	hBitmap1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
	hBitmap2 = StretchBitMap(hBitmap1);
	DeleteObject(hBitmap1);
	AppendMenu(hMenu, MF_BITMAP | MF_POPUP, (int)hMenuPop, (TCHAR *)hBitmap2);

	return hMenu;
}
void DeleteAllBitmaps(HWND hwnd)
{
	HMENU hMenu;
	int i;
	MENUITEMINFO mi{ sizeof(MENUITEMINFO), MIIM_SUBMENU|MIIM_TYPE};
	hMenu = GetSystemMenu(hwnd, false);
	GetMenuItemInfo(hMenu, ID_HELP, false, &mi);//false��ָ��ID_HELP��ID������λ��
	DeleteObject((HBITMAP)mi.dwTypeData);

	hMenu= GetMenu(hwnd);
	for (int i = 0; i < 3; i++)
	{
		GetMenuItemInfo(hMenu,ID_FONTCOUR+i, false, &mi);
		DeleteObject((HBITMAP)mi.dwTypeData);
	}

	for (int i = 0; i < 3; i++)
	{
		GetMenuItemInfo(hMenu, i, true, &mi);
		DeleteObject((HBITMAP)mi.dwTypeData);
	}

}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc,hdcMem;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static HBITMAP bitmap1;
	static HMENU hMenu;
	static int idCurFont = ID_FONTCOUR;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		AddHelpToSys(hInst, hwnd);
		hMenu = CreateMyMenu(hInst);
		SetMenu(hwnd, hMenu);
		CheckMenuItem(hMenu, idCurFont, MF_CHECKED);
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
		case ID_FONTARI:
		case ID_FONTCOUR:
		case ID_FONTTIME:
			CheckMenuItem(hMenu, idCurFont, MF_UNCHECKED);
			idCurFont = wmId;
			CheckMenuItem(hMenu, idCurFont, MF_CHECKED);
			break;
		}
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		bitmap1= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, bitmap1);
		BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
		DeleteDC(hdcMem);
		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������ʱ
		DeleteObject(bitmap1);
		DeleteAllBitmaps(hwnd);
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