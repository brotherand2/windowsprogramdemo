#include<Windows.h>
#include<ctime>
#include<iostream>
const int num = 100;
int WinMain(HINSTANCE hInst, HINSTANCE hPreInst, char *szCmdCommand, int nShow)
{
	using namespace std;
























	HWND hwnd = GetDesktopWindow();
	LockWindowUpdate(hwnd);
	while (true)
	{
		Sleep(2000);
		cout << "sleep" << endl;
	}
}