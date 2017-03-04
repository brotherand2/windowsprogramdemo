#include<memory>
#include<string>

#include<Windows.h>
using namespace std;
#include"resource.h"
int _stdcall DialogProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)
{
	int wmID = LOWORD(wParam);//¿Ø¼þID
	static HWND hListLeft, hListMiddle, hListRight; 
	int index;
	TCHAR szBuffer[256];
	TCHAR *pText;
	int len;
	int count;
	switch (message)
	{
	case WM_INITDIALOG:
		hListLeft = GetDlgItem(hDlg, IDC_LISTLEFT);
		hListMiddle = GetDlgItem(hDlg, IDC_LISTMIDDLE);
		hListRight = GetDlgItem(hDlg, IDC_LISTRIGHT);

		return false;
	case WM_COMMAND:
		switch (wmID)
		{
			case IDC_MOVERIGHT1:
				index = SendMessage(hListLeft, LB_GETCURSEL, 0,0);
				if (index != LB_ERR)
				{
					len = SendMessage(hListLeft, LB_GETTEXTLEN, index, 0);
					pText = new TCHAR[len + 1];
					shared_ptr<TCHAR>sp(pText, [](TCHAR *p){delete[]p; });
					SendMessage(hListLeft, LB_GETTEXT, index, (long)pText);
					SendMessage(hListMiddle, LB_INSERTSTRING, -1, (long)pText);
					SendMessage(hListLeft, LB_DELETESTRING, index, 0);
				}
				break;
			case IDC_MOVERIGHT2:
				count = SendMessage(hListMiddle, LB_GETCOUNT, 0, 0);
				if (count)
				{
					for (int index = count - 1; index >= 0; index--)
					{
						if (SendMessage(hListMiddle, LB_GETSEL, index, 0))
						{
							len = SendMessage(hListMiddle, LB_GETTEXTLEN, index, 0);
							pText = new TCHAR[len + 1];
							shared_ptr<TCHAR>sp(pText, [](TCHAR *p){delete[]p; });
							SendMessage(hListMiddle, LB_GETTEXT, index, (long)pText);
							SendMessage(hListRight, LB_INSERTSTRING, -1, (long)pText);
							SendMessage(hListMiddle, LB_DELETESTRING, index, 0);
						}
					}
				}
			case IDC_INSERTTHREELEFT:
				SendMessage(hListLeft, LB_ADDSTRING, 0, (long)TEXT("hello"));
				SendMessage(hListLeft, LB_ADDSTRING, 0, (long)TEXT("world"));
				SendMessage(hListLeft, LB_ADDSTRING, 0, (long)TEXT("program"));
				break;
			case IDC_INSERTTHREERIGHT:
				SendMessage(hListMiddle, LB_INSERTSTRING, -1, (long)TEXT("one"));
				SendMessage(hListMiddle, LB_INSERTSTRING, -1, (long)TEXT("two"));
				SendMessage(hListMiddle, LB_INSERTSTRING, -1, (long)TEXT("three"));
				break;
			case IDC_DELETETHIRD:
				SendMessage(hListLeft, LB_DELETESTRING, 2, 0);
				break;
			case IDC_INSERTTHIRD:
				SendMessage(hListMiddle, LB_INSERTSTRING, 2, (long)TEXT("nihao"));
				break;
			case IDC_SETTHIRDANDSIXTH:
				SendMessage(hListMiddle, LB_SETSEL,1, 2);
				SendMessage(hListMiddle, LB_SETSEL,1, 5);
				break;
			case IDC_UNSETTHIRDANDSIXTH:
				SendMessage(hListMiddle, LB_SETSEL, 0 ,2);
				SendMessage(hListMiddle, LB_SETSEL,  0,5);
				break;
			case IDC_SELECTALL:
				SendMessage(hListMiddle, LB_SETSEL, 1, -1);
				break;
			case IDC_RESET:
				SendMessage(hListLeft, LB_RESETCONTENT, 0, 0);
				break;
			case IDC_SELECTTHIRD:
				SendMessage(hListLeft, LB_SETCURSEL, 2,0);
				break;
			case IDC_UNSET:
				index = SendMessage(hListLeft, LB_GETCURSEL, 0, 0);
				if (index != LB_ERR)
					SendMessage(hListLeft, LB_SETCURSEL, -1, 0);
				break;

		}
		return true;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return false;
	}
	return false;
}
int WinMain(HINSTANCE hInstance, HINSTANCE hPreHinstance, char *szCmdCommand, int nShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DialogProc);
}