#include<Windows.h>

void DrawRuler(HDC hdc, int cx, int cy)
{
	static LOGFONT lf;
	SelectObject(hdc, CreatePen(PS_SOLID, cx / 72 / 6, 0));
	Rectangle(hdc, 0, 0, cx + 1, cy + 1);
	int y = 0;
	for (int i = 1; i < 96; i++)
	{
		if (i % 16 == 0)
			y = cy / 2;
		else
			if (i % 8 == 0)
				y = cy / 3;
			else
				if (i % 4 == 0)
					y = cy / 5;
				else
					if (i % 2 == 0)
						y = cy / 8;
					else
						y = cy / 12;
		MoveToEx(hdc, i*cx / 96, cy, 0);
		LineTo(hdc, i*cx / 96, cy - y);
	}
	lf.lfHeight = cy / 2;
	lstrcpy(lf.lfFaceName, TEXT("Times New Roman"));
	SelectObject(hdc, CreateFontIndirect(&lf));
	SetBkMode(hdc,TRANSPARENT);
	SetTextAlign(hdc, TA_BOTTOM | TA_CENTER);
	for (int i = 1; i < 6; i++)
	{
		TCHAR ch = (TCHAR)'0' + i;
		TextOut(hdc, i*cx / 6, cy / 2, &ch, 1);
	}
	DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
	DeleteObject( SelectObject(hdc, GetStockObject(BLACK_PEN)));
}
void CreateRoutine()
{
	HENHMETAFILE hemf;
	HDC hdcEmf;
	hdcEmf = CreateEnhMetaFile(0, TEXT("emf12.emf"), 0, TEXT("EMF12\0EMF DEMO #12\0"));
	if (!hdcEmf)
		return;
	int cxDpi = GetDeviceCaps(hdcEmf, HORZRES) * 254 / GetDeviceCaps(hdcEmf, HORZSIZE) / 10;
	int cyDpi = GetDeviceCaps(hdcEmf, VERTRES) * 254 / GetDeviceCaps(hdcEmf, VERTSIZE) / 10;
	DrawRuler(hdcEmf, cxDpi * 6, cyDpi);
	hemf=CloseEnhMetaFile(hdcEmf);
	DeleteEnhMetaFile(hemf);
}
void PaintRoutine(HDC hdc,int cxArea,int cyArea)
{
	HENHMETAFILE hemf;
	hemf = GetEnhMetaFile(TEXT("emf12.emf"));
	if (!hemf)
		return;
	ENHMETAHEADER emh;
	GetEnhMetaFileHeader(hemf, sizeof(emh), &emh);
	int cxImage = emh.rclFrame.right - emh.rclFrame.left;
	int cyImage = emh.rclFrame.bottom - emh.rclFrame.top;
	SetMapMode(hdc, MM_HIMETRIC);
	SetViewportOrgEx(hdc, 0, cyArea, 0);
	POINT pt{ cxArea, 0 };
	DPtoLP(hdc, &pt, 1);
	RECT rect;
	rect.left = (pt.x - cxImage) / 2;
	rect.right = (pt.x + cxImage) / 2;
	rect.top = (pt.y + cyImage) / 2;
	rect.bottom = (pt.y - cyImage) / 2;
	PlayEnhMetaFile(hdc, hemf, &rect);
	DeleteEnhMetaFile(hemf);
}
void PrintRoutine(HWND hwnd, TCHAR *name)
{
	static DOCINFO di{ sizeof(di), name };
	static PRINTDLG pd{ sizeof(pd) };
	HDC hdcPrn;
	pd.Flags = PD_RETURNDC;
	if (!PrintDlg(&pd))
		return;
	hdcPrn = pd.hDC;
	if (!hdcPrn)
		return;
	int cxPage = GetDeviceCaps(hdcPrn, HORZRES);
	int cyPage = GetDeviceCaps(hdcPrn, VERTRES);
	bool bSuccess = false;
	if (StartDoc(hdcPrn, &di) > 0 && StartPage(hdcPrn) > 0)
	{
		PaintRoutine(hdcPrn, cxPage, cyPage);
		if (EndPage(hdcPrn) > 0)
		{
			bSuccess = true;
			EndDoc(hdcPrn);
		}
	}
	if (!bSuccess)
		MessageBox(hwnd, TEXT("¥Ú”° ß∞‹"), 0, 0);
}