#define _WIN32_WINNT 0x0501     // 将此值更改为相应的值，以适用于 Windows 的其他版本,最低兼容版本5.1即windows xp及以上。 ，如果未设置则气泡在WIN XP,WIN 2003上不能显示

#include<Windows.h>
#include<WindowsX.h>
#include<string>
#include<vector>
#include<ctime>
#include<sstream>
#include<memory>
#include<algorithm>
#include<strsafe.h>
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
using namespace std;
#define IDI_ICON1                       101
#define IDR_MENU1                       102
#define IDI_ICON2                       103
#define IDI_ICON3                       104
#define IDI_ICON4                       105
#define  IDM_MODIFYWAPPERDIRECTORY      40001
#define ID_ADDWAPPER                    40002
#define ID_WAPPERSET  
# define IDM_EDIT_FIND 12  
# define IDM_EDIT_FINDNEXT 13  
# define IDM_EDIT_GOTO 15  
# define IDM_FORMAT_FONT 19
# define IDM_FONTCOLOR 24
# define IDM_BKCOLOR 25
# define IDM_WALLPAPERSET 26
# define ID_EDIT 27
# define IDS_STATE 28
# define IDC_GOTO 32
# define IDC_CANCELGOTO 33
# define IDC_EDITGOTO 34
# define IDC_EDITDIRECTORY 35
#define IDC_LISTLEFT 36
#define IDC_LISTRIGHT 37
#define IDC_MOVERIGHT 38
# define IDC_MOVELEFT 39
# define IDC_BTNDELETELEFT 40
# define IDC_BTNDELETERIGHT 41
# define IDC_BTNADDDIRECTORY 42
# define IDC_BTNOK 43
# define IDC_STATIC -1
# define IDC_BTNSETFINISH 44
TCHAR szTip[128] = TEXT("咄，呆子看什么");//气泡的提示信息
TCHAR szInfoTitle[64] = TEXT("对着屏幕那么久，该出去休息休息了，注意眼睛");//字符串的内容为气球提示的标题。此标题出现在气球提示框的上部，最多含有63个字符。
TCHAR szInfo[256] = TEXT("现在换壁纸了，在回到桌面的时候，每2秒换一次壁纸");////字符串的内容为气球提示内容。最多含有255个字符
TCHAR szDirectoryName[MAX_PATH];//用户选择的目录名
TCHAR szDirectoryPath[MAX_PATH];//用户选择返回的目录路径
TCHAR szMyPictures[MAX_PATH];//电脑上我的图片文件夹路径
TCHAR szMyDocument[MAX_PATH];//电脑上我的文档文件夹路径
TCHAR szName[31];//文件名
HWND hStatus;//状态栏
HWND   hDlgFind = 0;//非模态对话框，保存公共对话框如“查找”，“查找下一个”
HMENU hMenu;//菜单句柄
LPITEMIDLIST plDlist;//保存用户选择的目录信息结构
LPMALLOC pMalloc;//LPMALLOC是一个IMalloc指针，IMALLOC实现了一个非常完整的内存分配引擎，用于释放ITEMIDLIST指针所指向的空间
vector<tstring>fileName;//查找到的图片文件名
vector<tstring>filePath;//查找到的图片文件路径
vector<tstring> format = { TEXT("\\*.jpg"), TEXT("\\*.bmp"), TEXT("\\*.png") };//壁纸后缀格式

vector<tstring>allTitle;//保存所有顶层窗口的标题
tstring temp;
tstring fileCreateTime;//文件创建时间
tstring fileLastAccessTime; // 文件最后一次访问时间
tstring fileLastWriteTime; // 文件最后一次修改时间
tstring fileAttributes; //文件属性
tstring fileSize;//文件大小
tstring wallpaper = TEXT("E:\\壁纸\\SogouWP\\Local\\WallPaper");//壁纸路径
tstring  statusInfo[4]={TEXT(""), TEXT(""), TEXT("行号:1"), TEXT("列号:1")};//状态栏4条信息
bool startChange = true;//是否开始换壁纸
bool includeSubDirectory=true;//是否包含子目录
bool changeByOnlyDestop = true;//是否仅有桌面这个窗口才换壁纸
const int  wallpaperID = 1;//壁纸定时器ID
const int  tipID = 2;//气泡定时器ID
const int clockID = 3;//时钟ID
int  wallPaperTime = 5000;//换壁纸的时间间隔
int tipTime = 360000;//所泡弹出的时间间隔
int nSelectWallpaper = -1;//被选中的壁纸
int cxClient, cyClient;//客户区大小
int cxChar, cxCaps, cyChar;//字母高度、宽度
int nPageLine;//一页的行数
int row, col;//光标的行列
const int IDM_SHOWWALLPAPER = 3;//弹出菜单ID
const int IDM_NEXTWALLPAPER = 4;
const int IDM_PREWALLPAPER = 5;
const int IDM_PAUSE = 6;
const int IDM_EXIT = 7;
typedef struct
{
	unsigned int G, M, K, B;//分别代表GB,MB,KB,BYTE
}capacity;//容量结构
WNDPROC oldEditTextProc;//保留原编辑框的窗口过程

