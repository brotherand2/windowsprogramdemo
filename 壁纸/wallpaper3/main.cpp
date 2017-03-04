#include"resource.h"
#include<shlobj.h>
BROWSEINFO bi;//包含有用户选中目录的重要信息
LPITEMIDLIST plDlist;//保存用户选择的目录信息结构
LPMALLOC pMalloc;//LPMALLOC是一个IMalloc指针，IMALLOC实现了一个非常完整的内存分配引擎，用于释放ITEMIDLIST指针所指向的空间

bool CreateWallpaperDirectory()//在我的文档创建wallpaper目录
{
	WIN32_FIND_DATA fd;//文件结构
	tstring find = tstring(szMyDocument)+ TEXT("\\wallpaper");
	HANDLE handle = FindFirstFile(find.c_str(), &fd);//查找第1个文件
	if (handle == INVALID_HANDLE_VALUE || (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)//如果找不到文件夹wallpaper或者找到的不是目录
	{
		SetCurrentDirectory(szMyDocument);//设置当前目录
		CreateDirectory(TEXT("wallpaper"), 0);//创建目录
		return false;
	}
	return true;
}
void ReadDiretoryPath()//读取壁纸路径
{
	TCHAR szBuffer[MAX_PATH];
	tstring save = tstring(szMyDocument) + TEXT("\\wallpaper\\backUpDirectorys.lu");//存档文件全路径
	ifstream fin(save,ios::in| ios::binary);//输入流,找开文件backUpDirectorys.lu,以二进制方式读入
	if (!fin.is_open())
		return;
	while (fin)
	{
		fin.getline(szBuffer,MAX_PATH,'\n');//读取一行字符串
		if (szBuffer[0] != '\0')//如果不空
		backupDirectorys.push_back(szBuffer);//保存每个壁纸目录
	}
	fin.close();
	save = tstring(szMyDocument) + TEXT("\\wallpaper\\currentDirectorys.lu");//存档文件全路径
	fin.open(save, ios::binary|ios::in);
	if (!fin.is_open())
		return;
	while (fin)
	{
		fin.getline(szBuffer, MAX_PATH, '\n');//读取一行字符串
		if (szBuffer[0] != '\0')//如果不空
		currentDirectorys.push_back(szBuffer);//保存每个壁纸目录
	}
	fin.close();
}
bool ReadSaveFile()//读取存档文件
{
	WIN32_FIND_DATA fd;//文件结构
	TCHAR szBuffer[MAX_PATH];
	int count;//路径个数
	Time timeTemp;//时间结构临时变量
	tstring find = tstring(szMyDocument) + TEXT("\\wallpaper\\save.lu");//要查找的存档文件全路径
	if (!CreateWallpaperDirectory())//如果没有目录创建目录
		return false;//目录中无存档退出
	HANDLE handle = FindFirstFile(find.c_str(), &fd); //查找存档文件save.lu
	if (handle == INVALID_HANDLE_VALUE || fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//如果找不到，或找到的是目录save.lu
		return false;
	fstream fin(find, ios_base::in | ios_base::binary);//输入流，以进制方式读
	if (!fin.is_open())//打开失败
		return true;
	//读取各种配置属性

	fin.read((char*)&timeTemp, sizeof(Time));//读取壁纸更换时间结构
	if (timeTemp.hour < 1000 && timeTemp.minute < 1000 && timeTemp.second < 1000)//将时间范围限制在1000，因为每个成员是unsigned int，一定是正数，如果存档被破坏，读取错误可能产生很大的整数如538976288
		Wallpaper = timeTemp;//保存时间
	fin.read((char*)&timeTemp, sizeof(Time));//读取气泡时间结构
	if (timeTemp.hour < 1000 && timeTemp.minute < 1000 && timeTemp.second < 1000)//将时间范围限制在1000，因为每个成员是unsigned int，一定是正数，如果存档被破坏，读取错误可能产生很大的整数如538976288
		Notify = timeTemp;//保存时间
	fin.read((char*)&randomChange, sizeof(bool));//读取是否随机布尔值
	fin.read((char*)&startingUpBoot, sizeof(bool));//读取是否开机自启动布尔值
	fin.read((char*)&changeByOnlyDestop, sizeof(bool));//读取是否仅有桌面这个窗口才换壁纸布尔值
	fin.read((char*)&includeSubDirectory, sizeof(bool));//读取是否包含子目录布尔值
	fin.read((char*)&startChange, sizeof(bool));//读取是否开始换布尔值
	fin.read((char*)&startingUpBoot, sizeof(bool));//读取是否开机启动标记
	//读取气泡信息
	fin.read((char*)&szTip, sizeof(szTip));
	fin.read((char*)&szInfo, sizeof(szInfo));
	fin.read((char*)&szInfoTitle, sizeof(szInfoTitle));
	fin.read((char*)&windowRect, sizeof(RECT));//读取窗口大小位置
	fin.read((char*)&lf, sizeof(LOGFONT));//读取字体结构
	fin.read((char*)&textColor, sizeof(COLORREF));//读取颜色
	fin.read((char*)&textBkColor, sizeof(COLORREF));//读取背景颜色
	fin.read((char*)&nSelectWallpaper, sizeof(UINT));//读取当前壁纸索引
	fin.read((char*)&color, sizeof(color));//读取自定义的颜色数组
	ReadDiretoryPath();//读取壁纸路径
	fin.close();
	return true;
}
void SaveDiretoryPath()//保存壁纸路径
{
	tstring save = tstring(szMyDocument) + TEXT("\\wallpaper\\backUpDirectorys.lu");//存档文件全路径
	int count;//路径个数
	ofstream fout(save, ios::binary);//输出流,找开文件save.lu,以二进制方式写入
	if (!fout.is_open())
		return;
	count = backupDirectorys.size();//备份文件路径数量
	for (int i = 0; i < count; i++)
		fout<<backupDirectorys[i].c_str()<<endl;//写入每个壁纸目录
	fout.close();
	save = tstring(szMyDocument) + TEXT("\\wallpaper\\currentDirectorys.lu");//存档文件全路径
	fout.open(save, ios::binary);
	if (!fout.is_open())
		return;
	count = currentDirectorys.size();//当前壁纸路径数量
	for (int i = 0; i < count; i++)
		fout<<currentDirectorys[i].c_str()<<endl;//写入每个壁纸目录
	fout.close();
}
void WriteSaveFile()//保存在存档文件
{
	tstring save = tstring(szMyDocument) + TEXT("\\wallpaper\\save.lu");//存档文件全路径
	TCHAR *pBackFileName,*pCurrentFileName;
	CreateWallpaperDirectory();//如果没有wallpaper目录则创建
	ofstream fout(save, ios::binary);//输出流,找开文件save.lu,以二进制方式写入
	if (!fout.is_open())//打开失败
		return;
	fout.write((char*)&Wallpaper, sizeof(Time));//保存壁纸时间结构
	fout.write((char*)&Notify, sizeof(Time));//保存气泡时间结构
	fout.write((char*)&randomChange, sizeof(bool));//保存是否随机布尔值
	fout.write((char*)&startingUpBoot, sizeof(bool));//保存是否开机自启动布尔值
	fout.write((char*)&changeByOnlyDestop, sizeof(bool));//保存是否仅有桌面这个窗口才换壁纸布尔值
	fout.write((char*)&includeSubDirectory, sizeof(bool));//保存是否包含子目录布尔值
	fout.write((char*)&startChange, sizeof(bool));//保存是否开始换布尔值
	fout.write((char*)&startingUpBoot, sizeof(bool));//保存是否开机启动标记
	//保存气泡信息
	fout.write((char*)&szTip, sizeof(szTip));
	fout.write((char*)&szInfo, sizeof(szInfo));
	fout.write((char*)&szInfoTitle, sizeof(szInfoTitle));
	fout.write((char*)&windowRect, sizeof(RECT));//保存窗口大小位置
	fout.write((char*)&lf, sizeof(LOGFONT));//保存字体结构
	fout.write((char*)&textColor, sizeof(COLORREF));//保存颜色
	fout.write((char*)&textBkColor, sizeof(COLORREF));//保存背景颜色
	fout.write((char*)&nSelectWallpaper, sizeof(UINT));//保存当前壁纸索引
	fout.write((char*)&color, sizeof(color));//保存自定义的颜色数组
	SaveDiretoryPath();////保存在存档文件
	fout.close();
}

tstring calculateCapacity(long fileSizeHigh, long fileSizeLow)//计算容量大小
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
HMENU CreateMyPopMenu(HINSTANCE hInst)//创建弹出菜单
{
	HMENU hMenu = CreatePopupMenu();//创建弹出菜单
	HBITMAP hBm[8];
	//添加菜单
	AppendMenu(hMenu, MF_STRING, IDM_SHOWWALLPAPER, TEXT("查看更多壁纸\tCtrl+Alt+M"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(hMenu, MF_STRING, IDM_NEXTWALLPAPER, TEXT("下一张\tAlt+N"));
	AppendMenu(hMenu, MF_STRING, IDM_PREWALLPAPER, TEXT("上一张\tAlt+P"));
	AppendMenu(hMenu, MF_STRING, IDM_PAUSE, startChange ? TEXT("暂停\tCtrl+P") : TEXT("开始\tCtrl+P"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(hMenu, MF_STRING, IDM_RANDOM, randomChange ? TEXT("顺序\tAlt+R") :TEXT("随机\tAlt+R"));
	AppendMenu(hMenu, MF_STRING, IDM_MINI,TEXT("所有窗口最小化才更换壁纸"));
	AppendMenu(hMenu, MF_STRING, IDM_SET, TEXT("壁纸设置\tAlt+S"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("退出\tAlt+F4"));
	CheckMenuItem(hMenu, IDM_MINI, changeByOnlyDestop ? MF_CHECKED : MF_UNCHECKED);//设置选择状态
	hBm[0] = LoadBitmap(hInst, TEXT("IDB_BITMAP1"));//读取位图
	SetMenuItemBitmaps(hMenu, IDM_SHOWWALLPAPER, MF_BYCOMMAND, hBm[0], 0);//设置菜单位图
	hBm[1] = LoadBitmap(hInst, TEXT("IDB_BITMAP2"));//读取位图
	SetMenuItemBitmaps(hMenu, IDM_NEXTWALLPAPER, MF_BYCOMMAND, hBm[1], 0);//设置菜单位图
	hBm[2] = LoadBitmap(hInst, TEXT("IDB_BITMAP3"));//读取位图
	SetMenuItemBitmaps(hMenu, IDM_PREWALLPAPER, MF_BYCOMMAND, hBm[2], 0);//设置菜单位图
	hBm[3] = LoadBitmap(hInst, TEXT("IDB_BITMAP4"));//读取位图
	SetMenuItemBitmaps(hMenu, IDM_PAUSE, MF_BYCOMMAND, hBm[3], 0);//设置菜单位图
	hBm[4] = LoadBitmap(hInst, TEXT("IDB_BITMAP5"));//读取位图
	SetMenuItemBitmaps(hMenu, IDM_EXIT, MF_BYCOMMAND, hBm[4], 0);//设置菜单位图
	hBm[5] = LoadBitmap(hInst, TEXT("IDB_BITMAP6"));//读取位图
	SetMenuItemBitmaps(hMenu,IDM_RANDOM, MF_BYCOMMAND, hBm[5], 0);//设置菜单位图
	hBm[6] = LoadBitmap(hInst, TEXT("IDB_BITMAP7"));//读取位图
	SetMenuItemBitmaps(hMenu,IDM_MINI, MF_BYCOMMAND, hBm[6], 0);//设置菜单位图
	hBm[7] = LoadBitmap(hInst, TEXT("IDB_BITMAP8"));//读取位图
	SetMenuItemBitmaps(hMenu,IDM_SET, MF_BYCOMMAND, hBm[7], 0);//设置菜单位图
	return hMenu;
}
tstring FormatFileTime(FILETIME &ft)//格式化文件时间
{
	FILETIME temp;//文件时间结构
	FileTimeToLocalFileTime(&ft, &temp);//将国际文件时间转换成本地文件时间
	SYSTEMTIME st;//时间结构
	FileTimeToSystemTime(&temp, &st);//将本地文件时间转换成系统时间
	tstringstream out;//字符串流
	out << st.wYear << TEXT("年") << st.wMonth <<TEXT( "月") << st.wDay << TEXT("日,") << st.wHour <<TEXT( ":") << st.wMinute << TEXT(":") << st.wSecond;//格式化字符串
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
void FindFile(tstring CurrentPath,tstring type)//查找文件
{
	WIN32_FIND_DATA fd;
	string find;//指定文件夹要查找的类型，如d:\\wallpaper\\*.jpg
	find=CurrentPath +type;//当前要查找文件格式的路径
	HANDLE handle = FindFirstFile(find.c_str(), &fd);//查找第1个文件
	if (handle != INVALID_HANDLE_VALUE)//找到了
	{
		do
		{
			if (fd.cFileName[0] == '.')//win32下一般会有三种文件夹，一种是“.”指的是当前文件夹，一种是“..”指的是上层文件夹，最后一种就是普通的下层文件夹用文件夹名显示，cd .是本目录，cd ..是上级目录
				break;//所以为了不回到上级目录和在原目录打转，过滤.和..这二个目录
			tempDirectory[fd.cFileName] = CurrentPath;//将当前图片名字与路径插入map中保存
		} while (FindNextFile(handle, &fd));//查找下一个文件
		FindClose(handle);//关闭文件句柄
	}
}
void FindAllFile(tstring CurrentPath, bool includeSubDirectory)//查找所有壁纸
{//CurrentPath当前路径,includeSubDirectory为true则显示当前目录、子目录文壁纸，为false则只显示当前目录壁纸
	WIN32_FIND_DATA fd;//fd文件信息结构
	HANDLE handle;//handle文件名柄
	string find;//指定文件夹要查找的类型，如d:\\wallpaper\\*.jpg
	for (int i = 0; i < format.size(); i++)//查找所有格式(jpg,bmp,png)的图片
		FindFile(CurrentPath,format[i]);//查找当前文件格式的路径
	if (includeSubDirectory)//如果要查找子目录的图片
	{
		find = CurrentPath +TEXT("\\*.*");//查找所有文件
		handle = FindFirstFile(find.c_str(), &fd);//查找第1个文件
		if (handle!= INVALID_HANDLE_VALUE)//找到了
		{
			do
			{
				if (fd.cFileName[0] == '.')//win32下一般会有三种文件夹，一种是“.”指的是当前文件夹，一种是“..”指的是上层文件夹，最后一种就是普通的下层文件夹用文件夹名显示，cd .是本目录，cd ..是上级目录
					continue;//所以为了不回到上级目录和在原目录打转，过滤.和..这二个目录
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//只要FILE_ATTRIBUTE_DIRECTORY是0x00000010，即只要从右数，第5位为1，即是文件目录，其它位是标记只读、只写、隐藏等属性
					FindAllFile(CurrentPath+"\\"+tstring(fd.cFileName),includeSubDirectory);
			} while (FindNextFile(handle, &fd));//FindFirstFile函数时指定的一个文件名查找下一个文件
			FindClose(handle);//关闭文件句柄
		}
	}
}
int _stdcall EnumWindowProc(HWND hwnd, long lParam)//枚举函数的窗口过程
{
	TCHAR title[256];//接收窗口标题
	if (IsWindow(hwnd) && IsWindowVisible(hwnd) && !IsIconic(hwnd))
	{//如果存在这个窗口，并且窗口可见，不是最小化
		GetWindowText(hwnd, title, 256);//得到窗口标题
		if (title[0] != '\0'&&lstrcmp(title,TEXT("开始"))!=0)//如果标题不为空,或者不是开始标题过滤掉WIN7开始按钮
			allTitle.push_back(title);//加入标题数组
		temp += tstring(title)+TEXT("   ");
	}
	return true;//为继续枚举下一个窗口，回调函数必须返回TRUE；若停止列表，它必须返回FALSE
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

void InitNotifyIconData(HWND hwnd, HINSTANCE hInst, NOTIFYICONDATA &nid)//初始化托盘结构
{
	nid.cbSize = sizeof(nid);//结构体的大小，以字节为单位
	nid.dwInfoFlags = NIIF_USER;//设置此成员用来给气球提示框增加一个图标，NIIF_USER是用户的图标，即成员nid.hIcon
	nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));//读取托盘用的图标，增加、修改或删除的图标的句柄
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
void PopupMyMenu(HWND hwnd, HMENU hMenu)//弹出菜单
{
	POINT mousePos;//鼠标位置
	GetCursorPos(&mousePos);//得到鼠标位置
	SetForegroundWindow(hwnd);//将窗口置顶，才能捕捉到在菜单外鼠标单击的操作，解决在菜单外单击左键菜单不消失的问题
	if (recordIndex == 0 || wallPaperCount==0)//如果当前壁纸是换过的壁纸记录中第一张或者无壁纸
		EnableMenuItem(hMenu,IDM_PREWALLPAPER, MF_GRAYED);//上一张图片菜单置为禁用
	else
		EnableMenuItem(hMenu,IDM_PREWALLPAPER,MF_ENABLED);//上一张图片菜单置为可用
	EnableMenuItem(hMenu, IDM_NEXTWALLPAPER,wallPaperCount? MF_ENABLED:MF_GRAYED);//下一张图片菜单置为禁用还是启用
	EnableMenuItem(hMenu,IDM_PAUSE, wallPaperCount ?  MF_ENABLED:MF_GRAYED );//开始菜单置为禁用还是启用
	int id = TrackPopupMenu(hMenu, TPM_RETURNCMD, mousePos.x, mousePos.y, 0, hwnd, 0);//弹出菜单,TPM_RETURNCMD返回菜单ID
	switch (id)
	{
	case IDM_EXIT:
		SendMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	case IDM_SHOWWALLPAPER://查看更多壁纸
		SendMessage(hwnd, WM_COMMAND, IDM_SHOWWALLPAPER, 0);//发送消息，显示窗口
		break;
	case IDM_PREWALLPAPER://上一张
		SendMessage(hwnd, WM_COMMAND, IDM_PREWALLPAPER, 0);
		break;
	case IDM_NEXTWALLPAPER://下一张
		SendMessage(hwnd, WM_COMMAND,IDM_NEXTWALLPAPER, 0);
		break;
	case IDM_PAUSE://暂停
		SendMessage(hwnd, WM_COMMAND, IDM_PAUSE, 0);
		break;
	case IDM_RANDOM://换壁纸模式，随机换还是顺序换
		SendMessage(hwnd, WM_COMMAND,IDM_RANDOM, 0);
		break;
	case IDM_MINI://所有窗口最小化才更换壁纸菜单
		SendMessage(hwnd, WM_COMMAND,IDM_MINI, 0);
		break;
	case IDM_SET://设置壁纸
		DialogBox((HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE), TEXT("IDD_DIALOGSET"), hwnd, DlgWallpaperSet);
		break;
	}
}
void HighLineWallPaper(HWND hEdit, int  nSelectWallpaper)//对当前的壁纸进行高亮显示
{
	if (nSelectWallpaper > 0)//如果不为0
	{
		lineIndex = SendMessage(hEdit, EM_LINEINDEX,nSelectWallpaper, 0);//得到指定行首字符在文本中的位置（以字节数表示）
		nextLineIndex = SendMessage(hEdit, EM_LINEINDEX, nSelectWallpaper +1, 0);//得到指定行下一行首字符在文本中的位置（以字节数表示）
		SendMessage(hEdit, EM_SETSEL, lineIndex, nextLineIndex);//选中指定行
		SendMessage(hEdit, EM_SCROLLCARET, 0, 0);// 把可见范围移至光标处,因定设置光标后，光标可能不在范围内，所以要滚动到光标处
		if (!buttonDown)//如果鼠标未单击，说明是自动换
		SendMessage(hEdit, WM_VSCROLL, SB_LINEUP, 0);//因为当选中客户区最后一行时，光标会在客户区最后一行的下一行，EM_SCROLLCARET会将滚动到选中行的下一页，看不到高亮显示，要向上滚一行才能看到，所以发送WM_VSCROLL向上滚一行，显示高亮
	}
}
void DrawBorder(HWND hwnd, HBRUSH hBlueBrush)//画条蓝色边框
{
	RECT rect;
	HDC hdc;
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);//得到客户区矩形
	rect.bottom -= 26;//边框不包围状态栏，减去状态栏高度
	FrameRect(hdc, &rect, hBlueBrush);//画淡蓝色画刷边框
	ReleaseDC(hwnd, hdc);
}
void FormatTime()//对系统时间进行格式化
{
	SYSTEMTIME st;//时间结构
	GetLocalTime(&st);//得到本地时间
	tstringstream out;//用这个字符串流格式化字符串
	out << TEXT("现在时间:") << st.wYear << TEXT("年") << st.wMonth << TEXT("月") << st.wDay << TEXT("  ") << st.wHour <<
		TEXT("小时") << st.wMinute << TEXT("分") << st.wSecond << TEXT("秒");
	statusInfo[1] = out.str();//将流中的字符串保存在字符串中
}
void InitColor(HWND hwnd, CHOOSECOLOR &cc, COLORREF color[])//初始化颜色结构
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
void InitFont(HWND hwnd, CHOOSEFONT &cf, LOGFONT &lf)//初始化字体结构
{
	cf.Flags = CF_EFFECTS | CF_NOVERTFONTS;//使对话框显示控件允许用户指定下划线,和文本颜色选项。 如果设置此标志,您可以使用rgbColors成员指定初始文本颜色
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
	cf.rgbColors = RGB(255, 0, 0);//初始字体对话框文本颜色
}
void DisplayDistribitue()//显示文件属性
{
	TCHAR szBuffer[1024];//字符缓冲
	WIN32_FILE_ATTRIBUTE_DATA attri;//文件属性结构
	tstring szPath;//文件路径
	TCHAR szName[31];//文件名
	static TCHAR *szColumn[8] = {TEXT("索引"),TEXT("文件名"),TEXT("大小"),TEXT("属性"),TEXT("创建时间"),TEXT("最后一次访问时间"),TEXT("最后一次修改时间"),TEXT("路径")};
	StringCchPrintf(szBuffer, 1024, TEXT("%-9s%-30s%-20s%-15s%-30s%-30s%-30s%-30s\r\n"),szColumn[0],szColumn[1],
		szColumn[2], szColumn[3], szColumn[4], szColumn[5], szColumn[6], szColumn[7]);//格式化列名
	tstring temp = szBuffer;//在编辑上显示的字符串
	tstring index;//索引
	wallPaperCount = 0;//文件个数，初始化为0
	directoryIndex.clear();//将上一次索引清除
	for (int i=0; i<AllDirectorys.size();i++)//显示所有目录的信息
	{
		directoryIndex.push_back(wallPaperCount);//将当前目录第一张图片索引保存
		for (auto iter=AllDirectorys[i].begin(); iter!=AllDirectorys[i].end();iter++)//显示当前目录信息
		{
			wallPaperCount++;//文件个数加1
			GetFileAttributesEx((iter->second+TEXT("\\")+iter->first).c_str(), GetFileExInfoStandard, &attri);//得到当前文件的属性,GetFileExInfoStandard表示表准文件属性，最后一个参数是 WIN32_FILE_ATTRIBUTE_DATA结构的指针
			fileCreateTime = FormatFileTime(attri.ftCreationTime);//文件创建时间
			fileLastAccessTime = FormatFileTime(attri.ftLastAccessTime); // 文件最后一次访问时间
			fileLastWriteTime = FormatFileTime(attri.ftLastWriteTime);//文件最后一次修改时间
			fileAttributes = FileAttribute(attri.dwFileAttributes);//文件属性
			szPath = (iter->second).c_str();//文件路径
			lstrcpyn(szName,(iter->first).c_str(),30);//拷贝文件名名的前30个字符
			if ((iter->first).size()>29)//如果文件名大于29，文件名太长
				lstrcpyn(szName+24, TEXT("*.jpg"),6);//将最后5个字符改为*.jpg以表示文件名省略
			fileSize = calculateCapacity(attri.nFileSizeHigh, attri.nFileSizeLow);//计算文件大小
			index = ToString(wallPaperCount) + TEXT(":");//将文件索引整数转换成字符串
			StringCchPrintf(szBuffer, 1024, TEXT("%-9s%-30s%-20s%-15s%-30s%-30s%-30s%-30s\r\n"), index.c_str(), szName, fileSize.c_str(),
			fileAttributes.c_str(), fileCreateTime.c_str(), fileLastAccessTime.c_str(), fileLastWriteTime.c_str(),
			szPath.c_str());//格式化字符串
			temp += szBuffer;
		}
	}
	SetWindowText(hEdit, temp.c_str());//在编辑框显示
}
void ChangeWallPaper()//改变壁纸
{
	int nDirectory = AllDirectorys.size() - 1;//哪个目录,初始化为最后一个目录的迭代器
	int nIndex;//该目录的哪张图片(索引)			
	for (int i = 0; i <= nDirectory; i++)
	{
		if (nSelectWallpaper - 1 < directoryIndex[i])//因为索引数组从小到大，如果当前选中的图片索引(索引从0开始，要减1)比当前目录的最后一张图片小
		{//则i为当前目录索引
			nDirectory = --i;//保存当前图片的上一个目录的索引
			break;//找到目录了，退出循环
		}
	}
	nIndex = (nSelectWallpaper - 1) - directoryIndex[nDirectory];//减去当前目录上一个目录首张图片的索引，获得该目录的哪张图片(索引)，因为(索引从0开始，所以要减1)
	auto tempMap = AllDirectorys[nDirectory];//当前目录的map结构
	auto begin = tempMap.begin();//当然图片目录的第1个迭代器
	for (int i = 0; i <nIndex; i++)
		begin++;//将迭代器移到当前图片的索引处
	tstring szPath = begin->second;//文件路径
	tstring szName = begin->first;//文件名
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)(szPath + TEXT("\\") + szName).c_str(), 0);//将双击的图片，作为壁纸
	statusInfo[0] = tstring("当前壁纸:") + szPath + TEXT("\\") + szName;
	SendMessage(hStatus, SB_SETTEXT, 0, (long)statusInfo[0].c_str());//第一栏显示路径	
}
HWND FindDialog(HWND hwnd,FINDREPLACE &fr ,TCHAR szFindText[])//查找结构
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
int _stdcall EditTextProc(HWND hEdit, unsigned message, unsigned wParam, long lPram)//重写编辑控件窗口过程
{
	int first;//当前光标所在行，首字符在编辑中的位置
	int startPos;//当前选中文字的开始位置，若未选中则是当前光标相对于开头位置的偏移（字符个数）
	int endPos;//当前选中文字的结束位置+1，若未选中则是当前光标相对于开头位置的偏移（字符个数）
	switch (message)
	{
	case WM_KEYUP://当键盘松开和
	case WM_LBUTTONUP://当鼠标左键松开的时候，更新行列信息
		if (wParam == VK_F3)//如果是查找一一行快捷键
			break;//退出
		row = SendMessage(hEdit, EM_LINEFROMCHAR, -1, 0) + 1;//获得指定位置所在的行号，-1表示当前光标所在位置，因为位置的索引从0开始，所以要加1
		statusInfo[2] = TEXT("行号:") + ToString(row);//将行号转换成字符串
		SendMessage(hStatus, SB_SETTEXT, 2, (long)statusInfo[2].c_str());//对第二栏更新行号 
		first = SendMessage(hEdit, EM_LINEINDEX, -1, 0);//当前光标所在行，首字符在编辑中的位置
		SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//当前选中文字的开始、结束位置，若未选中则是当前光标相对于开头位置的偏移（字符个数）			
		col = startPos - first + 1;//当前光标的位置-当前光标所在行第1个字符的位置为列数，，因为位置的索引从0开始，所以要加1
		statusInfo[3] = TEXT("列号:") + ToString(col);//将列号转换成字符串
		SendMessage(hStatus, SB_SETTEXT, 3, (long)statusInfo[3].c_str());//对第三栏更新列号 
		if (row >= 2)//如果不是选择第1行
		{
			nSelectWallpaper = row -1;//因为第1行是列名称，第2行是第1张，第3行是第2张，所以当前选中的索引是row-1张
			HighLineWallPaper(hEdit, nSelectWallpaper);//对当前行高亮显示，但还未设置为当前行为壁纸，只有鼠标双击才设置
		}
		buttonDown =false;//鼠标左键松开
		break;
	case WM_LBUTTONDBLCLK:
		SendMessage(hEdit, EM_SETSEL, lineIndex,lineIndex);//因为单击后光标指向了下一行，因为WM_LBUTTONDBLCLK后是WM_LBUTTONUP，为了继续选中当前行，要将光标设回去
		if (row >= 2 && row <= wallPaperCount + 1)//如果在第2行到最后一行,+1是因为总行数要包括第1行
		{
			ChangeWallPaper();//改变壁纸
			recordChange.push_back(nSelectWallpaper);//保存换过的壁纸中的记录中的索引
			recordIndex = recordChange.size() - 1;//当前壁纸在换过的壁纸中的记录中的索引是最后一个索引
		}
		buttonDown = true;//鼠标左键按下
		break;
	case WM_RBUTTONDOWN://右键单击
		PopupMyMenu(GetParent(hEdit), hMenu);//弹出菜单
		break;
	case WM_LBUTTONDOWN:
		buttonDown = true;//鼠标左键按下
		break;
	}
	return CallWindowProc(oldEditTextProc, hEdit, message, wParam, lPram);//调用默认的编辑控件窗口处理过程处理其它消息和未处理完的操作
}
void OKMessage(HWND hwnd, TCHAR szFormat[], TCHAR szFindText[])//弹出一个OK提示框
{
	TCHAR szBuffer[MAX_PATH];
	StringCchPrintf(szBuffer, MAX_PATH, szFormat, szFindText);//进行格式化字符串
	MessageBox(hwnd, szBuffer,TEXT("找不到"), MB_OK | MB_ICONEXCLAMATION);//弹出消息框
}
bool MyFindText(FINDREPLACE &fr, bool bDown)//查找文本
{
	int startPos;//当前选中文字的开始位置，若未选中则是当前光标相对于开头位置的偏移（字符个数）
	int endPos;//当前选中文字的结束位置+1，若未选中则是当前光标相对于开头位置的偏移（字符个数）
	int len = GetWindowTextLength(hEdit);//获得编辑框字符个数
	TCHAR *pText = new TCHAR[len + 1];//分配空间
	shared_ptr<TCHAR>sp1(pText, [](TCHAR *p){delete[]p; });//将分配的空间交智能指针管理	
	GetWindowText(hEdit, pText, len + 1);//获得编辑框字符
	int findLen = lstrlen(fr.lpstrFindWhat);//要查找的字符串长度
	int pos;//查找到的位置
	SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//当前选中文字的开始位置、结束，若未选中则是当前光标相对于开头位置的偏移（字符个数）
	if (startPos != endPos&&bDown)//开始结束位置不同，说明选中文字并且是向下查找
		startPos++;//则从当前选中位置的下一个位置继续查找,否则没选中则从当前位置开始查找
	if (!(fr.Flags&FR_MATCHCASE))//如果区分大小写按钮没被按下
	{
		for (int i = 0; i < findLen; i++)//将要查找的字符串转成小写
			fr.lpstrFindWhat[i] = tolower(fr.lpstrFindWhat[i]);
		for (int i = 0; i < len; i++)//将编辑框中的字母也转成小写，这样就大小写一致
			pText[i] = tolower(pText[i]);
	}
	if (bDown)//向下查找
	{
		auto p = search(pText + startPos, pText + len, fr.lpstrFindWhat, fr.lpstrFindWhat + findLen);//查找第一个匹配的字符串位置
		if (p == pText + len)//向下查找不到
			return false;
		pos = p - pText;
	}
	else
	{
		auto p = find_end(pText, pText + startPos, fr.lpstrFindWhat, fr.lpstrFindWhat + findLen);//查找从最后一个匹配的字符串位置
		if (p == pText + startPos)//向上查找不到
			return false;
		pos = p - pText;
	}
	SendMessage(hEdit, EM_SETSEL, pos, pos + findLen);//选中查找到的字符串
	SendMessage(hEdit, EM_SCROLLCARET, 0, 0);//把可见范围移置光标处，即滚动到选中的位置
	return true;
}
int _stdcall DlgGotoProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)//转到对话框窗口过程
{
	static HWND hEditGoto;//编辑框
	TCHAR strNum[24];//编辑框上的数字
	int lineIndex;//指定行首字符在文本中的位置（以字节数表示）
	static HWND hwnd;
	static HWND hEdit;//编辑框句柄
	static int lineCount;//编辑框总行数
	switch (message)
	{
	case WM_INITDIALOG://初始对话框
		hwnd = GetParent(hDlg);//得到主窗口句柄
		hEdit = GetDlgItem(hwnd, ID_EDIT);
		lineCount = SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);
		hEditGoto = GetDlgItem(hDlg, IDC_EDITGOTO);//得到转到编辑框句柄
		SetWindowText(hEditGoto, ToString(row).c_str());
		return true;//因为在对话框资源只对编辑框设置WS_TABSTOP，这里返回true，就会使编辑框获得输入焦点，文字会全选
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_GOTO:
			GetWindowText(hEditGoto, strNum, sizeof(strNum));
			row = strToInt(strNum);//将字符串转换成整数
			if (row > lineCount)
			{
				SetWindowText(hEditGoto, ToString(lineCount).c_str());//在转到编辑框显示编辑的总行数
				SetFocus(hEditGoto);//让转到编辑框获得焦点，是为了让文全选，显眼提示总行数
				SendMessage(hEditGoto, EM_SETSEL, 0, -1);//转到编辑框文字全选
				MessageBox(hEditGoto, TEXT("查找超过了壁纸总数"), TEXT("壁纸 -跳行"), 0);
				break;
			}
			lineIndex = SendMessage(hEdit, EM_LINEINDEX, row - 1, 0);//得到指定行首字符在文本中的位置（以字节数表示），-1是因为索引从0开始
			SendMessage(hEdit, EM_SETSEL, lineIndex, lineIndex);//将光标移到指定行
			SendMessage(hEdit, EM_SCROLLCARET, 0, 0);// 把可见范围移至光标处,因定设置光标后，光标可能不在范围内，所以要滚动到光标处
			SendMessage(hEdit, WM_LBUTTONUP, 0, 0);//发送一条鼠标松开消息以更新行号
		case IDC_CANCELGOTO:
			EndDialog(hDlg, 0);
			break;
		}
		return true;
	case WM_CLOSE:
		EndDialog(hDlg, 0);//关闭时销毁窗口,不能用DestroyWindow(hDlg)因为它只会销毁窗口不会退出对话框的消息循环，并没有允许对话框应用程序把控制权返回系统，主窗口还是不能动，
		//而EndDialog则会则在窗口销毁后结束对话框消息队列，允许返回控件权给主窗口
		return true;
	}
	return false;//如果没有处理，返回0,会调用默认对话框过程，非0则处理了，不调用默认的
}
void AddWapperDirectory(HWND hwnd)//添加壁纸目录
{
	recordChange.clear();//添加目录会使先前一部分索引失效，所以将记录清除
	recordIndex = 0;//将索引记录初始化为0
	plDlist = SHBrowseForFolder(&bi);//弹出选择目录对话框,返回用户选择的目录信息结构的指针plDlist
	if (plDlist)//如果选择了一个目录
	{
		SHGetPathFromIDList(plDlist, szDirectoryPath);//将plDlist路径（结构体）转换成字符串路径				
		auto iter =find(currentDirectorys.begin(),currentDirectorys.end(), szDirectoryPath);//从现有壁纸目录中查找是否已存在该目录
		if (iter ==currentDirectorys.end())//旧目录找不到新添加的目录
		{
			tempDirectory.clear();//将上一次添加的目录，所有临时文件名、文件路径清除
			FindAllFile(szDirectoryPath, includeSubDirectory);//查找新目录的所有图片
			AllDirectorys.push_back( tempDirectory);//将当前目录的所有信息添加
			currentDirectorys.push_back(szDirectoryPath);//将新目录添加到当前目录
			DisplayDistribitue();//更新文件属性显示
		}
		else
			MessageBox(hwnd, TEXT("请添加新目录,不要重复添加已存在目录"),TEXT("该目录已存在:"), MB_ICONWARNING | MB_OK);
		if (SHGetMalloc(&pMalloc) == S_OK);//创建IMALLOC对象
		{
			pMalloc->Free(plDlist);//释放plDlist指向的空间
			pMalloc->Release();//释放pMalloc的空间
		}
	}
}
void GetPictureAndDocumentPath(HWND hwnd)//得到我的文档和我的图片路径
{
	SHGetSpecialFolderLocation(hwnd, CSIDL_MYPICTURES, &plDlist);//获得我的图片文件夹路径
	SHGetPathFromIDList(plDlist,szMyPictures);//将plDlist路径（结构体）转换成字符串路径
	SHGetSpecialFolderLocation(hwnd, CSIDL_PERSONAL, &plDlist);//获得我的文档文件夹路径
	SHGetPathFromIDList(plDlist, szMyDocument);//将plDlist路径（结构体）转换成字符串路径
	if (SHGetMalloc(&pMalloc) == S_OK);//创建IMALLOC对象
	{
		pMalloc->Free(plDlist);//释放plDlist指向的空间
		pMalloc->Release();//释放pMalloc的空间
	}
}
UINT calculateTime(const Time &time)//计算时间，以毫秒计
{
	unsigned millisecond;//毫秒
	millisecond = time.second * 1000 + time.minute * 60 * 1000 + time.hour * 60 * 60 * 1000;//计算时间，以毫秒计
	return millisecond;
}
int _stdcall DlgWallpaperSet(HWND hDlg, unsigned int message, unsigned wParam, long lParam)//设置壁纸对话框窗口过程
{
	int wmID = LOWORD(wParam);//控件ID
	static HWND hWallPaperHour, hWallPaperMinute, hWallPaperSecond;//壁纸更换时、分、秒编辑框句柄
	static HWND hNotifyHour, hNotifyMinute, hNotifySecond;//气泡提示时、分、秒编辑框句柄
	static HWND hNotifyTip, hNotifyTitle, hNotifyContent;//气泡提示、标题、内容编辑框
	static HWND hRadioRandom, hRadioSequence;//随机和和顺序单选按钮句柄
	static HWND hCheckBoxMini, hCheckBoxSub,hCheckBoxAutoBoot;//所有窗口最小化、子目录和开机自动启动check box按钮句柄
	static HWND hwnd;//父窗口
	static HINSTANCE hInst;//实例句柄
	TCHAR szBuffer[MAX_PATH];//缓冲区
	switch (message)
	{
	case WM_INITDIALOG://对话框初始化
		hWallPaperHour = GetDlgItem(hDlg, IDC_EDITHOUR);//获得对话框各个控件的句柄
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
		//在各个控件上显示当前属性
		SetWindowText(hWallPaperHour, ToString(Wallpaper.hour).c_str());
		SetWindowText(hWallPaperMinute, ToString(Wallpaper.minute).c_str());
		SetWindowText(hWallPaperSecond, ToString(Wallpaper.second).c_str());
		SetWindowText(hNotifyHour, ToString(Notify.hour).c_str());
		SetWindowText(hNotifyMinute, ToString(Notify.minute).c_str());
		SetWindowText(hNotifySecond, ToString(Notify.second).c_str());
		SetWindowText(hNotifyTip, szTip);
		SetWindowText(hNotifyTitle, szInfoTitle);
		SetWindowText(hNotifyContent, szInfo);
		if (changeByOnlyDestop)//仅有桌面一个窗口才换
			SendMessage(hCheckBoxMini, BM_SETCHECK, 1, 0);
		else
			SendMessage(hCheckBoxMini, BM_SETCHECK, 0, 0);
		if (includeSubDirectory)//查找文件包含子目录
			SendMessage(hCheckBoxSub, BM_SETCHECK, 1, 0);
		else
			SendMessage(hCheckBoxSub, BM_SETCHECK, 0, 0);
		if (randomChange)//如果是随机换
			SendMessage(hRadioRandom, BM_SETCHECK, 1, 0);
		else
			SendMessage(hRadioSequence, BM_SETCHECK, 1, 0);
		SendMessage(hCheckBoxAutoBoot, BM_SETCHECK,startingUpBoot ? 1 : 0, 0);
		break;
	case WM_COMMAND:
		switch (wmID)
		{
		case IDM_SETFINISH://设置完成
			GetWindowText(hWallPaperHour,szBuffer,MAX_PATH);//得到小时字符串
			Wallpaper.hour = strToInt(szBuffer);//转换成整数
			GetWindowText(hWallPaperMinute, szBuffer, MAX_PATH);//得到分钟字符串
			Wallpaper.minute = strToInt(szBuffer);//转换成整数
			GetWindowText(hWallPaperSecond, szBuffer, MAX_PATH);//得到秒字符串
			Wallpaper.second = strToInt(szBuffer);//转换成整数
			wallPaperTime = calculateTime(Wallpaper);//计算新壁纸更换时间，以毫秒计
			if (wallPaperTime>0)//如果时间>0
			KillTimer(hwnd, wallpaperID);//杀死换壁纸定时器
			if (startChange)//如果不是暂停，则
			SetTimer(hwnd, wallpaperID, wallPaperTime, 0);//开启换壁纸定时器
			GetWindowText(hNotifyHour, szBuffer, MAX_PATH);//得到小时字符串
			Notify.hour = strToInt(szBuffer);//转换成整数
			GetWindowText(hNotifyMinute, szBuffer, MAX_PATH);//得到分钟字符串
			Notify.minute = strToInt(szBuffer);//转换成整数
			GetWindowText(hNotifySecond, szBuffer, MAX_PATH);//得到秒字符串
			Notify.second = strToInt(szBuffer);//转换成整数
			tipTime = calculateTime(Notify);//计算新壁纸更换时间，以毫秒计
			if (tipTime>0)//如果时间>0
			KillTimer(hwnd,tipID);//杀死换壁纸定时器
			SetTimer(hwnd,tipID,tipTime, 0);//开启换壁纸定时器
			randomChange = SendMessage(hRadioRandom, BM_GETCHECK, 0, 0);//获得换壁纸模式，是否随机
			changeByOnlyDestop = SendMessage(hCheckBoxMini, BM_GETCHECK, 0, 0);//所有窗口最小化才更换壁纸选择状态
			includeSubDirectory = SendMessage(hCheckBoxSub, BM_GETCHECK, 0, 0);//查找包括子目录选择状态
		    startingUpBoot= SendMessage(hCheckBoxAutoBoot, BM_GETCHECK, 0, 0);//查找开机自启动选择状态
		    SetStartingUpBoot();//设置开机启动与否
			GetWindowText(hNotifyTip, szBuffer, MAX_PATH);//获得气泡提示字符串
			if (szBuffer[0] != '\0')//不空
				lstrcpyn(szTip, szBuffer, 128);
			GetWindowText(hNotifyTitle, szBuffer, MAX_PATH);//获得气泡标题字符串
			if (szBuffer[0] != '\0')//不空
				lstrcpyn(szInfoTitle, szBuffer,64);
			GetWindowText(hNotifyContent, szBuffer, MAX_PATH);//获得气泡内容字符串
			if (szBuffer[0] != '\0')//不空
				lstrcpyn(szInfo, szBuffer,256);
			InitNotifyIconData(hwnd, hInst,nid);//初始化托盘图标结构
			Shell_NotifyIcon(NIM_MODIFY, &nid);//修改托盘区域的一个图标，再次显示气泡
			EndDialog(hDlg, 0);//销毁对话框
			return false;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);//销毁对话框
		return false;
	}
	return false;
}
int _stdcall DlgModifyDirectoryProc(HWND hDlg, unsigned int message, unsigned wParam, long lParam)//修改壁纸目录对话框窗口过程
{
	int wmID = LOWORD(wParam);//控件ID
	static HWND hListLeft, hListRight;//左右列表框句柄
	static HWND hEditPath;//路径编辑框
	static HWND hBTNOK;//按钮OK
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	int index;//列表框上索引
	int count;//列表框上的目录个数
	vector<tstring>temp;//保存列表框上字符串
	switch (message)
	{
	case WM_INITDIALOG:
		hListLeft = GetDlgItem(hDlg, IDC_LISTLEFT);//得到2个列表框句柄
		hListRight = GetDlgItem(hDlg, IDC_LISTRIGHT);
		hEditPath = GetDlgItem(hDlg, IDC_EDITDIRECTORY);//得到路径编辑框句柄
		hBTNOK = GetDlgItem(hDlg, IDC_BTNOK);//得到确定按钮名柄
		for (int i = 0; i < backupDirectorys.size(); i++)//在左边列表框显示备份壁纸目录
			SendMessage(hListLeft, LB_INSERTSTRING, -1, (long)backupDirectorys[i].c_str());//将当前目录插入到列表框尾部
		for (int i = 0; i <currentDirectorys.size(); i++)//在右边列表框显示当前壁纸目录
			SendMessage(hListRight, LB_INSERTSTRING, -1, (long)currentDirectorys[i].c_str());//将当前目录插入到列表框尾部
		return true;
	case WM_COMMAND:
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		switch (wmId)
		{
		case IDC_EDITDIRECTORY://目录编辑框
			switch (wmEvent)//在编辑框上的操作
			{
			case EN_UPDATE://编辑框上的内容更新了
				GetWindowText(hEditPath, szDirectoryPath, MAX_PATH);//得到编辑框上字符串
				if (szDirectoryPath[0] != '\0')//如果字符串不为空
					EnableWindow(hBTNOK, true);//让按钮可用
				else
					EnableWindow(hBTNOK, false);//让按钮不可用
				break;
			}
			break;
		case IDC_BTNADDDIRECTORY://添加壁纸按钮
			plDlist = SHBrowseForFolder(&bi);//弹出选择目录对话框,路径保存的指针plDlist
			if (plDlist)//如果选择了一个目录
			{
				SHGetPathFromIDList(plDlist, szDirectoryPath);//将plDlist路径（结构体）转换成字符串路径	
				SetWindowText(hEditPath, szDirectoryPath);//在编辑框上显示路径
				if (SHGetMalloc(&pMalloc) == S_OK);//创建IMALLOC对象
				{
					pMalloc->Free(plDlist);//释放plDlist指向的空间
					pMalloc->Release();//释放pMalloc的空间
				}
			}
			break;
		case IDC_MOVERIGHT://将左列表框选中的路径移到右列表框
			index = SendMessage(hListLeft, LB_GETCURSEL, 0, 0);//获得当前选中目录的索引
			if (index != LB_ERR)
			{
				SendMessage(hListLeft, LB_GETTEXT, index, (long)szDirectoryPath);//获得当前选中的路径
				SendMessage(hListRight, LB_INSERTSTRING, -1, (long)szDirectoryPath);//将当前目录插入到右列表框尾部
				SendMessage(hListLeft, LB_DELETESTRING, index, 0);//从左列表框中删除
			}
			break;
		case IDC_MOVELEFT://将右列表框选中的路径移到左列表框
			index = SendMessage(hListRight, LB_GETCURSEL, 0, 0);//获得当前选中目录的索引
			if (index != LB_ERR)
			{
				SendMessage(hListRight, LB_GETTEXT, index, (long)szDirectoryPath);//获得当前选中的路径
				SendMessage(hListLeft, LB_INSERTSTRING, -1, (long)szDirectoryPath);//将当前目录插入到左列表框尾部
				SendMessage(hListRight, LB_DELETESTRING, index, 0);//从右列表框中删除
			}
			break;
		case IDC_BTNOK://按下确定按钮
			SendMessage(hListRight, LB_INSERTSTRING, -1, (long)szDirectoryPath);//将当前目录插入到右列表框尾部
			break;
		case IDC_BTNDELETELEFT://对左列表框选中路径进行删除
			index = SendMessage(hListLeft, LB_GETCURSEL, 0, 0);//获得当前选中目录的索引
			SendMessage(hListLeft, LB_DELETESTRING, index, 0);//删除
			break;
		case IDC_BTNDELETERIGHT://对右列表框选中路径进行删除
			index = SendMessage(hListRight, LB_GETCURSEL, 0, 0);//获得当前选中目录的索引
			SendMessage(hListRight, LB_DELETESTRING, index, 0);//删除
			break;
		case IDC_BTNSETFINISH://设置完成
			backupDirectorys.clear();//清除备份目录
			count = SendMessage(hListLeft, LB_GETCOUNT, 0, 0);//得到左列表框中目录的个数
			for (int i = 0; i < count; i++)
			{
				SendMessage(hListLeft, LB_GETTEXT, i, (long)szDirectoryPath);//获得当前索引的路径
				backupDirectorys.push_back(szDirectoryPath);//将路径插入备份目录
			}
			count = SendMessage(hListRight, LB_GETCOUNT, 0, 0);//得到右列表框中目录的个数
			for (int i = 0; i < count; i++)
			{
				SendMessage(hListRight, LB_GETTEXT, i, (long)szDirectoryPath);//获得当前索引的路径
				temp.push_back(szDirectoryPath);//将路径插入临时数组
			}
			for (auto iter = currentDirectorys.begin(); iter != currentDirectorys.end();)//将当前目录中在列表框中不存在的目录删除
			{
				auto result = find(temp.begin(), temp.end(), *iter);//在右列表框中查找当前目录
				if (result == temp.end())//找不到
				{
					iter = currentDirectorys.erase(iter);//删除这个目录，返回下一个迭代器
					int index = iter - currentDirectorys.begin();//获得当前迭代器的索引
					AllDirectorys.erase(AllDirectorys.begin() + index);//删除当前目录的所有信息
				}
				else
					iter++;//找到了不删除，测试下一个
			}
			for (auto iter = temp.begin(); iter != temp.end(); iter++)//将列表框中目录在当前目录中不存在的插入当前目录
			{
				auto result = find(currentDirectorys.begin(), currentDirectorys.end(), *iter);//将列表框中目录在当前目录中查找
				if (result ==currentDirectorys.end())//找不到，插入当前目录
				{
					tempDirectory.clear();//将上一次添加的目录，所有临时文件名、文件路径清除
					currentDirectorys.push_back(*iter);//将当前目录中不存在的目录插入
					FindAllFile(*iter, includeSubDirectory);//查找新目录的所有图片
					AllDirectorys.push_back(tempDirectory);//将当前目录的所有信息添加
					
				}
			}
			DisplayDistribitue();//更新文件属性显示
			EndDialog(hDlg, 0);//销毁对话框
			break;
		}
		return true;
	case WM_CLOSE:
		EndDialog(hDlg, 0);//销毁对话框
		return false;
	}
	return false;
}
void NextWallPaper()//下一张壁纸
{
	if (randomChange)//如果是随机换
		nSelectWallpaper = rand() % wallPaperCount + 1;//随机选一张图片
	else
	{
		nSelectWallpaper = nSelectWallpaper + 1;//下一张图片
		if (nSelectWallpaper >= wallPaperCount + 1||nSelectWallpaper<=1)//如果是最后一张过了,或者超出范围
			nSelectWallpaper = 1;//回到开头
	}
}
void SetStartingUpBoot()//设置开机启动
{
	HKEY m_regkey;//注册表句柄
	TCHAR filename[MAX_PATH];
	GetModuleFileName(NULL, filename, MAX_PATH);//返回程序的全路径
	TCHAR lpRun[] = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");//子键路径
	long iRet=RegOpenKeyEx(HKEY_LOCAL_MACHINE,lpRun,0,KEY_WRITE, &m_regkey);//打开自启动注册表键
	if (iRet == ERROR_SUCCESS)//打开成功
	{
		if (startingUpBoot)//开机自启
		    RegSetValueEx(m_regkey,TEXT("曾勇华壁纸"), 0, REG_SZ, (const unsigned char *)filename, MAX_PATH);//将当前程序路径添加到注册表自启动中
		else
			RegDeleteValue(m_regkey, TEXT("曾勇华壁纸"));//删除注册表
	}
	RegCloseKey(m_regkey);//关闭注册表
}
void InitWindow(HWND hwnd,HINSTANCE hInst)//窗口默认初始化
{
	int width, heigh;//窗口宽度、高度
	hBKBrush = CreateSolidBrush(textBkColor);//创建背景画刷轻灰色
	InitBrowseInfo(hwnd, bi);//初始化选择目录结构
	InitFont(hwnd, cf,lf);//初始化字体结构
	InitColor(hwnd, cc, color);//初始化颜色结构
	InitNotifyIconData(hwnd, hInst, nid);//初始化托盘图标结构
	WM_FIND = RegisterWindowMessage(FINDMSGSTRING);//查找和替换对换框会发送消息给用FINDMSGSTRING注册的消息		
	Shell_NotifyIcon(NIM_ADD, &nid);//向托盘区域添加一个图标
	hMenu = CreateMyPopMenu(hInst);//创建菜单
	blueBrush = CreateSolidBrush(RGB(102, 179, 255));//蓝色画刷
	hFont = CreateFontIndirect(&lf);//根据逻辑字体创建字体句柄
	//hFont= CreateFont(0, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);//默认字体
	hStatus = CreateWindow(STATUSCLASSNAME, 0, WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, hwnd, (HMENU)IDS_STATE, hInst, nullptr);//创建状态栏
	hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL | ES_READONLY | ES_MULTILINE | ES_NOHIDESEL, 0, 0, 0, 0,
		hwnd, (HMENU)ID_EDIT, hInst, nullptr);//创建有边界的多行只读编辑框，ES_NOHIDESEL在编辑框失去焦点后选中的文本依然保持高亮显示
	SetClassLong(hEdit, GCLP_HCURSOR, (long)LoadCursor(nullptr, IDC_ICON));//设置鼠标光标
	SendMessage(hEdit, EM_LIMITTEXT, 0, 0);//当wParam为0时，多行编辑控伯最多0x7FFFFFFE个字符，默认是30000个字符
	SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);//设置字体
	oldEditTextProc = (WNDPROC)SetWindowLong(hEdit, GWL_WNDPROC, (long)EditTextProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘、鼠标消息，因为
	//默认是捕获不到这些消息的，但要用的这些消息，比如当鼠标或按键按下时更新状态栏显示的行列信息
	//但是不能用SetClassLong(hEdit,GCL_WNDPROC),因为它是修改edit类的窗口过程，它只会对下一个创建的编辑框生效，对当前
	//编辑框不会改变，而且用SetClassLong,会影响以后的编辑框，而我只想改变当前编辑框，不想改变其它编辑框
	srand(time(0));//将当前时间作为随机函数种子
	wallPaperTime=calculateTime(Wallpaper);//计算时间以秒计
	tipTime = calculateTime(Notify);
	if (startChange)//如果开始换
	SetTimer(hwnd, wallpaperID, wallPaperTime, nullptr);//设置壁纸定时器
	SetTimer(hwnd, tipID, tipTime, nullptr);//设置气泡提示定时器
	SetTimer(hwnd, clockID, 1000, nullptr);//设置时钟定时器，每一秒更新时间一次
	width = windowRect.right - windowRect.left;//窗口宽度
	heigh = windowRect.bottom - windowRect.top;//窗口高度
	if (width>0&&heigh>0&&windowRect.left>=0&&windowRect.top>=0)//如果读取的宽度和高度不为0
	MoveWindow(hwnd, windowRect.left, windowRect.top,width,heigh ,true);//移动窗口，显示上一次窗口启动的位置大小
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//主窗口 窗口过程
{
	PAINTSTRUCT ps;//画笔结构
	HDC hdc;//画笔
	RECT rect;//矩形
	int size;//数组大小
	static POINT mousePos;//鼠标位置
	static HBRUSH oldBrush;//保存旧画刷
	static HINSTANCE hInst;
	static bool bDown;//向下按钮是否按下
	static TCHAR szFindText[MAX_PATH]=TEXT("");//要查找的字符串
	int xPos[4]{0, 0, 0, 0};//状态栏的四个右横坐标
	static FINDREPLACE fr;//查找和替换结构
	LPFINDREPLACE pfr = nullptr;//查找和替换结构的指针
	switch (UMsg)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;//程序实例句柄
		GetPictureAndDocumentPath(hwnd);//得到我的文档和我的图片路径
		ReadSaveFile();//读取用户设置,如果有存档，将默认设置更改为用户设置
		InitWindow(hwnd,hInst);//初始化窗口各种设置
		if (currentDirectorys.size()!= 0)//如果有壁纸路径
		{
			for (int i=0; i < currentDirectorys.size(); i++)//查找所以目录的图片
			{
				tempDirectory.clear();//将上一次添加的目录，所有临时文件名、文件路径清除
				FindAllFile(currentDirectorys[i], includeSubDirectory);//查找当前目录的所有图片
				AllDirectorys.push_back(tempDirectory);//将当前目录的所有信息添加
			}
			DisplayDistribitue();//显示文件属性
		}
		break;
	case WM_SETFOCUS:
		SetFocus(hEdit);//让编辑框获得焦点
		HideCaret(hEdit);//编辑框有光标，因为在资源中设置为只读，所以让光标隐藏才比较好，一定要有setfoucs后这语句才有效，因为初始化过程中编辑控还未获得焦点，所以设置
		//隐藏光标无效，setfocus让它获得焦点设置隐藏光标才有效
		break;
	case WM_SIZE://大小改变后
		cxClient = LOWORD(lParam);//客户区的宽度、高度
		cyClient = HIWORD(lParam);
		xPos[3] = -1;//最后一个窗格右边界-1，表球到客户区最右边
		xPos[2] = cxClient - 170;
		xPos[1] = cxClient - 250;
		xPos[0] = cxClient - 500;//第一个窗格右边界
		SendMessage(hStatus, SB_SETPARTS,4, (long)xPos);//根据四个横坐标成四栏
		if (IsIconic(hwnd))//如果按下最小化按钮
			ShowWindow(hwnd, SW_HIDE);//隐藏窗口
		MoveWindow(hEdit, 1, 1, cxClient - 4, cyClient - 29, true);//-4是留出宽度画边框，-29是把边框画在状态栏上面，状态栏高度25，留4个宽度画边框
		MoveWindow(hStatus, 0, 0, 0, 0, true);//调整状态栏在客户区变化后的最下面，不用设置大小位置，因为大小位置固定不可变
		for (int i = 0; i <4; i++)
			SendMessage(hStatus, SB_SETTEXT, i, (long)statusInfo[i].c_str());//对每一栏显示文本
		if (nSelectWallpaper > 0)//如果设置了某张壁纸
			HighLineWallPaper(hEdit, nSelectWallpaper);//高亮显示
		break;
	case WM_CTLCOLORSTATIC://编辑框初始化过程
		hdc = (HDC)wParam;//wParam是编辑框的画笔，lParam是编辑框的句柄
		SetBkColor(hdc, textBkColor);//设置字体背景颜色，应与编辑框背景颜色一致，否则存在2种背景色不好看
		SetTextColor(hdc, textColor);//设置字体的颜色
		return (long)hBKBrush;//这里不能用break,否则会调用下面的DefWindowProc，将颜色设回默认的，返回一个画刷，编辑框的默认窗口过程会用这个画刷作为背景色
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);//控件ID
		int wmEvent = HIWORD(wParam);//控件上的操作
		switch (wmId)
		{
		case ID_ADDWAPPER://添加壁纸目录
			AddWapperDirectory(hwnd);//添加壁纸目录
			break;
		case IDM_MODIFYWAPPERDIRECTORY://修改壁纸路径
			DialogBox(hInst,TEXT("IDD_DIALOGMODIFYDIRECTORY"),hwnd,DlgModifyDirectoryProc);//弹出修改壁纸路径对话框
			break;
		case IDM_EDIT_FIND: //查找
			hDlgFind = FindDialog(hwnd,fr, szFindText);//弹出查找对话框操作
			break;
		case IDM_EDIT_FINDNEXT://查找下一个
			if (szFindText[0] == '\0')//如果还没有要查找的字符串
				hDlgFind =FindDialog(hwnd,fr, szFindText);//弹出查找对话框操作，让用户输入字符串查找
			else
			{
				if (!MyFindText(fr, bDown))//查找下一个
					OKMessage(hDlgFind, TEXT("找不到\"%s\""), fr.lpstrFindWhat);//查找不到
			}
			return 0;
		case IDM_EDIT_GOTO: //转到
			DialogBox(hInst, TEXT("IDD_DIALOGGOTO"), hwnd, DlgGotoProc);
			return 0;
		case IDM_FORMAT_FONT://字体
			if (ChooseFont(&cf))//选择字体
			{
				DeleteObject(hFont);//删除上一次的画毛
				hFont = CreateFontIndirect(&lf);//根据逻辑字体创建字体句柄
				SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);//对编辑框发送消息，设置新画笔
				InvalidateRect(hEdit, nullptr, true);//用新的字体重新显示
			}
			break;
		case IDM_FONTCOLOR://字体颜色
			if (ChooseColor(&cc))//该函数创建一个能使用户从中选择颜色的通用颜色对话框
			{
				textColor = cc.rgbResult;//选择的字体颜色
				InvalidateRect(hEdit, nullptr, true);//编辑框重绘
			};
			break;
		case IDM_BKCOLOR://背景颜色
			if (ChooseColor(&cc))//该函数创建一个能使用户从中选择颜色的通用颜色对话框
			{
				textBkColor = cc.rgbResult;//选择的字体背景颜色
				DeleteObject(hBKBrush);//删除旧画刷
				hBKBrush = CreateSolidBrush(cc.rgbResult);//用背景色，创建新画刷
				InvalidateRect(hEdit, nullptr, true);//编辑框重绘
			};
			break;
		case IDM_WALLPAPERSET:
			DialogBox(hInst, TEXT("IDD_DIALOGSET"), hwnd, DlgWallpaperSet);
			break;
		case IDM_SHOWWALLPAPER://查看更多壁纸
			ShowWindow(hwnd, SW_NORMAL);//显示窗口
			SetForegroundWindow(hwnd);//将窗口放到前台，即置顶
			break;
		case IDM_PAUSE://暂停
			if (startChange)//如果已开始换壁纸，点击后暂停，提示点击后能开始
			{
				ModifyMenu(hMenu, IDM_PAUSE, MF_BYCOMMAND, IDM_PAUSE, TEXT("开始\tCtrl+P"));//提示点击后能开始
				startChange = false;//暂停标记
				KillTimer(hwnd, wallpaperID);//杀死换壁纸定时器
			}
			else
			{
				ModifyMenu(hMenu, IDM_PAUSE, MF_BYCOMMAND, IDM_PAUSE, TEXT("暂停\tCtrl+P"));
				startChange = true;//开始标记
				SetTimer(hwnd,wallpaperID, wallPaperTime, 0);//开启换壁纸定时器
			}
			break;
		case IDM_PREWALLPAPER://上一张
			recordIndex--;//上一张的记录索引
			if (recordIndex>=0)
			nSelectWallpaper = recordChange[recordIndex];//返回上一张壁纸的索引
			HighLineWallPaper(hEdit, nSelectWallpaper);//高亮显示
			ChangeWallPaper();//改变壁纸
			break;
		case IDM_NEXTWALLPAPER://下一张
			if (recordIndex == recordChange.size() - 1 || recordChange.size() == 0)//当前壁纸是最新壁纸，不是以前换过的或者换壁纸记录为0
			{
				NextWallPaper();//产生下一张壁纸索引
				recordChange.push_back(nSelectWallpaper);//保存换过的壁纸中的记录中的索引
				recordIndex = recordChange.size() - 1;//当前壁纸在换过的壁纸中的记录中的索引是最后一个索引
			}
			else
			{
				recordIndex++;//下一张的记录索引
				nSelectWallpaper = recordChange[recordIndex];//返回下一张壁纸的索引
			}
			HighLineWallPaper(hEdit, nSelectWallpaper);//高亮显示
			ChangeWallPaper();//改变壁纸
			break;
		case IDM_RANDOM://换壁纸模式，随机换还是顺序换
			if (randomChange)//如果现在是随机换模式，点击后顺序换，提示点击后能随机
			{
				ModifyMenu(hMenu, IDM_RANDOM, MF_BYCOMMAND, IDM_RANDOM, TEXT("随机"));//提示点击后能随机
				randomChange = false;//顺序换标记
			}
			else
			{
				ModifyMenu(hMenu, IDM_RANDOM, MF_BYCOMMAND, IDM_RANDOM, TEXT("顺序"));
				randomChange = true;//随机换标记
			}
			break;
		case IDM_MINI://所有窗口最小化才更换壁纸菜单
			if (changeByOnlyDestop)
			{
				CheckMenuItem(hMenu, IDM_MINI, MF_UNCHECKED);//在所有窗口最小化才更换壁纸菜单上去掉勾
				changeByOnlyDestop = false;//所有窗口最小化才更换壁标记为false
			}
			else
			{
				CheckMenuItem(hMenu, IDM_MINI, MF_CHECKED);//在所有窗口最小化才更换壁纸菜单上打上勾
				changeByOnlyDestop = true;
			}
			break;
		}
	}
		break;
	case WM_TIMER://定时器消息
		switch (wParam)
		{
		case clockID://更新时间
			FormatTime();//对系统时间进行格式化
			SendMessage(hStatus, SB_SETTEXT, 1, (long)statusInfo[1].c_str());//对第二栏更新时间 
			break;
		case tipID://气泡提示定时到了
			Shell_NotifyIcon(NIM_MODIFY, &nid);//修改托盘区域的一个图标，再次显示气泡
			break;
		case wallpaperID://换壁纸提示到了
			EnumWindows(EnumWindowProc, 0);//枚举所有屏幕上的顶层窗口，并将窗口句柄传送给应用程序定义的回调函数
			{
				HWND hDestop = GetDesktopWindow();
				HDC  hdcScr = GetDCEx(hDestop, 0, DCX_LOCKWINDOWUPDATE);//获得屏幕的画笔,即使屏幕锁住也能刷新
				//TextOut(hdcScr, 20, 400, temp.c_str(), temp.size());
			}
			if (wallPaperCount > 0)//图片数量不为0
			{
				if (changeByOnlyDestop)//如果仅有桌面这个窗口才换壁纸
				{
					if (allTitle.size() == 1)//如果只有1个窗口即桌面Window Manager在桌面上显示
					{//因为只有桌面窗口，所以程序在后台运行，不用高亮显示
						NextWallPaper();//产生下一张壁纸索引
						ChangeWallPaper();//改变壁纸
						recordChange.push_back(nSelectWallpaper);//保存换过的壁纸中的记录中的索引
						recordIndex = recordChange.size() - 1;//当前壁纸在换过的壁纸中的记录中的索引是最后一个索引
					}
				}
				else
				{
					NextWallPaper();//产生下一张壁纸索引
					HighLineWallPaper(hEdit, nSelectWallpaper);//对当前壁纸高亮显示
					ChangeWallPaper();//改变壁纸
					recordChange.push_back(nSelectWallpaper);//保存换过的壁纸中的记录中的索引
					recordIndex = recordChange.size() - 1;//当前壁纸在换过的壁纸中的记录中的索引是最后一个索引
				}
			}
			temp.clear();
			allTitle.clear();//清空窗口标题
			break;
		}
		break;
	case WM_USER:
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN://在托盘图标上单击后
			ShowWindow(hwnd, SW_NORMAL);//显示窗口
			SetForegroundWindow(hwnd);//将窗口放到前台，即置顶
			break;
		case WM_RBUTTONDOWN:////在托盘图标上右键单击后
		{
			PopupMyMenu(hwnd, hMenu);//弹出菜单
			break;
		}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		DrawBorder(hwnd, blueBrush);//绘制边框
		break;
	case WM_CLOSE://窗口关闭
		Shell_NotifyIcon(NIM_DELETE, &nid);//删除托盘图标
		DestroyWindow(hwnd);//销毁窗口
		GetWindowRect(hwnd, &windowRect);//获得窗口大小信息
		WriteSaveFile();//将壁纸的信息保存到存档
		break;
	case WM_DESTROY://窗口销毁
		PostQuitMessage(0);//抛出返回值为0的消息，从GetMessage中的循环体中出来
		KillTimer(hwnd, wallpaperID);//删除定时器
		KillTimer(hwnd, tipID);
		KillTimer(hwnd,clockID);
		DeleteObject(blueBrush);//删除画刷
		DeleteObject(hFont);//删除字体
		if (hBKBrush)
			DeleteObject(hBKBrush);//删除旧画刷
		break;
	default:
		if (UMsg == WM_FIND)//不能放在case 语句中，因为case语句要常量，而WM_FINDREPALCE是自己定义的一个变量
		{
			pfr = (LPFINDREPLACE)lParam;//当收到这个消息，lparam参数保留着FINDREPLACE结构指针
			if (pfr->Flags&FR_DIALOGTERM)//如果按下了关闭按钮
				hDlgFind = 0;//对对话框句柄置空
			if (pfr->Flags&FR_DOWN)//向下按钮被按下
				bDown = true;
			else
				bDown = false;
			if (pfr->Flags&FR_FINDNEXT)//如果按下了查找下一个
			{
				if (!MyFindText(*pfr, bDown))//查找下一个
					OKMessage(hDlgFind, TEXT("找不到\"%s\""), pfr->lpstrFindWhat);//查找不到
			}
		}
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	WNDCLASSEX wndclass;//窗口类
	wndclass.cbClsExtra = 0;//为类分配的额外空间，可以保存一些地址、变量
	wndclass.cbWndExtra = 0;//为窗口分配的额外空间，可以保存一些地址、变量
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//背景画刷
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//鼠标光标
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON3));//图标
	wndclass.hInstance = hInstance;//实例名柄，程序的PID，一个整数，根据这个整数，系统可以找到进程的地址
	wndclass.lpfnWndProc = WndProc;//窗口过程
	wndclass.lpszClassName = TEXT("zeng");//类名
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);//菜单
	wndclass.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;//哪些改变后要重画，设置水平和垂直改变后重绘, CS_DBLCLKS 接收鼠标双击消息
	wndclass.cbSize = sizeof(wndclass);//窗口大小 
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));//小图标
	if (!RegisterClassEx(&wndclass))//注册窗口类
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("wallpaper"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);//创建窗口
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	ShowWindow(hwnd, nCmdShow);//显示窗口
	UpdateWindow(hwnd);//进行重绘
	if (currentDirectorys.size() == 0)//如果没有壁纸目录
	{
		MessageBox(hwnd, TEXT("请在【文件】->【添加壁纸目录】进行添加目录"), TEXT("没有壁纸目录"), MB_OK);
		SendMessage(hwnd, WM_COMMAND, ID_ADDWAPPER, 0);//发送消息添加目录
	}
	HACCEL hAccel = LoadAccelerators(hInstance, TEXT("WALLPAPER"));//从资源文件读取加速键句柄
	MSG msg;//消息结构
	RegisterHotKey(0, ID_HOTKEY,MOD_CONTROL|MOD_ALT, 'M');//定义系统热键（系统快捷键，即使在后台运行，也能收到快捷键消息),用来显示窗口
	while (GetMessage(&msg, nullptr, 0, 0))//从消息队列获取消息
	{
		if (msg.message == WM_HOTKEY)//如果是系统热键
			SendMessage(hwnd, WM_COMMAND,IDM_SHOWWALLPAPER, 0);//发送消息，显示窗口
		if (!TranslateAccelerator(hwnd, hAccel, &msg))////TranslateAccelerator，函数功能：翻译加速键表。该函数处理菜单命令中的加速键
		{
			TranslateMessage(&msg);//将一些按键消息转换成字符消息放在按键消息后面
			DispatchMessage(&msg);//分发消息，即调用窗口过程进行处理消息
		}
	}
}
