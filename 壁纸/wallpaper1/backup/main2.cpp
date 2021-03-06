#include"resource.h"
tstring calculateCapacity(long fileSizeHigh, long fileSizeLow)//计算容量大小
{
	DWORD64 size=0;//容量大小
	LARGE_INTEGER  large;//大整数结构
	large.LowPart = fileSizeLow;//低32位
	large.HighPart = fileSizeHigh;//高32位
	memcpy(&size,&large,sizeof(DWORD64));//将文件大小低位，高位拷贝到size上，计算文件大小
	tstring calculate;//保存容量大小的字符串
	capacity c;//保存容量大小的结构
	c.G = size / (1024 * 1024 * 1024);//求GB大小
	if (c.G)//大小GB级别
		calculate+=ToString(c.G)+TEXT("Gb,");
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
	hBm[1]= LoadBitmap(hInst, TEXT("IDB_BITMAP2"));//读取位图
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
	out << st.wYear << "年" << st.wMonth << "月" << st.wDay << "日," << st.wHour << ":" << st.wMinute << ":" << st.wSecond ;//格式化字符串
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
	HANDLE handle=FindFirstFile(wallpaper.c_str(),&fd);//查找第1个文件
	if (handle!= INVALID_HANDLE_VALUE)//找到了
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
void InitNotifyIconData(HWND hwnd,HINSTANCE hInst, NOTIFYICONDATA &nid)//初始化托盘结构
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
void PopupMyMenu(HWND hwnd,HMENU hMenu)//弹出菜单
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
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//主窗口 窗口过程
{
	PAINTSTRUCT ps;//画笔结构
	HDC hdc;//画笔
	TEXTMETRIC tm;//文本尺寸结构
	static int cxClient, cyClient;//客户区大小
	static int cxChar, cxCaps, cyChar;//字母高度、宽度
	RECT rect;//矩形
	static RECT invaliteRect;//矩形
	tstring wallpaper = TEXT("E:\\壁纸\\SogouWP\\Local\\WallPaper\\");//壁纸路径
	tstring format = TEXT("*.jpg");//壁纸后缀格式
	int size;//数组大小
	static SCROLLINFO si;//滚动条信息
	static int iVertPos, iHorzPos;//水平和垂直滚动条位置
	int start, end;//滚动条滚动时，重绘的起始和结束位置
	TCHAR szBuffer[1024];//字符缓冲
	static int iPerLine;//滚动一行的刻度
	static int iAccumLine ;//滚动的刻度
	static int ulScrollLines;//滚轴滚动一次滚动的行数
	static NOTIFYICONDATA nid;//系统需要的用来传递托盘区域消息的信息
	static POINT mousePos;//鼠标位置
	static int nSelectedLine;//当前被选中的行,索引从0开始
	static int nLastSelectedWallpapaer;//上一次被选中的壁纸
	static int nSelectWallpaper;//被选中的壁纸
	static int nPageLine;//一页的行数
	static HBRUSH oldBrush;//保存旧画刷
	static HBRUSH blueBrush;//蓝色画刷，选中时高亮显示的颜色
	static HINSTANCE hInst;
	static HMENU hMenu;//菜单句柄
	static WIN32_FILE_ATTRIBUTE_DATA attri;//文件属性结构
	static HFONT hFont;//字体
	static RECT scrollRect;//滚动条滚动的范围
	switch (UMsg)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;//程序实例句柄
		InitNotifyIconData(hwnd, hInst, nid);//初始化托盘图标结构
		Shell_NotifyIcon(NIM_ADD, &nid);//向托盘区域添加一个图标
		hMenu = CreateMyPopMenu(hInst);//创建菜单
		hdc = GetDC(hwnd);//得到窗口画笔
		GetTextMetrics(hdc, &tm);//得到画笔的文本尺寸
		cxChar = tm.tmAveCharWidth;//小写字母的平均宽度
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;//大写字母的平均宽度
		cyChar = tm.tmHeight + tm.tmExternalLeading;//字母的平均高度
		ReleaseDC(hwnd, hdc);
		FindFile(wallpaper + format);//查找所有图片
		SetTimer(hwnd, wallpaperID,wallPaperTime, nullptr);//设置壁纸定时器
		SetTimer(hwnd, tipID,tipTime, nullptr);//设置气泡提示定时器
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ulScrollLines, 0);//获取滚轴滚动一次滚动的行数
		if (ulScrollLines)
			iPerLine = WHEEL_DELTA / ulScrollLines;//计算滚动一行的刻度
		else
			iPerLine = 0;
		srand(time(0));//将当前时间作为随机函数种子
		blueBrush = CreateSolidBrush(RGB(0, 0, 255));//蓝色画刷
		hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);//默认字体
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
			//break;
		}
	}
		break;
	case WM_LBUTTONDBLCLK://鼠标双击
		//MessageBox(hwnd, TEXT(" double mouse"), 0, 0);
		break;
	case WM_LBUTTONDOWN://鼠标单击
		mousePos.x=GET_X_LPARAM(lParam);//计算鼠标当前所在的位置（横、纵坐标)
		mousePos.y=GET_Y_LPARAM(lParam);
		nSelectedLine = mousePos.y / cyChar;//计算鼠标当前被击中的行
		if (nSelectedLine >= 1 && nSelectedLine < nPageLine)//如果鼠标在客户区里并且不是第1行,是因为第1行是列名
		{
			nSelectWallpaper=nSelectedLine-1 +iVertPos;//nSelectedLine加上当前垂直滚动条的位置，即当前的选中的壁纸，-1因为索引从0开始
			//invaliteRect.top = (nLastSelectedWallpapaer-iVertPos)*cyChar;
			//invaliteRect.bottom = (nLastSelectedWallpapaer - iVertPos + 1)*cyChar;//当上一次选中行和下一行范围内的矩形要重绘，去掉高亮
			//InvalidateRect(hwnd, &invaliteRect, true);//将选中行进行重画，进行高亮显示
			//invaliteRect.top = nSelectedLine*cyChar;
			//invaliteRect.bottom = (nSelectedLine + 1)*cyChar;//当前行和下一行范围内的矩形要重绘
			InvalidateRect(hwnd, 0, true);//将选中行进行重画，进行高亮显示
			//nLastSelectedWallpapaer = nSelectWallpaper;//保存上一次选中的壁纸
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
			 if (allTitle.size() < 2&&size>0)//如果只有1个窗口即桌面Window Manager在桌面上显示并且图片数量不为0
			 {
				SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,(PVOID) (wallpaper+fileName[rand()%size]).c_str(), 0);//从图片中随机选1个，作为壁纸
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
	case WM_SIZE://大小改变后
		cxClient = LOWORD(lParam);//客户区的宽度、高度
		cyClient = HIWORD(lParam);
		nPageLine = cyClient / cyChar;//获得一页中行数
		si.cbSize = sizeof(si);//结构大小
		si.fMask = SIF_PAGE | SIF_RANGE;//设置的标记，只设置SIF_PAGE和SIF_RANGE，使用SIF_PAGE标志能够取得页面大小，SIF_RANGE标志时，必须把nMin和nMax字段设定为所需的卷动列范围
		si.nMin = 0;
		si.nMax = fileName.size();//最大范围的文字的个数,不是fileName.size()-1是因为要包括第1行的列名
		si.nPage=nPageLine;//一页的的行数
		SetScrollInfo(hwnd, SB_VERT, &si, true);//设置垂直滚动条参数
		si.nMax =222;//水平滚动的最大范围，一行最长文字的字符个数，大约估计
		si.nPage = cxClient / cxChar;//滚动一页列数
		SetScrollInfo(hwnd, SB_HORZ, &si, true);//设置水平滚动条参数
		scrollRect.left = 0;
		scrollRect.top = cyChar;
		scrollRect.right = cxClient;
		scrollRect.bottom = cyClient;//滚动条的滚动范围为客户区内除去第1行的矩形
		invaliteRect.left = 0;//高亮显示区域矩形的左右边为客户区宽度
		invaliteRect.right = cxClient;
		if (IsIconic(hwnd))//如果按下最小化按钮
			ShowWindow(hwnd, SW_HIDE);//隐藏窗口
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
			//ScrollWindow(hwnd, 0, cyChar*(iVertPos - si.nPos), &scrollRect, &scrollRect);//将窗口内容进行滚动iVertPos-si.nPos行，正为向下滚，负向上滚
			InvalidateRect(hwnd, 0, true);
		break;
	case WM_HSCROLL:
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;//获得滚动条所有信息
		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos= si.nPos;//当前水平滚动条的位置
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
			//ScrollWindow(hwnd, cxChar*(iHorzPos - si.nPos), 0, &scrollRect, &scrollRect);//将窗口内容进行滚动iVertPos-si.nPos行，正为向右滚，负向左滚
			InvalidateRect(hwnd,0, true);

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
		SelectObject(hdc,hFont);//设置字体
		if (fileName.size() > 0)
		{
			SetMapMode(hdc,MM_ANISOTROPIC);//设置映射模式，逻辑单位转换成具有任意比例轴的任意单位
			SetWindowExtEx(hdc, 1, 1, nullptr);//设置逻辑单位横坐标1个单位为物理坐标的一个字符宽度，纵坐标1个单位为物理坐标的一个字符高度
			SetViewportExtEx(hdc, cxChar, cyChar, nullptr);
			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;//获得滚动条所有信息
			GetScrollInfo(hwnd, SB_VERT, &si);//当前垂直滚动条的位置
			iVertPos = si.nPos;
			GetScrollInfo(hwnd, SB_HORZ, &si);//当前水平滚动条的位置
			iHorzPos = si.nPos;
			start = max(0, min(fileName.size(), iVertPos + ps.rcPaint.top/cyChar));//滚动条滚动后，ps.rcPaint.top/cyChar要重绘的起始行，加上iVertPos获得在滚动条上绘制的位置
			end = max(0, min(fileName.size(), iVertPos + ps.rcPaint.bottom/cyChar));//滚动条滚动后，ps.rcPaint.bottom/cyChar要重绘的结束行，加上iVertPos获得在滚动条上绘制的位置
			SetBkMode(hdc, TRANSPARENT);//设置背景模式为透明，这样文字和下划线才能在一起，显示下划线时才不会擦掉文字
			TextOut(hdc, -iHorzPos, 0, szTop, lstrlen(szTop));//显示列名
			TextOut(hdc, -iHorzPos, 0, szUnd, lstrlen(szUnd));//显示下划线
			SetBkMode(hdc, OPAQUE);//恢复不透明背景模式
			for (int i = start,j=0; i <end; i++,j++)//对滚动操作后的失效区域重绘
			{
				int x = -iHorzPos;//水平绘制的起始位置，如果3，表示第3列在开头，从-3开始绘制
				int y = i - iVertPos+1;//垂直绘制的起始位置,加上1是因为要不包括第1行的列名
				GetFileAttributesEx((wallpaper + fileName[i]).c_str(),GetFileExInfoStandard,&attri);//得到当前文件的属性,GetFileExInfoStandard表示表准文件属性，最后一个参数是 WIN32_FILE_ATTRIBUTE_DATA结构的指针
				fileCreateTime= FormatFileTime(attri.ftCreationTime);//文件创建时间
				fileLastAccessTime = FormatFileTime(attri.ftLastAccessTime); // 文件最后一次访问时间
				fileLastWriteTime = FormatFileTime(attri.ftLastWriteTime);//文件最后一次修改时间
				fileAttributes = FileAttribute(attri.dwFileAttributes);//文件属性
				filePath = wallpaper;//文件路径
				fileSize = calculateCapacity(attri.nFileSizeHigh, attri.nFileSizeLow);//计算文件大小
				StringCchPrintf(szBuffer,1024, TEXT("%3d%-5c%-15s%-20s%-15s%-30s%-30s%-30s%-30s"), i,':',fileName[i].c_str(),fileSize.c_str(),
					fileAttributes.c_str(),	fileCreateTime.c_str(),fileLastAccessTime.c_str(),fileLastWriteTime.c_str(),
					filePath.c_str());//格式化字符串
				if (i == nSelectWallpaper)//如果当前行被选中
				{
					rect.left = 0;//要显示高亮的矩形区域，鼠标选中区域
					rect.top = y;
					rect.right = cxClient / cxChar;
					rect.bottom = y+1;
					FillRect(hdc, &rect, blueBrush);//用蓝色高亮显示
					SetBkColor(hdc, RGB(0, 0, 255));//设置字符串的背景颜色为蓝色
					TextOut(hdc, x,y,szBuffer,lstrlen(szBuffer));//显示字符串
					SetBkColor(hdc, RGB(255,255, 255));//恢复原来的的字符串的背景颜色白色
				}
				else
				   TextOut(hdc, x,y,szBuffer,lstrlen(szBuffer));//显示字符串
			}
		}
		EndPaint(hwnd, &ps);
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
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("wallpaper"), WS_OVERLAPPEDWINDOW ,
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
