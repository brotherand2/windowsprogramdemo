#include<Windows.h>
#include"resource.h"
void PintTheBlock(HWND hwnd, int drawColor, int iFigure)
{
	HDC hdc = GetDC(hwnd);
	HBRUSH brush = CreateSolidBrush(drawColor);
	HBRUSH old =(HBRUSH) SelectObject(hdc, brush);
	RECT rect;
	GetClientRect(hwnd, &rect);
	InvalidateRect(hwnd, nullptr, true);
	UpdateWindow(hwnd);
	if (iFigure == IDC_RECTANGLE)
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	else
		Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
	SelectObject(hdc, old);
	DeleteObject(brush);
	ReleaseDC(hwnd, hdc);
}
int _stdcall DialogProc(HWND hDlg, unsigned message, unsigned wParam, long lParam)
{
	static HWND hCtrlBlock;//»æÍ¼Çø¾ä±ú
	static int iFigure;//ÂÖÀª
	static int drawColor;//ÑÕÉ«
	int wmID = LOWORD(wParam);//¿Ø¼þID
	switch (message)
	{
	case WM_INITDIALOG:
		hCtrlBlock = GetDlgItem(hDlg,IDC_PAINT);
		iFigure = IDC_RECTANGLE;
		drawColor = RGB(255, 255, 255);
		SendMessage(GetDlgItem(hDlg, IDC_RECTANGLE), BM_SETCHECK, 1, 0);
		SendMessage(GetDlgItem(hDlg, IDC_WHITE), BM_SETCHECK, 1, 0);
		return true;
	case WM_COMMAND:
		switch (wmID)
		{
		case IDC_BLACK:
			drawColor = RGB(0, 0, 0);
			break;
		case IDC_BLUE:
			drawColor = RGB(0, 0, 255);
			break;
		case IDC_GREEN:
			drawColor = RGB(0, 255, 0);
			break;
		case IDC_CYAN:
			drawColor = RGB(0, 255,255);
			break;
		case IDC_RED:
			drawColor = RGB(255, 0, 0);
			break;
		case IDC_MAGENTA:
			drawColor = RGB(255, 0, 255);
			break;
		case IDC_YELLOW:
			drawColor = RGB(255, 255, 0);
			break;
		case IDC_WHITE:
			drawColor = RGB(255, 255,255);
			break;
		case IDC_RECTANGLE:
			iFigure=IDC_RECTANGLE;
			break;
		case IDC_ELLIPSE:
			iFigure=IDC_ELLIPSE;
			break;
		}
		PintTheBlock(hCtrlBlock, drawColor, iFigure);
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return false;
	case WM_PAINT:
		PintTheBlock(hCtrlBlock, drawColor, iFigure);
		return false;
	}
	return false;
}
int WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, char *szCmdCommand, int nShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DialogProc);

}