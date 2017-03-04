#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <time.h>
#include <math.h>

#define WINDOW_WIDTH    968                            
#define WINDOW_HEIGHT   751                            
#define WINDOW_TITLE    _T("Demo")
#define WINDOW_CLASS    _T("ForTheTest")

HWND InitInstance(HINSTANCE hInstance, int nShowCmd);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL GameInit(HWND hWnd);
BOOL GameAction(HWND hWnd);
BOOL GamePaint(HWND hWnd);
BOOL GameClearup(HWND hWnd);

HINSTANCE g_hInst;

struct FireStar
{
	float x;
	float y;
	float vx;
	float vy;
	int time;
};

enum HeadState{ DEAD, UP, BOW };

struct FireHead
{
	float x;
	float y;
	float power;
	float v;
	HeadState state;
};

struct FireBow
{
	FireStar sFireStar[12][80];
	int life;
};

#define NUM_FIRESTAR 10000		// 烟火上升轨迹粒子数
#define NUM_FIREHEAD 5			// 烟火个数
#define NUM_FIREBOW  10			// 烟火爆炸轨迹数
#define NUM_DOWN	 0.03		// 烟火粒子下落像素

FireStar g_sFireStar[NUM_FIRESTAR];			// 烟火上升轨迹
FireHead g_sFireHead[NUM_FIREHEAD];			// 烟火中心点
FireBow  g_sFireBow[NUM_FIREBOW];			// 烟火爆炸轨迹

HDC g_hdcMem;
HDC g_hdcMem1;
HBITMAP g_hbmpMem;
HBITMAP g_hbmpBk;
HPEN g_hpen[35];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = GetStockBrush(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = WINDOW_CLASS;

	if (!RegisterClassEx(&wndClass))
		return -1;

	g_hInst = hInstance;

	HWND hWnd = InitInstance(hInstance, nShowCmd);
	if (!hWnd)
		return -1;

	MSG msg = { 0 };
	DWORD dwTick, dwPreTick = GetTickCount();
	if (!GameInit(hWnd))
		return -1;

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			dwTick = GetTickCount();
			if (dwTick - dwPreTick > 50)
			{
				if (GameAction(hWnd))
					GamePaint(hWnd);
				dwPreTick = GetTickCount();
			}
		}
	}
	UnregisterClass(WINDOW_CLASS, wndClass.hInstance);
	return 0;
}

// 居中显示
HWND InitInstance(HINSTANCE hInstance, int nShowCmd)
{
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	HWND hWnd = CreateWindow(WINDOW_CLASS, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		(cx - WINDOW_WIDTH) / 2, (cy - WINDOW_HEIGHT) / 2, WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL, NULL, hInstance, NULL);
	if (!hWnd)
		return NULL;
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);
	return hWnd;
}

// 窗口过程
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
	{
		GameClearup(hwnd);
		DestroyWindow(hwnd);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

// 游戏初始化
BOOL GameInit(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	g_hdcMem = CreateCompatibleDC(hdc);
	g_hdcMem1 = CreateCompatibleDC(hdc);
	g_hbmpMem = CreateCompatibleBitmap(hdc, 954, 718);
	g_hbmpBk = (HBITMAP)LoadImage(g_hInst, _T("Res/background1.bmp"), IMAGE_BITMAP, 954, 718, LR_LOADFROMFILE);

	SelectBitmap(g_hdcMem, g_hbmpMem);
	SelectBitmap(g_hdcMem1, g_hbmpBk);
	ReleaseDC(hWnd, hdc);

	for (int i = 0; i<35; i++)
	{
		g_hpen[i] = CreatePen(PS_SOLID, 1, RGB(255 - i * 7, 255, 255));
	}

	// 数据初始化
	srand((unsigned int)time(NULL));
	memset(g_sFireStar, 0, sizeof(FireStar)*NUM_FIRESTAR);
	memset(g_sFireHead, 0, sizeof(FireHead)*NUM_FIREHEAD);
	memset(g_sFireBow, 0, sizeof(FireBow)*NUM_FIREBOW);

	return TRUE;
}

// FireHead Up
void FireHeadUp(int nIndex)
{
	// 前进
	g_sFireHead[nIndex].y -= g_sFireHead[nIndex].v;
	g_sFireHead[nIndex].v--;
	if (g_sFireHead[nIndex].v == 0)
		g_sFireHead[nIndex].state = BOW;

	// 产生 FireStar
	int n = (int)(g_sFireHead[nIndex].power / 10);
	int count = (int)g_sFireHead[nIndex].v + 1;

	for (int i = 0; i<NUM_FIRESTAR && count > 0;)	// 填充路程每个像素
	{
		for (int j = 0; j<n; i++)		// 根据 power 每个像素产生 n个 firestar
		{
			if (g_sFireStar[i].time == 0)
			{
				g_sFireStar[i].time = rand() % 10 + 10;
				g_sFireStar[i].y = g_sFireHead[nIndex].y + count;
				g_sFireStar[i].x = g_sFireHead[nIndex].x;
				g_sFireStar[i].vx = ((rand() % 2 == 0) ? 1 : -1) *
					((float)(rand() % (10 + n * 3)) / (3 * g_sFireStar[i].time));	// 在随机时间内依据 power大小 随机横向速度
				g_sFireStar[i].vy = 0;	// 暂时不掉落

				j++;	// 继续产生
			}
		}
		count--; // 提升高度
	}
}

// FireHead Bow
void FireHeadBow(int nIndex)
{
	g_sFireHead[nIndex].v--;
	if (g_sFireHead[nIndex].v == -3)
		g_sFireHead[nIndex].state = DEAD;

	// 产生爆炸轨迹
	for (int i = 0; i<NUM_FIREBOW; i++)
	{
		if (g_sFireBow[i].life == 0)
		{
			int nMax = 0;
			for (int j = 0; j<12; j++)		// 产生12条轨迹线
			{
				int n = rand() % 50 + 20;		// 随机爆炸力度
				if (nMax < n)
					nMax = n;

				g_sFireBow[i].sFireStar[j][0].time = n;		// 设定生存时间
				g_sFireBow[i].sFireStar[j][0].vx = (float)(cos(30 * j*3.14 / 180)*1.0);
				g_sFireBow[i].sFireStar[j][0].vy = (float)(sin(30 * j*3.14 / 180)*1.0);
				g_sFireBow[i].sFireStar[j][0].x = g_sFireHead[nIndex].x + 3 * g_sFireBow[i].sFireStar[j][0].vx;
				g_sFireBow[i].sFireStar[j][0].y = g_sFireHead[nIndex].y + 3 * g_sFireBow[i].sFireStar[j][0].vy;
			}
			g_sFireBow[i].life = nMax;
			break;
		}
	}

}

// 游戏逻辑
BOOL GameAction(HWND hWnd)
{
	// 产生 FireHead  630 830  650  710
	static int tag = 0;	// 随机概率
	int nRandCreate = rand() % 300;
	if (nRandCreate < tag)
	{
		for (int i = 0; i<NUM_FIREHEAD; i++)
		{
			if (g_sFireHead[i].state == DEAD)	// 寻找空闲的 FireHead
			{
				g_sFireHead[i].state = UP;
				g_sFireHead[i].x = (float)(rand() % 200 + 630);
				g_sFireHead[i].y = (float)(rand() % 60 + 650);
				g_sFireHead[i].power = (float)(rand() % 15 + 20);  // 20-35 威力
				g_sFireHead[i].v = g_sFireHead[i].power;

				tag = 0;
				break;
			}
		}
	}
	else
	{
		tag++;
	}

	// 遍历 FireHead 状态，计算 FireHead 位置
	for (int i = 0; i<NUM_FIREHEAD; i++)
	{
		switch (g_sFireHead[i].state)
		{
		case UP:
			FireHeadUp(i);
			break;
		case BOW:
			FireHeadBow(i);
			break;
		default:
			break;
		}
	}

	// 遍历 FireStar 状态，计算 FireStar 位置
	for (int i = 0; i<NUM_FIRESTAR; i++)
	{
		if (g_sFireStar[i].time > 0)
		{
			g_sFireStar[i].time--;
			g_sFireStar[i].x += g_sFireStar[i].vx;
		}
	}

	// 遍历 FireBow 状态，计算 FireBow 位置
	for (int i = 0; i<NUM_FIREBOW; i++)
	{
		if (g_sFireBow[i].life > 0)
		{
			g_sFireBow[i].life--;

			for (int j = 0; j<12; j++)		// 遍历12条轨迹线
			{
				for (int k = 0; k<79; k++)	// 修改轨迹线time值
				{
					if (g_sFireBow[i].sFireStar[j][k].time > 0)
					{
						if (g_sFireBow[i].sFireStar[j][k + 1].time == 0)
						{
							g_sFireBow[i].sFireStar[j][k + 1].time = g_sFireBow[i].sFireStar[j][k].time - 1;
							g_sFireBow[i].sFireStar[j][k + 1].x = g_sFireBow[i].sFireStar[j][k].x + g_sFireBow[i].sFireStar[j][k].vx;
							g_sFireBow[i].sFireStar[j][k + 1].y = g_sFireBow[i].sFireStar[j][k].y + g_sFireBow[i].sFireStar[j][k].vy;
							g_sFireBow[i].sFireStar[j][k + 1].vy = g_sFireBow[i].sFireStar[j][k].vy + (float)NUM_DOWN;
							g_sFireBow[i].sFireStar[j][k + 1].vx = g_sFireBow[i].sFireStar[j][k].vx;

							g_sFireBow[i].sFireStar[j][k].time = ((g_sFireBow[i].sFireStar[j][k].time - 2)<0) ? 0 :
								(g_sFireBow[i].sFireStar[j][k].time - 2);
							break;
						}
						g_sFireBow[i].sFireStar[j][k].time = ((g_sFireBow[i].sFireStar[j][k].time - 2)<0) ? 0 : (g_sFireBow[i].sFireStar[j][k].time - 2);
					}
				}
			}
		}
	}

	return TRUE;
}

// 游戏绘制
BOOL GamePaint(HWND hWnd)
{
	BitBlt(g_hdcMem, 0, 0, 954, 718, g_hdcMem1, 0, 0, SRCCOPY);

	// 遍历 FireHead 绘制
	for (int i = 0; i<NUM_FIREHEAD; i++)
	{
		if (g_sFireHead[i].state == BOW)
		{
			float v = fabs(g_sFireHead[i].v);
			int n = (int)(v*(g_sFireHead[i].power / 12.0));	// 计算 光晕半径

			while (n >= 0)	// 绘制渐变光晕
			{
				SelectPen(g_hdcMem, g_hpen[n]);
				Ellipse(g_hdcMem, (int)(g_sFireHead[i].x - n), (int)(g_sFireHead[i].y - n), (int)(g_sFireHead[i].x + n), (int)(g_sFireHead[i].y + n));
				n--;
			}
		}
	}

	// 遍历 FireStar 绘制
	for (int i = 0; i<NUM_FIRESTAR; i++)
	{
		if (g_sFireStar[i].time > 15)
		{
			SetPixel(g_hdcMem, (int)g_sFireStar[i].x, (int)g_sFireStar[i].y, RGB(185, 235, 255));
		}
		else if (g_sFireStar[i].time > 10 && g_sFireStar[i].time <= 15)
		{
			SetPixel(g_hdcMem, (int)g_sFireStar[i].x, (int)g_sFireStar[i].y, RGB(140, 240, 240));
		}
		else if (g_sFireStar[i].time > 5 && g_sFireStar[i].time <= 10)
		{
			SetPixel(g_hdcMem, (int)g_sFireStar[i].x, (int)g_sFireStar[i].y, RGB(88, 240, 240));
		}
		else if (g_sFireStar[i].time > 0)
		{
			SetPixel(g_hdcMem, (int)g_sFireStar[i].x, (int)g_sFireStar[i].y, RGB(240, 200, 80));
		}
	}

	// 遍历 FireBow 绘制
	for (int i = 0; i<NUM_FIREBOW; i++)
	{
		if (g_sFireBow[i].life > 0)
		{
			for (int j = 0; j<12; j++)
			{
				for (int k = 0; k<80; k++)
				{
					if (g_sFireBow[i].sFireStar[j][k].time > 0)
					{
						SetPixel(g_hdcMem, (int)g_sFireBow[i].sFireStar[j][k].x, (int)g_sFireBow[i].sFireStar[j][k].y, RGB(140, 240, 240));
					}
				}
			}
		}
	}

	// 贴图
	HDC hdc = GetDC(hWnd);
	BitBlt(hdc, 0, 0, 954, 718, g_hdcMem, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hdc);

	return TRUE;
}

// 游戏清除
BOOL GameClearup(HWND hWnd)
{
	DeleteBitmap(g_hbmpBk);
	DeleteBitmap(g_hbmpMem);

	DeleteDC(g_hdcMem);
	DeleteDC(g_hdcMem1);

	for (int i = 0; i<35; i++)
		DeletePen(g_hpen[i]);

	return TRUE;
}