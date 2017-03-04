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
void PaintRoutine(HDC hdc,int cxArea,int cyArea)
{
	HFONT hFont;
	int iPointSize;
	TCHAR szBuffer[128]=TEXT("   Rotation");
	LOGFONT lf;
	TEXTMETRIC tm;
	hFont = EZCreateFont(hdc, TEXT("Times New Roman"), 0, 540, true);
	GetObject(hFont, sizeof(LOGFONT), &lf);
	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, TA_BASELINE);
	for (int i = 0; i < 12; i++)
	{
		lf.lfEscapement =i * 300;
		SelectObject(hdc, CreateFontIndirect(&lf));
		TextOut(hdc,cxArea / 2, cyArea / 2 ,szBuffer ,lstrlen( szBuffer));
		DeleteObject((HFONT) GetStockObject(SYSTEM_FONT));
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
		PaintRoutine(hdc,cxPage,cyPage);
		if (EndPage(hdc) > 0)
		{
			bSuccess = true;
			EndDoc(hdc);
			return true;
		}
	}
	return false;
}