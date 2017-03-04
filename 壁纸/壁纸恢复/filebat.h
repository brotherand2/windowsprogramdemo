//文件批处理
//#define IDD_FILEBAT   71
#define  IDC_ADDSUFFIXOK   72
#define IDC_FILEDIRECTORY   73
#define IDC_DIGITNUM  74
#define IDC_LETTERNUM   75
#define IDC_RENAMEFIRSTNUM   76
#define IDC_FIRSTNUM   77
#define IDC_RENAMEOK   78
#define IDC_CREATEDIRECTORYNUM  79
#define IDC_CREATEFILENUM   80
#define IDC_CREATEFILEOK   81
#define IDC_DELETEINCLUDESUB   82
#define IDC_DELETEOK   83
#define IDC_SUFFIXALLFILE   84
#define IDC_SUFFIXTXT   85
#define IDC_SUFFIXJPG   86
#define IDC_SUFFIXPNG   87
#define  IDC_SUFFIXOTHER   88
#define IDC_LISTSUFFIX   89
#define IDC_ADDOTHERSUFFIX   90
#define IDC_ADDDIRECTORY   91
#define IDC_DELETESUFFIX   92
#define  IDC_FILENAMEPREFIX   93
#define IDC_FILENAMESUFFIX  94
#define IDC_RANDOMFILENAME   95
#define IDC_DELETESAMESIZEFILE   96
#define IDC_DELETEEMPTYDIRECTORY  97
#define IDC_FILEEXTENTION   98
#define IDC_CREATEDIRECTORYOK 99
#define IDB_BITMAP2 100
#define IDB_BITMAP12 101
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

extern int _stdcall EditTextProc(HWND hEdit, unsigned message, unsigned wParam, long lPram);//重写编辑控件窗口过程
