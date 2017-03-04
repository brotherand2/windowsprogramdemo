#include"resource.h"
#include<shlobj.h>
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
	HBITMAP hBm[5];
	AppendMenu(hMenu, MF_STRING, IDM_SHOWWALLPAPER, TEXT("查看更多壁纸"));
	AppendMenu(hMenu, MF_STRING, IDM_NEXTWALLPAPER, TEXT("下一张"));
	AppendMenu(hMenu, MF_STRING, IDM_PREWALLPAPER, TEXT("上一张"));
	AppendMenu(hMenu, MF_STRING, IDM_PAUSE, TEXT("暂停"));
	AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("退出"));
	hBm[0] = LoadBitmap(hInst, TEXT("IDB_BITMAP1"));//读取位图
	SetMenuItemBitmaps(hMenu, IDM_SHOWWALLPAPER, MF_BYCOMMAND, hBm[0], 0);//设置菜单位图
	DeleteObject(hBm);
	hBm[1] = LoadBitmap(hInst, TEXT("IDB_BITMAP2"));//读取位图
	SetMenuItemBitmaps(hMenu, IDM_NEXTWALLPAPER, MF_BYCOMMAND, hBm[1], 0);//设置菜单位图
	DeleteObject(hBm);
	hBm[2] = LoadBitmap(hInst, TEXT("IDB_BITMAP3"));//读取位图
	SetMenuItemBitmaps(hMenu, IDM_PREWALLPAPER, MF_BYCOMMAND, hBm[2], 0);//设置菜单位图
	DeleteObject(hBm);
	hBm[3] = LoadBitmap(hInst, TEXT("IDB_BITMAP4"));//读取位图
	SetMenuItemBitmaps(hMenu, IDM_PAUSE, MF_BYCOMMAND, hBm[3], 0);//设置菜单位图
	hBm[4] = LoadBitmap(hInst, TEXT("IDB_BITMAP5"));//读取位图
	SetMenuItemBitmaps(hMenu, IDM_EXIT, MF_BYCOMMAND, hBm[4], 0);//设置菜单位图
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
			fileName.push_back(fd.cFileName);//将当前文件名保存
			filePath.push_back(CurrentPath);//将当前文件路径保存
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
		if (title[0] != '\0')//如果标题不为空
			allTitle.push_back(title);//加入标题数组
		temp += tstring(title)+TEXT("   ");
	}
	return true;//为继续枚举下一个窗口，回调函数必须返回TRUE；若停止列表，它必须返回FALSE
}
int _stdcall DlgProc(HWND hwnd, unsigned int message, unsigned wParam, long lParam)//对话框窗口过程
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
	int id = TrackPopupMenu(hMenu, TPM_RETURNCMD, mousePos.x, mousePos.y, 0, hwnd, 0);//弹出菜单,TPM_RETURNCMD返回菜单ID
	switch (id)
	{
	case IDM_EXIT:
		SendMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	case IDM_NEXTWALLPAPER://上一张
		break;
	case IDM_PREWALLPAPER://下一张
		break;
	case IDM_PAUSE://暂停
		if (startChange)//如果开始换壁纸
		{
			ModifyMenu(hMenu, IDM_PAUSE, MF_BYCOMMAND, IDM_PAUSE, TEXT("开始"));
			startChange = false;//暂停标记
		}
		else
		{
			ModifyMenu(hMenu, IDM_PAUSE, MF_BYCOMMAND, IDM_PAUSE, TEXT("暂停"));
			startChange = true;//开始标记
		}
		break;
	}
}
//选择文件夹对话框回调函数  
int CALLBACK BrowseCallBackFun(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szPath[MAX_PATH];//保存选择的路径
	switch (uMsg)
	{
	case BFFM_INITIALIZED://选择文件夹对话框初始化 
		SendMessage(hwnd, BFFM_SETSELECTION, true, lpData);//从BROWSEINFO中的lParam传来的默认路径在lpData中，将wParam设为true,lParam设为以'\0'为尾字符串则可设置默认打开目录
		SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, lpData);//设置状态栏上文本的我的图片路径
		SetWindowText(hwnd, TEXT("选择图片文件夹"));
		break;
	case BFFM_SELCHANGED://选择其它文件夹后
		SHGetPathFromIDList((LPCITEMIDLIST)lParam, szPath);//lParam参数是一个LPCITEMIDLIST结构，转换成字符串的路径
		SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (long)szPath);//设置状态栏上文本的新路径
		break;
	}
	return false;
}
void HighLineWallPaper(HWND hEdit, int  nSelectWallpaper)//对当前的壁纸进行高亮显示
{
	int lineIndex;//指定行首字符在文本中的位置（以字节数表示）
	int nextLineIndex;//指定行下一行首字符在文本中的位置（以字节数表示）
	lineIndex = SendMessage(hEdit, EM_LINEINDEX, nSelectWallpaper + 1, 0);//得到指定行首字符在文本中的位置（以字节数表示），因第1行列名称，第0张图在第2行，所以第1张图在第3行
	nextLineIndex = SendMessage(hEdit, EM_LINEINDEX, nSelectWallpaper + 2, 0);//得到指定行下一行首字符在文本中的位置（以字节数表示）,因第1行列名称，第0张图在第2行，所以第1张图在第3行
	SendMessage(hEdit, EM_SETSEL, lineIndex, nextLineIndex);//选中指定行
	SendMessage(hEdit, EM_SCROLLCARET, 0, 0);// 把可见范围移至光标处,因定设置光标后，光标可能不在范围内，所以要滚动到光标处
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
void InitBrowseInfo(HWND hwnd, BROWSEINFO &bi)// 初始化打开目录结构
{
	bi.hwndOwner = hwnd;//浏览文件夹对话框的父窗体句柄
	bi.iImage = 0;// 被选择的文件夹的图片序号
	bi.lParam = (long)szMyPictures;//对话框传递给回调函数的一个参数指针,传给回调函数的参数,设置默认路径为我的图片文件夹
	bi.lpfn = BrowseCallBackFun;//应用程序定义的浏览对话框回调函数的地址。当对话框中的事件发生时，该对话框将调用回调函数。该参数可用为NULL
	bi.lpszTitle = TEXT("你来了，我等你很久了，先选择一个图片文件夹先");//该浏览文件夹对话框的显示文本，用来提示该浏览文件夹对话框的功能、作用和目的
	bi.pidlRoot = 0;// ITEMIDLIST结构的地址，包含浏览时的初始根目录，而且只有被指定的目录和其子目录才显示在浏览文件夹对话框中。该成员变量可以是NULL，在此时桌面目录将被使用
	bi.pszDisplayName = szDirectoryName;//用来保存用户选中的目录字符串的内存地址。该缓冲区的大小缺省是定义的MAX_PATH常量宏
	bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS;//该标志位描述了对话框的选项。它可以为0
	//BIF_EDITBOX：浏览对话框中包含一个编辑框，在该编辑框中用户可以输入选中项的名字
	//BIF_STATUSTEXT：在对话框中包含一个状态区域。通过给对话框发送消息使回调函数设置状态文本
	//BIF_RETURNONLYFSDIRS：仅仅返回文件系统的目录。例如：在浏览文件夹对话框中，当选中任意一个目录时，该“OK”按钮可用，而当选中“我的电脑”或“网上邻居”等非有意义的节点时，“OK”按钮为灰色
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
void DisplayDistribitue(HWND hEdit, const vector<tstring> &fileName)//显示文件属性
{
	TCHAR szBuffer[1024];//字符缓冲
	WIN32_FILE_ATTRIBUTE_DATA attri;//文件属性结构
	tstring szPath;//文件路径
	static TCHAR *szColumn[8] = {TEXT("索引"),TEXT("文件名"),TEXT("大小"),TEXT("属性"),TEXT("创建时间"),TEXT("最后一次访问时间"),TEXT("最后一次修改时间"),TEXT("路径")};
	StringCchPrintf(szBuffer, 1024, TEXT("%-9s%-30s%-20s%-15s%-30s%-30s%-30s%-30s\r\n"),szColumn[0],szColumn[1],
		szColumn[2], szColumn[3], szColumn[4], szColumn[5], szColumn[6], szColumn[7]);//格式化列名
	tstring temp = szBuffer;//在编辑上显示的字符串
	tstring index;//索引
	for (int i = 0; i < fileName.size(); i++)
	{
		GetFileAttributesEx((filePath[i]+TEXT("\\")+fileName[i]).c_str(), GetFileExInfoStandard, &attri);//得到当前文件的属性,GetFileExInfoStandard表示表准文件属性，最后一个参数是 WIN32_FILE_ATTRIBUTE_DATA结构的指针
		fileCreateTime = FormatFileTime(attri.ftCreationTime);//文件创建时间
		fileLastAccessTime = FormatFileTime(attri.ftLastAccessTime); // 文件最后一次访问时间
		fileLastWriteTime = FormatFileTime(attri.ftLastWriteTime);//文件最后一次修改时间
		fileAttributes = FileAttribute(attri.dwFileAttributes);//文件属性
		szPath =filePath[i].c_str();//文件路径
		lstrcpyn(szName, fileName[i].c_str(),30);//文件名
		if (fileName[i].size()>29)//如果文件名太长
			lstrcpyn(szName+24, TEXT("*.jpg"),6);
		fileSize = calculateCapacity(attri.nFileSizeHigh, attri.nFileSizeLow);//计算文件大小
		index = ToString(i) + TEXT(":");//将索引整数转换成字符串
		StringCchPrintf(szBuffer, 1024, TEXT("%-9s%-30s%-20s%-15s%-30s%-30s%-30s%-30s\r\n"), index.c_str(), szName, fileSize.c_str(),
		fileAttributes.c_str(), fileCreateTime.c_str(), fileLastAccessTime.c_str(), fileLastWriteTime.c_str(),
		szPath.c_str());//格式化字符串
		temp += szBuffer;
	}
	SetWindowText(hEdit, temp.c_str());//在编辑框显示
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
		row = SendMessage(hEdit, EM_LINEFROMCHAR, -1, 0) + 1;//获得指定位置所在的行号，-1表示当前光标所在位置，因为位置的索引从0开始，所以要加1
		statusInfo[2] = TEXT("行号:") + ToString(row);//将行号转换成字符串
		SendMessage(hStatus, SB_SETTEXT, 2, (long)statusInfo[2].c_str());//对第二栏更新行号 
		first = SendMessage(hEdit, EM_LINEINDEX, -1, 0);//当前光标所在行，首字符在编辑中的位置
		SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//当前选中文字的开始、结束位置，若未选中则是当前光标相对于开头位置的偏移（字符个数）			
		col = startPos - first + 1;//当前光标的位置-当前光标所在行第1个字符的位置为列数，，因为位置的索引从0开始，所以要加1
		statusInfo[3] = TEXT("列号:") + ToString(col);//将列号转换成字符串
		SendMessage(hStatus, SB_SETTEXT, 3, (long)statusInfo[3].c_str());//对第三栏更新列号 
		nSelectWallpaper = row - 2;//因为第1行是列名称，第2行是第0张，第3行是第1张，所以当前选中的索引是row-2张
		HighLineWallPaper(hEdit, nSelectWallpaper);//对当前行高亮显示，但还未设置为当前行为壁纸，只有鼠标双击才设置
		break;
	case WM_LBUTTONDBLCLK:
		SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)(filePath[nSelectWallpaper] + TEXT("\\") + fileName[nSelectWallpaper]).c_str(), 0);//从图片中随机选1个，作为壁纸
		statusInfo[0] = tstring("当前壁纸:") + filePath[nSelectWallpaper] + TEXT("\\") + fileName[nSelectWallpaper];
		SendMessage(hStatus, SB_SETTEXT, 0, (long)statusInfo[0].c_str());//第一栏显示路径
		break;
	case WM_RBUTTONDOWN://右键单击
		PopupMyMenu(hEdit, hMenu);//弹出菜单
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
bool MyFindText(HWND hEdit, FINDREPLACE &fr, bool bDown)//查找文本
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
	static HWND hEdit;//记事本编辑框句柄
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
				MessageBox(hEditGoto, TEXT("记事本超过了总行数"), TEXT("记事本 -跳行"), 0);
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
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//主窗口 窗口过程
{
	PAINTSTRUCT ps;//画笔结构
	HDC hdc;//画笔
	RECT rect;//矩形
	int size;//数组大小
	static NOTIFYICONDATA nid;//系统需要的用来传递托盘区域消息的信息
	static POINT mousePos;//鼠标位置
	static HBRUSH oldBrush;//保存旧画刷
	static HBRUSH blueBrush;//蓝色画刷，选中时高亮显示的颜色
	static HINSTANCE hInst;
	static HFONT hFont;//字体
	static BROWSEINFO bi;//包含有用户选中目录的重要信息
	static LPITEMIDLIST plDlist;//保存用户选择的目录信息结构
	static LPMALLOC pMalloc;//LPMALLOC是一个IMalloc指针，IMALLOC实现了一个非常完整的内存分配引擎，用于释放ITEMIDLIST指针所指向的空间
	static CHOOSEFONT cf;//字体结构
	static CHOOSECOLOR cc;//颜色结构
	static LOGFONT lf;//逻辑字体
	static COLORREF textColor = RGB(0, 0, 0);//默认的字体颜色
	static COLORREF textBkColor=RGB(200,200,200);//默认的字体背景颜色轻灰色
	static HBRUSH hBKBrush;//背景画刷
	static COLORREF color[16];//颜色对话框中保存16个自定义颜色
	static HWND hEdit;//编辑框
	static bool bDown;//向下按钮是否按下
	static TCHAR szFindText[MAX_PATH]=TEXT("");//要查找的字符串
	int xPos[4]{0, 0, 0, 0};//状态栏的四个右横坐标
	static FINDREPLACE fr;//查找和替换结构
	LPFINDREPLACE pfr = nullptr;//查找和替换结构的指针
	static unsigned int WM_FIND;//自定义的消息，用于接收查找的消息
	switch (UMsg)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;//程序实例句柄
		hBKBrush = CreateSolidBrush(textBkColor);//创建背景画刷轻灰色
		InitFont(hwnd, cf, lf);//初始化字体结构
		InitColor(hwnd, cc, color);//初始化颜色结构
		InitNotifyIconData(hwnd, hInst, nid);//初始化托盘图标结构
		WM_FIND = RegisterWindowMessage(FINDMSGSTRING);//查找和替换对换框会发送消息给用FINDMSGSTRING注册的消息		
		Shell_NotifyIcon(NIM_ADD, &nid);//向托盘区域添加一个图标
		hMenu = CreateMyPopMenu(hInst);//创建菜单
		FindAllFile(wallpaper,includeSubDirectory);//查找所有图片
		SetTimer(hwnd, wallpaperID, wallPaperTime, nullptr);//设置壁纸定时器
		SetTimer(hwnd, tipID, tipTime, nullptr);//设置气泡提示定时器
		SetTimer(hwnd,clockID,1000, nullptr);//设置时钟定时器，每一秒更新时间一次
		blueBrush = CreateSolidBrush(RGB(102, 179, 255));//蓝色画刷
		hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);//默认字体
		SHGetSpecialFolderLocation(hwnd, CSIDL_MYPICTURES, &plDlist);//获得我的图片文件夹路径
		SHGetPathFromIDList(plDlist, szMyPictures);//将plDlist路径（结构体）转换成字符串路径
		SHGetSpecialFolderLocation(hwnd, CSIDL_PERSONAL, &plDlist);//获得我的文档文件夹路径
		SHGetPathFromIDList(plDlist, szMyDocument);//将plDlist路径（结构体）转换成字符串路径
		if (SHGetMalloc(&pMalloc) == S_OK);//创建IMALLOC对象
		{
			pMalloc->Free(plDlist);//释放plDlist指向的空间
			pMalloc->Release();//释放pMalloc的空间
		}
		hStatus = CreateWindow(STATUSCLASSNAME, 0, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, hwnd, (HMENU)IDS_STATE, hInst, nullptr);//创建状态栏
		hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL |ES_READONLY| ES_MULTILINE | ES_NOHIDESEL, 0, 0, 0, 0,
			hwnd, (HMENU)ID_EDIT, hInst, nullptr);//创建有边界的多行只读编辑框，ES_NOHIDESEL在编辑框失去焦点后选中的文本依然保持高亮显示
		SendMessage(hEdit, EM_LIMITTEXT, 0, 0);//当wParam为0时，多行编辑控伯最多0x7FFFFFFE个字符，默认是30000个字符
		SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);
		oldEditTextProc = (WNDPROC)SetWindowLong(hEdit, GWL_WNDPROC, (long)EditTextProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘、鼠标消息，因为
		//默认是捕获不到这些消息的，但要用的这些消息，比如当鼠标或按键按下时更新状态栏显示的行列信息
		//但是不能用SetClassLong(hEdit,GCL_WNDPROC),因为它是修改edit类的窗口过程，它只会对下一个创建的编辑框生效，对当前
		//编辑框不会改变，而且用SetClassLong,会影响以后的编辑框，而我只想改变当前编辑框，不想改变其它编辑框
		DisplayDistribitue(hEdit, fileName);//显示文件属性
		srand(time(0));//将当前时间作为随机函数种子
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
		if (nSelectWallpaper >= 0)//如果设置了某张壁纸
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
			InitBrowseInfo(hwnd, bi);//初始化选择目录结构
			plDlist = SHBrowseForFolder(&bi);//弹出选择目录对话框,路径保存的指针plDlist
			if (plDlist)//如果选择了一个目录
			{
				SHGetPathFromIDList(plDlist, szDirectoryPath);//将plDlist路径（结构体）转换成字符串路径				
				wallpaper = szDirectoryPath;
				fileName.clear();
				filePath.clear();
				FindAllFile(wallpaper,includeSubDirectory);
				SetWindowText(hEdit, 0);//清空编辑框
				DisplayDistribitue(hEdit, fileName);//显示新的文件属性
				if (SHGetMalloc(&pMalloc) == S_OK);//创建IMALLOC对象
				{
					pMalloc->Free(plDlist);//释放plDlist指向的空间
					pMalloc->Release();//释放pMalloc的空间
				}
			}
			break;
		case IDM_EDIT_FIND: //查找
			hDlgFind = FindDialog(hwnd,fr, szFindText);//弹出查找对话框操作
			break;
		case IDM_EDIT_FINDNEXT://查找下一个
			if (szFindText[0] == '\0')//如果还没有要查找的字符串
				hDlgFind =FindDialog(hwnd,fr, szFindText);//弹出查找对话框操作，让用户输入字符串查找
			else
			{
				if (!MyFindText(hEdit, fr, bDown))//查找下一个
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
			size = fileName.size();//文件的个数
			{
				HWND hDestop = GetDesktopWindow();
				HDC  hdcScr = GetDCEx(hDestop, 0, DCX_LOCKWINDOWUPDATE);//获得屏幕的画笔,即使屏幕锁住也能刷新
				//TextOut(hdcScr, 20, 400, temp.c_str(), temp.size());
			}
			if (size > 0)//图片数量不为0
			{
				//MessageBox(hwnd, ToString(nSelectWallpaper).c_str, 0, 0);
				if (changeByOnlyDestop)//如果仅有桌面这个窗口才换壁纸
				{
					if (allTitle.size() == 1)//如果只有1个窗口即桌面Window Manager在桌面上显示
					{
						nSelectWallpaper = rand() % size;//随机选一张图片
						SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)(filePath[nSelectWallpaper] + TEXT("\\") + fileName[nSelectWallpaper]).c_str(), 0);//从图片中随机选1个，作为壁纸
						statusInfo[0] = tstring("当前壁纸:") + filePath[nSelectWallpaper] + TEXT("\\") + fileName[nSelectWallpaper];
						SendMessage(hStatus, SB_SETTEXT, 0, (long)statusInfo[0].c_str());//第一栏显示路径
					}
				}
				else
				{
					nSelectWallpaper = rand() % size;//随机选一张图片
					SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)(filePath[nSelectWallpaper] + TEXT("\\") + fileName[nSelectWallpaper]).c_str(), 0);//从图片中随机选1个，作为壁纸
					HighLineWallPaper(hEdit,nSelectWallpaper);//对当前壁纸高亮显示
					statusInfo[0] = tstring("当前壁纸:") + filePath[nSelectWallpaper] + TEXT("\\") + fileName[nSelectWallpaper];
					SendMessage(hStatus, SB_SETTEXT, 0, (long)statusInfo[0].c_str());//第一栏显示路径
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
				if (!MyFindText(hEdit, *pfr, bDown))//查找下一个
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
	MSG msg;//消息结构
	while (GetMessage(&msg, nullptr, 0, 0))//从消息队列获取消息
	{
		TranslateMessage(&msg);//将一些按键消息转换成字符消息放在按键消息后面
		DispatchMessage(&msg);//分发消息，即调用窗口过程进行处理消息
	}
}
