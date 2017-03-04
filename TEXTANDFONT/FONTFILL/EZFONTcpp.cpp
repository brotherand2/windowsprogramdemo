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
	SIZE size;
	static TCHAR szBuffer[128] = TEXT("Filling");
	SelectObject(hdc, EZCreateFont(hdc, TEXT("Times New Roman"), 0, 1440, true));
	GetTextExtentPoint32(hdc, szBuffer, lstrlen(szBuffer), &size);
	SetBkMode(hdc, TRANSPARENT);
	BeginPath(hdc);
	TextOut(hdc, (cxArea - size.cx) / 2, (cyArea - size.cy) / 2, szBuffer, lstrlen(szBuffer));
	EndPath(hdc);
	SelectObject(hdc, CreateHatchBrush(HS_DIAGCROSS,RGB(255, 0, 0)));
	SetBkMode(hdc, OPAQUE);
	SetBkColor(hdc,RGB(0,0,255) );
	StrokeAndFillPath(hdc);
	DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
	DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
}

bool PrintText(HDC hdc)
{
	int cxPage, cyPage;
	bool bSuccess = false;
	cxPage = GetDeviceCaps(hdc,PHYSICALWIDTH);
	cyPage = GetDeviceCaps(hdc,PHYSICALHEIGHT);
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