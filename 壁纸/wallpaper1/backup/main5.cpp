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
	stringstream out;//字符串流
	out << st.wYear << "年" << st.wMonth << "月" << st.wDay << "日," << st.wHour << ":" << st.wMinute << ":" << st.wSecond;//格式化字符串
	return out.str();
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
void FindFile(const tstring &wallpaper)//查找文件
{
	WIN32_FIND_DATA fd;
	HANDLE handle = FindFirstFile(wallpaper.c_str(), &fd);//查找第1个文件
	if (handle != INVALID_HANDLE_VALUE)//找到了
	{
		do
		{
			if (fd.cFileName[0] == '.')//win32下一般会有三种文件夹，一种是“.”指的是当前文件夹，一种是“..”指的是上层文件夹，最后一种就是普通的下层文件夹用文件夹名显示，cd .是本目录，cd ..是上级目录
				break;//所以为了不回到上级目录和在原目录打转，过滤.和..这二个目录
			fileName.push_back(fd.cFileName);//将当前文件名保存
		} while (FindNextFile(handle, &fd));//查找下一个文件
	}
}
int _stdcall EnumWindowProc(HWND hwnd, long lParam)//枚举函数的窗口过程
{
	TCHAR title[256];//接收窗口标题
	if (IsWindow(hwnd) && IsWindowVisible(hwnd) && !IsIconic(hwnd))
	{//如果存在这个窗口，并且窗口可见，不是最小化
		GetWindowText(hwnd, title, 256);//得到窗口标题
		temp += title + tstring(TEXT(" "));//将标题加入字符中
		if (title[0] != '\0')//如果标题不为空
			allTitle.push_back(title);//加入标题数组
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
void DrawBorder(HWND hwnd, HBRUSH hBlueBrush)//画条蓝色边框
{
	RECT rect;
	HDC hdc;
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);//得到客户区矩形
	FrameRect(hdc, &rect, hBlueBrush);//画淡蓝色画刷边框
	ReleaseDC(hwnd, hdc);
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
void SetScroll(HWND hwnd, SCROLLINFO &si)//设置滚动条
{
	si.cbSize = sizeof(si);//结构大小
	si.fMask = SIF_PAGE | SIF_RANGE;//设置的标记，只设置SIF_PAGE和SIF_RANGE，使用SIF_PAGE标志能够取得页面大小，SIF_RANGE标志时，必须把nMin和nMax字段设定为所需的卷动列范围
	si.nMin = 0;
	si.nMax = fileName.size();//最大范围的文字的个数,不是fileName.size()-1是因为要包括第1行的列名
	si.nPage = nPageLine;//一页的的行数
	SetScrollInfo(hwnd, SB_VERT, &si, true);//设置垂直滚动条参数
	si.nMax = 222;//水平滚动的最大范围，一行最长文字的字符个数，大约估计
	si.nPage = cxClient / cxChar;//滚动一页列数
	SetScrollInfo(hwnd, SB_HORZ, &si, true);//设置水平滚动条参数
}
void GetCharSize(HWND hwnd,HFONT hFont)//得到指定字体的字符大小
{
	TEXTMETRIC tm;//文本尺寸结构
	HDC hdc;
	hdc = GetDC(hwnd);//得到窗口画笔
	SelectObject(hdc, hFont);
	GetTextMetrics(hdc, &tm);//得到画笔的文本尺寸
	cxChar = tm.tmAveCharWidth;//小写字母的平均宽度
	cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;//大写字母的平均宽度
	cyChar = tm.tmHeight + tm.tmExternalLeading;//字母的平均高度
	ReleaseDC(hwnd, hdc);
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
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//主窗口 窗口过程
{
	PAINTSTRUCT ps;//画笔结构
	HDC hdc;//画笔
	RECT rect;//矩形
	static tstring wallpaper = TEXT("E:\\壁纸\\SogouWP\\Local\\WallPaper");//壁纸路径
	tstring format = TEXT("\\*.jpg");//壁纸后缀格式
	int size;//数组大小
	static SCROLLINFO si;//滚动条信息
	static int iVertPos, iHorzPos;//水平和垂直滚动条位置
	int start, end;//滚动条滚动时，重绘的起始和结束位置
	TCHAR szBuffer[1024];//字符缓冲
	static int iPerLine;//滚动一行的刻度
	static int iAccumLine;//滚动的刻度
	static int ulScrollLines;//滚轴滚动一次滚动的行数
	static NOTIFYICONDATA nid;//系统需要的用来传递托盘区域消息的信息
	static POINT mousePos;//鼠标位置
	static int nSelectedLine;//当前被选中的行,索引从0开始
	static int nLastSelectedWallpapaer;//上一次被选中的壁纸
	static int nSelectWallpaper;//被选中的壁纸
	static HBRUSH oldBrush;//保存旧画刷
	static HBRUSH blueBrush;//蓝色画刷，选中时高亮显示的颜色
	static HINSTANCE hInst;
	static HMENU hMenu;//菜单句柄
	static WIN32_FILE_ATTRIBUTE_DATA attri;//文件属性结构
	static HFONT hFont;//字体
	static BROWSEINFO bi;//包含有用户选中目录的重要信息
	static LPITEMIDLIST plDlist;//保存用户选择的目录信息结构
	static LPMALLOC pMalloc;//LPMALLOC是一个IMalloc指针，IMALLOC实现了一个非常完整的内存分配引擎，用于释放ITEMIDLIST指针所指向的空间
	static CHOOSEFONT cf;//字体结构
	static CHOOSECOLOR cc;//颜色结构
	static LOGFONT lf;//逻辑字体
	static COLORREF textColor = RGB(0, 0, 0);//默认的字体颜色
	static COLORREF textBkColor = RGB(255, 255, 255);//默认的字体背景颜色
	static HBRUSH hBKBrush;//背景画刷
	static COLORREF color[16];//颜色对话框中保存16个自定义颜色
	switch (UMsg)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;//程序实例句柄
		hBKBrush = CreateSolidBrush(textBkColor);//创建背景画刷
		InitFont(hwnd, cf, lf);//初始化字体结构
		InitColor(hwnd, cc, color);//初始化颜色结构
		InitNotifyIconData(hwnd, hInst, nid);//初始化托盘图标结构
		Shell_NotifyIcon(NIM_ADD, &nid);//向托盘区域添加一个图标
		hMenu = CreateMyPopMenu(hInst);//创建菜单
		FindFile(wallpaper + format);//查找所有图片
		SetTimer(hwnd, wallpaperID, wallPaperTime, nullptr);//设置壁纸定时器
		SetTimer(hwnd, tipID, tipTime, nullptr);//设置气泡提示定时器
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ulScrollLines, 0);//获取滚轴滚动一次滚动的行数
		if (ulScrollLines)
			iPerLine = WHEEL_DELTA / ulScrollLines;//计算滚动一行的刻度
		else
			iPerLine = 0;
		srand(time(0));//将当前时间作为随机函数种子
		blueBrush = CreateSolidBrush(RGB(102, 179, 255));//蓝色画刷
		hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);//默认字体
		GetCharSize(hwnd, hFont);//得到指定字体的字符大小
		SHGetSpecialFolderLocation(hwnd, CSIDL_MYPICTURES, &plDlist);//获得我的图片文件夹路径
		SHGetPathFromIDList(plDlist, szMyPictures);//将plDlist路径（结构体）转换成字符串路径
		SHGetSpecialFolderLocation(hwnd, CSIDL_PERSONAL, &plDlist);//获得我的文档文件夹路径
		SHGetPathFromIDList(plDlist, szMyDocument);//将plDlist路径（结构体）转换成字符串路径
		if (SHGetMalloc(&pMalloc) == S_OK);//创建IMALLOC对象
		{
			pMalloc->Free(plDlist);//释放plDlist指向的空间
			pMalloc->Release();//释放pMalloc的空间
		}
		break;
	case WM_SIZE://大小改变后
		cxClient = LOWORD(lParam);//客户区的宽度、高度
		cyClient = HIWORD(lParam);
		nPageLine = cyClient / cyChar;//获得一页中行数
		SetScroll(hwnd, si);//设置滚动条
		if (IsIconic(hwnd))//如果按下最小化按钮
			ShowWindow(hwnd, SW_HIDE);//隐藏窗口
		break;
	case WM_MOUSEWHEEL://鼠标滚动
		if (ulScrollLines == 0)//鼠标不允许滚动，退出
			break;
		iAccumLine += (SHORT)HIWORD(wParam);//加上滚动的刻度
		while (iAccumLine >= iPerLine)//如果滚动超过一行的刻度,鼠标向下滚，内容往上滚
		{
			iAccumLine -= iPerLine;//减去一行的刻度
			SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, lParam);//发送消息，内容向上滚动一行
		}
		while (iAccumLine <= -iPerLine)//如果滚动少于一行的刻度，鼠标向上滚，内容往下滚
		{
			iAccumLine += iPerLine;//加上一行的刻度
			SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, lParam);//内容向下滚动
		}
		break;
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
				//MessageBox(hwnd, szDirectoryPath, 0, 0);
				wallpaper = szDirectoryPath;
				fileName.clear();
				FindFile(wallpaper+format);
				SetScroll(hwnd, si);
				if (SHGetMalloc(&pMalloc) == S_OK);//创建IMALLOC对象
				{
					pMalloc->Free(plDlist);//释放plDlist指向的空间
					pMalloc->Release();//释放pMalloc的空间
				}
			}
			break;
		case IDM_FORMAT_FONT://字体
			if (ChooseFont(&cf))//选择字体
			{
				DeleteObject(hFont);//删除上一次的画毛
				hFont = 0;
				//hFont = CreateFontIndirect(&lf);//根据逻辑字体创建字体句柄
				hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0,lf.lfFaceName);//默认字体
				GetCharSize(hwnd, hFont);//计算新字体字符大小
				InvalidateRect(hwnd, nullptr, true);//用新的字体重新显示
			}
			break;
		case IDM_FONTCOLOR://字体颜色
			if (ChooseColor(&cc))//该函数创建一个能使用户从中选择颜色的通用颜色对话框
			{
				textColor = cc.rgbResult;//选择的字体颜色
				InvalidateRect(hwnd, nullptr, true);//用新的颜色重新显示
			};
			break;
		case IDM_BKCOLOR://背景颜色
			if (ChooseColor(&cc))//该函数创建一个能使用户从中选择颜色的通用颜色对话框
			{
				textBkColor = cc.rgbResult;//选择的字体背景颜色
				if (hBKBrush)
				DeleteObject(hBKBrush);//删除旧画刷
				hBKBrush = CreateSolidBrush(cc.rgbResult);//用背景色，创建新画刷
				InvalidateRect(hwnd, nullptr, true);//用新的颜色重新显示
			};
			break;
		}
	}
		break;
	case WM_LBUTTONDBLCLK://鼠标双击
		//MessageBox(hwnd, TEXT(" double mouse"), 0, 0);
		break;
	case WM_LBUTTONDOWN://鼠标单击
		mousePos.x = GET_X_LPARAM(lParam);//计算鼠标当前所在的位置（横、纵坐标)
		mousePos.y = GET_Y_LPARAM(lParam);
		nSelectedLine = mousePos.y / cyChar;//计算鼠标当前被击中的行
		if (nSelectedLine >= 1 && nSelectedLine < nPageLine)//如果鼠标在客户区里并且不是第1行,是因为第1行是列名
		{
			nSelectWallpaper = nSelectedLine - 1 + iVertPos;//nSelectedLine加上当前垂直滚动条的位置，即当前的选中的壁纸，-1因为索引从0开始
			InvalidateRect(hwnd, 0, true);//将选中行进行重画，进行高亮显示
		}
		break;
	case  WM_RBUTTONDOWN://鼠标右键单击
		PopupMyMenu(hwnd, hMenu);//弹出菜单
		break;
	case WM_TIMER://定时器消息
		switch (wParam)
		{
		case tipID://提示定时到了
			Shell_NotifyIcon(NIM_MODIFY, &nid);//修改托盘区域的一个图标，再次显示气泡
			break;
		case wallpaperID://换壁纸提示到了
			EnumWindows(EnumWindowProc, 0);//枚举所有屏幕上的顶层窗口，并将窗口句柄传送给应用程序定义的回调函数
			size = fileName.size();//文件的个数
			{
				HWND hDestop = GetDesktopWindow();
				HDC  hdcScr = GetDCEx(hDestop, 0, DCX_LOCKWINDOWUPDATE);//获得屏幕的画笔,即使屏幕锁住也能刷新
				// TextOut(hdcScr, 20, 400, temp.c_str(), temp.size());
			}
			if (allTitle.size() < 2 && size>0)//如果只有1个窗口即桌面Window Manager在桌面上显示并且图片数量不为0
			{
				SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)(wallpaper +TEXT("\\")+ fileName[rand() % size]).c_str(), 0);//从图片中随机选1个，作为壁纸
			}
			temp.clear();//清空窗口标题
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
	case WM_VSCROLL://垂直滚动
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;//获得滚动条所有信息
		GetScrollInfo(hwnd, SB_VERT, &si);
		iVertPos = si.nPos;//当前垂直滚动条的位置
		switch (LOWORD(wParam))
		{
		case SB_LINEUP://向上滚
			si.nPos -= 1;
			break;
		case SB_LINEDOWN://向下滚
			si.nPos += 1;
			break;
		case SB_PAGEUP://向上翻页
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN://向下翻页
			si.nPos += si.nPage;
			break;
		case SB_TOP://回到顶部
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM://回到底端
			si.nPos = si.nMax;
			break;
		case SB_THUMBTRACK://拖动滚动条
			si.nPos = si.nTrackPos;
			break;
		}
		SetScrollInfo(hwnd, SB_VERT, &si, true);//设置新的滚动条信息
		GetScrollInfo(hwnd, SB_VERT, &si);//获得当前的滚动条信息
		if (iVertPos != si.nPos)//滚动条位置变化了
			InvalidateRect(hwnd, 0, true);
		break;
	case WM_HSCROLL:
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;//获得滚动条所有信息
		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos = si.nPos;//当前水平滚动条的位置
		switch (LOWORD(wParam))
		{
		case SB_LINELEFT://向左滚
			si.nPos -= 1;
			break;
		case SB_LINERIGHT://向右滚
			si.nPos += 1;
			break;
		case SB_PAGELEFT://向左翻页
			si.nPos -= si.nPage;
			break;
		case SB_PAGERIGHT://向右翻页
			si.nPos += si.nPage;
			break;
		case SB_TOP://回到左顶端
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM://回到右顶端
			si.nPos = si.nMax;
			break;
		case SB_THUMBPOSITION://滚动条拖拽
			si.nPos = si.nTrackPos;
			break;
		}
		SetScrollInfo(hwnd, SB_HORZ, &si, true);//设置新的滚动条信息
		GetScrollInfo(hwnd, SB_HORZ, &si);//获得当前的滚动条信息
		if (iHorzPos != si.nPos)//滚动条位置变化了
			InvalidateRect(hwnd, 0, true);
		break;
	case WM_KEYDOWN://键盘按下
		switch (wParam)
		{
		case VK_LEFT://方向键左
			SendMessage(hwnd, WM_HSCROLL, SB_LINELEFT, 0);//发送消息向左滚
			break;
		case VK_RIGHT://方向键右
			SendMessage(hwnd, WM_HSCROLL, SB_LINERIGHT, 0);//发送消息向右滚
			break;
		case VK_UP://方向键上
			SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);//发送消息向上滚
			break;
		case VK_DOWN://方向键下
			SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);//发送消息向下滚
			break;
		case VK_HOME://HOME键
			SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0);//发送消息回到顶端
			break;
		case VK_END://END键
			SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0);//发送消息回到底端
			break;
		case VK_PRIOR://PAGE UP键
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0);//发送消息向上翻页
			break;
		case VK_NEXT://PAGE DOWN 键
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0);//发送消息向下翻页
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, hFont);//设置字体
		SetTextColor(hdc, textColor);//设置字体颜色
		GetClientRect(hwnd, &rect);
		FillRect(hdc, &rect, hBKBrush);
		if (fileName.size() > 0)
		{
			SetMapMode(hdc, MM_ANISOTROPIC);//设置映射模式，逻辑单位转换成具有任意比例轴的任意单位
			SetWindowExtEx(hdc, 1, 1, nullptr);//设置逻辑单位横坐标1个单位为物理坐标的一个字符宽度，纵坐标1个单位为物理坐标的一个字符高度
			SetViewportExtEx(hdc, cxChar, cyChar, nullptr);
			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;//获得滚动条所有信息
			GetScrollInfo(hwnd, SB_VERT, &si);//当前垂直滚动条的位置
			iVertPos = si.nPos;
			GetScrollInfo(hwnd, SB_HORZ, &si);//当前水平滚动条的位置
			iHorzPos = si.nPos;
			start = max(0, min(fileName.size(), iVertPos + ps.rcPaint.top / cyChar));//滚动条滚动后，ps.rcPaint.top/cyChar要重绘的起始行，加上iVertPos获得在滚动条上绘制的位置
			end = max(0, min(fileName.size(), iVertPos + ps.rcPaint.bottom / cyChar));//滚动条滚动后，ps.rcPaint.bottom/cyChar要重绘的结束行，加上iVertPos获得在滚动条上绘制的位置
			TextOut(hdc, -iHorzPos, 0, szTop, lstrlen(szTop));//显示列名
			for (int i = start, j = 0; i <end; i++, j++)//对滚动操作后的失效区域重绘
			{
				int x = -iHorzPos;//水平绘制的起始位置，如果3，表示第3列在开头，从-3开始绘制
				int y = i - iVertPos + 1;//垂直绘制的起始位置,加上1是因为要不包括第1行的列名
				GetFileAttributesEx((wallpaper +TEXT("\\")+ fileName[i]).c_str(), GetFileExInfoStandard, &attri);//得到当前文件的属性,GetFileExInfoStandard表示表准文件属性，最后一个参数是 WIN32_FILE_ATTRIBUTE_DATA结构的指针
				fileCreateTime = FormatFileTime(attri.ftCreationTime);//文件创建时间
				fileLastAccessTime = FormatFileTime(attri.ftLastAccessTime); // 文件最后一次访问时间
				fileLastWriteTime = FormatFileTime(attri.ftLastWriteTime);//文件最后一次修改时间
				fileAttributes = FileAttribute(attri.dwFileAttributes);//文件属性
				filePath = wallpaper;//文件路径
				lstrcpyn(szName, fileName[i].c_str(),30);//文件名
				if (fileName[i].size()>29)//如果文件句太长
					lstrcpyn(szName+24, TEXT("*.jpg"),6);
				fileSize = calculateCapacity(attri.nFileSizeHigh, attri.nFileSizeLow);//计算文件大小
				StringCchPrintf(szBuffer, 1024, TEXT("%3d%-5c%-30s%-20s%-15s%-30s%-30s%-30s%-30s"), i, ':', szName, fileSize.c_str(),
					fileAttributes.c_str(), fileCreateTime.c_str(), fileLastAccessTime.c_str(), fileLastWriteTime.c_str(),
					filePath.c_str());//格式化字符串
				if (i == nSelectWallpaper)//如果当前行被选中
				{
					rect.left = 0;//要显示高亮的矩形区域，鼠标选中区域
					rect.top = y;
					rect.right = cxClient / cxChar;
					rect.bottom = y + 1;
					FillRect(hdc, &rect, blueBrush);//用蓝色高亮显示
					SetBkColor(hdc, RGB(0, 0, 255));//设置字符串的背景颜色为蓝色
					TextOut(hdc, x, y, szBuffer, lstrlen(szBuffer));//显示字符串
					SetBkColor(hdc, RGB(255, 255, 255));//恢复原来的的字符串的背景颜色白色
				}
				else
					TextOut(hdc, x, y, szBuffer, lstrlen(szBuffer));//显示字符串
			}
		}
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
		DeleteObject(blueBrush);//删除画刷
		DeleteObject(hFont);//删除字体
		if (hBKBrush)
			DeleteObject(hBKBrush);//删除旧画刷
		break;
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
