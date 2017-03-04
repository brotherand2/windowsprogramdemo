#include<iostream>
#include<Windows.h>
#include<fstream>
using namespace std;
ofstream out("font.txt");

int _stdcall EnumFontCallBack(const LOGFONT *lf, const TEXTMETRIC *tm, DWORD fontType, LPARAM lParam)
{
	int *count = (int*)lParam;
	if (fontType&RASTER_FONTTYPE)
	{
		out << "��դ����\t";
		count[0]++;

	}
	else
		if (fontType&TRUETYPE_FONTTYPE)
		{
		out << "TrueType����\t";
			count[1]++;

		}
		else
		{
			out << "ʸ������\t";
			count[2]++;

		}
	int italic = lf->lfItalic;
	out << lf->lfFaceName << "  ,Italic=" <<italic << endl;
	cout << lf->lfFaceName << "  ,Italic=" <<italic << endl;

	return true;
}
void ListFont(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);
	static int count[3];
	//EnumFontFamilies(hdc, 0, EnumFontCallBack, (long)count);

	EnumFontFamiliesEx(hdc, 0, EnumFontCallBack, (long)count,0);
	out << "��դ����:" << count[0] << endl;
	out << "TrueType����:" << count[1] << endl;
	out << "ʸ������:" << count[2] << endl;
	out.close();

	ReleaseDC(hwnd, hdc);
}
int main()
{
	ListFont(0);
	system("pause");
	return 0;
}