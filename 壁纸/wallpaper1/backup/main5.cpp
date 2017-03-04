#include"resource.h"
#include<shlobj.h>
tstring calculateCapacity(long fileSizeHigh, long fileSizeLow)//����������С
{
	DWORD64 size = 0;//������С
	LARGE_INTEGER  large;//�������ṹ
	large.LowPart = fileSizeLow;//��32λ
	large.HighPart = fileSizeHigh;//��32λ
	memcpy(&size, &large, sizeof(DWORD64));//���ļ���С��λ����λ������size�ϣ������ļ���С
	tstring calculate;//����������С���ַ���
	capacity c;//����������С�Ľṹ
	c.G = size / (1024 * 1024 * 1024);//��GB��С
	if (c.G)//��СGB����
		calculate += ToString(c.G) + TEXT("Gb,");
	size %= (1024 * 1024 * 1024);//��GBȡ��
	c.M = size / (1024 * 1024);//ʣ���MB��С
	if (c.M)//��СMB����
		calculate += ToString(c.M) + TEXT("Mb,");
	size %= (1024 * 1024);//��KBȡ��
	c.K = size / 1024;//ʣ���KB��С
	if (c.K)//��СKB����
		calculate += ToString(c.K) + TEXT("Kb,");
	size %= 1024;//��byteȡ��
	c.B = size;//ʣ���byte��С
	calculate += ToString(c.B) + TEXT("byte");
	return calculate;
}
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
	hBm[1] = LoadBitmap(hInst, TEXT("IDB_BITMAP2"));//��ȡλͼ
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
tstring FormatFileTime(FILETIME &ft)//��ʽ���ļ�ʱ��
{
	FILETIME temp;//�ļ�ʱ��ṹ
	FileTimeToLocalFileTime(&ft, &temp);//�������ļ�ʱ��ת���ɱ����ļ�ʱ��
	SYSTEMTIME st;//ʱ��ṹ
	FileTimeToSystemTime(&temp, &st);//�������ļ�ʱ��ת����ϵͳʱ��
	stringstream out;//�ַ�����
	out << st.wYear << "��" << st.wMonth << "��" << st.wDay << "��," << st.wHour << ":" << st.wMinute << ":" << st.wSecond;//��ʽ���ַ���
	return out.str();
}
tstring FileAttribute(long attribute)//�ļ�����
{
	tstring attri;
	if (attribute&FILE_ATTRIBUTE_ARCHIVE)
		attri += TEXT("�浵��");//һ���ļ�������֮��ϵͳ���Զ��������óɹ鵵���ԣ�������Գ������ļ��ı���
	if (attribute&FILE_ATTRIBUTE_COMPRESSED)
		attri += TEXT("ѹ����");
	if (attribute&FILE_ATTRIBUTE_DIRECTORY)
		attri += TEXT("Ŀ¼��");
	if (attribute&FILE_ATTRIBUTE_DEVICE)
		attri += TEXT("������");
	if (attribute&FILE_ATTRIBUTE_ENCRYPTED)
		attri += TEXT("���ܡ�");
	if (attribute&FILE_ATTRIBUTE_HIDDEN)
		attri += TEXT("���ء�");
	if (attribute&FILE_ATTRIBUTE_OFFLINE)
		attri += TEXT("�ѻ���");
	if (attribute&FILE_ATTRIBUTE_READONLY)
		attri += TEXT("ֻ��");
	if (attribute&FILE_ATTRIBUTE_SYSTEM)
		attri += TEXT("ϵͳ��");
	if (attribute&FILE_ATTRIBUTE_TEMPORARY)
		attri += TEXT("��ʱ��");
	if (attribute&FILE_ATTRIBUTE_NORMAL)
		attri += TEXT("��ͨ");
	attri += TEXT("�ļ�");
	return attri;
}
void FindFile(const tstring &wallpaper)//�����ļ�
{
	WIN32_FIND_DATA fd;
	HANDLE handle = FindFirstFile(wallpaper.c_str(), &fd);//���ҵ�1���ļ�
	if (handle != INVALID_HANDLE_VALUE)//�ҵ���
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
void InitNotifyIconData(HWND hwnd, HINSTANCE hInst, NOTIFYICONDATA &nid)//��ʼ�����̽ṹ
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
void PopupMyMenu(HWND hwnd, HMENU hMenu)//�����˵�
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
//ѡ���ļ��жԻ���ص�����  
int CALLBACK BrowseCallBackFun(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szPath[MAX_PATH];//����ѡ���·��
	switch (uMsg)
	{
	case BFFM_INITIALIZED://ѡ���ļ��жԻ����ʼ�� 
		SendMessage(hwnd, BFFM_SETSELECTION, true, lpData);//��BROWSEINFO�е�lParam������Ĭ��·����lpData�У���wParam��Ϊtrue,lParam��Ϊ��'\0'Ϊβ�ַ����������Ĭ�ϴ�Ŀ¼
		SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, lpData);//����״̬�����ı����ҵ�ͼƬ·��
		SetWindowText(hwnd, TEXT("ѡ��ͼƬ�ļ���"));
		break;
	case BFFM_SELCHANGED://ѡ�������ļ��к�
		SHGetPathFromIDList((LPCITEMIDLIST)lParam, szPath);//lParam������һ��LPCITEMIDLIST�ṹ��ת�����ַ�����·��
		SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (long)szPath);//����״̬�����ı�����·��
		break;
	}
	return false;
}
void DrawBorder(HWND hwnd, HBRUSH hBlueBrush)//������ɫ�߿�
{
	RECT rect;
	HDC hdc;
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);//�õ��ͻ�������
	FrameRect(hdc, &rect, hBlueBrush);//������ɫ��ˢ�߿�
	ReleaseDC(hwnd, hdc);
}
void InitBrowseInfo(HWND hwnd, BROWSEINFO &bi)// ��ʼ����Ŀ¼�ṹ
{
	bi.hwndOwner = hwnd;//����ļ��жԻ���ĸ�������
	bi.iImage = 0;// ��ѡ����ļ��е�ͼƬ���
	bi.lParam = (long)szMyPictures;//�Ի��򴫵ݸ��ص�������һ������ָ��,�����ص������Ĳ���,����Ĭ��·��Ϊ�ҵ�ͼƬ�ļ���
	bi.lpfn = BrowseCallBackFun;//Ӧ�ó����������Ի���ص������ĵ�ַ�����Ի����е��¼�����ʱ���öԻ��򽫵��ûص��������ò�������ΪNULL
	bi.lpszTitle = TEXT("�����ˣ��ҵ���ܾ��ˣ���ѡ��һ��ͼƬ�ļ�����");//������ļ��жԻ������ʾ�ı���������ʾ������ļ��жԻ���Ĺ��ܡ����ú�Ŀ��
	bi.pidlRoot = 0;// ITEMIDLIST�ṹ�ĵ�ַ���������ʱ�ĳ�ʼ��Ŀ¼������ֻ�б�ָ����Ŀ¼������Ŀ¼����ʾ������ļ��жԻ����С��ó�Ա����������NULL���ڴ�ʱ����Ŀ¼����ʹ��
	bi.pszDisplayName = szDirectoryName;//���������û�ѡ�е�Ŀ¼�ַ������ڴ��ַ���û������Ĵ�Сȱʡ�Ƕ����MAX_PATH������
	bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS;//�ñ�־λ�����˶Ի����ѡ�������Ϊ0
	//BIF_EDITBOX������Ի����а���һ���༭���ڸñ༭�����û���������ѡ���������
	//BIF_STATUSTEXT���ڶԻ����а���һ��״̬����ͨ�����Ի�������Ϣʹ�ص���������״̬�ı�
	//BIF_RETURNONLYFSDIRS�����������ļ�ϵͳ��Ŀ¼�����磺������ļ��жԻ����У���ѡ������һ��Ŀ¼ʱ���á�OK����ť���ã�����ѡ�С��ҵĵ��ԡ��������ھӡ��ȷ�������Ľڵ�ʱ����OK����ťΪ��ɫ
}
void SetScroll(HWND hwnd, SCROLLINFO &si)//���ù�����
{
	si.cbSize = sizeof(si);//�ṹ��С
	si.fMask = SIF_PAGE | SIF_RANGE;//���õı�ǣ�ֻ����SIF_PAGE��SIF_RANGE��ʹ��SIF_PAGE��־�ܹ�ȡ��ҳ���С��SIF_RANGE��־ʱ�������nMin��nMax�ֶ��趨Ϊ����ľ��з�Χ
	si.nMin = 0;
	si.nMax = fileName.size();//���Χ�����ֵĸ���,����fileName.size()-1����ΪҪ������1�е�����
	si.nPage = nPageLine;//һҳ�ĵ�����
	SetScrollInfo(hwnd, SB_VERT, &si, true);//���ô�ֱ����������
	si.nMax = 222;//ˮƽ���������Χ��һ������ֵ��ַ���������Լ����
	si.nPage = cxClient / cxChar;//����һҳ����
	SetScrollInfo(hwnd, SB_HORZ, &si, true);//����ˮƽ����������
}
void GetCharSize(HWND hwnd,HFONT hFont)//�õ�ָ��������ַ���С
{
	TEXTMETRIC tm;//�ı��ߴ�ṹ
	HDC hdc;
	hdc = GetDC(hwnd);//�õ����ڻ���
	SelectObject(hdc, hFont);
	GetTextMetrics(hdc, &tm);//�õ����ʵ��ı��ߴ�
	cxChar = tm.tmAveCharWidth;//Сд��ĸ��ƽ�����
	cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;//��д��ĸ��ƽ�����
	cyChar = tm.tmHeight + tm.tmExternalLeading;//��ĸ��ƽ���߶�
	ReleaseDC(hwnd, hdc);
}
void InitColor(HWND hwnd, CHOOSECOLOR &cc, COLORREF color[])//��ʼ����ɫ�ṹ
{
	cc.lStructSize = sizeof(cc);//ָ���ṹ�ĳ��ȣ��ֽ�
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;//һ�����������ʼ����ɫ�Ի����λ�������Ի��򷵻�ʱ����������Щ��ʶ������ʶ�û�������
	//CC_FULLOPEN�öԻ�����ʾ����Ŀؼ���ʹ�û������Զ������ɫ
	//CC_RGBINIT�öԻ���Ĭ��ʹ����rgbResult��Աָ������ɫ
	cc.hInstance = nullptr;
	cc.hwndOwner = hwnd;//ӵ�жԻ���Ĵ��ڵľ�����ó�Ա������������Ч�Ĵ��ھ�������ڶԻ���û��������ʱ����ΪNULL
	cc.lCustData = 0;//ָ��Ӧ�ó����Զ�������ݣ������ݻᱻϵͳ���͸����ӳ���
	cc.lpfnHook = 0;//ָ��CCHookProc���ӳ����ָ�룬�ù��ӿ��Դ����͸��Ի������Ϣ
	cc.lpTemplateName = 0;//ָ��һ��NULL��β���ַ��������ַ����ǶԻ���ģ����Դ������
	cc.rgbResult = RGB(255, 0, 0);//���CC_RGBINIT��ʶ��������ʱ���ó�Աָ���˶Ի����ʱĬ�ϵ�ѡ����ɫ
	cc.lpCustColors = color;//ָ��һ������16��ֵ�����飬����������˶Ի������Զ�����ɫ�ĺ졢�̡�����RGB��ֵ
}
void InitFont(HWND hwnd, CHOOSEFONT &cf, LOGFONT &lf)//��ʼ������ṹ
{
	cf.Flags = CF_EFFECTS | CF_NOVERTFONTS;//ʹ�Ի�����ʾ�ؼ������û�ָ���»���,���ı���ɫѡ� ������ô˱�־,������ʹ��rgbColors��Աָ����ʼ�ı���ɫ
	// CF_NOVERTFONTS��������Ի����б�ֻ����ˮƽ������,��δ�������Ǵ�ֱ��
	cf.hDC = nullptr;
	cf.hInstance = nullptr;
	cf.hwndOwner = hwnd;
	cf.iPointSize = 0;//ѡ������Ĵ�С
	cf.lCustData = 0;
	cf.lpfnHook = 0;
	cf.lpLogFont = &lf;//�߼�����
	cf.lStructSize = sizeof(cf);
	cf.lpszStyle = 0;//������
	cf.lpTemplateName = 0;
	cf.nFontType = 0;//���ص���������
	cf.nSizeMax = 0;//�û�����ѡ�������С
	cf.nSizeMin = 0;// �û�����ѡ����С���С
	cf.rgbColors = RGB(255, 0, 0);//��ʼ����Ի����ı���ɫ
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//������ ���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ
	HDC hdc;//����
	RECT rect;//����
	static tstring wallpaper = TEXT("E:\\��ֽ\\SogouWP\\Local\\WallPaper");//��ֽ·��
	tstring format = TEXT("\\*.jpg");//��ֽ��׺��ʽ
	int size;//�����С
	static SCROLLINFO si;//��������Ϣ
	static int iVertPos, iHorzPos;//ˮƽ�ʹ�ֱ������λ��
	int start, end;//����������ʱ���ػ����ʼ�ͽ���λ��
	TCHAR szBuffer[1024];//�ַ�����
	static int iPerLine;//����һ�еĿ̶�
	static int iAccumLine;//�����Ŀ̶�
	static int ulScrollLines;//�������һ�ι���������
	static NOTIFYICONDATA nid;//ϵͳ��Ҫ��������������������Ϣ����Ϣ
	static POINT mousePos;//���λ��
	static int nSelectedLine;//��ǰ��ѡ�е���,������0��ʼ
	static int nLastSelectedWallpapaer;//��һ�α�ѡ�еı�ֽ
	static int nSelectWallpaper;//��ѡ�еı�ֽ
	static HBRUSH oldBrush;//����ɻ�ˢ
	static HBRUSH blueBrush;//��ɫ��ˢ��ѡ��ʱ������ʾ����ɫ
	static HINSTANCE hInst;
	static HMENU hMenu;//�˵����
	static WIN32_FILE_ATTRIBUTE_DATA attri;//�ļ����Խṹ
	static HFONT hFont;//����
	static BROWSEINFO bi;//�������û�ѡ��Ŀ¼����Ҫ��Ϣ
	static LPITEMIDLIST plDlist;//�����û�ѡ���Ŀ¼��Ϣ�ṹ
	static LPMALLOC pMalloc;//LPMALLOC��һ��IMallocָ�룬IMALLOCʵ����һ���ǳ��������ڴ�������棬�����ͷ�ITEMIDLISTָ����ָ��Ŀռ�
	static CHOOSEFONT cf;//����ṹ
	static CHOOSECOLOR cc;//��ɫ�ṹ
	static LOGFONT lf;//�߼�����
	static COLORREF textColor = RGB(0, 0, 0);//Ĭ�ϵ�������ɫ
	static COLORREF textBkColor = RGB(255, 255, 255);//Ĭ�ϵ����屳����ɫ
	static HBRUSH hBKBrush;//������ˢ
	static COLORREF color[16];//��ɫ�Ի����б���16���Զ�����ɫ
	switch (UMsg)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;//����ʵ�����
		hBKBrush = CreateSolidBrush(textBkColor);//����������ˢ
		InitFont(hwnd, cf, lf);//��ʼ������ṹ
		InitColor(hwnd, cc, color);//��ʼ����ɫ�ṹ
		InitNotifyIconData(hwnd, hInst, nid);//��ʼ������ͼ��ṹ
		Shell_NotifyIcon(NIM_ADD, &nid);//�������������һ��ͼ��
		hMenu = CreateMyPopMenu(hInst);//�����˵�
		FindFile(wallpaper + format);//��������ͼƬ
		SetTimer(hwnd, wallpaperID, wallPaperTime, nullptr);//���ñ�ֽ��ʱ��
		SetTimer(hwnd, tipID, tipTime, nullptr);//����������ʾ��ʱ��
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ulScrollLines, 0);//��ȡ�������һ�ι���������
		if (ulScrollLines)
			iPerLine = WHEEL_DELTA / ulScrollLines;//�������һ�еĿ̶�
		else
			iPerLine = 0;
		srand(time(0));//����ǰʱ����Ϊ�����������
		blueBrush = CreateSolidBrush(RGB(102, 179, 255));//��ɫ��ˢ
		hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);//Ĭ������
		GetCharSize(hwnd, hFont);//�õ�ָ��������ַ���С
		SHGetSpecialFolderLocation(hwnd, CSIDL_MYPICTURES, &plDlist);//����ҵ�ͼƬ�ļ���·��
		SHGetPathFromIDList(plDlist, szMyPictures);//��plDlist·�����ṹ�壩ת�����ַ���·��
		SHGetSpecialFolderLocation(hwnd, CSIDL_PERSONAL, &plDlist);//����ҵ��ĵ��ļ���·��
		SHGetPathFromIDList(plDlist, szMyDocument);//��plDlist·�����ṹ�壩ת�����ַ���·��
		if (SHGetMalloc(&pMalloc) == S_OK);//����IMALLOC����
		{
			pMalloc->Free(plDlist);//�ͷ�plDlistָ��Ŀռ�
			pMalloc->Release();//�ͷ�pMalloc�Ŀռ�
		}
		break;
	case WM_SIZE://��С�ı��
		cxClient = LOWORD(lParam);//�ͻ����Ŀ�ȡ��߶�
		cyClient = HIWORD(lParam);
		nPageLine = cyClient / cyChar;//���һҳ������
		SetScroll(hwnd, si);//���ù�����
		if (IsIconic(hwnd))//���������С����ť
			ShowWindow(hwnd, SW_HIDE);//���ش���
		break;
	case WM_MOUSEWHEEL://������
		if (ulScrollLines == 0)//��겻����������˳�
			break;
		iAccumLine += (SHORT)HIWORD(wParam);//���Ϲ����Ŀ̶�
		while (iAccumLine >= iPerLine)//�����������һ�еĿ̶�,������¹����������Ϲ�
		{
			iAccumLine -= iPerLine;//��ȥһ�еĿ̶�
			SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, lParam);//������Ϣ���������Ϲ���һ��
		}
		while (iAccumLine <= -iPerLine)//�����������һ�еĿ̶ȣ�������Ϲ����������¹�
		{
			iAccumLine += iPerLine;//����һ�еĿ̶�
			SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, lParam);//�������¹���
		}
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);//�ؼ�ID
		int wmEvent = HIWORD(wParam);//�ؼ��ϵĲ���
		switch (wmId)
		{
		case ID_ADDWAPPER://��ӱ�ֽĿ¼
			InitBrowseInfo(hwnd, bi);//��ʼ��ѡ��Ŀ¼�ṹ
			plDlist = SHBrowseForFolder(&bi);//����ѡ��Ŀ¼�Ի���,·�������ָ��plDlist
			if (plDlist)//���ѡ����һ��Ŀ¼
			{
				SHGetPathFromIDList(plDlist, szDirectoryPath);//��plDlist·�����ṹ�壩ת�����ַ���·��				
				//MessageBox(hwnd, szDirectoryPath, 0, 0);
				wallpaper = szDirectoryPath;
				fileName.clear();
				FindFile(wallpaper+format);
				SetScroll(hwnd, si);
				if (SHGetMalloc(&pMalloc) == S_OK);//����IMALLOC����
				{
					pMalloc->Free(plDlist);//�ͷ�plDlistָ��Ŀռ�
					pMalloc->Release();//�ͷ�pMalloc�Ŀռ�
				}
			}
			break;
		case IDM_FORMAT_FONT://����
			if (ChooseFont(&cf))//ѡ������
			{
				DeleteObject(hFont);//ɾ����һ�εĻ�ë
				hFont = 0;
				//hFont = CreateFontIndirect(&lf);//�����߼����崴��������
				hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0,lf.lfFaceName);//Ĭ������
				GetCharSize(hwnd, hFont);//�����������ַ���С
				InvalidateRect(hwnd, nullptr, true);//���µ�����������ʾ
			}
			break;
		case IDM_FONTCOLOR://������ɫ
			if (ChooseColor(&cc))//�ú�������һ����ʹ�û�����ѡ����ɫ��ͨ����ɫ�Ի���
			{
				textColor = cc.rgbResult;//ѡ���������ɫ
				InvalidateRect(hwnd, nullptr, true);//���µ���ɫ������ʾ
			};
			break;
		case IDM_BKCOLOR://������ɫ
			if (ChooseColor(&cc))//�ú�������һ����ʹ�û�����ѡ����ɫ��ͨ����ɫ�Ի���
			{
				textBkColor = cc.rgbResult;//ѡ������屳����ɫ
				if (hBKBrush)
				DeleteObject(hBKBrush);//ɾ���ɻ�ˢ
				hBKBrush = CreateSolidBrush(cc.rgbResult);//�ñ���ɫ�������»�ˢ
				InvalidateRect(hwnd, nullptr, true);//���µ���ɫ������ʾ
			};
			break;
		}
	}
		break;
	case WM_LBUTTONDBLCLK://���˫��
		//MessageBox(hwnd, TEXT(" double mouse"), 0, 0);
		break;
	case WM_LBUTTONDOWN://��굥��
		mousePos.x = GET_X_LPARAM(lParam);//������굱ǰ���ڵ�λ�ã��ᡢ������)
		mousePos.y = GET_Y_LPARAM(lParam);
		nSelectedLine = mousePos.y / cyChar;//������굱ǰ�����е���
		if (nSelectedLine >= 1 && nSelectedLine < nPageLine)//�������ڿͻ����ﲢ�Ҳ��ǵ�1��,����Ϊ��1��������
		{
			nSelectWallpaper = nSelectedLine - 1 + iVertPos;//nSelectedLine���ϵ�ǰ��ֱ��������λ�ã�����ǰ��ѡ�еı�ֽ��-1��Ϊ������0��ʼ
			InvalidateRect(hwnd, 0, true);//��ѡ���н����ػ������и�����ʾ
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
			if (allTitle.size() < 2 && size>0)//���ֻ��1�����ڼ�����Window Manager����������ʾ����ͼƬ������Ϊ0
			{
				SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)(wallpaper +TEXT("\\")+ fileName[rand() % size]).c_str(), 0);//��ͼƬ�����ѡ1������Ϊ��ֽ
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
			InvalidateRect(hwnd, 0, true);
		break;
	case WM_HSCROLL:
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;//��ù�����������Ϣ
		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos = si.nPos;//��ǰˮƽ��������λ��
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
			InvalidateRect(hwnd, 0, true);
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
		SelectObject(hdc, hFont);//��������
		SetTextColor(hdc, textColor);//����������ɫ
		GetClientRect(hwnd, &rect);
		FillRect(hdc, &rect, hBKBrush);
		if (fileName.size() > 0)
		{
			SetMapMode(hdc, MM_ANISOTROPIC);//����ӳ��ģʽ���߼���λת���ɾ����������������ⵥλ
			SetWindowExtEx(hdc, 1, 1, nullptr);//�����߼���λ������1����λΪ���������һ���ַ���ȣ�������1����λΪ���������һ���ַ��߶�
			SetViewportExtEx(hdc, cxChar, cyChar, nullptr);
			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;//��ù�����������Ϣ
			GetScrollInfo(hwnd, SB_VERT, &si);//��ǰ��ֱ��������λ��
			iVertPos = si.nPos;
			GetScrollInfo(hwnd, SB_HORZ, &si);//��ǰˮƽ��������λ��
			iHorzPos = si.nPos;
			start = max(0, min(fileName.size(), iVertPos + ps.rcPaint.top / cyChar));//������������ps.rcPaint.top/cyCharҪ�ػ����ʼ�У�����iVertPos����ڹ������ϻ��Ƶ�λ��
			end = max(0, min(fileName.size(), iVertPos + ps.rcPaint.bottom / cyChar));//������������ps.rcPaint.bottom/cyCharҪ�ػ�Ľ����У�����iVertPos����ڹ������ϻ��Ƶ�λ��
			TextOut(hdc, -iHorzPos, 0, szTop, lstrlen(szTop));//��ʾ����
			for (int i = start, j = 0; i <end; i++, j++)//�Թ����������ʧЧ�����ػ�
			{
				int x = -iHorzPos;//ˮƽ���Ƶ���ʼλ�ã����3����ʾ��3���ڿ�ͷ����-3��ʼ����
				int y = i - iVertPos + 1;//��ֱ���Ƶ���ʼλ��,����1����ΪҪ��������1�е�����
				GetFileAttributesEx((wallpaper +TEXT("\\")+ fileName[i]).c_str(), GetFileExInfoStandard, &attri);//�õ���ǰ�ļ�������,GetFileExInfoStandard��ʾ��׼�ļ����ԣ����һ�������� WIN32_FILE_ATTRIBUTE_DATA�ṹ��ָ��
				fileCreateTime = FormatFileTime(attri.ftCreationTime);//�ļ�����ʱ��
				fileLastAccessTime = FormatFileTime(attri.ftLastAccessTime); // �ļ����һ�η���ʱ��
				fileLastWriteTime = FormatFileTime(attri.ftLastWriteTime);//�ļ����һ���޸�ʱ��
				fileAttributes = FileAttribute(attri.dwFileAttributes);//�ļ�����
				filePath = wallpaper;//�ļ�·��
				lstrcpyn(szName, fileName[i].c_str(),30);//�ļ���
				if (fileName[i].size()>29)//����ļ���̫��
					lstrcpyn(szName+24, TEXT("*.jpg"),6);
				fileSize = calculateCapacity(attri.nFileSizeHigh, attri.nFileSizeLow);//�����ļ���С
				StringCchPrintf(szBuffer, 1024, TEXT("%3d%-5c%-30s%-20s%-15s%-30s%-30s%-30s%-30s"), i, ':', szName, fileSize.c_str(),
					fileAttributes.c_str(), fileCreateTime.c_str(), fileLastAccessTime.c_str(), fileLastWriteTime.c_str(),
					filePath.c_str());//��ʽ���ַ���
				if (i == nSelectWallpaper)//�����ǰ�б�ѡ��
				{
					rect.left = 0;//Ҫ��ʾ�����ľ����������ѡ������
					rect.top = y;
					rect.right = cxClient / cxChar;
					rect.bottom = y + 1;
					FillRect(hdc, &rect, blueBrush);//����ɫ������ʾ
					SetBkColor(hdc, RGB(0, 0, 255));//�����ַ����ı�����ɫΪ��ɫ
					TextOut(hdc, x, y, szBuffer, lstrlen(szBuffer));//��ʾ�ַ���
					SetBkColor(hdc, RGB(255, 255, 255));//�ָ�ԭ���ĵ��ַ����ı�����ɫ��ɫ
				}
				else
					TextOut(hdc, x, y, szBuffer, lstrlen(szBuffer));//��ʾ�ַ���
			}
		}
		EndPaint(hwnd, &ps);
		DrawBorder(hwnd, blueBrush);//���Ʊ߿�
		break;
	case WM_CLOSE://���ڹر�
		Shell_NotifyIcon(NIM_DELETE, &nid);//ɾ������ͼ��
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������
		PostQuitMessage(0);//�׳�����ֵΪ0����Ϣ����GetMessage�е�ѭ�����г���
		KillTimer(hwnd, wallpaperID);//ɾ����ʱ��
		KillTimer(hwnd, tipID);
		DeleteObject(blueBrush);//ɾ����ˢ
		DeleteObject(hFont);//ɾ������
		if (hBKBrush)
			DeleteObject(hBKBrush);//ɾ���ɻ�ˢ
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
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("wallpaper"), WS_OVERLAPPEDWINDOW,
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
