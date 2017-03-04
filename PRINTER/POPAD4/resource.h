# define ID_EDIT 0
//�ļ��˵�
# define IDM_FILE_NEW 1
# define IDM_FILE_OPEN 2
# define IDM_FILE_SAVE 3
# define IDM_FILE_SAVEAS 4
# define IDM_FILE_PAGESET 5
# define IDM_FILE_PRINT 6
# define IDM_APP_EXIT 7

//�༭�˵�
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

//��ʽ�˵�
# define IDM_FORMAT_AUTOLINE 18 
# define IDM_FORMAT_FONT 19
# define IDM_FONTCOLOR 24
# define IDM_BKCOLOR 25

//�鿴
#define IDM_LOOKUP_STATE 20

//�����˵�
# define IDM_APP_HELP 21
# define IDM_APP_ABOUT 22
#define  IDS_STATE  23//״̬��ID

#define ID_TIMER 30//��ʱ��ID
#define ID_EDITHELP 31//�����༭��

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
#ifndef UNICODE //���û�ж���UNICODE ,��tstring�滻Ϊstring,�����滻Ϊwstring
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
HWND   hDlgModeLess = 0;//��ģ̬�Ի��򣬱��湫���Ի����硰���ҡ�����������һ���������滻��
HWND hDlgPrint = 0;//��ӡ���õķ�ģ̬�Ի����û�������ֹ��ӡ
HWND hStatus;//״̬��
TCHAR unTitled[] = TEXT("�ޱ���");//�½��ļ�ʱ�ڱ������ϵ��ַ���
TCHAR szAppName[] = TEXT("���±�");//���ڱ�������ע�������
bool bNeedSave = false;//�Ƿ���Ҫ���棬���ļ���������δ����ʱ��Ϊtrue
bool bUserAbort = false;//�û��Ƿ���ֹ��ӡ
int startPos;//��ǰѡ�����ֵĿ�ʼλ�ã���δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������
int endPos;//��ǰѡ�����ֵĽ���λ��+1����δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������
int row, col;//��������
TCHAR szWorkSpacePath[MAX_PATH];//����Ӧ�ó����������·����
//��ΪӦ�ó�������ڹ�ȥʹ�ù��򿪻����Ϊ�Ի��򣬽�ʹ�����ѡ���·����Ϊ��ʼĿ¼������Ӧ�ó����ڴ򿪻����Ϊ�Ի���ѡ����ı䵱ǰĿ¼���������Ҳ�����ǰĿ¼�µ���Դ�ļ�������Ҫ����Ӧ�ó���ĵ�ǰĿ¼
LPFINDREPLACE pfr = nullptr;//���Һ��滻�ṹ��ָ��
WNDPROC oldEditTextProc;//�������±�ԭ�༭��Ĵ��ڹ���
tstring  statusInfo[5]{TEXT("�½��ļ�"), TEXT(""), TEXT("�к�:1"), TEXT("�к�:1"), TEXT("")};//״̬��5����Ϣ
enum TextCode//�ı���������
{
	ANSI = 0,//ANSI����
	UNICODETEXT = 1,//UNICODE���룬�ļ�ͷ��ʮ�����Ʊ�ʾΪFF FE���س����з�  ˫�ֽ�   000d  000a,�����Ч�ֽ����ȣ���INTEL���������ַ�'A',��0x0041,�����ڴ��е��ֽ����ȣ������ǰ�0x41,0x00������˳��洢
	UNICODEBIGENDIAN = 2,//UNICODE big endian���룬�ļ�ͷʮ�����Ʊ�ʾΪFE FF ���س����з�,˫�ֽ�,ʮ�����Ʊ�ʾΪ0d00  0a00,�����Ч�ֽ����ȣ���Macintosh���������ַ�'A',��0x0041,�����ڴ��и��ֽ����ȣ������ǰ�0x00��0x41������˳��洢
	UTF8 = 3,//UTF-8���룬
	/*
	�ļ�ͷ��ʮ�����Ʊ�ʾΪEF BB BF��
	UTF-8��Unicode��һ�ֱ䳤�ַ����룬���֡���ĸ���س������ж���һ���ֽڱ�ʾ,����ռ3���ֽ�.
	�س����з������ֽڣ�ʮ�����Ʊ�ʾΪ0d 0a
	*/
};
