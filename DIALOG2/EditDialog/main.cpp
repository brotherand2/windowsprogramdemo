#include<Windows.h>
#include"resource.h"
int _stdcall DialogProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)
{
	int wmID = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);
	TCHAR szBuffer[1024];
	static HWND hEdit1, hEdit2;
	switch (message)
	{
	case WM_INITDIALOG:
		hEdit1 = GetDlgItem(hDlg, IDC_EDIT1);
		hEdit2 = GetDlgItem(hDlg, IDC_EDIT2);
		return true;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return false;
	case WM_COMMAND:
		switch (wmID)
		{
		case IDC_EDIT1:
			//if (wmEvent == EN_UPDATE)
			//{
			//	GetWindowText(hEdit1, szBuffer, 1024);
			//	SetWindowText(hEdit2, szBuffer);
			//}
			switch (wmEvent)
			{
			case EN_UPDATE:
				GetWindowText(hEdit1, szBuffer, 1024);
				SetWindowText(hEdit2, szBuffer);
				break;
			}
			break;
		case IDC_EDIT_CLER:
			SendMessage(hEdit1, WM_CLEAR, 0, 0);
			break;
		case IDC_EDIT_COPY:
			SendMessage(hEdit1, WM_COPY, 0, 0);
			break;
		case IDC_EDIT_RESET:
			SetWindowText(hEdit1, TEXT("This is a line"));
			break;
		case IDC_EDIT_PASTE:
			SendMessage(hEdit2, WM_PASTE, 0, 0);
			break;
		case IDC_EDIT_GET:
			GetWindowText(hEdit1, szBuffer, 1024);
			SetWindowText(hEdit2, szBuffer);
			break;
		}
		return true;
	}
	return false;
}
int WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, char *szCmdCommand, int nShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DialogProc);
}