//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
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

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient,cxDib,cyDib;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static OPENFILENAME ofn;
	static BITMAPFILEHEADER *pbmfh;
	static BITMAPINFOHEADER *pbmih;
	static unsigned char *pBits,*bits;
	static HMENU hMenu;
	static TCHAR szTitle[MAX_PATH], szPath[MAX_PATH];
	static HBITMAP hBitmap;
	BITMAP bm;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		DibFileInitialize(hwnd, ofn);
		hMenu = GetMenu(hwnd);
		EnableMenuItem(hMenu, ID_SAVE, pbmfh ? MF_ENABLED : MF_GRAYED);
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
		case ID_OPEN:
			ofn.lpstrFile = szPath;
			ofn.lpstrFileTitle = szTitle;
			if (GetOpenFileName(&ofn))
			{
				if (pbmfh)
				{
					free(pbmfh);
					pbmfh = 0;
				}
				if (hBitmap)
				{
					DeleteObject(hBitmap);
					hBitmap = 0;
				}

				pbmfh= DibLoadImage(hwnd, szPath);
				EnableMenuItem(hMenu, ID_SAVE, pbmfh ? MF_ENABLED : MF_GRAYED);
				if (pbmfh)
				{
					pbmih = (BITMAPINFOHEADER*)(pbmfh + 1);
					if (pbmih->biSize == sizeof(BITMAPCOREHEADER))
					{
						cxDib = ((BITMAPCOREHEADER*)(pbmih))->bcWidth;
						cyDib = ((BITMAPCOREHEADER*)(pbmih))->bcHeight;
					}
					else
					{
						cxDib =abs( pbmih->biWidth);
						cyDib =abs( pbmih->biHeight);
					}
					pBits = (unsigned char *)pbmfh + pbmfh->bfOffBits;
					hBitmap = CreateDIBSection(0, (BITMAPINFO*)pbmih, DIB_RGB_COLORS, (void**)&bits, 0, 0);
					CopyMemory(bits, pBits, pbmfh->bfSize -pbmfh->bfOffBits);
					InvalidateRect(hwnd, 0, true);
				}
			}
			break;
		case ID_SAVE:
			if (GetSaveFileName(&ofn))
			{
				DibWriteImage(szPath, pbmfh);
			}
			break;
		default:
			break;
		}
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		if (hBitmap)
		{
			GetObject(hBitmap, sizeof(BITMAP), &bm);
			HDC hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmap);
			BitBlt(hdc, (cxClient - bm.bmWidth) / 2, (cyClient - bm.bmHeight) / 2, bm.bmWidth, bm.bmHeight,
				hdcMem, 0, 0, SRCCOPY);
			DeleteDC(hdcMem);
		}
		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������ʱ
		if (pbmfh)
			free(pbmfh);
		if (hBitmap)
		{
			DeleteObject(hBitmap);
			hBitmap = 0;

		}
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