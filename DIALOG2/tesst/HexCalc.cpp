#include<Windows.h>
#include"RESOURCE1.H"
int _stdcall DialogProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)
{
	int wmID = LOWORD(wParam);//控件ID
	int wmEvent = HIWORD(wParam);//控件操作
	HDC hdc;
	TCHAR Select[] = TEXT("按钮被选择");
	TCHAR UnSelect[] = TEXT("按钮未选择");
	TCHAR szBuffer[128];
	static int cxChar, cyChar;
	static HWND hButton, hRadio, hCheck;
	switch (message)
	{
	case WM_INITDIALOG:
		cxChar =LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		hButton = GetDlgItem(hDlg, IDC_OK);
		hCheck = GetDlgItem(hDlg, IDC_CHECK);
		hRadio = GetDlgItem(hDlg, IDC_RADIO);
		return false;
	case WM_COMMAND:
		switch (wmID)
		{
		case IDC_OK:
			MessageBox(hDlg, TEXT("you push ok"), TEXT(""), 0);
			break;
		case IDC_SETCHECK:
			SendMessage(GetDlgItem(hDlg, IDC_CHECK), BM_SETCHECK, 1, 0);
			SendMessage(GetDlgItem(hDlg, IDC_RADIO), BM_SETCHECK, 1, 0);
			break;
		case IDC_SETUNCHECK:
			SendMessage(GetDlgItem(hDlg, IDC_CHECK), BM_SETCHECK, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_RADIO), BM_SETCHECK, 0, 0);
			break;
		case IDC_GETCHECK:
			hdc = GetDC(hDlg);
			SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
			//SetTextColor(hdc, GetSysColor(COLOR_BTNTEXT));
			
			if (SendMessage(GetDlgItem(hDlg, IDC_CHECK), BM_GETCHECK, 0, 0))
				TextOut(hdc, 41, 20, Select, lstrlen(Select));
			else
				TextOut(hdc, 41, 20, UnSelect, lstrlen(UnSelect));

			ReleaseDC(hDlg,hdc);
			break;
		case IDC_SETTEXT:
			SetWindowText(GetDlgItem(hDlg, IDC_OK), TEXT("hello"));
			SetWindowText(GetDlgItem(hDlg, IDC_CHECK), TEXT("hello"));
			SetWindowText(GetDlgItem(hDlg, IDC_RADIO), TEXT("hello"));
			break;
		case IDC_GETTEXT:
			GetWindowText(GetDlgItem(hDlg, IDC_OK), szBuffer, 128);
			hdc = GetDC(hDlg);
			SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
			TextOut(hdc, 190, 20, szBuffer, lstrlen(szBuffer));
			ReleaseDC(hDlg, hdc);
			break;
		case IDC_SHOWHIDE:
			ShowWindow(hButton, IsWindowVisible(hButton) ? SW_HIDE : SW_NORMAL);
			ShowWindow(hCheck, IsWindowVisible(hCheck) ? SW_HIDE : SW_NORMAL);
			ShowWindow(hRadio, IsWindowVisible(hRadio) ? SW_HIDE : SW_NORMAL);
			break;
		case IDC_ENABLE:
			EnableWindow(hButton, IsWindowEnabled(hButton) ? false:true);
			EnableWindow(hCheck, IsWindowEnabled(hCheck) ? false : true);
			EnableWindow(hRadio, IsWindowEnabled(hRadio) ? false : true);
			break;
		}
		return true;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return false;
	}
	return false;
}
int WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, char *szCmdCommand, int nShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), nullptr, DialogProc);

}