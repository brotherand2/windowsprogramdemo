#include"filebat.h"


#include<ZYHFile.h>
#include<ZYHGlobal.h>
//int _stdcall EditTextProc(HWND hEdit, unsigned message, unsigned wParam, long lPram);//��д�༭�ؼ����ڹ���

extern WNDPROC oldEditTextProc;//����ԭ�༭��Ĵ��ڹ���
void openDirectory(TCHAR *szDirectoryPath)
{
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.lpVerb = TEXT("open");//�������Թ���Ա�������
	sei.lpFile = szDirectoryPath;//�ļ�·��
	sei.nShow = SW_SHOWMAXIMIZED;
	ShellExecuteEx(&sei);
}
static unsigned int digitNum;//���ָ���
static unsigned int letterNum;//��ĸ����
static unsigned int firstNum;//��ʼ����
static unsigned int directoryNum;//Ŀ¼����
static unsigned int fileNum;//�ļ�����
static tstring suffixName = TEXT("*.*");//�ļ�����׺��
static tstring prefix, suffix;//�ļ���ǰ׺��׺
//����������ļ���
tstring makeRandomFileName()
{
	static TCHAR c[53] = TEXT("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	if (digitNum == 0 && letterNum == 0)
		digitNum = letterNum = 5;
	TCHAR name[200];
	for (int i = 0; i < digitNum; i++)
		name[i] = rand() % 10 + TEXT('0');
	for (int i = digitNum; i < digitNum + letterNum; i++)
		name[i] = c[rand() % 52];
	name[digitNum + letterNum] = TEXT('\0');
	return tstring(name);
}

static vector<tstring> otherSuffix;//���������ļ���׺

static bool haveDir = false;//��Ŀ¼
bool includeSub;//�Ƿ������Ŀ¼
bool isOtherSuffix = false;//�Ƿ������ļ���׺
bool isRandom = false;//�Ƿ����
bool isDeleteTheSameSizeFile = false;//ɾ����ͬ��С�ļ�
std::vector<WIN32_FIND_DATA> vect;
enum  operation
{
	renameFileName, deleteFile, deleteDirectory
};
bool sortSize(WIN32_FIND_DATA &fd1, WIN32_FIND_DATA &fd2)
{
	return(fd1.nFileSizeLow < fd2.nFileSizeLow);
}
//ɾ��ָ����ʽ��ͬ��С�ļ�
void deleteSameSizeFile(tstring CurrentPath)
{
	int count = 0;//�ļ�����
	int repeat = 0;//�ظ�����
	vector<WIN32_FIND_DATA>::iterator iter;
	sort(vect.begin(), vect.end(), sortSize);
	if (vect.size() >= 2)
	{
		for (iter = vect.begin(); iter != vect.end() - 1; iter++)
		{
			count++;

			if ((*iter).nFileSizeLow == (*(iter + 1)).nFileSizeLow)//��ǰ�ļ�����һ���ļ���С��ͬ
			{
				repeat++;
				DeleteFile((*iter).cFileName);//ɾ����ǰ�ļ�
			}
		}
		//if(repeat)
		//Message(TEXT("��ǰĿ¼:%s,�ļ�����:%d,ɾ���ظ�����:%d"),CurrentPath.c_str(),count + 1, repeat);
	}
}
void FindFile(tstring CurrentPath, tstring type, operation oper)//�����ļ�
{
	WIN32_FIND_DATA fd;
	tstring find;//��ǰҪ�����ļ���ʽ��·��
	find = CurrentPath + TEXT("\\") + type;//typeָ���ļ���Ҫ���ҵ����ͣ���d:\\wallpaper\\*.jpg
	HANDLE handle = FindFirstFile(find.c_str(), &fd);//���ҵ�1���ļ�
	SetCurrentDirectory(CurrentPath.c_str());
	if (handle != INVALID_HANDLE_VALUE)//�ҵ���
	{
		do
		{
			if (fd.cFileName[0] == '.')//win32��һ����������ļ��У�һ���ǡ�.��ָ���ǵ�ǰ�ļ��У�һ���ǡ�..��ָ�����ϲ��ļ��У����һ�־�����ͨ���²��ļ������ļ�������ʾ��cd .�Ǳ�Ŀ¼��cd ..���ϼ�Ŀ¼
				continue;//����Ϊ�˲��ص��ϼ�Ŀ¼����ԭĿ¼��ת������.��..�����Ŀ¼
			switch (oper)
			{
			case renameFileName:
			{
				TCHAR *extention = PathFindExtension(fd.cFileName);
				string newFileName;
				if (!isRandom)
					newFileName = prefix + ToString(firstNum++) + suffix + extention;
				else
					newFileName = prefix + makeRandomFileName() + suffix + extention;
#ifdef UNICODE
				rename(fd.cFileName, WCharToAnsi(newFileName).c_str());
#else
				rename(fd.cFileName, newFileName.c_str());
#endif
			}
				break;
			case deleteFile:
				if (fd.dwFileAttributes& FILE_ATTRIBUTE_DIRECTORY)//ֻҪFILE_ATTRIBUTE_DIRECTORY=0x00000010����ֻҪ����������5λΪ1�������ļ�Ŀ¼������λ�Ǳ�ֻ����ֻд�����ص�����
					continue;
				if (!isDeleteTheSameSizeFile)
					DeleteFile(fd.cFileName);
				else
					vect.push_back(fd);//���浱ǰ�ļ��м�¼
				break;
			case deleteDirectory:
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//ֻҪFILE_ATTRIBUTE_DIRECTORY��0x00000010����ֻҪ����������5λΪ1�������ļ�Ŀ¼������λ�Ǳ��ֻ����ֻд�����ص�����
					RemoveDirectory(fd.cFileName);
				break;
			default:
				break;
			}
		} while (FindNextFile(handle, &fd));//������һ���ļ�
		FindClose(handle);//�ر��ļ����
	}
}
void FindAllFile(tstring CurrentPath, tstring fileType, operation oper)//�������б�ֽ
{//CurrentPath��ǰ·��,includeSubDirectoryΪtrue����ʾ��ǰĿ¼����Ŀ¼�ı�ֽ��Ϊfalse��ֻ��ʾ��ǰĿ¼��ֽ
	WIN32_FIND_DATA fd;//fd�ļ���Ϣ�ṹ
	HANDLE handle;//handle�ļ�����
	tstring find;//formatָ���ļ���Ҫ���ҵ����ͣ���d:\\wallpaper\\*.jpg
	vect.clear();
	if (lstrcmp(TEXT("other"), fileType.c_str()) == 0)
	{
		for (size_t i = 0; i < otherSuffix.size(); i++)//�������и�ʽ(jpg,bmp,png)��ͼƬ
		{
			FindFile(CurrentPath, tstring(TEXT("*.")) + otherSuffix[i], oper);//���ҵ�ǰ�ļ���ʽ��·��
			if (oper == deleteFile&&isDeleteTheSameSizeFile)
				deleteSameSizeFile(CurrentPath);
		}
	}
	else
	{
		FindFile(CurrentPath, fileType, oper);//���ҵ�ǰ�ļ���ʽ��·��
		if (oper == deleteFile&&isDeleteTheSameSizeFile)
			deleteSameSizeFile(CurrentPath);
	}
	if (includeSub)//���Ҫ������Ŀ¼��ͼƬ
	{
		find = CurrentPath + TEXT("\\*.*");//���������ļ�
		handle = FindFirstFile(find.c_str(), &fd);//���ҵ�1���ļ�
		if (handle != INVALID_HANDLE_VALUE)//�ҵ���
		{
			do
			{
				if (fd.cFileName[0] == '.')//win32��һ����������ļ��У�һ���ǡ�.��ָ���ǵ�ǰ�ļ��У�һ���ǡ�..��ָ�����ϲ��ļ��У����һ�־�����ͨ���²��ļ������ļ�������ʾ��cd .�Ǳ�Ŀ¼��cd ..���ϼ�Ŀ¼
					continue;//����Ϊ�˲��ص��ϼ�Ŀ¼����ԭĿ¼��ת������.��..�����Ŀ¼
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//ֻҪFILE_ATTRIBUTE_DIRECTORY��0x00000010����ֻҪ����������5λΪ1�������ļ�Ŀ¼������λ�Ǳ��ֻ����ֻд�����ص�����
					FindAllFile(CurrentPath + TEXT("\\") + tstring(fd.cFileName), fileType, oper);
			} while (FindNextFile(handle, &fd));//FindFirstFile����ʱָ����һ���ļ���������һ���ļ�
			FindClose(handle);//�ر��ļ����
		}
	}
}
int _stdcall DlgFileBatProc(HWND hDlg, UINT message, UINT wParam, long lParam)//�ļ�������Ի������
{
	HDC hdc, hdcMem;//����
	static HBITMAP hBitmap;//λͼ���
	BITMAP bm;//λͼ�ṹ
	static HBRUSH hBrush;
	//��ť���
	static HWND hBTNRENAME;
	static HWND hBTNCREATEDIRECTORY;
	static HWND hBTNDELETEFILE;
	static HWND hBTNDELETEDIRECTORY;
	static HWND hBTNCREATEFILE;
	static HWND hBTNADDSUFFIX;
	static HWND hBTNDELETESUFFIX;

	//�༭����
	static HWND hEditFileDirectory;//Ŀ¼�༭����
	static HWND hEditDIGITNUM;
	static HWND hEditLETTERNUM;
	static HWND hEditFIRSTNUM;
	static HWND hEditCREATEDIRECTORYNUM;
	static HWND hEditCREATEFILENUM;
	static HWND hEditADDOTHERSUFFIX;

	static HWND hDELETEINCLUDESUB;//������Ŀ¼ ��ť
	static HWND hLISTSUFFIX;//�б��

	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	int wmID;//�ؼ�ID
	int wmEvent;//�ؼ��ϵĲ���
	static TCHAR szDirectoryPath[MAX_PATH];//����Ŀ¼·��
	TCHAR szBuffer[64];//����
	int index = 0;
	switch (message)
	{
	case WM_INITDIALOG://�Ի����ʼ��
		//��ʾͼƬ�����static�ؼ������ʼ��
		//�õ������ؼ����
		hBTNADDSUFFIX = GetDlgItem(hDlg, IDC_ADDSUFFIXOK);
		hBTNCREATEDIRECTORY = GetDlgItem(hDlg, IDC_CREATEDIRECTORYOK);
		hBTNCREATEFILE = GetDlgItem(hDlg, IDC_CREATEFILEOK);
		hBTNDELETEFILE = GetDlgItem(hDlg, IDC_DELETEOK);
		hBTNDELETEDIRECTORY = GetDlgItem(hDlg, IDC_DELETEEMPTYDIRECTORY);
		hBTNRENAME = GetDlgItem(hDlg, IDC_RENAMEOK);
		hBTNDELETESUFFIX = GetDlgItem(hDlg, IDC_DELETESUFFIX);
		hEditFileDirectory = GetDlgItem(hDlg, IDC_FILEDIRECTORY);
		hEditFIRSTNUM = GetDlgItem(hDlg, IDC_FIRSTNUM);
		hEditLETTERNUM = GetDlgItem(hDlg, IDC_LETTERNUM);
		hEditDIGITNUM = GetDlgItem(hDlg, IDC_DIGITNUM);
		hEditCREATEDIRECTORYNUM = GetDlgItem(hDlg, IDC_CREATEDIRECTORYNUM);
		hEditCREATEFILENUM = GetDlgItem(hDlg, IDC_CREATEFILENUM);
		hEditADDOTHERSUFFIX = GetDlgItem(hDlg, IDC_ADDOTHERSUFFIX);

		hDELETEINCLUDESUB = GetDlgItem(hDlg, IDC_DELETEINCLUDESUB);
		hLISTSUFFIX = GetDlgItem(hDlg, IDC_LISTSUFFIX);

		SendMessage(GetDlgItem(hDlg, IDC_SUFFIXALLFILE), BM_SETCHECK, 1, 0);//ѡ��*.*��ѡ��ť
		oldEditTextProc = (WNDPROC)SetWindowLong(hEditDIGITNUM, GWL_WNDPROC, (long)EditTextProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ�����Ϣ
		SetWindowLong(hEditCREATEDIRECTORYNUM, GWL_WNDPROC, (long)EditTextProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ�����Ϣ
		SetWindowLong(hEditFIRSTNUM, GWL_WNDPROC, (long)EditTextProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ�����Ϣ
		SetWindowLong(hEditLETTERNUM, GWL_WNDPROC, (long)EditTextProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ�����Ϣ
		SetWindowLong(hEditCREATEFILENUM, GWL_WNDPROC, (long)EditTextProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ�����Ϣ
		SendMessage(hEditLETTERNUM, EM_LIMITTEXT, 2, 0);
		SendMessage(hEditDIGITNUM, EM_LIMITTEXT, 2, 0);
		hBitmap = LoadBitmap((HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_BITMAP12));//����λͼ
		hBrush = CreatePatternBrush(hBitmap);
		EnableWindow(hBTNADDSUFFIX, false);
		EnableWindow(hBTNDELETESUFFIX, false);
		srand(GetTickCount());
		break;
	case WM_COMMAND:
		wmID = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmID)
		{
		case IDC_FILEDIRECTORY://��ȡ�ļ�Ŀ¼
		{
			switch (wmEvent)//�ڱ༭���ϵĲ���
			{
			case EN_UPDATE://�༭���ϵ����ݸ�����
				GetWindowText(hEditFileDirectory, szDirectoryPath, MAX_PATH);//�õ��༭�����ַ���
				if (szDirectoryPath[0] != '\0')//����ַ�����Ϊ��
				{
					haveDir = true;//��Ŀ¼
					//�ָ�ԭ���İ�ť״̬�����༭��Ϊ�����ð�ť����
					GetWindowText(hEditCREATEDIRECTORYNUM, szBuffer, 64);
					if (szBuffer[0] != '\0')
						EnableWindow(hBTNCREATEDIRECTORY, true);
					GetWindowText(hEditCREATEFILENUM, szBuffer, 64);
					if (szBuffer[0] != '\0')
						EnableWindow(hBTNCREATEFILE, true);
					GetWindowText(hEditFIRSTNUM, szBuffer, 64);
				}
				else
				{
					haveDir = false;
					//��ʾ��ť��״̬������
					EnableWindow(hBTNCREATEDIRECTORY, haveDir);
					EnableWindow(hBTNCREATEFILE, haveDir);
				}
				EnableWindow(hBTNDELETEFILE, haveDir);
				EnableWindow(hBTNDELETEDIRECTORY, haveDir);
				EnableWindow(hBTNRENAME, haveDir);
				EnableWindow(hBTNDELETEDIRECTORY, haveDir);
				EnableWindow(hBTNDELETEFILE, haveDir);
				break;
			}
		}
			break;
		case IDC_DIGITNUM://������ָ���
		{
			if (wmEvent == EN_UPDATE)
				digitNum = GetDlgItemInt(hDlg, wmID, 0, false);
			if (wmEvent == EN_MAXTEXT)
				showBallonTip(hEditDIGITNUM, L"�������ܴ��ڵ���100", L"�ļ�������̫��");
		}
			break;
		case IDC_LETTERNUM://�����ĸ����
		{
			if (wmEvent == EN_UPDATE)
				letterNum = GetDlgItemInt(hDlg, wmID, 0, false);
			if (wmEvent == EN_MAXTEXT)
				showBallonTip(hEditLETTERNUM, L"�������ܴ��ڵ���100", L"�ļ�������̫��");
		}
			break;
		case IDC_FIRSTNUM://�����ʼ����
		{
			if (wmEvent == EN_UPDATE)
				firstNum = GetDlgItemInt(hDlg, wmID, 0, false);
		}
			break;
		case IDC_RANDOMFILENAME:
			isRandom = SendMessage(GetDlgItem(hDlg, wmID), BM_GETCHECK, 0, 0);
			break;
		case IDC_DELETEINCLUDESUB:
			includeSub = SendMessage(GetDlgItem(hDlg, wmID), BM_GETCHECK, 0, 0);
			break;
		case IDC_DELETESAMESIZEFILE:
			isDeleteTheSameSizeFile = SendMessage(GetDlgItem(hDlg, wmID), BM_GETCHECK, 0, 0);
			break;
		case  IDC_FILENAMEPREFIX:
			if (wmEvent == EN_UPDATE)
			{
				GetDlgItemText(hDlg, wmID, szBuffer, 64);
				prefix = szBuffer;
			}
			break;
		case  IDC_FILENAMESUFFIX:
			if (wmEvent == EN_UPDATE)
			{
				GetDlgItemText(hDlg, wmID, szBuffer, 64);
				suffix = szBuffer;
			}
			break;

		case IDC_CREATEDIRECTORYNUM://���Ŀ¼����
		{
			if (wmEvent == EN_UPDATE)
			{
				//�༭���ϵ����ݸ�����
				GetWindowText(hEditCREATEDIRECTORYNUM, szBuffer, 64);//�õ��༭�����ַ���
				if (szBuffer[0] != '\0')//�ַ�����Ϊ��
				{
					if (haveDir)//�����Ŀ¼
						EnableWindow(hBTNCREATEDIRECTORY, haveDir);
					directoryNum = GetDlgItemInt(hDlg, wmID, 0, false);
				}
				else
					EnableWindow(hBTNCREATEDIRECTORY, false);

				break;
			}
		}
			break;
		case IDC_CREATEFILENUM://����ļ�����
		{
			if (wmEvent == EN_UPDATE)
			{
				//�༭���ϵ����ݸ�����
				GetWindowText(hEditCREATEFILENUM, szBuffer, 64);//�õ��༭�����ַ���
				if (szBuffer[0] != '\0')//�ַ�����Ϊ��
				{
					if (haveDir)//�����Ŀ¼
						EnableWindow(hBTNCREATEFILE, haveDir);
					fileNum = GetDlgItemInt(hDlg, wmID, 0, false);
				}
				else
					EnableWindow(hBTNCREATEFILE, false);
				break;
			}
			break;
		}
		case IDC_ADDOTHERSUFFIX://���������׺��
		{
			switch (wmEvent)
			{
			case EN_UPDATE://�༭���ϵ����ݸ�����
				GetWindowText(hEditADDOTHERSUFFIX, szBuffer, 64);//�õ��༭�����ַ���
				if (szBuffer[0] != '\0'&&lstrcmp(TEXT("other"), suffixName.c_str()) == 0)//�ַ�����Ϊ��
					EnableWindow(hBTNADDSUFFIX, true);
				else
					EnableWindow(hBTNADDSUFFIX, false);
				break;
			}
		}
			break;
		case IDC_ADDDIRECTORY:
			bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_USENEWUI | BIF_RETURNONLYFSDIRS | BIF_BROWSEFORCOMPUTER;
			plDlist = SHBrowseForFolder(&bi);//����ѡ��Ŀ¼�Ի���,·�������ָ��plDlist
			if (plDlist)//���ѡ����һ��Ŀ¼
			{
				SHGetPathFromIDList(plDlist, szDirectoryPath);//��plDlist·�����ṹ�壩ת�����ַ���·��	
				SetWindowText(hEditFileDirectory, szDirectoryPath);//�ڱ༭������ʾ·��
				if (SHGetMalloc(&pMalloc) == S_OK);//����IMALLOC����
				{
					pMalloc->Free(plDlist);//�ͷ�plDlistָ��Ŀռ�
					pMalloc->Release();//�ͷ�pMalloc�Ŀռ�
				}
			}
			break;
		case IDC_ADDSUFFIXOK://��Ӻ�׺
		{
			GetWindowText(hEditADDOTHERSUFFIX, szBuffer, 64);//�õ��༭�����ַ���
			auto result = find(otherSuffix.begin(), otherSuffix.end(), szBuffer);
			if (result == otherSuffix.end())
			{
				otherSuffix.push_back(szBuffer);
				SendMessage(hLISTSUFFIX, LB_INSERTSTRING, 0, (long)szBuffer);
				SetWindowText(hEditADDOTHERSUFFIX, TEXT(""));
			}
		}
			break;
		case IDC_DELETESUFFIX://ɾ����׺
			index = SendMessage(hLISTSUFFIX, LB_GETCURSEL, 0, 0);//��õ�ǰѡ��Ŀ¼������
			if (index != LB_ERR)
			{
				SendMessage(hLISTSUFFIX, LB_GETTEXT, index, (long)szBuffer);//�õ��༭�����ַ���
				auto result = find(otherSuffix.begin(), otherSuffix.end(), szBuffer);
				SendMessage(hLISTSUFFIX, LB_DELETESTRING, index, 0);//ɾ��
				if (result != otherSuffix.end())
					otherSuffix.erase(result);
			}
			break;
		case IDC_CREATEFILEOK:
		{
			tstring first = tstring(szDirectoryPath) + TEXT("\\");
			firstNum = GetDlgItemInt(hDlg, IDC_FIRSTNUM, 0, false);
			tstring command;
			TCHAR  fileExtention[64];
			TCHAR writeContent[] = TEXT("little busters!!!");
			DWORD readWirte;
			GetDlgItemText(hDlg, IDC_FILEEXTENTION, fileExtention, 64);
			for (int i = 0; i < fileNum; i++)
			{
				if (isRandom)
					command = first + prefix + makeRandomFileName() + suffix;
				else
					command = first + prefix + ToString(firstNum + i) + suffix;
				if (fileExtention[0] != '\0');
				command = command + TEXT(".") + fileExtention;
				HANDLE handle = CreateFile(command.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
				if (handle != INVALID_HANDLE_VALUE)
					WriteFile(handle, writeContent, lstrlen(writeContent)*sizeof(TCHAR), &readWirte, 0);
				CloseHandle(handle);
			}
			openDirectory(szDirectoryPath);
		}
			break;
		case IDC_CREATEDIRECTORYOK:
		{
			tstring first = tstring(szDirectoryPath) + TEXT("\\");
			firstNum = GetDlgItemInt(hDlg, IDC_FIRSTNUM, 0, false);
			tstring command;
			if (directoryNum)
			{
				for (int i = 0; i < directoryNum; i++)
				{
					if (isRandom)
						command = first + prefix + makeRandomFileName() + suffix;
					else
						command = first + prefix + ToString(firstNum + i) + suffix;
					CreateDirectory(command.c_str(), 0);
				}
			}
			openDirectory(szDirectoryPath);
		}
			break;
		case  IDC_RENAMEOK://������
		{
			firstNum = GetDlgItemInt(hDlg, IDC_FIRSTNUM, 0, false);
			FindAllFile(szDirectoryPath, suffixName, renameFileName);
			openDirectory(szDirectoryPath);
		}
			break;
		case IDC_DELETEEMPTYDIRECTORY:
		{
			FindAllFile(szDirectoryPath, TEXT("*.*"), deleteDirectory);
			openDirectory(szDirectoryPath);
		}
			break;
		case IDC_DELETEOK:
		{
			FindAllFile(szDirectoryPath, suffixName, deleteFile);
			openDirectory(szDirectoryPath);
		}
			break;
		case IDC_SUFFIXALLFILE:
			suffixName = TEXT("*.*");
			EnableWindow(hBTNADDSUFFIX, false);
			break;
		case IDC_SUFFIXJPG:
			suffixName = TEXT("*.jpg");
			EnableWindow(hBTNADDSUFFIX, false);
			break;
		case IDC_SUFFIXOTHER:
			suffixName = TEXT("other");
			GetWindowText(hEditADDOTHERSUFFIX, szBuffer, 64);//�õ��༭�����ַ���
			if (szBuffer[0] != '\0')//�ַ�����Ϊ��
				EnableWindow(hBTNADDSUFFIX, true);
			EnableWindow(hBTNDELETESUFFIX, true);
			break;
		case IDC_SUFFIXPNG:
			suffixName = TEXT("*.png");
			EnableWindow(hBTNADDSUFFIX, false);
			break;
		case IDC_SUFFIXTXT:
			suffixName = TEXT("*.txt");
			EnableWindow(hBTNADDSUFFIX, false);
			break;

		}
		break;
	case WM_CTLCOLORDLG:
		hdc = (HDC)wParam;//wParam�Ǳ༭��Ļ��ʣ�lParam�Ǳ༭��ľ��
		return (long)hBrush;//���ﲻ����break,�������������DefWindowProc������ɫ���Ĭ�ϵģ�����һ����ˢ���༭���Ĭ�ϴ��ڹ��̻��������ˢ��Ϊ����ɫ
	case WM_CTLCOLORSTATIC:
		hdc = (HDC)wParam;//wParam�Ǳ༭��Ļ��ʣ�lParam�Ǳ༭��ľ��
		SetBkColor(hdc, RGB(240, 207, 219));//�������屳����ɫ��Ӧ��༭�򱳾���ɫһ�£��������2�ֱ���ɫ���ÿ�
		return (long)CreateSolidBrush(RGB(240, 207, 219));
	case WM_PAINT:
		break;
	case WM_CLOSE:
		DeleteObject(hBitmap);
		EndDialog(hDlg, 0);
		break;
	}
	return false;
}