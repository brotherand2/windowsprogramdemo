#define _WIN32_WINNT 0x0501     // 将此值更改为相应的值，以适用于 Windows 的其他版本,最低兼容版本5.1即windows xp及以上。 ，如果未设置则气泡在WIN XP,WIN 2003上不能显示

#include<Windows.h>
#include<WindowsX.h>
#include<string>
#include<vector>
#include<ctime>
#include<map>
#include<sstream>
#include<fstream>
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
#define IDR_MENUMAIN                       102
#define IDI_ICON2                       103
#define IDI_ICON3                       104
#define IDI_ICON4                       105
#define IDR_MENUCLIP                    106
#define IDD_ABOUTBOX                    107
#define IDI_ZYHTEMPLATE                   108
//弹出菜单ID
#define IDM_SHOWWALLPAPER  3
#define IDM_NEXTWALLPAPER  4
#define IDM_PREWALLPAPER  5
#define IDM_PAUSE  6
#define IDM_EXIT  7
#define IDM_RANDOM  8
#define IDM_MINI  9
#define IDM_SET  10


//菜单ID
#define  IDM_MODIFYWAPPERDIRECTORY      40001

#define ID_ADDWAPPER                    40002
#define IDM_ABOUT                  40003

#define IDM_TOOLPRINT 11
# define IDM_EDIT_FIND 12  
# define IDM_EDIT_FINDNEXT 13  
# define IDM_EDIT_GOTO 15  
# define IDM_FORMAT_FONT 19
# define IDM_FONTCOLOR 24
# define IDM_BKCOLOR 25
# define IDM_WALLPAPERSET 26
//转到对话框ID
# define ID_EDIT 27
# define IDS_STATE 28
# define IDC_GOTO 32
# define IDC_CANCELGOTO 33
# define IDC_EDITGOTO 34
//修改目录ID
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
//设置对话框ID
#define  IDM_SETFINISH 45
#define   IDC_EDITHOUR 46
#define  IDC_EDITMINUTE 47
# define IDC_EDITSECOND 48
# define  IDC_CHANGEMINI 49
#define IDC_RANDOM 50
#define IDC_SEQUENCE 51
#define  IDC_EDITTIP 52
#define IDC_EDITTITLE 53
#define IDC_EDITCONTENT 54
#define IDC_FINDSUBDIRECTORY 55
#define IDC_NOTIFYHOUR 56
#define IDC_NOTIFYMINUTE 57
#define IDC_NOTIFYSECOND 58 
#define IDC_AUTOBOOT 59

#define ID_HOTKEYSHOW 60//热键ID
#define ID_HOTKEYCLIP 61

# define IDD_CLIPBITMAP 62
#define IDM_NEWCLIP   63

#define IDC_STARTINGMINISHOW         64
#define IDC_FILTERWINDOW                65
#define IDC_CURRENTWINDOW               66
#define IDC_SHUTDOWNBIBLE            67
#define IDC_MINISIZE            68
#define IDC_BTNFILTERWINDOW            69
#define IDM_FILEBAT            70
#define IDB_BITMAP10 1000
#define IDB_BITMAP11 1001

static NOTIFYICONDATA nid;//系统需要的用来传递托盘区域消息的信息
TCHAR szDirectoryPath[MAX_PATH];//用户选择返回的目录路径
COLORREF textColor = RGB(0, 0, 0);//默认的字体颜色
COLORREF textBkColor = RGB(200, 200, 200);//默认的字体背景颜色轻灰色
HWND hEdit;//编辑框
HWND hStatus;//状态栏
static HWND hDlg;//非模态截图对话框句柄
HWND   hDlgFind = 0;//非模态对话框，保存公共对话框如“查找”，“查找下一个”
HMENU hMenu;//菜单句柄
unsigned int WM_FIND;//自定义的消息，用于接收查找的消息
LOGFONT lf;//逻辑字体
CHOOSEFONT cf;//字体结构
CHOOSECOLOR cc;//颜色结构
HBRUSH hBKBrush;//背景画刷
COLORREF color[16];//颜色对话框中保存16个自定义颜色
HFONT hFont;//字体
HBRUSH blueBrush;//蓝色画刷，选中时高亮显示的颜色
vector<tstring> format = { TEXT("\\*.jpg"), TEXT("\\*.bmp"), TEXT("\\*.png") };//壁纸后缀格式
vector<tstring>allTitle;//保存所有顶层窗口的标题
vector<tstring>backupDirectorys;//备份目录
vector<tstring>currentDirectorys;//当前壁纸目录
vector<tstring>filterWindowName;//被过滤的窗口名，开启【仅有桌面这个窗口才换壁纸】不正常时进行过滤
vector<int>directoryIndex;//每个目录第1张图片的索引
vector<int>recordChange;//保存换过的壁纸的索引，实现上一张下一张功能
map<tstring, tstring> tempDirectory;//临时目录map,第1个元素是图片名，第2个元素是图片路径
vector<map<tstring,tstring>> AllDirectorys;//所有的壁纸目录，每个壁纸目录用map结构保存,map中第1个元素是图片名，第2个元素是图片路径
tstring fileCreateTime;//文件创建时间
tstring fileLastAccessTime; // 文件最后一次访问时间
tstring fileLastWriteTime; // 文件最后一次修改时间
tstring fileAttributes; //文件属性
tstring fileSize;//文件大小
tstring  statusInfo[4] = { TEXT(""), TEXT(""), TEXT("行号:1"), TEXT("列号:1") };//状态栏4条信息
bool startChange = true;//是否开始换壁纸
bool includeSubDirectory = true;//是否包含子目录
bool changeByOnlyDestop =false;//是否仅有桌面这个窗口才换壁纸
bool buttonDown = false;//鼠标单击
bool randomChange = true;//换壁纸模式，随机换和顺序换
bool startingUpBoot = false;//开机自启动标记
bool bFullScreen = false;//是否全屏
bool hotKeyClip = false;//截图是由热键引起
bool closeBubble = false;//是否关闭气泡功能
bool runningMiniShow = true;//开机最小化显示
static UINT miniPhotoSize = 0;//图片大小不得低于
int width, heigh;//截图的宽、高
RECT clipRect;//剪切区域的矩形
const int  wallpaperID = 1;//壁纸定时器ID
const int  tipID = 2;//气泡定时器ID
const int clockID = 3;//时钟ID
const int timingID = 4;//定时保存ID
unsigned int  wallPaperTime;//换壁纸的时间间隔
unsigned  int tipTime;//所泡弹出的时间间隔
unsigned  int timingTime=10*60*1000;//定时保存存档时间间隔为10分钟
UINT nSelectWallpaper =0;//被选中的壁纸
int cxClient, cyClient;//客户区大小
int cxScreen, cyScreen;//屏幕宽高
int cxChar, cxCaps, cyChar;//字母高度、宽度
int nPageLine;//一页的行数
size_t row, col;//光标的行列
UINT wallPaperCount = 0;//图片文件个数
int lineIndex;//指定行首字符在文本中的位置（以字节数表示）
int nextLineIndex;//指定行下一行首字符在文本中的位置（以字节数表示）
int recordIndex=0;//当前壁纸在换过的壁纸中的记录中的索引
RECT windowSize;//窗口大小位置
typedef struct
{
	unsigned int hour, minute, second;//时分秒
}Time;//时间结构

static Time Wallpaper{ 0, 0, 2 };//壁纸更换时间
static Time Notify{ 0, 0, 36 };//气泡弹出时间
WNDPROC oldEditTextProc;//保留原编辑框的窗口过程
//函数声明
void ChangeWallPaper();//改变壁纸
void HighLineWallPaper(HWND hEdit, int  nSelectWallpaper);//对当前的壁纸进行高亮显示
void NextWallPaper();//下一张壁纸
int _stdcall DlgWallpaperSet(HWND hDlg, unsigned int message, unsigned wParam, long lParam);//设置壁纸对话框窗口过程
int _stdcall DlgPrintProc(HWND hDlg, unsigned int UMsg, unsigned int wParam, long lParam);//截图窗口过程
void SetStartingUpBoot(TCHAR*);//设置开机启动
bool readFromXML(HWND);//读取存档文件
void writeToXML(HWND);//保存在存档文件
void FindPhoto();
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
int _stdcall DlgFileBatProc(HWND hDlg, UINT message, UINT wParam, long lParam);//文件批处理对话框过程
//#pragma comment(lib,"tinyxml.lib")//导入第三方库tinyxml
#define TIXML_USE_STL//第三方库tinyxml使用stl

