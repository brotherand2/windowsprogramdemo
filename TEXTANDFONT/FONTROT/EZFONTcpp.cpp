#include<Windows.h>
#include"resource.h"
HFONT EZCreateFont(HDC hdc,TCHAR *szFaceName,int iDeciPtWidth,int iDeciPtHeight,bool fLogRes)
{
	HFONT hFont;
	static LOGFONT lf;
	double cxDpi, cyDpi;
	SaveDC(hdc);
	POINT pt;
	if (fLogRes)
	{
		cxDpi = (double)GetDeviceCaps(hdc, LOGPIXELSX);
		cyDpi = (double)GetDeviceCaps(hdc, LOGPIXELSY);
	}
	else
	{
		cxDpi = (double)(25.4*GetDeviceCaps(hdc, HORZRES) / GetDeviceCaps(hdc, HORZSIZE));
		cyDpi = (double)(25.4*GetDeviceCaps(hdc, VERTRES) / GetDeviceCaps(hdc, VERTSIZE));
	}
	pt.y = (int)(cyDpi*iDeciPtHeight / 720); 
	pt.x= (int)(cxDpi*iDeciPtWidth / 720);
	DPtoLP(hdc, &pt, 1);
	lf.lfHeight =-(int)(pt.y+0.5);
	lf.lfWidth = 0;
	lstrcpyn(lf.lfFaceName, szFaceName, LF_FULLFACESIZE);
	hFont = CreateFontIndirect(&lf);
	RestoreDC(hdc, -1);
	return hFont;
}
void PaintRoutine(HDC hdc)
{
	HFONT hFont;
	int iPointSize;
	TCHAR szBuffer[128];
	LOGFONT lf;
	TEXTMETRIC tm;
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, 1440, 1440, 0);
	SetViewportExtEx(hdc, GetDeviceCaps(hdc, LOGPIXELSX), GetDeviceCaps(hdc, LOGPIXELSY), 0);
	int y = 0;
	for (iPointSize = 80; iPointSize < 120; iPointSize++)
	{
		hFont = EZCreateFont(hdc, TEXT("Times New Roman"), 0, iPointSize, true);
		SelectObject(hdc, hFont);
		GetObject(hFont, sizeof(LOGFONT), &lf);
		GetTextMetrics(hdc, &tm);
		wsprintf(szBuffer, TEXT("Times New Roman font of%d.%d points,lf.lfHeight=%d,tm.tmHeight=%d"), iPointSize / 10, iPointSize % 10, lf.lfHeight, tm.tmHeight);
		TextOut(hdc, 0, y, szBuffer, lstrlen(szBuffer));
		y += tm.tmHeight;
		DeleteObject((HFONT)SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
	}
}

bool PrintText(HDC hdc)
{
	int cxPage, cyPage;
	bool bSuccess = false;
	cxPage = GetDeviceCaps(hdc, VERTRES);
	cyPage = GetDeviceCaps(hdc, HORZRES);
	DOCINFO di{ sizeof(di), TEXT("SZTEXT") };
	if (StartDoc(hdc, &di) > 0 && StartPage(hdc) > 0)
	{
		PaintRoutine(hdc);
		if (EndPage(hdc) > 0)
		{
			bSuccess = true;
			EndDoc(hdc);
			return true;
		}
	}
	return false;
}