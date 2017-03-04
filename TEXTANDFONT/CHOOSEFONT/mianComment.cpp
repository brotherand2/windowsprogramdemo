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

HFONT ChooseNewFont(CHOOSEFONT &cf)
{
	static LOGFONT lf;
	;
	lf.lfHeight = 60;
	lstrcpy( lf.lfFaceName,TEXT("����"));
	cf.lStructSize = sizeof(cf);
	cf.lpLogFont = &lf;
	cf.Flags = CF_EFFECTS|CF_INITTOLOGFONTSTRUCT;
	ChooseFont(&cf);
	return CreateFontIndirect(&lf);
}
HFONT CreateMyFont1()
{
	static LOGFONT lf;
	lf.lfHeight = 60;
	lstrcpy( lf.lfFaceName,TEXT("����"));
	lf.lfStrikeOut = 1;
	lf.lfUnderline = 1;
	return CreateFontIndirect(&lf);

}

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	HFONT hFont,old;
	TCHAR szBuffer[128] = TEXT("Abcdef����");
	static CHOOSEFONT cf;
	LOGFONT lf;
	TEXTMETRIC tm;
	int y;
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
	case WM_LBUTTONDBLCLK:
		hdc = GetDC(hwnd);
		hFont = ChooseNewFont(cf);
		y = 0;

		old = (HFONT)SelectObject(hdc, hFont);
		SetTextColor(hdc, cf.rgbColors);
		GetTextMetrics(hdc, &tm);
		GetObject(hFont, sizeof(LOGFONT), &lf);
		TextOut(hdc, 100, cyClient/2+100, szBuffer, lstrlen(szBuffer));
		SelectObject(hdc, old);
		DeleteObject(hFont);

		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmAscent=%d"), tm.tmAscent));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmAveCharWidth=%d"), tm.tmAveCharWidth));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT(" tm.tmBreakChar=%d"), tm.tmBreakChar));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmCharSet=%d"), tm.tmCharSet));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmDefaultChar=%d"), tm.tmDefaultChar));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT(" tm.tmDescent=%d"), tm.tmDescent));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmDigitizedAspectX=%d"), tm.tmDigitizedAspectX));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmDigitizedAspectY=%d"), tm.tmDigitizedAspectY));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmExternalLeading=%d"), tm.tmExternalLeading));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmFirstChar=%d"), tm.tmFirstChar));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmHeight=%d"), tm.tmHeight));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmInternalLeading=%d"), tm.tmInternalLeading));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmItalic=%d"), tm.tmItalic));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT(" tm.tmLastChar=%d"), tm.tmLastChar));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmMaxCharWidth=%d"), tm.tmMaxCharWidth));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT(" tm.tmOverhang=%d"), tm.tmOverhang));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmPitchAndFamily=0x%2x"), tm.tmPitchAndFamily));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmStruckOut=%d"), tm.tmStruckOut));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmUnderlined=%d"), tm.tmUnderlined));
		TextOut(hdc, cxClient / 2, y += cyChar, szBuffer, wsprintf(szBuffer, TEXT("tm.tmWeight=%d"), tm.tmWeight));



		ReleaseDC(hwnd, hdc);

		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		hFont = CreateMyFont1();
		old=(HFONT)SelectObject(hdc, hFont);
		TextOut(hdc, 100,100, szBuffer, lstrlen(szBuffer));
		SelectObject(hdc, old);
		DeleteObject(hFont);
		SetGraphicsMode(hdc, GM_ADVANCED);
		hFont = CreateFont(40, 0,300,0,400,0, 1, 1, 1, 0, 0, 0, 0, TEXT("������"));
		old=(HFONT)SelectObject(hdc, hFont);
		TextOut(hdc, 100,cyClient/2+20, szBuffer, lstrlen(szBuffer));
		SelectObject(hdc, old);
		DeleteObject(hFont);

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
	wndclass.style = CS_VREDRAW | CS_HREDRAW|CS_DBLCLKS;//��Щ�ı��Ҫ�ػ�������ˮƽ�ʹ�ֱ�ı���ػ�
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