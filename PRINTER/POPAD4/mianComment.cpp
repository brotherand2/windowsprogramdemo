/*
问题1：只能识别BOM（字节顺序标志）的编码
问题2：当文件比较大时，最小化是要经过10多秒才显示文字，打开文件速度明显很慢，比起EDITPLUS慢很多

现情况好多了，打开大文件时文字滚动有延时，记事本却不会
问题3：加速键冲突  问题已解决
*/
#include<Windows.h>
#include<string>
#include<sstream>
#include<memory>
#include<algorithm>
#include<commctrl.h>
#include<tchar.h>
#include<strsafe.h>
#include"resource.h"
int AskConfirmation()//程序退出时询问是否退出
{
	return	MessageBox(nullptr, TEXT("你真的要退出记事本吗?"), TEXT("Quit"), MB_ICONQUESTION|MB_YESNO);
}
int AskAboutSave(HWND hwnd,TCHAR szFileName[])//询问是否是保存文件
{
	TCHAR szBuffer[64 + MAX_PATH];//长度为提示长度+标题长度
	StringCchPrintf(szBuffer, 64 + MAX_PATH, TEXT("是否将更改保存到 %s?"),szFileName[0] ? szFileName : unTitled);//进行格式化字符串
	//如果标题为空，标题显示“无标题”	
	int iReturn =MessageBox(hwnd, szBuffer, szAppName, MB_YESNOCANCEL | MB_ICONQUESTION);//弹出是否要保存
	if (iReturn == IDYES)//如果要保存
	{
		if (!SendMessage(hwnd, WM_COMMAND, IDM_FILE_SAVE, 0))//进行另存为
			iReturn = IDCANCEL;//另存为对话框选择了了取消或关闭
	};
	return iReturn;
}
void OKMessage(HWND hwnd,TCHAR szMessage[], TCHAR szTitle[])//弹出一个OK提示框,内容为提示+标题
{
	TCHAR szBuffer[64 + MAX_PATH];//长度为提示长度+标题长度
	StringCchPrintf(szBuffer, 64 + MAX_PATH,szMessage, szTitle[0]?szTitle:unTitled);//进行格式化字符串
	//如果标题为空，标题显示“无标题”
	MessageBox(hwnd, szBuffer, szAppName, MB_OK | MB_ICONEXCLAMATION);//弹出消息框
}
void InitOpenFileName(OPENFILENAME &ofn,HWND hwnd)//初始化文件结构
{	
	static TCHAR szFilter[] = TEXT("文本文件(*.txt)\0*.txt\0编程文件(*.cpp;*.c;*.java)\0*.cpp;*.c;*.java\0所有文件(*.*)\0*.*\0\0");//过滤区字符串
	ofn.lStructSize = sizeof(ofn);//结构的大小
	ofn.hwndOwner = hwnd;//指向所有者对话框窗口的句柄。这个成员可以是任意有效窗口句柄，或如果对话框没有所有者它可以为NULL。
	ofn.hInstance = nullptr;
	ofn.lpstrFilter = szFilter;//指向一对以空字符结束的过滤字符串的一个缓冲。缓冲中的最后一个字符串必须以两个NULL字符结束
	ofn.lpstrCustomFilter = nullptr;//指向一个静态缓冲，它包含了一对以空字符结束的过滤器字符串，这个字符串是为了保留用户选择的过滤样式
	ofn.nMaxCustFilter = 0;
	//指定特意为lpstrCustomFilter准备的以TCHARs为单位的缓冲大小。对于ANSI版本，是字节的个数；对于Unicode版本，是字符的个数。这缓冲应该最小在40个字符长。如果lpstrCustomFilter成员是NULL或是指向NULL的字符串，这个成员被忽略
	ofn.nFilterIndex = 2;//显示文件类型的第2个索引，即编程文件
	//指定在文件类型控件中当前选择的过滤器的索引。缓冲指向被lpstrFilter包含的一对定义了的过滤器的字符串。过滤器的第一对字符串的索引值为1，第二对为2，等等
	ofn.lpstrFile = nullptr;//当GetOpenFileName或GetSaveFileName函数返回成功时，这个缓冲包含驱动器，路径，文件名，及所选择的文件的扩展名
	ofn.nMaxFile = MAX_PATH;//指定lpstrFile缓冲的大小
	ofn.lpstrFileTitle = nullptr;//指向接收选择的文件的文件名和扩展名的缓冲（不带路径信息）。这个成员可以是NULL
	ofn.nMaxFileTitle = MAX_PATH;//指定lpstrFileTitle缓冲的大小
	ofn.Flags = 0;//位标记的设置，你可以使用来初始化对话框
	ofn.lpstrInitialDir = TEXT("");//指向以空字符结束的字符串，可以在这个字符串中指定初始目录
	ofn.lpstrTitle = nullptr;//指向在对话框的标题栏中放置的字符串。如果这个成员是NULL，系统使用默认标题（另存为或打开)
	ofn.nFileOffset = 0;//返回指定从路径开始到通过lpstrFile指定的文件名字符串基于0的偏移，文件名第1个字母在路径中偏移
	ofn.nFileExtension = 0;//指定从路径开始到通过lpstrFile指定的文件名字符串中扩展名基于0的偏移
	ofn.lpstrDefExt =TEXT("");//指向包含默认扩展名的缓冲
	ofn.lCustData = 0;//指定应用程序定义的数据，这数据是能被lpfnHook成员识别的系统传到的钩子程序
	ofn.lpfnHook = 0;//指向一个钩子程序。除非Flags成员中包含OFN_ENABLEHOOK标记，要么这个成员将被忽略
	ofn.lpTemplateName = 0;//指向一个以空字符结束的字符串，字符串是对话框模板资源的名字，资源保存在能被hInstance成员识别的模块中
}
HWND FindDialog(HWND hwnd,TCHAR szFindText[])//查找结构
{
	static FINDREPLACE fr;//这个必须为静态的，因为调用FindText,查找对话框显示后FindText函数立即返回,FindDialog函数执行完后，释放局部变量fr,fr的地址的空间变销毁，所的对话框需要的结构fr销毁了，程序崩溃
	//初始化查找替换结构
	fr.Flags = FR_DOWN | FR_HIDEWHOLEWORD;//初始化查找替换对话框的标记
	//FR_DOWN初始化向下按钮被按下，FR_HIDEWHOLEWORD不显示全字匹配
	fr.hInstance = nullptr;
	fr.hwndOwner=hwnd;//拥有对话框的窗口句柄
	fr.lCustData = 0;//用户定义的数据
	fr.lpfnHook = 0;
	fr.lpstrFindWhat =szFindText;//要查找的字符串
	fr.lpstrReplaceWith = 0;//要替换的字符串
	fr.lpTemplateName = 0;
	fr.lStructSize = sizeof(fr);//结构大小
	fr.wFindWhatLen = MAX_PATH;//要查找的字符串szFindText的缓冲区空间
	fr.wReplaceWithLen = 0;//要替换的字符串szFindText的缓冲区空间
	pfr = &fr;//将pfr，指向fr
	return FindText(&fr);//进行查找
}
HWND ReplaceDialog(HWND hwnd, TCHAR szFindText[],TCHAR szReplaceText[])//替换结构
{
	static FINDREPLACE fr;//这个必须为静态的，因为调用ReplaceText,替换对话框显示后ReplaceText函数立即返回,ReplaceDialog函数执行完后，释放局部变量fr,fr的地址的空间变销毁，所的对话框需要的结构fr销毁了，程序崩溃
	//初始化查找替换结构
	fr.Flags = FR_DOWN | FR_HIDEWHOLEWORD;//初始化查找替换对话框的标记
	fr.hInstance = nullptr;
	fr.hwndOwner = hwnd;//拥有对话框的窗口句柄
	fr.lCustData = 0;//用户定义的数据
	fr.lpfnHook = 0;
	fr.lpstrFindWhat = szFindText;//要查找的字符串
	fr.lpstrReplaceWith =szReplaceText;//要替换的字符串
	fr.lpTemplateName = 0;
	fr.lStructSize = sizeof(fr);//结构大小
	fr.wFindWhatLen = MAX_PATH;//要查找的字符串szFindText的缓冲区空间
	fr.wReplaceWithLen = MAX_PATH;//要替换的字符串szFindText的缓冲区空间
	return ReplaceText(&fr);//进行查找
}
void GetTextType(unsigned char text[], TextCode &fileType)//得到文本编码类型
{

	if (text[0] ==0xff && text[1] == 0xfe)
		fileType =UNICODETEXT;//UNICODE编码
	else
	{
		if (text[0] == 0xfe && text[1] == 0xff)
			fileType = UNICODEBIGENDIAN;//UNICODEBIGENDIAN编码
		else
		{
			if (text[0] == 0xef && text[1] == 0xbb && text[2] == 0xbf)
				fileType = UTF8;//UTF-8编码
			else
				fileType = ANSI;//ANSI编码
		}
	}
}
void ShowCaption(HWND hwnd, TCHAR szTitle[])//显示打开的文件标题
{
	TCHAR szBuffer[2*MAX_PATH];//长度为文件名长度+标题长度
	StringCchPrintf(szBuffer,2*MAX_PATH,TEXT("%s - %s"), szTitle[0] ? szTitle : unTitled, szAppName);//进行格式化字符串
	//如果标题为空，标题显示“无标题”,否则显示文件名+记事本
	SetWindowText(hwnd, szBuffer);
}
bool MyReadFile(HWND hEdit,HWND hStatus, const TCHAR szFileName[])//读取文件内容
{
	HANDLE hFile;//文件句柄
	/*
	HANDLE CreateFile(
 LPCTSTR lpFileName,    // 指向文件名的指针 
 DWORD dwDesiredAccess,    // 访问模式（写 / 读） 
 DWORD dwShareMode,    // 共享模式 
 LPSECURITY_ATTRIBUTES lpSecurityAttributes, // 指向安全属性的指针 
 DWORD dwCreationDisposition,   // 如何创建 
 DWORD dwFlagsAndAttributes,   // 文件属性 
 HANDLE hTemplateFile    // 用于复制文件句柄 
);
	*/
	bool isConvert = false;//是否转换了字符串
	hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	//OPEN_EXISTING 文件必须已经存在,以共享读、打开已存在文件，正常属性进行读取文件
	if (hFile == INVALID_HANDLE_VALUE)//文件读取失败
		return false;
	int nFileLength = GetFileSize(hFile, nullptr);//返回文件的大小,字节,如果函数调用成功，则返回值为文件大小的低位双字
    unsigned char *pText=nullptr;//保存读取的字符串的字节，因为读的字节是0-FF范围，所有用无符号保存,若用char 则读取ansi字符无问题，因为ansi字符不超过128,但UNICODE字符值超过128，如255，unsigned char 
	//读取是255(FF)，用char 读取-1(FFFFFFFF)
	TCHAR *pConvert=nullptr;//保存转换后的字符串
	wchar_t *temp = nullptr;//作为交换用的临时宽字符串
	unsigned long  nRealSize;//读取文件真实读的大小
	pText =new unsigned char[nFileLength + 2];//分配多2个空间放置2个\0\0，因为可能是UNICODE编码，UNICODE字符串后面有2个\0\0
	shared_ptr<unsigned char>sp1(pText, [](unsigned char *p){delete []p; });//将分配的空间交智能指针管理	
	shared_ptr<TCHAR>sp2(pConvert, [](TCHAR *p){delete []p; });//将分配的空间交智能指针管理
	shared_ptr<wchar_t>sp3(temp,[](wchar_t *p){delete[]p; });//将分配的空间交智能指针管理
	ReadFile(hFile, pText, nFileLength, &nRealSize, nullptr);//读取文件内容
	pText[nFileLength] = pText[nFileLength + 1] = '\0';//最后2个字符放置\0
    TextCode  fileType;//文件编码类型
	GetTextType(pText, fileType);//得到文本编码类型
	int len;//转换后的字符个数
	unsigned char cc;//字符交换时用的临时字符
	switch (fileType)
	{
	case ANSI://ansi编码文件
#ifdef UNICODE//如果是UNICODE程序
		len=MultiByteToWideChar(CP_ACP, 0, (char *)pText, -1, nullptr, 0);//将多字节字符转换成宽字符需要的字符个数,包括空字符
		pConvert = new TCHAR[len];
		MultiByteToWideChar(CP_ACP,0,(char *)pText,-1,(wchar_t *)pConvert,nFileLength+1);//将多字节字符转换成宽字符
		isConvert = true;//转换了
#endif
		SendMessage(hStatus, SB_SETTEXT, 4,(long) TEXT("ANSI"));//在第四栏显示编码类型
		break;
	case UNICODETEXT://UNICODE编码文件
		SendMessage(hStatus, SB_SETTEXT, 4, (long)TEXT("UNICODE"));//在第四栏显示编码类型
		pText += 2;//跳过2个字符，即字符标记FF FE
		nFileLength-=2;//不包括FF FE这2个字符长度
#ifndef UNICODE//如果不是UNICODE程序
		len=WideCharToMultiByte(CP_ACP,0,(wchar_t *)pText,-1,nullptr,0,nullptr,nullptr);//获得要转换的字符个数，包括\0
		pConvert = new TCHAR[len];
		WideCharToMultiByte(CP_ACP,0,(wchar_t *)pText,-1,(char *)pConvert,len,nullptr,nullptr);
		isConvert = true;//转换了
#endif
		break;
	case UTF8://UTF8编码，本程序只对BOM(有字节标志)的UTF8文件转换才有效
		SendMessage(hStatus, SB_SETTEXT, 4, (long)TEXT("UTF-8"));//在第四栏显示编码类型
		//UTF8的转换要先转成UNICODE，因为MultiByteToWideChar可以将UTF8转UNICODE，如果是ANSI程序，再将UNICODE程序字符串转ANSI
		pText += 3;//跳过3个字符，即字符标记EF BB BF
		nFileLength -= 3;//不包括EF BB BF这3个字符长度
		len = MultiByteToWideChar(CP_UTF8, 0, (char *)pText, -1, nullptr, 0);//将UTF8字符转换成宽字符需要的字符个数,包括空字符
		temp = new wchar_t[len];
		MultiByteToWideChar(CP_UTF8, 0, (char *)pText, -1, (wchar_t *)temp, nFileLength + 1);//将UTF8字符转换成宽字符
		isConvert = true;//转换了
#ifdef UNICODE//如果是UNICODE程序
		pConvert = new TCHAR[len + 1];//不用进行转换多字节，直接拷贝
		lstrcpy((PTSTR)pConvert, (PTSTR)temp);
#else//否则,再将UNICODE字符串转成ANSI字符串
		len = WideCharToMultiByte(CP_ACP, 0, (wchar_t *)temp, -1, nullptr, 0, nullptr, nullptr);//获得要转换的字符个数，包括\0
		pConvert = new TCHAR[len];//分配len长度的空间
		WideCharToMultiByte(CP_ACP, 0, (wchar_t *)temp, -1, (char *)pConvert, len, nullptr, nullptr);//将宽字符转换成多字节字符
#endif
		break;
	case UNICODEBIGENDIAN:
		SendMessage(hStatus, SB_SETTEXT, 4, (long)TEXT("unicode big endian"));//在第四栏显示编码类型
		pText += 2;//跳过2个字符，即字符标记 FE FF
		nFileLength -= 2;//不包括 FE FF这2个字符长度
		for (int i = 0; i < nFileLength / 2; i++)//交每个双字的低字节与高字节交换
		{
			//之所以要进行交换是因为最高有效字节优先，如Macintosh处理器，字符'A',是0x0041,但在内存中高字节优先，所以是按0x00，0x41这样的顺序存储
			//而UNICODE是按最 低有效字节优先
			cc = pText[2 * i];
			pText[2*i]=pText[2*i+1];
			pText[2*i+1]=cc;
		}
#ifdef UNICODE//如果是UNICODE程序
#else//否则
		len = WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pText, -1, nullptr, 0, nullptr, nullptr);//获得要转换的字符个数，包括\0
		pConvert = new TCHAR[len];//分配len长度的空间
		WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pText, -1, (char *)pConvert, len, nullptr, nullptr);//将宽字符转换成多字节字符
		isConvert = true;//转换了
#endif
		break;
	}
	if (isConvert)//转换了，将转换后的字符串放进编辑框
		SetWindowText(hEdit,(LPCTSTR) pConvert);//LPCSTR根据是否UNICODE 转变成const char*或const wchar *
	else//将未转换的字符串放进编辑框
		SetWindowText(hEdit, (LPCTSTR)pText);//LPCSTR根据是否UNICODE 转变成const char*或const wchar *
	CloseHandle(hFile);//读取文件后关闭文件句柄
	return true;
}
bool SaveFile(HWND hEdit, TCHAR szFileName[])//保存文件
{
	HANDLE hFile;//文件句柄
	hFile = CreateFile(szFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);//以写的方式打开文件，若文件不存在，则创建新的
	//CREATE_ALWAYS 创建文件，会改写前一个文件
	if (hFile == INVALID_HANDLE_VALUE)//文件读取失败或创建失败
		return false;
	int nFileLength = GetWindowTextLength(hEdit);//返回编辑框字符个数
	TCHAR *pText=new TCHAR [nFileLength+1];//保存要写入的字符串,+1是因为\0
	if (!pText)//内存分配失败
	{
		CloseHandle(hFile);
		return false;
	}
	shared_ptr<TCHAR>sp1(pText, [](TCHAR *p){delete[]p; });//将分配的空间交智能指针管理	
	int ByteOrderMask = 0xFEFF;//字节顺序标记，写入文件的时候，低位在前，所以依次写入FF FE，是UNICODE标识
	unsigned long realWrite;//写入文件过程中实际写入的字节个数
#ifdef UNICODE//如果定义了UNICODE标识
	WriteFile(hFile, &ByteOrderMask, 2, &realWrite, nullptr);//将字节顺序标记写入文件开头，让编辑软件识别它是UNICODE文件，以UNICODE编码的方式去读
#endif
	GetWindowText(hEdit, pText, nFileLength+1);//返回编辑框字符串，包括\0
	WriteFile(hFile, pText, nFileLength*sizeof(TCHAR), &realWrite, nullptr);//写入文件
	if (nFileLength*sizeof(TCHAR) != realWrite)//写入的实际字符数与要写入的字符数不相等，说明写入失败
	{
		CloseHandle(hFile);//关闭文件句柄
		return false;
	}
	CloseHandle(hFile);//关闭文件句柄
	return true;
}
bool MyFindText(HWND hEdit,FINDREPLACE &fr,bool bDown)//查找文本
{
	int len = GetWindowTextLength(hEdit);//获得编辑框字符个数
	TCHAR *pText = new TCHAR[len + 1];//分配空间
	shared_ptr<TCHAR>sp1(pText, [](TCHAR *p){delete[]p; });//将分配的空间交智能指针管理	
	GetWindowText(hEdit, pText, len + 1);//获得编辑框字符
	int findLen = lstrlen(fr.lpstrFindWhat);//要查找的字符串长度
	int pos ;//查找到的位置
	SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos,(long)&endPos);//当前选中文字的开始位置、结束，若未选中则是当前光标相对于开头位置的偏移（字符个数）
	if (startPos !=endPos&&bDown)//开始结束位置不同，说明选中文字并且是向下查找
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
		auto p=search(pText+startPos,pText+len, fr.lpstrFindWhat , fr.lpstrFindWhat+findLen);//查找第一个匹配的字符串位置
		if (p == pText + len)//向下查找不到
			return false;
		pos = p - pText;
	}
	else
	{
		auto p = find_end(pText,pText + startPos, fr.lpstrFindWhat, fr.lpstrFindWhat + findLen);//查找从最后一个匹配的字符串位置
		if (p == pText + startPos)//向上查找不到
			return false;
		pos = p - pText;
	}
	SendMessage(hEdit, EM_SETSEL, pos, pos + findLen);//选中查找到的字符串
	SendMessage(hEdit, EM_SCROLLCARET, 0, 0);//把可见范围移置光标处，即滚动到选中的位置
	return true;
}
void InitFont(HWND hwnd,CHOOSEFONT &cf,LOGFONT &lf)//初始化字体结构
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
void InitColor(HWND hwnd,CHOOSECOLOR &cc,COLORREF color[])//初始化颜色结构
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
void InitFind(HWND hEdit,TCHAR szFindText[])//初始化查找对话框
{
	SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//当前选中文字的开始、结束位置，若未选中则是当前光标相对于开头位置的偏移（字符个数）			
	if (startPos != endPos)//如果开始和结束位置不相等说明选中了文字
	{
		int len = GetWindowTextLength(hEdit);//获得编辑框字符个数
		TCHAR *pText = new TCHAR[len + 1];//分配空间
		shared_ptr<TCHAR>sp1(pText, [](TCHAR *p){delete[]p; });//将分配的空间交智能指针管理	
		GetWindowText(hEdit, pText, len + 1);//获得编辑框字符
		tstring temp(pText + startPos, pText + endPos);//获得选中的字符串
		lstrcpy(szFindText, temp.c_str());//拷贝到数组中
	}
	if (hDlgModeLess)//如果上一个对话框未关闭
		SendMessage(hDlgModeLess, WM_CLOSE, 0, 0);//发送消息关闭上1个对话框
}
void DrawBorder(HWND hwnd,bool bShowStatus)//画条蓝色边框
{
	RECT rect;
	HDC hdc;
	HBRUSH hBlueBrush = CreateSolidBrush(RGB(102, 179, 255));//编辑框的淡蓝色边框用的画刷
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);//得到客户区矩形
	if (bShowStatus)//如果显示状态栏
	rect.bottom -= 26;//边框不包围状态栏，减去状态栏高度
	FrameRect(hdc, &rect, hBlueBrush);//画淡蓝色画刷边框
	ReleaseDC(hwnd, hdc);
	DeleteObject(hBlueBrush);//删除旧画刷
}
int _stdcall DlgHelpProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)//帮助对话框
{
	HWND hEdit;//编辑框
	tstring szFileName;//文件名的路径
	switch (message)
	{
	case WM_INITDIALOG://初始对话框
		hEdit = GetDlgItem(hDlg, ID_EDITHELP);//得到帮助编辑框句柄
		szFileName = tstring(szWorkSpacePath) + TEXT("\\help.txt");//获得帮助文件的完整路径
		MyReadFile(hEdit, 0,szFileName.c_str());//读取帮助文件的内容到编辑框
		SetFocus(hEdit);//除非程序在处理WM_INITDIALOG消息将输入焦点设置为某个控件并返回false,否则Windows会把输入焦点设置为对话框中第一个含有WS_TAB的控件
		//因为帮助对话框只有一个控件编辑框，对话框必需有一个TABSTOP属性控件，所以有即使设置TABSTOP属性为false，它还是有有TABSTOP属性，除非增加一个控件放在编辑框前面，或用SetFocus
		//让控件获得焦点，并返回false,否则默认在对话框显示时会获得输入焦点，因为是只读的编辑框，文字会全选，所以setfous去掉文字全选
		HideCaret(hEdit);//编辑框有光标，因为在资源中设置为只读，所以让光标隐藏才比较好，一定要有setfoucs后这语句才有效，因为初始化过程中编辑控还未获得焦点，所以设置
		//隐藏光标无效，setfocus让它获得焦点设置隐藏光标才有效
		return false;
	case WM_CLOSE:
		EndDialog(hDlg,0);//关闭时销毁窗口,不能用DestroyWindow(hDlg)因为它只会销毁窗口不会退出对话框的消息循环，并没有允许对话框应用程序把控制权返回系统，主窗口还是不能动，
		//而EndDialog则会则在窗口销毁后结束对话框消息队列，允许返回控件权给主窗口
		return true;
	}
	return false;
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
		SetWindowText(hEditGoto,toString(row).c_str());
		return true;//因为在对话框资源只对编辑框设置WS_TABSTOP，这里返回true，就会使编辑框获得输入焦点，文字会全选
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_GOTO:
			GetWindowText(hEditGoto, strNum, sizeof(strNum));
			row = strToInt(strNum);//将字符串转换成整数
			if (row > lineCount)
			{
				SetWindowText(hEditGoto, toString(lineCount).c_str());//在转到编辑框显示编辑的总行数
				SetFocus(hEditGoto);//让转到编辑框获得焦点，是为了让文全选，显眼提示总行数
				SendMessage(hEditGoto, EM_SETSEL, 0, -1);//转到编辑框文字全选
				MessageBox(hEditGoto, TEXT("记事本超过了总行数"), TEXT("记事本 -跳行"), 0);
				break;
			}
			lineIndex = SendMessage(hEdit, EM_LINEINDEX,row-1, 0);//得到指定行首字符在文本中的位置（以字节数表示），-1是因为索引从0开始
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
int _stdcall DlgPrintProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)//打印终止对话框窗口过程
{
	switch (message)
	{
	case WM_INITDIALOG://对话框初始化
		EnableMenuItem(GetSystemMenu(hDlg, false), SC_CLOSE, MF_GRAYED);//让对话框关闭按钮不可用
		SetWindowText(hDlg, TEXT("记事本打印终止对话框"));//设置标题
		return true;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)//如果用户取消打印
		{
			bUserAbort = true;//用户终止打印标记为true
			EnableWindow(GetParent(hDlg), true);//让主窗口恢复活动
			DestroyWindow(hDlg);//销毁打印对话框
			hDlgPrint = 0;//打印对话框句柄为空，不再发送打印对话框消息
		}
		return true;
	}
	return false;//如果没有处理，返回0,会调用默认对话框过程，非0则处理了，不调用默认的
}
int _stdcall AbortProc(HDC hPrinterDC, int iMode)//打印机终止过程
{
	MSG msg;//进入打印开始后，主窗口终止活动，之所以终止活动，是防止在打印过程再次按下打印菜单，造成冲突，主窗口不能处理消息，所以消息由可活动的对话来处理
	while (!bUserAbort&&PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))//如果用用户没有终止打印，并且消息队列有消息，取得消息并从消息队列取得的消息，进入循环体处理
	{
		if (!hDlgPrint || !IsDialogMessage(hDlgPrint, &msg))
		{//如果打印终止对话框被销毁，或者不是对话框消息，进行消息处理
			TranslateMessage(&msg);//将一些按键消息转换成字符消息放在按键消息后面
			DispatchMessage(&msg);//分发消息，即调用窗口过程进行处理消息
		}
	}
	return !bUserAbort;//如果用户没有取消打印，bUserAbout是false,取反返回true,表示继续打印下一页
}
int _stdcall EditTextProc(HWND hEdit, unsigned message, unsigned wParam, long lPram)//重写记事本编辑对话框中编辑控件对话框过程
{
	int first;//当前光标所在行，首字符在编辑中的位置
	switch (message)
	{
	case WM_KEYUP://当键盘松开和
	case WM_LBUTTONUP://当鼠标左键松开的时候，更新行列信息
		row = SendMessage(hEdit, EM_LINEFROMCHAR, -1, 0)+1;//获得指定位置所在的行号，-1表示当前光标所在位置，因为位置的索引从0开始，所以要加1
		statusInfo[2] = TEXT("行号:") + toString(row);//将行号转换成字符串
		SendMessage(hStatus, SB_SETTEXT, 2, (long)statusInfo[2].c_str());//对第二栏更新行号 
		first = SendMessage(hEdit, EM_LINEINDEX, -1, 0);////当前光标所在行，首字符在编辑中的位置
		SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//当前选中文字的开始、结束位置，若未选中则是当前光标相对于开头位置的偏移（字符个数）			
		col = startPos - first+1;//当前光标的位置-当前光标所在行第1个字符的位置为列数，，因为位置的索引从0开始，所以要加1
		statusInfo[3] = TEXT("列号:") + toString(col);//将列号转换成字符串
		SendMessage(hStatus, SB_SETTEXT, 3, (long)statusInfo[3].c_str());//对第三栏更新列号 
		break;
	}
	return CallWindowProc(oldEditTextProc, hEdit, message, wParam, lPram);//调用默认的编辑控件窗口处理过程处理其它消息和未处理完的操作
}
bool PrintFile(HINSTANCE hInst, HWND hwnd, HWND hEdit, TCHAR szTitle[])//打印文件
{
	DOCINFO di{ sizeof(DOCINFO)};//打印文档，设置打印机文档名
	TCHAR szWindowText[MAX_PATH];//保存主窗口标题
	GetWindowText(hwnd, szWindowText, MAX_PATH);//得到主窗口标题
	di.lpszDocName = szWindowText;//设置打印的文档名为
	PRINTDLG pd;//打印机结构
	bool bSuccess=true;//错误是否发出，初始化true是未发生
	bUserAbort =false;//初始化false是用户未终止
	memset(&pd, 0, sizeof(pd));//打印结构清零，因为里面很多字段用不到，设为0
	pd.Flags =PD_RETURNDC| PD_ALLPAGES | PD_COLLATE | PD_NOPAGENUMS|PD_NOSELECTION;
	//PD_ALLPAGES 指定“全部”单选钮在初始时被选中(缺省标志)
	//PD_COLLATE(0x10) 指定“自动分页”复选框在初始时被选中
	//PD_NOPAGENUMS(0x08) 使“页码范围”单选钮和开始 / 结束页码编辑控件失效(缺省标志即是不设置也会失效)，但不设置会隐藏自动分页的按钮，所以要设置
	//PD_RETURNDC(0x100) 使打印对话框返回与用户选择相匹配的DC到hDC成员，返回所选的打印机画笔
	//PD_NOSELECTION(0x04) 使“选定范围”单选钮失效(缺省标志)
	pd.hwndOwner = hwnd;//父窗口句柄(可为NULL)
	pd.lStructSize = sizeof(pd);//本结构大小(字节数)
	pd.nCopies = 1;//指定拷贝份数编辑控件的初始值
	/* 激活打印对话框
	如果用户点击 OK 按钮，返回值为非零值。
	如果用户取消或关闭 Print 或 PrinterDlg对话框或错误出现，返回值为零 */
	if (!PrintDlg(&pd))
		return true;//不打印，不是打印失败
	int lineCount = SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);//得到编辑框的行数
	if (!lineCount)
		return true;//无内容不用打印
	TEXTMETRIC tm;//文本尺寸结构
	GetTextMetrics(pd.hDC, &tm);//得到指定打印机的文本尺寸结构
	int cyChar = tm.tmHeight + tm.tmExternalLeading;//得到打印机上一个字符的高度
	int cxChar = tm.tmAveCharWidth;//得到打印机上一个字符的宽度
	int xPage = GetDeviceCaps(pd.hDC, VERTRES);//获得打印机在纸上可打印范围的高度
	int yPage = GetDeviceCaps(pd.hDC, HORZRES);//获得打印机在纸上可打印范围的宽度
	int iLinesPerPage = yPage / cyChar;//一页有多少行
	int iCharsPerLine = xPage / cxChar;//一行有多少个字符
	TCHAR *pStrBuffer = new TCHAR[iCharsPerLine + 1];//保留每行字符串的缓冲区
	shared_ptr<TCHAR>sp(pStrBuffer, [](TCHAR*p){delete[]p; });//将分配的内存交智能指针管理
	int Pages = lineCount / iLinesPerPage;//文本要被打印成多少页
	if (lineCount%iLinesPerPage)//如果不够除，有余，说明多出几行，也算一页
		Pages++;
	EnableWindow(hwnd, false);//进入打印开始后，主窗口终止活动，之所以终止活动，是防止在打印过程再次按下打印菜单又从头打印，跟已经在打印的造成冲突
	SetAbortProc(pd.hDC, AbortProc);//设置打印终止过程，每打印一页调用该函数查询是否取消打印
	hDlgPrint = CreateDialog(hInst, TEXT("IDD_DIALOGPRINTER"), hwnd, DlgPrintProc);//创建终止打印对话框（非模态)
	RECT rect;//输入文字的矩形区域
	if (StartDoc(pd.hDC, &di))//开始打印
	{
		//如果文档有3页且用户要求打印3份，逐份打印则是打印3次，每次各页打印一份，打印顺序1,2,3打印3次
		//不是逐份打印则是则是打印一次，每次各页打印3份，打印顺序1，1，1，2，2，2，3，3，3
		int times = pd.Flags&PD_COLLATE ? pd.nCopies : 1;//如果逐份打印，则打印的份数就是要打印的次数，否则只打印一次
		for (int i = 0; i < times; i++)//打印每一次
		{
			for (int iPage = 0; iPage < Pages; iPage++)//打印每一页
			{
				int copys = pd.Flags&PD_COLLATE ? 1 : pd.nCopies;//每一页打印的份数，如果逐份打印，分几次打，所以每次只打印一份，否则，每页一次性打印多份
				for (int iCopy = 0; iCopy < copys; iCopy++)//打印每页的每一份
				{
					if (StartPage(pd.hDC))//打印当前页
					{
						for (int iLine = 0; iLine < iLinesPerPage; iLine++)//打印页面上每一行
						{
							int iLineNum = iPage*iLinesPerPage + iLine;//计算该行是编辑上的第几行
							if (iLineNum >= lineCount)//因为最后一页可能有空白，所以到了编辑框最后一行
								break;//退出打印
							*(int *)pStrBuffer = iCharsPerLine;//
							/*iCharsPerLine 为打印机每行可打印字符数 .
							该语句是为后面发送 EM_GETLINE 消息获得指定行的字符串做准备 ,
							因为pstrBuffer 的第一个双字需要标明该缓冲区的大小才能知道一行最多能获得多少个字符，因为有
							可能有水平滚动条，所以一行的字符数可能超过可打印范围的宽度，超过范围的跳过*/
							int len=SendMessage(hEdit, EM_GETLINE, iLineNum, (long)pStrBuffer);//获得指定行的字符串，
							//因为拷贝一行的时候不包括终止字符\0，所以不能用lstrlen判断拷贝的个数，要用SendMessage返回的拷贝的字符个数
							rect.left = 0;//从最左边开始，矩形区域为每一行的矩形区域
							rect.top = iLine*cyChar;//高度
							rect.right = yPage;//打印机可打印范围的最右边
							rect.bottom = (iLine + 1)*cyChar;
							DrawText(pd.hDC, pStrBuffer, len, &rect, DT_EXPANDTABS);//打印每一行，注意不能用TextOut,因为不支持制表符换行符，空格与字符不等宽之类的问题
							//默认DrawText会把制表符输出|， DT_EXPANDTABS会将制表符输入7个空格
						}
						if (!EndPage(pd.hDC))//结束该页的打印
						{
							bSuccess = false;//结束失败，发生错误
							break;//打印失败，退出
						}
						if (bUserAbort)//如果用户终止打印
							break;//打印失败，退出							
					}
					else
					{
						bSuccess = false;//发生错误
						break;//打印失败，退出
					}
				}
				if (!bSuccess || bUserAbort)//如果打错过程发生错误或用户终止打印
					break;//打印失败，退出
			}
			if (!bSuccess || bUserAbort)//如果打错过程发生错误或用户终止打印
				break;//打印失败，退出
		}
	}
	else
		bSuccess = false;//发生错误
	if (bSuccess)//打印未发生错误
		EndDoc(pd.hDC);//结束打印
	if (hDlgPrint)//用户未按取消按钮
	{
		EnableWindow(hwnd, true);//让主窗口恢复活动
		DestroyWindow(hDlgPrint);//销毁终止打印对话框
	}
	DeleteDC(pd.hDC);
	return bSuccess&&!bUserAbort;//只有打印过程未发生错误并且用户未按终止按钮才是打印成功
}
void FormatTime()//对系统时间进行格式化
{
	SYSTEMTIME st;//时间结构
	GetLocalTime(&st);//得到本地时间
	tstringstream out;//用这个字符串流格式化字符串
	out << _T("现在时间:") << st.wYear << _T("年") << st.wMonth << _T("月") << st.wDay << _T("  ") << st.wHour <<
		_T("小时") << st.wMinute << _T("分") << st.wSecond << _T("秒");
	statusInfo[1] = out.str();//将流中的字符串保存在字符串中
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//窗口过程
{
	PAINTSTRUCT ps;//画笔结构，保留画笔作息
	HDC hdc;//画笔
	static int cxClient, cyClient;//客户区宽度、高度
	static int cxChar, cyChar;//字母宽度、高度
	int wmId, wmEvent;//控件ID,控件通知码，则操作
	static HWND hEdit;//编辑框
	static HINSTANCE hInst;//窗口实例句柄
	static TCHAR szFileName[MAX_PATH] = TEXT("");//保存文件路径名包括文件名
	//初始化文件名编辑控件使用的文件名的缓冲。如果不需要初始值，这个缓冲的第一个字符必须是NULL
	static TCHAR szTitle[MAX_PATH];//保存文件名不包括路径
	static OPENFILENAME ofn;//用来初始化打开或另存为对话框的信息
	static CHOOSEFONT cf;//字体结构
	static CHOOSECOLOR cc;//颜色结构
	static LOGFONT lf;//逻辑字体
	static HFONT hFont;//字体句柄
	static COLORREF color[16];//颜色对话框中保存16个自定义颜色
	static TCHAR szFindText[MAX_PATH];//要查找的字符串
	static TCHAR szReplaceText[MAX_PATH];//进行替换的字符串
	static unsigned int WM_FINDREPLACE;//自定义的消息，用于接收查找和替换的消息
	static bool bDown ;//向下按钮是否按下
	static bool bAutoLine=false;//是否自动换行
	static bool bShowStatus=true;//是否显示状态栏
	static HMENU hMenu;//菜单句柄
	static unsigned style;//滚动条窗口样式
	static HBRUSH hBrush=CreateSolidBrush(RGB(255,255,255));//默认编辑框背景画刷，白色
	static COLORREF textColor = RGB(0, 0, 0);//默认的编辑框字体颜色
	static COLORREF textBkColor = RGB(255,255,255);//默认的编辑框字体背景颜色
	LPCREATESTRUCT cs;//窗口结构的指针
	int xPos[5]{0, 0, 0,0,0};//状态栏的5个右横坐标
	TCHAR *pText;//保存编辑的字符串
	switch (UMsg)
	{
	case WM_CREATE://窗口被创建
		GetCurrentDirectory(MAX_PATH, szWorkSpacePath);//获得当前工作区间路径
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//获得窗口实例句柄
		hMenu = GetMenu(hwnd);//得到窗口句柄
		cxChar = LOWORD(GetDialogBaseUnits());//得到对话框中字母的宽度
		cyChar = HIWORD(GetDialogBaseUnits());//得到对话框中字母的高度
		hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|ES_AUTOHSCROLL|ES_MULTILINE|ES_NOHIDESEL, 0, 0, 0, 0,
			hwnd, (HMENU)ID_EDIT, hInst, nullptr);//创建有边界的多行编辑框，ES_NOHIDESEL在编辑框失去焦点后选中的文本依然保持高亮显示
		hStatus = CreateWindow(STATUSCLASSNAME, 0, WS_CHILD | WS_VISIBLE,
			0, 0,0,0, hwnd, (HMENU)IDS_STATE, hInst, nullptr);//创建状态栏
		InitOpenFileName(ofn,hwnd);//初始化文件结构
		InitFont(hwnd, cf, lf);//初始化字体结构
		InitColor(hwnd, cc, color);//初始化颜色结构
		ShowCaption(hwnd, szTitle);//显示无标题
		SendMessage(hEdit, EM_LIMITTEXT,0, 0);//当wParam为0时，多行编辑控伯最多0x7FFFFFFE个字符，默认是30000个字符
		WM_FINDREPLACE = RegisterWindowMessage(FINDMSGSTRING);//查找和替换对换框会发送消息给用FINDMSGSTRING注册的消息		
		hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0,0, NULL);//默认字体
		SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);
		cs = (LPCREATESTRUCT)lParam;//当窗口被创建时，会把窗口的各种作息保存在一个CREATESTRUCT结构中，创建窗口时会发送WM_CREATE消息，其中lParam保存着这个结构的地址
		//将命令行字符串拷贝到文件名数组
		lstrcpy(szFileName, (TCHAR*)(cs->lpCreateParams));//在调用CreateWinodow时，最后一个参数lpParam保留着命令行字符串szCmdLine,lpParam参数在CREATESTRUCT的 lpCreateParam字段中
		if (szFileName[0] != '\0')//如果字符串不为空，即运行时进行了传参，或者程序是通过拖拽一个文件在程序上打开
			MyReadFile(hEdit,hStatus, szFileName);//读取文件内容
		SetTimer(hwnd, ID_TIMER, 1000, nullptr);//计时，每一秒更新一次时间
		oldEditTextProc = (WNDPROC)SetWindowLong(hEdit, GWL_WNDPROC, (long)EditTextProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘、鼠标消息，因为
		//默认是捕获不到这些消息的，但要用的这些消息，比如当鼠标或按键按下时更新状态栏显示的行列信息
		//但是不能用SetClassLong(hEdit,GCL_WNDPROC),因为它是修改edit类的窗口过程，它只会对下一个创建的编辑框生效，对当前
		//编辑框不会改变，而且用SetClassLong,会影响以后的编辑框，而我只想改变当前编辑框，不想改变其它编辑框
		return 0;
	case WM_SETFOCUS://窗口获得焦点时
		SetFocus(hEdit);//让编辑框获得焦点
		return 0;
	case WM_DROPFILES://发生拖拽操作
		DragQueryFile((HDROP)wParam, 0, szFileName, MAX_PATH);//获得拖拽的文件名路径
		MyReadFile(hEdit,hStatus, szFileName);//根据路径读取文件内容
		return 0;
	case WM_SIZE://窗口大小改变时
		cxClient = LOWORD(lParam);//更新客户区的大小
		cyClient = HIWORD(lParam);
		xPos[4] =-1;//最后一个窗格右边界-1，表球到客户区最右边
		xPos[3] = cxClient - 100;
		xPos[2] = cxClient - 160;
		xPos[1] = cxClient - 250;
		xPos[0] = cxClient - 500;//第一个窗格右边界
		SendMessage(hStatus, SB_SETPARTS,5, (long)xPos);//根据5个横坐标成四栏
		if (bShowStatus)//如果显示状态栏，编辑框在状态栏上面
		    MoveWindow(hEdit,1,1, cxClient-4, cyClient-29, true);//-4是留出宽度画边框，-29是把边框画在状态栏上面，状态栏高度25，留4个宽度画边框
		else
			MoveWindow(hEdit, 1, 1, cxClient - 4, cyClient -4, true);//-4是留出宽度画边框，调整编辑大小为整个客户区
		MoveWindow(hStatus, 0,0, 0,0, true);//调整状态栏在客户区最下面，不用设置大小位置，因为大小位置固定不可变
		for (int i = 0; i <5; i++)
			SendMessage(hStatus, SB_SETTEXT, i, (long)statusInfo[i].c_str());//对每一栏显示文本
		return 0;
	case WM_INITMENUPOPUP://菜单弹出时的消息项初始化后才会弹出
		if (LOWORD(lParam) == 1)//如果索引为1，即是弹出编辑菜单
		{//wParam参数是弹出式菜单的句柄，lParam的低位是弹出式菜单的索引
			//如果该菜单是系统菜单，那么高位是1，否则为0
			EnableMenuItem((HMENU)wParam, IDM_EDIT_UNDO, SendMessage(hEdit, EM_CANUNDO, 0, 0) ? MF_ENABLED : MF_GRAYED);
			//先发送一条消息到编辑框，如果可撤销，则撤销菜单项可用，否则，不可用
			EnableMenuItem((HMENU)wParam, IDM_EDIT_PASTE, IsClipboardFormatAvailable(CF_TEXT) ? MF_ENABLED : MF_GRAYED);
			//如果剪贴板有文字，则剪切菜单可用，否则，置为灰色
			long isSelected = SendMessage(hEdit, EM_GETSEL, 0, 0);//获得当前被选文字的开始和结束位置
			int isEnable = (LOWORD(isSelected) == HIWORD(isSelected));//isSelected低位保存着开始位置，高位保存结束位置，如果开始结束位置相等说明未选中
			if (isEnable)//开始结束位置相等，说明未选中文字
				isEnable = MF_GRAYED;
			else
				isEnable = MF_ENABLED;
			EnableMenuItem((HMENU)wParam, IDM_EDIT_COPY, isEnable);//选中择变亮，否则变灰
			EnableMenuItem((HMENU)wParam, IDM_EDIT_CUT, isEnable);
			EnableMenuItem((HMENU)wParam, IDM_EDIT_DELETE, isEnable);
		}
		return 0;
	case WM_COMMAND://在子窗口、菜单上操作时产生命令
		wmId = LOWORD(wParam);//控件ID
		wmEvent = HIWORD(wParam);//控件通知码，则操作
		if (wmId == ID_EDIT)
		{
			switch (wmEvent)
			{
				case EN_UPDATE:
					bNeedSave = true;//编辑框更新了，需要保存
					break;
				case EN_ERRSPACE://如果编辑框字符超过0x7FFFFFFE个字符
				case EN_MAXTEXT:
					MessageBox(nullptr, TEXT("超出编辑控件的最大空间，最多0x7FFFFFFE个字符"), TEXT("Error"), MB_ICONSTOP);
					break;
			}
		}
		switch (wmId)
		{
		case IDM_FILE_NEW://新建文件
			if (bNeedSave&&IDCANCEL == AskAboutSave(hwnd, szFileName))
				return 0;//如果文本更改了，需要进行保存，但回答是不保存则不允许新建文件，不往下执行
			SetWindowText(hEdit, TEXT(""));//将编辑框清空
			szTitle[0] = '\0';//将标题置空,以显示标题为无标题
			szFileName[0] = '\0';//将文件路径清空，使保存的时候找不到文件，进行另存为操作
			ShowCaption(hwnd, szTitle);//显示无标题
			SendMessage(hStatus, SB_SETTEXT,0,(long)TEXT("新建文件"));//第一栏显示新建文件
			break;
		case IDM_FILE_OPEN://打开文件
			if (bNeedSave&&IDCANCEL == AskAboutSave(hwnd, szFileName))
				return 0;//如果文本更改了，需要进行保存，但回答是不保存则不允许打开新文件，不往下执行
			ofn.lpstrTitle = TEXT("亲，要选一个文件来打开");//指向在对话框的标题栏中放置的字符串。如果这个成员是NULL，系统使用默认标题（另存为或打开)
			ofn.Flags = OFN_HIDEREADONLY;//位标记的设置，你可以使用来初始化对话框
			//OFN_ALLOWMULTISELECT 指定文件名列表框允许多选,OFN_HIDEREADONLY 隐藏只读复选框。
			// OFN_CREATEPROMPT如果用户指定了一个不存在的文件，这个标记使用对话框能提示用户是否新建这个文件
			ofn.lpstrFile = szFileName;//当GetOpenFileName或GetSaveFileName函数返回成功时，这个缓冲包含驱动器，路径，文件名，及所选择的文件的扩展名
			ofn.lpstrFileTitle = szTitle;//指向接收选择的文件的文件名和扩展名的缓冲（不带路径信息）。这个成员可以是NULL
			if (GetOpenFileName(&ofn))//创建一个Open公共对话框，使用户指定驱动器、目录和文件名、或使用户打开文件,如果用户指定了一个文件名，点击OK按钮，返回值为非零
			{
				if (!MyReadFile(hEdit,hStatus, szFileName))//读取文件内容到编辑框
				{
					//读取失败
					OKMessage(hwnd, TEXT("%s\n找不到文件。\n请检查文件名是否正确，然后重试"), szTitle);
					szTitle[0] = '\0';//设置标题为空，因为下面要显示标题，如果不置为空，会显示上1个标题
				}
				else
				{
					bNeedSave = false;//打开新文件后不需要再保存，设为false
					statusInfo[0] = tstring(TEXT("打开文件:")) + szFileName;
					SendMessage(hStatus, SB_SETTEXT,0, (long)statusInfo[0].c_str());//对第一栏显示打开文件
				}
			}
			ShowCaption(hwnd, szTitle);//更新标题
			break;
		case IDM_FILE_SAVE://保存文件
			if (!SaveFile(hEdit, szFileName))//保存文件
			{//保存失败进行另存为，选择别的路径
				return SendMessage(hwnd, WM_COMMAND, MAKELONG(IDM_FILE_SAVEAS, 0), 0);//如果保存失败，说明文件是只读属性，不能写，发生一条另存为信息，转去另存为对话框，保存在另一个位置
			}
			else
			{
				statusInfo[0] = tstring(TEXT("保存文件:")) + szFileName;
				SendMessage(hStatus, SB_SETTEXT,0, (long)statusInfo[0].c_str());//对第一栏显示保存文件
			}
			bNeedSave = false;//保存完必后不需要再保存，设为false
			ShowCaption(hwnd, szTitle);//更新标题
			return 1;//表示保存成功，因为其它地方通过SendMessage()调用这个消息，返回1个值判断是否保存成功
		case IDM_FILE_SAVEAS://另存为
			ofn.lpstrTitle = TEXT("亲,这是另存为哟");//指向在对话框的标题栏中放置的字符串。如果这个成员是NULL，系统使用默认标题（另存为或打开)
			ofn.Flags = OFN_OVERWRITEPROMPT;//如果选择的文件已经存在，使用另存为对话框产生一个消息框。用户必须确认是否覆盖这个文件。
			ofn.lpstrFile = szFileName;//当GetOpenFileName或GetSaveFileName函数返回成功时，这个缓冲包含驱动器，路径，文件名，及所选择的文件的扩展名
			ofn.lpstrFileTitle = szTitle;//指向接收选择的文件的文件名和扩展名的缓冲（不带路径信息）。这个成员可以是NULL
			if (GetSaveFileName(&ofn))//创建一个Save公共对话框，以便用户指定驱动器、目录和文件名
			{//如果选择了一个路径
				return SendMessage(hwnd, WM_COMMAND, MAKELONG(IDM_FILE_SAVE, 0), 0);//根据所选路径保存文件
			}
			return 0;//没有选择路径，即取消保存，保存失败，因为其它地方通过SendMessage()调用这个消息，返回值0表示 保存失败
		case IDM_FILE_PAGESET://页面设置
			return 0;
		case IDM_FILE_PRINT://打印
			if (!PrintFile(hInst, hwnd, hEdit, szTitle))//进行打印文件
				MessageBox(hwnd, TEXT("打印文件失败"), szAppName, MB_OK | MB_ICONWARNING);//打印失败
			return 0;
		case IDM_APP_EXIT://退出
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;
		case IDM_EDIT_FIND: //查找
			InitFind(hEdit, szFindText);//初始化查找对话框
			hDlgModeLess = FindDialog(hwnd, szFindText);//弹出查找对话框操作
			return 0;
		case IDM_EDIT_FINDNEXT://查找下一个
			InitFind(hEdit, szFindText);//初始化查找对话框
			if (szFindText[0] == '\0'||!pfr)//如果还没有要查找的字符串或从未打开过查找对话框
				hDlgModeLess = FindDialog(hwnd, szFindText);//弹出查找对话框操作，让用户输入字符串查找
			else
			{
				if (!MyFindText(hEdit, *pfr, bDown))//查找下一个
					OKMessage(hDlgModeLess, TEXT("找不到\"%s\""),pfr->lpstrFindWhat);//查找不到
			}
			return 0;
		case IDM_EDIT_REPLACE ://替换
			InitFind(hEdit, szFindText);//初始化查找对话框
			hDlgModeLess = ReplaceDialog(hwnd, szFindText, szReplaceText);////弹出替换对话框操作
			break;
		case IDM_EDIT_GOTO: //转到
			DialogBox(hInst, TEXT("IDD_DIALOGGOTO"), hwnd,DlgGotoProc);
			return 0;
		case IDM_EDIT_TIMEDATE://时间/日期
			FormatTime();//对系统时间进行格式化
			SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//当前选中文字的开始、结束位置，若未选中则是当前光标相对于开头位置的偏移（字符个数）			
			SendMessage(hEdit, EM_REPLACESEL, startPos,(long)statusInfo[1].c_str());//在当前光标插入时间
			return 0;
		case IDM_EDIT_SELECTALL://全选
			SendMessage(hEdit, EM_SETSEL, 0, -1);//0表示从第1个字符开始，-1表示到最后1个字符
			return 0;//发送消息让编辑框默认的窗口过程处理
		case IDM_EDIT_UNDO :  //撤销
			SendMessage(hEdit, WM_UNDO, 0, 0);//发送消息让编辑框默认的窗口过程处理
			return 0;
		case IDM_EDIT_CUT ://剪切
			SendMessage(hEdit, WM_CUT, 0, 0);//发送消息让编辑框默认的窗口过程处理
			return 0;
		case IDM_EDIT_COPY :  //复制
			SendMessage(hEdit, WM_COPY, 0, 0);//发送消息让编辑框默认的窗口过程处理
			return 0;
		case IDM_EDIT_PASTE:  //粘贴
			SendMessage(hEdit, WM_PASTE, 0, 0);//发送消息让编辑框默认的窗口过程处理
			return 0;
		case IDM_EDIT_DELETE://删除选中   未处理删除后面
			SendMessage(hEdit, WM_CLEAR, 0, 0);
			break;
		case IDM_FORMAT_AUTOLINE://自动换行
			{
				int nFileLength = GetWindowTextLength(hEdit);//返回编辑框字符个数
				pText = new TCHAR[nFileLength + 1];//保存要写入的字符串,+1是因为\0
				GetWindowText(hEdit, pText, nFileLength+1);
				shared_ptr<TCHAR>sp1(pText, [](TCHAR *p){delete[]p; });//将分配的空间交智能指针管理	
				DestroyWindow(hEdit);//销毁原来的编辑框
				if (bAutoLine)//如果原先自动换行
				{
					CheckMenuItem(hMenu, IDM_FORMAT_AUTOLINE, MF_UNCHECKED);//取消选择状态
					bAutoLine = false;
					//创建编辑框，加个水平滚动条，不换行
					hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL |WS_HSCROLL| ES_MULTILINE | ES_NOHIDESEL, 0, 0, 0, 0,
						hwnd, (HMENU)ID_EDIT, hInst, nullptr);//创建有边界的多行编辑框，ES_NOHIDESEL在编辑框失去焦点后选中的文本依然保持高亮显示
					MoveWindow(hEdit, 2,2, cxClient-4, cyClient-29, true);//-4是留出宽度画边框，-29是把边框画在状态栏上面，状态栏高度25，留4个宽度画边框	
					ShowWindow(hStatus, SW_NORMAL);//显示状态栏
					EnableMenuItem(hMenu, IDM_LOOKUP_STATE, MF_ENABLED);//让状态栏有效
					CheckMenuItem(hMenu, IDM_LOOKUP_STATE, MF_CHECKED);//选中状态栏按钮
					bShowStatus = true;//状态栏被选中标记
				}
				else
				{
					CheckMenuItem(hMenu, IDM_FORMAT_AUTOLINE, MF_CHECKED);//选择状态
					bAutoLine =true;
					//创建编辑框，无水平滚动条，会自动换行
					hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_NOHIDESEL, 0, 0, 0, 0,
						hwnd, (HMENU)ID_EDIT, hInst, nullptr);//创建有边界的多行编辑框，ES_NOHIDESEL在编辑框失去焦点后选中的文本依然保持高亮显示
					MoveWindow(hEdit,2,2, cxClient-4, cyClient-4, true);//-4是留出宽度画边框，将编辑框覆盖客户区		
					ShowWindow(hStatus, SW_HIDE);//隐藏状态栏
					EnableMenuItem(hMenu, IDM_LOOKUP_STATE, MF_GRAYED);//让状态栏无效
					CheckMenuItem(hMenu, IDM_LOOKUP_STATE, MF_UNCHECKED);//取消选中状态栏按钮
					bShowStatus = false;//状态栏未被选中标记
				}
				oldEditTextProc = (WNDPROC)SetWindowLong(hEdit, GWL_WNDPROC, (long)EditTextProc);//设置编辑框新的对话框过程，以处理WM_KEYDOWN之类的键盘、鼠标消息
				SendMessage(hEdit, EM_LIMITTEXT, 0, 0);//当wParam为0时，多行编辑控伯最多0x7FFFFFFE个字符，默认是30000个字符
				SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);//设置编辑框字体
				SetWindowText(hEdit, pText);
			}
			return 0;
		case IDM_FORMAT_FONT://字体
			if (ChooseFont(&cf))//选择字体
			{
				DeleteObject(hFont);//删除上一次的画毛
				hFont = CreateFontIndirect(&lf);//根据逻辑字体创建字体句柄
				SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);//对编辑框发送消息，设置新画笔
				InvalidateRect(hwnd, nullptr, true);//用新的字体重新显示
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
				DeleteObject(hBrush);//删除旧画刷
				hBrush=CreateSolidBrush(cc.rgbResult);//用背景色，创建新画刷
				InvalidateRect(hEdit, nullptr, true);//编辑框重绘
			};
			break;
		case IDM_LOOKUP_STATE://状态栏
			if (bShowStatus)//如果状态栏显示
			{
				bShowStatus = false;//标记不显示
				CheckMenuItem(hMenu, IDM_LOOKUP_STATE, MF_UNCHECKED);//取消选中
				ShowWindow(hStatus, SW_HIDE);//隐藏状态栏
				MoveWindow(hEdit, 2, 2, cxClient - 4, cyClient - 4, true);//-4是留出宽度画边框，调整编辑大小为整个客户区		
			}
			else
			{
				bShowStatus = true;//标记显示
				CheckMenuItem(hMenu, IDM_LOOKUP_STATE, MF_CHECKED);//选中
				ShowWindow(hStatus, SW_NORMAL);//显示状态栏
				MoveWindow(hEdit, 2, 2, cxClient - 4, cyClient - 29, true);//-4是留出宽度画边框，-29是把边框画在状态栏上面，状态栏高度25，留4个宽度画边框
			}
			return 0;
		case IDM_APP_HELP://帮助
			DialogBox(hInst,TEXT("IDD_DIALOGHELP"),hwnd, DlgHelpProc);//弹出帮助对话框
			return 0;
		case IDM_APP_ABOUT: //关于
			MessageBox(nullptr, TEXT("Notepad Program\n(c)yong hua by,2015"), TEXT("关于Menu"), MB_ICONINFORMATION | MB_OK);
			return 0;
		}
		break;
	case WM_CTLCOLOREDIT:
			hdc = (HDC)wParam;//wParam是编辑框的画笔，lParam是编辑框的句柄
			SetBkColor(hdc,textBkColor);//设置字体背景颜色，应与编辑框背景颜色一致，否则存在2种背景色不好看
			SetTextColor(hdc, textColor);//设置字体的颜色
			return (long)hBrush;//这里不能用break,否则会调用下面的DefWindowProc，将颜色设回默认的，返回一个画刷，编辑框的默认窗口过程会用这个画刷作为背景色
	case WM_PAINT://有区域失效，发生重绘
		hdc = BeginPaint(hwnd, &ps);//获得窗口画笔
		EndPaint(hwnd, &ps);//释放画笔资源
		DrawBorder(hwnd,bShowStatus);
		return 0;
	case WM_TIMER://每一秒更新一次时间
		FormatTime();//对系统时间进行格式化
		SendMessage(hStatus, SB_SETTEXT, 1, (long)statusInfo[1].c_str());//对第二栏更新时间 
		return 0;
	case WM_QUERYENDSESSION://关机时询问是否退出，点确定才退出关机，否则会取消关机
		if (IDYES == AskConfirmation())//窗口关闭时询问是否退出，点确定才退出
			return 1;//当结束对话或者一个程序调用系统关闭功能的时候，WM_QUERYENDSESSION消息会被发送给尚未终止的所有窗口。当程序在处理这个消息的时候，如果返回了false（0），那么系统将不结束对话或者关机（注销）
		else
			return 0;
	case WM_CLOSE://窗口关闭
		if (bNeedSave&&IDCANCEL == AskAboutSave(hwnd, szFileName))
			return 0;//如果文本更改了，需要进行保存，但回答是不保存则不允许退出程序，不往下执行
		if (IDYES == AskConfirmation())//窗口关闭时询问是否退出，点确定才退出
		DestroyWindow(hwnd);//销毁窗口
		return 0;
	case WM_DESTROY://窗口销毁时
		DeleteObject(hFont);//删除上一次的画毛
		DeleteObject(hBrush);//删除旧画刷
		KillTimer(hwnd, ID_TIMER);
		PostQuitMessage(0);//抛出一条返回值为0退出消息
		return 0;
	default:
		if (UMsg == WM_FINDREPLACE)//不能放在case 语句中，因为case语句要常量，而WM_FINDREPALCE是自己定义的一个变量
		{
			pfr = (LPFINDREPLACE)lParam;//当收到这个消息，lparam参数保留着FINDREPLACE结构指针
			if (pfr->Flags&FR_DIALOGTERM)//如果按下了关闭按钮
				hDlgModeLess = 0;//对对话框句柄置空
			if (pfr->Flags&FR_DOWN)//向下按钮被按下
				bDown = true;
			else
				bDown = false;
			if (pfr->Flags&FR_FINDNEXT)//如果按下了查找下一个
			{
				if(!MyFindText(hEdit,*pfr,bDown))//查找下一个
					OKMessage(hDlgModeLess, TEXT("找不到\"%s\""),pfr->lpstrFindWhat);//查找不到
			}
			if (pfr->Flags&FR_REPLACE)//如果按下了替换下一个
			{
				SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//当前选中文字的开始、结束位置，若未选中则是当前光标相对于开头位置的偏移（字符个数）			
				if (startPos != endPos)//如果开始和结束位置不相等说明选中了文字，进行替换
				SendMessage(hEdit, EM_REPLACESEL, 0,(long) pfr->lpstrReplaceWith);//替换选中的字符串
				if(!MyFindText(hEdit,*pfr,bDown))//查找下一个
					OKMessage(hDlgModeLess, TEXT("找不到\"%s\""),pfr->lpstrFindWhat);//查找不到
			}
			if (pfr->Flags&FR_REPLACEALL)//如果按下了替换全部
			{
				int count = 0;//替换的个数
				SendMessage(hEdit, EM_SETSEL, 0, 0);//设置当前光标在文件开头
				TCHAR szBuffer[64];
				while (MyFindText(hEdit, *pfr, bDown))//查找下一个
				{
					SendMessage(hEdit, EM_REPLACESEL, 0, (long)pfr->lpstrReplaceWith);//替换选中的字符串
					count++;
				}
				StringCchPrintf(szBuffer, 64, TEXT("已替换%d个 \"%s\""), count, pfr->lpstrFindWhat);
				MessageBox(hDlgModeLess, szBuffer, szAppName, MB_OK | MB_ICONINFORMATION);
			}
		}
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//用默认的窗口过程处理其它消息，做些其它处理
}
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,TCHAR *  szCmdCommand, int nCmdShow)
{
	HACCEL hAccel;//加速键（快捷键）句柄
	WNDCLASS wndclass;//窗口类
	wndclass.cbClsExtra = 0;//为类分配的额外空间，可以保存一些地址、变量
	wndclass.cbWndExtra = 0;//为窗口分配的额外空间，可以保存一些地址、变量
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//背景画刷
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//鼠标光标
	wndclass.hIcon = LoadIcon(hInstance,TEXT("POPAD"));//图标
	wndclass.hInstance = hInstance;//实例名柄，程序的PID，一个整数，根据这个整数，系统可以找到进程的地址
	wndclass.lpfnWndProc = WndProc;//窗口过程
	wndclass.lpszClassName = szAppName;//类名
	wndclass.lpszMenuName = TEXT("POPADMENU");//菜单
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//哪些改变后要重画，设置水平和垂直改变后重绘
	if (!RegisterClass(&wndclass))//注册窗口类
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindowEx(WS_EX_ACCEPTFILES,szAppName, nullptr, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance,szCmdCommand);//创建窗口
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	ShowWindow(hwnd, nCmdShow);//显示窗口
	UpdateWindow(hwnd);//进行重绘
	MSG msg;//消息结构
	hAccel = LoadAccelerators(hInstance, TEXT("POPAD"));//从资源文件读取加速键句柄
	while (GetMessage(&msg, nullptr, 0, 0))//从消息队列获取消息
	{
		if (hDlgModeLess == 0 || !IsDialogMessage(hDlgModeLess, &msg))//因为非模态对话框会进入消息队列，此句是将如果无此语句，在对话框中不能输入任何信息，在对话框输入却输入进了
			//编辑框，加入此语句，将队列消息翻译成对话框消息，窗口按键会变成对话框中的按键
		{
			if (!TranslateAccelerator(hwnd, hAccel, &msg))
			{//TranslateAccelerator，函数功能：翻译加速键表。该函数处理菜单命令中的加速键。
				//该函数将一个WM_KEYDOWN或WM_SYSKEYDOWN消息翻译成一个WM_COMMAND或WM_SYSCOMMAND消息（如果在给定的加速键表中有该键的入口），
				//然后将WM_COMMAND或WM_SYSCOMMAND消息直接送到相应的窗口处理过程
				//若TransLateAccelerator返回非零值且消息已被翻译，说明从加速捷表中找到了对应的按键，直接调用窗口过程
				//直到窗口过程处理完消息后才返回，应用程序就不能调用TranslateMessage函数对消息再做处理，若没找到，不翻译，返回0
				TranslateMessage(&msg);//将一些按键消息转换成字符消息放在按键消息后面
				DispatchMessage(&msg);//分发消息，即调用窗口过程进行处理消息
			}
		}
	}
}