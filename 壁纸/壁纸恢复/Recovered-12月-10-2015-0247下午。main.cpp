#include"resource.h"
#include"tinyxml.h"
#include<ZYHFile.h>
#undef _STRSAFE_H_INCLUDED_
#include<ZYHGlobal.h>

//������ɫ
void saveColor(TiXmlElement *parent,COLORREF cr,string colorName)
{
	TiXmlElement *element=new TiXmlElement(colorName);
	element->SetIntAttribute("r", (int)(BYTE)cr);
	element->SetIntAttribute("g", (int)(BYTE)(cr >> 8));
	element->SetIntAttribute("b", (int)(BYTE)(cr >> 16));
	parent->LinkEndChild(element);
}
//����½ڵ㣬���ڽڵ������text
template <class T>
void addTextToNewElement(TiXmlElement *parent,string elementName, T value)
{
	TiXmlElement *subElement;
	(subElement = new TiXmlElement(elementName))->LinkEndChild(new TiXmlText(value));
	parent->LinkEndChild(subElement);
}
//����Ŀ¼
void saveDirectory(TiXmlElement *element,string name, vector<tstring> &directory)
{
	if(element)
	{
		for (size_t i = 1; i <= directory.size(); i++)
		{
#ifdef UNICODE
			addTextToNewElement(element, name + to_string(i), WCharToAnsi(directory[i-1]).c_str());
#else
			addTextToNewElement(element, name + to_string(i),directory[i-1]);
#endif
		}
	}
}
//������д��XML�ļ�����
void writeToXML(HWND hwnd)
{
	CreateArchiveDirectory(hwnd, TEXT("wallpaper"));//���û��wallpaperĿ¼�򴴽�
	tstring save = tstring(szMyDocument) + TEXT("\\wallpaper\\wallpaper.xml");//�浵�ļ�ȫ·��
	//����һ��XML���ĵ�����
#ifdef UNICODE
	TiXmlDocument myDocument(WCharToAnsi(save));
#else
	TiXmlDocument myDocument(save);
#endif
	TiXmlElement *element;
	//����һ����Ԫ�ز����ӡ�
	TiXmlElement *RootElement=new TiXmlElement("Wallpaper");
	myDocument.LinkEndChild(new TiXmlDeclaration("xml 2.0", "ansi"));//�������
	myDocument.LinkEndChild(new TiXmlComment("���»���ֽ�����ļ�"));//���ע��
	myDocument.LinkEndChild(RootElement);//��Ӹ��ڵ�

	element = new TiXmlElement("��ֽ����ʱ����");//�����ֽʱ��ṹ
	element->SetIntAttribute("ʱ", Wallpaper.hour);
	element->SetIntAttribute("��", Wallpaper.minute);
	element->SetIntAttribute("��", Wallpaper.second);
	RootElement->LinkEndChild(element);

	element = new TiXmlElement("����������ʾ���");//��������ʱ��ṹ
	element->SetIntAttribute("ʱ", Notify.hour);
	element->SetIntAttribute("��", Notify.minute);
	element->SetIntAttribute("��", Notify.second);
	RootElement->LinkEndChild(element);

	element = new TiXmlElement("���ƿ��ر���");//������ƿ��ر���
	element->LinkEndChild(new TiXmlComment("���ƿ��ر���,����yes,true��1��ʾ�����������ǹرոù���"));
	addTextToNewElement(element, "�������ֽ", randomChange);
	addTextToNewElement(element,"��������������" , startingUpBoot);
	addTextToNewElement(element,"������û���κδ��ڲŸ�����ֽ" , changeByOnlyDestop);
	addTextToNewElement(element, "����ͼƬʱ������Ŀ¼", includeSubDirectory);
	addTextToNewElement(element, "�Ƿ�ʼ��", startChange);
	RootElement->LinkEndChild(element);


	element = new TiXmlElement("������Ϣ");//����������Ϣ
#ifdef UNICODE
	addTextToNewElement(element,"������ʾ",WCharToAnsi( szTip));
	addTextToNewElement(element, "���ݱ���", WCharToAnsi(szInfoTitle) );
	addTextToNewElement(element, "������Ϣ", WCharToAnsi(szInfo));
#else
	addTextToNewElement(element, "������ʾ", szTip);
	addTextToNewElement(element, "���ݱ���", szInfoTitle);
	addTextToNewElement(element, "������Ϣ", szInfo);
#endif
	RootElement->LinkEndChild(element);


	element = new TiXmlElement("���ڴ�Сλ��");//��Сλ��
	if(IsWindowVisible(hwnd))
	{//��ȡ���´��ڴ�С��Ϣ
		GetWindowRect(hwnd, &windowRect);//��ô��ڴ�С��Ϣ
		windowRect.right -= windowRect.left;//������λ�ñ���Ϊ���ڴ�С
		windowRect.bottom -= windowRect.top;
	}
	element->SetIntAttribute("x", windowRect.left);
	element->SetIntAttribute("y", windowRect.top);
	element->SetIntAttribute("width", windowRect.right);
	element->SetIntAttribute("heigh", windowRect.bottom);
	RootElement->LinkEndChild(element);

	element = new TiXmlElement("��ǰ��ֽ����");//��ǰ��ֽ����
	element->SetIntAttribute("index", nSelectWallpaper);
	RootElement->LinkEndChild(element);

	element = new TiXmlElement("������Ϣ");//��������ṹ
	addTextToNewElement(element,"�ַ���ƽ�����" ,lf.lfWidth );
	addTextToNewElement(element,"����Ĵ�ϸ�̶�" , lf.lfWeight);
	addTextToNewElement(element,"�ַ��ĸ߶�" , abs(lf.lfHeight));
#ifdef UNICODE
	addTextToNewElement(element,"������" ,WCharToAnsi(lf.lfFaceName));
#else
	addTextToNewElement(element, "������", lf.lfFaceName);
#endif
	addTextToNewElement(element,"ʹ��б��" ,(bool)lf.lfItalic );
	addTextToNewElement(element, "���ɾ����", (bool)lf.lfStrikeOut);
	addTextToNewElement(element, "����»���", (bool)lf.lfUnderline);
	RootElement->LinkEndChild(element);

	saveColor(RootElement, textColor, "������ɫ");//������ɫ
	saveColor(RootElement, textBkColor, "���屳����ɫ");
	element = new TiXmlElement("�Զ�����ɫ");
	for (size_t i = 1; i <=16; i++)
		saveColor(element, color[i], string("�Զ�����ɫ") + to_string(i));
	RootElement->LinkEndChild(element);

	//����Ŀ¼
	element = new TiXmlElement("��ǰ��ֽĿ¼");//��ǰ��ֽĿ¼
	saveDirectory(element, "Ŀ¼", currentDirectorys);
	RootElement->LinkEndChild(element);
	element = new TiXmlElement("���ݱ�ֽĿ¼");//���ݱ�ֽĿ¼
	saveDirectory(element, "Ŀ¼", backupDirectorys);
	RootElement->LinkEndChild(element);
	myDocument.SaveFile();
}

//��ȡʱ��
void readTime(Time &time,TiXmlElement *element)
{
	if (element)
	{
		int temp = 0;
		element->Attribute("ʱ", &temp);
		time.hour = makeInRange(0, temp, 100);
		element->Attribute("��", &temp);
		time.minute = makeInRange(0, temp,65535);
		element->Attribute("��", &temp);
		time.second = makeInRange(0, temp,65535);
	}
}
//��ȡ��ɫ
void readColor(TiXmlElement *element,COLORREF &cr)
{
	int r, g, b;
	if(element)
	{
		element->Attribute("r", &r);
		element->Attribute("g", &g);
		element->Attribute("b", &b);
		r = makeInRange(0, r, 255);
		b = makeInRange(0, b, 255);
		g = makeInRange(0, g, 255);
		cr = RGB(r, g, b);
	}
}
//��ȡ�ı�
void GetText(TiXmlElement *element,TCHAR *save,int length,char *name)
{
	TiXmlElement *subElement = element->FirstChildElement(name);
	const char *text =subElement->GetText();
	if (subElement&&text)
	{
#ifdef UNICODE
		StringCchCopy(save, length,AnsiToWChar(text).c_str());
#else
		StringCchCopy(save, length,text);
#endif
	}
}
//��ȡint�ı�
void GetTextFromInt(TiXmlElement *element, int &save, char *name)
{
	TiXmlHandle handle(element);
	TiXmlText *text = handle.FirstChildElement(name).FirstChild().ToText();
	if(text)
		  save=text->IntValue();
}
//��ȡbool�ı�
void GetTextFromBool(TiXmlElement *element, bool &save, char *name)
{
	TiXmlHandle handle(element);
	TiXmlText *text = handle.FirstChildElement(name).FirstChild().ToText();
	if (text)
			save = text->BoolValue();
}
//��ȡĿ¼
void readDirectory(TiXmlElement *element,vector<tstring>&directory)
{
	if(element)
	{
		for (TiXmlElement *sub = element->FirstChildElement(); sub;sub=sub->NextSiblingElement())
		{
			const char *text = sub->GetText();
			if(text)
			{
#ifdef UNICODE
				directory.push_back(AnsiToWChar(text));
#else 
				directory.push_back(text);
#endif
			}
		}
	}
}
//��xml��ȡ�浵
bool readFromXML(HWND hwnd)
{
	if (!CreateArchiveDirectory(hwnd,TEXT("wallpaper")))//���û��Ŀ¼����Ŀ¼
		return false;//Ŀ¼���޴浵�˳�
	tstring save = tstring(szMyDocument) + TEXT("\\wallpaper\\wallpaper.xml");//�浵�ļ�ȫ·��
#ifdef UNICODE
	TiXmlDocument myDocument(WCharToAnsi(save));
#else
	TiXmlDocument myDocument(save);
#endif
	if(!checkFileExists(save.c_str()))
		return false;
	bool loadOkay = myDocument.LoadFile();
	if (!loadOkay)
		return false;
	TiXmlElement *rootElement=0, *element=0,*subElement=0;
	rootElement=myDocument.FirstChildElement("Wallpaper");
	if (!rootElement)
		return false;
	element = rootElement->FirstChildElement("��ֽ����ʱ����");
	readTime(Wallpaper, element);//��ȡ��ֽ����ʱ����
	element = rootElement->FirstChildElement("����������ʾ���");
	readTime(Notify, element);//��ȡ����������ʾ���

	//��ȡ���ƿ��ر���
	element = rootElement->FirstChildElement("���ƿ��ر���");
	if(element)
	{
		GetTextFromBool(element,randomChange , "�������ֽ");
		GetTextFromBool(element,startingUpBoot ,"��������������" );
		GetTextFromBool(element,changeByOnlyDestop , "������û���κδ��ڲŸ�����ֽ");
		GetTextFromBool(element, includeSubDirectory,"����ͼƬʱ������Ŀ¼" );
		GetTextFromBool(element, startChange,"�Ƿ�ʼ��" );
	}

	//��ȡ������Ϣ
	element = rootElement->FirstChildElement("������Ϣ");
	if(element)
	{
		GetText(element, szTip,128, "������ʾ");
		GetText(element, szInfo,256, "������Ϣ");
		GetText(element, szInfoTitle,64, "���ݱ���");
	}

	//��ȡ���ڴ�Сλ��
	element = rootElement->FirstChildElement("���ڴ�Сλ��");
	if(element)
	{
		int temp;
		element->Attribute("x", &temp);
		windowRect.left = makeInRange(0, temp, cxScreen);
		element->Attribute("y", &temp);
		windowRect.top = makeInRange(0, temp, cyScreen);
		element->Attribute("width", &temp);
		windowRect.right = makeInRange(0, temp, cxScreen);
		element->Attribute("heigh", &temp);
		windowRect.bottom = makeInRange(0, temp, cyScreen);
	}

	//��ȡ��ǰ��ֽ����
	element = rootElement->FirstChildElement("��ǰ��ֽ����");
	if (element)
	{
		int temp;
		element->Attribute("index", &temp);
		nSelectWallpaper = makeInRange(1, temp,65535);
	}
	//��ȡ������Ϣ
	element = rootElement->FirstChildElement("������Ϣ");
	if (element)
	{
		int temp;
		GetTextFromInt(element,temp, "�ַ���ƽ�����");
		lf.lfWidth = makeInRange(0, temp, 100);
		GetTextFromInt(element, temp, "�ַ��ĸ߶�");
		lf.lfHeight = makeInRange(-100, temp, 100);
		GetTextFromInt(element, temp, "����Ĵ�ϸ�̶�");
		lf.lfWeight = makeInRange(0, temp, 1000);
		GetText(element, lf.lfFaceName,LF_FACESIZE, "������");
		bool tempBool;
		GetTextFromBool(element, tempBool, "ʹ��б��");
		lf.lfItalic = tempBool;
		GetTextFromBool(element, tempBool, "���ɾ����");
		lf.lfStrikeOut = tempBool;
		GetTextFromBool(element, tempBool, "����»���");
		lf.lfUnderline = tempBool;
	}

	//��ȡ��ɫ��Ϣ
	element = rootElement->FirstChildElement("������ɫ");
	readColor(element, textColor);
	element = rootElement->FirstChildElement("���屳����ɫ");
	readColor(element, textBkColor);
	element = rootElement->FirstChildElement("�Զ�����ɫ");
	if(element)
	{
		for (int i = 1; i <= 16;i++)
		{
			string name = string("�Զ�����ɫ") + to_string(i);
			subElement = element->FirstChildElement(name);
			readColor(subElement, color[i]);
		}
	}

	//��ȡĿ¼
	element = rootElement->FirstChildElement("��ǰ��ֽĿ¼");
	readDirectory(element, currentDirectorys);
	element = rootElement->FirstChildElement("���ݱ�ֽĿ¼");
	readDirectory(element, backupDirectorys);
	return true;
}
//���������˵�
HMENU CreateMyPopMenu(HINSTANCE hInst)
{
	HMENU hMenu = CreatePopupMenu();//���������˵�
	//��Ӳ˵�
	AppendMenu(hMenu, MF_STRING, IDM_SHOWWALLPAPER, TEXT("�鿴�����ֽ\tCtrl+Alt+M"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(hMenu, MF_STRING, IDM_NEXTWALLPAPER, TEXT("��һ��\tAlt+N"));
	AppendMenu(hMenu, MF_STRING, IDM_PREWALLPAPER, TEXT("��һ��\tAlt+P"));
	AppendMenu(hMenu, MF_STRING, IDM_PAUSE, startChange ? TEXT("��ͣ\tCtrl+P") : TEXT("��ʼ\tCtrl+P"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(hMenu, MF_STRING, IDM_RANDOM, randomChange ? TEXT("˳��\tAlt+R") :TEXT("���\tAlt+R"));
	AppendMenu(hMenu, MF_STRING, IDM_MINI,TEXT("���д�����С���Ÿ�����ֽ"));
	AppendMenu(hMenu, MF_STRING, IDM_SET, TEXT("��ֽ����\tAlt+S"));
	AppendMenu(hMenu, MF_STRING, IDM_TOOLPRINT, TEXT("��ͼ\tCtrl+Alt+P"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("�˳�\tAlt+F4"));
	CheckMenuItem(hMenu, IDM_MINI, changeByOnlyDestop ? MF_CHECKED : MF_UNCHECKED);//����ѡ��״̬
	SetMenuItemPhoto(hInst, hMenu,TEXT("IDB_BITMAP1") , IDM_SHOWWALLPAPER);
	SetMenuItemPhoto(hInst, hMenu,TEXT("IDB_BITMAP2") , IDM_NEXTWALLPAPER);
	SetMenuItemPhoto(hInst, hMenu, TEXT("IDB_BITMAP3") ,IDM_PREWALLPAPER );
	SetMenuItemPhoto(hInst, hMenu, TEXT("IDB_BITMAP4"), IDM_PAUSE);
	SetMenuItemPhoto(hInst, hMenu, TEXT("IDB_BITMAP5") ,IDM_EXIT );
	SetMenuItemPhoto(hInst, hMenu, TEXT("IDB_BITMAP6"),IDM_RANDOM );
	SetMenuItemPhoto(hInst, hMenu,TEXT("IDB_BITMAP7") ,IDM_MINI );
	SetMenuItemPhoto(hInst, hMenu,TEXT("IDB_BITMAP8") , IDM_SET);
	SetMenuItemPhoto(hInst, hMenu, TEXT("IDB_BITMAP9") ,IDM_TOOLPRINT );
	return hMenu;
}

void FindFile(tstring CurrentPath,tstring type)//�����ļ�
{
	WIN32_FIND_DATA fd;
	tstring find;//��ǰҪ�����ļ���ʽ��·��
	find=CurrentPath +type;//typeָ���ļ���Ҫ���ҵ����ͣ���d:\\wallpaper\\*.jpg
	HANDLE handle = FindFirstFile(find.c_str(), &fd);//���ҵ�1���ļ�
	if (handle != INVALID_HANDLE_VALUE)//�ҵ���
	{
		do
		{
			tempDirectory[fd.cFileName] = CurrentPath;//����ǰͼƬ������·������map�б���
		} while (FindNextFile(handle, &fd));//������һ���ļ�
		FindClose(handle);//�ر��ļ����
	}
}
void FindAllFile(tstring CurrentPath, bool includeSubDirectory)//�������б�ֽ
{//CurrentPath��ǰ·��,includeSubDirectoryΪtrue����ʾ��ǰĿ¼����Ŀ¼�ı�ֽ��Ϊfalse��ֻ��ʾ��ǰĿ¼��ֽ
	WIN32_FIND_DATA fd;//fd�ļ���Ϣ�ṹ
	HANDLE handle;//handle�ļ�����
	tstring find;//formatָ���ļ���Ҫ���ҵ����ͣ���d:\\wallpaper\\*.jpg
	for (size_t i = 0; i < format.size(); i++)//�������и�ʽ(jpg,bmp,png)��ͼƬ
		FindFile(CurrentPath,format[i]);//���ҵ�ǰ�ļ���ʽ��·��
	if (includeSubDirectory)//���Ҫ������Ŀ¼��ͼƬ
	{
		find = CurrentPath +TEXT("\\*.*");//���������ļ�
		handle = FindFirstFile(find.c_str(), &fd);//���ҵ�1���ļ�
		if (handle!= INVALID_HANDLE_VALUE)//�ҵ���
		{
			do
			{
				if (fd.cFileName[0] == '.')//win32��һ����������ļ��У�һ���ǡ�.��ָ���ǵ�ǰ�ļ��У�һ���ǡ�..��ָ�����ϲ��ļ��У����һ�־�����ͨ���²��ļ������ļ�������ʾ��cd .�Ǳ�Ŀ¼��cd ..���ϼ�Ŀ¼
					continue;//����Ϊ�˲��ص��ϼ�Ŀ¼����ԭĿ¼��ת������.��..�����Ŀ¼
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//ֻҪFILE_ATTRIBUTE_DIRECTORY��0x00000010����ֻҪ����������5λΪ1�������ļ�Ŀ¼������λ�Ǳ��ֻ����ֻд�����ص�����
					FindAllFile(CurrentPath+TEXT("\\")+tstring(fd.cFileName),includeSubDirectory);
			} while (FindNextFile(handle, &fd));//FindFirstFile����ʱָ����һ���ļ���������һ���ļ�
			FindClose(handle);//�ر��ļ����
		}
	}
}
int _stdcall EnumWindowProc(HWND hwnd, long lParam)//ö�ٺ����Ĵ��ڹ���
{
	TCHAR title[256];//���մ��ڱ���
	if (IsWindow(hwnd) && IsWindowVisible(hwnd) && !IsIconic(hwnd))
	{//�������������ڣ����Ҵ��ڿɼ���������С��
		GetWindowText(hwnd, title, 256);//�õ����ڱ���
		if (title[0] != '\0'&&lstrcmp(title,TEXT("��ʼ"))!=0)//������ⲻΪ��,���߲��ǿ�ʼ������˵�WIN7��ʼ��ť
			allTitle.push_back(title);//�����������
		temp += tstring(title)+TEXT("   ");
	}
	return true;//Ϊ����ö����һ�����ڣ��ص��������뷵��TRUE����ֹͣ�б������뷵��FALSE
}

void PopupMyMenu(HWND hwnd, HMENU hMenu)//�����˵�
{
	POINT mousePos;//���λ��
	GetCursorPos(&mousePos);//�õ����λ��
	SetForegroundWindow(hwnd);//�������ö������ܲ�׽���ڲ˵�����굥���Ĳ���������ڲ˵��ⵥ������˵�����ʧ������
	if (recordIndex == 0 || wallPaperCount==0)//�����ǰ��ֽ�ǻ����ı�ֽ��¼�е�һ�Ż����ޱ�ֽ
		EnableMenuItem(hMenu,IDM_PREWALLPAPER, MF_GRAYED);//��һ��ͼƬ�˵���Ϊ����
	else
		EnableMenuItem(hMenu,IDM_PREWALLPAPER,MF_ENABLED);//��һ��ͼƬ�˵���Ϊ����
	EnableMenuItem(hMenu, IDM_NEXTWALLPAPER,wallPaperCount? MF_ENABLED:MF_GRAYED);//��һ��ͼƬ�˵���Ϊ���û�������
	EnableMenuItem(hMenu,IDM_PAUSE, wallPaperCount ?  MF_ENABLED:MF_GRAYED );//��ʼ�˵���Ϊ���û�������
	int id = TrackPopupMenu(hMenu, TPM_RETURNCMD, mousePos.x, mousePos.y, 0, hwnd, 0);//�����˵�,TPM_RETURNCMD���ز˵�ID
	switch (id)
	{
	case IDM_EXIT:
		SendMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	case IDM_SHOWWALLPAPER://�鿴�����ֽ
		SendMessage(hwnd, WM_COMMAND, IDM_SHOWWALLPAPER, 0);//������Ϣ����ʾ����
		break;
	case IDM_PREWALLPAPER://��һ��
		SendMessage(hwnd, WM_COMMAND, IDM_PREWALLPAPER, 0);
		break;
	case IDM_NEXTWALLPAPER://��һ��
		SendMessage(hwnd, WM_COMMAND,IDM_NEXTWALLPAPER, 0);
		break;
	case IDM_PAUSE://��ͣ
		SendMessage(hwnd, WM_COMMAND, IDM_PAUSE, 0);
		break;
	case IDM_RANDOM://����ֽģʽ�����������˳��
		SendMessage(hwnd, WM_COMMAND,IDM_RANDOM, 0);
		break;
	case IDM_MINI://���д�����С���Ÿ�����ֽ�˵�
		SendMessage(hwnd, WM_COMMAND,IDM_MINI, 0);
		break;
	case IDM_SET://���ñ�ֽ
		DialogBox((HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE), TEXT("IDD_DIALOGSET"), hwnd, DlgWallpaperSet);
		break;
	case IDM_TOOLPRINT://��ͼ
		SendMessage(hwnd, WM_COMMAND, IDM_TOOLPRINT, 0);
		break;
	}
}
//�Ե�ǰ�ı�ֽ���и�����ʾ
void HighLineWallPaper(HWND hEdit, int  nSelectWallpaper)
{
	if (nSelectWallpaper > 0)//�����Ϊ0
	{
		lineIndex = SendMessage(hEdit, EM_LINEINDEX,nSelectWallpaper, 0);//�õ�ָ�������ַ����ı��е�λ�ã����ֽ�����ʾ��
		nextLineIndex = SendMessage(hEdit, EM_LINEINDEX, nSelectWallpaper +1, 0);//�õ�ָ������һ�����ַ����ı��е�λ�ã����ֽ�����ʾ��
		SendMessage(hEdit, EM_SETSEL, lineIndex, nextLineIndex);//ѡ��ָ����
		SendMessage(hEdit, EM_SCROLLCARET, 0, 0);// �ѿɼ���Χ������괦,�����ù��󣬹����ܲ��ڷ�Χ�ڣ�����Ҫ��������괦
		if (!buttonDown)//������δ������˵�����Զ���
		   SendMessage(hEdit, WM_VSCROLL, SB_LINEUP, 0);//��Ϊ��ѡ�пͻ������һ��ʱ�������ڿͻ������һ�е���һ�У�EM_SCROLLCARET�Ὣ������ѡ���е���һҳ��������������ʾ��Ҫ���Ϲ�һ�в��ܿ��������Է���WM_VSCROLL���Ϲ�һ�У���ʾ����
	}
}
void DrawBorder(HWND hwnd, HBRUSH hBlueBrush)//������ɫ�߿�
{
	RECT rect;
	HDC hdc;
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);//�õ��ͻ�������
	rect.bottom -= 26;//�߿򲻰�Χ״̬������ȥ״̬���߶�
	FrameRect(hdc, &rect, hBlueBrush);//������ɫ��ˢ�߿�
	ReleaseDC(hwnd, hdc);
}
//��ʾ�ļ�����
void DisplayDistribitue()
{
	TCHAR szBuffer[1024];//�ַ�����
	WIN32_FILE_ATTRIBUTE_DATA attri;//�ļ����Խṹ
	tstring szPath;//�ļ�·��
	TCHAR szName[31];//�ļ���
	static TCHAR *szColumn[8] = {TEXT("����"),TEXT("�ļ���"),TEXT("��С"),TEXT("����"),TEXT("����ʱ��"),TEXT("���һ�η���ʱ��"),TEXT("���һ���޸�ʱ��"),TEXT("·��")};
	StringCchPrintf(szBuffer, 1024, TEXT("%-9s%-30s%-20s%-15s%-30s%-30s%-30s%-30s\r\n"),szColumn[0],szColumn[1],
		szColumn[2], szColumn[3], szColumn[4], szColumn[5], szColumn[6], szColumn[7]);//��ʽ������
	tstring temp = szBuffer;//�ڱ༭����ʾ���ַ���
	tstring index;//����
	wallPaperCount = 0;//�ļ���������ʼ��Ϊ0
	directoryIndex.clear();//����һ���������
	for (size_t i = 0; i<AllDirectorys.size(); i++)//��ʾ����Ŀ¼����Ϣ
	{
		directoryIndex.push_back(wallPaperCount);//����ǰĿ¼��һ��ͼƬ��������
		for (auto iter=AllDirectorys[i].begin(); iter!=AllDirectorys[i].end();iter++)//��ʾ��ǰĿ¼��Ϣ
		{
			wallPaperCount++;//�ļ�������1
			GetFileAttributesEx((iter->second+TEXT("\\")+iter->first).c_str(), GetFileExInfoStandard, &attri);//�õ���ǰ�ļ�������,GetFileExInfoStandard��ʾ��׼�ļ����ԣ����һ�������� WIN32_FILE_ATTRIBUTE_DATA�ṹ��ָ��
			fileCreateTime = FormatFileTime(attri.ftCreationTime);//�ļ�����ʱ��
			fileLastAccessTime = FormatFileTime(attri.ftLastAccessTime); // �ļ����һ�η���ʱ��
			fileLastWriteTime = FormatFileTime(attri.ftLastWriteTime);//�ļ����һ���޸�ʱ��
			fileAttributes = FileAttribute(attri.dwFileAttributes);//�ļ�����
			szPath = (iter->second).c_str();//�ļ�·��
			lstrcpyn(szName,(iter->first).c_str(),30);//�����ļ�������ǰ30���ַ�
			if ((iter->first).size()>29)//����ļ�������29���ļ���̫��
				lstrcpyn(szName+24, TEXT("*.jpg"),6);//�����5���ַ���Ϊ*.jpg�Ա�ʾ�ļ���ʡ��
			fileSize = calculateCapacity(attri.nFileSizeHigh, attri.nFileSizeLow);//�����ļ���С
			index = ToString(wallPaperCount) + TEXT(":");//���ļ���������ת�����ַ���
			StringCchPrintf(szBuffer, 1024, TEXT("%-9s%-30s%-20s%-15s%-30s%-30s%-30s%-30s\r\n"), index.c_str(), szName, fileSize.c_str(),
			fileAttributes.c_str(), fileCreateTime.c_str(), fileLastAccessTime.c_str(), fileLastWriteTime.c_str(),
			szPath.c_str());//��ʽ���ַ���
			temp += szBuffer;
		}
	}
	SetWindowText(hEdit, temp.c_str());//�ڱ༭����ʾ
}
void ChangeWallPaper()//�ı��ֽ
{
	int nDirectory = AllDirectorys.size() - 1;//�ĸ�Ŀ¼,��ʼ��Ϊ���һ��Ŀ¼�ĵ�����
	int nIndex;//��Ŀ¼������ͼƬ(����)			
	for (int i = 0; i <= nDirectory; i++)
	{
		if (nSelectWallpaper - 1 < directoryIndex[i])//��Ϊ���������С���������ǰѡ�е�ͼƬ����(������0��ʼ��Ҫ��1)�ȵ�ǰĿ¼�����һ��ͼƬС
		{//��iΪ��ǰĿ¼����
			nDirectory = --i;//���浱ǰͼƬ����һ��Ŀ¼������
			break;//�ҵ�Ŀ¼�ˣ��˳�ѭ��
		}
	}
	nIndex = (nSelectWallpaper - 1) - directoryIndex[nDirectory];//��ȥ��ǰĿ¼��һ��Ŀ¼����ͼƬ����������ø�Ŀ¼������ͼƬ(����)����Ϊ(������0��ʼ������Ҫ��1)
	auto tempMap = AllDirectorys[nDirectory];//��ǰĿ¼��map�ṹ
	auto begin = tempMap.begin();//��ȻͼƬĿ¼�ĵ�1��������
	for (int i = 0; i <nIndex; i++)
		begin++;//���������Ƶ���ǰͼƬ��������
	tstring szPath = begin->second;//�ļ�·��
	tstring szName = begin->first;//�ļ���
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)(szPath + TEXT("\\") + szName).c_str(), 0);//��˫����ͼƬ����Ϊ��ֽ
	statusInfo[0] = tstring(TEXT("��ǰ��ֽ:")) + szPath + TEXT("\\") + szName;
	SendMessage(hStatus, SB_SETTEXT, 0, (long)statusInfo[0].c_str());//��һ����ʾ·��	
}

int _stdcall EditTextProc(HWND hEdit, unsigned message, unsigned wParam, long lPram)//��д�༭�ؼ����ڹ���
{
	int first;//��ǰ��������У����ַ��ڱ༭�е�λ��
	int startPos;//��ǰѡ�����ֵĿ�ʼλ�ã���δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������
	int endPos;//��ǰѡ�����ֵĽ���λ��+1����δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������
	switch (message)
	{
	case WM_KEYUP://�������ɿ���
	case WM_LBUTTONUP://���������ɿ���ʱ�򣬸���������Ϣ
		if (wParam == VK_F3)//����ǲ���һһ�п�ݼ�
			break;//�˳�
		row = SendMessage(hEdit, EM_LINEFROMCHAR, -1, 0) + 1;//���ָ��λ�����ڵ��кţ�-1��ʾ��ǰ�������λ�ã���Ϊλ�õ�������0��ʼ������Ҫ��1
		statusInfo[2] = TEXT("�к�:") + ToString(row);//���к�ת�����ַ���
		SendMessage(hStatus, SB_SETTEXT, 2, (long)statusInfo[2].c_str());//�Եڶ��������к� 
		first = SendMessage(hEdit, EM_LINEINDEX, -1, 0);//��ǰ��������У����ַ��ڱ༭�е�λ��
		SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//��ǰѡ�����ֵĿ�ʼ������λ�ã���δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������			
		col = startPos - first + 1;//��ǰ����λ��-��ǰ��������е�1���ַ���λ��Ϊ����������Ϊλ�õ�������0��ʼ������Ҫ��1
		statusInfo[3] = TEXT("�к�:") + ToString(col);//���к�ת�����ַ���
		SendMessage(hStatus, SB_SETTEXT, 3, (long)statusInfo[3].c_str());//�Ե����������к� 
		if (row >= 2)//�������ѡ���1��
		{
			nSelectWallpaper = row -1;//��Ϊ��1���������ƣ���2���ǵ�1�ţ���3���ǵ�2�ţ����Ե�ǰѡ�е�������row-1��
			HighLineWallPaper(hEdit, nSelectWallpaper);//�Ե�ǰ�и�����ʾ������δ����Ϊ��ǰ��Ϊ��ֽ��ֻ�����˫��������
		}
		buttonDown =false;//�������ɿ�
		break;
	case WM_LBUTTONDBLCLK:
		SendMessage(hEdit, EM_SETSEL, lineIndex,lineIndex);//��Ϊ��������ָ������һ�У���ΪWM_LBUTTONDBLCLK����WM_LBUTTONUP��Ϊ�˼���ѡ�е�ǰ�У�Ҫ��������ȥ
		if (row >= 2 && row <= wallPaperCount + 1)//����ڵ�2�е����һ��,+1����Ϊ������Ҫ������1��
		{
			ChangeWallPaper();//�ı��ֽ
			recordChange.push_back(nSelectWallpaper);//���滻���ı�ֽ�еļ�¼�е�����
			recordIndex = recordChange.size() - 1;//��ǰ��ֽ�ڻ����ı�ֽ�еļ�¼�е����������һ������
		}
		buttonDown = true;//����������
		break;
	case WM_RBUTTONDOWN://�Ҽ�����
		PopupMyMenu(GetParent(hEdit), hMenu);//�����˵�
		break;
	case WM_LBUTTONDOWN:
		buttonDown = true;//����������
		break;
	}
	return CallWindowProc(oldEditTextProc, hEdit, message, wParam, lPram);//����Ĭ�ϵı༭�ؼ����ڴ�����̴���������Ϣ��δ������Ĳ���
}
void OKMessage(HWND hwnd, TCHAR szFormat[], TCHAR szFindText[])//����һ��OK��ʾ��
{
	TCHAR szBuffer[MAX_PATH];
	StringCchPrintf(szBuffer, MAX_PATH, szFormat, szFindText);//���и�ʽ���ַ���
	MessageBox(hwnd, szBuffer,TEXT("�Ҳ���"), MB_OK | MB_ICONEXCLAMATION);//������Ϣ��
}
bool MyFindText(FINDREPLACE &fr, bool bDown)//�����ı�
{
	int startPos;//��ǰѡ�����ֵĿ�ʼλ�ã���δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������
	int endPos;//��ǰѡ�����ֵĽ���λ��+1����δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������
	int len = GetWindowTextLength(hEdit);//��ñ༭���ַ�����
	TCHAR *pText = new TCHAR[len + 1];//����ռ�
	shared_ptr<TCHAR>sp1(pText, [](TCHAR *p){delete[]p; });//������Ŀռ佻����ָ�����	
	GetWindowText(hEdit, pText, len + 1);//��ñ༭���ַ�
	int findLen = lstrlen(fr.lpstrFindWhat);//Ҫ���ҵ��ַ�������
	int pos;//���ҵ���λ��
	SendMessage(hEdit, EM_GETSEL, (unsigned)&startPos, (long)&endPos);//��ǰѡ�����ֵĿ�ʼλ�á���������δѡ�����ǵ�ǰ�������ڿ�ͷλ�õ�ƫ�ƣ��ַ�������
	if (startPos != endPos&&bDown)//��ʼ����λ�ò�ͬ��˵��ѡ�����ֲ��������²���
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
		auto p = search(pText + startPos, pText + len, fr.lpstrFindWhat, fr.lpstrFindWhat + findLen);//���ҵ�һ��ƥ����ַ���λ��
		if (p == pText + len)//���²��Ҳ���
			return false;
		pos = p - pText;
	}
	else
	{
		auto p = find_end(pText, pText + startPos, fr.lpstrFindWhat, fr.lpstrFindWhat + findLen);//���Ҵ����һ��ƥ����ַ���λ��
		if (p == pText + startPos)//���ϲ��Ҳ���
			return false;
		pos = p - pText;
	}
	SendMessage(hEdit, EM_SETSEL, pos, pos + findLen);//ѡ�в��ҵ����ַ���
	SendMessage(hEdit, EM_SCROLLCARET, 0, 0);//�ѿɼ���Χ���ù�괦����������ѡ�е�λ��
	return true;
}
int _stdcall DlgGotoProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)//ת���Ի��򴰿ڹ���
{
	static HWND hEditGoto;//�༭��
	TCHAR strNum[24];//�༭���ϵ�����
	int lineIndex;//ָ�������ַ����ı��е�λ�ã����ֽ�����ʾ��
	static HWND hwnd;
	static HWND hEdit;//�༭����
	static size_t lineCount;//�༭��������
	switch (message)
	{
	case WM_INITDIALOG://��ʼ�Ի���
		hwnd = GetParent(hDlg);//�õ������ھ��
		hEdit = GetDlgItem(hwnd, ID_EDIT);
		lineCount = SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);
		hEditGoto = GetDlgItem(hDlg, IDC_EDITGOTO);//�õ�ת���༭����
		SetWindowText(hEditGoto, ToString(row).c_str());
		return true;//��Ϊ�ڶԻ�����Դֻ�Ա༭������WS_TABSTOP�����ﷵ��true���ͻ�ʹ�༭�������뽹�㣬���ֻ�ȫѡ
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_GOTO:
			GetWindowText(hEditGoto, strNum, sizeof(strNum));
			row = strToInt(strNum);//���ַ���ת��������
			if (row > lineCount)
			{
				SetWindowText(hEditGoto, ToString(lineCount).c_str());//��ת���༭����ʾ�༭��������
				SetFocus(hEditGoto);//��ת���༭���ý��㣬��Ϊ������ȫѡ��������ʾ������
				SendMessage(hEditGoto, EM_SETSEL, 0, -1);//ת���༭������ȫѡ
				MessageBox(hEditGoto, TEXT("���ҳ����˱�ֽ����"), TEXT("��ֽ -����"), 0);
				break;
			}
			lineIndex = SendMessage(hEdit, EM_LINEINDEX, row - 1, 0);//�õ�ָ�������ַ����ı��е�λ�ã����ֽ�����ʾ����-1����Ϊ������0��ʼ
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
void AddWapperDirectory(HWND hwnd)//��ӱ�ֽĿ¼
{
	recordChange.clear();//���Ŀ¼��ʹ��ǰһ��������ʧЧ�����Խ���¼���
	recordIndex = 0;//��������¼��ʼ��Ϊ0
	plDlist = SHBrowseForFolder(&bi);//����ѡ��Ŀ¼�Ի���,�����û�ѡ���Ŀ¼��Ϣ�ṹ��ָ��plDlist
	if (plDlist)//���ѡ����һ��Ŀ¼
	{
		SHGetPathFromIDList(plDlist, szDirectoryPath);//��plDlist·�����ṹ�壩ת�����ַ���·��				
		auto iter =find(currentDirectorys.begin(),currentDirectorys.end(), szDirectoryPath);//�����б�ֽĿ¼�в����Ƿ��Ѵ��ڸ�Ŀ¼
		if (iter ==currentDirectorys.end())//��Ŀ¼�Ҳ�������ӵ�Ŀ¼
		{
			tempDirectory.clear();//����һ����ӵ�Ŀ¼��������ʱ�ļ������ļ�·�����
			FindAllFile(szDirectoryPath, includeSubDirectory);//������Ŀ¼������ͼƬ
			AllDirectorys.push_back( tempDirectory);//����ǰĿ¼��������Ϣ���
			currentDirectorys.push_back(szDirectoryPath);//����Ŀ¼��ӵ���ǰĿ¼
			DisplayDistribitue();//�����ļ�������ʾ
			writeToXML(hwnd);
		}
		else
			MessageBox(hwnd, TEXT("�������Ŀ¼,��Ҫ�ظ�����Ѵ���Ŀ¼"),TEXT("��Ŀ¼�Ѵ���:"), MB_ICONWARNING | MB_OK);
		if (SHGetMalloc(&pMalloc) == S_OK)//����IMALLOC����
		{
			pMalloc->Free(plDlist);//�ͷ�plDlistָ��Ŀռ�
			pMalloc->Release();//�ͷ�pMalloc�Ŀռ�
		}
	}
}
UINT calculateTime(const Time &time)//����ʱ�䣬�Ժ����
{
	unsigned millisecond;//����
	millisecond = time.second * 1000 + time.minute * 60 * 1000 + time.hour * 60 * 60 * 1000;//����ʱ�䣬�Ժ����
	return millisecond;
}
//��д�༭�ؼ����ڹ���
int _stdcall EditNumberProc(HWND hEdit, unsigned message, unsigned wParam, long lPram)
{
	LPWSTR pszText = L"��ֻ���ڴ˴���������";
	LPWSTR pszTitle = L"���ܽ��ܵ��ַ�";
	switch (message)
	{
	case WM_CHAR://���°���
		if (!(wParam <= '9'&&wParam >= '0') && wParam != VK_BACK)//�����ֺͻ��˼������а�������
		    showBallonTip(hEdit, pszText, pszTitle);
		break;
	}
	return CallWindowProc(oldEditTextProc, hEdit, message, wParam, lPram);//����Ĭ�ϵı༭�ؼ����ڴ�����̴���������Ϣ��δ������Ĳ���
}
//���ñ�ֽ�Ի��򴰿ڹ���
int _stdcall DlgWallpaperSet(HWND hDlg, unsigned int message, unsigned wParam, long lParam)
{
	int wmID = LOWORD(wParam);//�ؼ�ID
	static HWND hWallPaperHour, hWallPaperMinute, hWallPaperSecond;//��ֽ����ʱ���֡���༭����
	static HWND hNotifyHour, hNotifyMinute, hNotifySecond;//������ʾʱ���֡���༭����
	static HWND hNotifyTip, hNotifyTitle, hNotifyContent;//������ʾ�����⡢���ݱ༭��
	static HWND hRadioRandom, hRadioSequence;//����ͺ�˳��ѡ��ť���
	static HWND hCheckBoxMini, hCheckBoxSub,hCheckBoxAutoBoot;//���д�����С������Ŀ¼�Ϳ����Զ�����check box��ť���
	static HWND hwnd;//������
	static HWND hListFilterWindow, hListCurrentWindow;//�б��
	static HINSTANCE hInst;//ʵ�����
	TCHAR szBuffer[MAX_PATH];//������
	bool temp;
	int count;//�б�����ݸ���
	switch (message)
	{
	case WM_INITDIALOG://�Ի����ʼ��
		hWallPaperHour = GetDlgItem(hDlg, IDC_EDITHOUR);//��öԻ�������ؼ��ľ��
		hWallPaperMinute = GetDlgItem(hDlg, IDC_EDITMINUTE);
		hWallPaperSecond = GetDlgItem(hDlg, IDC_EDITSECOND);
		hNotifyHour = GetDlgItem(hDlg, IDC_NOTIFYHOUR);
		hNotifyMinute = GetDlgItem(hDlg, IDC_NOTIFYMINUTE);
		hNotifySecond = GetDlgItem(hDlg, IDC_NOTIFYSECOND);
		hNotifyTip = GetDlgItem(hDlg, IDC_EDITTIP);
		hNotifyTitle = GetDlgItem(hDlg, IDC_EDITTITLE);
		hNotifyContent = GetDlgItem(hDlg, IDC_EDITCONTENT);
		hRadioRandom = GetDlgItem(hDlg, IDC_RANDOM);
		hRadioSequence = GetDlgItem(hDlg, IDC_SEQUENCE);
		hCheckBoxMini = GetDlgItem(hDlg, IDC_CHANGEMINI);
		hCheckBoxSub = GetDlgItem(hDlg, IDC_FINDSUBDIRECTORY);
		hCheckBoxAutoBoot = GetDlgItem(hDlg,IDC_AUTOBOOT);
		hListFilterWindow=GetDlgItem(hDlg, IDC_FILTERWINDOW);
		hListCurrentWindow = GetDlgItem(hDlg, IDC_CURRENTWINDOW);
		hwnd = GetParent(hDlg);
		hInst =(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE);
		//�ڸ����ؼ�����ʾ��ǰ����
		SetDlgItemInt(hDlg, IDC_EDITHOUR, Wallpaper.hour, false);
		SetDlgItemInt(hDlg, IDC_EDITMINUTE, Wallpaper.minute, false);
		SetDlgItemInt(hDlg, IDC_EDITSECOND, Wallpaper.second, false);
		SetDlgItemInt(hDlg, IDC_NOTIFYHOUR, Notify.hour, false);
		SetDlgItemInt(hDlg, IDC_NOTIFYMINUTE, Notify.minute, false);
		SetDlgItemInt(hDlg, IDC_NOTIFYSECOND, Notify.second, false);
		SetDlgItemText(hDlg, IDC_EDITTIP,szTip);
		SetDlgItemText(hDlg, IDC_EDITTITLE, szInfoTitle);
		SetDlgItemText(hDlg, IDC_EDITCONTENT, szInfo);
		SendMessage(hCheckBoxMini, BM_SETCHECK, changeByOnlyDestop, 0);//��������һ�����ڲŻ�
		SendMessage(hCheckBoxSub, BM_SETCHECK, includeSubDirectory, 0);//�����ļ�������Ŀ¼
		SendMessage(hRadioRandom, BM_SETCHECK, randomChange, 0);//����������
		SendMessage(hCheckBoxAutoBoot,BM_SETCHECK,startingUpBoot, 0);
		oldEditTextProc = (WNDPROC)SetWindowLong(hWallPaperHour, GWL_WNDPROC, (long)EditNumberProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ�����Ϣ
		//SetWindowLong(hWallPaperMinute, GWL_WNDPROC, (long)EditNumberProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ�����Ϣ
		//SetWindowLong(hWallPaperSecond, GWL_WNDPROC, (long)EditNumberProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ�����Ϣ
		//SetWindowLong(hNotifyHour, GWL_WNDPROC, (long)EditNumberProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ�����Ϣ
		//SetWindowLong(hNotifyMinute, GWL_WNDPROC, (long)EditNumberProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ�����Ϣ
		//SetWindowLong(hNotifySecond, GWL_WNDPROC, (long)EditNumberProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ�����Ϣ
		SendMessage(hWallPaperHour, EM_LIMITTEXT, 3, 0);
		SendMessage(hWallPaperMinute, EM_LIMITTEXT, 3, 0);
		SendMessage(hNotifyHour, EM_LIMITTEXT, 3, 0);
		SendMessage(hWallPaperSecond, EM_LIMITTEXT, 3, 0);
		SendMessage(hNotifyMinute, EM_LIMITTEXT, 3, 0);
		SendMessage(hNotifySecond, EM_LIMITTEXT, 3, 0);
		SendMessage(hNotifyTip, EM_LIMITTEXT, 128, 0);
		SendMessage(hNotifyTitle, EM_LIMITTEXT,64, 0);
		SendMessage(hNotifyContent, EM_LIMITTEXT,256,0);
		for (int i = 0; i < filterWindowName.size(); i++)//��ʾ���˵Ĵ�����
			SendMessage(hListFilterWindow, LB_INSERTSTRING, -1, (long)filterWindowName[i].c_str());
		break;
	case WM_COMMAND:
		switch (wmID)
		{
		case  IDC_EDITHOUR:
		case  IDC_EDITMINUTE:
		case  IDC_EDITSECOND:
		case  IDC_NOTIFYHOUR:
		case  IDC_NOTIFYMINUTE:
		case  IDC_NOTIFYSECOND:
			if (HIWORD(wParam) == EN_MAXTEXT)
				showBallonTip(GetDlgItem(hDlg, wmID),L"���ܴ��ڵ���1000", L"������ʾ:");
			break;
		case  IDC_EDITTIP:
		case  IDC_EDITTITLE:
		case  IDC_EDITCONTENT:
			if (HIWORD(wParam) == EN_MAXTEXT)
				showBallonTip(GetDlgItem(hDlg, wmID), L"����ֽ���,����64,��ʾ128,����256", L"����̫��:");
			break;

		case IDM_SETFINISH://�������
			temp = startingUpBoot;//����������״̬
			randomChange = SendMessage(hRadioRandom, BM_GETCHECK, 0, 0);//��û���ֽģʽ���Ƿ����
			changeByOnlyDestop = SendMessage(hCheckBoxMini, BM_GETCHECK, 0, 0);//���д�����С���Ÿ�����ֽѡ��״̬
			includeSubDirectory = SendMessage(hCheckBoxSub, BM_GETCHECK, 0, 0);//���Ұ�����Ŀ¼ѡ��״̬
			startingUpBoot = SendMessage(hCheckBoxAutoBoot, BM_GETCHECK, 0, 0);//���ҿ���������ѡ��״̬
			runningMiniShow = SendMessage(GetDlgItem(hDlg, IDC_STARTINGMINISHOW), BM_GETCHECK, 0, 0);//������С��������ʾ
			closeBubble = SendMessage(GetDlgItem(hDlg, IDC_SHUTDOWNBIBLE), BM_GETCHECK, 0, 0);//���ݹ��ܹر�
			if (temp != startingUpBoot)//״̬�ı�
				SetStartingUpBoot(TEXT("���»���ֽ"));//���ÿ����������

			miniPhotoSize = GetDlgItemInt(hDlg, IDC_MINISIZE, 0, false);
			Wallpaper.hour = GetDlgItemInt(hDlg, IDC_EDITHOUR, 0, false);
			Wallpaper.minute = GetDlgItemInt(hDlg, IDC_EDITMINUTE, 0, false);
			Wallpaper.second = GetDlgItemInt(hDlg, IDC_EDITSECOND, 0, false);
			wallPaperTime = calculateTime(Wallpaper);//�����±�ֽ����ʱ�䣬�Ժ����
			if (wallPaperTime>0)//���������ͣʱ��>0����
			{
				KillTimer(hwnd, wallpaperID);//ɱ������ֽ��ʱ��
				if(startChange)
				SetTimer(hwnd, wallpaperID, wallPaperTime, 0);//��������ֽ��ʱ��
			}

			Notify.hour = GetDlgItemInt(hDlg, IDC_NOTIFYHOUR, 0, false);
			Notify.minute = GetDlgItemInt(hDlg, IDC_NOTIFYMINUTE, 0, false);
			Notify.second = GetDlgItemInt(hDlg, IDC_NOTIFYSECOND, 0, false);
			tipTime = calculateTime(Notify);//�����±�ֽ����ʱ�䣬�Ժ����
			if (tipTime>0)//���ʱ��>0
			{
				KillTimer(hwnd,tipID);//ɱ�����ݶ�ʱ��
				if(!closeBubble)
				SetTimer(hwnd,tipID,tipTime, 0);//�������ݶ�ʱ��
			}

			GetDlgItemText(hDlg, IDC_EDITTIP,szBuffer, MAX_PATH);//���������ʾ�ַ���
			if (szBuffer[0] != '\0')//����
				lstrcpyn(szTip, szBuffer, 128);
			GetDlgItemText(hDlg, IDC_EDITTITLE, szBuffer, MAX_PATH);//������ݱ����ַ���
			if (szBuffer[0] != '\0')//����
				lstrcpyn(szInfoTitle, szBuffer, 64);
			GetDlgItemText(hDlg, IDC_EDITCONTENT, szBuffer, MAX_PATH);//������������ַ���
			if (szBuffer[0] != '\0')//����
				lstrcpyn(szInfo, szBuffer, 256);
			if (!closeBubble)
			{
				InitNotifyIconData(hwnd, hInst, nid, IDI_ICON1);//��ʼ������ͼ��ṹ
				Shell_NotifyIcon(NIM_MODIFY, &nid);//�޸����������һ��ͼ�꣬�ٴ���ʾ����
			}
			filterWindowName.clear();
			count = SendMessage(hListFilterWindow, LB_GETCOUNT, 0, 0);//�õ����˴����б���д������ĸ���
			for (int i = 0; i < count; i++)
			{
				SendMessage(hListFilterWindow, LB_GETTEXT, i, (long)szBuffer);//��õ�ǰ�����Ĵ�����
				filterWindowName.push_back(szBuffer);//��������������˴�������
			}

			writeToXML(hwnd);
			EndDialog(hDlg, 0);//���ٶԻ���
			return false;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);//���ٶԻ���
		return false;
	}
	return false;
}
//�޸ı�ֽĿ¼�Ի��򴰿ڹ���
int _stdcall DlgModifyDirectoryProc(HWND hDlg, unsigned int message, unsigned wParam, long lParam)
{
	int wmID = LOWORD(wParam);//�ؼ�ID
	static HWND hListLeft, hListRight;//�����б����
	static HWND hEditPath;//·���༭��
	static HWND hBTNOK;//��ťOK
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	int index;//�б��������
	int count;//�б���ϵ�Ŀ¼����
	vector<tstring>temp;//�����б�����ַ���
	switch (message)
	{
	case WM_INITDIALOG:
		hListLeft = GetDlgItem(hDlg, IDC_LISTLEFT);//�õ�2���б����
		hListRight = GetDlgItem(hDlg, IDC_LISTRIGHT);
		hEditPath = GetDlgItem(hDlg, IDC_EDITDIRECTORY);//�õ�·���༭����
		hBTNOK = GetDlgItem(hDlg, IDC_BTNOK);//�õ�ȷ����ť����
		for (size_t i = 0; i < backupDirectorys.size(); i++)//������б����ʾ���ݱ�ֽĿ¼
			SendMessage(hListLeft, LB_INSERTSTRING, -1, (long)backupDirectorys[i].c_str());//����ǰĿ¼���뵽�б��β��
		for (size_t i = 0; i <currentDirectorys.size(); i++)//���ұ��б����ʾ��ǰ��ֽĿ¼
			SendMessage(hListRight, LB_INSERTSTRING, -1, (long)currentDirectorys[i].c_str());//����ǰĿ¼���뵽�б��β��
		return true;
	case WM_COMMAND:
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		switch (wmId)
		{
		case IDC_EDITDIRECTORY://Ŀ¼�༭��
			switch (wmEvent)//�ڱ༭���ϵĲ���
			{
			case EN_UPDATE://�༭���ϵ����ݸ�����
				GetWindowText(hEditPath, szDirectoryPath, MAX_PATH);//�õ��༭�����ַ���
				EnableWindow(hBTNOK, szDirectoryPath[0]);//����ַ�����Ϊ���ð�ť����
				break;
			}
			break;
		case IDC_BTNADDDIRECTORY://��ӱ�ֽ��ť
			plDlist = SHBrowseForFolder(&bi);//����ѡ��Ŀ¼�Ի���,·�������ָ��plDlist
			if (plDlist)//���ѡ����һ��Ŀ¼
			{
				SHGetPathFromIDList(plDlist, szDirectoryPath);//��plDlist·�����ṹ�壩ת�����ַ���·��	
				SetWindowText(hEditPath, szDirectoryPath);//�ڱ༭������ʾ·��
				if (SHGetMalloc(&pMalloc) == S_OK)//����IMALLOC����
				{
					pMalloc->Free(plDlist);//�ͷ�plDlistָ��Ŀռ�
					pMalloc->Release();//�ͷ�pMalloc�Ŀռ�
				}
			}
			break;
		case IDC_MOVERIGHT://�����б��ѡ�е�·���Ƶ����б��
			index = SendMessage(hListLeft, LB_GETCURSEL, 0, 0);//��õ�ǰѡ��Ŀ¼������
			if (index != LB_ERR)
			{
				SendMessage(hListLeft, LB_GETTEXT, index, (long)szDirectoryPath);//��õ�ǰѡ�е�·��
				SendMessage(hListRight, LB_INSERTSTRING, -1, (long)szDirectoryPath);//����ǰĿ¼���뵽���б��β��
				SendMessage(hListLeft, LB_DELETESTRING, index, 0);//�����б����ɾ��
			}
			break;
		case IDC_MOVELEFT://�����б��ѡ�е�·���Ƶ����б��
			index = SendMessage(hListRight, LB_GETCURSEL, 0, 0);//��õ�ǰѡ��Ŀ¼������
			if (index != LB_ERR)
			{
				SendMessage(hListRight, LB_GETTEXT, index, (long)szDirectoryPath);//��õ�ǰѡ�е�·��
				SendMessage(hListLeft, LB_INSERTSTRING, -1, (long)szDirectoryPath);//����ǰĿ¼���뵽���б��β��
				SendMessage(hListRight, LB_DELETESTRING, index, 0);//�����б����ɾ��
			}
			break;
		case IDC_BTNOK://����ȷ����ť
			SendMessage(hListRight, LB_INSERTSTRING, -1, (long)szDirectoryPath);//����ǰĿ¼���뵽���б��β��
			break;
		case IDC_BTNDELETELEFT://�����б��ѡ��·������ɾ��
			index = SendMessage(hListLeft, LB_GETCURSEL, 0, 0);//��õ�ǰѡ��Ŀ¼������
			SendMessage(hListLeft, LB_DELETESTRING, index, 0);//ɾ��
			break;
		case IDC_BTNDELETERIGHT://�����б��ѡ��·������ɾ��
			index = SendMessage(hListRight, LB_GETCURSEL, 0, 0);//��õ�ǰѡ��Ŀ¼������
			SendMessage(hListRight, LB_DELETESTRING, index, 0);//ɾ��
			break;
		case IDC_BTNSETFINISH://�������
			backupDirectorys.clear();//�������Ŀ¼
			count = SendMessage(hListLeft, LB_GETCOUNT, 0, 0);//�õ����б����Ŀ¼�ĸ���
			for (int i = 0; i < count; i++)
			{
				SendMessage(hListLeft, LB_GETTEXT, i, (long)szDirectoryPath);//��õ�ǰ������·��
				backupDirectorys.push_back(szDirectoryPath);//��·�����뱸��Ŀ¼
			}
			count = SendMessage(hListRight, LB_GETCOUNT, 0, 0);//�õ����б����Ŀ¼�ĸ���
			for (int i = 0; i < count; i++)
			{
				SendMessage(hListRight, LB_GETTEXT, i, (long)szDirectoryPath);//��õ�ǰ������·��
				temp.push_back(szDirectoryPath);//��·��������ʱ����
			}
			for (auto iter = currentDirectorys.begin(); iter != currentDirectorys.end();)//����ǰĿ¼�����б���в����ڵ�Ŀ¼ɾ��
			{
				auto result = find(temp.begin(), temp.end(), *iter);//�����б���в��ҵ�ǰĿ¼
				if (result == temp.end())//�Ҳ���
				{
					iter = currentDirectorys.erase(iter);//ɾ�����Ŀ¼��������һ��������
					int index = iter - currentDirectorys.begin();//��õ�ǰ������������
					AllDirectorys.erase(AllDirectorys.begin() + index);//ɾ����ǰĿ¼��������Ϣ
				}
				else
					iter++;//�ҵ��˲�ɾ����������һ��
			}
			for (auto iter = temp.begin(); iter != temp.end(); iter++)//���б����Ŀ¼�ڵ�ǰĿ¼�в����ڵĲ��뵱ǰĿ¼
			{
				auto result = find(currentDirectorys.begin(), currentDirectorys.end(), *iter);//���б����Ŀ¼�ڵ�ǰĿ¼�в���
				if (result ==currentDirectorys.end())//�Ҳ��������뵱ǰĿ¼
				{
					tempDirectory.clear();//����һ����ӵ�Ŀ¼��������ʱ�ļ������ļ�·�����
					currentDirectorys.push_back(*iter);//����ǰĿ¼�в����ڵ�Ŀ¼����
					FindAllFile(*iter, includeSubDirectory);//������Ŀ¼������ͼƬ
					AllDirectorys.push_back(tempDirectory);//����ǰĿ¼��������Ϣ���
					
				}
			}
			DisplayDistribitue();//�����ļ�������ʾ
			writeToXML(GetParent(hDlg));//����ֽ����Ϣ���浽�浵
			EndDialog(hDlg, 0);//���ٶԻ���
			break;
		}
		return true;
	case WM_CLOSE:
		EndDialog(hDlg, 0);//���ٶԻ���
		return false;
	}
	return false;
}
void NextWallPaper()//��һ�ű�ֽ
{
	if (randomChange)//����������
		nSelectWallpaper = rand() % wallPaperCount + 1;//���ѡһ��ͼƬ
	else
	{
		nSelectWallpaper = nSelectWallpaper + 1;//��һ��ͼƬ
		if (nSelectWallpaper >= wallPaperCount + 1||nSelectWallpaper<=1)//��������һ�Ź���,���߳�����Χ
			nSelectWallpaper = 1;//�ص���ͷ
	}
}
//���ÿ�������
void SetStartingUpBoot(TCHAR *value)
{
	HKEY m_regkey;//ע�����
	TCHAR filename[MAX_PATH];
	GetModuleFileName(NULL, filename, MAX_PATH);//���س����ȫ·��
	TCHAR lpRun[] = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");//�Ӽ�·��
	long iRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_WRITE, &m_regkey);//��������ע����
	if (iRet == ERROR_SUCCESS)//�򿪳ɹ�
	{
		if (startingUpBoot)//��������
		    RegSetValueEx(m_regkey,value, 0, REG_SZ, (const unsigned char *)filename, MAX_PATH);//����ǰ����·����ӵ�ע�����������
		else
			RegDeleteValue(m_regkey,value);//ɾ��ע���
	}
	RegCloseKey(m_regkey);//�ر�ע���
}
void InitWindow(HWND hwnd,HINSTANCE hInst)//�ö�ȡ�������ݳ�ʼ��
{
	hBKBrush = CreateSolidBrush(textBkColor);//����������ˢ���ɫ
	InitBrowseInfo(hwnd, bi);//��ʼ��ѡ��Ŀ¼�ṹ
	InitFont(hwnd, cf,lf);//��ʼ������ṹ
	InitColor(hwnd, cc, color);//��ʼ����ɫ�ṹ
	InitNotifyIconData(hwnd, hInst, nid, IDI_ICON1);//��ʼ������ͼ��ṹ
	WM_FIND = RegisterWindowMessage(FINDMSGSTRING);//���Һ��滻�Ի���ᷢ����Ϣ����FINDMSGSTRINGע�����Ϣ		
	if (!closeBubble)
	Shell_NotifyIcon(NIM_ADD, &nid);//�������������һ��ͼ��
	hMenu = CreateMyPopMenu(hInst);//�����˵�
	blueBrush = CreateSolidBrush(RGB(102, 179, 255));//��ɫ��ˢ
	hFont = CreateFontIndirect(&lf);//�����߼����崴��������
	//hFont= CreateFont(0, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);//Ĭ������
	hStatus = CreateWindow(STATUSCLASSNAME, 0, WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, hwnd, (HMENU)IDS_STATE, hInst, nullptr);//����״̬��
	hEdit = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL | ES_READONLY | ES_MULTILINE | ES_NOHIDESEL, 0, 0, 0, 0,
		hwnd, (HMENU)ID_EDIT, hInst, nullptr);//�����б߽�Ķ���ֻ���༭��ES_NOHIDESEL�ڱ༭��ʧȥ�����ѡ�е��ı���Ȼ���ָ�����ʾ
	SetClassLong(hEdit, GCLP_HCURSOR, (long)LoadCursor(nullptr, IDC_ICON));//���������
	SendMessage(hEdit, EM_LIMITTEXT, 0, 0);//��wParamΪ0ʱ�����б༭�ز����0x7FFFFFFE���ַ���Ĭ����30000���ַ�
	SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);//��������
	oldEditTextProc = (WNDPROC)SetWindowLong(hEdit, GWL_WNDPROC, (long)EditTextProc);//���ñ༭���µĶԻ�����̣��Դ���WM_KEYDOWN֮��ļ��̡������Ϣ����Ϊ
	//Ĭ���ǲ��񲻵���Щ��Ϣ�ģ���Ҫ�õ���Щ��Ϣ�����統���򰴼�����ʱ����״̬����ʾ��������Ϣ
	//���ǲ�����SetClassLong(hEdit,GCL_WNDPROC),��Ϊ�����޸�edit��Ĵ��ڹ��̣���ֻ�����һ�������ı༭����Ч���Ե�ǰ
	//�༭�򲻻�ı䣬������SetClassLong,��Ӱ���Ժ�ı༭�򣬶���ֻ��ı䵱ǰ�༭�򣬲���ı������༭��
	srand(time(0));//����ǰʱ����Ϊ�����������
	wallPaperTime=calculateTime(Wallpaper);//����ʱ�������
	tipTime = calculateTime(Notify);
	if (startChange)//�����ʼ��
	SetTimer(hwnd, wallpaperID, wallPaperTime, nullptr);//���ñ�ֽ��ʱ��
	if (!closeBubble)
	SetTimer(hwnd, tipID, tipTime, nullptr);//����������ʾ��ʱ��
	SetTimer(hwnd, clockID, 1000, nullptr);//����ʱ�Ӷ�ʱ����ÿһ�����ʱ��һ��
	SetTimer(hwnd, timingID,timingTime, nullptr);//���ö�ʱ����ÿ10���ӱ���һ��
	if (windowRect.right > 0 && windowRect.bottom > 0 && windowRect.left >= 0 && windowRect.top >= 0)//�����ȡ�Ŀ�Ⱥ͸߶Ȳ�Ϊ0
		MoveWindow(hwnd, windowRect.left, windowRect.top, windowRect.right, windowRect.bottom, true);//�ƶ����ڣ���ʾ��һ�δ���������λ�ô�С
}
void defaultInit(HWND hwnd,HINSTANCE hInst)//����Ĭ�ϳ�ʼ��
{
	//��ʼ������
	StringCchCopy(lf.lfFaceName, LF_FACESIZE, TEXT("����"));
	lf.lfHeight = 27;
	lf.lfWeight = 400;
	InitWindow(hwnd, hInst);
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//������ ���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ
	HDC hdc;//����
	static POINT mousePos;//���λ��
	static HBRUSH oldBrush;//����ɻ�ˢ
	static HINSTANCE hInst;
	static bool bDown;//���°�ť�Ƿ���
	static TCHAR szFindText[MAX_PATH]=TEXT("");//Ҫ���ҵ��ַ���
	int xPos[4]{0, 0, 0, 0};//״̬�����ĸ��Һ�����
	static FINDREPLACE fr;//���Һ��滻�ṹ
	LPFINDREPLACE pfr = nullptr;//���Һ��滻�ṹ��ָ��
	switch (UMsg)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;//����ʵ�����
		if (readFromXML(hwnd))//��ȡ�û�����,����д浵����Ĭ�����ø���Ϊ�û�����
			InitWindow(hwnd, hInst);//��ʼ�����ڸ�������
		else
			defaultInit(hwnd, hInst);//Ĭ�ϳ�ʼ��
		if (currentDirectorys.size()!= 0)//����б�ֽ·��
		{
			for (size_t i = 0; i < currentDirectorys.size(); i++)//��������Ŀ¼��ͼƬ
			{
				tempDirectory.clear();//����һ����ӵ�Ŀ¼��������ʱ�ļ������ļ�·�����
				FindAllFile(currentDirectorys[i], includeSubDirectory);//���ҵ�ǰĿ¼������ͼƬ
				AllDirectorys.push_back(tempDirectory);//����ǰĿ¼��������Ϣ���
			}
			DisplayDistribitue();//��ʾ�ļ�����
		}
		break;
	case WM_QUERYENDSESSION://�ػ�ʱ����
 		writeToXML(hwnd);//����ֽ����Ϣ���浽�浵        
		 break;
	case WM_SETFOCUS:
		SetFocus(hEdit);//�ñ༭���ý���
		HideCaret(hEdit);//�༭���й�꣬��Ϊ����Դ������Ϊֻ���������ù�����زűȽϺã�һ��Ҫ��setfoucs����������Ч����Ϊ��ʼ�������б༭�ػ�δ��ý��㣬��������
		//���ع����Ч��setfocus������ý����������ع�����Ч
		break;
	case WM_SIZE://��С�ı��
		cxClient = LOWORD(lParam);//�ͻ����Ŀ�ȡ��߶�
		cyClient = HIWORD(lParam);
		xPos[3] = -1;//���һ�������ұ߽�-1�����򵽿ͻ������ұ�
		xPos[2] = cxClient - 170;
		xPos[1] = cxClient - 250;
		xPos[0] = cxClient - 500;//��һ�������ұ߽�
		SendMessage(hStatus, SB_SETPARTS,4, (long)xPos);//�����ĸ������������
		if (IsIconic(hwnd))//���������С����ť
			ShowWindow(hwnd, SW_HIDE);//���ش���
		MoveWindow(hEdit, 1, 1, cxClient - 4, cyClient - 29, true);//-4��������Ȼ��߿�-29�ǰѱ߿���״̬�����棬״̬���߶�25����4����Ȼ��߿�
		MoveWindow(hStatus, 0, 0, 0, 0, true);//����״̬���ڿͻ����仯��������棬�������ô�Сλ�ã���Ϊ��Сλ�ù̶����ɱ�
		for (int i = 0; i <4; i++)
			SendMessage(hStatus, SB_SETTEXT, i, (long)statusInfo[i].c_str());//��ÿһ����ʾ�ı�
		if (nSelectWallpaper > 0)//���������ĳ�ű�ֽ
			HighLineWallPaper(hEdit, nSelectWallpaper);//������ʾ
		break;
	case WM_CTLCOLORSTATIC://�༭���ʼ������
		hdc = (HDC)wParam;//wParam�Ǳ༭��Ļ��ʣ�lParam�Ǳ༭��ľ��
		SetBkColor(hdc, textBkColor);//�������屳����ɫ��Ӧ��༭�򱳾���ɫһ�£��������2�ֱ���ɫ���ÿ�
		SetTextColor(hdc, textColor);//�����������ɫ
		return (long)hBKBrush;//���ﲻ����break,�������������DefWindowProc������ɫ���Ĭ�ϵģ�����һ����ˢ���༭���Ĭ�ϴ��ڹ��̻��������ˢ��Ϊ����ɫ
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);//�ؼ�ID
		int wmEvent = HIWORD(wParam);//�ؼ��ϵĲ���
		switch (wmId)
		{
		case ID_ADDWAPPER://��ӱ�ֽĿ¼
			AddWapperDirectory(hwnd);//��ӱ�ֽĿ¼
			break;
		case IDM_MODIFYWAPPERDIRECTORY://�޸ı�ֽ·��
			DialogBox(hInst,TEXT("IDD_DIALOGMODIFYDIRECTORY"),hwnd,DlgModifyDirectoryProc);//�����޸ı�ֽ·���Ի���
			break;
		case IDM_EDIT_FIND: //����
			hDlgFind = FindDialog(hwnd,fr, szFindText);//�������ҶԻ������
			break;
		case IDM_EDIT_FINDNEXT://������һ��
			if (szFindText[0] == '\0')//�����û��Ҫ���ҵ��ַ���
				hDlgFind =FindDialog(hwnd,fr, szFindText);//�������ҶԻ�����������û������ַ�������
			else
			{
				if (!MyFindText(fr, bDown))//������һ��
					OKMessage(hDlgFind, TEXT("�Ҳ���\"%s\""), fr.lpstrFindWhat);//���Ҳ���
			}
			return 0;
		case IDM_EDIT_GOTO: //ת��
			DialogBox(hInst, TEXT("IDD_DIALOGGOTO"), hwnd, DlgGotoProc);
			return 0;
		case IDM_FORMAT_FONT://����
			if (ChooseFont(&cf))//ѡ������
			{
				DeleteObject(hFont);//ɾ����һ�εĻ���
				hFont = CreateFontIndirect(&lf);//�����߼����崴��������
				SendMessage(hEdit, WM_SETFONT, (unsigned)hFont, 0);//�Ա༭������Ϣ�������»���
				InvalidateRect(hEdit, nullptr, true);//���µ�����������ʾ
				writeToXML(hwnd);//�浵
			}
			break;
		case IDM_FONTCOLOR://������ɫ
			if (ChooseColor(&cc))//�ú�������һ����ʹ�û�����ѡ����ɫ��ͨ����ɫ�Ի���
			{
				textColor = cc.rgbResult;//ѡ���������ɫ
				InvalidateRect(hEdit, nullptr, true);//�༭���ػ�
				writeToXML(hwnd);//�浵
			};
			break;
		case IDM_BKCOLOR://������ɫ
			if (ChooseColor(&cc))//�ú�������һ����ʹ�û�����ѡ����ɫ��ͨ����ɫ�Ի���
			{
				textBkColor = cc.rgbResult;//ѡ������屳����ɫ
				DeleteObject(hBKBrush);//ɾ���ɻ�ˢ
				hBKBrush = CreateSolidBrush(cc.rgbResult);//�ñ���ɫ�������»�ˢ
				InvalidateRect(hEdit, nullptr, true);//�༭���ػ�
				writeToXML(hwnd);//�浵
			};
			break;
		case IDM_WALLPAPERSET:
			DialogBox(hInst, TEXT("IDD_DIALOGSET"),0, DlgWallpaperSet);
			break;
		case IDM_TOOLPRINT://��ͼ
			if (!hotKeyClip)//��������ȼ������,��ʱ�����ڻ�����
			{
				ShowWindow(hwnd, SW_HIDE);//���������ڣ���ʾ����
				Sleep(200);//ͣס200����������������
			}
			if (hDlg)
				DestroyWindow(hDlg);//�����һ�δ���δ�رգ���������
			hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CLIPBITMAP), hwnd, DlgPrintProc);
			break;
		case IDM_SHOWWALLPAPER://�鿴�����ֽ
			ShowWindow(hwnd, SW_NORMAL);//��ʾ����
			SetForegroundWindow(hwnd);//�����ڷŵ�ǰ̨�����ö�
			break;
		case IDM_PAUSE://��ͣ
			if (startChange)//����ѿ�ʼ����ֽ���������ͣ����ʾ������ܿ�ʼ
			{
				ModifyMenu(hMenu, IDM_PAUSE, MF_BYCOMMAND, IDM_PAUSE, TEXT("��ʼ\tCtrl+P"));//��ʾ������ܿ�ʼ
				startChange = false;//��ͣ���
				KillTimer(hwnd, wallpaperID);//ɱ������ֽ��ʱ��
			}
			else
			{
				ModifyMenu(hMenu, IDM_PAUSE, MF_BYCOMMAND, IDM_PAUSE, TEXT("��ͣ\tCtrl+P"));
				startChange = true;//��ʼ���
				SetTimer(hwnd,wallpaperID, wallPaperTime, 0);//��������ֽ��ʱ��
			}
			break;
		case IDM_PREWALLPAPER://��һ��
			recordIndex--;//��һ�ŵļ�¼����
			if (recordIndex>=0)
			nSelectWallpaper = recordChange[recordIndex];//������һ�ű�ֽ������
			HighLineWallPaper(hEdit, nSelectWallpaper);//������ʾ
			ChangeWallPaper();//�ı��ֽ
			break;
		case IDM_NEXTWALLPAPER://��һ��
			if (recordIndex == recordChange.size() - 1 || recordChange.size() == 0)//��ǰ��ֽ�����±�ֽ��������ǰ�����Ļ��߻���ֽ��¼Ϊ0
			{
				NextWallPaper();//������һ�ű�ֽ����
				recordChange.push_back(nSelectWallpaper);//���滻���ı�ֽ�еļ�¼�е�����
				recordIndex = recordChange.size() - 1;//��ǰ��ֽ�ڻ����ı�ֽ�еļ�¼�е����������һ������
			}
			else
			{
				recordIndex++;//��һ�ŵļ�¼����
				nSelectWallpaper = recordChange[recordIndex];//������һ�ű�ֽ������
			}
			HighLineWallPaper(hEdit, nSelectWallpaper);//������ʾ
			ChangeWallPaper();//�ı��ֽ
			break;
		case IDM_RANDOM://����ֽģʽ�����������˳��
			//��������������ģʽ�������˳�򻻣���ʾ����������
			ModifyMenu(hMenu, IDM_RANDOM, MF_BYCOMMAND, IDM_RANDOM, randomChange ? TEXT("���") : TEXT("˳��"));//��ʾ����������
			randomChange = !randomChange;//˳�򻻱��
			break;
		case IDM_MINI://���д�����С���Ÿ�����ֽ�˵�
			CheckMenuItem(hMenu, IDM_MINI, randomChange?MF_UNCHECKED:MF_CHECKED);//�����д�����С���Ÿ�����ֽ�˵���ȥ����
			changeByOnlyDestop = !changeByOnlyDestop;//���д�����С���Ÿ����ڱ��
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
			break;
		}
	}
		break;
	case WM_TIMER://��ʱ����Ϣ
		switch (wParam)
		{
		case clockID://����ʱ��
			statusInfo[1] =FormatTime().str();//��ϵͳʱ����и�ʽ��
			SendMessage(hStatus, SB_SETTEXT, 1, (long)statusInfo[1].c_str());//�Եڶ�������ʱ�� 
			break;
		case tipID://������ʾ��ʱ����
			if (!closeBubble)
			Shell_NotifyIcon(NIM_MODIFY, &nid);//�޸����������һ��ͼ�꣬�ٴ���ʾ����
			break;
		case timingID://10���Ӷ�ʱ����һ��
			writeToXML(hwnd);
			break;
		case wallpaperID://����ֽ��ʾ����
			{//���й¶������
				//HWND hDestop = GetDesktopWindow();
				//HDC  hdcScr = GetDCEx(hDestop, 0, DCX_LOCKWINDOWUPDATE);//�����Ļ�Ļ���,��ʹ��Ļ��סҲ��ˢ��
				//TextOut(hdcScr, 20, 400, temp.c_str(), temp.size());
			}
			if (wallPaperCount > 0)//ͼƬ������Ϊ0
			{
				if (changeByOnlyDestop)//�����������������ڲŻ���ֽ
				{   //֮ǰ����case wallpaperID:��ʹ�ó������˴������ù�
			        EnumWindows(EnumWindowProc, 0);//ö��������Ļ�ϵĶ��㴰�ڣ��������ھ�����͸�Ӧ�ó�����Ļص�����

					if (allTitle.size() == 1)//���ֻ��1�����ڼ�����Window Manager����������ʾ
					{//��Ϊֻ�����洰�ڣ����Գ����ں�̨���У����ø�����ʾ
						NextWallPaper();//������һ�ű�ֽ����
						ChangeWallPaper();//�ı��ֽ
						recordChange.push_back(nSelectWallpaper);//���滻���ı�ֽ�еļ�¼�е�����
						recordIndex = recordChange.size() - 1;//��ǰ��ֽ�ڻ����ı�ֽ�еļ�¼�е����������һ������
					}
				}
				else
				{
					NextWallPaper();//������һ�ű�ֽ����
					HighLineWallPaper(hEdit, nSelectWallpaper);//�Ե�ǰ��ֽ������ʾ
					ChangeWallPaper();//�ı��ֽ
					recordChange.push_back(nSelectWallpaper);//���滻���ı�ֽ�еļ�¼�е�����
					recordIndex = recordChange.size() - 1;//��ǰ��ֽ�ڻ����ı�ֽ�еļ�¼�е����������һ������
				}
			}
			temp.clear();
			allTitle.clear();//��մ��ڱ���
			break;
		}
		break;
	case WM_USER:
		switch (lParam)
		{
			case WM_LBUTTONDBLCLK:
			case WM_LBUTTONDOWN://������ͼ���ϵ�����
				ShowWindow(hwnd, SW_NORMAL);//��ʾ����
				SetForegroundWindow(hwnd);//�����ڷŵ�ǰ̨�����ö�
				break;
			case WM_RBUTTONDOWN:////������ͼ�����Ҽ�������
			{
				PopupMyMenu(hwnd, hMenu);//�����˵�
				break;
			}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		DrawBorder(hwnd, blueBrush);//���Ʊ߿�
		break;
	case WM_CLOSE://���ڹر�
		if (!closeBubble)
		Shell_NotifyIcon(NIM_DELETE, &nid);//ɾ������ͼ��
		DestroyWindow(hwnd);//���ٴ���
		writeToXML(hwnd);//����ֽ����Ϣ���浽�浵
		break;
	case WM_DESTROY://��������
		PostQuitMessage(0);//�׳�����ֵΪ0����Ϣ����GetMessage�е�ѭ�����г���
		KillTimer(hwnd, wallpaperID);//ɾ����ʱ��
		KillTimer(hwnd, tipID);
		KillTimer(hwnd,clockID);
		DeleteObject(blueBrush);//ɾ����ˢ
		DeleteObject(hFont);//ɾ������
		if (hBKBrush)
			DeleteObject(hBKBrush);//ɾ���ɻ�ˢ
		break;
	default:
		if (UMsg == WM_FIND)//���ܷ���case ����У���Ϊcase���Ҫ��������WM_FINDREPALCE���Լ������һ������
		{
			pfr = (LPFINDREPLACE)lParam;//���յ������Ϣ��lparam����������FINDREPLACE�ṹָ��
			if (pfr->Flags&FR_DIALOGTERM)//��������˹رհ�ť
				hDlgFind = 0;//�ԶԻ������ÿ�
			if (pfr->Flags&FR_DOWN)//���°�ť������
				bDown = true;
			else
				bDown = false;
			if (pfr->Flags&FR_FINDNEXT)//��������˲�����һ��
			{
				if (!MyFindText(*pfr, bDown))//������һ��
					OKMessage(hDlgFind, TEXT("�Ҳ���\"%s\""), pfr->lpstrFindWhat);//���Ҳ���
			}
		}
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);
}

void ShowFullScreen(HWND hwnd, int cxScreen, int cyScreen)//�ô���ȫ��
{
	SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);//���ô�����ʽΪ����ʽ���ڣ�����ȥ��ϵͳ�˵������������߿�ȣ�Ҳ����WS_BORDER��ʽ
	SetMenu(hwnd, 0);//ȥ���˵�
	SetWindowPos(hwnd, HWND_TOPMOST, -3, -3, cxScreen + 3, cyScreen + 3, SWP_SHOWWINDOW);//�ô�����ʾȫ�����ö���(0,0)��ʹ�Ի��򴰿ڲ�������Ļ���Ͻ�-3����ʾ�����������Ǳ߿򳤶�3
	bFullScreen = true;//��ȫ��
	SetClassLong(hwnd, GCLP_HCURSOR,(long)LoadCursor(nullptr,IDC_CROSS));//���������Ϊʮ�ּ�ͷ
}
void RecoveryWindow(HWND hwnd, HMENU hMenu)//�ָ�����ԭ���Ĵ�Сλ��
{
	SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);//�ָ�������ʽ
	SetMenu(hwnd, hMenu);//��Ӳ˵�
	SetWindowPos(hwnd, HWND_NOTOPMOST, 122, 122, width + 140, heigh + 180, SWP_SHOWWINDOW);//���ô��ڴ�С��Ϊ��ͼ��С����120����60������Ϊ��ͼ��߿����
	bFullScreen = false;//����ȫ��
	SetClassLong(hwnd, GCLP_HCURSOR, (long)LoadCursor(nullptr,IDC_ARROW));//�ָ�������ͷ
}
HBITMAP CopyBitmap(const HBITMAP &hBitmap)//λͼ����
{
	HBITMAP copyBitmap;
	HDC hdcSrc = CreateCompatibleDC(0);//����ڴ滭��
	HDC hdcDst = CreateCompatibleDC(0);//����ڴ滭��
	SelectObject(hdcSrc, hBitmap);//��λͼѡ���ڴ滭��
	BITMAP bm;//λͼ�ṹ
	GetObject(hBitmap, sizeof(BITMAP), &bm);//���Ҫ������λͼ��Ϣ
	copyBitmap = CreateCompatibleBitmap(hdcSrc, bm.bmWidth, bm.bmHeight);//����Ҫ������λͼ
	SelectObject(hdcDst, copyBitmap);//��λͼѡ���ڴ滭��
	BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);//��λͼ��������һ��λͼ
	DeleteObject(hdcSrc);
	DeleteObject(hdcDst);
	return copyBitmap;
}
int _stdcall DlgPrintProc(HWND hDlg, unsigned int UMsg, unsigned int wParam, long lParam)//��ͼ���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	static HDC hdc;//����
	static HDC hdcMem;//�ڴ滭��
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	static POINT ptBeg, ptEnd;//��ͼ��㡢�յ�
	static bool  bBlock = false;//�Ƿ�ʼ�����ο�
	static bool bNewBlcok = false;//�Ƿ�ʼ��ͼ
	static HWND hDestop;//������
	static HBITMAP hBitmap;//λͼ���
	static HBITMAP copyBitmap;//������λͼ�������и��ơ�����ʱ������ͼ����һ�ݵ�������
	static HBITMAP clipBitmap;//�Ӽ������л�õ�λͼ���
	static HBITMAP blockBitmap;//�����ͼ��λͼ
	static RECT windowRect;//���ڵ����꣬������ȫ��ʱ����ԭ����
	static HBRUSH redBrush;//��ɫ��ˢ������ͼ���εı߿����ɫ
	static HMENU hMenu;//���ڲ˵�
	switch (UMsg)
	{
	case WM_INITDIALOG://���ڱ�����
		hInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);//��ô���ʵ�����
		cxChar = LOWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�Ŀ��
		cyChar = HIWORD(GetDialogBaseUnits());//�õ��Ի�������ĸ�ĸ߶�
		redBrush = CreateSolidBrush(RGB(255, 0, 0));//��ɫ��ˢ������ͼ���εı߿����ɫ
		hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENUCLIP));//���ز˵�
		SetMenu(hDlg, hMenu);//���ò˵�
		hDestop = GetDesktopWindow();//������
		SendMessage(hDlg, WM_COMMAND, IDM_NEWCLIP, 0);//������Ϣ����ͼ
		break;
	case WM_SIZE://���ڴ�С�ı�ʱ
		cxClient = LOWORD(lParam);//���¿ͻ����Ĵ�С
		cyClient = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		if (bNewBlcok&&hBitmap)//�����ʼ��ͼ����ȫ��λͼ��Ϊ��
		{
			bBlock = true;//��ʼ�����ο�
			SetCapture(hDlg);//��׽���
			ptBeg.x = LOWORD(lParam);//��ý�ͼ����ʼ����
			ptBeg.y = HIWORD(lParam);
			ptEnd = ptBeg;
			hdc = GetDC(hDlg);//��ô��ڻ���
			hdcMem = CreateCompatibleDC(hdc);//��õĻ���
			SelectObject(hdcMem, hBitmap);//����Ļλͼѡ�뻭��
			SetCursor(LoadCursor(0, IDC_CROSS));//�������Ϊʮ�ּ�ͷ����ʾ�ڽ�ͼ��
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE://���ո����ȫ��
			if (bFullScreen)//�������ȫ��
				break;//�˳�
			GetWindowRect(hDlg, &windowRect);
			if (hBitmap)//���λͼ�����Ϊ��
			{
				DeleteObject(hBitmap);//����λͼ
				hBitmap = 0;
			}
			GetDestopBK(hDestop, hBitmap, cxScreen, cyScreen);//�õ�����λͼ
			ShowFullScreen(hDlg, cxScreen, cyScreen);//ȫ����ʾ
			InvalidateRect(hDlg, 0, true);//�ڴ��ڻ������汳��
			break;
		}
		break;
	case WM_MOUSEMOVE:
		ptEnd.x = (short)LOWORD(lParam);//�յ�����
		ptEnd.y = (short)HIWORD(lParam);
		if (ptBeg.x == ptEnd.x&&ptBeg.y == ptEnd.y)
			break;
		if (bBlock)//����Ѿ�����������
		{
			BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY);//����ǰ���ľ��β���	
			if (ptBeg.x < ptEnd.x)//��ģ������С����Ϊ���ε����
			{
				clipRect.left = ptBeg.x;
				clipRect.right = ptEnd.x;
			}
			else
			{
				clipRect.left = ptEnd.x;
				clipRect.right = ptBeg.x;
			}
			if (ptBeg.y < ptEnd.y)//��ģ������С����Ϊ���ε��յ�
			{
				clipRect.top = ptBeg.y;
				clipRect.bottom = ptEnd.y;
			}
			else
			{
				clipRect.top = ptEnd.y;
				clipRect.bottom = ptBeg.y;
			}
		 FrameRect(hdc, &clipRect, redBrush);//�ú�ɫ�߿򻭳���������
		}
		break;
	case WM_RBUTTONDOWN://����Ҽ���ȡ�����ν�ͼ���شν�ͼ
		bBlock = false;//��δ�����ο�
		BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY);//����ǰ���ľ��β���	
		SetCursor(LoadCursor(0, IDC_ARROW));//�ָ�����ͷ
		ReleaseDC(hDlg, hdc);
		DeleteDC(hdcMem);
		break;
	case WM_LBUTTONUP:
		if (bNewBlcok)//�����ʼ��ͼ
		{
			ptEnd.x = (short)LOWORD(lParam);//�յ�����
			ptEnd.y = (short)HIWORD(lParam);
			if (ptBeg.x == ptEnd.x&&ptBeg.y == ptEnd.y)
			{
				bBlock = false;
				DeleteDC(hdcMem);
			}
		}
		if (bBlock)
		{
			bBlock = bNewBlcok = false;//��ͼ����
			width = abs(ptEnd.x - ptBeg.x);//��ͼ�Ŀ�ȣ��߶�
			heigh = abs(ptEnd.y - ptBeg.y);
			RecoveryWindow(hDlg, hMenu);//�ָ�ԭ���Ĵ���
			HDC hdcClip = CreateCompatibleDC(hdc);//������������Ļ���
			if (blockBitmap)//�����һ�εĽ�ͼλͼδɾ��
			{
				DeleteObject(blockBitmap);//ɾ��
				blockBitmap = 0;
			}
			blockBitmap = CreateCompatibleBitmap(hdc, width, heigh);//�������ݵĻ���
			SelectObject(hdcClip, blockBitmap);//����ͼλͼ����
			BitBlt(hdcClip, 0, 0, width, heigh, hdcMem, clipRect.left, clipRect.top, SRCCOPY);//����ͼ�����ڽ�ͼλͼ
			ReleaseCapture();//�ͷ����Ĳ�׽
			ReleaseDC(hDlg, hdc);
			DeleteDC(hdcMem);
			DeleteDC(hdcClip);
			hotKeyClip = false;//�ָ���ʼ״̬
			ShowWindow(hDlg, SW_NORMAL);//��ʾ����
			SetForegroundWindow(hDlg);//�����ö�
			InvalidateRect(hDlg, 0, true);//�ڴ���������ʾ��ͼ
			if (!blockBitmap)
				break;//û��Ҫ������λͼ
			copyBitmap = CopyBitmap(blockBitmap);//����λͼ
			OpenClipboard(hDlg);//�򿪼�����
			EmptyClipboard();//�����������
			SetClipboardData(CF_BITMAP, copyBitmap);//��λͼ����ŵ�������
			CloseClipboard();//�رռ�����
		}
		break;
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		switch (wmId)
		{
		case IDM_NEWCLIP:
			bNewBlcok = true;//��ʼ��ͼ
			ShowWindow(hDlg, SW_HIDE);//���ش���
			if (hBitmap)//���λͼ��Ϊ��
			{
				DeleteObject(hBitmap);//����ǰλͼɾ��
				hBitmap = 0;
			}
			GetDestopBK(hDestop, hBitmap, cxScreen, cyScreen);//�õ�����λͼ
			GetWindowRect(hDlg, &windowRect);//��ô������꣬�ڽ���ͼ��ָ�
			ShowFullScreen(hDlg, cxScreen, cyScreen);//�ô���ȫ��
			InvalidateRect(hDlg, 0, true);//��ʾ������Ļ��ͼ
			break;
		case IDCANCEL://����ESC��
			RecoveryWindow(hDlg, hMenu);
			break;
		}
		break;
	case WM_PAINT://������ʧЧ�������ػ�
	{
		hdc = BeginPaint(hDlg, &ps);//��ô��ڻ���
		HBITMAP tempBitmap = 0;//��ʱλͼ������Ҫ��ʾ��λͼ
		tempBitmap = (bFullScreen) ? hBitmap : blockBitmap;//�����ȫ������ȫ��λͼhBitmap,�����ý�ͼλͼblockBitmap
		if (tempBitmap)//λͼ����
		{
			hdcMem = CreateCompatibleDC(0);//�������Ļ���ݵĻ���
			BITMAP bm;//λͼ�ṹ
			GetObject(tempBitmap, sizeof(BITMAP), &bm);//���λͼ��Ϣ
			SelectObject(hdcMem, tempBitmap);//��λͼѡ�뻭��
			if (bFullScreen)//ȫ��
				BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);//�ڴ�������ʾ��ͼ
			else
				BitBlt(hdc, 60, 60, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);//�ڴ�������ʾ��ͼ
			DeleteObject(hdcMem);
		}
	    EndPaint(hDlg, &ps);//�ͷŻ�����Դ
	}
	break;
	case WM_CLOSE://���ڹر�
		if (hBitmap)
			DeleteObject(hBitmap);//ɾ��λͼ
		if (blockBitmap)//�����һ�εĽ�ͼλͼδɾ��
			DeleteObject(blockBitmap);//ɾ��
		DeleteObject(redBrush);//ɾ����ˢ
		DestroyWindow(hDlg);//���ٴ���
		break;
	}
	return false;
}
// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	HWND hwnd;
	hwnd = FindWindow(TEXT("zeng"), TEXT("wallpaper"));//�����Լ��Ĵ���
	if (IsWindow(hwnd))
	{
		SetForegroundWindow(hwnd);//�Լ��Ĵ��������У���ǰ��
		ShowWindow(hwnd	, SW_SHOWNORMAL);
		return 0;
	}
	cxScreen = GetSystemMetrics(SM_CXSCREEN);//��Ļ���
	cyScreen = GetSystemMetrics(SM_CYSCREEN);
	WNDCLASSEX wndclass;//������
	wndclass.cbClsExtra = 0;//Ϊ�����Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.cbWndExtra = 0;//Ϊ���ڷ���Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//������ˢ
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//�����
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON3));//ͼ��
	wndclass.hInstance = hInstance;//ʵ�������������PID��һ���������������������ϵͳ�����ҵ����̵ĵ�ַ
	wndclass.lpfnWndProc = WndProc;//���ڹ���
	wndclass.lpszClassName = TEXT("zeng");//����
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENUMAIN);//�˵�
	wndclass.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;//��Щ�ı��Ҫ�ػ�������ˮƽ�ʹ�ֱ�ı���ػ�, CS_DBLCLKS �������˫����Ϣ
	wndclass.cbSize = sizeof(wndclass);//���ڴ�С 
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));//Сͼ��
	if (!RegisterClassEx(&wndclass))//ע�ᴰ����
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	hwnd = CreateWindow(TEXT("zeng"), TEXT("wallpaper"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);//��������
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	ShowWindow(hwnd, runningMiniShow?SW_HIDE:SW_SHOWNORMAL);//��ʾ�����ش��� 
	UpdateWindow(hwnd);//�����ػ�
	if (currentDirectorys.size() == 0)//���û�б�ֽĿ¼
	{
		MessageBox(hwnd, TEXT("���ڡ��ļ���->����ӱ�ֽĿ¼���������Ŀ¼"), TEXT("û�б�ֽĿ¼"), MB_OK);
		SendMessage(hwnd, WM_COMMAND, ID_ADDWAPPER, 0);//������Ϣ���Ŀ¼
	}
	HACCEL hAccel = LoadAccelerators(hInstance, TEXT("WALLPAPER"));//����Դ�ļ���ȡ���ټ����
	MSG msg;//��Ϣ�ṹ
	RegisterHotKey(0, ID_HOTKEYSHOW,MOD_CONTROL|MOD_ALT, 'M');//����ϵͳ�ȼ���ϵͳ��ݼ�����ʹ�ں�̨���У�Ҳ���յ���ݼ���Ϣ),������ʾ����
	RegisterHotKey(0, ID_HOTKEYCLIP, MOD_CONTROL | MOD_ALT, 'P');//��ͼ�ȼ�
	while (GetMessage(&msg, nullptr, 0, 0))//����Ϣ���л�ȡ��Ϣ
	{
		if (msg.message == WM_HOTKEY)//�����ϵͳ�ȼ�
		{
			switch (msg.wParam)
			{
			case ID_HOTKEYCLIP:
				hotKeyClip = true;//�ȼ������ͼ
				SendMessage(hwnd, WM_COMMAND,IDM_TOOLPRINT, 0);//������Ϣ����ͼ
				break;
			case ID_HOTKEYSHOW:
				SendMessage(hwnd, WM_COMMAND,IDM_SHOWWALLPAPER, 0);//������Ϣ����ʾ����
				break;
			}
		}
		if (!TranslateAccelerator(hwnd, hAccel, &msg))////TranslateAccelerator���������ܣ�������ټ����ú�������˵������еļ��ټ�
		{
			if (!hDlg || !IsDialogMessage(hDlg, &msg))
			{//�����ͼ�Ի������٣����߲��ǶԻ�����Ϣ��������Ϣ����
				TranslateMessage(&msg);//��һЩ������Ϣת�����ַ���Ϣ���ڰ�����Ϣ����
				DispatchMessage(&msg);//�ַ���Ϣ�������ô��ڹ��̽��д�����Ϣ
			}
		}
	}
	return 0;
}
