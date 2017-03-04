#include"filebat.h"


#include<ZYHFile.h>
#include<ZYHGlobal.h>
//int _stdcall EditTextProc(HWND hEdit, unsigned message, unsigned wParam, long lPram);//重写编辑控件窗口过程

extern WNDPROC oldEditTextProc;//保留原编辑框的窗口过程
void openDirectory(TCHAR *szDirectoryPath)
{
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.lpVerb = TEXT("open");//动作，以管理员身份运行
	sei.lpFile = szDirectoryPath;//文件路径
	sei.nShow = SW_SHOWMAXIMIZED;
	ShellExecuteEx(&sei);
}
static unsigned int digitNum;//数字个数
static unsigned int letterNum;//字母个数
static unsigned int firstNum;//起始数字
static unsigned int directoryNum;//目录个数
static unsigned int fileNum;//文件个数
static tstring suffixName = TEXT("*.*");//文件名后缀名
static tstring prefix, suffix;//文件名前缀后缀
//产生随机的文件名
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

static vector<tstring> otherSuffix;//其它类型文件后缀

static bool haveDir = false;//有目录
bool includeSub;//是否包含子目录
bool isOtherSuffix = false;//是否其它文件后缀
bool isRandom = false;//是否随机
bool isDeleteTheSameSizeFile = false;//删除相同大小文件
std::vector<WIN32_FIND_DATA> vect;
enum  operation
{
	renameFileName, deleteFile, deleteDirectory
};
bool sortSize(WIN32_FIND_DATA &fd1, WIN32_FIND_DATA &fd2)
{
	return(fd1.nFileSizeLow < fd2.nFileSizeLow);
}
//删除指定格式相同大小文件
void deleteSameSizeFile(tstring CurrentPath)
{
	int count = 0;//文件个数
	int repeat = 0;//重复个数
	vector<WIN32_FIND_DATA>::iterator iter;
	sort(vect.begin(), vect.end(), sortSize);
	if (vect.size() >= 2)
	{
		for (iter = vect.begin(); iter != vect.end() - 1; iter++)
		{
			count++;

			if ((*iter).nFileSizeLow == (*(iter + 1)).nFileSizeLow)//当前文件与下一个文件大小相同
			{
				repeat++;
				DeleteFile((*iter).cFileName);//删除当前文件
			}
		}
		//if(repeat)
		//Message(TEXT("当前目录:%s,文件个数:%d,删除重复个数:%d"),CurrentPath.c_str(),count + 1, repeat);
	}
}
void FindFile(tstring CurrentPath, tstring type, operation oper)//查找文件
{
	WIN32_FIND_DATA fd;
	tstring find;//当前要查找文件格式的路径
	find = CurrentPath + TEXT("\\") + type;//type指定文件夹要查找的类型，如d:\\wallpaper\\*.jpg
	HANDLE handle = FindFirstFile(find.c_str(), &fd);//查找第1个文件
	SetCurrentDirectory(CurrentPath.c_str());
	if (handle != INVALID_HANDLE_VALUE)//找到了
	{
		do
		{
			if (fd.cFileName[0] == '.')//win32下一般会有三种文件夹，一种是“.”指的是当前文件夹，一种是“..”指的是上层文件夹，最后一种就是普通的下层文件夹用文件夹名显示，cd .是本目录，cd ..是上级目录
				continue;//所以为了不回到上级目录和在原目录打转，过滤.和..这二个目录
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
				if (fd.dwFileAttributes& FILE_ATTRIBUTE_DIRECTORY)//只要FILE_ATTRIBUTE_DIRECTORY=0x00000010，即只要从右数，第5位为1，即是文件目录，其它位是标只读、只写、隐藏等属性
					continue;
				if (!isDeleteTheSameSizeFile)
					DeleteFile(fd.cFileName);
				else
					vect.push_back(fd);//保存当前文件夹记录
				break;
			case deleteDirectory:
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//只要FILE_ATTRIBUTE_DIRECTORY是0x00000010，即只要从右数，第5位为1，即是文件目录，其它位是标记只读、只写、隐藏等属性
					RemoveDirectory(fd.cFileName);
				break;
			default:
				break;
			}
		} while (FindNextFile(handle, &fd));//查找下一个文件
		FindClose(handle);//关闭文件句柄
	}
}
void FindAllFile(tstring CurrentPath, tstring fileType, operation oper)//查找所有壁纸
{//CurrentPath当前路径,includeSubDirectory为true则显示当前目录、子目录文壁纸，为false则只显示当前目录壁纸
	WIN32_FIND_DATA fd;//fd文件信息结构
	HANDLE handle;//handle文件名柄
	tstring find;//format指定文件夹要查找的类型，如d:\\wallpaper\\*.jpg
	vect.clear();
	if (lstrcmp(TEXT("other"), fileType.c_str()) == 0)
	{
		for (size_t i = 0; i < otherSuffix.size(); i++)//查找所有格式(jpg,bmp,png)的图片
		{
			FindFile(CurrentPath, tstring(TEXT("*.")) + otherSuffix[i], oper);//查找当前文件格式的路径
			if (oper == deleteFile&&isDeleteTheSameSizeFile)
				deleteSameSizeFile(CurrentPath);
		}
	}
	else
	{
		FindFile(CurrentPath, fileType, oper);//查找当前文件格式的路径
		if (oper == deleteFile&&isDeleteTheSameSizeFile)
			deleteSameSizeFile(CurrentPath);
	}
	if (includeSub)//如果要查找子目录的图片
	{
		find = CurrentPath + TEXT("\\*.*");//查找所有文件
		handle = FindFirstFile(find.c_str(), &fd);//查找第1个文件
		if (handle != INVALID_HANDLE_VALUE)//找到了
		{
			do
			{
				if (fd.cFileName[0] == '.')//win32下一般会有三种文件夹，一种是“.”指的是当前文件夹，一种是“..”指的是上层文件夹，最后一种就是普通的下层文件夹用文件夹名显示，cd .是本目录，cd ..是上级目录
					continue;//所以为了不回到上级目录和在原目录打转，过滤.和..这二个目录
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//只要FILE_ATTRIBUTE_DIRECTORY是0x00000010，即只要从右数，第5位为1，即是文件目录，其它位是标记只读、只写、隐藏等属性
					FindAllFile(CurrentPath + TEXT("\\") + tstring(fd.cFileName), fileType, oper);
			} while (FindNextFile(handle, &fd));//FindFirstFile函数时指定的一个文件名查找下一个文件
			FindClose(handle);//关闭文件句柄
		}
	}
}
int _stdcall DlgFileBatProc(HWND hDlg, UINT message, UINT wParam, long lParam)//文件批处理对话框过程
{
	HDC hdc, hdcMem;//画笔
	static HBITMAP hBitmap;//位图句柄
	BITMAP bm;//位图结构
	static HBRUSH hBrush;
	//按钮句柄
	static HWND hBTNRENAME;
	static HWND hBTNCREATEDIRECTORY;
	static HWND hBTNDELETEFILE;
	static HWND hBTNDELETEDIRECTORY;
	static HWND hBTNCREATEFILE;
	static HWND hBTNADDSUFFIX;
	static HWND hBTNDELETESUFFIX;

	//编辑框句柄
	static HWND hEditFileDirectory;//目录编辑框句柄
	static HWND hEditDIGITNUM;
	static HWND hEditLETTERNUM;
	static HWND hEditFIRSTNUM;
	static HWND hEditCREATEDIRECTORYNUM;
	static HWND hEditCREATEFILENUM;
	static HWND hEditADDOTHERSUFFIX;

	static HWND hDELETEINCLUDESUB;//包含子目录 按钮
	static HWND hLISTSUFFIX;//列表框

	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	int wmID;//控件ID
	int wmEvent;//控件上的操作
	static TCHAR szDirectoryPath[MAX_PATH];//保存目录路径
	TCHAR szBuffer[64];//缓冲
	int index = 0;
	switch (message)
	{
	case WM_INITDIALOG://对话框初始化
		//显示图片区域的static控件句柄初始化
		//得到各个控件句柄
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

		SendMessage(GetDlgItem(hDlg, IDC_SUFFIXALLFILE), BM_SETCHECK, 1, 0);//选中*.*单选按钮
		oldEditTextProc = (WNDPROC)SetWindowLong(hEditDIGITNUM, GWL_WNDPROC, (long)EditTextProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘消息
		SetWindowLong(hEditCREATEDIRECTORYNUM, GWL_WNDPROC, (long)EditTextProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘消息
		SetWindowLong(hEditFIRSTNUM, GWL_WNDPROC, (long)EditTextProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘消息
		SetWindowLong(hEditLETTERNUM, GWL_WNDPROC, (long)EditTextProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘消息
		SetWindowLong(hEditCREATEFILENUM, GWL_WNDPROC, (long)EditTextProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘消息
		SendMessage(hEditLETTERNUM, EM_LIMITTEXT, 2, 0);
		SendMessage(hEditDIGITNUM, EM_LIMITTEXT, 2, 0);
		hBitmap = LoadBitmap((HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_BITMAP12));//加载位图
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
		case IDC_FILEDIRECTORY://获取文件目录
		{
			switch (wmEvent)//在编辑框上的操作
			{
			case EN_UPDATE://编辑框上的内容更新了
				GetWindowText(hEditFileDirectory, szDirectoryPath, MAX_PATH);//得到编辑框上字符串
				if (szDirectoryPath[0] != '\0')//如果字符串不为空
				{
					haveDir = true;//有目录
					//恢复原来的按钮状态，即编辑框不为空则让按钮可用
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
					//显示按钮的状态不可用
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
		case IDC_DIGITNUM://获得数字个数
		{
			if (wmEvent == EN_UPDATE)
				digitNum = GetDlgItemInt(hDlg, wmID, 0, false);
			if (wmEvent == EN_MAXTEXT)
				showBallonTip(hEditDIGITNUM, L"个数不能大于等于100", L"文件名不能太长");
		}
			break;
		case IDC_LETTERNUM://获得字母个数
		{
			if (wmEvent == EN_UPDATE)
				letterNum = GetDlgItemInt(hDlg, wmID, 0, false);
			if (wmEvent == EN_MAXTEXT)
				showBallonTip(hEditLETTERNUM, L"个数不能大于等于100", L"文件名不能太长");
		}
			break;
		case IDC_FIRSTNUM://获得起始数字
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

		case IDC_CREATEDIRECTORYNUM://获得目录个数
		{
			if (wmEvent == EN_UPDATE)
			{
				//编辑框上的内容更新了
				GetWindowText(hEditCREATEDIRECTORYNUM, szBuffer, 64);//得到编辑框上字符串
				if (szBuffer[0] != '\0')//字符串不为空
				{
					if (haveDir)//如果有目录
						EnableWindow(hBTNCREATEDIRECTORY, haveDir);
					directoryNum = GetDlgItemInt(hDlg, wmID, 0, false);
				}
				else
					EnableWindow(hBTNCREATEDIRECTORY, false);

				break;
			}
		}
			break;
		case IDC_CREATEFILENUM://获得文件个数
		{
			if (wmEvent == EN_UPDATE)
			{
				//编辑框上的内容更新了
				GetWindowText(hEditCREATEFILENUM, szBuffer, 64);//得到编辑框上字符串
				if (szBuffer[0] != '\0')//字符串不为空
				{
					if (haveDir)//如果有目录
						EnableWindow(hBTNCREATEFILE, haveDir);
					fileNum = GetDlgItemInt(hDlg, wmID, 0, false);
				}
				else
					EnableWindow(hBTNCREATEFILE, false);
				break;
			}
			break;
		}
		case IDC_ADDOTHERSUFFIX://获得其它后缀名
		{
			switch (wmEvent)
			{
			case EN_UPDATE://编辑框上的内容更新了
				GetWindowText(hEditADDOTHERSUFFIX, szBuffer, 64);//得到编辑框上字符串
				if (szBuffer[0] != '\0'&&lstrcmp(TEXT("other"), suffixName.c_str()) == 0)//字符串不为空
					EnableWindow(hBTNADDSUFFIX, true);
				else
					EnableWindow(hBTNADDSUFFIX, false);
				break;
			}
		}
			break;
		case IDC_ADDDIRECTORY:
			bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_USENEWUI | BIF_RETURNONLYFSDIRS | BIF_BROWSEFORCOMPUTER;
			plDlist = SHBrowseForFolder(&bi);//弹出选择目录对话框,路径保存的指针plDlist
			if (plDlist)//如果选择了一个目录
			{
				SHGetPathFromIDList(plDlist, szDirectoryPath);//将plDlist路径（结构体）转换成字符串路径	
				SetWindowText(hEditFileDirectory, szDirectoryPath);//在编辑框上显示路径
				if (SHGetMalloc(&pMalloc) == S_OK);//创建IMALLOC对象
				{
					pMalloc->Free(plDlist);//释放plDlist指向的空间
					pMalloc->Release();//释放pMalloc的空间
				}
			}
			break;
		case IDC_ADDSUFFIXOK://添加后缀
		{
			GetWindowText(hEditADDOTHERSUFFIX, szBuffer, 64);//得到编辑框上字符串
			auto result = find(otherSuffix.begin(), otherSuffix.end(), szBuffer);
			if (result == otherSuffix.end())
			{
				otherSuffix.push_back(szBuffer);
				SendMessage(hLISTSUFFIX, LB_INSERTSTRING, 0, (long)szBuffer);
				SetWindowText(hEditADDOTHERSUFFIX, TEXT(""));
			}
		}
			break;
		case IDC_DELETESUFFIX://删除后缀
			index = SendMessage(hLISTSUFFIX, LB_GETCURSEL, 0, 0);//获得当前选中目录的索引
			if (index != LB_ERR)
			{
				SendMessage(hLISTSUFFIX, LB_GETTEXT, index, (long)szBuffer);//得到编辑框上字符串
				auto result = find(otherSuffix.begin(), otherSuffix.end(), szBuffer);
				SendMessage(hLISTSUFFIX, LB_DELETESTRING, index, 0);//删除
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
		case  IDC_RENAMEOK://重命名
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
			GetWindowText(hEditADDOTHERSUFFIX, szBuffer, 64);//得到编辑框上字符串
			if (szBuffer[0] != '\0')//字符串不为空
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
		hdc = (HDC)wParam;//wParam是编辑框的画笔，lParam是编辑框的句柄
		return (long)hBrush;//这里不能用break,否则会调用下面的DefWindowProc，将颜色设回默认的，返回一个画刷，编辑框的默认窗口过程会用这个画刷作为背景色
	case WM_CTLCOLORSTATIC:
		hdc = (HDC)wParam;//wParam是编辑框的画笔，lParam是编辑框的句柄
		SetBkColor(hdc, RGB(240, 207, 219));//设置字体背景颜色，应与编辑框背景颜色一致，否则存在2种背景色不好看
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