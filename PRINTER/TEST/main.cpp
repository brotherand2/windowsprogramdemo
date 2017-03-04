#include<windows.h>
#include<memory>
#include<commctrl.h>
#include"resource.h"

using namespace std;
enum TextCode//文本编码类型
{
	ANSI = 0,//ANSI编码
	UNICODETEXT = 1,//UNICODE编码，文件头，十六进制表示为FF FE，回车换行符  双字节   000d  000a,最低有效字节优先，如INTEL处理器，字符'A',是0x0041,但在内存中低字节优先，所以是按0x41,0x00这样的顺序存储
	UNICODEBIGENDIAN = 2,//UNICODE big endian编码，文件头十六进制表示为FE FF ，回车换行符,双字节,十六进制表示为0d00  0a00,最高有效字节优先，如Macintosh处理器，字符'A',是0x0041,但在内存中高字节优先，所以是按0x00，0x41这样的顺序存储
	UTF8 = 3,//UTF-8编码，
	/*
	文件头，十六进制表示为EF BB BF。
	UTF-8是Unicode的一种变长字符编码，数字、字母、回车、换行都用一个字节表示,汉字占3个字节.
	回车换行符，单字节，十六进制表示为0d 0a
	*/
};
void GetTextType(unsigned char text[], TextCode &fileType)//得到文本编码类型
{

	if (text[0] == 0xff && text[1] == 0xfe)
		fileType = UNICODETEXT;//UNICODE编码
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

bool MyReadFile(HWND hEdit, HWND hStatus, TCHAR szFileName[])//读取文件内容
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
	unsigned char *pText = nullptr;//保存读取的字符串的字节，因为读的字节是0-FF范围，所有用无符号保存,若用char 则读取ansi字符无问题，因为ansi字符不超过128,但UNICODE字符值超过128，如255，unsigned char 
	//读取是255(FF)，用char 读取-1(FFFFFFFF)
	TCHAR *pConvert = nullptr;//保存转换后的字符串
	wchar_t *temp = nullptr;//作为交换用的临时宽字符串
	unsigned long  nRealSize;//读取文件真实读的大小
	pText = new unsigned char[nFileLength + 2];//分配多2个空间放置2个\0\0，因为可能是UNICODE编码，UNICODE字符串后面有2个\0\0
	shared_ptr<unsigned char>sp1(pText, [](unsigned char *p){delete[]p; });//将分配的空间交智能指针管理	
	shared_ptr<TCHAR>sp2(pConvert, [](TCHAR *p){delete[]p; });//将分配的空间交智能指针管理
	shared_ptr<wchar_t>sp3(temp, [](wchar_t *p){delete[]p; });//将分配的空间交智能指针管理
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
		len = MultiByteToWideChar(CP_ACP, 0, (char *)pText, -1, nullptr, 0);//将多字节字符转换成宽字符需要的字符个数,包括空字符
		pConvert = new TCHAR[len];
		MultiByteToWideChar(CP_ACP, 0, (char *)pText, -1, (wchar_t *)pConvert, nFileLength + 1);//将多字节字符转换成宽字符
		isConvert = true;//转换了
#endif
		SendMessage(hStatus, SB_SETTEXT, 4, (long)TEXT("ANSI"));//在第四栏显示编码类型
		break;
	case UNICODETEXT://UNICODE编码文件
		SendMessage(hStatus, SB_SETTEXT, 4, (long)TEXT("UNICODE"));//在第四栏显示编码类型
		pText += 2;//跳过2个字符，即字符标记FF FE
		nFileLength -= 2;//不包括FF FE这2个字符长度
#ifndef UNICODE//如果不是UNICODE程序
		len = WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pText, -1, nullptr, 0, nullptr, nullptr);//获得要转换的字符个数，包括\0
		pConvert = new TCHAR[len];
		WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pText, -1, (char *)pConvert, len, nullptr, nullptr);
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
			pText[2 * i] = pText[2 * i + 1];
			pText[2 * i + 1] = cc;
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
		SetWindowText(hEdit, (LPCTSTR)pConvert);//LPCSTR根据是否UNICODE 转变成const char*或const wchar *
	else//将未转换的字符串放进编辑框
		SetWindowText(hEdit, (LPCTSTR)pText);//LPCSTR根据是否UNICODE 转变成const char*或const wchar *
	CloseHandle(hFile);//读取文件后关闭文件句柄
	return true;
}
static WNDPROC oldHelpProc;

int _stdcall EditHelpProc(HWND hEdit, unsigned message, unsigned wParam, long lPram)//重写编辑控件对话框过程
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		//MessageBox(0, TEXT("your key"), 0, 0);
		break;

	}
	return CallWindowProc(oldHelpProc, hEdit, message, wParam, lPram);//调用默认的编辑控件窗口处理过程处理其它消息和未处理完的操作
}
int _stdcall DlgHelpProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)
{
	HWND hEdit;//编辑框
	switch (message)
	{
	case WM_INITDIALOG://初始对话框//E:\\mfc编程\\PRINTER\\TEST\\ /
		hEdit = GetDlgItem(hDlg, IDC_EDITGOTO);//得到帮助编辑框句柄

		MyReadFile(hEdit, 0, TEXT("help.txt"));//读取帮助文件的内容到编辑框
		//SetFocus(hEdit);
		//oldHelpProc = (WNDPROC)SetWindowLong(hEdit, GWL_WNDPROC,(long) EditHelpProc);
		return true;
	case WM_CLOSE:
		EndDialog(hDlg, 0);//关闭时销毁窗口
		return true;
	}
	return false;
}
int WinMain(HINSTANCE hInst, HINSTANCE hPreInst, char *command, int show)
{
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGGOTO), 0, DlgHelpProc);
}