//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<strsafe.h>
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
#pragma comment(lib,"winmm.lib")//����winmm.lib�⣬���������ļ�playsoundҪ�õ�
void ShowNumber(HWND hwnd, int num)
{
	TCHAR temp[20];
	StringCchPrintf(temp, 20, TEXT("%X"), num);//����ת����16����
	SetDlgItemText(hwnd, IDC_RESULT,temp);//����ʾ��̬�ı�����ʾ����
}
int Calulate(int first, int operation, int second)//���м��㣬�����ֱ��ǵ�1����������������2����
{
	switch (operation)
	{
	case '+':return first + second;
	case '-':return first - second;
	case '*':return first* second;
	case '/':return second?first / second:MAXDWORD;//�������Ϊ0����ʾ���FFFFFFFF
	case '%':return second ? first % second : MAXDWORD;//�������Ϊ0����ʾ���FFFFFFFF
	case '=':return second;
	case '&':return first & second;
	case '|':return first |second;
	case '^':return first ^second;
	case '>':return first >>second;
	case '<':return first <<second;
	}
}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)//���ڹ���
{
	PAINTSTRUCT ps;//���ʽṹ������������Ϣ
	HDC hdc;//����
	static int cxClient, cyClient;//�ͻ�����ȡ��߶�
	static int cxChar, cyChar;//��ĸ��ȡ��߶�
	int wmId, wmEvent;//�ؼ�ID,�ؼ�֪ͨ�룬�����
	static HINSTANCE hInst;//����ʵ�����
	HWND hButton;
	static int number;//��ǰ���µ�����
	static int firstNum;//��1�����µ�����
	static bool bNewNum=true;//�Ƿ����°��µ���
	static int iOperation = '=';//������,��ʼ��Ϊ'='�������1�������������=�����㣬�Ժ�ÿ������һ������������ǰһ������������ǰһ�ε�����
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
	case WM_KEYDOWN://���°���
		if (wParam != VK_LEFT&&wParam != VK_DELETE)
			break;//���������������߲���ɾ��������գ����ܿ������ַ������򲻴���
		//����Ƿ��������Ӧɾ��һ�����֣���Del������Ӧ��գ�����Ϊ�������ַ���Ϣ�����Բ���fall through��char ��Ϣ
		if (wParam == VK_LEFT)//�����
			wParam = VK_BACK;//ת���ɻ�����Ϣ
	case WM_CHAR:
		if (isalpha(wParam))//�������ĸ����
			wParam=toupper(wParam);//����ĸת���ɴ�д��ĸ
		if (wParam == VK_RETURN)//������»س�����ת����=�ż�
			wParam = '=';
		if (wParam == 0x1b)
			wParam = VK_DELETE;//�������ESC����ת����ɾ��������գ�
		if (hButton = GetDlgItem(hwnd, wParam))//������¼���ASCII�ַ��ڼ��������ж�Ӧ�ļ�
		{
			SendMessage(hButton, BM_SETSTATE, 1, 0);//�ð�ť����
			//SND_FILENAME����ʾ��1������ָ����WAVE�ļ�����SND_SYNCͬ�������������ڲ������PlaySound�����ŷ���
			Sleep(100);//���ְ���״̬100����
			SendMessage(hButton, BM_SETSTATE, 0, 0);//�ð�ť�ɿ�
		}
		else
		{
			PlaySound(TEXT("error.wav"), nullptr, SND_SYNC | SND_FILENAME);//���Ű����ʱ������
			break;
		}
	case WM_COMMAND://���Ӵ��ڡ��˵��ϲ���ʱ��������
		wmId = LOWORD(wParam);//�ؼ�ID
		wmEvent = HIWORD(wParam);//�ؼ�֪ͨ�룬�����
		SetFocus(hwnd);//�ڿؼ��ϵ����������ʧȥ���㣬�����ٽ��ռ�����Ϣ�����������ػ񽹵�
		if (wmId == ID_ABOUT)//�˵���ֻ��1���˵���������˲˵�
		{
			break;
		}
		PlaySound(TEXT("push.wav"), nullptr, SND_SYNC | SND_FILENAME);//���Ű���ʱ������
		if(wmId== VK_BACK)//���»��˼���ɾ��һ������
		   ShowNumber(hwnd, number /= 16);//����16�൱������һλ��ɾ�����һ������
		else
		{
			if (wmId == VK_DELETE)//����ɾ���������������
				ShowNumber(hwnd, number = 0);
			else
			{
				if (isxdigit(wmId))//���������ʮ����������
				{
					if (bNewNum)//��Ϊ���²�����������Ű��µ����֣�����bNewNum��ʱΪtrue���ְ�����һ�����ֺ���Ϊfalse
					{
						firstNum = number;//����2����������1����
						number = 0;//��2������ʼ��Ϊ0
						bNewNum = false;//�ڶ��ΰ����ֵ�ʱ��ӦΪfalse
					}
					int theUnit = wParam - (isdigit(wParam) ? '0' : 'A' - 10);//��λ��������������ַ����ȥ�ַ���0���õ������������ȥ'A�����ټ���10
					//FFFFFFFF/16=FFFFFFF��FFFFFFF*16+����λ������FFFFFFFF
					if (number <MAXDWORD / 16)//���С��FFFFFFF����ô���������FҲ�������
					{
						number = number * 16 + theUnit;//����ǰ��������16���ټ��ϸ�λ��
						ShowNumber(hwnd, number);
					}
					else//���ڵ���32λ�������FFFFFFFF��1/16����FFFFFFF������ʱ*16+��λ���Ϳ��������̫���п������
					{
						if (theUnit == 0 && number == MAXDWORD / 16)
						{//���number�պ���FFFFFFF�����һλ��0�����������FҲ�������
							number = number * 16 + theUnit;//����ǰ��������16���ټ��ϸ�λ��
							ShowNumber(hwnd, number);
						}
						else//���number�պ���FFFFFFF�����һλ����0������F�ͻ����
						{//����number����8λ����������1λ���9λ�������
							MessageBox(hwnd, TEXT("ǰ��λ��ò�Ҫ����FFFFFFF\n���ǰ��λ����FFFFFFF�������һλֻ����0��������ܻ����\n���Ҳ�������9λ�������ΪFFFFFFFF"), TEXT("Warning"), MB_OK | MB_ICONWARNING);
						}
					}
				}
				else//�ǲ�����
				{
					if (!bNewNum)//���ǰһ����������֣������������
					{//�����1�������������=�����㣬�Ժ�ÿ������һ������������ǰһ������������ǰһ�ε�����
						number=Calulate(firstNum,iOperation, number);//����ǰһ�εĲ���������,�����Ľ�������ڵ�2 ����������
						ShowNumber(hwnd,number);//��ʾ������
					}
					bNewNum = true;//�����ַ����󣬽�������������־���������
					iOperation = wmId;//������ǰ������������һ�����������ʱ��ʹ��
				}
			}
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
	static TCHAR szAppName[]=TEXT("IDD_DIALOG");
	WNDCLASS wndclass;//������
	wndclass.cbClsExtra = 0;//Ϊ�����Ķ���ռ䣬���Ա���һЩ��ַ������
	wndclass.cbWndExtra =DLGWINDOWEXTRA	;//ע���Լ��ĶԻ�����̱����ڴ��ڶ���ռ����30���ֽ�
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);//������ˢ
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);//�����
	wndclass.hIcon = LoadIcon(hInstance,TEXT("MYICON"));//ͼ��
	wndclass.hInstance = hInstance;//ʵ�������������PID��һ���������������������ϵͳ�����ҵ����̵ĵ�ַ
	wndclass.lpfnWndProc = WndProc;//���ڹ���
	wndclass.lpszClassName = TEXT("zeng"); //����
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);//�˵�
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//��Щ�ı��Ҫ�ػ�������ˮƽ�ʹ�ֱ�ı���ػ�
	if (!RegisterClass(&wndclass))//ע�ᴰ����
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	MSG msg;//��Ϣ�ṹ
	//HWND hwnd = CreateDialog(hInstance,TEXT(IDD_DIALOG), 0, nullptr);
	HWND hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, nullptr);

	ShowWindow(hwnd, nCmdShow);
	while (GetMessage(&msg, nullptr, 0, 0))//����Ϣ���л�ȡ��Ϣ
	{
		TranslateMessage(&msg);//��һЩ������Ϣת�����ַ���Ϣ���ڰ�����Ϣ����
		DispatchMessage(&msg);//�ַ���Ϣ�������ô��ڹ��̽��д�����Ϣ
	}
}