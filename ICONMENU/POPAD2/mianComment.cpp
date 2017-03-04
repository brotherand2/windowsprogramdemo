//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include"resource.h"
using namespace std;
//#ifndef UNICODE 
//typedef  string tstring;
//#else
//typedef  wstring tstring;
//#endif
#ifndef UNICODE //���û�ж���UNICODE ,��tstring�滻Ϊstring,�����滻Ϊwstring
using  tstring = string;
#else
using  tstring = wstring;
#endif
int AskConfirmation()//�����˳�ʱѯ���Ƿ��˳�
{
	return	MessageBox(nullptr, TEXT("Really want to quit Notepad?"), TEXT("Quit"), MB_ICONQUESTION|MB_YESNOCANCEL);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HWND hEdit;//�༭��
	static HINSTANCE hInst;//����ʵ�����

	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE|WS_VSCROLL|ES_MULTILINE, 0, 0, 0, 0,
			hwnd, (HMENU)ID_EDIT, hInst, nullptr);//�����б߽�Ķ��б༭��
		
		return 0;
	case WM_SETFOCUS://���ڻ�ý���ʱ
		SetFocus(hEdit);//�ñ༭���ý���
		return 0;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		MoveWindow(hEdit, 0, 0, cxClient, cyClient, true);//�����༭��СΪ�����ͻ���
		return 0;
	case WM_INITMENUPOPUP://�˵�����ʱ����Ϣ���ʼ����Żᵯ��
		if (LOWORD(lParam) == 1)//�������Ϊ1�����ǵ����༭�˵�
		{//wParam�����ǵ���ʽ�˵��ľ����lParam�ĵ�λ�ǵ���ʽ�˵�������
			//����ò˵���ϵͳ�˵�����ô��λ��1������Ϊ0
			EnableMenuItem((HMENU)wParam, IDM_EDIT_UNDO, SendMessage(hEdit, EM_CANUNDO, 0, 0) ? MF_ENABLED : MF_GRAYED);
			//�ȷ���һ����Ϣ���༭������ɳ����������˵�����ã����򣬲�����
			EnableMenuItem((HMENU)wParam, IDM_EDIT_PASTE, IsClipboardFormatAvailable(CF_TEXT) ? MF_ENABLED : MF_GRAYED);
			//��������������֣�����в˵����ã�������Ϊ��ɫ
			long isSelected = SendMessage(hEdit, EM_GETSEL, 0, 0);//��õ�ǰ��ѡ���ֵĿ�ʼ�ͽ���λ��
			int isEnable = (LOWORD(isSelected) == HIWORD(isSelected));//isSelected��λ�����ſ�ʼλ�ã���λ�������λ�ã������ʼ����λ�����˵��δѡ��
			if (isEnable)//��ʼ����λ����ȣ�˵��δѡ������
				isEnable = MF_GRAYED;
			else
				isEnable = MF_ENABLED;
			EnableMenuItem((HMENU)wParam, IDM_EDIT_COPY, isEnable);
			EnableMenuItem((HMENU)wParam, IDM_EDIT_CUT, isEnable);
			EnableMenuItem((HMENU)wParam, IDM_EDIT_CLEAR, isEnable);
		}
		return 0;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		if (wmId == ID_EDIT&&wmEvent == EN_ERRSPACE || wmEvent == EN_MAXTEXT)//����༭���ַ�����30000���ַ�
		{
			MessageBox(nullptr, TEXT("Edit control out of space"), TEXT("Error"), MB_ICONSTOP);
			return 0;
		}
		switch (wmId)
		{
		case IDM_FILE_NEW ://�½��ļ�
		case IDM_FILE_OPEN ://���ļ�
		case IDM_FILE_SAVE ://�����ļ�
		case IDM_FILE_SAVEAS ://���Ϊ
		case IDM_FILE_PAGESET ://ҳ������
		case IDM_FILE_PRINT ://��ӡ
			MessageBeep(0);
			return 0;
		case IDM_APP_EXIT ://�˳�
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;

		case IDM_EDIT_FIND: //����
		case IDM_EDIT_FINDNEXT://������һ��
		case IDM_EDIT_REPLACE ://�滻
		case IDM_EDIT_GOTO: //ת��
		case IDM_EDIT_TIMEDATE://ʱ��/����
			MessageBeep(0);
			return 0;
		case IDM_EDIT_SELECTALL://ȫѡ
			SendMessage(hEdit, EM_SETSEL, 0, -1);//0��ʾ�ӵ�1���ַ���ʼ��-1��ʾ�����1���ַ�
			return 0;//������Ϣ�ñ༭��Ĭ�ϵĴ��ڹ��̴���
		case IDM_EDIT_UNDO :  //����
			SendMessage(hEdit, WM_UNDO, 0, 0);//������Ϣ�ñ༭��Ĭ�ϵĴ��ڹ��̴���
			return 0;
		case IDM_EDIT_CUT ://����
			SendMessage(hEdit, WM_CUT, 0, 0);//������Ϣ�ñ༭��Ĭ�ϵĴ��ڹ��̴���
			return 0;
		case IDM_EDIT_COPY :  //����
			SendMessage(hEdit, WM_COPY, 0, 0);//������Ϣ�ñ༭��Ĭ�ϵĴ��ڹ��̴���
			return 0;
		case IDM_EDIT_PASTE:  //ճ��
			SendMessage(hEdit, WM_PASTE, 0, 0);//������Ϣ�ñ༭��Ĭ�ϵĴ��ڹ��̴���
			return 0;
		case IDM_EDIT_CLEAR://ɾ��ѡ��   δ����ɾ������
			SendMessage(hEdit, WM_CLEAR, 0, 0);
			return 0;
		case IDM_FORMAT_AUTOLINE://�Զ�����
		case IDM_FORMAT_FONT://����
		case IDM_LOOKUP_STATE://״̬��
			MessageBeep(0);
			return 0;
		case IDM_APP_HELP://����
			MessageBox(nullptr, TEXT("Notepad Help not yet implement"), TEXT("help and support"), MB_OK);
			return 0;
		case IDM_APP_ABOUT: //����
			MessageBox(nullptr, TEXT("Notepad Program\n(c)yong hua by,2015"), TEXT("����Menu"), MB_ICONINFORMATION | MB_OK);
			return 0;
		}
		return 0;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���

		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		return 0;
	case WM_QUERYENDSESSION://�ػ�ʱѯ���Ƿ��˳�����ȷ�����˳��ػ��������ȡ���ػ�
		if (IDYES == AskConfirmation())//���ڹر�ʱѯ���Ƿ��˳�����ȷ�����˳�
			return 1;//�������Ի�����һ���������ϵͳ�رչ��ܵ�ʱ��WM_QUERYENDSESSION��Ϣ�ᱻ���͸���δ��ֹ�����д��ڡ��������ڴ��������Ϣ��ʱ�����������false��0������ôϵͳ���������Ի����߹ػ���ע����
		else
			return 0;
	case WM_CLOSE://���ڹر�
		if (IDYES == AskConfirmation())//���ڹر�ʱѯ���Ƿ��˳�����ȷ�����˳�
		DestroyWindow(hwnd);//���ٴ���
		return 0;
	case WM_DESTROY://��������ʱ
		PostQuitMessage(0);//�׳�һ������ֵΪ0�˳���Ϣ
		return 0;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//��Ĭ�ϵĴ��ڹ��̴���������Ϣ����Щ��������
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	HACCEL hAccel;//���ټ�����ݼ������
	WNDCLASS wndclass;//������
	wndclass.cbClsExtra = 0;//Ϊ�����Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.cbWndExtra = 0;//Ϊ���ڷ���Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//������ˢ
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//�����
	wndclass.hIcon = LoadIcon(hInstance,TEXT("POPAD"));//ͼ��
	wndclass.hInstance = hInstance;//ʵ�������������PID��һ���������������������ϵͳ�����ҵ����̵ĵ�ַ
	wndclass.lpfnWndProc = WndProc;//���ڹ���
	wndclass.lpszClassName = TEXT("zeng");//����
	wndclass.lpszMenuName = TEXT("POPADMENU");//�˵�
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//��Щ�ı��Ҫ�ػ�������ˮƽ�ʹ�ֱ�ı���ػ�
	if (!RegisterClass(&wndclass))//ע�ᴰ����
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("�»�"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);//��������
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	ShowWindow(hwnd, nCmdShow);//��ʾ����
	UpdateWindow(hwnd);//�����ػ�
	MSG msg;//��Ϣ�ṹ
	hAccel = LoadAccelerators(hInstance, TEXT("POPAD"));//����Դ�ļ���ȡ���ټ����
	while (GetMessage(&msg, nullptr, 0, 0))//����Ϣ���л�ȡ��Ϣ
	{
		if (!TranslateAccelerator(hwnd, hAccel, &msg))
		{//TranslateAccelerator���������ܣ�������ټ����ú�������˵������еļ��ټ���
			//�ú�����һ��WM_KEYDOWN��WM_SYSKEYDOWN��Ϣ�����һ��WM_COMMAND��WM_SYSCOMMAND��Ϣ������ڸ����ļ��ټ������иü�����ڣ���
			//Ȼ��WM_COMMAND��WM_SYSCOMMAND��Ϣֱ���͵���Ӧ�Ĵ��ڴ������
			//��TransLateAccelerator���ط���ֵ����Ϣ�ѱ����룬˵���Ӽ��ٽݱ����ҵ��˶�Ӧ�İ�����ֱ�ӵ��ô��ڹ���
			//ֱ�����ڹ��̴�������Ϣ��ŷ��أ�Ӧ�ó���Ͳ��ܵ���TranslateMessage��������Ϣ����������û�ҵ��������룬����0
			TranslateMessage(&msg);//��һЩ������Ϣת�����ַ���Ϣ���ڰ�����Ϣ����
			DispatchMessage(&msg);//�ַ���Ϣ�������ô��ڹ��̽��д�����Ϣ
		}
	}
}