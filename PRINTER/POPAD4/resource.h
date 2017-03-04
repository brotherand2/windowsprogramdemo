# define ID_EDIT 0
//文件菜单
# define IDM_FILE_NEW 1
# define IDM_FILE_OPEN 2
# define IDM_FILE_SAVE 3
# define IDM_FILE_SAVEAS 4
# define IDM_FILE_PAGESET 5
# define IDM_FILE_PRINT 6
# define IDM_APP_EXIT 7

//编辑菜单
# define IDM_EDIT_UNDO 8  
# define IDM_EDIT_CUT 9  
# define IDM_EDIT_COPY 10  
# define IDM_EDIT_PASTE 11  
# define IDM_EDIT_DELETE 23  
# define IDM_EDIT_FIND 12  
# define IDM_EDIT_FINDNEXT 13  
# define IDM_EDIT_REPLACE 14
# define IDM_EDIT_GOTO 15  
# define IDM_EDIT_SELECTALL 16 
# define IDM_EDIT_TIMEDATE 17

//格式菜单
# define IDM_FORMAT_AUTOLINE 18 
# define IDM_FORMAT_FONT 19
# define IDM_FONTCOLOR 24
# define IDM_BKCOLOR 25

//查看
#define IDM_LOOKUP_STATE 20

//帮助菜单
# define IDM_APP_HELP 21
# define IDM_APP_ABOUT 22
#define  IDS_STATE  23//状态栏ID

#define ID_TIMER 30//计时器ID
#define ID_EDITHELP 31//帮助编辑框

# define IDC_GOTO 32
# define IDC_CANCELGOTO 33
# define IDC_EDITGOTO 34

#pragma warning(disable : 4995)
using namespace std;
//#ifndef UNICODE 
//typedef  string tstring;
//#else
//typedef  wstring tstring;
//#endif
#ifndef UNICODE //如果没有定义UNICODE ,用tstring替换为string,否则替换为wstring
using  tstring = string;
using tstringstream = ostringstream;
# define toString to_string
# define strToInt atoi
#else
using tstringstream = wstringstream;
using  tstring = wstring;
# define toString to_wstring
# define strToInt _wtoi
#endif
HWND   hDlgModeLess = 0;//非模态对话框，保存公共对话框如“查找”，“查找下一个”，“替换”
HWND hDlgPrint = 0;//打印机用的非模态对话框，用户用来终止打印
HWND hStatus;//状态栏
TCHAR unTitled[] = TEXT("无标题");//新建文件时在标题栏上的字符串
TCHAR szAppName[] = TEXT("记事本");//窗口标题名、注册的类名
bool bNeedSave = false;//是否需要保存，当文件被更改且未保存时置为true
bool bUserAbort = false;//用户是否终止打印
int startPos;//当前选中文字的开始位置，若未选中则是当前光标相对于开头位置的偏移（字符个数）
int endPos;//当前选中文字的结束位置+1，若未选中则是当前光标相对于开头位置的偏移（字符个数）
int row, col;//光标的行列
TCHAR szWorkSpacePath[MAX_PATH];//保留应用程序工作区间的路径，
//因为应用程序如果在过去使用过打开或另存为对话框，将使用最近选择的路径作为初始目录，所以应用程序在打开或另存为对话框选择后会改变当前目录，这样会找不到当前目录下的资源文件，所以要保留应用程序的当前目录
LPFINDREPLACE pfr = nullptr;//查找和替换结构的指针
WNDPROC oldEditTextProc;//保留记事本原编辑框的窗口过程
tstring  statusInfo[5]{TEXT("新建文件"), TEXT(""), TEXT("行号:1"), TEXT("列号:1"), TEXT("")};//状态栏5条信息
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
