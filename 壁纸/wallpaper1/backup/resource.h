//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ 生成的包含文件。
// 供 wallpaper1.rc 使用
//
#define _WIN32_WINNT 0x0501     // 将此值更改为相应的值，以适用于 Windows 的其他版本,最低兼容版本5.1即windows xp及以上。 ，如果未设置则气泡在WIN XP,WIN 2003上不能显示

#include<Windows.h>
#include<WindowsX.h>
#include<string>
#include<vector>
#include<ctime>
#include<sstream>
#include<strsafe.h>
#ifndef UNICODE 
# define  tstring string 
# define ToString to_string
#else
#define   tstring wstring
# define ToString to_wstring
#endif
using namespace std;
#define IDI_ICON1                       101
#define IDR_MENU1                       102
#define IDI_ICON2                       103
#define IDI_ICON3                       104
#define IDI_ICON4                       105
#define ID_40001                        40001
#define ID_ADDWAPPER                    40002
#define ID_WAPPERSET                    40003

TCHAR szTip[128] = TEXT("咄，呆子看什么");//气泡的提示信息
TCHAR szInfoTitle[64] = TEXT("对着屏幕那么久，该出去休息休息了，注意眼睛");//字符串的内容为气球提示的标题。此标题出现在气球提示框的上部，最多含有63个字符。
TCHAR szInfo[256] = TEXT("现在换壁纸了，在回到桌面的时候，每2秒换一次壁纸");////字符串的内容为气球提示内容。最多含有255个字符
TCHAR szDirectoryName[MAX_PATH];//用户选择的目录名
TCHAR szDirectoryPath[MAX_PATH];//用户选择返回的目录路径
TCHAR szMyPictures[MAX_PATH];//电脑上我的图片文件夹路径
TCHAR szMyDocument[MAX_PATH];//电脑上我的文档文件夹路径
TCHAR szName[31];//文件名
const int  wallpaperID = 1;//壁纸定时器ID
const int  tipID = 2;//气泡定时器ID
int  wallPaperTime = 1000;//换壁纸的时间间隔
int tipTime = 360000;//所泡弹出的时间间隔
vector<tstring>fileName;//查找到的图片文件名
vector<tstring>allTitle;//保存所有顶层窗口的标题
tstring temp;//
tstring fileCreateTime;//文件创建时间
tstring fileLastAccessTime; // 文件最后一次访问时间
tstring fileLastWriteTime; // 文件最后一次修改时间
tstring fileAttributes; //文件属性
tstring fileSize;//文件大小
tstring filePath;//文件路径
bool startChange = true;//是否开始换壁纸
int cxClient, cyClient;//客户区大小
int cxChar, cxCaps, cyChar;//字母高度、宽度
int nPageLine;//一页的行数

const int IDM_SHOWWALLPAPER = 3;//弹出菜单ID
const int IDM_NEXTWALLPAPER = 4;
const int IDM_PREWALLPAPER = 5;
const int IDM_PAUSE = 6;
const int IDM_EXIT = 7;
TCHAR szTop[] = TEXT("索引    文件名                        大小                属性           \
创建时间                      最后一次访问时间              最后一次修改时间              路径");//顶部的列句
typedef struct
{
	unsigned int G, M, K, B;//分别代表GB,MB,KB,BYTE
}capacity;//容量结构

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        103
#define _APS_NEXT_COMMAND_VALUE         40004
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
