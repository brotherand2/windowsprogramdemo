#include<Windows.h>
#include<ctime>
const int num =100;
int WinMain(HINSTANCE hInst, HINSTANCE hPreInst, char *szCmdCommand, int nShow)
{
	HWND hwnd = GetDesktopWindow();
	int x1, x2, y1, y2,cx,cy;
	cx= GetSystemMetrics(SM_CXSCREEN) / 10;
	cy = GetSystemMetrics(SM_CYSCREEN) / 10;
	HDC hdcScr, hdcMem;
	hdcScr = GetDCEx(hwnd, 0, DCX_LOCKWINDOWUPDATE);
	hdcMem = CreateCompatibleDC(hdcScr);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScr, cx, cy);
	SelectObject(hdcMem, hBitmap);
	static int keep[num][4];
	srand(time(0));
	Sleep(1000);
	//if (!LockWindowUpdate(hwnd))
	//	return 0;
	for (int i = 0; i <2; i++)
	{
		for (int j = 0; j < num; j++)
		{
			if (i == 0)
			{
				x1 =keep[j][0]= cx*(rand() % 10);
				y1 = keep[j][1] = cy*(rand() % 10);
				x2 = keep[j][2] = cx*(rand() % 10);
				y2 = keep[j][3] = cy*(rand() % 10);
			}
			else
			{
				x1 = keep[num - j - 1][0];
				y1 = keep[num - j - 1][1];
				x2 = keep[num -j - 1][2];
				y2 = keep[num - j - 1][3];
			}
			BitBlt(hdcMem, 0, 0, cx, cy, hdcScr, x1, y1, SRCCOPY);
			BitBlt(hdcScr, x1, y1, cx, cy, hdcScr, x2, y2, SRCCOPY);
			BitBlt(hdcScr, x2, y2, cx, cy, hdcMem, 0, 0, SRCCOPY);
			Sleep(5);
		}
	}
	ReleaseDC(hwnd,hdcScr);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);
	//LockWindowUpdate(0);
}