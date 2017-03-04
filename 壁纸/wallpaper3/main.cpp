#include"resource.h"
#include<shlobj.h>
BROWSEINFO bi;//�������û�ѡ��Ŀ¼����Ҫ��Ϣ
LPITEMIDLIST plDlist;//�����û�ѡ���Ŀ¼��Ϣ�ṹ
LPMALLOC pMalloc;//LPMALLOC��һ��IMallocָ�룬IMALLOCʵ����һ���ǳ��������ڴ�������棬�����ͷ�ITEMIDLISTָ����ָ��Ŀռ�

bool CreateWallpaperDirectory()//���ҵ��ĵ�����wallpaperĿ¼
{
	WIN32_FIND_DATA fd;//�ļ��ṹ
	tstring find = tstring(szMyDocument)+ TEXT("\\wallpaper");
	HANDLE handle = FindFirstFile(find.c_str(), &fd);//���ҵ�1���ļ�
	if (handle == INVALID_HANDLE_VALUE || (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)//����Ҳ����ļ���wallpaper�����ҵ��Ĳ���Ŀ¼
	{
		SetCurrentDirectory(szMyDocument);//���õ�ǰĿ¼
		CreateDirectory(TEXT("wallpaper"), 0);//����Ŀ¼
		return false;
	}
	return true;
}
void ReadDiretoryPath()//��ȡ��ֽ·��
{
	TCHAR szBuffer[MAX_PATH];
	tstring save = tstring(szMyDocument) + TEXT("\\wallpaper\\backUpDirectorys.lu");//�浵�ļ�ȫ·��
	ifstream fin(save,ios::in| ios::binary);//������,�ҿ��ļ�backUpDirectorys.lu,�Զ����Ʒ�ʽ����
	if (!fin.is_open())
		return;
	while (fin)
	{
		fin.getline(szBuffer,MAX_PATH,'\n');//��ȡһ���ַ���
		if (szBuffer[0] != '\0')//�������
		backupDirectorys.push_back(szBuffer);//����ÿ����ֽĿ¼
	}
	fin.close();
	save = tstring(szMyDocument) + TEXT("\\wallpaper\\currentDirectorys.lu");//�浵�ļ�ȫ·��
	fin.open(save, ios::binary|ios::in);
	if (!fin.is_open())
		return;
	while (fin)
	{
		fin.getline(szBuffer, MAX_PATH, '\n');//��ȡһ���ַ���
		if (szBuffer[0] != '\0')//�������
		currentDirectorys.push_back(szBuffer);//����ÿ����ֽĿ¼
	}
	fin.close();
}
bool ReadSaveFile()//��ȡ�浵�ļ�
{
	WIN32_FIND_DATA fd;//�ļ��ṹ
	TCHAR szBuffer[MAX_PATH];
	int count;//·������
	Time timeTemp;//ʱ��ṹ��ʱ����
	tstring find = tstring(szMyDocument) + TEXT("\\wallpaper\\save.lu");//Ҫ���ҵĴ浵�ļ�ȫ·��
	if (!CreateWallpaperDirectory())//���û��Ŀ¼����Ŀ¼
		return false;//Ŀ¼���޴浵�˳�
	HANDLE handle = FindFirstFile(find.c_str(), &fd); //���Ҵ浵�ļ�save.lu
	if (handle == INVALID_HANDLE_VALUE || fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//����Ҳ��������ҵ�����Ŀ¼save.lu
		return false;
	fstream fin(find, ios_base::in | ios_base::binary);//���������Խ��Ʒ�ʽ��
	if (!fin.is_open())//��ʧ��
		return true;
	//��ȡ������������

	fin.read((char*)&timeTemp, sizeof(Time));//��ȡ��ֽ����ʱ��ṹ
	if (timeTemp.hour < 1000 && timeTemp.minute < 1000 && timeTemp.second < 1000)//��ʱ�䷶Χ������1000����Ϊÿ����Ա��unsigned int��һ��������������浵���ƻ�����ȡ������ܲ����ܴ��������538976288
		Wallpaper = timeTemp;//����ʱ��
	fin.read((char*)&timeTemp, sizeof(Time));//��ȡ����ʱ��ṹ
	if (timeTemp.hour < 1000 && timeTemp.minute < 1000 && timeTemp.second < 1000)//��ʱ�䷶Χ������1000����Ϊÿ����Ա��unsigned int��һ��������������浵���ƻ�����ȡ������ܲ����ܴ��������538976288
		Notify = timeTemp;//����ʱ��
	fin.read((char*)&randomChange, sizeof(bool));//��ȡ�Ƿ��������ֵ
	fin.read((char*)&startingUpBoot, sizeof(bool));//��ȡ�Ƿ񿪻�����������ֵ
	fin.read((char*)&changeByOnlyDestop, sizeof(bool));//��ȡ�Ƿ��������������ڲŻ���ֽ����ֵ
	fin.read((char*)&includeSubDirectory, sizeof(bool));//��ȡ�Ƿ������Ŀ¼����ֵ
	fin.read((char*)&startChange, sizeof(bool));//��ȡ�Ƿ�ʼ������ֵ
	fin.read((char*)&startingUpBoot, sizeof(bool));//��ȡ�Ƿ񿪻��������
	//��ȡ������Ϣ
	fin.read((char*)&szTip, sizeof(szTip));
	fin.read((char*)&szInfo, sizeof(szInfo));
	fin.read((char*)&szInfoTitle, sizeof(szInfoTitle));
	fin.read((char*)&windowRect, sizeof(RECT));//��ȡ���ڴ�Сλ��
	fin.read((char*)&lf, sizeof(LOGFONT));//��ȡ����ṹ
	fin.read((char*)&textColor, sizeof(COLORREF));//��ȡ��ɫ
	fin.read((char*)&textBkColor, sizeof(COLORREF));//��ȡ������ɫ
	fin.read((char*)&nSelectWallpaper, sizeof(UINT));//��ȡ��ǰ��ֽ����
	fin.read((char*)&color, sizeof(color));//��ȡ�Զ������ɫ����
	ReadDiretoryPath();//��ȡ��ֽ·��
	fin.close();
	return true;
}
void SaveDiretoryPath()//�����ֽ·��
{
	tstring save = tstring(szMyDocument) + TEXT("\\wallpaper\\backUpDirectorys.lu");//�浵�ļ�ȫ·��
	int count;//·������
	ofstream fout(save, ios::binary);//�����,�ҿ��ļ�save.lu,�Զ����Ʒ�ʽд��
	if (!fout.is_open())
		return;
	count = backupDirectorys.size();//�����ļ�·������
	for (int i = 0; i < count; i++)
		fout<<backupDirectorys[i].c_str()<<endl;//д��ÿ����ֽĿ¼
	fout.close();
	save = tstring(szMyDocument) + TEXT("\\wallpaper\\currentDirectorys.lu");//�浵�ļ�ȫ·��
	fout.open(save, ios::binary);
	if (!fout.is_open())
		return;
	count = currentDirectorys.size();//��ǰ��ֽ·������
	for (int i = 0; i < count; i++)
		fout<<currentDirectorys[i].c_str()<<endl;//д��ÿ����ֽĿ¼
	fout.close();
}
void WriteSaveFile()//�����ڴ浵�ļ�
{
	tstring save = tstring(szMyDocument) + TEXT("\\wallpaper\\save.lu");//�浵�ļ�ȫ·��
	TCHAR *pBackFileName,*pCurrentFileName;
	CreateWallpaperDirectory();//���û��wallpaperĿ¼�򴴽�
	ofstream fout(save, ios::binary);//�����,�ҿ��ļ�save.lu,�Զ����Ʒ�ʽд��
	if (!fout.is_open())//��ʧ��
		return;
	fout.write((char*)&Wallpaper, sizeof(Time));//�����ֽʱ��ṹ
	fout.write((char*)&Notify, sizeof(Time));//��������ʱ��ṹ
	fout.write((char*)&randomChange, sizeof(bool));//�����Ƿ��������ֵ
	fout.write((char*)&startingUpBoot, sizeof(bool));//�����Ƿ񿪻�����������ֵ
	fout.write((char*)&changeByOnlyDestop, sizeof(bool));//�����Ƿ��������������ڲŻ���ֽ����ֵ
	fout.write((char*)&includeSubDirectory, sizeof(bool));//�����Ƿ������Ŀ¼����ֵ
	fout.write((char*)&startChange, sizeof(bool));//�����Ƿ�ʼ������ֵ
	fout.write((char*)&startingUpBoot, sizeof(bool));//�����Ƿ񿪻��������
	//����������Ϣ
	fout.write((char*)&szTip, sizeof(szTip));
	fout.write((char*)&szInfo, sizeof(szInfo));
	fout.write((char*)&szInfoTitle, sizeof(szInfoTitle));
	fout.write((char*)&windowRect, sizeof(RECT));//���洰�ڴ�Сλ��
	fout.write((char*)&lf, sizeof(LOGFONT));//��������ṹ
	fout.write((char*)&textColor, sizeof(COLORREF));//������ɫ
	fout.write((char*)&textBkColor, sizeof(COLORREF));//���汳����ɫ
	fout.write((char*)&nSelectWallpaper, sizeof(UINT));//���浱ǰ��ֽ����
	fout.write((char*)&color, sizeof(color));//�����Զ������ɫ����
	SaveDiretoryPath();////�����ڴ浵�ļ�
	fout.close();
}

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
	HBITMAP hBm[8];
	//��Ӳ˵�
	AppendMenu(hMenu, MF_STRING, IDM_SHOWWALLPAPER, TEXT("�鿴�����ֽ\tCtrl+Alt+M"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(hMenu, MF_STRING, IDM_NEXTWALLPAPER, TEXT("��һ��\tAlt+N"));
	AppendMenu(hMenu, MF_STRING, IDM_PREWALLPAPER, TEXT("��һ��\tAlt+P"));
	AppendMenu(hMenu, MF_STRING, IDM_PAUSE, startChange ? TEXT("��ͣ\tCtrl+P") : TEXT("��ʼ\tCtrl+P"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(hMenu, MF_STRING, IDM_RANDOM, randomChange ? TEXT("˳��\tAlt+R") :TEXT("���\tAlt+R"));
	AppendMenu(hMenu, MF_STRING, IDM_MINI,TEXT("���д�����С���Ÿ�����ֽ"));
	AppendMenu(hMenu, MF_STRING, IDM_SET, TEXT("��ֽ����\tAlt+S"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("�˳�\tAlt+F4"));
	CheckMenuItem(hMenu, IDM_MINI, changeByOnlyDestop ? MF_CHECKED : MF_UNCHECKED);//����ѡ��״̬
	hBm[0] = LoadBitmap(hInst, TEXT("IDB_BITMAP1"));//��ȡλͼ
	SetMenuItemBitmaps(hMenu, IDM_SHOWWALLPAPER, MF_BYCOMMAND, hBm[0], 0);//���ò˵�λͼ
	hBm[1] = LoadBitmap(hInst, TEXT("IDB_BITMAP2"));//��ȡλͼ
	SetMenuItemBitmaps(hMenu, IDM_NEXTWALLPAPER, MF_BYCOMMAND, hBm[1], 0);//���ò˵�λͼ
	hBm[2] = LoadBitmap(hInst, TEXT("IDB_BITMAP3"));//��ȡλͼ
	SetMenuItemBitmaps(hMenu, IDM_PREWALLPAPER, MF_BYCOMMAND, hBm[2], 0);//���ò˵�λͼ
	hBm[3] = LoadBitmap(hInst, TEXT("IDB_BITMAP4"));//��ȡλͼ
	SetMenuItemBitmaps(hMenu, IDM_PAUSE, MF_BYCOMMAND, hBm[3], 0);//���ò˵�λͼ
	hBm[4] = LoadBitmap(hInst, TEXT("IDB_BITMAP5"));//��ȡλͼ
	SetMenuItemBitmaps(hMenu, IDM_EXIT, MF_BYCOMMAND, hBm[4], 0);//���ò˵�λͼ
	hBm[5] = LoadBitmap(hInst, TEXT("IDB_BITMAP6"));//��ȡλͼ
	SetMenuItemBitmaps(hMenu,IDM_RANDOM, MF_BYCOMMAND, hBm[5], 0);//���ò˵�λͼ
	hBm[6] = LoadBitmap(hInst, TEXT("IDB_BITMAP7"));//��ȡλͼ
	SetMenuItemBitmaps(hMenu,IDM_MINI, MF_BYCOMMAND, hBm[6], 0);//���ò˵�λͼ
	hBm[7] = LoadBitmap(hInst, TEXT("IDB_BITMAP8"));//��ȡλͼ
	SetMenuItemBitmaps(hMenu,IDM_SET, MF_BYCOMMAND, hBm[7], 0);//���ò˵�λͼ
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
			tempDirectory[fd.cFileName] = CurrentPath;//����ǰͼƬ������·������map�б���
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
		if (title[0] != '\0'&&lstrcmp(title,TEXT("��ʼ"))!=0)//������ⲻΪ��,���߲��ǿ�ʼ������˵�WIN7��ʼ��ť
			allTitle.push_back(title);//�����������
		temp += tstring(title)+TEXT("   ");
	}
	return true;//Ϊ����ö����һ�����ڣ��ص��������뷵��TRUE����ֹͣ�б������뷵��FALSE
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
	if (recordIndex == 0 || wallPaperCount==0)//�����ǰ��ֽ�ǻ����ı�ֽ��¼�е�һ�Ż����ޱ�ֽ
		EnableMenuItem(hMenu,IDM_PREWALLPAPER, MF_GRAYED);//��һ��ͼƬ�˵���Ϊ����
	else
		EnableMenuItem(hMenu,IDM_PREWALLPAPER,MF_ENABLED);//��һ��ͼƬ�˵���Ϊ����
	EnableMenuItem(hMenu, IDM_NEXTWALLPAPER,wallPaperCount? MF_ENABLED:MF_GRAYED);//��һ��ͼƬ�˵���Ϊ���û�������
	EnableMenuItem(hMenu,IDM_PAUSE, wallPaperCount ?  MF_ENABLED:MF_GRAYED );//��ʼ�˵���Ϊ���û�������
	int id = TrackPopupMenu(hMenu, TPM_RETURNCMD, mousePos.x, mousePos.y, 0, hwnd, 0);//�����˵�,TPM_RETURNCMD���ز˵�ID
	switch (id)
	{
	case IDM_EXIT:
		SendMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	case IDM_SHOWWALLPAPER://�鿴�����ֽ
		SendMessage(hwnd, WM_COMMAND, IDM_SHOWWALLPAPER, 0);//������Ϣ����ʾ����
		break;
	case IDM_PREWALLPAPER://��һ��
		SendMessage(hwnd, WM_COMMAND, IDM_PREWALLPAPER, 0);
		break;
	case IDM_NEXTWALLPAPER://��һ��
		SendMessage(hwnd, WM_COMMAND,IDM_NEXTWALLPAPER, 0);
		break;
	case IDM_PAUSE://��ͣ
		SendMessage(hwnd, WM_COMMAND, IDM_PAUSE, 0);
		break;
	case IDM_RANDOM://����ֽģʽ�����������˳��
		SendMessage(hwnd, WM_COMMAND,IDM_RANDOM, 0);
		break;
	case IDM_MINI://���д�����С���Ÿ�����ֽ�˵�
		SendMessage(hwnd, WM_COMMAND,IDM_MINI, 0);
		break;
	case IDM_SET://���ñ�ֽ
		DialogBox((HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE), TEXT("IDD_DIALOGSET"), hwnd, DlgWallpaperSet);
		break;
	}
}
void HighLineWallPaper(HWND hEdit, int  nSelectWallpaper)//�Ե�ǰ�ı�ֽ���и�����ʾ
{
	if (nSelectWallpaper > 0)//�����Ϊ0
	{
		lineIndex = SendMessage(hEdit, EM_LINEINDEX,nSelectWallpaper, 0);//�õ�ָ�������ַ����ı��е�λ�ã����ֽ�����ʾ��
		nextLineIndex = SendMessage(hEdit, EM_LINEINDEX, nSelectWallpaper +1, 0);//�õ�ָ������һ�����ַ����ı��е�λ�ã����ֽ�����ʾ��
		SendMessage(hEdit, EM_SETSEL, lineIndex, nextLineIndex);//ѡ��ָ����
		SendMessage(hEdit, EM_SCROLLCARET, 0, 0);// �ѿɼ���Χ������괦,�����ù��󣬹����ܲ��ڷ�Χ�ڣ�����Ҫ��������괦
		if (!buttonDown)//������δ������˵�����Զ���
		SendMessage(hEdit, WM_VSCROLL, SB_LINEUP, 0);//��Ϊ��ѡ�пͻ������һ��ʱ�������ڿͻ������һ�е���һ�У�EM_SCROLLCARET�Ὣ������ѡ���е���һҳ��������������ʾ��Ҫ���Ϲ�һ�в��ܿ��������Է���WM_VSCROLL���Ϲ�һ�У���ʾ����
	}
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
void DisplayDistribitue()//��ʾ�ļ�����
{
	TCHAR szBuffer[1024];//�ַ�����
	WIN32_FILE_ATTRIBUTE_DATA attri;//�ļ����Խṹ
	tstring szPath;//�ļ�·��
	TCHAR szName[31];//�ļ���
	static TCHAR *szColumn[8] = {TEXT("����"),TEXT("�ļ���"),TEXT("��С"),TEXT("����"),TEXT("����ʱ��"),TEXT("���һ�η���ʱ��"),TEXT("���һ���޸�ʱ��"),TEXT("·��")};
	StringCchPrintf(szBuffer, 1024, TEXT("%-9s%-30s%-20s%-15s%-30s%-30s%-30s%-30s\r\n"),szColumn[0],szColumn[1],
		szColumn[2], szColumn[3], szColumn[4], szColumn[5], szColumn[6], szColumn[7]);//��ʽ������
	tstring temp = szBuffer;//�ڱ༭����ʾ���ַ���
	tstring index;//����
	wallPaperCount = 0;//�ļ���������ʼ��Ϊ0
	directoryIndex.clear();//����һ���������
	for (int i=0; i<AllDirectorys.size();i++)//��ʾ����Ŀ¼����Ϣ
	{
		directoryIndex.push_back(wallPaperCount);//����ǰĿ¼��һ��ͼƬ��������
		for (auto iter=AllDirectorys[i].begin(); iter!=AllDirectorys[i].end();iter++)//��ʾ��ǰĿ¼��Ϣ
		{
			wallPaperCount++;//�ļ�������1
			GetFileAttributesEx((iter->second+TEXT("\\")+iter->first).c_str(), GetFileExInfoStandard, &attri);//�õ���ǰ�ļ�������,GetFileExInfoStandard��ʾ��׼�ļ����ԣ����һ�������� WIN32_FILE_ATTRIBUTE_DATA�ṹ��ָ��
			fileCreateTime = FormatFileTime(attri.ftCreationTime);//�ļ�����ʱ��
			fileLastAccessTime = FormatFileTime(attri.ftLastAccessTime); // �ļ����һ�η���ʱ��
			fileLastWriteTime = FormatFileTime(attri.ftLastWriteTime);//�ļ����һ���޸�ʱ��
			fileAttributes = FileAttribute(attri.dwFileAttributes);//�ļ�����
			szPath = (iter->second).c_str();//�ļ�·��
			lstrcpyn(szName,(iter->first).c_str(),30);//�����ļ�������ǰ30���ַ�
			if ((iter->first).size()>29)//����ļ�������29���ļ���̫��
				lstrcpyn(szName+24, TEXT("*.jpg"),6);//�����5���ַ���Ϊ*.jpg�Ա�ʾ�ļ���ʡ��
			fileSize = calculateCapacity(attri.nFileSizeHigh, attri.nFileSizeLow);//�����ļ���С
			index = ToString(wallPaperCount) + TEXT(":");//���ļ���������ת�����ַ���
			StringCchPrintf(szBuffer, 1024, TEXT("%-9s%-30s%-20s%-15s%-30s%-30s%-30s%-30s\r\n"), index.c_str(), szName, fileSize.c_str(),
			fileAttributes.c_str(), fileCreateTime.c_str(), fileLastAccessTime.c_str(), fileLastWriteTime.c_str(),
			szPath.c_str());//��ʽ���ַ���
			temp += szBuffer;
		}
	}
	SetWindowText(hEdit, temp.c_str());//�ڱ༭����ʾ
}
void ChangeWallPaper()//�ı��ֽ
{
	int nDirectory = AllDirectorys.size() - 1;//�ĸ�Ŀ¼,��ʼ��Ϊ���һ��Ŀ¼�ĵ�����
	int nIndex;//��Ŀ¼������ͼƬ(����)			
	for (int i = 0; i <= nDirectory; i++)
	{
		if (nSelectWallpaper - 1 < directoryIndex[i])//��Ϊ���������С���������ǰѡ�е�ͼƬ����(������0��ʼ��Ҫ��1)�ȵ�ǰĿ¼�����һ��ͼƬС
		{//��iΪ��ǰĿ¼����
			nDirectory = --i;//���浱ǰͼƬ����һ��Ŀ¼������
			break;//�ҵ�Ŀ¼�ˣ��˳�ѭ��
		}
	}
	nIndex = (nSelectWallpaper - 1) - directoryIndex[nDirectory];//��ȥ��ǰĿ¼��һ��Ŀ¼����ͼƬ����������ø�Ŀ¼������ͼƬ(����)����Ϊ(������0��ʼ������Ҫ��1)
	auto tempMap = AllDirectorys[nDirectory];//��ǰĿ¼��map�ṹ
	auto begin = tempMap.begin();//��ȻͼƬĿ¼�ĵ�1��������
	for (int i = 0; i <nIndex; i++)
		begin++;//���������Ƶ���ǰͼƬ��������
	tstring szPath = begin->second;//�ļ�·��
	tstring szName = begin->first;//�ļ���
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)(szPath + TEXT("\\") + szName).c_str(), 0);//��˫����ͼƬ����Ϊ��ֽ
	statusInfo[0] = tstring("��ǰ��ֽ:") + szPath + TEXT("\\") + szName;
	SendMessage(hStatus, SB_SETTEXT, 0, (long)statusInfo[0].c_str());//��һ����ʾ·��	
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
		if (wParam == VK_F3)//����ǲ���һһ�п�ݼ�
			break;//�˳�
		row = SendMessage(hEdit, EM_LINEFROMCHAR, -1, 0) + 1;//���ָ��λ�����ڵ��кţ�-1��ʾ��ǰ�������λ�ã���Ϊλ�õ�������0��ʼ������Ҫ��1
		statusInfo[2] = TEXT("�к�:") + ToString(row);//���к�ת�����ַ���
		SendMessage(hStatus, SB_SETTEXT, 2, (long)statusInfo[2].c_str());//�Եڶ��������к� 
		first = SendMessage(hEdit, EM_LINEINDEX, -1, 0);//��ǰ��������У����ַ��ڱ༭�е�λ��
		SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//��ǰѡ�����ֵĿ�ʼ������λ�ã���δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������			
		col = startPos - first + 1;//��ǰ����λ��-��ǰ��������е�1���ַ���λ��Ϊ����������Ϊλ�õ�������0��ʼ������Ҫ��1
		statusInfo[3] = TEXT("�к�:") + ToString(col);//���к�ת�����ַ���
		SendMessage(hStatus, SB_SETTEXT, 3, (long)statusInfo[3].c_str());//�Ե����������к� 
		if (row >= 2)//�������ѡ���1��
		{
			nSelectWallpaper = row -1;//��Ϊ��1���������ƣ���2���ǵ�1�ţ���3���ǵ�2�ţ����Ե�ǰѡ�е�������row-1��
			HighLineWallPaper(hEdit, nSelectWallpaper);//�Ե�ǰ�и�����ʾ������δ����Ϊ��ǰ��Ϊ��ֽ��ֻ�����˫��������
		}
		buttonDown =false;//�������ɿ�
		break;
	case WM_LBUTTONDBLCLK:
		SendMessage(hEdit, EM_SETSEL, lineIndex,lineIndex);//��Ϊ��������ָ������һ�У���ΪWM_LBUTTONDBLCLK����WM_LBUTTONUP��Ϊ�˼���ѡ�е�ǰ�У�Ҫ��������ȥ
		if (row >= 2 && row <= wallPaperCount + 1)//����ڵ�2�е����һ��,+1����Ϊ������Ҫ������1��
		{
			ChangeWallPaper();//�ı��ֽ
			recordChange.push_back(nSelectWallpaper);//���滻���ı�ֽ�еļ�¼�е�����
			recordIndex = recordChange.size() - 1;//��ǰ��ֽ�ڻ����ı�ֽ�еļ�¼�е����������һ������
		}
		buttonDown = true;//����������
		break;
	case WM_RBUTTONDOWN://�Ҽ�����
		PopupMyMenu(GetParent(hEdit), hMenu);//�����˵�
		break;
	case WM_LBUTTONDOWN:
		buttonDown = true;//����������
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
bool MyFindText(FINDREPLACE &fr, bool bDown)//�����ı�
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
	static HWND hEdit;//�༭����
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
				MessageBox(hEditGoto, TEXT("���ҳ����˱�ֽ����"), TEXT("��ֽ -����"), 0);
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
void AddWapperDirectory(HWND hwnd)//��ӱ�ֽĿ¼
{
	recordChange.clear();//���Ŀ¼��ʹ��ǰһ��������ʧЧ�����Խ���¼���
	recordIndex = 0;//��������¼��ʼ��Ϊ0
	plDlist = SHBrowseForFolder(&bi);//����ѡ��Ŀ¼�Ի���,�����û�ѡ���Ŀ¼��Ϣ�ṹ��ָ��plDlist
	if (plDlist)//���ѡ����һ��Ŀ¼
	{
		SHGetPathFromIDList(plDlist, szDirectoryPath);//��plDlist·�����ṹ�壩ת�����ַ���·��				
		auto iter =find(currentDirectorys.begin(),currentDirectorys.end(), szDirectoryPath);//�����б�ֽĿ¼�в����Ƿ��Ѵ��ڸ�Ŀ¼
		if (iter ==currentDirectorys.end())//��Ŀ¼�Ҳ�������ӵ�Ŀ¼
		{
			tempDirectory.clear();//����һ����ӵ�Ŀ¼��������ʱ�ļ������ļ�·�����
			FindAllFile(szDirectoryPath, includeSubDirectory);//������Ŀ¼������ͼƬ
			AllDirectorys.push_back( tempDirectory);//����ǰĿ¼��������Ϣ���
			currentDirectorys.push_back(szDirectoryPath);//����Ŀ¼��ӵ���ǰĿ¼
			DisplayDistribitue();//�����ļ�������ʾ
		}
		else
			MessageBox(hwnd, TEXT("�������Ŀ¼,��Ҫ�ظ�����Ѵ���Ŀ¼"),TEXT("��Ŀ¼�Ѵ���:"), MB_ICONWARNING | MB_OK);
		if (SHGetMalloc(&pMalloc) == S_OK);//����IMALLOC����
		{
			pMalloc->Free(plDlist);//�ͷ�plDlistָ��Ŀռ�
			pMalloc->Release();//�ͷ�pMalloc�Ŀռ�
		}
	}
}
void GetPictureAndDocumentPath(HWND hwnd)//�õ��ҵ��ĵ����ҵ�ͼƬ·��
{
	SHGetSpecialFolderLocation(hwnd, CSIDL_MYPICTURES, &plDlist);//����ҵ�ͼƬ�ļ���·��
	SHGetPathFromIDList(plDlist,szMyPictures);//��plDlist·�����ṹ�壩ת�����ַ���·��
	SHGetSpecialFolderLocation(hwnd, CSIDL_PERSONAL, &plDlist);//����ҵ��ĵ��ļ���·��
	SHGetPathFromIDList(plDlist, szMyDocument);//��plDlist·�����ṹ�壩ת�����ַ���·��
	if (SHGetMalloc(&pMalloc) == S_OK);//����IMALLOC����
	{
		pMalloc->Free(plDlist);//�ͷ�plDlistָ��Ŀռ�
		pMalloc->Release();//�ͷ�pMalloc�Ŀռ�
	}
}
UINT calculateTime(const Time &time)//����ʱ�䣬�Ժ����
{
	unsigned millisecond;//����
	millisecond = time.second * 1000 + time.minute * 60 * 1000 + time.hour * 60 * 60 * 1000;//����ʱ�䣬�Ժ����
	return millisecond;
}
int _stdcall DlgWallpaperSet(HWND hDlg, unsigned int message, unsigned wParam, long lParam)//���ñ�ֽ�Ի��򴰿ڹ���
{
	int wmID = LOWORD(wParam);//�ؼ�ID
	static HWND hWallPaperHour, hWallPaperMinute, hWallPaperSecond;//��ֽ����ʱ���֡���༭����
	static HWND hNotifyHour, hNotifyMinute, hNotifySecond;//������ʾʱ���֡���༭����
	static HWND hNotifyTip, hNotifyTitle, hNotifyContent;//������ʾ�����⡢���ݱ༭��
	static HWND hRadioRandom, hRadioSequence;//����ͺ�˳��ѡ��ť���
	static HWND hCheckBoxMini, hCheckBoxSub,hCheckBoxAutoBoot;//���д�����С������Ŀ¼�Ϳ����Զ�����check box��ť���
	static HWND hwnd;//������
	static HINSTANCE hInst;//ʵ�����
	TCHAR szBuffer[MAX_PATH];//������
	switch (message)
	{
	case WM_INITDIALOG://�Ի����ʼ��
		hWallPaperHour = GetDlgItem(hDlg, IDC_EDITHOUR);//��öԻ�������ؼ��ľ��
		hWallPaperMinute = GetDlgItem(hDlg, IDC_EDITMINUTE);
		hWallPaperSecond = GetDlgItem(hDlg, IDC_EDITSECOND);
		hNotifyHour = GetDlgItem(hDlg, IDC_NOTIFYHOUR);
		hNotifyMinute = GetDlgItem(hDlg, IDC_NOTIFYMINUTE);
		hNotifySecond = GetDlgItem(hDlg, IDC_NOTIFYSECOND);
		hNotifyTip = GetDlgItem(hDlg, IDC_EDITTIP);
		hNotifyTitle = GetDlgItem(hDlg, IDC_EDITTITLE);
		hNotifyContent = GetDlgItem(hDlg,IDC_EDITCONTENT);
		hRadioRandom = GetDlgItem(hDlg, IDC_RANDOM);
		hRadioSequence = GetDlgItem(hDlg, IDC_SEQUENCE);
		hCheckBoxMini = GetDlgItem(hDlg, IDC_CHANGEMINI);
		hCheckBoxSub = GetDlgItem(hDlg, IDC_FINDSUBDIRECTORY);
		hCheckBoxAutoBoot = GetDlgItem(hDlg,IDC_AUTOBOOT);
		hwnd = GetParent(hDlg);
		hInst =(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE);
		//�ڸ����ؼ�����ʾ��ǰ����
		SetWindowText(hWallPaperHour, ToString(Wallpaper.hour).c_str());
		SetWindowText(hWallPaperMinute, ToString(Wallpaper.minute).c_str());
		SetWindowText(hWallPaperSecond, ToString(Wallpaper.second).c_str());
		SetWindowText(hNotifyHour, ToString(Notify.hour).c_str());
		SetWindowText(hNotifyMinute, ToString(Notify.minute).c_str());
		SetWindowText(hNotifySecond, ToString(Notify.second).c_str());
		SetWindowText(hNotifyTip, szTip);
		SetWindowText(hNotifyTitle, szInfoTitle);
		SetWindowText(hNotifyContent, szInfo);
		if (changeByOnlyDestop)//��������һ�����ڲŻ�
			SendMessage(hCheckBoxMini, BM_SETCHECK, 1, 0);
		else
			SendMessage(hCheckBoxMini, BM_SETCHECK, 0, 0);
		if (includeSubDirectory)//�����ļ�������Ŀ¼
			SendMessage(hCheckBoxSub, BM_SETCHECK, 1, 0);
		else
			SendMessage(hCheckBoxSub, BM_SETCHECK, 0, 0);
		if (randomChange)//����������
			SendMessage(hRadioRandom, BM_SETCHECK, 1, 0);
		else
			SendMessage(hRadioSequence, BM_SETCHECK, 1, 0);
		SendMessage(hCheckBoxAutoBoot, BM_SETCHECK,startingUpBoot ? 1 : 0, 0);
		break;
	case WM_COMMAND:
		switch (wmID)
		{
		case IDM_SETFINISH://�������
			GetWindowText(hWallPaperHour,szBuffer,MAX_PATH);//�õ�Сʱ�ַ���
			Wallpaper.hour = strToInt(szBuffer);//ת��������
			GetWindowText(hWallPaperMinute, szBuffer, MAX_PATH);//�õ������ַ���
			Wallpaper.minute = strToInt(szBuffer);//ת��������
			GetWindowText(hWallPaperSecond, szBuffer, MAX_PATH);//�õ����ַ���
			Wallpaper.second = strToInt(szBuffer);//ת��������
			wallPaperTime = calculateTime(Wallpaper);//�����±�ֽ����ʱ�䣬�Ժ����
			if (wallPaperTime>0)//���ʱ��>0
			KillTimer(hwnd, wallpaperID);//ɱ������ֽ��ʱ��
			if (startChange)//���������ͣ����
			SetTimer(hwnd, wallpaperID, wallPaperTime, 0);//��������ֽ��ʱ��
			GetWindowText(hNotifyHour, szBuffer, MAX_PATH);//�õ�Сʱ�ַ���
			Notify.hour = strToInt(szBuffer);//ת��������
			GetWindowText(hNotifyMinute, szBuffer, MAX_PATH);//�õ������ַ���
			Notify.minute = strToInt(szBuffer);//ת��������
			GetWindowText(hNotifySecond, szBuffer, MAX_PATH);//�õ����ַ���
			Notify.second = strToInt(szBuffer);//ת��������
			tipTime = calculateTime(Notify);//�����±�ֽ����ʱ�䣬�Ժ����
			if (tipTime>0)//���ʱ��>0
			KillTimer(hwnd,tipID);//ɱ������ֽ��ʱ��
			SetTimer(hwnd,tipID,tipTime, 0);//��������ֽ��ʱ��
			randomChange = SendMessage(hRadioRandom, BM_GETCHECK, 0, 0);//��û���ֽģʽ���Ƿ����
			changeByOnlyDestop = SendMessage(hCheckBoxMini, BM_GETCHECK, 0, 0);//���д�����С���Ÿ�����ֽѡ��״̬
			includeSubDirectory = SendMessage(hCheckBoxSub, BM_GETCHECK, 0, 0);//���Ұ�����Ŀ¼ѡ��״̬
		    startingUpBoot= SendMessage(hCheckBoxAutoBoot, BM_GETCHECK, 0, 0);//���ҿ���������ѡ��״̬
		    SetStartingUpBoot();//���ÿ����������
			GetWindowText(hNotifyTip, szBuffer, MAX_PATH);//���������ʾ�ַ���
			if (szBuffer[0] != '\0')//����
				lstrcpyn(szTip, szBuffer, 128);
			GetWindowText(hNotifyTitle, szBuffer, MAX_PATH);//������ݱ����ַ���
			if (szBuffer[0] != '\0')//����
				lstrcpyn(szInfoTitle, szBuffer,64);
			GetWindowText(hNotifyContent, szBuffer, MAX_PATH);//������������ַ���
			if (szBuffer[0] != '\0')//����
				lstrcpyn(szInfo, szBuffer,256);
			InitNotifyIconData(hwnd, hInst,nid);//��ʼ������ͼ��ṹ
			Shell_NotifyIcon(NIM_MODIFY, &nid);//�޸����������һ��ͼ�꣬�ٴ���ʾ����
			EndDialog(hDlg, 0);//���ٶԻ���
			return false;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);//���ٶԻ���
		return false;
	}
	return false;
}
int _stdcall DlgModifyDirectoryProc(HWND hDlg, unsigned int message, unsigned wParam, long lParam)//�޸ı�ֽĿ¼�Ի��򴰿ڹ���
{
	int wmID = LOWORD(wParam);//�ؼ�ID
	static HWND hListLeft, hListRight;//�����б����
	static HWND hEditPath;//·���༭��
	static HWND hBTNOK;//��ťOK
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	int index;//�б��������
	int count;//�б���ϵ�Ŀ¼����
	vector<tstring>temp;//�����б�����ַ���
	switch (message)
	{
	case WM_INITDIALOG:
		hListLeft = GetDlgItem(hDlg, IDC_LISTLEFT);//�õ�2���б����
		hListRight = GetDlgItem(hDlg, IDC_LISTRIGHT);
		hEditPath = GetDlgItem(hDlg, IDC_EDITDIRECTORY);//�õ�·���༭����
		hBTNOK = GetDlgItem(hDlg, IDC_BTNOK);//�õ�ȷ����ť����
		for (int i = 0; i < backupDirectorys.size(); i++)//������б����ʾ���ݱ�ֽĿ¼
			SendMessage(hListLeft, LB_INSERTSTRING, -1, (long)backupDirectorys[i].c_str());//����ǰĿ¼���뵽�б��β��
		for (int i = 0; i <currentDirectorys.size(); i++)//���ұ��б����ʾ��ǰ��ֽĿ¼
			SendMessage(hListRight, LB_INSERTSTRING, -1, (long)currentDirectorys[i].c_str());//����ǰĿ¼���뵽�б��β��
		return true;
	case WM_COMMAND:
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		switch (wmId)
		{
		case IDC_EDITDIRECTORY://Ŀ¼�༭��
			switch (wmEvent)//�ڱ༭���ϵĲ���
			{
			case EN_UPDATE://�༭���ϵ����ݸ�����
				GetWindowText(hEditPath, szDirectoryPath, MAX_PATH);//�õ��༭�����ַ���
				if (szDirectoryPath[0] != '\0')//����ַ�����Ϊ��
					EnableWindow(hBTNOK, true);//�ð�ť����
				else
					EnableWindow(hBTNOK, false);//�ð�ť������
				break;
			}
			break;
		case IDC_BTNADDDIRECTORY://��ӱ�ֽ��ť
			plDlist = SHBrowseForFolder(&bi);//����ѡ��Ŀ¼�Ի���,·�������ָ��plDlist
			if (plDlist)//���ѡ����һ��Ŀ¼
			{
				SHGetPathFromIDList(plDlist, szDirectoryPath);//��plDlist·�����ṹ�壩ת�����ַ���·��	
				SetWindowText(hEditPath, szDirectoryPath);//�ڱ༭������ʾ·��
				if (SHGetMalloc(&pMalloc) == S_OK);//����IMALLOC����
				{
					pMalloc->Free(plDlist);//�ͷ�plDlistָ��Ŀռ�
					pMalloc->Release();//�ͷ�pMalloc�Ŀռ�
				}
			}
			break;
		case IDC_MOVERIGHT://�����б��ѡ�е�·���Ƶ����б��
			index = SendMessage(hListLeft, LB_GETCURSEL, 0, 0);//��õ�ǰѡ��Ŀ¼������
			if (index != LB_ERR)
			{
				SendMessage(hListLeft, LB_GETTEXT, index, (long)szDirectoryPath);//��õ�ǰѡ�е�·��
				SendMessage(hListRight, LB_INSERTSTRING, -1, (long)szDirectoryPath);//����ǰĿ¼���뵽���б��β��
				SendMessage(hListLeft, LB_DELETESTRING, index, 0);//�����б����ɾ��
			}
			break;
		case IDC_MOVELEFT://�����б��ѡ�е�·���Ƶ����б��
			index = SendMessage(hListRight, LB_GETCURSEL, 0, 0);//��õ�ǰѡ��Ŀ¼������
			if (index != LB_ERR)
			{
				SendMessage(hListRight, LB_GETTEXT, index, (long)szDirectoryPath);//��õ�ǰѡ�е�·��
				SendMessage(hListLeft, LB_INSERTSTRING, -1, (long)szDirectoryPath);//����ǰĿ¼���뵽���б��β��
				SendMessage(hListRight, LB_DELETESTRING, index, 0);//�����б����ɾ��
			}
			break;
		case IDC_BTNOK://����ȷ����ť
			SendMessage(hListRight, LB_INSERTSTRING, -1, (long)szDirectoryPath);//����ǰĿ¼���뵽���б��β��
			break;
		case IDC_BTNDELETELEFT://�����б��ѡ��·������ɾ��
			index = SendMessage(hListLeft, LB_GETCURSEL, 0, 0);//��õ�ǰѡ��Ŀ¼������
			SendMessage(hListLeft, LB_DELETESTRING, index, 0);//ɾ��
			break;
		case IDC_BTNDELETERIGHT://�����б��ѡ��·������ɾ��
			index = SendMessage(hListRight, LB_GETCURSEL, 0, 0);//��õ�ǰѡ��Ŀ¼������
			SendMessage(hListRight, LB_DELETESTRING, index, 0);//ɾ��
			break;
		case IDC_BTNSETFINISH://�������
			backupDirectorys.clear();//�������Ŀ¼
			count = SendMessage(hListLeft, LB_GETCOUNT, 0, 0);//�õ����б����Ŀ¼�ĸ���
			for (int i = 0; i < count; i++)
			{
				SendMessage(hListLeft, LB_GETTEXT, i, (long)szDirectoryPath);//��õ�ǰ������·��
				backupDirectorys.push_back(szDirectoryPath);//��·�����뱸��Ŀ¼
			}
			count = SendMessage(hListRight, LB_GETCOUNT, 0, 0);//�õ����б����Ŀ¼�ĸ���
			for (int i = 0; i < count; i++)
			{
				SendMessage(hListRight, LB_GETTEXT, i, (long)szDirectoryPath);//��õ�ǰ������·��
				temp.push_back(szDirectoryPath);//��·��������ʱ����
			}
			for (auto iter = currentDirectorys.begin(); iter != currentDirectorys.end();)//����ǰĿ¼�����б���в����ڵ�Ŀ¼ɾ��
			{
				auto result = find(temp.begin(), temp.end(), *iter);//�����б���в��ҵ�ǰĿ¼
				if (result == temp.end())//�Ҳ���
				{
					iter = currentDirectorys.erase(iter);//ɾ�����Ŀ¼��������һ��������
					int index = iter - currentDirectorys.begin();//��õ�ǰ������������
					AllDirectorys.erase(AllDirectorys.begin() + index);//ɾ����ǰĿ¼��������Ϣ
				}
				else
					iter++;//�ҵ��˲�ɾ����������һ��
			}
			for (auto iter = temp.begin(); iter != temp.end(); iter++)//���б����Ŀ¼�ڵ�ǰĿ¼�в����ڵĲ��뵱ǰĿ¼
			{
				auto result = find(currentDirectorys.begin(), currentDirectorys.end(), *iter);//���б����Ŀ¼�ڵ�ǰĿ¼�в���
				if (result ==currentDirectorys.end())//�Ҳ��������뵱ǰĿ¼
				{
					tempDirectory.clear();//����һ����ӵ�Ŀ¼��������ʱ�ļ������ļ�·�����
					currentDirectorys.push_back(*iter);//����ǰĿ¼�в����ڵ�Ŀ¼����
					FindAllFile(*iter, includeSubDirectory);//������Ŀ¼������ͼƬ
					AllDirectorys.push_back(tempDirectory);//����ǰĿ¼��������Ϣ���
					
				}
			}
			DisplayDistribitue();//�����ļ�������ʾ
			EndDialog(hDlg, 0);//���ٶԻ���
			break;
		}
		return true;
	case WM_CLOSE:
		EndDialog(hDlg, 0);//���ٶԻ���
		return false;
	}
	return false;
}
void NextWallPaper()//��һ�ű�ֽ
{
	if (randomChange)//����������
		nSelectWallpaper = rand() % wallPaperCount + 1;//���ѡһ��ͼƬ
	else
	{
		nSelectWallpaper = nSelectWallpaper + 1;//��һ��ͼƬ
		if (nSelectWallpaper >= wallPaperCount + 1||nSelectWallpaper<=1)//��������һ�Ź���,���߳�����Χ
			nSelectWallpaper = 1;//�ص���ͷ
	}
}
void SetStartingUpBoot()//���ÿ�������
{
	HKEY m_regkey;//ע�����
	TCHAR filename[MAX_PATH];
	GetModuleFileName(NULL, filename, MAX_PATH);//���س����ȫ·��
	TCHAR lpRun[] = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");//�Ӽ�·��
	long iRet=RegOpenKeyEx(HKEY_LOCAL_MACHINE,lpRun,0,KEY_WRITE, &m_regkey);//��������ע����
	if (iRet == ERROR_SUCCESS)//�򿪳ɹ�
	{
		if (startingUpBoot)//��������
		    RegSetValueEx(m_regkey,TEXT("���»���ֽ"), 0, REG_SZ, (const unsigned char *)filename, MAX_PATH);//����ǰ����·����ӵ�ע�����������
		else
			RegDeleteValue(m_regkey, TEXT("���»���ֽ"));//ɾ��ע���
	}
	RegCloseKey(m_regkey);//�ر�ע���
}
void InitWindow(HWND hwnd,HINSTANCE hInst)//����Ĭ�ϳ�ʼ��
{
	int width, heigh;//���ڿ�ȡ��߶�
	hBKBrush = CreateSolidBrush(textBkColor);//����������ˢ���ɫ
	InitBrowseInfo(hwnd, bi);//��ʼ��ѡ��Ŀ¼�ṹ
	InitFont(hwnd, cf,lf);//��ʼ������ṹ
	InitColor(hwnd, cc, color);//��ʼ����ɫ�ṹ
	InitNotifyIconData(hwnd, hInst, nid);//��ʼ������ͼ��ṹ
	WM_FIND = RegisterWindowMessage(FINDMSGSTRING);//���Һ��滻�Ի���ᷢ����Ϣ����FINDMSGSTRINGע�����Ϣ		
	Shell_NotifyIcon(NIM_ADD, &nid);//�������������һ��ͼ��
	hMenu = CreateMyPopMenu(hInst);//�����˵�
	blueBrush = CreateSolidBrush(RGB(102, 179, 255));//��ɫ��ˢ
	hFont = CreateFontIndirect(&lf);//�����߼����崴��������
	//hFont= CreateFont(0, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);//Ĭ������
	hStatus = CreateWindow(STATUSCLASSNAME, 0, WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, hwnd, (HMENU)IDS_STATE, hInst, nullptr);//����״̬��
	hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL | ES_READONLY | ES_MULTILINE | ES_NOHIDESEL, 0, 0, 0, 0,
		hwnd, (HMENU)ID_EDIT, hInst, nullptr);//�����б߽�Ķ���ֻ���༭��ES_NOHIDESEL�ڱ༭��ʧȥ�����ѡ�е��ı���Ȼ���ָ�����ʾ
	SetClassLong(hEdit, GCLP_HCURSOR, (long)LoadCursor(nullptr, IDC_ICON));//���������
	SendMessage(hEdit, EM_LIMITTEXT, 0, 0);//��wParamΪ0ʱ�����б༭�ز����0x7FFFFFFE���ַ���Ĭ����30000���ַ�
	SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);//��������
	oldEditTextProc = (WNDPROC)SetWindowLong(hEdit, GWL_WNDPROC, (long)EditTextProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ��̡������Ϣ����Ϊ
	//Ĭ���ǲ��񲻵���Щ��Ϣ�ģ���Ҫ�õ���Щ��Ϣ�����統���򰴼�����ʱ����״̬����ʾ��������Ϣ
	//���ǲ�����SetClassLong(hEdit,GCL_WNDPROC),��Ϊ�����޸�edit��Ĵ��ڹ��̣���ֻ�����һ�������ı༭����Ч���Ե�ǰ
	//�༭�򲻻�ı䣬������SetClassLong,��Ӱ���Ժ�ı༭�򣬶���ֻ��ı䵱ǰ�༭�򣬲���ı������༭��
	srand(time(0));//����ǰʱ����Ϊ�����������
	wallPaperTime=calculateTime(Wallpaper);//����ʱ�������
	tipTime = calculateTime(Notify);
	if (startChange)//�����ʼ��
	SetTimer(hwnd, wallpaperID, wallPaperTime, nullptr);//���ñ�ֽ��ʱ��
	SetTimer(hwnd, tipID, tipTime, nullptr);//����������ʾ��ʱ��
	SetTimer(hwnd, clockID, 1000, nullptr);//����ʱ�Ӷ�ʱ����ÿһ�����ʱ��һ��
	width = windowRect.right - windowRect.left;//���ڿ��
	heigh = windowRect.bottom - windowRect.top;//���ڸ߶�
	if (width>0&&heigh>0&&windowRect.left>=0&&windowRect.top>=0)//�����ȡ�Ŀ�Ⱥ͸߶Ȳ�Ϊ0
	MoveWindow(hwnd, windowRect.left, windowRect.top,width,heigh ,true);//�ƶ����ڣ���ʾ��һ�δ���������λ�ô�С
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//������ ���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ
	HDC hdc;//����
	RECT rect;//����
	int size;//�����С
	static POINT mousePos;//���λ��
	static HBRUSH oldBrush;//����ɻ�ˢ
	static HINSTANCE hInst;
	static bool bDown;//���°�ť�Ƿ���
	static TCHAR szFindText[MAX_PATH]=TEXT("");//Ҫ���ҵ��ַ���
	int xPos[4]{0, 0, 0, 0};//״̬�����ĸ��Һ�����
	static FINDREPLACE fr;//���Һ��滻�ṹ
	LPFINDREPLACE pfr = nullptr;//���Һ��滻�ṹ��ָ��
	switch (UMsg)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;//����ʵ�����
		GetPictureAndDocumentPath(hwnd);//�õ��ҵ��ĵ����ҵ�ͼƬ·��
		ReadSaveFile();//��ȡ�û�����,����д浵����Ĭ�����ø���Ϊ�û�����
		InitWindow(hwnd,hInst);//��ʼ�����ڸ�������
		if (currentDirectorys.size()!= 0)//����б�ֽ·��
		{
			for (int i=0; i < currentDirectorys.size(); i++)//��������Ŀ¼��ͼƬ
			{
				tempDirectory.clear();//����һ����ӵ�Ŀ¼��������ʱ�ļ������ļ�·�����
				FindAllFile(currentDirectorys[i], includeSubDirectory);//���ҵ�ǰĿ¼������ͼƬ
				AllDirectorys.push_back(tempDirectory);//����ǰĿ¼��������Ϣ���
			}
			DisplayDistribitue();//��ʾ�ļ�����
		}
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
		if (nSelectWallpaper > 0)//���������ĳ�ű�ֽ
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
			AddWapperDirectory(hwnd);//��ӱ�ֽĿ¼
			break;
		case IDM_MODIFYWAPPERDIRECTORY://�޸ı�ֽ·��
			DialogBox(hInst,TEXT("IDD_DIALOGMODIFYDIRECTORY"),hwnd,DlgModifyDirectoryProc);//�����޸ı�ֽ·���Ի���
			break;
		case IDM_EDIT_FIND: //����
			hDlgFind = FindDialog(hwnd,fr, szFindText);//�������ҶԻ������
			break;
		case IDM_EDIT_FINDNEXT://������һ��
			if (szFindText[0] == '\0')//�����û��Ҫ���ҵ��ַ���
				hDlgFind =FindDialog(hwnd,fr, szFindText);//�������ҶԻ�����������û������ַ�������
			else
			{
				if (!MyFindText(fr, bDown))//������һ��
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
		case IDM_WALLPAPERSET:
			DialogBox(hInst, TEXT("IDD_DIALOGSET"), hwnd, DlgWallpaperSet);
			break;
		case IDM_SHOWWALLPAPER://�鿴�����ֽ
			ShowWindow(hwnd, SW_NORMAL);//��ʾ����
			SetForegroundWindow(hwnd);//�����ڷŵ�ǰ̨�����ö�
			break;
		case IDM_PAUSE://��ͣ
			if (startChange)//����ѿ�ʼ����ֽ���������ͣ����ʾ������ܿ�ʼ
			{
				ModifyMenu(hMenu, IDM_PAUSE, MF_BYCOMMAND, IDM_PAUSE, TEXT("��ʼ\tCtrl+P"));//��ʾ������ܿ�ʼ
				startChange = false;//��ͣ���
				KillTimer(hwnd, wallpaperID);//ɱ������ֽ��ʱ��
			}
			else
			{
				ModifyMenu(hMenu, IDM_PAUSE, MF_BYCOMMAND, IDM_PAUSE, TEXT("��ͣ\tCtrl+P"));
				startChange = true;//��ʼ���
				SetTimer(hwnd,wallpaperID, wallPaperTime, 0);//��������ֽ��ʱ��
			}
			break;
		case IDM_PREWALLPAPER://��һ��
			recordIndex--;//��һ�ŵļ�¼����
			if (recordIndex>=0)
			nSelectWallpaper = recordChange[recordIndex];//������һ�ű�ֽ������
			HighLineWallPaper(hEdit, nSelectWallpaper);//������ʾ
			ChangeWallPaper();//�ı��ֽ
			break;
		case IDM_NEXTWALLPAPER://��һ��
			if (recordIndex == recordChange.size() - 1 || recordChange.size() == 0)//��ǰ��ֽ�����±�ֽ��������ǰ�����Ļ��߻���ֽ��¼Ϊ0
			{
				NextWallPaper();//������һ�ű�ֽ����
				recordChange.push_back(nSelectWallpaper);//���滻���ı�ֽ�еļ�¼�е�����
				recordIndex = recordChange.size() - 1;//��ǰ��ֽ�ڻ����ı�ֽ�еļ�¼�е����������һ������
			}
			else
			{
				recordIndex++;//��һ�ŵļ�¼����
				nSelectWallpaper = recordChange[recordIndex];//������һ�ű�ֽ������
			}
			HighLineWallPaper(hEdit, nSelectWallpaper);//������ʾ
			ChangeWallPaper();//�ı��ֽ
			break;
		case IDM_RANDOM://����ֽģʽ�����������˳��
			if (randomChange)//��������������ģʽ�������˳�򻻣���ʾ����������
			{
				ModifyMenu(hMenu, IDM_RANDOM, MF_BYCOMMAND, IDM_RANDOM, TEXT("���"));//��ʾ����������
				randomChange = false;//˳�򻻱��
			}
			else
			{
				ModifyMenu(hMenu, IDM_RANDOM, MF_BYCOMMAND, IDM_RANDOM, TEXT("˳��"));
				randomChange = true;//��������
			}
			break;
		case IDM_MINI://���д�����С���Ÿ�����ֽ�˵�
			if (changeByOnlyDestop)
			{
				CheckMenuItem(hMenu, IDM_MINI, MF_UNCHECKED);//�����д�����С���Ÿ�����ֽ�˵���ȥ����
				changeByOnlyDestop = false;//���д�����С���Ÿ����ڱ��Ϊfalse
			}
			else
			{
				CheckMenuItem(hMenu, IDM_MINI, MF_CHECKED);//�����д�����С���Ÿ�����ֽ�˵��ϴ��Ϲ�
				changeByOnlyDestop = true;
			}
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
			{
				HWND hDestop = GetDesktopWindow();
				HDC  hdcScr = GetDCEx(hDestop, 0, DCX_LOCKWINDOWUPDATE);//�����Ļ�Ļ���,��ʹ��Ļ��סҲ��ˢ��
				//TextOut(hdcScr, 20, 400, temp.c_str(), temp.size());
			}
			if (wallPaperCount > 0)//ͼƬ������Ϊ0
			{
				if (changeByOnlyDestop)//�����������������ڲŻ���ֽ
				{
					if (allTitle.size() == 1)//���ֻ��1�����ڼ�����Window Manager����������ʾ
					{//��Ϊֻ�����洰�ڣ����Գ����ں�̨���У����ø�����ʾ
						NextWallPaper();//������һ�ű�ֽ����
						ChangeWallPaper();//�ı��ֽ
						recordChange.push_back(nSelectWallpaper);//���滻���ı�ֽ�еļ�¼�е�����
						recordIndex = recordChange.size() - 1;//��ǰ��ֽ�ڻ����ı�ֽ�еļ�¼�е����������һ������
					}
				}
				else
				{
					NextWallPaper();//������һ�ű�ֽ����
					HighLineWallPaper(hEdit, nSelectWallpaper);//�Ե�ǰ��ֽ������ʾ
					ChangeWallPaper();//�ı��ֽ
					recordChange.push_back(nSelectWallpaper);//���滻���ı�ֽ�еļ�¼�е�����
					recordIndex = recordChange.size() - 1;//��ǰ��ֽ�ڻ����ı�ֽ�еļ�¼�е����������һ������
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
		GetWindowRect(hwnd, &windowRect);//��ô��ڴ�С��Ϣ
		WriteSaveFile();//����ֽ����Ϣ���浽�浵
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
				if (!MyFindText(*pfr, bDown))//������һ��
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
	if (currentDirectorys.size() == 0)//���û�б�ֽĿ¼
	{
		MessageBox(hwnd, TEXT("���ڡ��ļ���->����ӱ�ֽĿ¼���������Ŀ¼"), TEXT("û�б�ֽĿ¼"), MB_OK);
		SendMessage(hwnd, WM_COMMAND, ID_ADDWAPPER, 0);//������Ϣ���Ŀ¼
	}
	HACCEL hAccel = LoadAccelerators(hInstance, TEXT("WALLPAPER"));//����Դ�ļ���ȡ���ټ����
	MSG msg;//��Ϣ�ṹ
	RegisterHotKey(0, ID_HOTKEY,MOD_CONTROL|MOD_ALT, 'M');//����ϵͳ�ȼ���ϵͳ��ݼ�����ʹ�ں�̨���У�Ҳ���յ���ݼ���Ϣ),������ʾ����
	while (GetMessage(&msg, nullptr, 0, 0))//����Ϣ���л�ȡ��Ϣ
	{
		if (msg.message == WM_HOTKEY)//�����ϵͳ�ȼ�
			SendMessage(hwnd, WM_COMMAND,IDM_SHOWWALLPAPER, 0);//������Ϣ����ʾ����
		if (!TranslateAccelerator(hwnd, hAccel, &msg))////TranslateAccelerator���������ܣ�������ټ����ú�������˵������еļ��ټ�
		{
			TranslateMessage(&msg);//��һЩ������Ϣת�����ַ���Ϣ���ڰ�����Ϣ����
			DispatchMessage(&msg);//�ַ���Ϣ�������ô��ڹ��̽��д�����Ϣ
		}
	}
}
