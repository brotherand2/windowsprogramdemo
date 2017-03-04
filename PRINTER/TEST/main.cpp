#include<windows.h>
#include<memory>
#include<commctrl.h>
#include"resource.h"

using namespace std;
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
void GetTextType(unsigned char text[], TextCode &fileType)//�õ��ı���������
{

	if (text[0] == 0xff && text[1] == 0xfe)
		fileType = UNICODETEXT;//UNICODE����
	else
	{
		if (text[0] == 0xfe && text[1] == 0xff)
			fileType = UNICODEBIGENDIAN;//UNICODEBIGENDIAN����
		else
		{
			if (text[0] == 0xef && text[1] == 0xbb && text[2] == 0xbf)
				fileType = UTF8;//UTF-8����
			else
				fileType = ANSI;//ANSI����
		}
	}
}

bool MyReadFile(HWND hEdit, HWND hStatus, TCHAR szFileName[])//��ȡ�ļ�����
{
	HANDLE hFile;//�ļ����
	/*
	HANDLE CreateFile(
	LPCTSTR lpFileName,    // ָ���ļ�����ָ��
	DWORD dwDesiredAccess,    // ����ģʽ��д / ����
	DWORD dwShareMode,    // ����ģʽ
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, // ָ��ȫ���Ե�ָ��
	DWORD dwCreationDisposition,   // ��δ���
	DWORD dwFlagsAndAttributes,   // �ļ�����
	HANDLE hTemplateFile    // ���ڸ����ļ����
	);
	*/
	bool isConvert = false;//�Ƿ�ת�����ַ���
	hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	//OPEN_EXISTING �ļ������Ѿ�����,�Թ���������Ѵ����ļ����������Խ��ж�ȡ�ļ�
	if (hFile == INVALID_HANDLE_VALUE)//�ļ���ȡʧ��
		return false;
	int nFileLength = GetFileSize(hFile, nullptr);//�����ļ��Ĵ�С,�ֽ�,����������óɹ����򷵻�ֵΪ�ļ���С�ĵ�λ˫��
	unsigned char *pText = nullptr;//�����ȡ���ַ������ֽڣ���Ϊ�����ֽ���0-FF��Χ���������޷��ű���,����char ���ȡansi�ַ������⣬��Ϊansi�ַ�������128,��UNICODE�ַ�ֵ����128����255��unsigned char 
	//��ȡ��255(FF)����char ��ȡ-1(FFFFFFFF)
	TCHAR *pConvert = nullptr;//����ת������ַ���
	wchar_t *temp = nullptr;//��Ϊ�����õ���ʱ���ַ���
	unsigned long  nRealSize;//��ȡ�ļ���ʵ���Ĵ�С
	pText = new unsigned char[nFileLength + 2];//�����2���ռ����2��\0\0����Ϊ������UNICODE���룬UNICODE�ַ���������2��\0\0
	shared_ptr<unsigned char>sp1(pText, [](unsigned char *p){delete[]p; });//������Ŀռ佻����ָ�����	
	shared_ptr<TCHAR>sp2(pConvert, [](TCHAR *p){delete[]p; });//������Ŀռ佻����ָ�����
	shared_ptr<wchar_t>sp3(temp, [](wchar_t *p){delete[]p; });//������Ŀռ佻����ָ�����
	ReadFile(hFile, pText, nFileLength, &nRealSize, nullptr);//��ȡ�ļ�����
	pText[nFileLength] = pText[nFileLength + 1] = '\0';//���2���ַ�����\0
	TextCode  fileType;//�ļ���������
	GetTextType(pText, fileType);//�õ��ı���������
	int len;//ת������ַ�����
	unsigned char cc;//�ַ�����ʱ�õ���ʱ�ַ�
	switch (fileType)
	{
	case ANSI://ansi�����ļ�
#ifdef UNICODE//�����UNICODE����
		len = MultiByteToWideChar(CP_ACP, 0, (char *)pText, -1, nullptr, 0);//�����ֽ��ַ�ת���ɿ��ַ���Ҫ���ַ�����,�������ַ�
		pConvert = new TCHAR[len];
		MultiByteToWideChar(CP_ACP, 0, (char *)pText, -1, (wchar_t *)pConvert, nFileLength + 1);//�����ֽ��ַ�ת���ɿ��ַ�
		isConvert = true;//ת����
#endif
		SendMessage(hStatus, SB_SETTEXT, 4, (long)TEXT("ANSI"));//�ڵ�������ʾ��������
		break;
	case UNICODETEXT://UNICODE�����ļ�
		SendMessage(hStatus, SB_SETTEXT, 4, (long)TEXT("UNICODE"));//�ڵ�������ʾ��������
		pText += 2;//����2���ַ������ַ����FF FE
		nFileLength -= 2;//������FF FE��2���ַ�����
#ifndef UNICODE//�������UNICODE����
		len = WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pText, -1, nullptr, 0, nullptr, nullptr);//���Ҫת�����ַ�����������\0
		pConvert = new TCHAR[len];
		WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pText, -1, (char *)pConvert, len, nullptr, nullptr);
		isConvert = true;//ת����
#endif
		break;
	case UTF8://UTF8���룬������ֻ��BOM(���ֽڱ�־)��UTF8�ļ�ת������Ч
		SendMessage(hStatus, SB_SETTEXT, 4, (long)TEXT("UTF-8"));//�ڵ�������ʾ��������
		//UTF8��ת��Ҫ��ת��UNICODE����ΪMultiByteToWideChar���Խ�UTF8תUNICODE�������ANSI�����ٽ�UNICODE�����ַ���תANSI
		pText += 3;//����3���ַ������ַ����EF BB BF
		nFileLength -= 3;//������EF BB BF��3���ַ�����
		len = MultiByteToWideChar(CP_UTF8, 0, (char *)pText, -1, nullptr, 0);//��UTF8�ַ�ת���ɿ��ַ���Ҫ���ַ�����,�������ַ�
		temp = new wchar_t[len];
		MultiByteToWideChar(CP_UTF8, 0, (char *)pText, -1, (wchar_t *)temp, nFileLength + 1);//��UTF8�ַ�ת���ɿ��ַ�
		isConvert = true;//ת����
#ifdef UNICODE//�����UNICODE����
		pConvert = new TCHAR[len + 1];//���ý���ת�����ֽڣ�ֱ�ӿ���
		lstrcpy((PTSTR)pConvert, (PTSTR)temp);
#else//����,�ٽ�UNICODE�ַ���ת��ANSI�ַ���
		len = WideCharToMultiByte(CP_ACP, 0, (wchar_t *)temp, -1, nullptr, 0, nullptr, nullptr);//���Ҫת�����ַ�����������\0
		pConvert = new TCHAR[len];//����len���ȵĿռ�
		WideCharToMultiByte(CP_ACP, 0, (wchar_t *)temp, -1, (char *)pConvert, len, nullptr, nullptr);//�����ַ�ת���ɶ��ֽ��ַ�
#endif
		break;
	case UNICODEBIGENDIAN:
		SendMessage(hStatus, SB_SETTEXT, 4, (long)TEXT("unicode big endian"));//�ڵ�������ʾ��������
		pText += 2;//����2���ַ������ַ���� FE FF
		nFileLength -= 2;//������ FE FF��2���ַ�����
		for (int i = 0; i < nFileLength / 2; i++)//��ÿ��˫�ֵĵ��ֽ�����ֽڽ���
		{
			//֮����Ҫ���н�������Ϊ�����Ч�ֽ����ȣ���Macintosh���������ַ�'A',��0x0041,�����ڴ��и��ֽ����ȣ������ǰ�0x00��0x41������˳��洢
			//��UNICODE�ǰ��� ����Ч�ֽ�����
			cc = pText[2 * i];
			pText[2 * i] = pText[2 * i + 1];
			pText[2 * i + 1] = cc;
		}
#ifdef UNICODE//�����UNICODE����
#else//����
		len = WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pText, -1, nullptr, 0, nullptr, nullptr);//���Ҫת�����ַ�����������\0
		pConvert = new TCHAR[len];//����len���ȵĿռ�
		WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pText, -1, (char *)pConvert, len, nullptr, nullptr);//�����ַ�ת���ɶ��ֽ��ַ�
		isConvert = true;//ת����
#endif
		break;
	}
	if (isConvert)//ת���ˣ���ת������ַ����Ž��༭��
		SetWindowText(hEdit, (LPCTSTR)pConvert);//LPCSTR�����Ƿ�UNICODE ת���const char*��const wchar *
	else//��δת�����ַ����Ž��༭��
		SetWindowText(hEdit, (LPCTSTR)pText);//LPCSTR�����Ƿ�UNICODE ת���const char*��const wchar *
	CloseHandle(hFile);//��ȡ�ļ���ر��ļ����
	return true;
}
static WNDPROC oldHelpProc;

int _stdcall EditHelpProc(HWND hEdit, unsigned message, unsigned wParam, long lPram)//��д�༭�ؼ��Ի������
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		//MessageBox(0, TEXT("your key"), 0, 0);
		break;

	}
	return CallWindowProc(oldHelpProc, hEdit, message, wParam, lPram);//����Ĭ�ϵı༭�ؼ����ڴ�����̴���������Ϣ��δ������Ĳ���
}
int _stdcall DlgHelpProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)
{
	HWND hEdit;//�༭��
	switch (message)
	{
	case WM_INITDIALOG://��ʼ�Ի���//E:\\mfc���\\PRINTER\\TEST\\ /
		hEdit = GetDlgItem(hDlg, IDC_EDITGOTO);//�õ������༭����

		MyReadFile(hEdit, 0, TEXT("help.txt"));//��ȡ�����ļ������ݵ��༭��
		//SetFocus(hEdit);
		//oldHelpProc = (WNDPROC)SetWindowLong(hEdit, GWL_WNDPROC,(long) EditHelpProc);
		return true;
	case WM_CLOSE:
		EndDialog(hDlg, 0);//�ر�ʱ���ٴ���
		return true;
	}
	return false;
}
int WinMain(HINSTANCE hInst, HINSTANCE hPreInst, char *command, int show)
{
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGGOTO), 0, DlgHelpProc);
}