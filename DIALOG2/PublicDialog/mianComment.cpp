//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif

#include <Windows.h>
//#include <commdlg.h>
#include"resource.h"

#include<string>
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
int DialogProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)
{
	int wmId = LOWORD(wParam);//�ؼ�ID
	int wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
	switch (message)
	{
		//case WM_COMMAND:

		//	return false;
	case WM_CLOSE:
		EndDialog(hDlg, 0);//���ٶԻ���
		return false;
	}
	return false;//���û�д�������0,�����Ĭ�϶Ի�����̣���0�����ˣ�������Ĭ�ϵ�
}
void InitOpenFileName(OPENFILENAME &ofn,HWND hwnd,TCHAR szFileName[],TCHAR szTitle[])
{
	static TCHAR szFilter[] = TEXT("�ı��ļ�(*.txt)\0*.txt\0����ļ�(*.c;*.cpp;*.java)\0*.c;*.cpp;*.java\0�����ļ�(*.*)\0*.*\0\0");//�������ַ���
	ofn.lStructSize = sizeof(ofn);//�ṹ�Ĵ�С
	ofn.hwndOwner = hwnd;//ָ�������߶Ի��򴰿ڵľ���������Ա������������Ч���ھ����������Ի���û��������������ΪNULL��
	ofn.hInstance = nullptr;
	ofn.lpstrFilter = szFilter;//ָ��һ���Կ��ַ������Ĺ����ַ�����һ�����塣�����е����һ���ַ�������������NULL�ַ�����
	ofn.lpstrCustomFilter = nullptr;//ָ��һ����̬���壬��������һ���Կ��ַ������Ĺ������ַ���������ַ�����Ϊ�˱����û�ѡ��Ĺ�����ʽ
	ofn.nMaxCustFilter = 0;
	//ָ������ΪlpstrCustomFilter׼������TCHARsΪ��λ�Ļ����С������ANSI�汾�����ֽڵĸ���������Unicode�汾�����ַ��ĸ������⻺��Ӧ����С��40���ַ��������lpstrCustomFilter��Ա��NULL����ָ��NULL���ַ����������Ա������
	ofn.nFilterIndex = 2;//��ʾ�ļ����͵ĵ�2��������������ļ�
	//ָ�����ļ����Ϳؼ��е�ǰѡ��Ĺ�����������������ָ��lpstrFilter������һ�Զ����˵Ĺ��������ַ������������ĵ�һ���ַ���������ֵΪ1���ڶ���Ϊ2���ȵ�
	ofn.lpstrFile = szFileName;//��GetOpenFileName��GetSaveFileName�������سɹ�ʱ��������������������·�����ļ���������ѡ����ļ�����չ��
	ofn.nMaxFile = MAX_PATH;//ָ��lpstrFile����Ĵ�С
	ofn.lpstrFileTitle = szTitle;//ָ�����ѡ����ļ����ļ�������չ���Ļ��壨����·����Ϣ���������Ա������NULL
	ofn.nMaxFileTitle = MAX_PATH;//ָ��lpstrFileTitle����Ĵ�С
	ofn.lpstrInitialDir = TEXT("e:");//ָ���Կ��ַ��������ַ���������������ַ�����ָ����ʼĿ¼
	ofn.lpstrTitle = TEXT("�ף�ѡһ���ļ���");//ָ���ڶԻ���ı������з��õ��ַ�������������Ա��NULL��ϵͳʹ��Ĭ�ϱ��⣨���Ϊ���)
	ofn.Flags = OFN_CREATEPROMPT | OFN_HIDEREADONLY;//λ��ǵ����ã������ʹ������ʼ���Ի���
	//OFN_ALLOWMULTISELECT ָ���ļ����б�������ѡ,OFN_HIDEREADONLY ����ֻ����ѡ��
	// OFN_CREATEPROMPT����û�ָ����һ�������ڵ��ļ���������ʹ�öԻ�������ʾ�û��Ƿ��½�����ļ�
	ofn.nFileOffset = 0;//����ָ����·����ʼ��ͨ��lpstrFileָ�����ļ����ַ�������0��ƫ�ƣ��ļ�����1����ĸ��·����ƫ��
	ofn.nFileExtension = 0;//ָ����·����ʼ��ͨ��lpstrFileָ�����ļ����ַ�������չ������0��ƫ��
	ofn.lpstrDefExt = TEXT("cpp");//ָ�����Ĭ����չ���Ļ���
	ofn.lCustData = 0;//ָ��Ӧ�ó���������ݣ����������ܱ�lpfnHook��Աʶ���ϵͳ�����Ĺ��ӳ���
	ofn.lpfnHook = 0;//ָ��һ�����ӳ��򡣳���Flags��Ա�а���OFN_ENABLEHOOK��ǣ�Ҫô�����Ա��������
	ofn.lpTemplateName = 0;//ָ��һ���Կ��ַ��������ַ������ַ����ǶԻ���ģ����Դ�����֣���Դ�������ܱ�hInstance��Աʶ���ģ����

}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	TCHAR szFileName[MAX_PATH]=TEXT("");//�����ļ�·���������ļ���
	//��ʼ���ļ����༭�ؼ�ʹ�õ��ļ����Ļ��塣�������Ҫ��ʼֵ���������ĵ�һ���ַ�������NULL
	TCHAR szTitle[MAX_PATH];//�����ļ���������·��
	static OPENFILENAME ofn;//������ʼ���򿪻����Ϊ�Ի������Ϣ
	static CHOOSEFONT cf;//����ṹ
	static CHOOSECOLOR cc;//��ɫ�ṹ
	LOGFONT lf;//�߼�����
	HFONT hFont;//������
	TCHAR szBuffer[] = TEXT("This a test line �й���---������");//�����õ��ַ���
	static COLORREF color[16];
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
	   	hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����

		switch (wmId)
		{
		case ID_FILE_OPEN://�ļ���
			InitOpenFileName(ofn, hwnd, szFileName, szTitle);
			if (GetOpenFileName(&ofn))//����һ��Open�����Ի���ʹ�û�ָ����������Ŀ¼���ļ�������ʹ�û����ļ�,����û�ָ����һ���ļ��������OK��ť������ֵΪ����
				MessageBox(hwnd, szFileName, TEXT("��ѡ�����:"), MB_OK);
			return 0;
		case ID_FILE_SAVE://�ļ�����
			InitOpenFileName(ofn, hwnd, szFileName, szTitle);
			ofn.Flags = OFN_OVERWRITEPROMPT;//���ѡ����ļ��Ѿ����ڣ�ʹ�����Ϊ�Ի������һ����Ϣ���û�����ȷ���Ƿ񸲸�����ļ���
			if (GetSaveFileName(&ofn))//����һ��Save�����Ի����Ա��û�ָ����������Ŀ¼���ļ���
				MessageBox(hwnd, szFileName, TEXT("��ѡ�񱣴��·����:"), 0);
			return 0;
		case ID_FILE_SAVEAS://�ļ����Ϊ

			return 0;
		case ID_FILE_COLOR://�ļ���ɫ
			cc.lStructSize = sizeof(cc);//ָ���ṹ�ĳ��ȣ��ֽ�
			cc.Flags = CC_FULLOPEN | CC_RGBINIT ;//һ�����������ʼ����ɫ�Ի����λ�������Ի��򷵻�ʱ����������Щ��ʶ������ʶ�û�������
			//CC_FULLOPEN�öԻ�����ʾ����Ŀؼ���ʹ�û������Զ������ɫ
			//CC_RGBINIT�öԻ���Ĭ��ʹ����rgbResult��Աָ������ɫ
			cc.hInstance = nullptr;
			cc.hwndOwner = hwnd;//ӵ�жԻ���Ĵ��ڵľ�����ó�Ա������������Ч�Ĵ��ھ�������ڶԻ���û��������ʱ����ΪNULL
			cc.lCustData = 0;//ָ��Ӧ�ó����Զ�������ݣ������ݻᱻϵͳ���͸����ӳ���
			cc.lpfnHook = 0;//ָ��CCHookProc���ӳ����ָ�룬�ù��ӿ��Դ����͸��Ի������Ϣ
			cc.lpTemplateName = 0;//ָ��һ��NULL��β���ַ��������ַ����ǶԻ���ģ����Դ������
			cc.rgbResult = RGB(255, 0, 0);//���CC_RGBINIT��ʶ��������ʱ���ó�Աָ���˶Ի����ʱĬ�ϵ�ѡ����ɫ
			cc.lpCustColors = color;//ָ��һ������16��ֵ�����飬����������˶Ի������Զ�����ɫ�ĺ졢�̡�����RGB��ֵ
			if (ChooseColor(&cc))//�ú�������һ����ʹ�û�����ѡ����ɫ��ͨ����ɫ�Ի���
			{
				DeleteObject((HGDIOBJ)SetClassLong(hwnd,GCL_HBRBACKGROUND,(long)CreateSolidBrush(cc.rgbResult)));//�����µ���ɫ�����µĻ�ˢ�����뱳����ˢ����ɾ���ɵĻ�ˢ
				InvalidateRect(hwnd, nullptr, true);//�ô����ñ����ػ�
			};
			return 0;
		case ID_FILE_FONT://�ļ�����
			cf.Flags = CF_EFFECTS | CF_NOVERTFONTS;//ʹ�Ի�����ʾ�ؼ������û�ָ���»���,���ı���ɫѡ� ������ô˱�־,������ʹ��rgbColors��Աָ����ʼ�ı���ɫ
			// CF_NOVERTFONTS��������Ի����б�ֻ����ˮƽ������,��δ�������Ǵ�ֱ��
			cf.hDC = nullptr;
			cf.hInstance = nullptr;
			cf.hwndOwner = hwnd;
			cf.iPointSize = 0;//ѡ������Ĵ�С
			cf.lCustData = 0;
			cf.lpfnHook = 0;
			cf.lpLogFont = &lf;//�߼�����
			cf.lStructSize = sizeof(cf);
			cf.lpszStyle = 0;//������
			cf.lpTemplateName = 0;
			cf.nFontType = 0;//���ص���������
			cf.nSizeMax = 0;//�û�����ѡ�������С
			cf.nSizeMin = 0;// �û�����ѡ����С���С
			cf.rgbColors = RGB(255, 0, 0);//��ʼ�ı���ɫ
			if (ChooseFont(&cf))
			{
				hFont = CreateFontIndirect(&lf);//���ݴ�������
				hdc = GetDC(hwnd);
				hFont=(HFONT)SelectObject(hdc, hFont);//������ѡ�뻭��
				TextOut(hdc, 50, 20, szBuffer, lstrlen(szBuffer));
				hFont = (HFONT)SelectObject(hdc, hFont);//���ɻ��ʷŻ�ȥ
				DeleteObject(hFont);//ɾ������ �Ļ���
				DeleteDC(hdc);
			}
			return 0;

		case ID_APP_EXIT://�����˳�
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;
		}
		break;
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���

		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		break;
	case WM_CLOSE://���ڹر�
		DestroyWindow(hwnd);//���ٴ���
		break;
	case WM_DESTROY://��������ʱ
		PostQuitMessage(0);//�׳�һ������ֵΪ0�˳���Ϣ
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//��Ĭ�ϵĴ��ڹ��̴���������Ϣ����Щ��������
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	WNDCLASS wndclass;//������
	wndclass.cbClsExtra = 0;//Ϊ�����Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.cbWndExtra = 0;//Ϊ���ڷ���Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//������ˢ
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//�����
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);//ͼ��
	wndclass.hInstance = hInstance;//ʵ�������������PID��һ���������������������ϵͳ�����ҵ����̵ĵ�ַ
	wndclass.lpfnWndProc = WndProc;//���ڹ���
	wndclass.lpszClassName = TEXT("zeng");//����
	wndclass.lpszMenuName =MAKEINTRESOURCE(IDR_MENU1);//�˵�
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

	while (GetMessage(&msg, nullptr, 0, 0))//����Ϣ���л�ȡ��Ϣ
	{
		TranslateMessage(&msg);//��һЩ������Ϣת�����ַ���Ϣ���ڰ�����Ϣ����
		DispatchMessage(&msg);//�ַ���Ϣ�������ô��ڹ��̽��д�����Ϣ
	}
}