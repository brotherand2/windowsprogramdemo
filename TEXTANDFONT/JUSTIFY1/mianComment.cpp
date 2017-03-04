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
void DrawRuler(HDC hdc, POINT pt)
{
	SaveDC(hdc);
	static int justify[16] = {360,72,144,72,216,72,144,72,288,72,144,72,216,72,144,72};
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, 1440, 1440, 0);
	SetViewportExtEx(hdc, GetDeviceCaps(hdc, LOGPIXELSX), GetDeviceCaps(hdc, LOGPIXELSY), 0);
	SetWindowOrgEx(hdc, -720, -720, 0);
	DPtoLP(hdc, &pt, 1);
	pt.x = pt.x - 360;
	MoveToEx(hdc, 0, -360, 0);
	LineTo(hdc, pt.x, -360);
	MoveToEx(hdc, -360, 0, 0);
	LineTo(hdc, -360, pt.y);
	for (int i = 0, j = 0; i < pt.x; i += 1440 / 16, j++)
	{
		MoveToEx(hdc, i, -360, 0);
		LineTo(hdc, i, -360 - justify[j % 16]);
	}
	for (int i = 0, j = 0; j < pt.y; i +=1440 / 16, j++)
	{
		MoveToEx(hdc, -360, i, 0);
		LineTo(hdc, -360 - justify[j % 16], i);
	}
	RestoreDC(hdc, -1);
}
void Justify(HDC hdc, TCHAR *pText, RECT rect, int Align)
{
	if (!pText||*pText=='\0')
		return;
	TCHAR *pBegin, *pEnd;
	SIZE size;
	int xStart, yStart = rect.top;
	while (*pText != '\0')
	{
		int space = 0;
		while (*pText == ' ')
			pText++;
		pBegin = pText;
		do
		{
			pEnd = pText;
			while (*pText!='\0'&&*pText++ != ' ')
				;
			if (*pText == '\0')
				break;
			space++;
			GetTextExtentPoint32(hdc, pBegin, pText - pBegin - 1, &size);
		} while (size.cx < rect.right - rect.left);
		space--;
		while (*(pEnd - 1) == ' ')
		{
			pEnd--;
			space--;
		}
		if (*pText == '\0')
		{
			GetTextExtentPoint32(hdc, pBegin, pText - pBegin - 1, &size);
			if (size.cx < rect.right - rect.left)
			   pEnd = pText;
		}
		GetTextExtentPoint32(hdc, pBegin, pEnd-pBegin, &size);
		switch (Align)
		{
		case ID_ALIGN_LEFT:
			xStart = rect.left;
			break;
		case ID_ALIGN_RIGHT:
			xStart = rect.right - size.cx;
			break;
		case ID_ALIGN_CENTER:
			xStart = (rect.left + rect.right - size.cx) / 2;
			break;
		case ID_ALIGN_JUSTIFIED:
			if (*pText != '\0'&&space > 0)
				SetTextJustification(hdc, rect.right - rect.left - size.cx, space);
			xStart = rect.left;
		default:
			break;
		}
		TextOut(hdc, xStart, yStart, pBegin, pEnd - pBegin);
		SetTextJustification(hdc, 0, 0);
		yStart += size.cy;
		pText = pEnd;
	}
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	POINT pt;
	static TCHAR *pText;
	HANDLE hFile;
	unsigned long dwBytesRead,dwFileSize,dwHighSize;
	static HFONT hFont;
	static CHOOSEFONT cf;
	static LOGFONT lf;
	RECT rect;
	static int align = ID_ALIGN_LEFT;
	static DOCINFO di = { sizeof(DOCINFO), TEXT("JUSTIFY1") };
	static PRINTDLG pd = { sizeof(PRINTDLG) };
	static HMENU hMenu;
	bool bSuccess;
	int cxPage, cyPage;
	int iSaveFontHeigh;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		hMenu = GetMenu(hwnd);
		GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);
		cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_EFFECTS;
		cf.hwndOwner = hwnd;
		cf.lpLogFont = &lf;
		cf.lStructSize = sizeof(cf);
		hFile = CreateFile(TEXT("text.txt"), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hwnd, TEXT("�Ҳ���text.txt"), 0, 0);
			return 0;
		}
		dwFileSize = GetFileSize(hFile, &dwHighSize);
		pText =(TCHAR*) malloc(dwFileSize);
		ReadFile(hFile, pText, dwFileSize, &dwBytesRead, 0);
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
		case ID_PRINT:
			pd.Flags = PD_RETURNDC;
			if (!PrintDlg(&pd))
				return 0;
			hdc = pd.hDC;
			if (!hdc)
				MessageBox(hwnd, TEXT("can not cant dc"), 0, 0);
			bSuccess = false;
			rect.left = GetDeviceCaps(hdc, LOGPIXELSX);
			rect.top = GetDeviceCaps(hdc, LOGPIXELSY);
			rect.right = GetDeviceCaps(hdc, PHYSICALWIDTH) - rect.left;
			rect.bottom = GetDeviceCaps(hdc, PHYSICALHEIGHT) - rect.top;
			if (StartDoc(hdc, &di) > 0 && StartPage(hdc) > 0)
			{
				iSaveFontHeigh = lf.lfHeight;
				lf.lfHeight = -(cf.iPointSize*GetDeviceCaps(hdc, LOGPIXELSY)) / 720;
				SelectObject(hdc, CreateFontIndirect(&lf));
				lf.lfHeight = iSaveFontHeigh;
				SetTextColor(hdc, cf.rgbColors);
				Justify(hdc, pText, rect, align);
				if (EndPage(hdc) > 0)
				{
					bSuccess = true;
					EndDoc(hdc);
				}
			}
			DeleteDC(hdc);
			if (!bSuccess)
				MessageBox(hwnd, TEXT("��ӡʧ��"), 0, 0);
			break;
		case ID_FONT:
			if (ChooseFont(&cf))
				InvalidateRect(hwnd, 0, true);
			break;
		case ID_ALIGN_CENTER:
		case ID_ALIGN_JUSTIFIED:
		case ID_ALIGN_LEFT:
		case ID_ALIGN_RIGHT:
			CheckMenuItem(hMenu, align, MF_UNCHECKED);
			align = wmId;
			CheckMenuItem(hMenu, align, MF_CHECKED);
			InvalidateRect(hwnd, 0, true);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		pt.x = cxClient;
		pt.y = cyClient;
		DrawRuler(hdc,pt);
		GetClientRect(hwnd, &rect);
		rect.left = GetDeviceCaps(hdc,LOGPIXELSX) / 2;
		rect.top = GetDeviceCaps(hdc, LOGPIXELSY) / 2;
		rect.right -= GetDeviceCaps(hdc, LOGPIXELSX) / 4;
		SelectObject(hdc, CreateFontIndirect(&lf));
		SetTextColor(hdc, cf.rgbColors);
		Justify(hdc, pText, rect, align);
		DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������ʱ
		if (pText)
			free(pText);
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
	wndclass.lpszMenuName =MAKEINTRESOURCE(IDR_MENU1);//�˵�
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
