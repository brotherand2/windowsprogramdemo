/*
����1��ֻ��ʶ��BOM���ֽ�˳���־���ı���
����2�����ļ��Ƚϴ�ʱ����С����Ҫ����10�������ʾ���֣����ļ��ٶ����Ժ���������EDITPLUS���ܶ�

������ö��ˣ��򿪴��ļ�ʱ���ֹ�������ʱ�����±�ȴ����
����3�����ټ���ͻ  �����ѽ��
*/
#include<Windows.h>
#include<string>
#include<sstream>
#include<memory>
#include<algorithm>
#include<commctrl.h>
#include<tchar.h>
#include<strsafe.h>
#include"resource.h"
int AskConfirmation()//�����˳�ʱѯ���Ƿ��˳�
{
	return	MessageBox(nullptr, TEXT("�����Ҫ�˳����±���?"), TEXT("Quit"), MB_ICONQUESTION|MB_YESNO);
}
int AskAboutSave(HWND hwnd,TCHAR szFileName[])//ѯ���Ƿ��Ǳ����ļ�
{
	TCHAR szBuffer[64 + MAX_PATH];//����Ϊ��ʾ����+���ⳤ��
	StringCchPrintf(szBuffer, 64 + MAX_PATH, TEXT("�Ƿ񽫸��ı��浽 %s?"),szFileName[0] ? szFileName : unTitled);//���и�ʽ���ַ���
	//�������Ϊ�գ�������ʾ���ޱ��⡱	
	int iReturn =MessageBox(hwnd, szBuffer, szAppName, MB_YESNOCANCEL | MB_ICONQUESTION);//�����Ƿ�Ҫ����
	if (iReturn == IDYES)//���Ҫ����
	{
		if (!SendMessage(hwnd, WM_COMMAND, IDM_FILE_SAVE, 0))//�������Ϊ
			iReturn = IDCANCEL;//���Ϊ�Ի���ѡ������ȡ����ر�
	};
	return iReturn;
}
void OKMessage(HWND hwnd,TCHAR szMessage[], TCHAR szTitle[])//����һ��OK��ʾ��,����Ϊ��ʾ+����
{
	TCHAR szBuffer[64 + MAX_PATH];//����Ϊ��ʾ����+���ⳤ��
	StringCchPrintf(szBuffer, 64 + MAX_PATH,szMessage, szTitle[0]?szTitle:unTitled);//���и�ʽ���ַ���
	//�������Ϊ�գ�������ʾ���ޱ��⡱
	MessageBox(hwnd, szBuffer, szAppName, MB_OK | MB_ICONEXCLAMATION);//������Ϣ��
}
void InitOpenFileName(OPENFILENAME &ofn,HWND hwnd)//��ʼ���ļ��ṹ
{	
	static TCHAR szFilter[] = TEXT("�ı��ļ�(*.txt)\0*.txt\0����ļ�(*.cpp;*.c;*.java)\0*.cpp;*.c;*.java\0�����ļ�(*.*)\0*.*\0\0");//�������ַ���
	ofn.lStructSize = sizeof(ofn);//�ṹ�Ĵ�С
	ofn.hwndOwner = hwnd;//ָ�������߶Ի��򴰿ڵľ���������Ա������������Ч���ھ����������Ի���û��������������ΪNULL��
	ofn.hInstance = nullptr;
	ofn.lpstrFilter = szFilter;//ָ��һ���Կ��ַ������Ĺ����ַ�����һ�����塣�����е����һ���ַ�������������NULL�ַ�����
	ofn.lpstrCustomFilter = nullptr;//ָ��һ����̬���壬��������һ���Կ��ַ������Ĺ������ַ���������ַ�����Ϊ�˱����û�ѡ��Ĺ�����ʽ
	ofn.nMaxCustFilter = 0;
	//ָ������ΪlpstrCustomFilter׼������TCHARsΪ��λ�Ļ����С������ANSI�汾�����ֽڵĸ���������Unicode�汾�����ַ��ĸ������⻺��Ӧ����С��40���ַ��������lpstrCustomFilter��Ա��NULL����ָ��NULL���ַ����������Ա������
	ofn.nFilterIndex = 2;//��ʾ�ļ����͵ĵ�2��������������ļ�
	//ָ�����ļ����Ϳؼ��е�ǰѡ��Ĺ�����������������ָ��lpstrFilter������һ�Զ����˵Ĺ��������ַ������������ĵ�һ���ַ���������ֵΪ1���ڶ���Ϊ2���ȵ�
	ofn.lpstrFile = nullptr;//��GetOpenFileName��GetSaveFileName�������سɹ�ʱ��������������������·�����ļ���������ѡ����ļ�����չ��
	ofn.nMaxFile = MAX_PATH;//ָ��lpstrFile����Ĵ�С
	ofn.lpstrFileTitle = nullptr;//ָ�����ѡ����ļ����ļ�������չ���Ļ��壨����·����Ϣ���������Ա������NULL
	ofn.nMaxFileTitle = MAX_PATH;//ָ��lpstrFileTitle����Ĵ�С
	ofn.Flags = 0;//λ��ǵ����ã������ʹ������ʼ���Ի���
	ofn.lpstrInitialDir = TEXT("");//ָ���Կ��ַ��������ַ���������������ַ�����ָ����ʼĿ¼
	ofn.lpstrTitle = nullptr;//ָ���ڶԻ���ı������з��õ��ַ�������������Ա��NULL��ϵͳʹ��Ĭ�ϱ��⣨���Ϊ���)
	ofn.nFileOffset = 0;//����ָ����·����ʼ��ͨ��lpstrFileָ�����ļ����ַ�������0��ƫ�ƣ��ļ�����1����ĸ��·����ƫ��
	ofn.nFileExtension = 0;//ָ����·����ʼ��ͨ��lpstrFileָ�����ļ����ַ�������չ������0��ƫ��
	ofn.lpstrDefExt =TEXT("");//ָ�����Ĭ����չ���Ļ���
	ofn.lCustData = 0;//ָ��Ӧ�ó���������ݣ����������ܱ�lpfnHook��Աʶ���ϵͳ�����Ĺ��ӳ���
	ofn.lpfnHook = 0;//ָ��һ�����ӳ��򡣳���Flags��Ա�а���OFN_ENABLEHOOK��ǣ�Ҫô�����Ա��������
	ofn.lpTemplateName = 0;//ָ��һ���Կ��ַ��������ַ������ַ����ǶԻ���ģ����Դ�����֣���Դ�������ܱ�hInstance��Աʶ���ģ����
}
HWND FindDialog(HWND hwnd,TCHAR szFindText[])//���ҽṹ
{
	static FINDREPLACE fr;//�������Ϊ��̬�ģ���Ϊ����FindText,���ҶԻ�����ʾ��FindText������������,FindDialog����ִ������ͷžֲ�����fr,fr�ĵ�ַ�Ŀռ�����٣����ĶԻ�����Ҫ�Ľṹfr�����ˣ��������
	//��ʼ�������滻�ṹ
	fr.Flags = FR_DOWN | FR_HIDEWHOLEWORD;//��ʼ�������滻�Ի���ı��
	//FR_DOWN��ʼ�����°�ť�����£�FR_HIDEWHOLEWORD����ʾȫ��ƥ��
	fr.hInstance = nullptr;
	fr.hwndOwner=hwnd;//ӵ�жԻ���Ĵ��ھ��
	fr.lCustData = 0;//�û����������
	fr.lpfnHook = 0;
	fr.lpstrFindWhat =szFindText;//Ҫ���ҵ��ַ���
	fr.lpstrReplaceWith = 0;//Ҫ�滻���ַ���
	fr.lpTemplateName = 0;
	fr.lStructSize = sizeof(fr);//�ṹ��С
	fr.wFindWhatLen = MAX_PATH;//Ҫ���ҵ��ַ���szFindText�Ļ������ռ�
	fr.wReplaceWithLen = 0;//Ҫ�滻���ַ���szFindText�Ļ������ռ�
	pfr = &fr;//��pfr��ָ��fr
	return FindText(&fr);//���в���
}
HWND ReplaceDialog(HWND hwnd, TCHAR szFindText[],TCHAR szReplaceText[])//�滻�ṹ
{
	static FINDREPLACE fr;//�������Ϊ��̬�ģ���Ϊ����ReplaceText,�滻�Ի�����ʾ��ReplaceText������������,ReplaceDialog����ִ������ͷžֲ�����fr,fr�ĵ�ַ�Ŀռ�����٣����ĶԻ�����Ҫ�Ľṹfr�����ˣ��������
	//��ʼ�������滻�ṹ
	fr.Flags = FR_DOWN | FR_HIDEWHOLEWORD;//��ʼ�������滻�Ի���ı��
	fr.hInstance = nullptr;
	fr.hwndOwner = hwnd;//ӵ�жԻ���Ĵ��ھ��
	fr.lCustData = 0;//�û����������
	fr.lpfnHook = 0;
	fr.lpstrFindWhat = szFindText;//Ҫ���ҵ��ַ���
	fr.lpstrReplaceWith =szReplaceText;//Ҫ�滻���ַ���
	fr.lpTemplateName = 0;
	fr.lStructSize = sizeof(fr);//�ṹ��С
	fr.wFindWhatLen = MAX_PATH;//Ҫ���ҵ��ַ���szFindText�Ļ������ռ�
	fr.wReplaceWithLen = MAX_PATH;//Ҫ�滻���ַ���szFindText�Ļ������ռ�
	return ReplaceText(&fr);//���в���
}
void GetTextType(unsigned char text[], TextCode &fileType)//�õ��ı���������
{

	if (text[0] ==0xff && text[1] == 0xfe)
		fileType =UNICODETEXT;//UNICODE����
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
void ShowCaption(HWND hwnd, TCHAR szTitle[])//��ʾ�򿪵��ļ�����
{
	TCHAR szBuffer[2*MAX_PATH];//����Ϊ�ļ�������+���ⳤ��
	StringCchPrintf(szBuffer,2*MAX_PATH,TEXT("%s - %s"), szTitle[0] ? szTitle : unTitled, szAppName);//���и�ʽ���ַ���
	//�������Ϊ�գ�������ʾ���ޱ��⡱,������ʾ�ļ���+���±�
	SetWindowText(hwnd, szBuffer);
}
bool MyReadFile(HWND hEdit,HWND hStatus, const TCHAR szFileName[])//��ȡ�ļ�����
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
    unsigned char *pText=nullptr;//�����ȡ���ַ������ֽڣ���Ϊ�����ֽ���0-FF��Χ���������޷��ű���,����char ���ȡansi�ַ������⣬��Ϊansi�ַ�������128,��UNICODE�ַ�ֵ����128����255��unsigned char 
	//��ȡ��255(FF)����char ��ȡ-1(FFFFFFFF)
	TCHAR *pConvert=nullptr;//����ת������ַ���
	wchar_t *temp = nullptr;//��Ϊ�����õ���ʱ���ַ���
	unsigned long  nRealSize;//��ȡ�ļ���ʵ���Ĵ�С
	pText =new unsigned char[nFileLength + 2];//�����2���ռ����2��\0\0����Ϊ������UNICODE���룬UNICODE�ַ���������2��\0\0
	shared_ptr<unsigned char>sp1(pText, [](unsigned char *p){delete []p; });//������Ŀռ佻����ָ�����	
	shared_ptr<TCHAR>sp2(pConvert, [](TCHAR *p){delete []p; });//������Ŀռ佻����ָ�����
	shared_ptr<wchar_t>sp3(temp,[](wchar_t *p){delete[]p; });//������Ŀռ佻����ָ�����
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
		len=MultiByteToWideChar(CP_ACP, 0, (char *)pText, -1, nullptr, 0);//�����ֽ��ַ�ת���ɿ��ַ���Ҫ���ַ�����,�������ַ�
		pConvert = new TCHAR[len];
		MultiByteToWideChar(CP_ACP,0,(char *)pText,-1,(wchar_t *)pConvert,nFileLength+1);//�����ֽ��ַ�ת���ɿ��ַ�
		isConvert = true;//ת����
#endif
		SendMessage(hStatus, SB_SETTEXT, 4,(long) TEXT("ANSI"));//�ڵ�������ʾ��������
		break;
	case UNICODETEXT://UNICODE�����ļ�
		SendMessage(hStatus, SB_SETTEXT, 4, (long)TEXT("UNICODE"));//�ڵ�������ʾ��������
		pText += 2;//����2���ַ������ַ����FF FE
		nFileLength-=2;//������FF FE��2���ַ�����
#ifndef UNICODE//�������UNICODE����
		len=WideCharToMultiByte(CP_ACP,0,(wchar_t *)pText,-1,nullptr,0,nullptr,nullptr);//���Ҫת�����ַ�����������\0
		pConvert = new TCHAR[len];
		WideCharToMultiByte(CP_ACP,0,(wchar_t *)pText,-1,(char *)pConvert,len,nullptr,nullptr);
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
			pText[2*i]=pText[2*i+1];
			pText[2*i+1]=cc;
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
		SetWindowText(hEdit,(LPCTSTR) pConvert);//LPCSTR�����Ƿ�UNICODE ת���const char*��const wchar *
	else//��δת�����ַ����Ž��༭��
		SetWindowText(hEdit, (LPCTSTR)pText);//LPCSTR�����Ƿ�UNICODE ת���const char*��const wchar *
	CloseHandle(hFile);//��ȡ�ļ���ر��ļ����
	return true;
}
bool SaveFile(HWND hEdit, TCHAR szFileName[])//�����ļ�
{
	HANDLE hFile;//�ļ����
	hFile = CreateFile(szFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);//��д�ķ�ʽ���ļ������ļ������ڣ��򴴽��µ�
	//CREATE_ALWAYS �����ļ������дǰһ���ļ�
	if (hFile == INVALID_HANDLE_VALUE)//�ļ���ȡʧ�ܻ򴴽�ʧ��
		return false;
	int nFileLength = GetWindowTextLength(hEdit);//���ر༭���ַ�����
	TCHAR *pText=new TCHAR [nFileLength+1];//����Ҫд����ַ���,+1����Ϊ\0
	if (!pText)//�ڴ����ʧ��
	{
		CloseHandle(hFile);
		return false;
	}
	shared_ptr<TCHAR>sp1(pText, [](TCHAR *p){delete[]p; });//������Ŀռ佻����ָ�����	
	int ByteOrderMask = 0xFEFF;//�ֽ�˳���ǣ�д���ļ���ʱ�򣬵�λ��ǰ����������д��FF FE����UNICODE��ʶ
	unsigned long realWrite;//д���ļ�������ʵ��д����ֽڸ���
#ifdef UNICODE//���������UNICODE��ʶ
	WriteFile(hFile, &ByteOrderMask, 2, &realWrite, nullptr);//���ֽ�˳����д���ļ���ͷ���ñ༭���ʶ������UNICODE�ļ�����UNICODE����ķ�ʽȥ��
#endif
	GetWindowText(hEdit, pText, nFileLength+1);//���ر༭���ַ���������\0
	WriteFile(hFile, pText, nFileLength*sizeof(TCHAR), &realWrite, nullptr);//д���ļ�
	if (nFileLength*sizeof(TCHAR) != realWrite)//д���ʵ���ַ�����Ҫд����ַ�������ȣ�˵��д��ʧ��
	{
		CloseHandle(hFile);//�ر��ļ����
		return false;
	}
	CloseHandle(hFile);//�ر��ļ����
	return true;
}
bool MyFindText(HWND hEdit,FINDREPLACE &fr,bool bDown)//�����ı�
{
	int len = GetWindowTextLength(hEdit);//��ñ༭���ַ�����
	TCHAR *pText = new TCHAR[len + 1];//����ռ�
	shared_ptr<TCHAR>sp1(pText, [](TCHAR *p){delete[]p; });//������Ŀռ佻����ָ�����	
	GetWindowText(hEdit, pText, len + 1);//��ñ༭���ַ�
	int findLen = lstrlen(fr.lpstrFindWhat);//Ҫ���ҵ��ַ�������
	int pos ;//���ҵ���λ��
	SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos,(long)&endPos);//��ǰѡ�����ֵĿ�ʼλ�á���������δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������
	if (startPos !=endPos&&bDown)//��ʼ����λ�ò�ͬ��˵��ѡ�����ֲ��������²���
		startPos++;//��ӵ�ǰѡ��λ�õ���һ��λ�ü�������,����ûѡ����ӵ�ǰλ�ÿ�ʼ����
	if (!(fr.Flags&FR_MATCHCASE))//������ִ�Сд��ťû������
	{
		for (int i = 0; i < findLen; i++)//��Ҫ���ҵ��ַ���ת��Сд
			fr.lpstrFindWhat[i] = tolower(fr.lpstrFindWhat[i]);
		for (int i = 0; i < len; i++)//���༭���е���ĸҲת��Сд�������ʹ�Сдһ��
			pText[i] = tolower(pText[i]);
	}
	if (bDown)//���²���
	{
		auto p=search(pText+startPos,pText+len, fr.lpstrFindWhat , fr.lpstrFindWhat+findLen);//���ҵ�һ��ƥ����ַ���λ��
		if (p == pText + len)//���²��Ҳ���
			return false;
		pos = p - pText;
	}
	else
	{
		auto p = find_end(pText,pText + startPos, fr.lpstrFindWhat, fr.lpstrFindWhat + findLen);//���Ҵ����һ��ƥ����ַ���λ��
		if (p == pText + startPos)//���ϲ��Ҳ���
			return false;
		pos = p - pText;
	}
	SendMessage(hEdit, EM_SETSEL, pos, pos + findLen);//ѡ�в��ҵ����ַ���
	SendMessage(hEdit, EM_SCROLLCARET, 0, 0);//�ѿɼ���Χ���ù�괦����������ѡ�е�λ��
	return true;
}
void InitFont(HWND hwnd,CHOOSEFONT &cf,LOGFONT &lf)//��ʼ������ṹ
{
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
	cf.rgbColors = RGB(255, 0, 0);//��ʼ����Ի����ı���ɫ
}
void InitColor(HWND hwnd,CHOOSECOLOR &cc,COLORREF color[])//��ʼ����ɫ�ṹ
{
	cc.lStructSize = sizeof(cc);//ָ���ṹ�ĳ��ȣ��ֽ�
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;//һ�����������ʼ����ɫ�Ի����λ�������Ի��򷵻�ʱ����������Щ��ʶ������ʶ�û�������
	//CC_FULLOPEN�öԻ�����ʾ����Ŀؼ���ʹ�û������Զ������ɫ
	//CC_RGBINIT�öԻ���Ĭ��ʹ����rgbResult��Աָ������ɫ
	cc.hInstance = nullptr;
	cc.hwndOwner = hwnd;//ӵ�жԻ���Ĵ��ڵľ�����ó�Ա������������Ч�Ĵ��ھ�������ڶԻ���û��������ʱ����ΪNULL
	cc.lCustData = 0;//ָ��Ӧ�ó����Զ�������ݣ������ݻᱻϵͳ���͸����ӳ���
	cc.lpfnHook = 0;//ָ��CCHookProc���ӳ����ָ�룬�ù��ӿ��Դ����͸��Ի������Ϣ
	cc.lpTemplateName = 0;//ָ��һ��NULL��β���ַ��������ַ����ǶԻ���ģ����Դ������
	cc.rgbResult = RGB(255, 0, 0);//���CC_RGBINIT��ʶ��������ʱ���ó�Աָ���˶Ի����ʱĬ�ϵ�ѡ����ɫ
	cc.lpCustColors = color;//ָ��һ������16��ֵ�����飬����������˶Ի������Զ�����ɫ�ĺ졢�̡�����RGB��ֵ
}
void InitFind(HWND hEdit,TCHAR szFindText[])//��ʼ�����ҶԻ���
{
	SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//��ǰѡ�����ֵĿ�ʼ������λ�ã���δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������			
	if (startPos != endPos)//�����ʼ�ͽ���λ�ò����˵��ѡ��������
	{
		int len = GetWindowTextLength(hEdit);//��ñ༭���ַ�����
		TCHAR *pText = new TCHAR[len + 1];//����ռ�
		shared_ptr<TCHAR>sp1(pText, [](TCHAR *p){delete[]p; });//������Ŀռ佻����ָ�����	
		GetWindowText(hEdit, pText, len + 1);//��ñ༭���ַ�
		tstring temp(pText + startPos, pText + endPos);//���ѡ�е��ַ���
		lstrcpy(szFindText, temp.c_str());//������������
	}
	if (hDlgModeLess)//�����һ���Ի���δ�ر�
		SendMessage(hDlgModeLess, WM_CLOSE, 0, 0);//������Ϣ�ر���1���Ի���
}
void DrawBorder(HWND hwnd,bool bShowStatus)//������ɫ�߿�
{
	RECT rect;
	HDC hdc;
	HBRUSH hBlueBrush = CreateSolidBrush(RGB(102, 179, 255));//�༭��ĵ���ɫ�߿��õĻ�ˢ
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);//�õ��ͻ�������
	if (bShowStatus)//�����ʾ״̬��
	rect.bottom -= 26;//�߿򲻰�Χ״̬������ȥ״̬���߶�
	FrameRect(hdc, &rect, hBlueBrush);//������ɫ��ˢ�߿�
	ReleaseDC(hwnd, hdc);
	DeleteObject(hBlueBrush);//ɾ���ɻ�ˢ
}
int _stdcall DlgHelpProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)//�����Ի���
{
	HWND hEdit;//�༭��
	tstring szFileName;//�ļ�����·��
	switch (message)
	{
	case WM_INITDIALOG://��ʼ�Ի���
		hEdit = GetDlgItem(hDlg, ID_EDITHELP);//�õ������༭����
		szFileName = tstring(szWorkSpacePath) + TEXT("\\help.txt");//��ð����ļ�������·��
		MyReadFile(hEdit, 0,szFileName.c_str());//��ȡ�����ļ������ݵ��༭��
		SetFocus(hEdit);//���ǳ����ڴ���WM_INITDIALOG��Ϣ�����뽹������Ϊĳ���ؼ�������false,����Windows������뽹������Ϊ�Ի����е�һ������WS_TAB�Ŀؼ�
		//��Ϊ�����Ի���ֻ��һ���ؼ��༭�򣬶Ի��������һ��TABSTOP���Կؼ��������м�ʹ����TABSTOP����Ϊfalse������������TABSTOP���ԣ���������һ���ؼ����ڱ༭��ǰ�棬����SetFocus
		//�ÿؼ���ý��㣬������false,����Ĭ���ڶԻ�����ʾʱ�������뽹�㣬��Ϊ��ֻ���ı༭�����ֻ�ȫѡ������setfousȥ������ȫѡ
		HideCaret(hEdit);//�༭���й�꣬��Ϊ����Դ������Ϊֻ���������ù�����زűȽϺã�һ��Ҫ��setfoucs����������Ч����Ϊ��ʼ�������б༭�ػ�δ��ý��㣬��������
		//���ع����Ч��setfocus������ý����������ع�����Ч
		return false;
	case WM_CLOSE:
		EndDialog(hDlg,0);//�ر�ʱ���ٴ���,������DestroyWindow(hDlg)��Ϊ��ֻ�����ٴ��ڲ����˳��Ի������Ϣѭ������û������Ի���Ӧ�ó���ѿ���Ȩ����ϵͳ�������ڻ��ǲ��ܶ���
		//��EndDialog������ڴ������ٺ�����Ի�����Ϣ���У������ؿؼ�Ȩ��������
		return true;
	}
	return false;
}
int _stdcall DlgGotoProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)//ת���Ի��򴰿ڹ���
{
	static HWND hEditGoto;//�༭��
	TCHAR strNum[24];//�༭���ϵ�����
	int lineIndex;//ָ�������ַ����ı��е�λ�ã����ֽ�����ʾ��
	static HWND hwnd;
	static HWND hEdit;//���±��༭����
	static int lineCount;//�༭��������
	switch (message)
	{
	case WM_INITDIALOG://��ʼ�Ի���
		hwnd = GetParent(hDlg);//�õ������ھ��
		hEdit = GetDlgItem(hwnd, ID_EDIT);
		lineCount = SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);
		hEditGoto = GetDlgItem(hDlg, IDC_EDITGOTO);//�õ�ת���༭����
		SetWindowText(hEditGoto,toString(row).c_str());
		return true;//��Ϊ�ڶԻ�����Դֻ�Ա༭������WS_TABSTOP�����ﷵ��true���ͻ�ʹ�༭�������뽹�㣬���ֻ�ȫѡ
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_GOTO:
			GetWindowText(hEditGoto, strNum, sizeof(strNum));
			row = strToInt(strNum);//���ַ���ת��������
			if (row > lineCount)
			{
				SetWindowText(hEditGoto, toString(lineCount).c_str());//��ת���༭����ʾ�༭��������
				SetFocus(hEditGoto);//��ת���༭���ý��㣬��Ϊ������ȫѡ��������ʾ������
				SendMessage(hEditGoto, EM_SETSEL, 0, -1);//ת���༭������ȫѡ
				MessageBox(hEditGoto, TEXT("���±�������������"), TEXT("���±� -����"), 0);
				break;
			}
			lineIndex = SendMessage(hEdit, EM_LINEINDEX,row-1, 0);//�õ�ָ�������ַ����ı��е�λ�ã����ֽ�����ʾ����-1����Ϊ������0��ʼ
			SendMessage(hEdit, EM_SETSEL, lineIndex, lineIndex);//������Ƶ�ָ����
			SendMessage(hEdit, EM_SCROLLCARET, 0, 0);// �ѿɼ���Χ������괦,�����ù��󣬹����ܲ��ڷ�Χ�ڣ�����Ҫ��������괦
			SendMessage(hEdit, WM_LBUTTONUP, 0, 0);//����һ������ɿ���Ϣ�Ը����к�
		case IDC_CANCELGOTO:
			EndDialog(hDlg, 0);
			break;
		}
		return true;
	case WM_CLOSE:
		EndDialog(hDlg, 0);//�ر�ʱ���ٴ���,������DestroyWindow(hDlg)��Ϊ��ֻ�����ٴ��ڲ����˳��Ի������Ϣѭ������û������Ի���Ӧ�ó���ѿ���Ȩ����ϵͳ�������ڻ��ǲ��ܶ���
		//��EndDialog������ڴ������ٺ�����Ի�����Ϣ���У������ؿؼ�Ȩ��������
		return true;
	}
	return false;//���û�д�������0,�����Ĭ�϶Ի�����̣���0�����ˣ�������Ĭ�ϵ�
}
int _stdcall DlgPrintProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)//��ӡ��ֹ�Ի��򴰿ڹ���
{
	switch (message)
	{
	case WM_INITDIALOG://�Ի����ʼ��
		EnableMenuItem(GetSystemMenu(hDlg, false), SC_CLOSE, MF_GRAYED);//�öԻ���رհ�ť������
		SetWindowText(hDlg, TEXT("���±���ӡ��ֹ�Ի���"));//���ñ���
		return true;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)//����û�ȡ����ӡ
		{
			bUserAbort = true;//�û���ֹ��ӡ���Ϊtrue
			EnableWindow(GetParent(hDlg), true);//�������ڻָ��
			DestroyWindow(hDlg);//���ٴ�ӡ�Ի���
			hDlgPrint = 0;//��ӡ�Ի�����Ϊ�գ����ٷ��ʹ�ӡ�Ի�����Ϣ
		}
		return true;
	}
	return false;//���û�д�������0,�����Ĭ�϶Ի�����̣���0�����ˣ�������Ĭ�ϵ�
}
int _stdcall AbortProc(HDC hPrinterDC, int iMode)//��ӡ����ֹ����
{
	MSG msg;//�����ӡ��ʼ����������ֹ���֮������ֹ����Ƿ�ֹ�ڴ�ӡ�����ٴΰ��´�ӡ�˵�����ɳ�ͻ�������ڲ��ܴ�����Ϣ��������Ϣ�ɿɻ�ĶԻ�������
	while (!bUserAbort&&PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))//������û�û����ֹ��ӡ��������Ϣ��������Ϣ��ȡ����Ϣ������Ϣ����ȡ�õ���Ϣ������ѭ���崦��
	{
		if (!hDlgPrint || !IsDialogMessage(hDlgPrint, &msg))
		{//�����ӡ��ֹ�Ի������٣����߲��ǶԻ�����Ϣ��������Ϣ����
			TranslateMessage(&msg);//��һЩ������Ϣת�����ַ���Ϣ���ڰ�����Ϣ����
			DispatchMessage(&msg);//�ַ���Ϣ�������ô��ڹ��̽��д�����Ϣ
		}
	}
	return !bUserAbort;//����û�û��ȡ����ӡ��bUserAbout��false,ȡ������true,��ʾ������ӡ��һҳ
}
int _stdcall EditTextProc(HWND hEdit, unsigned message, unsigned wParam, long lPram)//��д���±��༭�Ի����б༭�ؼ��Ի������
{
	int first;//��ǰ��������У����ַ��ڱ༭�е�λ��
	switch (message)
	{
	case WM_KEYUP://�������ɿ���
	case WM_LBUTTONUP://���������ɿ���ʱ�򣬸���������Ϣ
		row = SendMessage(hEdit, EM_LINEFROMCHAR, -1, 0)+1;//���ָ��λ�����ڵ��кţ�-1��ʾ��ǰ�������λ�ã���Ϊλ�õ�������0��ʼ������Ҫ��1
		statusInfo[2] = TEXT("�к�:") + toString(row);//���к�ת�����ַ���
		SendMessage(hStatus, SB_SETTEXT, 2, (long)statusInfo[2].c_str());//�Եڶ��������к� 
		first = SendMessage(hEdit, EM_LINEINDEX, -1, 0);////��ǰ��������У����ַ��ڱ༭�е�λ��
		SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//��ǰѡ�����ֵĿ�ʼ������λ�ã���δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������			
		col = startPos - first+1;//��ǰ����λ��-��ǰ��������е�1���ַ���λ��Ϊ����������Ϊλ�õ�������0��ʼ������Ҫ��1
		statusInfo[3] = TEXT("�к�:") + toString(col);//���к�ת�����ַ���
		SendMessage(hStatus, SB_SETTEXT, 3, (long)statusInfo[3].c_str());//�Ե����������к� 
		break;
	}
	return CallWindowProc(oldEditTextProc, hEdit, message, wParam, lPram);//����Ĭ�ϵı༭�ؼ����ڴ�����̴���������Ϣ��δ������Ĳ���
}
bool PrintFile(HINSTANCE hInst, HWND hwnd, HWND hEdit, TCHAR szTitle[])//��ӡ�ļ�
{
	DOCINFO di{ sizeof(DOCINFO)};//��ӡ�ĵ������ô�ӡ���ĵ���
	TCHAR szWindowText[MAX_PATH];//���������ڱ���
	GetWindowText(hwnd, szWindowText, MAX_PATH);//�õ������ڱ���
	di.lpszDocName = szWindowText;//���ô�ӡ���ĵ���Ϊ
	PRINTDLG pd;//��ӡ���ṹ
	bool bSuccess=true;//�����Ƿ񷢳�����ʼ��true��δ����
	bUserAbort =false;//��ʼ��false���û�δ��ֹ
	memset(&pd, 0, sizeof(pd));//��ӡ�ṹ���㣬��Ϊ����ܶ��ֶ��ò�������Ϊ0
	pd.Flags =PD_RETURNDC| PD_ALLPAGES | PD_COLLATE | PD_NOPAGENUMS|PD_NOSELECTION;
	//PD_ALLPAGES ָ����ȫ������ѡť�ڳ�ʼʱ��ѡ��(ȱʡ��־)
	//PD_COLLATE(0x10) ָ�����Զ���ҳ����ѡ���ڳ�ʼʱ��ѡ��
	//PD_NOPAGENUMS(0x08) ʹ��ҳ�뷶Χ����ѡť�Ϳ�ʼ / ����ҳ��༭�ؼ�ʧЧ(ȱʡ��־���ǲ�����Ҳ��ʧЧ)���������û������Զ���ҳ�İ�ť������Ҫ����
	//PD_RETURNDC(0x100) ʹ��ӡ�Ի��򷵻����û�ѡ����ƥ���DC��hDC��Ա��������ѡ�Ĵ�ӡ������
	//PD_NOSELECTION(0x04) ʹ��ѡ����Χ����ѡťʧЧ(ȱʡ��־)
	pd.hwndOwner = hwnd;//�����ھ��(��ΪNULL)
	pd.lStructSize = sizeof(pd);//���ṹ��С(�ֽ���)
	pd.nCopies = 1;//ָ�����������༭�ؼ��ĳ�ʼֵ
	/* �����ӡ�Ի���
	����û���� OK ��ť������ֵΪ����ֵ��
	����û�ȡ����ر� Print �� PrinterDlg�Ի���������֣�����ֵΪ�� */
	if (!PrintDlg(&pd))
		return true;//����ӡ�����Ǵ�ӡʧ��
	int lineCount = SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);//�õ��༭�������
	if (!lineCount)
		return true;//�����ݲ��ô�ӡ
	TEXTMETRIC tm;//�ı��ߴ�ṹ
	GetTextMetrics(pd.hDC, &tm);//�õ�ָ����ӡ�����ı��ߴ�ṹ
	int cyChar = tm.tmHeight + tm.tmExternalLeading;//�õ���ӡ����һ���ַ��ĸ߶�
	int cxChar = tm.tmAveCharWidth;//�õ���ӡ����һ���ַ��Ŀ��
	int xPage = GetDeviceCaps(pd.hDC, VERTRES);//��ô�ӡ����ֽ�Ͽɴ�ӡ��Χ�ĸ߶�
	int yPage = GetDeviceCaps(pd.hDC, HORZRES);//��ô�ӡ����ֽ�Ͽɴ�ӡ��Χ�Ŀ��
	int iLinesPerPage = yPage / cyChar;//һҳ�ж�����
	int iCharsPerLine = xPage / cxChar;//һ���ж��ٸ��ַ�
	TCHAR *pStrBuffer = new TCHAR[iCharsPerLine + 1];//����ÿ���ַ����Ļ�����
	shared_ptr<TCHAR>sp(pStrBuffer, [](TCHAR*p){delete[]p; });//��������ڴ潻����ָ�����
	int Pages = lineCount / iLinesPerPage;//�ı�Ҫ����ӡ�ɶ���ҳ
	if (lineCount%iLinesPerPage)//��������������࣬˵��������У�Ҳ��һҳ
		Pages++;
	EnableWindow(hwnd, false);//�����ӡ��ʼ����������ֹ���֮������ֹ����Ƿ�ֹ�ڴ�ӡ�����ٴΰ��´�ӡ�˵��ִ�ͷ��ӡ�����Ѿ��ڴ�ӡ����ɳ�ͻ
	SetAbortProc(pd.hDC, AbortProc);//���ô�ӡ��ֹ���̣�ÿ��ӡһҳ���øú�����ѯ�Ƿ�ȡ����ӡ
	hDlgPrint = CreateDialog(hInst, TEXT("IDD_DIALOGPRINTER"), hwnd, DlgPrintProc);//������ֹ��ӡ�Ի��򣨷�ģ̬)
	RECT rect;//�������ֵľ�������
	if (StartDoc(pd.hDC, &di))//��ʼ��ӡ
	{
		//����ĵ���3ҳ���û�Ҫ���ӡ3�ݣ���ݴ�ӡ���Ǵ�ӡ3�Σ�ÿ�θ�ҳ��ӡһ�ݣ���ӡ˳��1,2,3��ӡ3��
		//������ݴ�ӡ�������Ǵ�ӡһ�Σ�ÿ�θ�ҳ��ӡ3�ݣ���ӡ˳��1��1��1��2��2��2��3��3��3
		int times = pd.Flags&PD_COLLATE ? pd.nCopies : 1;//�����ݴ�ӡ�����ӡ�ķ�������Ҫ��ӡ�Ĵ���������ֻ��ӡһ��
		for (int i = 0; i < times; i++)//��ӡÿһ��
		{
			for (int iPage = 0; iPage < Pages; iPage++)//��ӡÿһҳ
			{
				int copys = pd.Flags&PD_COLLATE ? 1 : pd.nCopies;//ÿһҳ��ӡ�ķ����������ݴ�ӡ���ּ��δ�����ÿ��ֻ��ӡһ�ݣ�����ÿҳһ���Դ�ӡ���
				for (int iCopy = 0; iCopy < copys; iCopy++)//��ӡÿҳ��ÿһ��
				{
					if (StartPage(pd.hDC))//��ӡ��ǰҳ
					{
						for (int iLine = 0; iLine < iLinesPerPage; iLine++)//��ӡҳ����ÿһ��
						{
							int iLineNum = iPage*iLinesPerPage + iLine;//��������Ǳ༭�ϵĵڼ���
							if (iLineNum >= lineCount)//��Ϊ���һҳ�����пհף����Ե��˱༭�����һ��
								break;//�˳���ӡ
							*(int *)pStrBuffer = iCharsPerLine;//
							/*iCharsPerLine Ϊ��ӡ��ÿ�пɴ�ӡ�ַ��� .
							�������Ϊ���淢�� EM_GETLINE ��Ϣ���ָ���е��ַ�����׼�� ,
							��ΪpstrBuffer �ĵ�һ��˫����Ҫ�����û������Ĵ�С����֪��һ������ܻ�ö��ٸ��ַ�����Ϊ��
							������ˮƽ������������һ�е��ַ������ܳ����ɴ�ӡ��Χ�Ŀ�ȣ�������Χ������*/
							int len=SendMessage(hEdit, EM_GETLINE, iLineNum, (long)pStrBuffer);//���ָ���е��ַ�����
							//��Ϊ����һ�е�ʱ�򲻰�����ֹ�ַ�\0�����Բ�����lstrlen�жϿ����ĸ�����Ҫ��SendMessage���صĿ������ַ�����
							rect.left = 0;//������߿�ʼ����������Ϊÿһ�еľ�������
							rect.top = iLine*cyChar;//�߶�
							rect.right = yPage;//��ӡ���ɴ�ӡ��Χ�����ұ�
							rect.bottom = (iLine + 1)*cyChar;
							DrawText(pd.hDC, pStrBuffer, len, &rect, DT_EXPANDTABS);//��ӡÿһ�У�ע�ⲻ����TextOut,��Ϊ��֧���Ʊ�����з����ո����ַ����ȿ�֮�������
							//Ĭ��DrawText����Ʊ�����|�� DT_EXPANDTABS�Ὣ�Ʊ������7���ո�
						}
						if (!EndPage(pd.hDC))//������ҳ�Ĵ�ӡ
						{
							bSuccess = false;//����ʧ�ܣ���������
							break;//��ӡʧ�ܣ��˳�
						}
						if (bUserAbort)//����û���ֹ��ӡ
							break;//��ӡʧ�ܣ��˳�							
					}
					else
					{
						bSuccess = false;//��������
						break;//��ӡʧ�ܣ��˳�
					}
				}
				if (!bSuccess || bUserAbort)//��������̷���������û���ֹ��ӡ
					break;//��ӡʧ�ܣ��˳�
			}
			if (!bSuccess || bUserAbort)//��������̷���������û���ֹ��ӡ
				break;//��ӡʧ�ܣ��˳�
		}
	}
	else
		bSuccess = false;//��������
	if (bSuccess)//��ӡδ��������
		EndDoc(pd.hDC);//������ӡ
	if (hDlgPrint)//�û�δ��ȡ����ť
	{
		EnableWindow(hwnd, true);//�������ڻָ��
		DestroyWindow(hDlgPrint);//������ֹ��ӡ�Ի���
	}
	DeleteDC(pd.hDC);
	return bSuccess&&!bUserAbort;//ֻ�д�ӡ����δ�����������û�δ����ֹ��ť���Ǵ�ӡ�ɹ�
}
void FormatTime()//��ϵͳʱ����и�ʽ��
{
	SYSTEMTIME st;//ʱ��ṹ
	GetLocalTime(&st);//�õ�����ʱ��
	tstringstream out;//������ַ�������ʽ���ַ���
	out << _T("����ʱ��:") << st.wYear << _T("��") << st.wMonth << _T("��") << st.wDay << _T("  ") << st.wHour <<
		_T("Сʱ") << st.wMinute << _T("��") << st.wSecond << _T("��");
	statusInfo[1] = out.str();//�����е��ַ����������ַ�����
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
	static TCHAR szFileName[MAX_PATH] = TEXT("");//�����ļ�·���������ļ���
	//��ʼ���ļ����༭�ؼ�ʹ�õ��ļ����Ļ��塣�������Ҫ��ʼֵ���������ĵ�һ���ַ�������NULL
	static TCHAR szTitle[MAX_PATH];//�����ļ���������·��
	static OPENFILENAME ofn;//������ʼ���򿪻����Ϊ�Ի������Ϣ
	static CHOOSEFONT cf;//����ṹ
	static CHOOSECOLOR cc;//��ɫ�ṹ
	static LOGFONT lf;//�߼�����
	static HFONT hFont;//������
	static COLORREF color[16];//��ɫ�Ի����б���16���Զ�����ɫ
	static TCHAR szFindText[MAX_PATH];//Ҫ���ҵ��ַ���
	static TCHAR szReplaceText[MAX_PATH];//�����滻���ַ���
	static unsigned int WM_FINDREPLACE;//�Զ������Ϣ�����ڽ��ղ��Һ��滻����Ϣ
	static bool bDown ;//���°�ť�Ƿ���
	static bool bAutoLine=false;//�Ƿ��Զ�����
	static bool bShowStatus=true;//�Ƿ���ʾ״̬��
	static HMENU hMenu;//�˵����
	static unsigned style;//������������ʽ
	static HBRUSH hBrush=CreateSolidBrush(RGB(255,255,255));//Ĭ�ϱ༭�򱳾���ˢ����ɫ
	static COLORREF textColor = RGB(0, 0, 0);//Ĭ�ϵı༭��������ɫ
	static COLORREF textBkColor = RGB(255,255,255);//Ĭ�ϵı༭�����屳����ɫ
	LPCREATESTRUCT cs;//���ڽṹ��ָ��
	int xPos[5]{0, 0, 0,0,0};//״̬����5���Һ�����
	TCHAR *pText;//����༭���ַ���
	switch (UMsg)
	{
	case WM_CREATE://���ڱ�����
		GetCurrentDirectory(MAX_PATH, szWorkSpacePath);//��õ�ǰ��������·��
		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);//��ô���ʵ�����
		hMenu = GetMenu(hwnd);//�õ����ھ��
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|ES_AUTOHSCROLL|ES_MULTILINE|ES_NOHIDESEL, 0, 0, 0, 0,
			hwnd, (HMENU)ID_EDIT, hInst, nullptr);//�����б߽�Ķ��б༭��ES_NOHIDESEL�ڱ༭��ʧȥ�����ѡ�е��ı���Ȼ���ָ�����ʾ
		hStatus = CreateWindow(STATUSCLASSNAME, 0, WS_CHILD | WS_VISIBLE,
			0, 0,0,0, hwnd, (HMENU)IDS_STATE, hInst, nullptr);//����״̬��
		InitOpenFileName(ofn,hwnd);//��ʼ���ļ��ṹ
		InitFont(hwnd, cf, lf);//��ʼ������ṹ
		InitColor(hwnd, cc, color);//��ʼ����ɫ�ṹ
		ShowCaption(hwnd, szTitle);//��ʾ�ޱ���
		SendMessage(hEdit, EM_LIMITTEXT,0, 0);//��wParamΪ0ʱ�����б༭�ز����0x7FFFFFFE���ַ���Ĭ����30000���ַ�
		WM_FINDREPLACE = RegisterWindowMessage(FINDMSGSTRING);//���Һ��滻�Ի���ᷢ����Ϣ����FINDMSGSTRINGע�����Ϣ		
		hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0,0, NULL);//Ĭ������
		SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);
		cs = (LPCREATESTRUCT)lParam;//�����ڱ�����ʱ����Ѵ��ڵĸ�����Ϣ������һ��CREATESTRUCT�ṹ�У���������ʱ�ᷢ��WM_CREATE��Ϣ������lParam����������ṹ�ĵ�ַ
		//���������ַ����������ļ�������
		lstrcpy(szFileName, (TCHAR*)(cs->lpCreateParams));//�ڵ���CreateWinodowʱ�����һ������lpParam�������������ַ���szCmdLine,lpParam������CREATESTRUCT�� lpCreateParam�ֶ���
		if (szFileName[0] != '\0')//����ַ�����Ϊ�գ�������ʱ�����˴��Σ����߳�����ͨ����קһ���ļ��ڳ����ϴ�
			MyReadFile(hEdit,hStatus, szFileName);//��ȡ�ļ�����
		SetTimer(hwnd, ID_TIMER, 1000, nullptr);//��ʱ��ÿһ�����һ��ʱ��
		oldEditTextProc = (WNDPROC)SetWindowLong(hEdit, GWL_WNDPROC, (long)EditTextProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ��̡������Ϣ����Ϊ
		//Ĭ���ǲ��񲻵���Щ��Ϣ�ģ���Ҫ�õ���Щ��Ϣ�����統���򰴼�����ʱ����״̬����ʾ��������Ϣ
		//���ǲ�����SetClassLong(hEdit,GCL_WNDPROC),��Ϊ�����޸�edit��Ĵ��ڹ��̣���ֻ�����һ�������ı༭����Ч���Ե�ǰ
		//�༭�򲻻�ı䣬������SetClassLong,��Ӱ���Ժ�ı༭�򣬶���ֻ��ı䵱ǰ�༭�򣬲���ı������༭��
		return 0;
	case WM_SETFOCUS://���ڻ�ý���ʱ
		SetFocus(hEdit);//�ñ༭���ý���
		return 0;
	case WM_DROPFILES://������ק����
		DragQueryFile((HDROP)wParam, 0, szFileName, MAX_PATH);//�����ק���ļ���·��
		MyReadFile(hEdit,hStatus, szFileName);//����·����ȡ�ļ�����
		return 0;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		xPos[4] =-1;//���һ�������ұ߽�-1�����򵽿ͻ������ұ�
		xPos[3] = cxClient - 100;
		xPos[2] = cxClient - 160;
		xPos[1] = cxClient - 250;
		xPos[0] = cxClient - 500;//��һ�������ұ߽�
		SendMessage(hStatus, SB_SETPARTS,5, (long)xPos);//����5�������������
		if (bShowStatus)//�����ʾ״̬�����༭����״̬������
		    MoveWindow(hEdit,1,1, cxClient-4, cyClient-29, true);//-4��������Ȼ��߿�-29�ǰѱ߿���״̬�����棬״̬���߶�25����4����Ȼ��߿�
		else
			MoveWindow(hEdit, 1, 1, cxClient - 4, cyClient -4, true);//-4��������Ȼ��߿򣬵����༭��СΪ�����ͻ���
		MoveWindow(hStatus, 0,0, 0,0, true);//����״̬���ڿͻ��������棬�������ô�Сλ�ã���Ϊ��Сλ�ù̶����ɱ�
		for (int i = 0; i <5; i++)
			SendMessage(hStatus, SB_SETTEXT, i, (long)statusInfo[i].c_str());//��ÿһ����ʾ�ı�
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
			EnableMenuItem((HMENU)wParam, IDM_EDIT_COPY, isEnable);//ѡ���������������
			EnableMenuItem((HMENU)wParam, IDM_EDIT_CUT, isEnable);
			EnableMenuItem((HMENU)wParam, IDM_EDIT_DELETE, isEnable);
		}
		return 0;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		if (wmId == ID_EDIT)
		{
			switch (wmEvent)
			{
				case EN_UPDATE:
					bNeedSave = true;//�༭������ˣ���Ҫ����
					break;
				case EN_ERRSPACE://����༭���ַ�����0x7FFFFFFE���ַ�
				case EN_MAXTEXT:
					MessageBox(nullptr, TEXT("�����༭�ؼ������ռ䣬���0x7FFFFFFE���ַ�"), TEXT("Error"), MB_ICONSTOP);
					break;
			}
		}
		switch (wmId)
		{
		case IDM_FILE_NEW://�½��ļ�
			if (bNeedSave&&IDCANCEL == AskAboutSave(hwnd, szFileName))
				return 0;//����ı������ˣ���Ҫ���б��棬���ش��ǲ������������½��ļ���������ִ��
			SetWindowText(hEdit, TEXT(""));//���༭�����
			szTitle[0] = '\0';//�������ÿ�,����ʾ����Ϊ�ޱ���
			szFileName[0] = '\0';//���ļ�·����գ�ʹ�����ʱ���Ҳ����ļ����������Ϊ����
			ShowCaption(hwnd, szTitle);//��ʾ�ޱ���
			SendMessage(hStatus, SB_SETTEXT,0,(long)TEXT("�½��ļ�"));//��һ����ʾ�½��ļ�
			break;
		case IDM_FILE_OPEN://���ļ�
			if (bNeedSave&&IDCANCEL == AskAboutSave(hwnd, szFileName))
				return 0;//����ı������ˣ���Ҫ���б��棬���ش��ǲ���������������ļ���������ִ��
			ofn.lpstrTitle = TEXT("�ף�Ҫѡһ���ļ�����");//ָ���ڶԻ���ı������з��õ��ַ�������������Ա��NULL��ϵͳʹ��Ĭ�ϱ��⣨���Ϊ���)
			ofn.Flags = OFN_HIDEREADONLY;//λ��ǵ����ã������ʹ������ʼ���Ի���
			//OFN_ALLOWMULTISELECT ָ���ļ����б�������ѡ,OFN_HIDEREADONLY ����ֻ����ѡ��
			// OFN_CREATEPROMPT����û�ָ����һ�������ڵ��ļ���������ʹ�öԻ�������ʾ�û��Ƿ��½�����ļ�
			ofn.lpstrFile = szFileName;//��GetOpenFileName��GetSaveFileName�������سɹ�ʱ��������������������·�����ļ���������ѡ����ļ�����չ��
			ofn.lpstrFileTitle = szTitle;//ָ�����ѡ����ļ����ļ�������չ���Ļ��壨����·����Ϣ���������Ա������NULL
			if (GetOpenFileName(&ofn))//����һ��Open�����Ի���ʹ�û�ָ����������Ŀ¼���ļ�������ʹ�û����ļ�,����û�ָ����һ���ļ��������OK��ť������ֵΪ����
			{
				if (!MyReadFile(hEdit,hStatus, szFileName))//��ȡ�ļ����ݵ��༭��
				{
					//��ȡʧ��
					OKMessage(hwnd, TEXT("%s\n�Ҳ����ļ���\n�����ļ����Ƿ���ȷ��Ȼ������"), szTitle);
					szTitle[0] = '\0';//���ñ���Ϊ�գ���Ϊ����Ҫ��ʾ���⣬�������Ϊ�գ�����ʾ��1������
				}
				else
				{
					bNeedSave = false;//�����ļ�����Ҫ�ٱ��棬��Ϊfalse
					statusInfo[0] = tstring(TEXT("���ļ�:")) + szFileName;
					SendMessage(hStatus, SB_SETTEXT,0, (long)statusInfo[0].c_str());//�Ե�һ����ʾ���ļ�
				}
			}
			ShowCaption(hwnd, szTitle);//���±���
			break;
		case IDM_FILE_SAVE://�����ļ�
			if (!SaveFile(hEdit, szFileName))//�����ļ�
			{//����ʧ�ܽ������Ϊ��ѡ����·��
				return SendMessage(hwnd, WM_COMMAND, MAKELONG(IDM_FILE_SAVEAS, 0), 0);//�������ʧ�ܣ�˵���ļ���ֻ�����ԣ�����д������һ�����Ϊ��Ϣ��תȥ���Ϊ�Ի��򣬱�������һ��λ��
			}
			else
			{
				statusInfo[0] = tstring(TEXT("�����ļ�:")) + szFileName;
				SendMessage(hStatus, SB_SETTEXT,0, (long)statusInfo[0].c_str());//�Ե�һ����ʾ�����ļ�
			}
			bNeedSave = false;//������غ���Ҫ�ٱ��棬��Ϊfalse
			ShowCaption(hwnd, szTitle);//���±���
			return 1;//��ʾ����ɹ�����Ϊ�����ط�ͨ��SendMessage()���������Ϣ������1��ֵ�ж��Ƿ񱣴�ɹ�
		case IDM_FILE_SAVEAS://���Ϊ
			ofn.lpstrTitle = TEXT("��,�������ΪӴ");//ָ���ڶԻ���ı������з��õ��ַ�������������Ա��NULL��ϵͳʹ��Ĭ�ϱ��⣨���Ϊ���)
			ofn.Flags = OFN_OVERWRITEPROMPT;//���ѡ����ļ��Ѿ����ڣ�ʹ�����Ϊ�Ի������һ����Ϣ���û�����ȷ���Ƿ񸲸�����ļ���
			ofn.lpstrFile = szFileName;//��GetOpenFileName��GetSaveFileName�������سɹ�ʱ��������������������·�����ļ���������ѡ����ļ�����չ��
			ofn.lpstrFileTitle = szTitle;//ָ�����ѡ����ļ����ļ�������չ���Ļ��壨����·����Ϣ���������Ա������NULL
			if (GetSaveFileName(&ofn))//����һ��Save�����Ի����Ա��û�ָ����������Ŀ¼���ļ���
			{//���ѡ����һ��·��
				return SendMessage(hwnd, WM_COMMAND, MAKELONG(IDM_FILE_SAVE, 0), 0);//������ѡ·�������ļ�
			}
			return 0;//û��ѡ��·������ȡ�����棬����ʧ�ܣ���Ϊ�����ط�ͨ��SendMessage()���������Ϣ������ֵ0��ʾ ����ʧ��
		case IDM_FILE_PAGESET://ҳ������
			return 0;
		case IDM_FILE_PRINT://��ӡ
			if (!PrintFile(hInst, hwnd, hEdit, szTitle))//���д�ӡ�ļ�
				MessageBox(hwnd, TEXT("��ӡ�ļ�ʧ��"), szAppName, MB_OK | MB_ICONWARNING);//��ӡʧ��
			return 0;
		case IDM_APP_EXIT://�˳�
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;
		case IDM_EDIT_FIND: //����
			InitFind(hEdit, szFindText);//��ʼ�����ҶԻ���
			hDlgModeLess = FindDialog(hwnd, szFindText);//�������ҶԻ������
			return 0;
		case IDM_EDIT_FINDNEXT://������һ��
			InitFind(hEdit, szFindText);//��ʼ�����ҶԻ���
			if (szFindText[0] == '\0'||!pfr)//�����û��Ҫ���ҵ��ַ������δ�򿪹����ҶԻ���
				hDlgModeLess = FindDialog(hwnd, szFindText);//�������ҶԻ�����������û������ַ�������
			else
			{
				if (!MyFindText(hEdit, *pfr, bDown))//������һ��
					OKMessage(hDlgModeLess, TEXT("�Ҳ���\"%s\""),pfr->lpstrFindWhat);//���Ҳ���
			}
			return 0;
		case IDM_EDIT_REPLACE ://�滻
			InitFind(hEdit, szFindText);//��ʼ�����ҶԻ���
			hDlgModeLess = ReplaceDialog(hwnd, szFindText, szReplaceText);////�����滻�Ի������
			break;
		case IDM_EDIT_GOTO: //ת��
			DialogBox(hInst, TEXT("IDD_DIALOGGOTO"), hwnd,DlgGotoProc);
			return 0;
		case IDM_EDIT_TIMEDATE://ʱ��/����
			FormatTime();//��ϵͳʱ����и�ʽ��
			SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//��ǰѡ�����ֵĿ�ʼ������λ�ã���δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������			
			SendMessage(hEdit, EM_REPLACESEL, startPos,(long)statusInfo[1].c_str());//�ڵ�ǰ������ʱ��
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
		case IDM_EDIT_DELETE://ɾ��ѡ��   δ����ɾ������
			SendMessage(hEdit, WM_CLEAR, 0, 0);
			break;
		case IDM_FORMAT_AUTOLINE://�Զ�����
			{
				int nFileLength = GetWindowTextLength(hEdit);//���ر༭���ַ�����
				pText = new TCHAR[nFileLength + 1];//����Ҫд����ַ���,+1����Ϊ\0
				GetWindowText(hEdit, pText, nFileLength+1);
				shared_ptr<TCHAR>sp1(pText, [](TCHAR *p){delete[]p; });//������Ŀռ佻����ָ�����	
				DestroyWindow(hEdit);//����ԭ���ı༭��
				if (bAutoLine)//���ԭ���Զ�����
				{
					CheckMenuItem(hMenu, IDM_FORMAT_AUTOLINE, MF_UNCHECKED);//ȡ��ѡ��״̬
					bAutoLine = false;
					//�����༭�򣬼Ӹ�ˮƽ��������������
					hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL |WS_HSCROLL| ES_MULTILINE | ES_NOHIDESEL, 0, 0, 0, 0,
						hwnd, (HMENU)ID_EDIT, hInst, nullptr);//�����б߽�Ķ��б༭��ES_NOHIDESEL�ڱ༭��ʧȥ�����ѡ�е��ı���Ȼ���ָ�����ʾ
					MoveWindow(hEdit, 2,2, cxClient-4, cyClient-29, true);//-4��������Ȼ��߿�-29�ǰѱ߿���״̬�����棬״̬���߶�25����4����Ȼ��߿�	
					ShowWindow(hStatus, SW_NORMAL);//��ʾ״̬��
					EnableMenuItem(hMenu, IDM_LOOKUP_STATE, MF_ENABLED);//��״̬����Ч
					CheckMenuItem(hMenu, IDM_LOOKUP_STATE, MF_CHECKED);//ѡ��״̬����ť
					bShowStatus = true;//״̬����ѡ�б��
				}
				else
				{
					CheckMenuItem(hMenu, IDM_FORMAT_AUTOLINE, MF_CHECKED);//ѡ��״̬
					bAutoLine =true;
					//�����༭����ˮƽ�����������Զ�����
					hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_NOHIDESEL, 0, 0, 0, 0,
						hwnd, (HMENU)ID_EDIT, hInst, nullptr);//�����б߽�Ķ��б༭��ES_NOHIDESEL�ڱ༭��ʧȥ�����ѡ�е��ı���Ȼ���ָ�����ʾ
					MoveWindow(hEdit,2,2, cxClient-4, cyClient-4, true);//-4��������Ȼ��߿򣬽��༭�򸲸ǿͻ���		
					ShowWindow(hStatus, SW_HIDE);//����״̬��
					EnableMenuItem(hMenu, IDM_LOOKUP_STATE, MF_GRAYED);//��״̬����Ч
					CheckMenuItem(hMenu, IDM_LOOKUP_STATE, MF_UNCHECKED);//ȡ��ѡ��״̬����ť
					bShowStatus = false;//״̬��δ��ѡ�б��
				}
				oldEditTextProc = (WNDPROC)SetWindowLong(hEdit, GWL_WNDPROC, (long)EditTextProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ��̡������Ϣ
				SendMessage(hEdit, EM_LIMITTEXT, 0, 0);//��wParamΪ0ʱ�����б༭�ز����0x7FFFFFFE���ַ���Ĭ����30000���ַ�
				SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);//���ñ༭������
				SetWindowText(hEdit, pText);
			}
			return 0;
		case IDM_FORMAT_FONT://����
			if (ChooseFont(&cf))//ѡ������
			{
				DeleteObject(hFont);//ɾ����һ�εĻ�ë
				hFont = CreateFontIndirect(&lf);//�����߼����崴��������
				SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);//�Ա༭������Ϣ�������»���
				InvalidateRect(hwnd, nullptr, true);//���µ�����������ʾ
			}
			break;
		case IDM_FONTCOLOR://������ɫ
			if (ChooseColor(&cc))//�ú�������һ����ʹ�û�����ѡ����ɫ��ͨ����ɫ�Ի���
			{
				textColor = cc.rgbResult;//ѡ���������ɫ
				InvalidateRect(hEdit, nullptr, true);//�༭���ػ�
			};
			break;
		case IDM_BKCOLOR://������ɫ
			if (ChooseColor(&cc))//�ú�������һ����ʹ�û�����ѡ����ɫ��ͨ����ɫ�Ի���
			{
				textBkColor = cc.rgbResult;//ѡ������屳����ɫ
				DeleteObject(hBrush);//ɾ���ɻ�ˢ
				hBrush=CreateSolidBrush(cc.rgbResult);//�ñ���ɫ�������»�ˢ
				InvalidateRect(hEdit, nullptr, true);//�༭���ػ�
			};
			break;
		case IDM_LOOKUP_STATE://״̬��
			if (bShowStatus)//���״̬����ʾ
			{
				bShowStatus = false;//��ǲ���ʾ
				CheckMenuItem(hMenu, IDM_LOOKUP_STATE, MF_UNCHECKED);//ȡ��ѡ��
				ShowWindow(hStatus, SW_HIDE);//����״̬��
				MoveWindow(hEdit, 2, 2, cxClient - 4, cyClient - 4, true);//-4��������Ȼ��߿򣬵����༭��СΪ�����ͻ���		
			}
			else
			{
				bShowStatus = true;//�����ʾ
				CheckMenuItem(hMenu, IDM_LOOKUP_STATE, MF_CHECKED);//ѡ��
				ShowWindow(hStatus, SW_NORMAL);//��ʾ״̬��
				MoveWindow(hEdit, 2, 2, cxClient - 4, cyClient - 29, true);//-4��������Ȼ��߿�-29�ǰѱ߿���״̬�����棬״̬���߶�25����4����Ȼ��߿�
			}
			return 0;
		case IDM_APP_HELP://����
			DialogBox(hInst,TEXT("IDD_DIALOGHELP"),hwnd, DlgHelpProc);//���������Ի���
			return 0;
		case IDM_APP_ABOUT: //����
			MessageBox(nullptr, TEXT("Notepad Program\n(c)yong hua by,2015"), TEXT("����Menu"), MB_ICONINFORMATION | MB_OK);
			return 0;
		}
		break;
	case WM_CTLCOLOREDIT:
			hdc = (HDC)wParam;//wParam�Ǳ༭��Ļ��ʣ�lParam�Ǳ༭��ľ��
			SetBkColor(hdc,textBkColor);//�������屳����ɫ��Ӧ��༭�򱳾���ɫһ�£��������2�ֱ���ɫ���ÿ�
			SetTextColor(hdc, textColor);//�����������ɫ
			return (long)hBrush;//���ﲻ����break,�������������DefWindowProc������ɫ���Ĭ�ϵģ�����һ����ˢ���༭���Ĭ�ϴ��ڹ��̻��������ˢ��Ϊ����ɫ
	case WM_PAINT://������ʧЧ�������ػ�
		hdc = BeginPaint(hwnd, &ps);//��ô��ڻ���
		EndPaint(hwnd, &ps);//�ͷŻ�����Դ
		DrawBorder(hwnd,bShowStatus);
		return 0;
	case WM_TIMER://ÿһ�����һ��ʱ��
		FormatTime();//��ϵͳʱ����и�ʽ��
		SendMessage(hStatus, SB_SETTEXT, 1, (long)statusInfo[1].c_str());//�Եڶ�������ʱ�� 
		return 0;
	case WM_QUERYENDSESSION://�ػ�ʱѯ���Ƿ��˳�����ȷ�����˳��ػ��������ȡ���ػ�
		if (IDYES == AskConfirmation())//���ڹر�ʱѯ���Ƿ��˳�����ȷ�����˳�
			return 1;//�������Ի�����һ���������ϵͳ�رչ��ܵ�ʱ��WM_QUERYENDSESSION��Ϣ�ᱻ���͸���δ��ֹ�����д��ڡ��������ڴ��������Ϣ��ʱ�����������false��0������ôϵͳ���������Ի����߹ػ���ע����
		else
			return 0;
	case WM_CLOSE://���ڹر�
		if (bNeedSave&&IDCANCEL == AskAboutSave(hwnd, szFileName))
			return 0;//����ı������ˣ���Ҫ���б��棬���ش��ǲ������������˳����򣬲�����ִ��
		if (IDYES == AskConfirmation())//���ڹر�ʱѯ���Ƿ��˳�����ȷ�����˳�
		DestroyWindow(hwnd);//���ٴ���
		return 0;
	case WM_DESTROY://��������ʱ
		DeleteObject(hFont);//ɾ����һ�εĻ�ë
		DeleteObject(hBrush);//ɾ���ɻ�ˢ
		KillTimer(hwnd, ID_TIMER);
		PostQuitMessage(0);//�׳�һ������ֵΪ0�˳���Ϣ
		return 0;
	default:
		if (UMsg == WM_FINDREPLACE)//���ܷ���case ����У���Ϊcase���Ҫ��������WM_FINDREPALCE���Լ������һ������
		{
			pfr = (LPFINDREPLACE)lParam;//���յ������Ϣ��lparam����������FINDREPLACE�ṹָ��
			if (pfr->Flags&FR_DIALOGTERM)//��������˹رհ�ť
				hDlgModeLess = 0;//�ԶԻ������ÿ�
			if (pfr->Flags&FR_DOWN)//���°�ť������
				bDown = true;
			else
				bDown = false;
			if (pfr->Flags&FR_FINDNEXT)//��������˲�����һ��
			{
				if(!MyFindText(hEdit,*pfr,bDown))//������һ��
					OKMessage(hDlgModeLess, TEXT("�Ҳ���\"%s\""),pfr->lpstrFindWhat);//���Ҳ���
			}
			if (pfr->Flags&FR_REPLACE)//����������滻��һ��
			{
				SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//��ǰѡ�����ֵĿ�ʼ������λ�ã���δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������			
				if (startPos != endPos)//�����ʼ�ͽ���λ�ò����˵��ѡ�������֣������滻
				SendMessage(hEdit, EM_REPLACESEL, 0,(long) pfr->lpstrReplaceWith);//�滻ѡ�е��ַ���
				if(!MyFindText(hEdit,*pfr,bDown))//������һ��
					OKMessage(hDlgModeLess, TEXT("�Ҳ���\"%s\""),pfr->lpstrFindWhat);//���Ҳ���
			}
			if (pfr->Flags&FR_REPLACEALL)//����������滻ȫ��
			{
				int count = 0;//�滻�ĸ���
				SendMessage(hEdit, EM_SETSEL, 0, 0);//���õ�ǰ������ļ���ͷ
				TCHAR szBuffer[64];
				while (MyFindText(hEdit, *pfr, bDown))//������һ��
				{
					SendMessage(hEdit, EM_REPLACESEL, 0, (long)pfr->lpstrReplaceWith);//�滻ѡ�е��ַ���
					count++;
				}
				StringCchPrintf(szBuffer, 64, TEXT("���滻%d�� \"%s\""), count, pfr->lpstrFindWhat);
				MessageBox(hDlgModeLess, szBuffer, szAppName, MB_OK | MB_ICONINFORMATION);
			}
		}
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);//��Ĭ�ϵĴ��ڹ��̴���������Ϣ����Щ��������
}
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,TCHAR *  szCmdCommand, int nCmdShow)
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
	wndclass.lpszClassName = szAppName;//����
	wndclass.lpszMenuName = TEXT("POPADMENU");//�˵�
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//��Щ�ı��Ҫ�ػ�������ˮƽ�ʹ�ֱ�ı���ػ�
	if (!RegisterClass(&wndclass))//ע�ᴰ����
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindowEx(WS_EX_ACCEPTFILES,szAppName, nullptr, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance,szCmdCommand);//��������
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
		if (hDlgModeLess == 0 || !IsDialogMessage(hDlgModeLess, &msg))//��Ϊ��ģ̬�Ի���������Ϣ���У��˾��ǽ�����޴���䣬�ڶԻ����в��������κ���Ϣ���ڶԻ�������ȴ�������
			//�༭�򣬼������䣬��������Ϣ����ɶԻ�����Ϣ�����ڰ������ɶԻ����еİ���
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
}