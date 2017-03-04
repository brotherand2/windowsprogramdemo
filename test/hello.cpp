# include<windows.h>
#pragma comment(lib, "WINMM.LIB")
//# include<tchar.h>
# include<stdio.h>
long _stdcall WinProc(HWND hwnd, unsigned int uMsg, unsigned int  wParam, long lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	char mouse[20];
	char *s = "textout";
	RECT rect;
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 100, 100, s, strlen(s));
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		hdc = GetDC(hwnd);
		char buffer[20];
		sprintf(buffer, "your enter %c", wParam);
		TextOut(hdc, 100, 200, buffer, strlen(buffer));
		//MessageBox(nullptr,buffer , "char message", MB_OK);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);
		DrawText(hdc, "this is zeng", -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		EndPaint(hwnd, &ps);
		break;

	case WM_CREATE:
		PlaySound(TEXT("login.wav"), NULL, SND_FILENAME | SND_SYNC | SND_LOOP);
		//MessageBox(NULL,"this is my first windows program","hello world!",MB_DEFBUTTON2);
		break;
	case WM_KEYDOWN:
		sprintf(mouse, "Äã°´ÏÂÁË¼üÅÌ");
		PlaySound(TEXT("Alarm01"), NULL, SND_SYNC | SND_LOOP);
		hdc = GetDC(hwnd);
		TextOut(hdc, 110, 150, mouse, strlen(mouse));
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SHOWWINDOW:
		DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	case WM_SIZE:
		DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;

	}
	return 0;
}

int  _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char * szCmdLine, int iCmdShow)
{
	char *message = "this is my first windows program";
	char info[100];
	sprintf(info, "hello %d world %d", 1, 2);
	// MessageBox(NULL,message,info,MB_ICONQUESTION|MB_YESNO);
	// MessageBox(NULL,"this is my first windows program","hello world!",MB_DEFBUTTON3);
	//	MessageBox(NULL,"this is my first windows program","hello world!",MB_DEFBUTTON4);
	WNDCLASS wndclass;
	HWND hwnd;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_CROSS);
	wndclass.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WinProc;
	wndclass.lpszClassName = "hello";
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClass(&wndclass);
	hwnd = CreateWindow("hello", "my window", WS_OVERLAPPEDWINDOW, 0, 0, 600, 400, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}