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
	static TCHAR szBuffer[128] = TEXT("Clipping");
	SelectObject(hdc, EZCreateFont(hdc, TEXT("Times New Roman"), 0, 1440, true));
	POINT pt[4];
	GetTextExtentPoint32(hdc, szBuffer, lstrlen(szBuffer), &size);
	//SetBkMode(hdc, TRANSPARENT);
	BeginPath(hdc);
	TextOut(hdc, (cxArea - size.cx) / 2, (cyArea - size.cy) / 2, szBuffer, lstrlen(szBuffer));
	EndPath(hdc);
	int iOffset = (cxArea + cyArea) / 4;
	srand(GetTickCount());
	SelectClipPath(hdc, RGN_COPY);
	for (int i = -iOffset; i < cyArea + iOffset; i++)
	{
		pt[0].x = 0;
		pt[1].x = cxArea / 3;
		pt[2].x = cxArea * 2 / 3;
		pt[3].x = cxArea;
		pt[0].y = i;
		pt[1].y = i + iOffset;
		pt[2].y = i - iOffset;
		pt[3].y = i;
		SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(rand() % 256, rand() % 256, rand() % 256)));
		PolyBezier(hdc, pt, 4);
		DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
	}
	DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
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