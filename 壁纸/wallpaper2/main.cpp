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
	tstringstream out;//�ַ�����
	out << st.wYear << TEXT("��") << st.wMonth <<TEXT( "��") << st.wDay << TEXT("��,") << st.wHour <<TEXT( ":") << st.wMinute << TEXT(":") << st.wSecond;//��ʽ���ַ���
	return out.str();//���ظ�ʽ������ַ���
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
void FindFile(tstring CurrentPath,tstring type)//�����ļ�
{
	WIN32_FIND_DATA fd;
	string find;//ָ���ļ���Ҫ���ҵ����ͣ���d:\\wallpaper\\*.jpg
	find=CurrentPath +type;//��ǰҪ�����ļ���ʽ��·��
	HANDLE handle = FindFirstFile(find.c_str(), &fd);//���ҵ�1���ļ�
	if (handle != INVALID_HANDLE_VALUE)//�ҵ���
	{
		do
		{
			if (fd.cFileName[0] == '.')//win32��һ����������ļ��У�һ���ǡ�.��ָ���ǵ�ǰ�ļ��У�һ���ǡ�..��ָ�����ϲ��ļ��У����һ�־�����ͨ���²��ļ������ļ�������ʾ��cd .�Ǳ�Ŀ¼��cd ..���ϼ�Ŀ¼
				break;//����Ϊ�˲��ص��ϼ�Ŀ¼����ԭĿ¼��ת������.��..�����Ŀ¼
			fileName.push_back(fd.cFileName);//����ǰ�ļ�������
			filePath.push_back(CurrentPath);//����ǰ�ļ�·������
		} while (FindNextFile(handle, &fd));//������һ���ļ�
		FindClose(handle);//�ر��ļ����
	}
}
void FindAllFile(tstring CurrentPath, bool includeSubDirectory)//�������б�ֽ
{//CurrentPath��ǰ·��,includeSubDirectoryΪtrue����ʾ��ǰĿ¼����Ŀ¼�ı�ֽ��Ϊfalse��ֻ��ʾ��ǰĿ¼��ֽ
	WIN32_FIND_DATA fd;//fd�ļ���Ϣ�ṹ
	HANDLE handle;//handle�ļ�����
	string find;//ָ���ļ���Ҫ���ҵ����ͣ���d:\\wallpaper\\*.jpg
	for (int i = 0; i < format.size(); i++)//�������и�ʽ(jpg,bmp,png)��ͼƬ
		FindFile(CurrentPath,format[i]);//���ҵ�ǰ�ļ���ʽ��·��
	if (includeSubDirectory)//���Ҫ������Ŀ¼��ͼƬ
	{
		find = CurrentPath +TEXT("\\*.*");//���������ļ�
		handle = FindFirstFile(find.c_str(), &fd);//���ҵ�1���ļ�
		if (handle!= INVALID_HANDLE_VALUE)//�ҵ���
		{
			do
			{
				if (fd.cFileName[0] == '.')//win32��һ����������ļ��У�һ���ǡ�.��ָ���ǵ�ǰ�ļ��У�һ���ǡ�..��ָ�����ϲ��ļ��У����һ�־�����ͨ���²��ļ������ļ�������ʾ��cd .�Ǳ�Ŀ¼��cd ..���ϼ�Ŀ¼
					continue;//����Ϊ�˲��ص��ϼ�Ŀ¼����ԭĿ¼��ת������.��..�����Ŀ¼
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//ֻҪFILE_ATTRIBUTE_DIRECTORY��0x00000010����ֻҪ����������5λΪ1�������ļ�Ŀ¼������λ�Ǳ��ֻ����ֻд�����ص�����
					FindAllFile(CurrentPath+"\\"+tstring(fd.cFileName),includeSubDirectory);
			} while (FindNextFile(handle, &fd));//FindFirstFile����ʱָ����һ���ļ���������һ���ļ�
			FindClose(handle);//�ر��ļ����
		}
	}
}
int _stdcall EnumWindowProc(HWND hwnd, long lParam)//ö�ٺ����Ĵ��ڹ���
{
	TCHAR title[256];//���մ��ڱ���
	if (IsWindow(hwnd) && IsWindowVisible(hwnd) && !IsIconic(hwnd))
	{//�������������ڣ����Ҵ��ڿɼ���������С��
		GetWindowText(hwnd, title, 256);//�õ����ڱ���
		if (title[0] != '\0')//������ⲻΪ��
			allTitle.push_back(title);//�����������
		temp += tstring(title)+TEXT("   ");
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
void HighLineWallPaper(HWND hEdit, int  nSelectWallpaper)//�Ե�ǰ�ı�ֽ���и�����ʾ
{
	int lineIndex;//ָ�������ַ����ı��е�λ�ã����ֽ�����ʾ��
	int nextLineIndex;//ָ������һ�����ַ����ı��е�λ�ã����ֽ�����ʾ��
	lineIndex = SendMessage(hEdit, EM_LINEINDEX, nSelectWallpaper + 1, 0);//�õ�ָ�������ַ����ı��е�λ�ã����ֽ�����ʾ�������1�������ƣ���0��ͼ�ڵ�2�У����Ե�1��ͼ�ڵ�3��
	nextLineIndex = SendMessage(hEdit, EM_LINEINDEX, nSelectWallpaper + 2, 0);//�õ�ָ������һ�����ַ����ı��е�λ�ã����ֽ�����ʾ��,���1�������ƣ���0��ͼ�ڵ�2�У����Ե�1��ͼ�ڵ�3��
	SendMessage(hEdit, EM_SETSEL, lineIndex, nextLineIndex);//ѡ��ָ����
	SendMessage(hEdit, EM_SCROLLCARET, 0, 0);// �ѿɼ���Χ������괦,�����ù��󣬹����ܲ��ڷ�Χ�ڣ�����Ҫ��������괦
}
void DrawBorder(HWND hwnd, HBRUSH hBlueBrush)//������ɫ�߿�
{
	RECT rect;
	HDC hdc;
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);//�õ��ͻ�������
	rect.bottom -= 26;//�߿򲻰�Χ״̬������ȥ״̬���߶�
	FrameRect(hdc, &rect, hBlueBrush);//������ɫ��ˢ�߿�
	ReleaseDC(hwnd, hdc);
}
void FormatTime()//��ϵͳʱ����и�ʽ��
{
	SYSTEMTIME st;//ʱ��ṹ
	GetLocalTime(&st);//�õ�����ʱ��
	tstringstream out;//������ַ�������ʽ���ַ���
	out << TEXT("����ʱ��:") << st.wYear << TEXT("��") << st.wMonth << TEXT("��") << st.wDay << TEXT("  ") << st.wHour <<
		TEXT("Сʱ") << st.wMinute << TEXT("��") << st.wSecond << TEXT("��");
	statusInfo[1] = out.str();//�����е��ַ����������ַ�����
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
void DisplayDistribitue(HWND hEdit, const vector<tstring> &fileName)//��ʾ�ļ�����
{
	TCHAR szBuffer[1024];//�ַ�����
	WIN32_FILE_ATTRIBUTE_DATA attri;//�ļ����Խṹ
	tstring szPath;//�ļ�·��
	static TCHAR *szColumn[8] = {TEXT("����"),TEXT("�ļ���"),TEXT("��С"),TEXT("����"),TEXT("����ʱ��"),TEXT("���һ�η���ʱ��"),TEXT("���һ���޸�ʱ��"),TEXT("·��")};
	StringCchPrintf(szBuffer, 1024, TEXT("%-9s%-30s%-20s%-15s%-30s%-30s%-30s%-30s\r\n"),szColumn[0],szColumn[1],
		szColumn[2], szColumn[3], szColumn[4], szColumn[5], szColumn[6], szColumn[7]);//��ʽ������
	tstring temp = szBuffer;//�ڱ༭����ʾ���ַ���
	tstring index;//����
	for (int i = 0; i < fileName.size(); i++)
	{
		GetFileAttributesEx((filePath[i]+TEXT("\\")+fileName[i]).c_str(), GetFileExInfoStandard, &attri);//�õ���ǰ�ļ�������,GetFileExInfoStandard��ʾ��׼�ļ����ԣ����һ�������� WIN32_FILE_ATTRIBUTE_DATA�ṹ��ָ��
		fileCreateTime = FormatFileTime(attri.ftCreationTime);//�ļ�����ʱ��
		fileLastAccessTime = FormatFileTime(attri.ftLastAccessTime); // �ļ����һ�η���ʱ��
		fileLastWriteTime = FormatFileTime(attri.ftLastWriteTime);//�ļ����һ���޸�ʱ��
		fileAttributes = FileAttribute(attri.dwFileAttributes);//�ļ�����
		szPath =filePath[i].c_str();//�ļ�·��
		lstrcpyn(szName, fileName[i].c_str(),30);//�ļ���
		if (fileName[i].size()>29)//����ļ���̫��
			lstrcpyn(szName+24, TEXT("*.jpg"),6);
		fileSize = calculateCapacity(attri.nFileSizeHigh, attri.nFileSizeLow);//�����ļ���С
		index = ToString(i) + TEXT(":");//����������ת�����ַ���
		StringCchPrintf(szBuffer, 1024, TEXT("%-9s%-30s%-20s%-15s%-30s%-30s%-30s%-30s\r\n"), index.c_str(), szName, fileSize.c_str(),
		fileAttributes.c_str(), fileCreateTime.c_str(), fileLastAccessTime.c_str(), fileLastWriteTime.c_str(),
		szPath.c_str());//��ʽ���ַ���
		temp += szBuffer;
	}
	SetWindowText(hEdit, temp.c_str());//�ڱ༭����ʾ
}
HWND FindDialog(HWND hwnd,FINDREPLACE &fr ,TCHAR szFindText[])//���ҽṹ
{
	fr.Flags = FR_DOWN | FR_HIDEWHOLEWORD;//��ʼ�������滻�Ի���ı��
	//FR_DOWN��ʼ�����°�ť�����£�FR_HIDEWHOLEWORD����ʾȫ��ƥ��
	fr.hInstance = nullptr;
	fr.hwndOwner = hwnd;//ӵ�жԻ���Ĵ��ھ��
	fr.lCustData = 0;//�û����������
	fr.lpfnHook = 0;
	fr.lpstrFindWhat = szFindText;//Ҫ���ҵ��ַ���
	fr.lpstrReplaceWith = 0;//Ҫ�滻���ַ���
	fr.lpTemplateName = 0;
	fr.lStructSize = sizeof(fr);//�ṹ��С
	fr.wFindWhatLen = MAX_PATH;//Ҫ���ҵ��ַ���szFindText�Ļ������ռ�
	fr.wReplaceWithLen = 0;//Ҫ�滻���ַ���szFindText�Ļ������ռ�
	return FindText(&fr);//���в���
}
int _stdcall EditTextProc(HWND hEdit, unsigned message, unsigned wParam, long lPram)//��д�༭�ؼ����ڹ���
{
	int first;//��ǰ��������У����ַ��ڱ༭�е�λ��
	int startPos;//��ǰѡ�����ֵĿ�ʼλ�ã���δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������
	int endPos;//��ǰѡ�����ֵĽ���λ��+1����δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������
	switch (message)
	{
	case WM_KEYUP://�������ɿ���
	case WM_LBUTTONUP://���������ɿ���ʱ�򣬸���������Ϣ
		row = SendMessage(hEdit, EM_LINEFROMCHAR, -1, 0) + 1;//���ָ��λ�����ڵ��кţ�-1��ʾ��ǰ�������λ�ã���Ϊλ�õ�������0��ʼ������Ҫ��1
		statusInfo[2] = TEXT("�к�:") + ToString(row);//���к�ת�����ַ���
		SendMessage(hStatus, SB_SETTEXT, 2, (long)statusInfo[2].c_str());//�Եڶ��������к� 
		first = SendMessage(hEdit, EM_LINEINDEX, -1, 0);//��ǰ��������У����ַ��ڱ༭�е�λ��
		SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//��ǰѡ�����ֵĿ�ʼ������λ�ã���δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������			
		col = startPos - first + 1;//��ǰ����λ��-��ǰ��������е�1���ַ���λ��Ϊ����������Ϊλ�õ�������0��ʼ������Ҫ��1
		statusInfo[3] = TEXT("�к�:") + ToString(col);//���к�ת�����ַ���
		SendMessage(hStatus, SB_SETTEXT, 3, (long)statusInfo[3].c_str());//�Ե����������к� 
		nSelectWallpaper = row - 2;//��Ϊ��1���������ƣ���2���ǵ�0�ţ���3���ǵ�1�ţ����Ե�ǰѡ�е�������row-2��
		HighLineWallPaper(hEdit, nSelectWallpaper);//�Ե�ǰ�и�����ʾ������δ����Ϊ��ǰ��Ϊ��ֽ��ֻ�����˫��������
		break;
	case WM_LBUTTONDBLCLK:
		SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)(filePath[nSelectWallpaper] + TEXT("\\") + fileName[nSelectWallpaper]).c_str(), 0);//��ͼƬ�����ѡ1������Ϊ��ֽ
		statusInfo[0] = tstring("��ǰ��ֽ:") + filePath[nSelectWallpaper] + TEXT("\\") + fileName[nSelectWallpaper];
		SendMessage(hStatus, SB_SETTEXT, 0, (long)statusInfo[0].c_str());//��һ����ʾ·��
		break;
	case WM_RBUTTONDOWN://�Ҽ�����
		PopupMyMenu(hEdit, hMenu);//�����˵�
		break;
	}
	return CallWindowProc(oldEditTextProc, hEdit, message, wParam, lPram);//����Ĭ�ϵı༭�ؼ����ڴ�����̴���������Ϣ��δ������Ĳ���
}
void OKMessage(HWND hwnd, TCHAR szFormat[], TCHAR szFindText[])//����һ��OK��ʾ��
{
	TCHAR szBuffer[MAX_PATH];
	StringCchPrintf(szBuffer, MAX_PATH, szFormat, szFindText);//���и�ʽ���ַ���
	MessageBox(hwnd, szBuffer,TEXT("�Ҳ���"), MB_OK | MB_ICONEXCLAMATION);//������Ϣ��
}
bool MyFindText(HWND hEdit, FINDREPLACE &fr, bool bDown)//�����ı�
{
	int startPos;//��ǰѡ�����ֵĿ�ʼλ�ã���δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������
	int endPos;//��ǰѡ�����ֵĽ���λ��+1����δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������
	int len = GetWindowTextLength(hEdit);//��ñ༭���ַ�����
	TCHAR *pText = new TCHAR[len + 1];//����ռ�
	shared_ptr<TCHAR>sp1(pText, [](TCHAR *p){delete[]p; });//������Ŀռ佻����ָ�����	
	GetWindowText(hEdit, pText, len + 1);//��ñ༭���ַ�
	int findLen = lstrlen(fr.lpstrFindWhat);//Ҫ���ҵ��ַ�������
	int pos;//���ҵ���λ��
	SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//��ǰѡ�����ֵĿ�ʼλ�á���������δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������
	if (startPos != endPos&&bDown)//��ʼ����λ�ò�ͬ��˵��ѡ�����ֲ��������²���
		startPos++;//��ӵ�ǰѡ��λ�õ���һ��λ�ü�������,����ûѡ����ӵ�ǰλ�ÿ�ʼ����
	if (!(fr.Flags&FR_MATCHCASE))//������ִ�Сд��ťû������
	{
		for (int i = 0; i < findLen; i++)//��Ҫ���ҵ��ַ���ת��Сд
			fr.lpstrFindWhat[i] = tolower(fr.lpstrFindWhat[i]);
		for (int i = 0; i < len; i++)//���༭���е���ĸҲת��Сд�������ʹ�Сдһ��
			pText[i] = tolower(pText[i]);
	}
	if (bDown)//���²���
	{
		auto p = search(pText + startPos, pText + len, fr.lpstrFindWhat, fr.lpstrFindWhat + findLen);//���ҵ�һ��ƥ����ַ���λ��
		if (p == pText + len)//���²��Ҳ���
			return false;
		pos = p - pText;
	}
	else
	{
		auto p = find_end(pText, pText + startPos, fr.lpstrFindWhat, fr.lpstrFindWhat + findLen);//���Ҵ����һ��ƥ����ַ���λ��
		if (p == pText + startPos)//���ϲ��Ҳ���
			return false;
		pos = p - pText;
	}
	SendMessage(hEdit, EM_SETSEL, pos, pos + findLen);//ѡ�в��ҵ����ַ���
	SendMessage(hEdit, EM_SCROLLCARET, 0, 0);//�ѿɼ���Χ���ù�괦����������ѡ�е�λ��
	return true;
}
int _stdcall DlgGotoProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)//ת���Ի��򴰿ڹ���
{
	static HWND hEditGoto;//�༭��
	TCHAR strNum[24];//�༭���ϵ�����
	int lineIndex;//ָ�������ַ����ı��е�λ�ã����ֽ�����ʾ��
	static HWND hwnd;
	static HWND hEdit;//���±��༭����
	static int lineCount;//�༭��������
	switch (message)
	{
	case WM_INITDIALOG://��ʼ�Ի���
		hwnd = GetParent(hDlg);//�õ������ھ��
		hEdit = GetDlgItem(hwnd, ID_EDIT);
		lineCount = SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);
		hEditGoto = GetDlgItem(hDlg, IDC_EDITGOTO);//�õ�ת���༭����
		SetWindowText(hEditGoto, ToString(row).c_str());
		return true;//��Ϊ�ڶԻ�����Դֻ�Ա༭������WS_TABSTOP�����ﷵ��true���ͻ�ʹ�༭�������뽹�㣬���ֻ�ȫѡ
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_GOTO:
			GetWindowText(hEditGoto, strNum, sizeof(strNum));
			row = strToInt(strNum);//���ַ���ת��������
			if (row > lineCount)
			{
				SetWindowText(hEditGoto, ToString(lineCount).c_str());//��ת���༭����ʾ�༭��������
				SetFocus(hEditGoto);//��ת���༭���ý��㣬��Ϊ������ȫѡ��������ʾ������
				SendMessage(hEditGoto, EM_SETSEL, 0, -1);//ת���༭������ȫѡ
				MessageBox(hEditGoto, TEXT("���±�������������"), TEXT("���±� -����"), 0);
				break;
			}
			lineIndex = SendMessage(hEdit, EM_LINEINDEX, row - 1, 0);//�õ�ָ�������ַ����ı��е�λ�ã����ֽ�����ʾ����-1����Ϊ������0��ʼ
			SendMessage(hEdit, EM_SETSEL, lineIndex, lineIndex);//������Ƶ�ָ����
			SendMessage(hEdit, EM_SCROLLCARET, 0, 0);// �ѿɼ���Χ������괦,�����ù��󣬹����ܲ��ڷ�Χ�ڣ�����Ҫ��������괦
			SendMessage(hEdit, WM_LBUTTONUP, 0, 0);//����һ������ɿ���Ϣ�Ը����к�
		case IDC_CANCELGOTO:
			EndDialog(hDlg, 0);
			break;
		}
		return true;
	case WM_CLOSE:
		EndDialog(hDlg, 0);//�ر�ʱ���ٴ���,������DestroyWindow(hDlg)��Ϊ��ֻ�����ٴ��ڲ����˳��Ի������Ϣѭ������û������Ի���Ӧ�ó���ѿ���Ȩ����ϵͳ�������ڻ��ǲ��ܶ���
		//��EndDialog������ڴ������ٺ�����Ի�����Ϣ���У������ؿؼ�Ȩ��������
		return true;
	}
	return false;//���û�д�������0,�����Ĭ�϶Ի�����̣���0�����ˣ�������Ĭ�ϵ�
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//������ ���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ
	HDC hdc;//����
	RECT rect;//����
	int size;//�����С
	static NOTIFYICONDATA nid;//ϵͳ��Ҫ��������������������Ϣ����Ϣ
	static POINT mousePos;//���λ��
	static HBRUSH oldBrush;//����ɻ�ˢ
	static HBRUSH blueBrush;//��ɫ��ˢ��ѡ��ʱ������ʾ����ɫ
	static HINSTANCE hInst;
	static HFONT hFont;//����
	static BROWSEINFO bi;//�������û�ѡ��Ŀ¼����Ҫ��Ϣ
	static LPITEMIDLIST plDlist;//�����û�ѡ���Ŀ¼��Ϣ�ṹ
	static LPMALLOC pMalloc;//LPMALLOC��һ��IMallocָ�룬IMALLOCʵ����һ���ǳ��������ڴ�������棬�����ͷ�ITEMIDLISTָ����ָ��Ŀռ�
	static CHOOSEFONT cf;//����ṹ
	static CHOOSECOLOR cc;//��ɫ�ṹ
	static LOGFONT lf;//�߼�����
	static COLORREF textColor = RGB(0, 0, 0);//Ĭ�ϵ�������ɫ
	static COLORREF textBkColor=RGB(200,200,200);//Ĭ�ϵ����屳����ɫ���ɫ
	static HBRUSH hBKBrush;//������ˢ
	static COLORREF color[16];//��ɫ�Ի����б���16���Զ�����ɫ
	static HWND hEdit;//�༭��
	static bool bDown;//���°�ť�Ƿ���
	static TCHAR szFindText[MAX_PATH]=TEXT("");//Ҫ���ҵ��ַ���
	int xPos[4]{0, 0, 0, 0};//״̬�����ĸ��Һ�����
	static FINDREPLACE fr;//���Һ��滻�ṹ
	LPFINDREPLACE pfr = nullptr;//���Һ��滻�ṹ��ָ��
	static unsigned int WM_FIND;//�Զ������Ϣ�����ڽ��ղ��ҵ���Ϣ
	switch (UMsg)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;//����ʵ�����
		hBKBrush = CreateSolidBrush(textBkColor);//����������ˢ���ɫ
		InitFont(hwnd, cf, lf);//��ʼ������ṹ
		InitColor(hwnd, cc, color);//��ʼ����ɫ�ṹ
		InitNotifyIconData(hwnd, hInst, nid);//��ʼ������ͼ��ṹ
		WM_FIND = RegisterWindowMessage(FINDMSGSTRING);//���Һ��滻�Ի���ᷢ����Ϣ����FINDMSGSTRINGע�����Ϣ		
		Shell_NotifyIcon(NIM_ADD, &nid);//�������������һ��ͼ��
		hMenu = CreateMyPopMenu(hInst);//�����˵�
		FindAllFile(wallpaper,includeSubDirectory);//��������ͼƬ
		SetTimer(hwnd, wallpaperID, wallPaperTime, nullptr);//���ñ�ֽ��ʱ��
		SetTimer(hwnd, tipID, tipTime, nullptr);//����������ʾ��ʱ��
		SetTimer(hwnd,clockID,1000, nullptr);//����ʱ�Ӷ�ʱ����ÿһ�����ʱ��һ��
		blueBrush = CreateSolidBrush(RGB(102, 179, 255));//��ɫ��ˢ
		hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);//Ĭ������
		SHGetSpecialFolderLocation(hwnd, CSIDL_MYPICTURES, &plDlist);//����ҵ�ͼƬ�ļ���·��
		SHGetPathFromIDList(plDlist, szMyPictures);//��plDlist·�����ṹ�壩ת�����ַ���·��
		SHGetSpecialFolderLocation(hwnd, CSIDL_PERSONAL, &plDlist);//����ҵ��ĵ��ļ���·��
		SHGetPathFromIDList(plDlist, szMyDocument);//��plDlist·�����ṹ�壩ת�����ַ���·��
		if (SHGetMalloc(&pMalloc) == S_OK);//����IMALLOC����
		{
			pMalloc->Free(plDlist);//�ͷ�plDlistָ��Ŀռ�
			pMalloc->Release();//�ͷ�pMalloc�Ŀռ�
		}
		hStatus = CreateWindow(STATUSCLASSNAME, 0, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, hwnd, (HMENU)IDS_STATE, hInst, nullptr);//����״̬��
		hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL |ES_READONLY| ES_MULTILINE | ES_NOHIDESEL, 0, 0, 0, 0,
			hwnd, (HMENU)ID_EDIT, hInst, nullptr);//�����б߽�Ķ���ֻ���༭��ES_NOHIDESEL�ڱ༭��ʧȥ�����ѡ�е��ı���Ȼ���ָ�����ʾ
		SendMessage(hEdit, EM_LIMITTEXT, 0, 0);//��wParamΪ0ʱ�����б༭�ز����0x7FFFFFFE���ַ���Ĭ����30000���ַ�
		SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);
		oldEditTextProc = (WNDPROC)SetWindowLong(hEdit, GWL_WNDPROC, (long)EditTextProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ��̡������Ϣ����Ϊ
		//Ĭ���ǲ��񲻵���Щ��Ϣ�ģ���Ҫ�õ���Щ��Ϣ�����統���򰴼�����ʱ����״̬����ʾ��������Ϣ
		//���ǲ�����SetClassLong(hEdit,GCL_WNDPROC),��Ϊ�����޸�edit��Ĵ��ڹ��̣���ֻ�����һ�������ı༭����Ч���Ե�ǰ
		//�༭�򲻻�ı䣬������SetClassLong,��Ӱ���Ժ�ı༭�򣬶���ֻ��ı䵱ǰ�༭�򣬲���ı������༭��
		DisplayDistribitue(hEdit, fileName);//��ʾ�ļ�����
		srand(time(0));//����ǰʱ����Ϊ�����������
		break;
	case WM_SETFOCUS:
		SetFocus(hEdit);//�ñ༭���ý���
		HideCaret(hEdit);//�༭���й�꣬��Ϊ����Դ������Ϊֻ���������ù�����زűȽϺã�һ��Ҫ��setfoucs����������Ч����Ϊ��ʼ�������б༭�ػ�δ��ý��㣬��������
		//���ع����Ч��setfocus������ý����������ع�����Ч
		break;
	case WM_SIZE://��С�ı��
		cxClient = LOWORD(lParam);//�ͻ����Ŀ�ȡ��߶�
		cyClient = HIWORD(lParam);
		xPos[3] = -1;//���һ�������ұ߽�-1�����򵽿ͻ������ұ�
		xPos[2] = cxClient - 170;
		xPos[1] = cxClient - 250;
		xPos[0] = cxClient - 500;//��һ�������ұ߽�
		SendMessage(hStatus, SB_SETPARTS,4, (long)xPos);//�����ĸ������������
		if (IsIconic(hwnd))//���������С����ť
			ShowWindow(hwnd, SW_HIDE);//���ش���
		MoveWindow(hEdit, 1, 1, cxClient - 4, cyClient - 29, true);//-4��������Ȼ��߿�-29�ǰѱ߿���״̬�����棬״̬���߶�25����4����Ȼ��߿�
		MoveWindow(hStatus, 0, 0, 0, 0, true);//����״̬���ڿͻ����仯��������棬�������ô�Сλ�ã���Ϊ��Сλ�ù̶����ɱ�
		for (int i = 0; i <4; i++)
			SendMessage(hStatus, SB_SETTEXT, i, (long)statusInfo[i].c_str());//��ÿһ����ʾ�ı�
		if (nSelectWallpaper >= 0)//���������ĳ�ű�ֽ
			HighLineWallPaper(hEdit, nSelectWallpaper);//������ʾ
		break;
	case WM_CTLCOLORSTATIC://�༭���ʼ������
		hdc = (HDC)wParam;//wParam�Ǳ༭��Ļ��ʣ�lParam�Ǳ༭��ľ��
		SetBkColor(hdc, textBkColor);//�������屳����ɫ��Ӧ��༭�򱳾���ɫһ�£��������2�ֱ���ɫ���ÿ�
		SetTextColor(hdc, textColor);//�����������ɫ
		return (long)hBKBrush;//���ﲻ����break,�������������DefWindowProc������ɫ���Ĭ�ϵģ�����һ����ˢ���༭���Ĭ�ϴ��ڹ��̻��������ˢ��Ϊ����ɫ
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
				wallpaper = szDirectoryPath;
				fileName.clear();
				filePath.clear();
				FindAllFile(wallpaper,includeSubDirectory);
				SetWindowText(hEdit, 0);//��ձ༭��
				DisplayDistribitue(hEdit, fileName);//��ʾ�µ��ļ�����
				if (SHGetMalloc(&pMalloc) == S_OK);//����IMALLOC����
				{
					pMalloc->Free(plDlist);//�ͷ�plDlistָ��Ŀռ�
					pMalloc->Release();//�ͷ�pMalloc�Ŀռ�
				}
			}
			break;
		case IDM_EDIT_FIND: //����
			hDlgFind = FindDialog(hwnd,fr, szFindText);//�������ҶԻ������
			break;
		case IDM_EDIT_FINDNEXT://������һ��
			if (szFindText[0] == '\0')//�����û��Ҫ���ҵ��ַ���
				hDlgFind =FindDialog(hwnd,fr, szFindText);//�������ҶԻ�����������û������ַ�������
			else
			{
				if (!MyFindText(hEdit, fr, bDown))//������һ��
					OKMessage(hDlgFind, TEXT("�Ҳ���\"%s\""), fr.lpstrFindWhat);//���Ҳ���
			}
			return 0;
		case IDM_EDIT_GOTO: //ת��
			DialogBox(hInst, TEXT("IDD_DIALOGGOTO"), hwnd, DlgGotoProc);
			return 0;
		case IDM_FORMAT_FONT://����
			if (ChooseFont(&cf))//ѡ������
			{
				DeleteObject(hFont);//ɾ����һ�εĻ�ë
				hFont = CreateFontIndirect(&lf);//�����߼����崴��������
				SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);//�Ա༭������Ϣ�������»���
				InvalidateRect(hEdit, nullptr, true);//���µ�����������ʾ
			}
			break;
		case IDM_FONTCOLOR://������ɫ
			if (ChooseColor(&cc))//�ú�������һ����ʹ�û�����ѡ����ɫ��ͨ����ɫ�Ի���
			{
				textColor = cc.rgbResult;//ѡ���������ɫ
				InvalidateRect(hEdit, nullptr, true);//�༭���ػ�
			};
			break;
		case IDM_BKCOLOR://������ɫ
			if (ChooseColor(&cc))//�ú�������һ����ʹ�û�����ѡ����ɫ��ͨ����ɫ�Ի���
			{
				textBkColor = cc.rgbResult;//ѡ������屳����ɫ
				DeleteObject(hBKBrush);//ɾ���ɻ�ˢ
				hBKBrush = CreateSolidBrush(cc.rgbResult);//�ñ���ɫ�������»�ˢ
				InvalidateRect(hEdit, nullptr, true);//�༭���ػ�
			};
			break;
		}
	}
		break;
	case WM_TIMER://��ʱ����Ϣ
		switch (wParam)
		{
		case clockID://����ʱ��
			FormatTime();//��ϵͳʱ����и�ʽ��
			SendMessage(hStatus, SB_SETTEXT, 1, (long)statusInfo[1].c_str());//�Եڶ�������ʱ�� 
			break;
		case tipID://������ʾ��ʱ����
			Shell_NotifyIcon(NIM_MODIFY, &nid);//�޸����������һ��ͼ�꣬�ٴ���ʾ����
			break;
		case wallpaperID://����ֽ��ʾ����
			EnumWindows(EnumWindowProc, 0);//ö��������Ļ�ϵĶ��㴰�ڣ��������ھ�����͸�Ӧ�ó�����Ļص�����
			size = fileName.size();//�ļ��ĸ���
			{
				HWND hDestop = GetDesktopWindow();
				HDC  hdcScr = GetDCEx(hDestop, 0, DCX_LOCKWINDOWUPDATE);//�����Ļ�Ļ���,��ʹ��Ļ��סҲ��ˢ��
				//TextOut(hdcScr, 20, 400, temp.c_str(), temp.size());
			}
			if (size > 0)//ͼƬ������Ϊ0
			{
				//MessageBox(hwnd, ToString(nSelectWallpaper).c_str, 0, 0);
				if (changeByOnlyDestop)//�����������������ڲŻ���ֽ
				{
					if (allTitle.size() == 1)//���ֻ��1�����ڼ�����Window Manager����������ʾ
					{
						nSelectWallpaper = rand() % size;//���ѡһ��ͼƬ
						SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)(filePath[nSelectWallpaper] + TEXT("\\") + fileName[nSelectWallpaper]).c_str(), 0);//��ͼƬ�����ѡ1������Ϊ��ֽ
						statusInfo[0] = tstring("��ǰ��ֽ:") + filePath[nSelectWallpaper] + TEXT("\\") + fileName[nSelectWallpaper];
						SendMessage(hStatus, SB_SETTEXT, 0, (long)statusInfo[0].c_str());//��һ����ʾ·��
					}
				}
				else
				{
					nSelectWallpaper = rand() % size;//���ѡһ��ͼƬ
					SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)(filePath[nSelectWallpaper] + TEXT("\\") + fileName[nSelectWallpaper]).c_str(), 0);//��ͼƬ�����ѡ1������Ϊ��ֽ
					HighLineWallPaper(hEdit,nSelectWallpaper);//�Ե�ǰ��ֽ������ʾ
					statusInfo[0] = tstring("��ǰ��ֽ:") + filePath[nSelectWallpaper] + TEXT("\\") + fileName[nSelectWallpaper];
					SendMessage(hStatus, SB_SETTEXT, 0, (long)statusInfo[0].c_str());//��һ����ʾ·��
				}
			}
			temp.clear();
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
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
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
		KillTimer(hwnd,clockID);
		DeleteObject(blueBrush);//ɾ����ˢ
		DeleteObject(hFont);//ɾ������
		if (hBKBrush)
			DeleteObject(hBKBrush);//ɾ���ɻ�ˢ
		break;
	default:
		if (UMsg == WM_FIND)//���ܷ���case ����У���Ϊcase���Ҫ��������WM_FINDREPALCE���Լ������һ������
		{
			pfr = (LPFINDREPLACE)lParam;//���յ������Ϣ��lparam����������FINDREPLACE�ṹָ��
			if (pfr->Flags&FR_DIALOGTERM)//��������˹رհ�ť
				hDlgFind = 0;//�ԶԻ������ÿ�
			if (pfr->Flags&FR_DOWN)//���°�ť������
				bDown = true;
			else
				bDown = false;
			if (pfr->Flags&FR_FINDNEXT)//��������˲�����һ��
			{
				if (!MyFindText(hEdit, *pfr, bDown))//������һ��
					OKMessage(hDlgFind, TEXT("�Ҳ���\"%s\""), pfr->lpstrFindWhat);//���Ҳ���
			}
		}
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
