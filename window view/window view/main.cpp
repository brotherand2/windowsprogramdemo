#include<tchar.h>
#include<strsafe.h>
#include<afxwin.h>
//#include<Windows.h>
void show(HWND hwnd, HDC hdc, int x, int y, int iMapMode, char *szMapMode)
{
	SaveDC(hdc);
	SetMapMode(hdc,iMapMode);
	RECT rect;
	GetClientRect(hwnd, &rect);
	DPtoLP(hdc,(PPOINT) &rect, 2);
	char buffer[128];
	RestoreDC(hdc, -1);
	TextOut(hdc, x, y, buffer, wsprintf(buffer, "%-20s %7d%7d%7d%7d", szMapMode, rect.left, rect.right, rect.top, rect.bottom));

}
long _stdcall WndProc(HWND hwnd, unsigned int UMsg, unsigned int  wParam, long lParam)
{
	static int cxClient, cyClient;
	static int cxChar, cyChar;
	TEXTMETRIC tm;
	char buffer[128];
	HDC hdc;
	PAINTSTRUCT ps;
	static POINT pt,ptScreen;
	HPEN hPen, oldPen;
	switch (UMsg)
	{
	case WM_SIZE:
		hdc = GetDC(hwnd);
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		pt.x = pt.y = 0;
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		//wsprintf(buffer, "ni hao wo shi %d", 10);
		StringCchPrintf(buffer, 12, "ni hao wo shi %d", 10);
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, 1, 1, nullptr);
		SetViewportExtEx(hdc, 10, 10, nullptr);
		TextOut(hdc,40, 40, buffer, lstrlen(buffer));
		Rectangle(hdc, 30, 30, 35, 35);
		SetMapMode(hdc, MM_LOENGLISH);
		TextOut(hdc, 50, -100, "hello", 5);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_RBUTTONDOWN:
		hdc = GetDC(hwnd);
		{
		   CPoint cpt(100,100);
			//POINT cpt;
			//cpt.x = cpt.y = 10;
		   int oldMode=SetMapMode(hdc, MM_HIMETRIC);
		   hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		   oldPen=(HPEN)SelectObject(hdc, hPen);
		   //MoveToEx(hdc, 0, -100, nullptr);
		   //LineTo(hdc, 1000,- 100);
		   DPtoLP(hdc, &cpt, 1);
		   MoveToEx(hdc, cpt.x, cpt.y, nullptr);
		   LineTo(hdc, cpt.x + 1000, cpt.y);
		   SetMapMode(hdc, oldMode);
		   //SetViewportOrgEx(hdc, 0, cyClient,nullptr);
		   //Rectangle(hdc, 200, -200, 300, -300);
		  // SetWindowOrgEx(hdc, 0, cyClient,nullptr);
		  // Rectangle(hdc, 100, cyClient + 100, 200, cyClient + 200);
		   //SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, nullptr);
		   //Rectangle(hdc, -200, -200, 200, 200);
		   //SetWindowOrgEx(hdc, -cxClient / 2,-cyClient / 2, nullptr);
		   //Rectangle(hdc, -200, -200, 200, 200);
		   //SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, nullptr);
		   //SetWindowOrgEx(hdc, -cxClient / 2, -cyClient / 2, nullptr);
		   //Rectangle(hdc, -100, -100, -200, -200);
		   //SetMapMode(hdc, MM_LOENGLISH);
		   //SetViewportOrgEx(hdc, 0, cyClient, nullptr);
		   //Rectangle(hdc, 100, 100, 200, 200);
		   //SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, nullptr);
		   //Rectangle(hdc, -100, 100, 100, -100);
		   //CPoint cp2(cxClient, cyClient);
		   //DPtoLP(hdc, &cp2, 1);
		   //SetWindowOrgEx(hdc, -cp2.x/ 2, -cp2.y / 2, nullptr);
		   //Rectangle(hdc, -100, 100, 100, -100);
		   SetMapMode(hdc, MM_TEXT);
		   SetMapMode(hdc, MM_ISOTROPIC);
		   //SetWindowExtEx(hdc, 100, 100, nullptr);
		   //SetViewportExtEx(hdc, cxClient, -cyClient, nullptr);
		   //SetViewportOrgEx(hdc, 0, cyClient, nullptr);
		   //Rectangle(hdc, 50, 50,100, 100);
		   //SetWindowOrgEx(hdc, 0, 100, nullptr);
		   //Rectangle(hdc,0,0, 50, 50);
		   SetWindowExtEx(hdc, 50, 50, nullptr);
		   SetViewportExtEx(hdc, cxClient/2 , -cyClient/2 , nullptr);
		   SetViewportOrgEx(hdc,cxClient/2,cyClient/2, nullptr);
		   Rectangle(hdc, -40, -40, 40,40);
		   DeleteObject(SelectObject(hdc, oldPen));
		}

		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, 1, 1, nullptr);
		SetViewportExtEx(hdc, 10, -10, nullptr);
		SetViewportOrgEx(hdc, 0, cyClient, nullptr);
		Rectangle(hdc, 10, 10, 20, 20);
		SetWindowExtEx(hdc, 100, 100, nullptr);
		SetViewportExtEx(hdc, cxClient, -cyClient, nullptr);
		SetViewportOrgEx(hdc, 0, cyClient, nullptr);
		Rectangle(hdc, 40, 40, 60, 60);
		SetWindowExtEx(hdc, 5, 5, nullptr);
		SetViewportExtEx(hdc, cxClient / 2, -cyClient / 2, nullptr);
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, nullptr);
		MoveToEx(hdc, -5,0, nullptr);
		LineTo(hdc,5, 0);
		MoveToEx(hdc, 0, -5, nullptr);
		LineTo(hdc, 0, 5);
		Ellipse(hdc, 0, 0, 1, 1);

		ReleaseDC(hwnd,hdc);
		break;
	case WM_MOUSEMOVE:
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		InvalidateRect(hwnd, nullptr, true);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		ptScreen = pt;
		ClientToScreen(hwnd, &ptScreen);
		StringCchPrintf(buffer, 128, "xClient:%d  yClient:%d \nxScreen:%d yScreen:%d", pt.x, pt.y,ptScreen.x,ptScreen.y);
		DrawText(hdc, buffer, -1, &ps.rcPaint, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, 1, 1, nullptr);
		SetViewportExtEx(hdc, cxChar, cyChar,nullptr);
		show(hwnd, hdc, 1, 1, MM_TEXT, "TEXT(pixels)");
		show(hwnd, hdc, 1,2, MM_LOMETRIC, "LOMETRIC(0.1mm)");
		show(hwnd, hdc, 1, 3, MM_HIMETRIC, "HIMETRIC(0.01mm)");
		show(hwnd, hdc, 1, 4, MM_LOENGLISH, "LOENGLISH(0.01in)");
		show(hwnd, hdc, 1, 5, MM_HIENGLISH, "HIENGLISH(0.001in)");
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
	HBITMAP pic = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(101));
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