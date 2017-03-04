#include<Windows.h>
#include<vector>
#include<string>
using namespace std;
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int  wParam, long lParam)
{
	static int cxClient, cyClient,col;
	PAINTSTRUCT ps;
	HDC hdc;
	static vector<string>vec;
	TEXTMETRIC tm;
	static int cyChar,cxChar;
	char szKeyName[21];
	switch (UMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		cxChar = tm.tmAveCharWidth;
		ReleaseDC(hwnd, hdc);
		break;
	case WM_KEYDOWN:
		GetKeyNameText(lParam, szKeyName, sizeof(szKeyName));
		vec.push_back(string("WM_KEYDOWN     ") + szKeyName);
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_SYSKEYDOWN:
		GetKeyNameText(lParam, szKeyName, sizeof(szKeyName));
		vec.push_back(string("WM_SYSKEYDOWN     ") + szKeyName);
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_SYSKEYUP:
		GetKeyNameText(lParam, szKeyName, sizeof(szKeyName));
		vec.push_back(string("WM_SYSKEYUP     ") + szKeyName);
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_KEYUP:
		GetKeyNameText(lParam, szKeyName, sizeof(szKeyName));
		vec.push_back(string("WM_KEYUP    ") + szKeyName);
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_CHAR:
		vec.push_back(string("WM_CHAR    ") + (char)wParam);
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_DEADCHAR:
		vec.push_back(string("WM_DEADCHAR") + (char)wParam);
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_SYSCHAR:
		vec.push_back(string("WM_SYSCHAR    ") + (char)wParam);
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_SYSDEADCHAR:
		vec.push_back(string("WM_SYSDEADCHAR") + (char)wParam);
		InvalidateRect(hwnd, nullptr, true);
		break;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		col = cyClient / cyChar;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SaveDC(hdc);
		SetMapMode(hdc,MM_ANISOTROPIC);
		SetWindowExtEx(hdc, 1, 1, nullptr);
		SetViewportExtEx(hdc, cxChar, cyChar, nullptr);
		if (vec.size() < col)
		{
			for (int i = 0; i < vec.size(); i++)
				TextOut(hdc, 0, i, vec.at(i).c_str(), vec.at(i).size());
		}
		else
		{
			for (int i = vec.size() - col,  j = 0; i < vec.size(); i++, j++)
				TextOut(hdc, 0, j, vec.at(i).c_str(), vec.at(i).size());
		}
		RestoreDC(hdc, -1);
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);
}
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *szCmdCommand, int nCmdShow)
{
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(nullptr, IDC_IBEAM);
	wndclass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = "zeng";
	wndclass.lpszMenuName = nullptr;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(nullptr, "register failed", "error", MB_APPLMODAL);
		return 0;
	}
	HWND hwnd = CreateWindow("zeng", "ÕýÏÒÇúÏß", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
	if (!hwnd)
	{
		MessageBox(nullptr, "create failed", "error", MB_APPLMODAL);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}