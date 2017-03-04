//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
using namespace std;
//#ifndef UNICODE 
//typedef  string tstring;
//#else
//typedef  wstring tstring;
//#endif
#ifndef UNICODE 
using  tstring = string;
#else
using  tstring = wstring;
#endif
const int  ID_TEXT = 1;
const int  ID_LIST = 2;
WNDPROC oldProc;
long _stdcall ListProc(HWND hwnd, unsigned int message, unsigned int wParam, long lParam)//��д�б�򴰿ڹ���
{
	if (message == WM_KEYDOWN&&wParam == VK_RETURN)//���»س���
		SendMessage(GetParent(hwnd), WM_COMMAND, MAKELONG(ID_LIST, LBN_DBLCLK), (long)hwnd);//���õ�ͬ��˫����Ч��
	return CallWindowProc(oldProc, hwnd, message, wParam, lParam);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;		
	HDC hdc;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	int wmId, wmEvent;
	static HWND hText, hList;
	HINSTANCE hInst;
	TCHAR path[MAX_PATH];
	static TCHAR szBuffer[MAX_PATH];//�����ļ���
	static RECT rect;//��ʾ�ı�������
	HANDLE hFile;
	switch (UMsg)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
		hText = CreateWindow(TEXT("static"), TEXT(""), WS_CHILD | WS_VISIBLE,0,0,0,0,
			hwnd, (HMENU)ID_TEXT, hInst, nullptr);
		hList = CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("listbox"), TEXT(""), WS_CHILD | WS_VISIBLE | LBS_NOTIFY | LBS_SORT | WS_VSCROLL, cxChar, 3 * cyChar, 30 * cxChar, 10 * cyChar,
			hwnd,(HMENU)ID_LIST,hInst,nullptr);
		SendMessage(hList, LB_DIR, DDL_ARCHIVE | DDL_DIRECTORY | DDL_DRIVES | DDL_EXCLUSIVE |
			DDL_HIDDEN | DDL_POSTMSGS | DDL_READONLY | DDL_READWRITE | DDL_SYSTEM,(long) TEXT("*.*"));
		
		GetCurrentDirectory(MAX_PATH, path);
		SetWindowText(hText, path);
		rect.left = 35 * cxChar;//���б����ұ���2���ַ�
		rect.top = 3 * cyChar;//�߶����б��һ��
		oldProc = (WNDPROC)SetWindowLong(hList, GWL_WNDPROC,(long)ListProc);//�����б���µĴ��ڹ��̣����ؾɵĴ��ڹ���
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		rect.right = cxClient;
		rect.bottom = cyClient;
		MoveWindow(hText, cxChar, cyChar, cxClient - cxChar, cyChar, true);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		if (wmId == ID_LIST&&wmEvent == LBN_DBLCLK)
		{
			int index = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (index == LB_ERR)
				break;
			SendMessage(hList, LB_GETTEXT, index, (long)szBuffer);
			GetCurrentDirectory(MAX_PATH, path);//��õ�ǰĿ¼
			hFile = CreateFile(szBuffer, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, nullptr);
			if (hFile != INVALID_HANDLE_VALUE)//��ǰѡ�е��Ǹ��ļ�
			{
				CloseHandle(hFile);//�ر�����
				tstring str(path);//str��ǰ·��
				if (path[lstrlen(path )- 1] != '\\')//���������c:\����治�ټ�\,���������c\a���ں����1��
					str += TEXT("\\");
				str+= szBuffer;//���ϵ�ǰ�ļ���
				SetWindowText(hText, str.c_str());//��ʾ�ļ���·��
				InvalidateRect(hwnd, nullptr, true);//����WM_PAINT��Ϣ��������ʾһ�����ļ�����
			}
			else//����򿪵���Ŀ¼,��ȻҲ������һЩ�������ļ����ѱ��򿪣���ס�ˣ����ܱ���ȡ
			{
				WIN32_FIND_DATA wfd;
				if (FindFirstFile(szBuffer, &wfd) != INVALID_HANDLE_VALUE && (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)
				{
					MessageBeep(0);
					MessageBox(nullptr, TEXT("���ļ��ѱ��򿪣��������ѱ�������ֻ�йرպ���ܴ�"),TEXT( "����!!"), MB_ICONWARNING);
					break;//������ļ����ڣ����Ҳ���Ŀ¼��˵����һЩ�������ļ����ѱ��򿪣���ס�ˣ����ܱ���ȡ

				}
				//Ҫô�ļ������ڣ�����������Ҫô�ļ����ڣ���Ŀ¼
				szBuffer[lstrlen(szBuffer) - 1] = '\0';//�����1���ַ�]��Ϊ'\0'
				if (!SetCurrentDirectory(szBuffer + 1))//�������Ŀ¼������[file],���ӵ�2���ַ���ʼ���ַ�����Ϊ��ǰĿ¼
				{
					//���ò��ɹ���˵��ѡ�е��Ǹ�Ŀ¼����[-c-],[-d-]
					szBuffer[3] = ':';//��Ŀ¼��[-c-]֮��ĸ�Ŀ¼��Ϊ[-c:\���'\0'
					szBuffer[4] = '\\';
					szBuffer[5] = '\0';
					SetCurrentDirectory(szBuffer + 2);//��ÿ3���ַ���ʼ���ַ�����Ϊ��Ŀ¼
				}
				GetCurrentDirectory(MAX_PATH, path);//��ȡ��ǰĿ¼
				SetWindowText(hText, path);//��ʾ��Ŀ¼
				SendMessage(hList, LB_RESETCONTENT, 0, 0);//���б����������
				SendMessage(hList, LB_DIR, DDL_ARCHIVE | DDL_DIRECTORY | DDL_DRIVES | DDL_EXCLUSIVE |
					DDL_HIDDEN | DDL_POSTMSGS | DDL_READONLY | DDL_READWRITE | DDL_SYSTEM, (long)TEXT("*.*"));
				//���б���г���Ŀ¼���������Ե��ļ�
			}
		}
		break;
	case WM_SETFOCUS:
		SetFocus(hList);//�����б���ý���
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hFile = CreateFile(szBuffer, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);//��ȡ�ļ�
		if (hFile != INVALID_HANDLE_VALUE)//�򿪳ɹ�
		{
			TCHAR temp[8192];//���ֻ��ȡ8K�ֽ�����
			unsigned long i;
			ReadFile(hFile, temp, 8192, &i, nullptr);
			SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));//���õȿ�����
			SetTextColor(hdc, GetSysColor(COLOR_BTNTEXT));//���û�������Ϊ�ı�������ɫ
			SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));//���û�������Ϊ��ť������ɫ
			DrawText(hdc, temp, i, &rect, DT_WORDBREAK | DT_EXPANDTABS );//
			//DT_WORDBREAK	�Ͽ��֡���һ���е��ַ��������쵽��lpRectָ���ľ��εı߿�ʱ�������Զ�������֮��Ͽ���һ���س�һ����Ҳ��ʹ���۶ϡ�
			//DT_EXPANDTABS	��չ�Ʊ����ÿ���Ʊ����ȱʡ�ַ�����8,���û��������ԣ������Ʊ������С���κڵ�
			
			CloseHandle(hFile);//�ͷ��ļ�
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = TEXT("zeng");
	wndclass.lpszMenuName = nullptr;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("�»�"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}