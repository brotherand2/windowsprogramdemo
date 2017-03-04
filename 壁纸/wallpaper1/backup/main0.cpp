#include"resource.h"
HMENU CreateMyPopMenu(HINSTANCE hInst)//���������˵�
{
	HMENU hMenu = CreatePopupMenu();//���������˵�
	HBITMAP hBm[5];
	AppendMenu(hMenu, MF_STRING, IDM_SHOWWALLPAPER, TEXT("�鿴�����ֽ"));
	AppendMenu(hMenu, MF_STRING, IDM_NEXTWALLPAPER, TEXT("��һ��"));
	AppendMenu(hMenu, MF_STRING, IDM_PREWALLPAPER, TEXT("��һ��"));
	AppendMenu(hMenu, MF_STRING, IDM_PAUSE, TEXT("��ͣ"));
	AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("�˳�"));
	hBm[0] = LoadBitmap(hInst, TEXT("IDB_BITMAP1"));//��ȡλͼ
	SetMenuItemBitmaps(hMenu, IDM_SHOWWALLPAPER, MF_BYCOMMAND, hBm[0], 0);//���ò˵�λͼ
	DeleteObject(hBm);
	hBm[1]= LoadBitmap(hInst, TEXT("IDB_BITMAP2"));//��ȡλͼ
	SetMenuItemBitmaps(hMenu, IDM_NEXTWALLPAPER, MF_BYCOMMAND, hBm[1], 0);//���ò˵�λͼ
	DeleteObject(hBm);
	hBm[2] = LoadBitmap(hInst, TEXT("IDB_BITMAP3"));//��ȡλͼ
	SetMenuItemBitmaps(hMenu, IDM_PREWALLPAPER, MF_BYCOMMAND, hBm[2], 0);//���ò˵�λͼ
	DeleteObject(hBm);
	hBm[3] = LoadBitmap(hInst, TEXT("IDB_BITMAP4"));//��ȡλͼ
	SetMenuItemBitmaps(hMenu, IDM_PAUSE, MF_BYCOMMAND, hBm[3], 0);//���ò˵�λͼ
	hBm[4] = LoadBitmap(hInst, TEXT("IDB_BITMAP5"));//��ȡλͼ
	SetMenuItemBitmaps(hMenu, IDM_EXIT, MF_BYCOMMAND, hBm[4], 0);//���ò˵�λͼ
	return hMenu;
}
void FindFile(const tstring &wallpaper)//�����ļ�
{
	WIN32_FIND_DATA fd;
	HANDLE handle=FindFirstFile(wallpaper.c_str(),&fd);//���ҵ�1���ļ�
	if (handle!= INVALID_HANDLE_VALUE)//�ҵ���
	{
		do
		{
			if (fd.cFileName[0] == '.')//win32��һ����������ļ��У�һ���ǡ�.��ָ���ǵ�ǰ�ļ��У�һ���ǡ�..��ָ�����ϲ��ļ��У����һ�־�����ͨ���²��ļ������ļ�������ʾ��cd .�Ǳ�Ŀ¼��cd ..���ϼ�Ŀ¼
				break;//����Ϊ�˲��ص��ϼ�Ŀ¼����ԭĿ¼��ת������.��..�����Ŀ¼
			fileName.push_back(fd.cFileName);//����ǰ�ļ�������
		} while (FindNextFile(handle, &fd));//������һ���ļ�
	}
}
int _stdcall EnumWindowProc(HWND hwnd, long lParam)//ö�ٺ����Ĵ��ڹ���
{
	TCHAR title[256];//���մ��ڱ���
	if (IsWindow(hwnd) && IsWindowVisible(hwnd) && !IsIconic(hwnd))
	{//�������������ڣ����Ҵ��ڿɼ���������С��
		GetWindowText(hwnd, title, 256);//�õ����ڱ���
		temp += title + tstring(TEXT(" "));//����������ַ���

		if (title[0] != '\0')//������ⲻΪ��
		allTitle.push_back(title);//�����������
	}
	return true;//Ϊ����ö����һ�����ڣ��ص��������뷵��TRUE����ֹͣ�б������뷵��FALSE
}
int _stdcall DlgProc(HWND hwnd, unsigned int message, unsigned wParam, long lParam)//�Ի��򴰿ڹ���
{

	switch (message)
	{
	case WM_INITDIALOG:
		return true;
	case WM_COMMAND:
		switch (wParam)
		{
		//case IDC_BUTTON1:
		//	EndDialog(hwnd, 0);
			return true;
		}
		return true;
	}
	return false;
}
void InitNotifyIconData(HWND hwnd,HINSTANCE hInst, NOTIFYICONDATA &nid)//��ʼ�����̽ṹ
{
	nid.cbSize = sizeof(nid);//�ṹ��Ĵ�С�����ֽ�Ϊ��λ
	nid.dwInfoFlags = NIIF_USER;//���ô˳�Ա������������ʾ������һ��ͼ�꣬NIIF_USER���û���ͼ�꣬����Աnid.hIcon
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));//��ȡ�����õ�ͼ�꣬���ӡ��޸Ļ�ɾ����ͼ��ľ��
	nid.uFlags = NIF_ICON | NIF_INFO | NIF_MESSAGE | NIF_TIP;//�˳�Ա����������Щ������ԱΪ�Ϸ����ݣ�����Щ��Ա�����ã�
	//NIF_ICONͼ�꣬NIF_INFO,ʹ��������ʾ������ͨ�Ĺ�����ʾ��szInfo, uTimeout, szInfoTitle��dwInfoFlags��Ա������
	//NIF_MESSAGE uCallbackMessage��Ա������
	//NIF_TIP  szTip��Ա������
	nid.hWnd = hwnd;//���ڵľ������ʾ�Ĵ�����������������ͼ����ص���Ϣ
	nid.uID = 0;//Ӧ�ó������������ͼ��ı�ʶ��
	StringCchCopy(nid.szInfo, 256, szInfo);//�ַ���������Ϊ������ʾ���ݡ���ຬ��255���ַ�
	StringCchCopy(nid.szInfoTitle, 64, szInfoTitle);//�ַ���������Ϊ������ʾ�ı��⡣�˱��������������ʾ����ϲ�����ຬ��63���ַ���
	StringCchCopy(nid.szTip, 128, szTip);//ָ��һ����\0�������ַ�����ָ�롣�ַ���������Ϊ��׼������ʾ����Ϣ
	nid.uCallbackMessage = WM_USER;//Ӧ�ó��������Ϣ��ʾ��������ͼ������������¼�����ʹ�ü���ѡ��򼤻�ͼ��ʱ��ϵͳ��ʹ�ô˱�ʾ����hWnd��Ա��ʾ�Ĵ��ڷ�����Ϣ
	//WM_USER Ϊ�˷�ֹ�û��������ϢID��ϵͳ����ϢID��ͻ��MS��Microsoft��������һ����WM_USER��С��WM_USER��ID��ϵͳʹ�ã�����WM_USER��ID���û�ʹ��
}
void PopupMyMenu(HWND hwnd,HMENU hMenu)//�����˵�
{
	POINT mousePos;//���λ��
	GetCursorPos(&mousePos);//�õ����λ��
	SetForegroundWindow(hwnd);//�������ö������ܲ�׽���ڲ˵�����굥���Ĳ���������ڲ˵��ⵥ������˵�����ʧ������
	int id = TrackPopupMenu(hMenu, TPM_RETURNCMD, mousePos.x, mousePos.y, 0, hwnd, 0);//�����˵�,TPM_RETURNCMD���ز˵�ID
	switch (id)
	{
	case IDM_EXIT:
		SendMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	case IDM_NEXTWALLPAPER://��һ��
		break;
	case IDM_PREWALLPAPER://��һ��
		break;
	case IDM_PAUSE://��ͣ
		if (startChange)//�����ʼ����ֽ
		{
			ModifyMenu(hMenu, IDM_PAUSE, MF_BYCOMMAND, IDM_PAUSE, TEXT("��ʼ"));
			startChange = false;//��ͣ���
		}
		else
		{
			ModifyMenu(hMenu, IDM_PAUSE, MF_BYCOMMAND, IDM_PAUSE, TEXT("��ͣ"));
			startChange = true;//��ʼ���
		}
		break;
	}
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//������ ���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ
	HDC hdc;//����
	TEXTMETRIC tm;//�ı��ߴ�ṹ
	static int cxClient, cyClient;//�ͻ�����С
	static int cxChar, cxCaps, cyChar;//��ĸ�߶ȡ����
	RECT rect;//����
	tstring wallpaper = TEXT("E:\\��ֽ\\SogouWP\\Local\\WallPaper\\");//��ֽ·��
	tstring format = TEXT("*.jpg");//��ֽ��׺��ʽ
	int size;//�����С
	static SCROLLINFO si;//��������Ϣ
	static int iVertPos, iHorzPos;//ˮƽ�ʹ�ֱ������λ��
	int start, end;//����������ʱ���ػ����ʼ�ͽ���λ��
	TCHAR szBuffer[256];//�ַ�����
	static int iPerLine;//����һ�еĿ̶�
	static int iAccumLine ;//�����Ŀ̶�
	static int ulScrollLines;//�������һ�ι���������
	static NOTIFYICONDATA nid;//ϵͳ��Ҫ��������������������Ϣ����Ϣ
	static POINT mousePos;//���λ��
	static int nSelectedLine;//��ѡ�е���
	static int nPageLine;//һҳ������
	static HBRUSH oldBrush;//����ɻ�ˢ
	static HBRUSH blueBrush;//��ɫ��ˢ��ѡ��ʱ������ʾ����ɫ
	static HINSTANCE hInst;
	static HMENU hMenu;//�˵����
	switch (UMsg)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;//����ʵ�����
		InitNotifyIconData(hwnd, hInst, nid);//��ʼ������ͼ��ṹ
		Shell_NotifyIcon(NIM_ADD, &nid);//�������������һ��ͼ��
		hMenu = CreateMyPopMenu(hInst);//�����˵�
		hdc = GetDC(hwnd);//�õ����ڻ���
		GetTextMetrics(hdc, &tm);//�õ����ʵ��ı��ߴ�
		cxChar = tm.tmAveCharWidth;//Сд��ĸ��ƽ�����
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;//��д��ĸ��ƽ�����
		cyChar = tm.tmHeight + tm.tmExternalLeading;//��ĸ��ƽ���߶�
		ReleaseDC(hwnd, hdc);
		FindFile(wallpaper + format);//��������ͼƬ
		SetTimer(hwnd, wallpaperID,wallPaperTime, nullptr);//���ñ�ֽ��ʱ��
		SetTimer(hwnd, tipID,tipTime, nullptr);//����������ʾ��ʱ��
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ulScrollLines, 0);//��ȡ�������һ�ι���������
		if (ulScrollLines)
			iPerLine = WHEEL_DELTA / ulScrollLines;//�������һ�еĿ̶�
		else
			iPerLine = 0;
		srand(time(0));//����ǰʱ����Ϊ�����������
		blueBrush = CreateSolidBrush(RGB(0, 0, 255));//��ɫ��ˢ
		break;
	case WM_MOUSEWHEEL://������
		if (ulScrollLines == 0)//��겻����������˳�
			break;
		iAccumLine += (SHORT)HIWORD(wParam);//���Ϲ����Ŀ̶�
		while (iAccumLine >= iPerLine)//�����������һ�еĿ̶�
		{
			iAccumLine -= iPerLine;//��ȥһ�еĿ̶�
			SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, lParam);//������Ϣ�����Ϲ���һ��
		}
		while (iAccumLine <= -iPerLine)//�����������һ�еĿ̶�
		{
			iAccumLine += iPerLine;//����һ�еĿ̶�
			SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, lParam);//���¹���
		}
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);//�ؼ�ID
		int wmEvent = HIWORD(wParam);//�ؼ��ϵĲ���
		switch (wmId)
		{
			//break;
		}
	}
		break;
	case WM_LBUTTONDBLCLK://���˫��
		//MessageBox(hwnd, TEXT(" double mouse"), 0, 0);
		break;
	case WM_LBUTTONDOWN://��굥��
		mousePos.x=GET_X_LPARAM(lParam);//������굱ǰ���ڵ�λ�ã��ᡢ������)
		mousePos.y=GET_Y_LPARAM(lParam);
		nSelectedLine = mousePos.y / cyChar;//������걻���е���
		if (nSelectedLine >= 0 && nSelectedLine < nPageLine)//�������ڿͻ�����
		{
			nSelectedLine +=iVertPos;//nSelectedLine���ϵ�ǰ��ֱ��������λ�ã�����ǰ��ѡ�е���
			InvalidateRect(hwnd, 0, true);//���и�����ʾ
		}
		break;
	case  WM_RBUTTONDOWN://����Ҽ�����
		PopupMyMenu(hwnd, hMenu);//�����˵�
		break;
	case WM_TIMER://��ʱ����Ϣ
		 switch (wParam)
		 {
		 case tipID://��ʾ��ʱ����
			 Shell_NotifyIcon(NIM_MODIFY, &nid);//�޸����������һ��ͼ�꣬�ٴ���ʾ����
			 break;
		 case wallpaperID://����ֽ��ʾ����
			 EnumWindows(EnumWindowProc, 0);//ö��������Ļ�ϵĶ��㴰�ڣ��������ھ�����͸�Ӧ�ó�����Ļص�����
			 size = fileName.size();//�ļ��ĸ���
			 {
				 HWND hDestop = GetDesktopWindow();
			     HDC  hdcScr = GetDCEx(hDestop, 0, DCX_LOCKWINDOWUPDATE);//�����Ļ�Ļ���,��ʹ��Ļ��סҲ��ˢ��
			     // TextOut(hdcScr, 20, 400, temp.c_str(), temp.size());
			 }
			 if (allTitle.size() < 2&&size>0)//���ֻ��1�����ڼ�����Window Manager����������ʾ����ͼƬ������Ϊ0
			 {
				SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,(PVOID) (wallpaper+fileName[rand()%size]).c_str(), 0);//��ͼƬ�����ѡ1������Ϊ��ֽ
			 }
			 temp.clear();//��մ��ڱ���
			 allTitle.clear();//��մ��ڱ���
			break;
		 }
		break;
	case WM_USER:
		switch (lParam)
		{
			case WM_LBUTTONDBLCLK:
			case WM_LBUTTONDOWN://������ͼ���ϵ�����
				ShowWindow(hwnd, SW_NORMAL);//��ʾ����
				SetForegroundWindow(hwnd);//�����ڷŵ�ǰ̨�����ö�
				break;
			case WM_RBUTTONDOWN:////������ͼ�����Ҽ�������
			{
				PopupMyMenu(hwnd, hMenu);//�����˵�
				break;
			}
		}
		break;
	case WM_SIZE://��С�ı��
		cxClient = LOWORD(lParam);//�ͻ����Ŀ�ȡ��߶�
		cyClient = HIWORD(lParam);
		nPageLine = cyClient / cyChar;//���һҳ������
		si.cbSize = sizeof(si);//�ṹ��С
		si.fMask = SIF_PAGE | SIF_RANGE;//���õı�ǣ�ֻ����SIF_PAGE��SIF_RANGE��ʹ��SIF_PAGE��־�ܹ�ȡ��ҳ���С��SIF_RANGE��־ʱ�������nMin��nMax�ֶ��趨Ϊ����ľ��з�Χ
		si.nMin = 0;
		si.nMax = fileName.size() - 1;//���Χ�����ֵĸ���
		si.nPage=nPageLine;//һҳ�ĵ�����
		SetScrollInfo(hwnd, SB_VERT, &si, true);//���ô�ֱ����������

		si.nMax = 100;//ˮƽ���������Χ
		si.nPage = cxClient / cxChar;//����һҳ����
		SetScrollInfo(hwnd, SB_HORZ, &si, true);//����ˮƽ����������
		if (IsIconic(hwnd))//���������С����ť
			ShowWindow(hwnd, SW_HIDE);//���ش���
		break;
	case WM_VSCROLL://��ֱ����
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;//��ù�����������Ϣ
		GetScrollInfo(hwnd, SB_VERT, &si);
		iVertPos = si.nPos;//��ǰ��ֱ��������λ��
		switch (LOWORD(wParam))
		{
		case SB_LINEUP://���Ϲ�
			si.nPos -= 1;
			break;
		case SB_LINEDOWN://���¹�
			si.nPos += 1;
			break;
		case SB_PAGEUP://���Ϸ�ҳ
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN://���·�ҳ
			si.nPos += si.nPage;
			break;
		case SB_TOP://�ص�����
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM://�ص��׶�
			si.nPos = si.nMax;
			break;
		case SB_THUMBTRACK://�϶�������
			si.nPos = si.nTrackPos;
			break;
		}
		SetScrollInfo(hwnd, SB_VERT, &si, true);//�����µĹ�������Ϣ
		GetScrollInfo(hwnd, SB_VERT, &si);//��õ�ǰ�Ĺ�������Ϣ
		if (iVertPos != si.nPos)//������λ�ñ仯��
		{
			ScrollWindow(hwnd, 0, cyChar*(iVertPos - si.nPos), nullptr, nullptr);//���������ݽ��й���iVertPos-si.nPos�У���Ϊ���¹��������Ϲ�
		}
		break;
	case WM_HSCROLL:
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;//��ù�����������Ϣ
		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos= si.nPos;//��ǰˮƽ��������λ��
		switch (LOWORD(wParam))
		{
		case SB_LINELEFT://�����
			si.nPos -= 1;
			break;
		case SB_LINERIGHT://���ҹ�
			si.nPos += 1;
			break;
		case SB_PAGELEFT://����ҳ
			si.nPos -= si.nPage;
			break;
		case SB_PAGERIGHT://���ҷ�ҳ
			si.nPos += si.nPage;
			break;
		case SB_TOP://�ص��󶥶�
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM://�ص��Ҷ���
			si.nPos = si.nMax;
			break;
		case SB_THUMBPOSITION://��������ק
			si.nPos = si.nTrackPos;
			break;
		}
		SetScrollInfo(hwnd, SB_HORZ, &si, true);//�����µĹ�������Ϣ
		GetScrollInfo(hwnd, SB_HORZ, &si);//��õ�ǰ�Ĺ�������Ϣ
		if (iHorzPos != si.nPos)//������λ�ñ仯��
		{
			ScrollWindow(hwnd, cxChar*(iHorzPos - si.nPos), 0, nullptr, nullptr);//���������ݽ��й���iVertPos-si.nPos�У���Ϊ���ҹ����������
		}
		break;
	case WM_KEYDOWN://���̰���
		switch (wParam)
		{
		case VK_LEFT://�������
			SendMessage(hwnd, WM_HSCROLL, SB_LINELEFT, 0);//������Ϣ�����
			break;
		case VK_RIGHT://�������
			SendMessage(hwnd, WM_HSCROLL, SB_LINERIGHT, 0);//������Ϣ���ҹ�
			break;
		case VK_UP://�������
			SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);//������Ϣ���Ϲ�
			break;
		case VK_DOWN://�������
			SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);//������Ϣ���¹�
			break;
		case VK_HOME://HOME��
			SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0);//������Ϣ�ص�����
			break;
		case VK_END://END��
			SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0);//������Ϣ�ص��׶�
			break;
		case VK_PRIOR://PAGE UP��
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0);//������Ϣ���Ϸ�ҳ
			break;
		case VK_NEXT://PAGE DOWN ��
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0);//������Ϣ���·�ҳ
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (fileName.size() > 0)
		{
			SetMapMode(hdc,MM_ANISOTROPIC);//����ӳ��ģʽ���߼���λת���ɾ����������������ⵥλ
			SetWindowExtEx(hdc, 1, 1, nullptr);//�����߼���λ������1����λΪ���������һ���ַ���ȣ�������1����λΪ���������һ���ַ��߶�
			SetViewportExtEx(hdc, cxChar, cyChar, nullptr);

			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;//��ù�����������Ϣ
			GetScrollInfo(hwnd, SB_VERT, &si);//��ǰ��ֱ��������λ��
			iVertPos = si.nPos;
			GetScrollInfo(hwnd, SB_HORZ, &si);//��ǰˮƽ��������λ��
			iHorzPos = si.nPos;
			start = max(0, min(fileName.size()-1, iVertPos + ps.rcPaint.top/cyChar));//������������ps.rcPaint.top/cyCharҪ�ػ����ʼ�У�����iVertPos����ڹ������ϻ��Ƶ�λ��
			end = max(0, min(fileName.size()-1, iVertPos + ps.rcPaint.bottom/cyChar));//������������ps.rcPaint.bottom/cyCharҪ�ػ�Ľ����У�����iVertPos����ڹ������ϻ��Ƶ�λ��
			for (int i = start,j=0; i <= end; i++,j++)//�Թ����������ʧЧ�����ػ�
			{
				int x = -iHorzPos;//ˮƽ���Ƶ���ʼλ�ã����3����ʾ��3���ڿ�ͷ����-3��ʼ����
				int y = i - iVertPos;//��ֱ���Ƶ���ʼλ��
				StringCchPrintf(szBuffer, 128, TEXT("%3d%-10c"), i,':');//��ʽ��ID
				tstring temp = tstring(szBuffer) + fileName[i];//�ļ��������ַ���
				if (i == nSelectedLine)//�����ǰ�б�ѡ��
				{
					rect.left = 0;//Ҫ��ʾ�����ľ����������ѡ������
					rect.top = y;
					rect.right = cxClient / cxChar;
					rect.bottom = y + 1;
					FillRect(hdc, &rect, blueBrush);//����ɫ������ʾ
					SetBkColor(hdc, RGB(0, 0, 255));//�����ַ����ı�����ɫΪ��ɫ
					TextOut(hdc, x,y, temp.c_str(), temp.size());//��ʾ�ַ���
					SetBkColor(hdc, RGB(255,255, 255));//�ָ�ԭ���ĵ��ַ����ı�����ɫ��ɫ
				}
				else
				   TextOut(hdc, x,y, temp.c_str(), temp.size());//��ʾ�ַ���
			}
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������
		PostQuitMessage(0);//�׳�����ֵΪ0����Ϣ����GetMessage�е�ѭ�����г���
		KillTimer(hwnd, wallpaperID);//ɾ����ʱ��
		KillTimer(hwnd, tipID);
		DeleteObject(blueBrush);//ɾ����ˢ
		Shell_NotifyIcon(NIM_DELETE, &nid);//ɾ������ͼ��
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	WNDCLASSEX wndclass;//������
	wndclass.cbClsExtra = 0;//Ϊ�����Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.cbWndExtra = 0;//Ϊ���ڷ���Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//������ˢ
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//�����
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON3));//ͼ��
	wndclass.hInstance = hInstance;//ʵ�������������PID��һ���������������������ϵͳ�����ҵ����̵ĵ�ַ
	wndclass.lpfnWndProc = WndProc;//���ڹ���
	wndclass.lpszClassName = TEXT("zeng");//����
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);//�˵�
	wndclass.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;//��Щ�ı��Ҫ�ػ�������ˮƽ�ʹ�ֱ�ı���ػ�, CS_DBLCLKS �������˫����Ϣ
	wndclass.cbSize = sizeof(wndclass);//���ڴ�С 
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));//Сͼ��
	if (!RegisterClassEx(&wndclass))//ע�ᴰ����
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("wallpaper"), WS_OVERLAPPEDWINDOW ,
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
