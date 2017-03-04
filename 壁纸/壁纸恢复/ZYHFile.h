#pragma once
#include<shlobj.h>
#include<Shlwapi.h>
#include<strsafe.h>
#include<string>
#include<sstream>
#ifndef UNICODE 
# define  tstring string 
# define ToString to_string
# define tstringstream stringstream
# define strToInt atoi
#else
#define   tstring wstring
# define ToString to_wstring
# define tstringstream wstringstream
# define strToInt _wtoi
#endif

#pragma comment(lib,"Shlwapi.lib")
TCHAR szMyPictures[MAX_PATH];//�������ҵ�ͼƬ�ļ���·��
TCHAR szMyDocument[MAX_PATH];//�������ҵ��ĵ��ļ���·��
TCHAR szDirectoryName[MAX_PATH];//�û�ѡ���Ŀ¼��
LPITEMIDLIST plDlist;//�����û�ѡ���Ŀ¼��Ϣ�ṹ
LPMALLOC pMalloc;//LPMALLOC��һ��IMallocָ�룬IMALLOCʵ����һ���ǳ��������ڴ�������棬�����ͷ�ITEMIDLISTָ����ָ��Ŀռ�
BROWSEINFO bi;//�������û�ѡ��Ŀ¼����Ҫ��Ϣ


//�õ��ҵ��ĵ����ҵ�ͼƬ·��
void GetPictureAndDocumentPath(HWND hwnd)
{
	SHGetSpecialFolderLocation(hwnd, CSIDL_MYPICTURES, &plDlist);//����ҵ�ͼƬ�ļ���·��
	SHGetPathFromIDList(plDlist, szMyPictures);//��plDlist·�����ṹ�壩ת�����ַ���·��
	SHGetSpecialFolderLocation(hwnd, CSIDL_PERSONAL, &plDlist);//����ҵ��ĵ��ļ���·��
	SHGetPathFromIDList(plDlist, szMyDocument);//��plDlist·�����ṹ�壩ת�����ַ���·��
	if (SHGetMalloc(&pMalloc) == S_OK)//����IMALLOC����
	{
		pMalloc->Free(plDlist);//�ͷ�plDlistָ��Ŀռ�
		pMalloc->Release();//�ͷ�pMalloc�Ŀռ�
	}
}
//���ҵ��ĵ������浵Ŀ¼,����ֵtrue��ʾ���и�Ŀ¼
bool CreateArchiveDirectory(HWND hwnd, TCHAR*archiveName)
{
	if (!archiveName)
		return false;
	if (szMyDocument[0] == '\0' || szMyDocument == nullptr)
		GetPictureAndDocumentPath(hwnd);//�ҵ��ĵ�Ϊ�գ����л�ȡ
	WIN32_FIND_DATA fd;//�ļ��ṹ
	tstring find = tstring(szMyDocument) +tstring(TEXT("\\"))+ archiveName;
	HANDLE handle = FindFirstFile(find.c_str(), &fd);//���ҵ�1���ļ�
	if (handle == INVALID_HANDLE_VALUE || (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)//����Ҳ����ļ���wallpaper�����ҵ��Ĳ���Ŀ¼
	{
		SetCurrentDirectory(szMyDocument);//���õ�ǰĿ¼
		CreateDirectory(archiveName, 0);//����Ŀ¼
		return false;
	}
	return true;
}
//���ؿ�ִ���ļ���Ŀ¼
void GetAppDirectory(char *modulePath)
{
	GetModuleFileNameA(NULL, modulePath, MAX_PATH);//�õ���ִ���ļ�·��
	char *separator = strrchr(modulePath, TEXT('\\'));
	*separator = TEXT('\0');
}
typedef struct
{
	unsigned int G, M, K, B;//�ֱ����GB,MB,KB,BYTE
}capacity;//�����ṹ

//�����ļ�������С
tstring calculateCapacity(long fileSizeHigh, long fileSizeLow)
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
//�ļ��Ƿ�����Ҫ���СС
bool isLessThanMiniSize(long fileSizeHigh, long fileSizeLow,UINT miniSize )
{
	DWORD64 size = 0;//������С
	LARGE_INTEGER  large;//�������ṹ
	large.LowPart = fileSizeLow;//��32λ
	large.HighPart = fileSizeHigh;//��32λ
	memcpy(&size, &large, sizeof(DWORD64));//���ļ���С��λ����λ������size�ϣ������ļ���С
	if (size < miniSize * 1024)
		return true;
	else
		return false;
}
//��õ�ǰĿ¼���̷�
char getDriveLetter(){
	char drive = '\0';
	TCHAR current[MAX_PATH];

	::GetCurrentDirectory(MAX_PATH, current);
	int driveNbr = ::PathGetDriveNumber(current);
	if (driveNbr != -1)
		drive = 'A' + char(driveNbr);

	return drive;
}
//����ļ����ļ����Ƿ��Ѵ���
bool checkFileExists(const TCHAR * fileName) {
	TCHAR fullpath[MAX_PATH];
	::GetFullPathName(fileName, MAX_PATH, fullpath, NULL);
	if (::PathFileExists(fullpath)) {
		return true;
	}
	return false;
}
tstring FormatFileTime(FILETIME &ft)//��ʽ���ļ�ʱ��
{
	FILETIME temp;//�ļ�ʱ��ṹ
	FileTimeToLocalFileTime(&ft, &temp);//�������ļ�ʱ��ת���ɱ����ļ�ʱ��
	SYSTEMTIME st;//ʱ��ṹ
	FileTimeToSystemTime(&temp, &st);//�������ļ�ʱ��ת����ϵͳʱ��
	tstringstream out;//�ַ�����
	out << st.wYear << TEXT("��") << st.wMonth << TEXT("��") << st.wDay << TEXT("��,") << st.wHour << TEXT(":") << st.wMinute << TEXT(":") << st.wSecond;//��ʽ���ַ���
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
//ѡ���ļ��жԻ���ص�����  
int CALLBACK BrowseCallBackFun(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR currentSelectPath[MAX_PATH];//���浱ǰѡ���·��
	switch (uMsg)
	{
	case BFFM_INITIALIZED://ѡ���ļ��жԻ�����ɳ�ʼ�����Ի������һ����Ϣ 
		SendMessage(hwnd, BFFM_SETSELECTION, true, lpData);//��BROWSEINFO�е�lParam������Ĭ��·����lpData�У���wParam��Ϊtrue,lParam��Ϊ��'\0'Ϊβ�ַ����������Ĭ�ϴ�Ŀ¼
		SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, lpData);//����״̬�����ı����ҵ�ͼƬ·��
		SetWindowText(hwnd, TEXT("ѡ��ͼƬ�ļ���"));
		break;
	case BFFM_SELCHANGED://ѡ�������ļ��к�
		SHGetPathFromIDList((LPCITEMIDLIST)lParam, currentSelectPath);//lParam������һ��LPCITEMIDLIST�ṹ��ת�����ַ�����·��
		SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (long)currentSelectPath);//����״̬�����ı�����·��
		if (currentSelectPath[0] != '\0')//������ǿ��ַ���
			lstrcpyn(szMyPictures, currentSelectPath, MAX_PATH);//���õ�ǰͼƬ·��Ϊ�µ�·��
		break;
	}
	return false;
}

void InitBrowseInfo(HWND hwnd, BROWSEINFO &bi)// ��ʼ����Ŀ¼�ṹ
{
	bi.hwndOwner = hwnd;//����ļ��жԻ���ĸ�������
	bi.iImage = 0;// ��ѡ����ļ��е�ͼƬ���
	bi.lParam = (long)szMyPictures;//�Ի��򴫵ݸ��ص�������һ������ָ��,�����ص������Ĳ���,��һ������Ĭ��·��Ϊ�ҵ�ͼƬ�ļ���
	bi.lpfn = BrowseCallBackFun;//Ӧ�ó����������Ի���ص������ĵ�ַ�����Ի����е��¼�����ʱ���öԻ��򽫵��ûص��������ò�������ΪNULL
	bi.lpszTitle = TEXT("�����ˣ��ҵ���ܾ��ˣ���ѡ��һ��ͼƬ�ļ�����");//������ļ��жԻ������ʾ�ı���������ʾ������ļ��жԻ���Ĺ��ܡ����ú�Ŀ��
	bi.pidlRoot = 0;// ITEMIDLIST�ṹ�ĵ�ַ���������ʱ�ĳ�ʼ��Ŀ¼������ֻ�б�ָ����Ŀ¼������Ŀ¼����ʾ������ļ��жԻ����С��ó�Ա����������NULL���ڴ�ʱ����Ŀ¼����ʹ��
	bi.pszDisplayName = szDirectoryName;//���������û�ѡ�е�Ŀ¼�ַ������ڴ��ַ���û������Ĵ�Сȱʡ�Ƕ����MAX_PATH������
	bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS;//�ñ�־λ�����˶Ի����ѡ�������Ϊ0
	//BIF_EDITBOX������Ի����а���һ���༭���ڸñ༭�����û���������ѡ���������
	//BIF_STATUSTEXT���ڶԻ����а���һ��״̬����ͨ�����Ի�������Ϣʹ�ص���������״̬�ı�
	//BIF_RETURNONLYFSDIRS�����������ļ�ϵͳ��Ŀ¼�����磺������ļ��жԻ����У���ѡ������һ��Ŀ¼ʱ���á�OK����ť���ã�����ѡ�С��ҵĵ��ԡ��������ھӡ��ȷ�������Ľڵ�ʱ����OK����ťΪ��ɫ
}



TCHAR szTip[128] = TEXT("�ͣ����ӿ�ʲô");//���ݵ���ʾ��Ϣ
TCHAR szInfoTitle[64] = TEXT("������Ļ��ô�ã��ó�ȥ��Ϣ��Ϣ�ˣ�ע���۾�");//�ַ���������Ϊ������ʾ�ı��⡣�˱��������������ʾ����ϲ�����ຬ��63���ַ���
TCHAR szInfo[256] = TEXT("���ڻ���ֽ�ˣ��ڻص������ʱ��ÿ2�뻻һ�α�ֽ");////�ַ���������Ϊ������ʾ���ݡ���ຬ��255���ַ�
//��ʼ�����̽ṹ
void InitNotifyIconData(HWND hwnd, HINSTANCE hInst, NOTIFYICONDATA &nid, int pictureID,bool closeBubble)
{
	nid.cbSize = sizeof(nid);//�ṹ��Ĵ�С�����ֽ�Ϊ��λ
	nid.dwInfoFlags = NIIF_USER;//���ô˳�Ա������������ʾ������һ��ͼ�꣬NIIF_USER���û���ͼ�꣬����Աnid.hIcon
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(pictureID));//��ȡ�����õ�ͼ�꣬���ӡ��޸Ļ�ɾ����ͼ��ľ��
	if(closeBubble)
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;//�˳�Ա����������Щ������ԱΪ�Ϸ����ݣ�����Щ��Ա�����ã�
	else
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
//��ʼ����ɫ�ṹ
void InitColor(HWND hwnd, CHOOSECOLOR &cc, COLORREF color[])
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
//��ʼ������ṹ
void InitFont(HWND hwnd, CHOOSEFONT &cf, LOGFONT &lf)
{
	cf.Flags = CF_NOVERTFONTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;// CF_EFFECTS |ʹ�Ի�����ʾ�ؼ������û�ָ���»���,���ı���ɫѡ� ������ô˱�־,������ʹ��rgbColors��Աָ����ʼ�ı���ɫ
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
	cf.rgbColors = 0;//��ʼ����Ի����ı���ɫ
}
HWND FindDialog(HWND hwnd, FINDREPLACE &fr, TCHAR szFindText[])//���ҽṹ
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