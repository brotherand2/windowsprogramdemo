#include"resource.h"
#include"tinyxml.h"
#include<ZYHFile.h>
#undef _STRSAFE_H_INCLUDED_
#include<ZYHGlobal.h>

//保存颜色
void saveColor(TiXmlElement *parent,COLORREF cr,string colorName)
{
	TiXmlElement *element=new TiXmlElement(colorName);
	element->SetIntAttribute("r", (int)(BYTE)cr);
	element->SetIntAttribute("g", (int)(BYTE)(cr >> 8));
	element->SetIntAttribute("b", (int)(BYTE)(cr >> 16));
	parent->LinkEndChild(element);
}
//添加新节点，并在节点上添加text
template <class T>
void addTextToNewElement(TiXmlElement *parent,string elementName, T value)
{
	TiXmlElement *subElement;
	(subElement = new TiXmlElement(elementName))->LinkEndChild(new TiXmlText(value));
	parent->LinkEndChild(subElement);
}
//保存目录
void saveDirectory(TiXmlElement *element,string name, vector<tstring> &directory)
{
	if(element)
	{
		for (size_t i = 1; i <= directory.size(); i++)
		{
#ifdef UNICODE
			addTextToNewElement(element, name + to_string(i), WCharToAnsi(directory[i-1]).c_str());
#else
			addTextToNewElement(element, name + to_string(i),directory[i-1]);
#endif
		}
	}
}
//将数据写到XML文件保存
void writeToXML(HWND hwnd)
{
	CreateArchiveDirectory(hwnd, TEXT("wallpaper"));//如果没有wallpaper目录则创建
	tstring save = tstring(szMyDocument) + TEXT("\\wallpaper\\wallpaper.xml");//存档文件全路径
	//创建一个XML的文档对象
#ifdef UNICODE
	TiXmlDocument myDocument(WCharToAnsi(save));
#else
	TiXmlDocument myDocument(save);
#endif
	TiXmlElement *element;
	//创建一个根元素并连接。
	TiXmlElement *RootElement=new TiXmlElement("Wallpaper");
	myDocument.LinkEndChild(new TiXmlDeclaration("xml 2.0", "ansi"));//添加声明
	myDocument.LinkEndChild(new TiXmlComment("曾勇华壁纸配置文件"));//添加注释
	myDocument.LinkEndChild(RootElement);//添加根节点

	element = new TiXmlElement("壁纸更换时间间隔");//保存壁纸时间结构
	element->SetIntAttribute("时", Wallpaper.hour);
	element->SetIntAttribute("分", Wallpaper.minute);
	element->SetIntAttribute("秒", Wallpaper.second);
	RootElement->LinkEndChild(element);

	element = new TiXmlElement("弹出气泡提示间隔");//保存气泡时间结构
	element->SetIntAttribute("时", Notify.hour);
	element->SetIntAttribute("分", Notify.minute);
	element->SetIntAttribute("秒", Notify.second);
	RootElement->LinkEndChild(element);

	element = new TiXmlElement("控制开关变量");//保存控制开关变量
	element->LinkEndChild(new TiXmlComment("控制开关变量,单词yes,true或1表示开启，否则是关闭该功能"));
	addTextToNewElement(element, "随机换壁纸", randomChange);
	addTextToNewElement(element,"开机程序自启动" , startingUpBoot);
	addTextToNewElement(element,"桌面上没有任何窗口才更换壁纸" , changeByOnlyDestop);
	addTextToNewElement(element, "查找图片时包含子目录", includeSubDirectory);
	addTextToNewElement(element, "是否开始换", startChange);
	RootElement->LinkEndChild(element);


	element = new TiXmlElement("气泡信息");//保存气泡信息
#ifdef UNICODE
	addTextToNewElement(element,"气泡提示",WCharToAnsi( szTip));
	addTextToNewElement(element, "气泡标题", WCharToAnsi(szInfoTitle) );
	addTextToNewElement(element, "气泡信息", WCharToAnsi(szInfo));
#else
	addTextToNewElement(element, "气泡提示", szTip);
	addTextToNewElement(element, "气泡标题", szInfoTitle);
	addTextToNewElement(element, "气泡信息", szInfo);
#endif
	RootElement->LinkEndChild(element);


	element = new TiXmlElement("窗口大小位置");//大小位置
	if(IsWindowVisible(hwnd))
	{//获取最新窗口大小信息
		GetWindowRect(hwnd, &windowRect);//获得窗口大小信息
		windowRect.right -= windowRect.left;//将窗口位置保存为窗口大小
		windowRect.bottom -= windowRect.top;
	}
	element->SetIntAttribute("x", windowRect.left);
	element->SetIntAttribute("y", windowRect.top);
	element->SetIntAttribute("width", windowRect.right);
	element->SetIntAttribute("heigh", windowRect.bottom);
	RootElement->LinkEndChild(element);

	element = new TiXmlElement("当前壁纸索引");//当前壁纸索引
	element->SetIntAttribute("index", nSelectWallpaper);
	RootElement->LinkEndChild(element);

	element = new TiXmlElement("字体信息");//保存字体结构
	addTextToNewElement(element,"字符的平均宽度" ,lf.lfWidth );
	addTextToNewElement(element,"字体的粗细程度" , lf.lfWeight);
	addTextToNewElement(element,"字符的高度" , abs(lf.lfHeight));
#ifdef UNICODE
	addTextToNewElement(element,"字体名" ,WCharToAnsi(lf.lfFaceName));
#else
	addTextToNewElement(element, "字体名", lf.lfFaceName);
#endif
	addTextToNewElement(element,"使用斜体" ,(bool)lf.lfItalic );
	addTextToNewElement(element, "添加删除线", (bool)lf.lfStrikeOut);
	addTextToNewElement(element, "添加下划线", (bool)lf.lfUnderline);
	RootElement->LinkEndChild(element);

	saveColor(RootElement, textColor, "字体颜色");//保存颜色
	saveColor(RootElement, textBkColor, "字体背景颜色");
	element = new TiXmlElement("自定义颜色");
	for (size_t i = 1; i <=16; i++)
		saveColor(element, color[i], string("自定义颜色") + to_string(i));
	RootElement->LinkEndChild(element);

	//保存目录
	element = new TiXmlElement("当前壁纸目录");//当前壁纸目录
	saveDirectory(element, "目录", currentDirectorys);
	RootElement->LinkEndChild(element);
	element = new TiXmlElement("备份壁纸目录");//备份壁纸目录
	saveDirectory(element, "目录", backupDirectorys);
	RootElement->LinkEndChild(element);
	myDocument.SaveFile();
}

//读取时间
void readTime(Time &time,TiXmlElement *element)
{
	if (element)
	{
		int temp = 0;
		element->Attribute("时", &temp);
		time.hour = makeInRange(0, temp, 100);
		element->Attribute("分", &temp);
		time.minute = makeInRange(0, temp,65535);
		element->Attribute("秒", &temp);
		time.second = makeInRange(0, temp,65535);
	}
}
//读取颜色
void readColor(TiXmlElement *element,COLORREF &cr)
{
	int r, g, b;
	if(element)
	{
		element->Attribute("r", &r);
		element->Attribute("g", &g);
		element->Attribute("b", &b);
		r = makeInRange(0, r, 255);
		b = makeInRange(0, b, 255);
		g = makeInRange(0, g, 255);
		cr = RGB(r, g, b);
	}
}
//获取文本
void GetText(TiXmlElement *element,TCHAR *save,int length,char *name)
{
	TiXmlElement *subElement = element->FirstChildElement(name);
	const char *text =subElement->GetText();
	if (subElement&&text)
	{
#ifdef UNICODE
		StringCchCopy(save, length,AnsiToWChar(text).c_str());
#else
		StringCchCopy(save, length,text);
#endif
	}
}
//获取int文本
void GetTextFromInt(TiXmlElement *element, int &save, char *name)
{
	TiXmlHandle handle(element);
	TiXmlText *text = handle.FirstChildElement(name).FirstChild().ToText();
	if(text)
		  save=text->IntValue();
}
//获取bool文本
void GetTextFromBool(TiXmlElement *element, bool &save, char *name)
{
	TiXmlHandle handle(element);
	TiXmlText *text = handle.FirstChildElement(name).FirstChild().ToText();
	if (text)
			save = text->BoolValue();
}
//读取目录
void readDirectory(TiXmlElement *element,vector<tstring>&directory)
{
	if(element)
	{
		for (TiXmlElement *sub = element->FirstChildElement(); sub;sub=sub->NextSiblingElement())
		{
			const char *text = sub->GetText();
			if(text)
			{
#ifdef UNICODE
				directory.push_back(AnsiToWChar(text));
#else 
				directory.push_back(text);
#endif
			}
		}
	}
}
//从xml读取存档
bool readFromXML(HWND hwnd)
{
	if (!CreateArchiveDirectory(hwnd,TEXT("wallpaper")))//如果没有目录创建目录
		return false;//目录中无存档退出
	tstring save = tstring(szMyDocument) + TEXT("\\wallpaper\\wallpaper.xml");//存档文件全路径
#ifdef UNICODE
	TiXmlDocument myDocument(WCharToAnsi(save));
#else
	TiXmlDocument myDocument(save);
#endif
	if(!checkFileExists(save.c_str()))
		return false;
	bool loadOkay = myDocument.LoadFile();
	if (!loadOkay)
		return false;
	TiXmlElement *rootElement=0, *element=0,*subElement=0;
	rootElement=myDocument.FirstChildElement("Wallpaper");
	if (!rootElement)
		return false;
	element = rootElement->FirstChildElement("壁纸更换时间间隔");
	readTime(Wallpaper, element);//读取壁纸更换时间间隔
	element = rootElement->FirstChildElement("弹出气泡提示间隔");
	readTime(Notify, element);//读取弹出气泡提示间隔

	//读取控制开关变量
	element = rootElement->FirstChildElement("控制开关变量");
	if(element)
	{
		GetTextFromBool(element,randomChange , "随机换壁纸");
		GetTextFromBool(element,startingUpBoot ,"开机程序自启动" );
		GetTextFromBool(element,changeByOnlyDestop , "桌面上没有任何窗口才更换壁纸");
		GetTextFromBool(element, includeSubDirectory,"查找图片时包含子目录" );
		GetTextFromBool(element, startChange,"是否开始换" );
	}

	//读取气泡信息
	element = rootElement->FirstChildElement("气泡信息");
	if(element)
	{
		GetText(element, szTip,128, "气泡提示");
		GetText(element, szInfo,256, "气泡信息");
		GetText(element, szInfoTitle,64, "气泡标题");
	}

	//读取窗口大小位置
	element = rootElement->FirstChildElement("窗口大小位置");
	if(element)
	{
		int temp;
		element->Attribute("x", &temp);
		windowRect.left = makeInRange(0, temp, cxScreen);
		element->Attribute("y", &temp);
		windowRect.top = makeInRange(0, temp, cyScreen);
		element->Attribute("width", &temp);
		windowRect.right = makeInRange(0, temp, cxScreen);
		element->Attribute("heigh", &temp);
		windowRect.bottom = makeInRange(0, temp, cyScreen);
	}

	//读取当前壁纸索引
	element = rootElement->FirstChildElement("当前壁纸索引");
	if (element)
	{
		int temp;
		element->Attribute("index", &temp);
		nSelectWallpaper = makeInRange(1, temp,65535);
	}
	//读取字体信息
	element = rootElement->FirstChildElement("字体信息");
	if (element)
	{
		int temp;
		GetTextFromInt(element,temp, "字符的平均宽度");
		lf.lfWidth = makeInRange(0, temp, 100);
		GetTextFromInt(element, temp, "字符的高度");
		lf.lfHeight = makeInRange(-100, temp, 100);
		GetTextFromInt(element, temp, "字体的粗细程度");
		lf.lfWeight = makeInRange(0, temp, 1000);
		GetText(element, lf.lfFaceName,LF_FACESIZE, "字体名");
		bool tempBool;
		GetTextFromBool(element, tempBool, "使用斜体");
		lf.lfItalic = tempBool;
		GetTextFromBool(element, tempBool, "添加删除线");
		lf.lfStrikeOut = tempBool;
		GetTextFromBool(element, tempBool, "添加下划线");
		lf.lfUnderline = tempBool;
	}

	//读取颜色信息
	element = rootElement->FirstChildElement("字体颜色");
	readColor(element, textColor);
	element = rootElement->FirstChildElement("字体背景颜色");
	readColor(element, textBkColor);
	element = rootElement->FirstChildElement("自定义颜色");
	if(element)
	{
		for (int i = 1; i <= 16;i++)
		{
			string name = string("自定义颜色") + to_string(i);
			subElement = element->FirstChildElement(name);
			readColor(subElement, color[i]);
		}
	}

	//读取目录
	element = rootElement->FirstChildElement("当前壁纸目录");
	readDirectory(element, currentDirectorys);
	element = rootElement->FirstChildElement("备份壁纸目录");
	readDirectory(element, backupDirectorys);
	return true;
}
//创建弹出菜单
HMENU CreateMyPopMenu(HINSTANCE hInst)
{
	HMENU hMenu = CreatePopupMenu();//创建弹出菜单
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
	AppendMenu(hMenu, MF_STRING, IDM_TOOLPRINT, TEXT("截图\tCtrl+Alt+P"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("退出\tAlt+F4"));
	CheckMenuItem(hMenu, IDM_MINI, changeByOnlyDestop ? MF_CHECKED : MF_UNCHECKED);//设置选择状态
	SetMenuItemPhoto(hInst, hMenu,TEXT("IDB_BITMAP1") , IDM_SHOWWALLPAPER);
	SetMenuItemPhoto(hInst, hMenu,TEXT("IDB_BITMAP2") , IDM_NEXTWALLPAPER);
	SetMenuItemPhoto(hInst, hMenu, TEXT("IDB_BITMAP3") ,IDM_PREWALLPAPER );
	SetMenuItemPhoto(hInst, hMenu, TEXT("IDB_BITMAP4"), IDM_PAUSE);
	SetMenuItemPhoto(hInst, hMenu, TEXT("IDB_BITMAP5") ,IDM_EXIT );
	SetMenuItemPhoto(hInst, hMenu, TEXT("IDB_BITMAP6"),IDM_RANDOM );
	SetMenuItemPhoto(hInst, hMenu,TEXT("IDB_BITMAP7") ,IDM_MINI );
	SetMenuItemPhoto(hInst, hMenu,TEXT("IDB_BITMAP8") , IDM_SET);
	SetMenuItemPhoto(hInst, hMenu, TEXT("IDB_BITMAP9") ,IDM_TOOLPRINT );
	return hMenu;
}

void FindFile(tstring CurrentPath,tstring type)//查找文件
{
	WIN32_FIND_DATA fd;
	tstring find;//当前要查找文件格式的路径
	find=CurrentPath +type;//type指定文件夹要查找的类型，如d:\\wallpaper\\*.jpg
	HANDLE handle = FindFirstFile(find.c_str(), &fd);//查找第1个文件
	if (handle != INVALID_HANDLE_VALUE)//找到了
	{
		do
		{
			tempDirectory[fd.cFileName] = CurrentPath;//将当前图片名字与路径插入map中保存
		} while (FindNextFile(handle, &fd));//查找下一个文件
		FindClose(handle);//关闭文件句柄
	}
}
void FindAllFile(tstring CurrentPath, bool includeSubDirectory)//查找所有壁纸
{//CurrentPath当前路径,includeSubDirectory为true则显示当前目录、子目录文壁纸，为false则只显示当前目录壁纸
	WIN32_FIND_DATA fd;//fd文件信息结构
	HANDLE handle;//handle文件名柄
	tstring find;//format指定文件夹要查找的类型，如d:\\wallpaper\\*.jpg
	for (size_t i = 0; i < format.size(); i++)//查找所有格式(jpg,bmp,png)的图片
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
					FindAllFile(CurrentPath+TEXT("\\")+tstring(fd.cFileName),includeSubDirectory);
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
	case IDM_TOOLPRINT://截图
		SendMessage(hwnd, WM_COMMAND, IDM_TOOLPRINT, 0);
		break;
	}
}
//对当前的壁纸进行高亮显示
void HighLineWallPaper(HWND hEdit, int  nSelectWallpaper)
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
//显示文件属性
void DisplayDistribitue()
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
	for (size_t i = 0; i<AllDirectorys.size(); i++)//显示所有目录的信息
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
	statusInfo[0] = tstring(TEXT("当前壁纸:")) + szPath + TEXT("\\") + szName;
	SendMessage(hStatus, SB_SETTEXT, 0, (long)statusInfo[0].c_str());//第一栏显示路径	
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
	static size_t lineCount;//编辑框总行数
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
			writeToXML(hwnd);
		}
		else
			MessageBox(hwnd, TEXT("请添加新目录,不要重复添加已存在目录"),TEXT("该目录已存在:"), MB_ICONWARNING | MB_OK);
		if (SHGetMalloc(&pMalloc) == S_OK)//创建IMALLOC对象
		{
			pMalloc->Free(plDlist);//释放plDlist指向的空间
			pMalloc->Release();//释放pMalloc的空间
		}
	}
}
UINT calculateTime(const Time &time)//计算时间，以毫秒计
{
	unsigned millisecond;//毫秒
	millisecond = time.second * 1000 + time.minute * 60 * 1000 + time.hour * 60 * 60 * 1000;//计算时间，以毫秒计
	return millisecond;
}
//重写编辑控件窗口过程
int _stdcall EditNumberProc(HWND hEdit, unsigned message, unsigned wParam, long lPram)
{
	LPWSTR pszText = L"你只能在此处键入数字";
	LPWSTR pszTitle = L"不能接受的字符";
	switch (message)
	{
	case WM_CHAR://按下按键
		if (!(wParam <= '9'&&wParam >= '0') && wParam != VK_BACK)//除数字和回退键的所有按键过滤
		    showBallonTip(hEdit, pszText, pszTitle);
		break;
	}
	return CallWindowProc(oldEditTextProc, hEdit, message, wParam, lPram);//调用默认的编辑控件窗口处理过程处理其它消息和未处理完的操作
}
//设置壁纸对话框窗口过程
int _stdcall DlgWallpaperSet(HWND hDlg, unsigned int message, unsigned wParam, long lParam)
{
	int wmID = LOWORD(wParam);//控件ID
	static HWND hWallPaperHour, hWallPaperMinute, hWallPaperSecond;//壁纸更换时、分、秒编辑框句柄
	static HWND hNotifyHour, hNotifyMinute, hNotifySecond;//气泡提示时、分、秒编辑框句柄
	static HWND hNotifyTip, hNotifyTitle, hNotifyContent;//气泡提示、标题、内容编辑框
	static HWND hRadioRandom, hRadioSequence;//随机和和顺序单选按钮句柄
	static HWND hCheckBoxMini, hCheckBoxSub,hCheckBoxAutoBoot;//所有窗口最小化、子目录和开机自动启动check box按钮句柄
	static HWND hwnd;//父窗口
	static HWND hListFilterWindow, hListCurrentWindow;//列表框
	static HINSTANCE hInst;//实例句柄
	TCHAR szBuffer[MAX_PATH];//缓冲区
	bool temp;
	int count;//列表框内容个数
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
		hNotifyContent = GetDlgItem(hDlg, IDC_EDITCONTENT);
		hRadioRandom = GetDlgItem(hDlg, IDC_RANDOM);
		hRadioSequence = GetDlgItem(hDlg, IDC_SEQUENCE);
		hCheckBoxMini = GetDlgItem(hDlg, IDC_CHANGEMINI);
		hCheckBoxSub = GetDlgItem(hDlg, IDC_FINDSUBDIRECTORY);
		hCheckBoxAutoBoot = GetDlgItem(hDlg,IDC_AUTOBOOT);
		hListFilterWindow=GetDlgItem(hDlg, IDC_FILTERWINDOW);
		hListCurrentWindow = GetDlgItem(hDlg, IDC_CURRENTWINDOW);
		hwnd = GetParent(hDlg);
		hInst =(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE);
		//在各个控件上显示当前属性
		SetDlgItemInt(hDlg, IDC_EDITHOUR, Wallpaper.hour, false);
		SetDlgItemInt(hDlg, IDC_EDITMINUTE, Wallpaper.minute, false);
		SetDlgItemInt(hDlg, IDC_EDITSECOND, Wallpaper.second, false);
		SetDlgItemInt(hDlg, IDC_NOTIFYHOUR, Notify.hour, false);
		SetDlgItemInt(hDlg, IDC_NOTIFYMINUTE, Notify.minute, false);
		SetDlgItemInt(hDlg, IDC_NOTIFYSECOND, Notify.second, false);
		SetDlgItemText(hDlg, IDC_EDITTIP,szTip);
		SetDlgItemText(hDlg, IDC_EDITTITLE, szInfoTitle);
		SetDlgItemText(hDlg, IDC_EDITCONTENT, szInfo);
		SendMessage(hCheckBoxMini, BM_SETCHECK, changeByOnlyDestop, 0);//仅有桌面一个窗口才换
		SendMessage(hCheckBoxSub, BM_SETCHECK, includeSubDirectory, 0);//查找文件包含子目录
		SendMessage(hRadioRandom, BM_SETCHECK, randomChange, 0);//如果是随机换
		SendMessage(hCheckBoxAutoBoot,BM_SETCHECK,startingUpBoot, 0);
		oldEditTextProc = (WNDPROC)SetWindowLong(hWallPaperHour, GWL_WNDPROC, (long)EditNumberProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘消息
		//SetWindowLong(hWallPaperMinute, GWL_WNDPROC, (long)EditNumberProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘消息
		//SetWindowLong(hWallPaperSecond, GWL_WNDPROC, (long)EditNumberProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘消息
		//SetWindowLong(hNotifyHour, GWL_WNDPROC, (long)EditNumberProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘消息
		//SetWindowLong(hNotifyMinute, GWL_WNDPROC, (long)EditNumberProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘消息
		//SetWindowLong(hNotifySecond, GWL_WNDPROC, (long)EditNumberProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘消息
		SendMessage(hWallPaperHour, EM_LIMITTEXT, 3, 0);
		SendMessage(hWallPaperMinute, EM_LIMITTEXT, 3, 0);
		SendMessage(hNotifyHour, EM_LIMITTEXT, 3, 0);
		SendMessage(hWallPaperSecond, EM_LIMITTEXT, 3, 0);
		SendMessage(hNotifyMinute, EM_LIMITTEXT, 3, 0);
		SendMessage(hNotifySecond, EM_LIMITTEXT, 3, 0);
		SendMessage(hNotifyTip, EM_LIMITTEXT, 128, 0);
		SendMessage(hNotifyTitle, EM_LIMITTEXT,64, 0);
		SendMessage(hNotifyContent, EM_LIMITTEXT,256,0);
		for (int i = 0; i < filterWindowName.size(); i++)//显示过滤的窗口名
			SendMessage(hListFilterWindow, LB_INSERTSTRING, -1, (long)filterWindowName[i].c_str());
		break;
	case WM_COMMAND:
		switch (wmID)
		{
		case  IDC_EDITHOUR:
		case  IDC_EDITMINUTE:
		case  IDC_EDITSECOND:
		case  IDC_NOTIFYHOUR:
		case  IDC_NOTIFYMINUTE:
		case  IDC_NOTIFYSECOND:
			if (HIWORD(wParam) == EN_MAXTEXT)
				showBallonTip(GetDlgItem(hDlg, wmID),L"不能大于等于1000", L"友情提示:");
			break;
		case  IDC_EDITTIP:
		case  IDC_EDITTITLE:
		case  IDC_EDITCONTENT:
			if (HIWORD(wParam) == EN_MAXTEXT)
				showBallonTip(GetDlgItem(hDlg, wmID), L"最大字节数,标题64,提示128,内容256", L"内容太长:");
			break;

		case IDM_SETFINISH://设置完成
			temp = startingUpBoot;//保存自启动状态
			randomChange = SendMessage(hRadioRandom, BM_GETCHECK, 0, 0);//获得换壁纸模式，是否随机
			changeByOnlyDestop = SendMessage(hCheckBoxMini, BM_GETCHECK, 0, 0);//所有窗口最小化才更换壁纸选择状态
			includeSubDirectory = SendMessage(hCheckBoxSub, BM_GETCHECK, 0, 0);//查找包括子目录选择状态
			startingUpBoot = SendMessage(hCheckBoxAutoBoot, BM_GETCHECK, 0, 0);//查找开机自启动选择状态
			runningMiniShow = SendMessage(GetDlgItem(hDlg, IDC_STARTINGMINISHOW), BM_GETCHECK, 0, 0);//运行最小化托盘显示
			closeBubble = SendMessage(GetDlgItem(hDlg, IDC_SHUTDOWNBIBLE), BM_GETCHECK, 0, 0);//气泡功能关闭
			if (temp != startingUpBoot)//状态改变
				SetStartingUpBoot(TEXT("曾勇华壁纸"));//设置开机启动与否

			miniPhotoSize = GetDlgItemInt(hDlg, IDC_MINISIZE, 0, false);
			Wallpaper.hour = GetDlgItemInt(hDlg, IDC_EDITHOUR, 0, false);
			Wallpaper.minute = GetDlgItemInt(hDlg, IDC_EDITMINUTE, 0, false);
			Wallpaper.second = GetDlgItemInt(hDlg, IDC_EDITSECOND, 0, false);
			wallPaperTime = calculateTime(Wallpaper);//计算新壁纸更换时间，以毫秒计
			if (wallPaperTime>0)//如果不是暂停时间>0，则
			{
				KillTimer(hwnd, wallpaperID);//杀死换壁纸定时器
				if(startChange)
				SetTimer(hwnd, wallpaperID, wallPaperTime, 0);//开启换壁纸定时器
			}

			Notify.hour = GetDlgItemInt(hDlg, IDC_NOTIFYHOUR, 0, false);
			Notify.minute = GetDlgItemInt(hDlg, IDC_NOTIFYMINUTE, 0, false);
			Notify.second = GetDlgItemInt(hDlg, IDC_NOTIFYSECOND, 0, false);
			tipTime = calculateTime(Notify);//计算新壁纸更换时间，以毫秒计
			if (tipTime>0)//如果时间>0
			{
				KillTimer(hwnd,tipID);//杀死气泡定时器
				if(!closeBubble)
				SetTimer(hwnd,tipID,tipTime, 0);//开启气泡定时器
			}

			GetDlgItemText(hDlg, IDC_EDITTIP,szBuffer, MAX_PATH);//获得气泡提示字符串
			if (szBuffer[0] != '\0')//不空
				lstrcpyn(szTip, szBuffer, 128);
			GetDlgItemText(hDlg, IDC_EDITTITLE, szBuffer, MAX_PATH);//获得气泡标题字符串
			if (szBuffer[0] != '\0')//不空
				lstrcpyn(szInfoTitle, szBuffer, 64);
			GetDlgItemText(hDlg, IDC_EDITCONTENT, szBuffer, MAX_PATH);//获得气泡内容字符串
			if (szBuffer[0] != '\0')//不空
				lstrcpyn(szInfo, szBuffer, 256);
			if (!closeBubble)
			{
				InitNotifyIconData(hwnd, hInst, nid, IDI_ICON1);//初始化托盘图标结构
				Shell_NotifyIcon(NIM_MODIFY, &nid);//修改托盘区域的一个图标，再次显示气泡
			}
			filterWindowName.clear();
			count = SendMessage(hListFilterWindow, LB_GETCOUNT, 0, 0);//得到过滤窗口列表框中窗口名的个数
			for (int i = 0; i < count; i++)
			{
				SendMessage(hListFilterWindow, LB_GETTEXT, i, (long)szBuffer);//获得当前索引的窗口名
				filterWindowName.push_back(szBuffer);//将窗口名插入过滤窗口数组
			}

			writeToXML(hwnd);
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
//修改壁纸目录对话框窗口过程
int _stdcall DlgModifyDirectoryProc(HWND hDlg, unsigned int message, unsigned wParam, long lParam)
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
		for (size_t i = 0; i < backupDirectorys.size(); i++)//在左边列表框显示备份壁纸目录
			SendMessage(hListLeft, LB_INSERTSTRING, -1, (long)backupDirectorys[i].c_str());//将当前目录插入到列表框尾部
		for (size_t i = 0; i <currentDirectorys.size(); i++)//在右边列表框显示当前壁纸目录
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
				EnableWindow(hBTNOK, szDirectoryPath[0]);//如果字符串不为空让按钮可用
				break;
			}
			break;
		case IDC_BTNADDDIRECTORY://添加壁纸按钮
			plDlist = SHBrowseForFolder(&bi);//弹出选择目录对话框,路径保存的指针plDlist
			if (plDlist)//如果选择了一个目录
			{
				SHGetPathFromIDList(plDlist, szDirectoryPath);//将plDlist路径（结构体）转换成字符串路径	
				SetWindowText(hEditPath, szDirectoryPath);//在编辑框上显示路径
				if (SHGetMalloc(&pMalloc) == S_OK)//创建IMALLOC对象
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
			writeToXML(GetParent(hDlg));//将壁纸的信息保存到存档
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
//设置开机启动
void SetStartingUpBoot(TCHAR *value)
{
	HKEY m_regkey;//注册表句柄
	TCHAR filename[MAX_PATH];
	GetModuleFileName(NULL, filename, MAX_PATH);//返回程序的全路径
	TCHAR lpRun[] = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");//子键路径
	long iRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_WRITE, &m_regkey);//打开自启动注册表键
	if (iRet == ERROR_SUCCESS)//打开成功
	{
		if (startingUpBoot)//开机自启
		    RegSetValueEx(m_regkey,value, 0, REG_SZ, (const unsigned char *)filename, MAX_PATH);//将当前程序路径添加到注册表自启动中
		else
			RegDeleteValue(m_regkey,value);//删除注册表
	}
	RegCloseKey(m_regkey);//关闭注册表
}
void InitWindow(HWND hwnd,HINSTANCE hInst)//用读取到的数据初始化
{
	hBKBrush = CreateSolidBrush(textBkColor);//创建背景画刷轻灰色
	InitBrowseInfo(hwnd, bi);//初始化选择目录结构
	InitFont(hwnd, cf,lf);//初始化字体结构
	InitColor(hwnd, cc, color);//初始化颜色结构
	InitNotifyIconData(hwnd, hInst, nid, IDI_ICON1);//初始化托盘图标结构
	WM_FIND = RegisterWindowMessage(FINDMSGSTRING);//查找和替换对换框会发送消息给用FINDMSGSTRING注册的消息		
	if (!closeBubble)
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
	if (!closeBubble)
	SetTimer(hwnd, tipID, tipTime, nullptr);//设置气泡提示定时器
	SetTimer(hwnd, clockID, 1000, nullptr);//设置时钟定时器，每一秒更新时间一次
	SetTimer(hwnd, timingID,timingTime, nullptr);//设置定时器，每10分钟保存一次
	if (windowRect.right > 0 && windowRect.bottom > 0 && windowRect.left >= 0 && windowRect.top >= 0)//如果读取的宽度和高度不为0
		MoveWindow(hwnd, windowRect.left, windowRect.top, windowRect.right, windowRect.bottom, true);//移动窗口，显示上一次窗口启动的位置大小
}
void defaultInit(HWND hwnd,HINSTANCE hInst)//窗口默认初始化
{
	//初始化字体
	StringCchCopy(lf.lfFaceName, LF_FACESIZE, TEXT("宋体"));
	lf.lfHeight = 27;
	lf.lfWeight = 400;
	InitWindow(hwnd, hInst);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//主窗口 窗口过程
{
	PAINTSTRUCT ps;//画笔结构
	HDC hdc;//画笔
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
		if (readFromXML(hwnd))//读取用户设置,如果有存档，将默认设置更改为用户设置
			InitWindow(hwnd, hInst);//初始化窗口各种设置
		else
			defaultInit(hwnd, hInst);//默认初始化
		if (currentDirectorys.size()!= 0)//如果有壁纸路径
		{
			for (size_t i = 0; i < currentDirectorys.size(); i++)//查找所有目录的图片
			{
				tempDirectory.clear();//将上一次添加的目录，所有临时文件名、文件路径清除
				FindAllFile(currentDirectorys[i], includeSubDirectory);//查找当前目录的所有图片
				AllDirectorys.push_back(tempDirectory);//将当前目录的所有信息添加
			}
			DisplayDistribitue();//显示文件属性
		}
		break;
	case WM_QUERYENDSESSION://关机时保存
 		writeToXML(hwnd);//将壁纸的信息保存到存档        
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
				DeleteObject(hFont);//删除上一次的画笔
				hFont = CreateFontIndirect(&lf);//根据逻辑字体创建字体句柄
				SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);//对编辑框发送消息，设置新画笔
				InvalidateRect(hEdit, nullptr, true);//用新的字体重新显示
				writeToXML(hwnd);//存档
			}
			break;
		case IDM_FONTCOLOR://字体颜色
			if (ChooseColor(&cc))//该函数创建一个能使用户从中选择颜色的通用颜色对话框
			{
				textColor = cc.rgbResult;//选择的字体颜色
				InvalidateRect(hEdit, nullptr, true);//编辑框重绘
				writeToXML(hwnd);//存档
			};
			break;
		case IDM_BKCOLOR://背景颜色
			if (ChooseColor(&cc))//该函数创建一个能使用户从中选择颜色的通用颜色对话框
			{
				textBkColor = cc.rgbResult;//选择的字体背景颜色
				DeleteObject(hBKBrush);//删除旧画刷
				hBKBrush = CreateSolidBrush(cc.rgbResult);//用背景色，创建新画刷
				InvalidateRect(hEdit, nullptr, true);//编辑框重绘
				writeToXML(hwnd);//存档
			};
			break;
		case IDM_WALLPAPERSET:
			DialogBox(hInst, TEXT("IDD_DIALOGSET"),0, DlgWallpaperSet);
			break;
		case IDM_TOOLPRINT://截图
			if (!hotKeyClip)//如果不是热键引起的,这时主窗口还存在
			{
				ShowWindow(hwnd, SW_HIDE);//隐藏主窗口，显示桌面
				Sleep(200);//停住200毫秒让主窗口隐藏
			}
			if (hDlg)
				DestroyWindow(hDlg);//如果上一次窗口未关闭，进行销毁
			hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CLIPBITMAP), hwnd, DlgPrintProc);
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
			//如果现在是随机换模式，点击后顺序换，提示点击后能随机
			ModifyMenu(hMenu, IDM_RANDOM, MF_BYCOMMAND, IDM_RANDOM, randomChange ? TEXT("随机") : TEXT("顺序"));//提示点击后能随机
			randomChange = !randomChange;//顺序换标记
			break;
		case IDM_MINI://所有窗口最小化才更换壁纸菜单
			CheckMenuItem(hMenu, IDM_MINI, randomChange?MF_UNCHECKED:MF_CHECKED);//在所有窗口最小化才更换壁纸菜单上去掉勾
			changeByOnlyDestop = !changeByOnlyDestop;//所有窗口最小化才更换壁标记
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
			break;
		}
	}
		break;
	case WM_TIMER://定时器消息
		switch (wParam)
		{
		case clockID://更新时间
			statusInfo[1] =FormatTime().str();//对系统时间进行格式化
			SendMessage(hStatus, SB_SETTEXT, 1, (long)statusInfo[1].c_str());//对第二栏更新时间 
			break;
		case tipID://气泡提示定时到了
			if (!closeBubble)
			Shell_NotifyIcon(NIM_MODIFY, &nid);//修改托盘区域的一个图标，再次显示气泡
			break;
		case timingID://10分钟定时保存一次
			writeToXML(hwnd);
			break;
		case wallpaperID://换壁纸提示到了
			{//句柄泄露代码区
				//HWND hDestop = GetDesktopWindow();
				//HDC  hdcScr = GetDCEx(hDestop, 0, DCX_LOCKWINDOWUPDATE);//获得屏幕的画笔,即使屏幕锁住也能刷新
				//TextOut(hdcScr, 20, 400, temp.c_str(), temp.size());
			}
			if (wallPaperCount > 0)//图片数量不为0
			{
				if (changeByOnlyDestop)//如果仅有桌面这个窗口才换壁纸
				{   //之前放在case wallpaperID:后使用程序作了大量无用功
			        EnumWindows(EnumWindowProc, 0);//枚举所有屏幕上的顶层窗口，并将窗口句柄传送给应用程序定义的回调函数

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
		if (!closeBubble)
		Shell_NotifyIcon(NIM_DELETE, &nid);//删除托盘图标
		DestroyWindow(hwnd);//销毁窗口
		writeToXML(hwnd);//将壁纸的信息保存到存档
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

void ShowFullScreen(HWND hwnd, int cxScreen, int cyScreen)//让窗口全屏
{
	SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);//设置窗口样式为弹出式窗口，就是去掉系统菜单、标题栏、边框等，也可用WS_BORDER样式
	SetMenu(hwnd, 0);//去掉菜单
	SetWindowPos(hwnd, HWND_TOPMOST, -3, -3, cxScreen + 3, cyScreen + 3, SWP_SHOWWINDOW);//让窗口显示全屏，置顶，(0,0)会使对话框窗口不能在屏幕左上角-3才显示正常，可能是边框长度3
	bFullScreen = true;//是全屏
	SetClassLong(hwnd, GCLP_HCURSOR,(long)LoadCursor(nullptr,IDC_CROSS));//设置鼠标光标为十字箭头
}
void RecoveryWindow(HWND hwnd, HMENU hMenu)//恢复窗口原来的大小位置
{
	SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);//恢复窗口样式
	SetMenu(hwnd, hMenu);//添加菜单
	SetWindowPos(hwnd, HWND_NOTOPMOST, 122, 122, width + 140, heigh + 180, SWP_SHOWWINDOW);//设置窗口大小，为截图大小加上120，留60像素作为截图与边框间间距
	bFullScreen = false;//不是全屏
	SetClassLong(hwnd, GCLP_HCURSOR, (long)LoadCursor(nullptr,IDC_ARROW));//恢复鼠标光标箭头
}
HBITMAP CopyBitmap(const HBITMAP &hBitmap)//位图拷贝
{
	HBITMAP copyBitmap;
	HDC hdcSrc = CreateCompatibleDC(0);//获得内存画笔
	HDC hdcDst = CreateCompatibleDC(0);//获得内存画笔
	SelectObject(hdcSrc, hBitmap);//将位图选入内存画笔
	BITMAP bm;//位图结构
	GetObject(hBitmap, sizeof(BITMAP), &bm);//获得要拷贝的位图信息
	copyBitmap = CreateCompatibleBitmap(hdcSrc, bm.bmWidth, bm.bmHeight);//创建要拷贝的位图
	SelectObject(hdcDst, copyBitmap);//将位图选入内存画笔
	BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);//将位图拷贝到另一副位图
	DeleteObject(hdcSrc);
	DeleteObject(hdcDst);
	return copyBitmap;
}
int _stdcall DlgPrintProc(HWND hDlg, unsigned int UMsg, unsigned int wParam, long lParam)//截图窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	static HDC hdc;//画笔
	static HDC hdcMem;//内存画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HINSTANCE hInst;//窗口实例句柄
	static POINT ptBeg, ptEnd;//截图起点、终点
	static bool  bBlock = false;//是否开始画矩形块
	static bool bNewBlcok = false;//是否开始截图
	static HWND hDestop;//桌面句柄
	static HBITMAP hBitmap;//位图句柄
	static HBITMAP copyBitmap;//拷贝的位图，当进行复制、剪裁时，将截图拷贝一份到剪贴板
	static HBITMAP clipBitmap;//从剪贴板中获得的位图句柄
	static HBITMAP blockBitmap;//保存截图的位图
	static RECT windowRect;//窗口的坐标，当窗口全屏时保存原窗口
	static HBRUSH redBrush;//红色画刷，画截图矩形的边框的颜色
	static HMENU hMenu;//窗口菜单
	switch (UMsg)
	{
	case WM_INITDIALOG://窗口被创建
		hInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);//获得窗口实例句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		redBrush = CreateSolidBrush(RGB(255, 0, 0));//红色画刷，画截图矩形的边框的颜色
		hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENUCLIP));//加载菜单
		SetMenu(hDlg, hMenu);//设置菜单
		hDestop = GetDesktopWindow();//桌面句柄
		SendMessage(hDlg, WM_COMMAND, IDM_NEWCLIP, 0);//发送消息，截图
		break;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		if (bNewBlcok&&hBitmap)//如果开始截图并且全屏位图不为空
		{
			bBlock = true;//开始画矩形块
			SetCapture(hDlg);//捕捉鼠标
			ptBeg.x = LOWORD(lParam);//获得截图的起始坐标
			ptBeg.y = HIWORD(lParam);
			ptEnd = ptBeg;
			hdc = GetDC(hDlg);//获得窗口画笔
			hdcMem = CreateCompatibleDC(hdc);//获得的画笔
			SelectObject(hdcMem, hBitmap);//将屏幕位图选入画笔
			SetCursor(LoadCursor(0, IDC_CROSS));//将光标设为十字箭头，表示在截图中
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE://按空格进行全屏
			if (bFullScreen)//如果已是全屏
				break;//退出
			GetWindowRect(hDlg, &windowRect);
			if (hBitmap)//如果位图句柄不为空
			{
				DeleteObject(hBitmap);//销毁位图
				hBitmap = 0;
			}
			GetDestopBK(hDestop, hBitmap, cxScreen, cyScreen);//得到窗口位图
			ShowFullScreen(hDlg, cxScreen, cyScreen);//全屏显示
			InvalidateRect(hDlg, 0, true);//在窗口绘制桌面背景
			break;
		}
		break;
	case WM_MOUSEMOVE:
		ptEnd.x = (short)LOWORD(lParam);//终点坐标
		ptEnd.y = (short)HIWORD(lParam);
		if (ptBeg.x == ptEnd.x&&ptBeg.y == ptEnd.y)
			break;
		if (bBlock)//如果已经画矩形区域
		{
			BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY);//将先前画的矩形擦除	
			if (ptBeg.x < ptEnd.x)//将模坐标最小的作为矩形的起点
			{
				clipRect.left = ptBeg.x;
				clipRect.right = ptEnd.x;
			}
			else
			{
				clipRect.left = ptEnd.x;
				clipRect.right = ptBeg.x;
			}
			if (ptBeg.y < ptEnd.y)//将模坐标最小的作为矩形的终点
			{
				clipRect.top = ptBeg.y;
				clipRect.bottom = ptEnd.y;
			}
			else
			{
				clipRect.top = ptEnd.y;
				clipRect.bottom = ptBeg.y;
			}
		 FrameRect(hdc, &clipRect, redBrush);//用红色边框画出矩形区域
		}
		break;
	case WM_RBUTTONDOWN://鼠标右键，取消本次截图，重次截图
		bBlock = false;//还未划矩形块
		BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY);//将先前画的矩形擦除	
		SetCursor(LoadCursor(0, IDC_ARROW));//恢复鼠标箭头
		ReleaseDC(hDlg, hdc);
		DeleteDC(hdcMem);
		break;
	case WM_LBUTTONUP:
		if (bNewBlcok)//如果开始截图
		{
			ptEnd.x = (short)LOWORD(lParam);//终点坐标
			ptEnd.y = (short)HIWORD(lParam);
			if (ptBeg.x == ptEnd.x&&ptBeg.y == ptEnd.y)
			{
				bBlock = false;
				DeleteDC(hdcMem);
			}
		}
		if (bBlock)
		{
			bBlock = bNewBlcok = false;//截图结束
			width = abs(ptEnd.x - ptBeg.x);//截图的宽度，高度
			heigh = abs(ptEnd.y - ptBeg.y);
			RecoveryWindow(hDlg, hMenu);//恢复原来的窗口
			HDC hdcClip = CreateCompatibleDC(hdc);//创建剪切区域的画笔
			if (blockBitmap)//如果上一次的截图位图未删除
			{
				DeleteObject(blockBitmap);//删除
				blockBitmap = 0;
			}
			blockBitmap = CreateCompatibleBitmap(hdc, width, heigh);//创建兼容的画笔
			SelectObject(hdcClip, blockBitmap);//将截图位图放入
			BitBlt(hdcClip, 0, 0, width, heigh, hdcMem, clipRect.left, clipRect.top, SRCCOPY);//将截图绘制在截图位图
			ReleaseCapture();//释放鼠标的捕捉
			ReleaseDC(hDlg, hdc);
			DeleteDC(hdcMem);
			DeleteDC(hdcClip);
			hotKeyClip = false;//恢复初始状态
			ShowWindow(hDlg, SW_NORMAL);//显示窗口
			SetForegroundWindow(hDlg);//窗口置顶
			InvalidateRect(hDlg, 0, true);//在窗口区域显示截图
			if (!blockBitmap)
				break;//没有要拷贝的位图
			copyBitmap = CopyBitmap(blockBitmap);//拷贝位图
			OpenClipboard(hDlg);//打开剪贴板
			EmptyClipboard();//将剪贴板清空
			SetClipboardData(CF_BITMAP, copyBitmap);//将位图句柄放到剪贴板
			CloseClipboard();//关闭剪贴板
		}
		break;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		switch (wmId)
		{
		case IDM_NEWCLIP:
			bNewBlcok = true;//开始截图
			ShowWindow(hDlg, SW_HIDE);//隐藏窗口
			if (hBitmap)//如果位图不为空
			{
				DeleteObject(hBitmap);//将以前位图删除
				hBitmap = 0;
			}
			GetDestopBK(hDestop, hBitmap, cxScreen, cyScreen);//得到窗口位图
			GetWindowRect(hDlg, &windowRect);//获得窗口坐标，在截完图后恢复
			ShowFullScreen(hDlg, cxScreen, cyScreen);//让窗口全屏
			InvalidateRect(hDlg, 0, true);//显示整个屏幕截图
			break;
		case IDCANCEL://按下ESC键
			RecoveryWindow(hDlg, hMenu);
			break;
		}
		break;
	case WM_PAINT://有区域失效，发生重绘
	{
		hdc = BeginPaint(hDlg, &ps);//获得窗口画笔
		HBITMAP tempBitmap = 0;//昨时位图，保存要显示的位图
		tempBitmap = (bFullScreen) ? hBitmap : blockBitmap;//如果是全屏，用全屏位图hBitmap,否则用截图位图blockBitmap
		if (tempBitmap)//位图不空
		{
			hdcMem = CreateCompatibleDC(0);//获得与屏幕兼容的画笔
			BITMAP bm;//位图结构
			GetObject(tempBitmap, sizeof(BITMAP), &bm);//获得位图信息
			SelectObject(hdcMem, tempBitmap);//将位图选入画笔
			if (bFullScreen)//全屏
				BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);//在窗口上显示截图
			else
				BitBlt(hdc, 60, 60, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);//在窗口上显示截图
			DeleteObject(hdcMem);
		}
	    EndPaint(hDlg, &ps);//释放画笔资源
	}
	break;
	case WM_CLOSE://窗口关闭
		if (hBitmap)
			DeleteObject(hBitmap);//删除位图
		if (blockBitmap)//如果上一次的截图位图未删除
			DeleteObject(blockBitmap);//删除
		DeleteObject(redBrush);//删除画刷
		DestroyWindow(hDlg);//销毁窗口
		break;
	}
	return false;
}
// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	HWND hwnd;
	hwnd = FindWindow(TEXT("zeng"), TEXT("wallpaper"));//查找自己的窗口
	if (IsWindow(hwnd))
	{
		SetForegroundWindow(hwnd);//自己的窗口已运行，置前端
		ShowWindow(hwnd	, SW_SHOWNORMAL);
		return 0;
	}
	cxScreen = GetSystemMetrics(SM_CXSCREEN);//屏幕宽高
	cyScreen = GetSystemMetrics(SM_CYSCREEN);
	WNDCLASSEX wndclass;//窗口类
	wndclass.cbClsExtra = 0;//为类分配的额外空间，可以保存一些地址、变量
	wndclass.cbWndExtra = 0;//为窗口分配的额外空间，可以保存一些地址、变量
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//背景画刷
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//鼠标光标
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON3));//图标
	wndclass.hInstance = hInstance;//实例名柄，程序的PID，一个整数，根据这个整数，系统可以找到进程的地址
	wndclass.lpfnWndProc = WndProc;//窗口过程
	wndclass.lpszClassName = TEXT("zeng");//类名
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENUMAIN);//菜单
	wndclass.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;//哪些改变后要重画，设置水平和垂直改变后重绘, CS_DBLCLKS 接收鼠标双击消息
	wndclass.cbSize = sizeof(wndclass);//窗口大小 
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));//小图标
	if (!RegisterClassEx(&wndclass))//注册窗口类
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	hwnd = CreateWindow(TEXT("zeng"), TEXT("wallpaper"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);//创建窗口
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	ShowWindow(hwnd, runningMiniShow?SW_HIDE:SW_SHOWNORMAL);//显示或隐藏窗口 
	UpdateWindow(hwnd);//进行重绘
	if (currentDirectorys.size() == 0)//如果没有壁纸目录
	{
		MessageBox(hwnd, TEXT("请在【文件】->【添加壁纸目录】进行添加目录"), TEXT("没有壁纸目录"), MB_OK);
		SendMessage(hwnd, WM_COMMAND, ID_ADDWAPPER, 0);//发送消息添加目录
	}
	HACCEL hAccel = LoadAccelerators(hInstance, TEXT("WALLPAPER"));//从资源文件读取加速键句柄
	MSG msg;//消息结构
	RegisterHotKey(0, ID_HOTKEYSHOW,MOD_CONTROL|MOD_ALT, 'M');//定义系统热键（系统快捷键，即使在后台运行，也能收到快捷键消息),用来显示窗口
	RegisterHotKey(0, ID_HOTKEYCLIP, MOD_CONTROL | MOD_ALT, 'P');//截图热键
	while (GetMessage(&msg, nullptr, 0, 0))//从消息队列获取消息
	{
		if (msg.message == WM_HOTKEY)//如果是系统热键
		{
			switch (msg.wParam)
			{
			case ID_HOTKEYCLIP:
				hotKeyClip = true;//热键引起截图
				SendMessage(hwnd, WM_COMMAND,IDM_TOOLPRINT, 0);//发送消息，截图
				break;
			case ID_HOTKEYSHOW:
				SendMessage(hwnd, WM_COMMAND,IDM_SHOWWALLPAPER, 0);//发送消息，显示窗口
				break;
			}
		}
		if (!TranslateAccelerator(hwnd, hAccel, &msg))////TranslateAccelerator，函数功能：翻译加速键表。该函数处理菜单命令中的加速键
		{
			if (!hDlg || !IsDialogMessage(hDlg, &msg))
			{//如果截图对话框被销毁，或者不是对话框消息，进行消息处理
				TranslateMessage(&msg);//将一些按键消息转换成字符消息放在按键消息后面
				DispatchMessage(&msg);//分发消息，即调用窗口过程进行处理消息
			}
		}
	}
	return 0;
}
