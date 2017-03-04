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
TCHAR szMyPictures[MAX_PATH];//电脑上我的图片文件夹路径
TCHAR szMyDocument[MAX_PATH];//电脑上我的文档文件夹路径
TCHAR szDirectoryName[MAX_PATH];//用户选择的目录名
LPITEMIDLIST plDlist;//保存用户选择的目录信息结构
LPMALLOC pMalloc;//LPMALLOC是一个IMalloc指针，IMALLOC实现了一个非常完整的内存分配引擎，用于释放ITEMIDLIST指针所指向的空间
BROWSEINFO bi;//包含有用户选中目录的重要信息


//得到我的文档和我的图片路径
void GetPictureAndDocumentPath(HWND hwnd)
{
	SHGetSpecialFolderLocation(hwnd, CSIDL_MYPICTURES, &plDlist);//获得我的图片文件夹路径
	SHGetPathFromIDList(plDlist, szMyPictures);//将plDlist路径（结构体）转换成字符串路径
	SHGetSpecialFolderLocation(hwnd, CSIDL_PERSONAL, &plDlist);//获得我的文档文件夹路径
	SHGetPathFromIDList(plDlist, szMyDocument);//将plDlist路径（结构体）转换成字符串路径
	if (SHGetMalloc(&pMalloc) == S_OK)//创建IMALLOC对象
	{
		pMalloc->Free(plDlist);//释放plDlist指向的空间
		pMalloc->Release();//释放pMalloc的空间
	}
}
//在我的文档创建存档目录,返回值true表示已有该目录
bool CreateArchiveDirectory(HWND hwnd, TCHAR*archiveName)
{
	if (!archiveName)
		return false;
	if (szMyDocument[0] == '\0' || szMyDocument == nullptr)
		GetPictureAndDocumentPath(hwnd);//我的文档为空，进行获取
	WIN32_FIND_DATA fd;//文件结构
	tstring find = tstring(szMyDocument) +tstring(TEXT("\\"))+ archiveName;
	HANDLE handle = FindFirstFile(find.c_str(), &fd);//查找第1个文件
	if (handle == INVALID_HANDLE_VALUE || (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)//如果找不到文件夹wallpaper或者找到的不是目录
	{
		SetCurrentDirectory(szMyDocument);//设置当前目录
		CreateDirectory(archiveName, 0);//创建目录
		return false;
	}
	return true;
}
//返回可执行文件的目录
void GetAppDirectory(char *modulePath)
{
	GetModuleFileNameA(NULL, modulePath, MAX_PATH);//得到可执行文件路径
	char *separator = strrchr(modulePath, TEXT('\\'));
	*separator = TEXT('\0');
}
typedef struct
{
	unsigned int G, M, K, B;//分别代表GB,MB,KB,BYTE
}capacity;//容量结构

//计算文件容量大小
tstring calculateCapacity(long fileSizeHigh, long fileSizeLow)
{
	DWORD64 size = 0;//容量大小
	LARGE_INTEGER  large;//大整数结构
	large.LowPart = fileSizeLow;//低32位
	large.HighPart = fileSizeHigh;//高32位
	memcpy(&size, &large, sizeof(DWORD64));//将文件大小低位，高位拷贝到size上，计算文件大小
	tstring calculate;//保存容量大小的字符串
	capacity c;//保存容量大小的结构
	c.G = size / (1024 * 1024 * 1024);//求GB大小
	if (c.G)//大小GB级别
		calculate += ToString(c.G) + TEXT("Gb,");
	size %= (1024 * 1024 * 1024);//对GB取余
	c.M = size / (1024 * 1024);//剩余的MB大小
	if (c.M)//大小MB级别
		calculate += ToString(c.M) + TEXT("Mb,");
	size %= (1024 * 1024);//对KB取余
	c.K = size / 1024;//剩余的KB大小
	if (c.K)//大小KB级别
		calculate += ToString(c.K) + TEXT("Kb,");
	size %= 1024;//对byte取余
	c.B = size;//剩余的byte大小
	calculate += ToString(c.B) + TEXT("byte");
	return calculate;
}
//文件是否比最低要求大小小
bool isLessThanMiniSize(long fileSizeHigh, long fileSizeLow,UINT miniSize )
{
	DWORD64 size = 0;//容量大小
	LARGE_INTEGER  large;//大整数结构
	large.LowPart = fileSizeLow;//低32位
	large.HighPart = fileSizeHigh;//高32位
	memcpy(&size, &large, sizeof(DWORD64));//将文件大小低位，高位拷贝到size上，计算文件大小
	if (size < miniSize * 1024)
		return true;
	else
		return false;
}
//获得当前目录的盘符
char getDriveLetter(){
	char drive = '\0';
	TCHAR current[MAX_PATH];

	::GetCurrentDirectory(MAX_PATH, current);
	int driveNbr = ::PathGetDriveNumber(current);
	if (driveNbr != -1)
		drive = 'A' + char(driveNbr);

	return drive;
}
//检查文件或文件夹是否已存在
bool checkFileExists(const TCHAR * fileName) {
	TCHAR fullpath[MAX_PATH];
	::GetFullPathName(fileName, MAX_PATH, fullpath, NULL);
	if (::PathFileExists(fullpath)) {
		return true;
	}
	return false;
}
tstring FormatFileTime(FILETIME &ft)//格式化文件时间
{
	FILETIME temp;//文件时间结构
	FileTimeToLocalFileTime(&ft, &temp);//将国际文件时间转换成本地文件时间
	SYSTEMTIME st;//时间结构
	FileTimeToSystemTime(&temp, &st);//将本地文件时间转换成系统时间
	tstringstream out;//字符串流
	out << st.wYear << TEXT("年") << st.wMonth << TEXT("月") << st.wDay << TEXT("日,") << st.wHour << TEXT(":") << st.wMinute << TEXT(":") << st.wSecond;//格式化字符串
	return out.str();//返回格式化后的字符串
}
tstring FileAttribute(long attribute)//文件属性
{
	tstring attri;
	if (attribute&FILE_ATTRIBUTE_ARCHIVE)
		attri += TEXT("存档、");//一个文件被创建之后，系统会自动将其设置成归档属性，这个属性常用于文件的备份
	if (attribute&FILE_ATTRIBUTE_COMPRESSED)
		attri += TEXT("压缩、");
	if (attribute&FILE_ATTRIBUTE_DIRECTORY)
		attri += TEXT("目录、");
	if (attribute&FILE_ATTRIBUTE_DEVICE)
		attri += TEXT("驱动、");
	if (attribute&FILE_ATTRIBUTE_ENCRYPTED)
		attri += TEXT("加密、");
	if (attribute&FILE_ATTRIBUTE_HIDDEN)
		attri += TEXT("隐藏、");
	if (attribute&FILE_ATTRIBUTE_OFFLINE)
		attri += TEXT("脱机、");
	if (attribute&FILE_ATTRIBUTE_READONLY)
		attri += TEXT("只读");
	if (attribute&FILE_ATTRIBUTE_SYSTEM)
		attri += TEXT("系统、");
	if (attribute&FILE_ATTRIBUTE_TEMPORARY)
		attri += TEXT("临时、");
	if (attribute&FILE_ATTRIBUTE_NORMAL)
		attri += TEXT("普通");
	attri += TEXT("文件");
	return attri;
}
//选择文件夹对话框回调函数  
int CALLBACK BrowseCallBackFun(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR currentSelectPath[MAX_PATH];//保存当前选择的路径
	switch (uMsg)
	{
	case BFFM_INITIALIZED://选择文件夹对话框完成初始化，对话框最后一条消息 
		SendMessage(hwnd, BFFM_SETSELECTION, true, lpData);//从BROWSEINFO中的lParam传来的默认路径在lpData中，将wParam设为true,lParam设为以'\0'为尾字符串则可设置默认打开目录
		SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, lpData);//设置状态栏上文本的我的图片路径
		SetWindowText(hwnd, TEXT("选择图片文件夹"));
		break;
	case BFFM_SELCHANGED://选择其它文件夹后
		SHGetPathFromIDList((LPCITEMIDLIST)lParam, currentSelectPath);//lParam参数是一个LPCITEMIDLIST结构，转换成字符串的路径
		SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (long)currentSelectPath);//设置状态栏上文本的新路径
		if (currentSelectPath[0] != '\0')//如果不是空字符串
			lstrcpyn(szMyPictures, currentSelectPath, MAX_PATH);//设置当前图片路径为新的路径
		break;
	}
	return false;
}

void InitBrowseInfo(HWND hwnd, BROWSEINFO &bi)// 初始化打开目录结构
{
	bi.hwndOwner = hwnd;//浏览文件夹对话框的父窗体句柄
	bi.iImage = 0;// 被选择的文件夹的图片序号
	bi.lParam = (long)szMyPictures;//对话框传递给回调函数的一个参数指针,传给回调函数的参数,第一次设置默认路径为我的图片文件夹
	bi.lpfn = BrowseCallBackFun;//应用程序定义的浏览对话框回调函数的地址。当对话框中的事件发生时，该对话框将调用回调函数。该参数可用为NULL
	bi.lpszTitle = TEXT("你来了，我等你很久了，先选择一个图片文件夹先");//该浏览文件夹对话框的显示文本，用来提示该浏览文件夹对话框的功能、作用和目的
	bi.pidlRoot = 0;// ITEMIDLIST结构的地址，包含浏览时的初始根目录，而且只有被指定的目录和其子目录才显示在浏览文件夹对话框中。该成员变量可以是NULL，在此时桌面目录将被使用
	bi.pszDisplayName = szDirectoryName;//用来保存用户选中的目录字符串的内存地址。该缓冲区的大小缺省是定义的MAX_PATH常量宏
	bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS;//该标志位描述了对话框的选项。它可以为0
	//BIF_EDITBOX：浏览对话框中包含一个编辑框，在该编辑框中用户可以输入选中项的名字
	//BIF_STATUSTEXT：在对话框中包含一个状态区域。通过给对话框发送消息使回调函数设置状态文本
	//BIF_RETURNONLYFSDIRS：仅仅返回文件系统的目录。例如：在浏览文件夹对话框中，当选中任意一个目录时，该“OK”按钮可用，而当选中“我的电脑”或“网上邻居”等非有意义的节点时，“OK”按钮为灰色
}



TCHAR szTip[128] = TEXT("咄，呆子看什么");//气泡的提示信息
TCHAR szInfoTitle[64] = TEXT("对着屏幕那么久，该出去休息休息了，注意眼睛");//字符串的内容为气球提示的标题。此标题出现在气球提示框的上部，最多含有63个字符。
TCHAR szInfo[256] = TEXT("现在换壁纸了，在回到桌面的时候，每2秒换一次壁纸");////字符串的内容为气球提示内容。最多含有255个字符
//初始化托盘结构
void InitNotifyIconData(HWND hwnd, HINSTANCE hInst, NOTIFYICONDATA &nid, int pictureID,bool closeBubble)
{
	nid.cbSize = sizeof(nid);//结构体的大小，以字节为单位
	nid.dwInfoFlags = NIIF_USER;//设置此成员用来给气球提示框增加一个图标，NIIF_USER是用户的图标，即成员nid.hIcon
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(pictureID));//读取托盘用的图标，增加、修改或删除的图标的句柄
	if(closeBubble)
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;//此成员表明具体哪些其他成员为合法数据（即哪些成员起作用）
	else
	   nid.uFlags = NIF_ICON | NIF_INFO | NIF_MESSAGE | NIF_TIP;//此成员表明具体哪些其他成员为合法数据（即哪些成员起作用）
	//NIF_ICON图标，NIF_INFO,使用气球提示代替普通的工具提示框。szInfo, uTimeout, szInfoTitle和dwInfoFlags成员起作用
	//NIF_MESSAGE uCallbackMessage成员起作用
	//NIF_TIP  szTip成员起作用
	nid.hWnd = hwnd;//窗口的句柄。标示的窗口用来接收与托盘图标相关的消息
	nid.uID = 0;//应用程序定义的任务栏图标的标识符
	StringCchCopy(nid.szInfo, 256, szInfo);//字符串的内容为气球提示内容。最多含有255个字符
	StringCchCopy(nid.szInfoTitle, 64, szInfoTitle);//字符串的内容为气球提示的标题。此标题出现在气球提示框的上部，最多含有63个字符。
	StringCchCopy(nid.szTip, 128, szTip);//指向一个以\0结束的字符串的指针。字符串的内容为标准工具提示的信息
	nid.uCallbackMessage = WM_USER;//应用程序定义的消息标示。当托盘图标区域发生鼠标事件或者使用键盘选择或激活图标时，系统将使用此标示向由hWnd成员标示的窗口发送消息
	//WM_USER 为了防止用户定义的消息ID与系统的消息ID冲突，MS（Microsoft）定义了一个宏WM_USER，小于WM_USER的ID被系统使用，大于WM_USER的ID被用户使用
}
//初始化颜色结构
void InitColor(HWND hwnd, CHOOSECOLOR &cc, COLORREF color[])
{
	cc.lStructSize = sizeof(cc);//指定结构的长度（字节
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;//一个可以让你初始化颜色对话框的位集。当对话框返回时，它用来这些标识符来标识用户的输入
	//CC_FULLOPEN让对话框显示额外的控件以使用户创建自定义的颜色
	//CC_RGBINIT让对话框默认使用由rgbResult成员指定的颜色
	cc.hInstance = nullptr;
	cc.hwndOwner = hwnd;//拥有对话框的窗口的句柄。该成员可以是任意有效的窗口句柄，或在对话框没有所有者时，可为NULL
	cc.lCustData = 0;//指定应用程序自定义的数据，该数据会被系统发送给钩子程序
	cc.lpfnHook = 0;//指向CCHookProc钩子程序的指针，该钩子可以处理发送给对话框的消息
	cc.lpTemplateName = 0;//指向一个NULL结尾的字符串，该字符串是对话框模板资源的名字
	cc.rgbResult = RGB(255, 0, 0);//如果CC_RGBINIT标识符被设置时，该成员指定了对话框打开时默认的选择颜色
	cc.lpCustColors = color;//指向一个包含16个值的数组，该数组包含了对话框中自定义颜色的红、绿、蓝（RGB）值
}
//初始化字体结构
void InitFont(HWND hwnd, CHOOSEFONT &cf, LOGFONT &lf)
{
	cf.Flags = CF_NOVERTFONTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;// CF_EFFECTS |使对话框显示控件允许用户指定下划线,和文本颜色选项。 如果设置此标志,您可以使用rgbColors成员指定初始文本颜色
	// CF_NOVERTFONTS导致字体对话框列表只面向水平的字体,或未设置则是垂直的
	cf.hDC = nullptr;
	cf.hInstance = nullptr;
	cf.hwndOwner = hwnd;
	cf.iPointSize = 0;//选择字体的大小
	cf.lCustData = 0;
	cf.lpfnHook = 0;
	cf.lpLogFont = &lf;//逻辑字体
	cf.lStructSize = sizeof(cf);
	cf.lpszStyle = 0;//字体风格
	cf.lpTemplateName = 0;
	cf.nFontType = 0;//返回的字体类型
	cf.nSizeMax = 0;//用户可以选择最大点大小
	cf.nSizeMin = 0;// 用户可以选择最小点大小
	cf.rgbColors = 0;//初始字体对话框文本颜色
}
HWND FindDialog(HWND hwnd, FINDREPLACE &fr, TCHAR szFindText[])//查找结构
{
	fr.Flags = FR_DOWN | FR_HIDEWHOLEWORD;//初始化查找替换对话框的标记
	//FR_DOWN初始化向下按钮被按下，FR_HIDEWHOLEWORD不显示全字匹配
	fr.hInstance = nullptr;
	fr.hwndOwner = hwnd;//拥有对话框的窗口句柄
	fr.lCustData = 0;//用户定义的数据
	fr.lpfnHook = 0;
	fr.lpstrFindWhat = szFindText;//要查找的字符串
	fr.lpstrReplaceWith = 0;//要替换的字符串
	fr.lpTemplateName = 0;
	fr.lStructSize = sizeof(fr);//结构大小
	fr.wFindWhatLen = MAX_PATH;//要查找的字符串szFindText的缓冲区空间
	fr.wReplaceWithLen = 0;//要替换的字符串szFindText的缓冲区空间
	return FindText(&fr);//进行查找
}