//#ifndef UNICODE 
//#define tstring string
//#else
//#define tstring wstring
//#endif
#include<Windows.h>
#include<string>
#include<vector>
#include<memory>
using namespace std;
//#ifndef UNICODE 
//typedef  string tstring;
//#else
//typedef  wstring tstring;
//#endif
#ifndef UNICODE 
using  tstring = string;
#else
using  tstring = wstring;
#endif

long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int wParam, long lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	static int cxClient, cyClient;
	static int cxChar, cxCaps, cyChar;
	static vector<POINT>point;
	POINT curPoint;
	POINT *temp;
	switch (UMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		point.clear();
	case WM_LBUTTONUP:
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_MOUSEMOVE:
		//hdc = GetDC(hwnd);
		if (wParam&MK_LBUTTON)
			//SetPixel(hdc, LOWORD(lParam), HIWORD(lParam), RGB(255, 0, 0));
		{
			curPoint.x = LOWORD(lParam);
			curPoint.y = HIWORD(lParam);
			point.push_back(curPoint);
		}
		//ReleaseDC(hwnd,hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//temp = new POINT[point.size()];
		//for (int i = 0; i < point.size(); i++)
		//	temp[i] = point[i];
		//for (int i = 0; i < point.size();i++)
		//Polygon(hdc,temp+i, point.size()-i);

		SetCursor(LoadCursor(nullptr,IDC_WAIT));
		for (int i = 0; i <(int) point.size() - 1; i++)
		{
			MoveToEx(hdc, point[i].x, point[i].y, nullptr);
			for (int j = i + 1; j < point.size(); j++)
			{
				LineTo(hdc, point[j].x, point[j].y);
				MoveToEx(hdc, point[i].x, point[i].y, nullptr);
			}
		}
		SetCursor(LoadCursor(nullptr,IDC_ARROW));
		//delete[]temp;
		EndPaint(hwnd, &ps);
		break;
	//case WM_NCHITTEST:
	//	return HTNOWHERE;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, UMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *  szCmdCommand, int nCmdShow)
{
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(nullptr, IDC_ICON);
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = TEXT("zeng");
	wndclass.lpszMenuName = nullptr;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(nullptr, TEXT("register failed"), TEXT("error"), MB_ICONINFORMATION);
		return 0;
	}
	HWND hwnd = CreateWindow(TEXT("zeng"), TEXT("keyScroll"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);
	if (!hwnd)
	{
		MessageBox(nullptr, TEXT("create window failed"), TEXT("error"), MB_ICONASTERISK);
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		DispatchMessage(&msg);
	}
}