//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<tchar.h>
#include"resource.h"
#include"DIBFILE.h"
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
void DisplayDibHeaders(HWND hwnd, TCHAR *szTitle, TCHAR *szPath)
{
	static TCHAR *szInfoName[] = {TEXT("BITMAPCOREHEADER"),TEXT("BITMAPINFOHEADER"),TEXT("BITMAPV4HEADER"),TEXT("BITMAPV5HEADER")};
	static TCHAR *szCompression[] = {TEXT("BI_RGB"),TEXT("BI_RLE8"),TEXT("BI_RLE4"),TEXT("BI_BITFIELDS"),TEXT("unknown")};
	Printf(hwnd, TEXT("File:%s\r\n\r\n"), szPath);
	BITMAPFILEHEADER *pbmfh=DibLoadImage(hwnd, szPath);
	if (!pbmfh)
		return;
	Printf(hwnd, TEXT("�ļ�ͷ\r\n"));
	Printf(hwnd, TEXT("\t.�ļ�����=0X%X\r\n"),pbmfh->bfType);
	Printf(hwnd, TEXT("\t.�ļ���С=%u\r\n"),pbmfh->bfSize);
	Printf(hwnd, TEXT("\t.�ļ�����1=%u\r\n"),pbmfh->bfReserved1);
	Printf(hwnd, TEXT("\t.�ļ�����1=%u\r\n"),pbmfh->bfReserved2);
	Printf(hwnd, TEXT("\t.����λƫ��=%u\r\n\r\n"),pbmfh->bfOffBits);
	BITMAPV5HEADER *pbmih = (BITMAPV5HEADER*)(pbmfh + 1);//��Ϣͷ
	int size;
	switch (pbmih->bV5Size)
	{
	case sizeof(BITMAPCOREHEADER) :
		size = 0;
		break;
	case sizeof(BITMAPINFOHEADER) :
		size = 1;
		break;
	case sizeof(BITMAPV4HEADER) :
		size = 2;
		break;
	case sizeof(BITMAPV5HEADER) :
		size = 3;
		break;
	default:
		size = 4;
		break;
	}
	Printf(hwnd, TEXT("��Ϣͷ:%s\r\n"), szInfoName[size]);
	if (size == 0)//����Ǻ����ļ�ͷ����ȡ��߶���WORDҪ�ȴ���
	{
		BITMAPCOREHEADER *pbmch = (BITMAPCOREHEADER*)pbmfh;
		Printf(hwnd, TEXT("\t.��Ϣͷ��С=%u\r\n"), pbmch->bcSize);
		Printf(hwnd, TEXT("\t.�����ؼƵ�ͼ����=%u\r\n"), pbmch->bcWidth);
		Printf(hwnd, TEXT("\t.�����ؼƵ�ͼ��߶�=%u\r\n"), pbmch->bcHeight);
		Printf(hwnd, TEXT("\t.ƽ����=%u\r\n"), pbmch->bcPlanes);
		Printf(hwnd, TEXT("\t.ÿ�����ص�λ��=%u\r\n"), pbmch->bcBitCount);
		free(pbmfh);
		return;
	}
	//��ʾ��Ϣͷ
	Printf(hwnd, TEXT("\t.��Ϣͷ��С=%u\r\n"),pbmih->bV5Size);
	Printf(hwnd, TEXT("\t.�����ؼƵ�ͼ����=%u\r\n"),pbmih->bV5Width);
	Printf(hwnd, TEXT("\t.�����ؼƵ�ͼ��߶�=%u\r\n"),pbmih->bV5Height);
	Printf(hwnd, TEXT("\t.ƽ����=%u\r\n"),pbmih->bV5Planes);
	Printf(hwnd, TEXT("\t.ÿ�����ص�λ��=%u\r\n"),pbmih->bV5BitCount);
	Printf(hwnd, TEXT("\t.ѹ������=%s\r\n"), szCompression[min(4, pbmih->bV5Compression)]);
	Printf(hwnd, TEXT("\t.ͼ����ֽ���=%u\r\n"), pbmih->bV5SizeImage);
	Printf(hwnd, TEXT("\t.ˮƽ�ֱ���=%u\r\n"), pbmih->bV5XPelsPerMeter);
	Printf(hwnd, TEXT("\t.��ֱ�ֱ���=%u\r\n"), pbmih->bV5YPelsPerMeter);
	Printf(hwnd, TEXT("\t.�õ�����ɫ��=%u\r\n"), pbmih->bV5ClrUsed);
	Printf(hwnd, TEXT("\t.��Ҫ��ɫ��=%u\r\n\r\n"), pbmih->bV5ClrImportant);
	if (size == 1)//��չ����Ϣͷ
	{
		if (pbmih->bV5Compression == BI_BITFIELDS)
		{
			Printf(hwnd, TEXT("\t.��ɫ����=%x\r\n"),pbmih->bV5RedMask);
			Printf(hwnd, TEXT("\t.��ɫ����=%x\r\n"),pbmih->bV5GreenMask);
			Printf(hwnd, TEXT("\t.��ɫ����=%x\r\n"),pbmih->bV5BlueMask);
		}
		free(pbmfh);
		return;
	}
	Printf(hwnd, TEXT("\t.��ɫ����=%x\r\n"), pbmih->bV5RedMask);
	Printf(hwnd, TEXT("\t.��ɫ����=%x\r\n"), pbmih->bV5GreenMask);
	Printf(hwnd, TEXT("\t.��ɫ����=%x\r\n"), pbmih->bV5BlueMask);
	Printf(hwnd, TEXT("\t.����������=%x\r\n"), pbmih->bV5AlphaMask);
	Printf(hwnd, TEXT("\t.ɫ�ʿռ�����=%x\r\n"), pbmih->bV5CSType);
	Printf(hwnd, TEXT("\t.��ɫ٤��ֵ=%x\r\n"), pbmih->bV5GammaRed);
	Printf(hwnd, TEXT("\t.��ɫ٤��ֵ=%x\r\n"), pbmih->bV5GammaGreen);
	Printf(hwnd, TEXT("\t.��ɫ٤��ֵ=%x\r\n"), pbmih->bV5GammaBlue);
	free(pbmfh);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static HWND hEdit;
	static TCHAR szTitle[MAX_PATH], szPath[MAX_PATH];
	static OPENFILENAME ofn;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		hEdit = CreateWindow(TEXT("edit"), 0, WS_CHILD |WS_VISIBLE|  ES_MULTILINE |ES_READONLY|  WS_VSCROLL | WS_HSCROLL,
			0, 0, 0, 0, hwnd, (HMENU)1,hInst, 0);
		DibFileInitialize(hwnd, ofn);
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		MoveWindow(hEdit, 0, 0, cxClient, cyClient, true);
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		switch (wmId)
		{
		case ID_OPEN:
			ofn.lpstrFileTitle = szTitle;
			ofn.lpstrFile = szPath;
			if (GetOpenFileName(&ofn))
				DisplayDibHeaders(hEdit,szTitle,szPath);
			break;
		default:
			break;
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