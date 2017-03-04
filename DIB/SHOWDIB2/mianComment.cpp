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

void ShowDib(HDC hdc, int show,PBYTE pBits,BITMAPINFO *pbmih,int cxDib,int cyDib,int cxClient,int cyClient)
{
	switch (show)
	{
	case ID_LEFTTOP:
		SetDIBitsToDevice(hdc, 0, 0, cxDib, cyDib, 0, 0, 0, cyDib, pBits, pbmih, DIB_RGB_COLORS);
		break;
	case ID_CENTER:
		SetDIBitsToDevice(hdc,(cxClient-cxDib)/2,(cyClient-cyDib)/2, cxDib, cyDib, 0, 0, 0, cyDib, pBits, pbmih, DIB_RGB_COLORS);
		break;
	case ID_STRETCH:
		StretchDIBits(hdc, 0, 0, cxClient, cyClient, 0, 0, cxDib, cyDib, pBits, pbmih, DIB_RGB_COLORS, SRCCOPY);
		break;
	case ID_ISOTROPICSTRETIC:
		SetMapMode(hdc, MM_ISOTROPIC);
		SetWindowExtEx(hdc, cxDib,cyDib, 0);
		SetViewportExtEx(hdc, cxClient, cyClient, 0);
		SetWindowOrgEx(hdc, cxDib / 2, cyDib / 2, 0);
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, 0);
		StretchDIBits(hdc, 0, 0, cxDib, cyDib, 0, 0, cxDib, cyDib, pBits, pbmih, DIB_RGB_COLORS, SRCCOPY);
		break;
	default:
		break;
	}

}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc,hdcPrn;//����
	static int cxClient, cyClient, cxDib, cyDib;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static OPENFILENAME ofn;
	static BITMAPFILEHEADER *pbmfh;
	static BITMAPINFO *pbmih;
	static unsigned char *pBits;
	static HMENU hMenu;
	static TCHAR szTitle[MAX_PATH], szPath[MAX_PATH];
	static bool isEnable;
	static int show = ID_LEFTTOP;
	HGLOBAL hGlobal;
	PBYTE pGlobal;
	static DOCINFO di = { sizeof(di),TEXT("print dib") };
	static PRINTDLG pt = { sizeof(pt) };
	int cxPage, cyPage;
	bool success;
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
	case WM_INITMENUPOPUP:
		if (pbmfh)
			isEnable = MF_ENABLED;
		else
			isEnable = MF_GRAYED;
		EnableMenuItem(hMenu, ID_SAVE, isEnable);
		EnableMenuItem(hMenu, ID_PRINT, isEnable);
		EnableMenuItem(hMenu, ID_CUT, isEnable);
		EnableMenuItem(hMenu, ID_COPY, isEnable);
		EnableMenuItem(hMenu, ID_DELETE, isEnable);
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
				pbmfh= DibLoadImage(hwnd, szPath);
				EnableMenuItem(hMenu, ID_SAVE, pbmfh ? MF_ENABLED : MF_GRAYED);
				if (pbmfh)
				{
					pbmih = (BITMAPINFO*)(pbmfh + 1);
					if (pbmih->bmiHeader.biSize == sizeof(BITMAPCOREHEADER))
					{
						cxDib = ((BITMAPCOREHEADER*)(pbmih))->bcWidth;
						cyDib = ((BITMAPCOREHEADER*)(pbmih))->bcHeight;
					}
					else
					{
						cxDib =abs( pbmih->bmiHeader.biWidth);
						cyDib =abs( pbmih->bmiHeader.biHeight);
					}
					pBits = (unsigned char *)pbmfh + pbmfh->bfOffBits;
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
		case ID_PRINT:
			pt.Flags = PD_RETURNDC|PD_NOSELECTION;
			if (!PrintDlg(&pt))
				return 0;
			hdcPrn = pt.hDC;
			if (!hdcPrn)
			{
				MessageBox(hwnd, TEXT("���ܻ�ô�ӡ��dc"), 0, MB_ICONERROR);
				return 0;
			}
			if (!(RC_BITBLT&GetDeviceCaps(hdcPrn, RASTERCAPS)))
			{
				MessageBox(hwnd, TEXT("��ӡ����֧�ִ�ӡλͼ"), 0, MB_ICONERROR);
				return 0;
			}
			cxPage = GetDeviceCaps(hdcPrn, HORZRES);
			cyPage = GetDeviceCaps(hdcPrn, VERTRES);
			success = true;
			if (StartDoc(hdcPrn, &di) > 0 )
			{
				for (int i = 0; i < pt.nCopies; i++)
				{
					if (StartPage(hdcPrn)>0)
					{
						ShowDib(hdcPrn, show, pBits, pbmih, cxDib, cyDib, cxPage, cyPage);
						if (EndPage(hdcPrn)<0)
						   break;
					}
					else
					{
						success = false;
						break;
					}
				}     
				if (success)					
					EndDoc(hdcPrn);
				else
					MessageBox(hwnd, TEXT("��ӡʧ��"), 0, MB_ICONERROR);
			}
			else
				MessageBox(hwnd, TEXT("��ӡʧ��"), 0, MB_ICONERROR);

			DeleteDC(hdcPrn);
			break;
		case ID_LEFTTOP:
		case ID_CENTER:
		case ID_ISOTROPICSTRETIC:
		case ID_STRETCH:
			CheckMenuItem(hMenu, show,MF_UNCHECKED);
			show = wmId;
			CheckMenuItem(hMenu, show,MF_CHECKED);
			InvalidateRect(hwnd, 0, true);
			break;
		case ID_COPY:
		case ID_CUT:
			if (!pbmfh)
				return 0;
			hGlobal = GlobalAlloc(GHND | GMEM_SHARE, pbmfh->bfSize - sizeof(BITMAPFILEHEADER));
			pGlobal =(PBYTE) GlobalLock(hGlobal);
			CopyMemory(pGlobal, (PBYTE)pbmfh + sizeof(BITMAPFILEHEADER), pbmfh->bfSize - sizeof(BITMAPFILEHEADER));
			GlobalUnlock(hGlobal);
			OpenClipboard(hwnd);
			EmptyClipboard();
			SetClipboardData(CF_DIB, hGlobal);
			CloseClipboard();
			if (wmId == ID_COPY)
			break;
		case ID_DELETE:
			if (pbmfh)
			{
				free(pbmfh);
				pbmfh = 0;
			}
			InvalidateRect(hwnd, 0, true);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		if (pbmfh)
			ShowDib(hdc, show, pBits, pbmih,cxDib,cyDib,cxClient,cyClient);
		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������ʱ
		if (pbmfh)
			free(pbmfh);
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