#include<cstdio>
#include<cstdlib>
#include<clocale>
#include<cwchar>
#include<Windows.h>
#include<tchar.h>
#include<strsafe.h>
#include<tchar.h>
#include<stdarg.h>
int my_printf(char *szFormat, ...)
{
	va_list pArgs;
	va_start(pArgs, szFormat);
	int i=vprintf(szFormat, pArgs);
	va_end(pArgs);
	return i;
}
void my_sprintf(char buffer[], char *szFormat, ...)
{
	va_list ap;
	va_start(ap, szFormat);
	vsprintf(buffer, szFormat, ap);
	va_end(ap);
}
//char buffer[80];
//int vspf(char *fmt, ...)
//{
//	va_list argptr;
//	int cnt;
//	va_start(argptr, fmt);
//	cnt = vsprintf(buffer, fmt, argptr);
//	va_end(argptr);
//	return(cnt);
//}
void MessageBoxPrintf(char s1[], char format[], ...)
{
	char buffer[128];
	va_list ap;
	va_start(ap, format);
	vsprintf(buffer, format, ap);
	MessageBox(nullptr,  buffer,s1, MB_OK);
	va_end(ap);
}
int main()
{
	char buffers[128];
	my_sprintf(buffers, "%d %s %f", 1, "hello", 3.14159);
	printf("buffer:%s", buffers);
	my_printf("hello %d %f", 1, 2.34);
	setlocale(LC_ALL, "chs");
	MessageBoxPrintf("nihao", "%d %s %f %c", 1, "hello", 3.14159,'a');
	//char c = '中';
	//printf("%c\n", c);
	//wchar_t ch = L'国';
	//wprintf(L"%c\n", ch);
	//wchar_t str[] = L"中国";
	//int i = wcslen(str);
	//wprintf(L"length:%d\n", i);
	//TCHAR str1[] = TEXT("中国人");
	//_tprintf(TEXT("%s"), str1);
		TCHAR buffer[10];
		// int i = sprintf(buffer, "abcdefghija");
	    int i=	_snprintf(buffer, 8, "nihao woshiaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	  _tprintf(TEXT("leng=%d,%s\n"), i,buffer);
	getchar();
}
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdCommand, int nCmdShow)
//{
//	TCHAR buffer[10];
//	_sntprintf_s(buffer,9, TEXT("niaho woshi%d"), 11);
//	_sntprintf_s(buffer, 10, TEXT("niaho woshi%d"), 11);
//	StringCchPrintf(buffer, 10,TEXT( "nihao woshi%daaaaaaaaaaaaaaaaaaaaaaaaaaa"), 11);
//	_snprintf(buffer, 12, "nihao woshi%daaaaaaaaaaaaaaaaaaaaaaaaaaa", 11);
//	_snprintf_s(buffer, 9, "nihao woshi%d", 11);
//	_snwprintf(buffer, 10, L"nihao woshi%d", 11);
//	sprintf(buffer, "nihao woshi%d", 11);
//	swprintf(buffer,10, L"nihao woshi%d", 11);
//	MessageBox(nullptr, buffer, TEXT("nihao"),MB_OK|MB_ICONERROR);
//}
