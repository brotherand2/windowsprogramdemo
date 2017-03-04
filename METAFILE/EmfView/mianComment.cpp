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
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static OPENFILENAME ofn;
	static TCHAR szPath[MAX_PATH], szTitle[MAX_PATH];
	static TCHAR szFilter[] = TEXT("emf�ļ�(*.emf)\0*.emf\0�����ļ�(*.*)\0*.*\0\0");
	static HENHMETAFILE hemf,hemfCopy;
	static RECT rect;
	static DOCINFO di = { sizeof(di), TEXT("EmfView") };
	static PRINTDLG pd = { sizeof(pd) };
	HDC hdcPrn;
	bool bSuccess;
	int length = 0;
	int i = 0;
	TCHAR szBuffer[256];
	ENHMETAHEADER header;
	int isEnabled;
	static HMENU hMenu;
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		hMenu = GetMenu(hwnd);
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFilter = szFilter;
		ofn.lpstrFile = szPath;
		ofn.lpstrFileTitle = szTitle;
		ofn.nMaxFile = MAX_PATH;
		ofn.nMaxFileTitle = MAX_PATH;
		ofn.nFilterIndex = 1;
		ofn.lpstrDefExt = TEXT("emf");
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		break;
	case WM_INITMENUPOPUP:
		if (hemf)
			isEnabled = MF_ENABLED;
		else
			isEnabled = MF_GRAYED;
		EnableMenuItem(hMenu, IDM_SAVEAS, isEnabled);
		EnableMenuItem(hMenu, ID_PRINT, isEnabled);
		EnableMenuItem(hMenu, IDM_PROPERTIES, isEnabled);
		EnableMenuItem(hMenu, IDM_CUT, isEnabled);
		EnableMenuItem(hMenu, IDM_COPY, isEnabled);
		EnableMenuItem(hMenu, IDM_DELETE, isEnabled);
		EnableMenuItem(hMenu, IDM_PASTE, IsClipboardFormatAvailable(CF_ENHMETAFILE)?MF_ENABLED:MF_GRAYED);
		break;

	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		switch (wmId)
		{
		case IDM_OPEN:
			if (!GetOpenFileName(&ofn))
				return 0;
			if (hemf)
			{
				DeleteEnhMetaFile(hemf);
				hemf = 0;
			}
			hemf = GetEnhMetaFile(szPath);
			if (hemf)
				InvalidateRect(hwnd, 0, true);
			else
				MessageBox(hwnd, TEXT("����ͼԪ�ļ�"), TEXT("�ļ���ȡ����"), MB_ICONERROR);
			break;
		case IDM_SAVEAS:
			ofn.Flags = OFN_OVERWRITEPROMPT;
			if(!GetSaveFileName(&ofn))
				return 0;
			hemfCopy = CopyEnhMetaFile(hemf, szPath);
			if (hemfCopy)
			{
				DeleteEnhMetaFile(hemf);
				hemf = hemfCopy;
			}
			else
				MessageBox(hwnd, TEXT("���ܱ���ͼԪ�ļ�"), TEXT("�ļ�����ʧ��"), MB_ICONERROR);
			break;
		case ID_PRINT:
			pd.Flags = PD_RETURNDC;
			if (!PrintDlg(&pd))
				return 0;
			hdcPrn = pd.hDC;
			if (!hdcPrn)
			{
				MessageBox(hwnd, TEXT("���ܻ��dc"), TEXT("��ӡʧ��"), MB_ICONERROR);
				return 0;
			}
			rect.right = GetDeviceCaps(hdcPrn, HORZRES);
			rect.bottom = GetDeviceCaps(hdcPrn, VERTRES);
			bSuccess = false;
			if (StartDoc(hdcPrn, &di) > 0 && StartPage(hdcPrn) > 0)
			{
				PlayEnhMetaFile(hdcPrn, hemf, &rect);
				if (EndPage(hdcPrn) > 0)
				{
					bSuccess = true;
					EndDoc(hdcPrn);
				}
			}
			if (!bSuccess)
				MessageBox(hwnd, TEXT("��ӡʧ��"), TEXT(""), MB_ICONERROR);
			break;
		case IDM_PROPERTIES:
			length = GetEnhMetaFileDescription(hemf, 0, 0);
			GetEnhMetaFileHeader(hemf, sizeof(ENHMETAHEADER),&header);
			i += wsprintf(szBuffer, TEXT("Bounds=(%d,%d)to(%d,%d)pixels\n"), header.rclBounds.left,
				header.rclBounds.top, header.rclBounds.right, header.rclBounds.bottom);
			i += wsprintf(szBuffer + i, TEXT("Frame=(%d,%d)to(%d,%d)mms\n"), header.rclFrame.left,
				header.rclFrame.top, header.rclFrame.right, header.rclFrame.bottom);
			i += wsprintf(szBuffer + i, TEXT("Resolution=(%d,%d)mms=(%d,%d)pixels\n"), header.szlDevice.cx,
				header.szlDevice.cy, header.szlMillimeters.cx, header.szlMillimeters.cy);
			i += wsprintf(szBuffer + i, TEXT("Size=%d,Records=%d,Handles=%d,Palette Entries=%d\n"),
				header.nSize, header.nRecords, header.nHandles, header.nPalEntries);
			if (length)
			{
				i += wsprintf(szBuffer+i, TEXT("Description:"));
				GetEnhMetaFileDescription(hemf, length, szBuffer + i);
				szBuffer[lstrlen(szBuffer)] = '\t';
			}
			MessageBox(hwnd, szBuffer, TEXT("MetaFile Properties"), 0);
			break;
		case ID_EXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case IDM_CUT:
		case IDM_COPY:
			if (!hemf)
				return 0;
			hemfCopy = CopyEnhMetaFile(hemf, 0);
			OpenClipboard(hwnd);
			EmptyClipboard();
			SetClipboardData(CF_ENHMETAFILE, hemfCopy);
			CloseClipboard();
			if (wmId==IDM_COPY)
				break;
		case IDM_DELETE:
			if (hemf)
			{
				DeleteEnhMetaFile(hemf);
				hemf = 0;
				InvalidateRect(hwnd, 0, true);
			}
			break;
		case IDM_PASTE:
			OpenClipboard(hwnd);
			hemfCopy =(HENHMETAFILE) GetClipboardData(CF_ENHMETAFILE);
			if (hemfCopy)
			{
				if (hemf)
				DeleteEnhMetaFile(hemf);
				hemf =CopyEnhMetaFile(hemfCopy,0);
				InvalidateRect(hwnd, 0, true);
			}
			CloseClipboard();
			break;
		case IDM_ABOUT:
			MessageBox(hwnd, TEXT("Metafile Viewer ver1.1\n(c)zeng,13"), TEXT("EmvView"), MB_OK);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		GetClientRect(hwnd, &rect);
		if (hemf)
			PlayEnhMetaFile(hdc, hemf,&rect);
		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������ʱ
		PostQuitMessage(0);//�׳�һ������ֵΪ0�˳���Ϣ
		if (hemf)
			DeleteEnhMetaFile(hemf);
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