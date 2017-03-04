#include<Windows.h>
#include"strlib.h"
#pragma data_seg("shared")
int iTotal = 0;
char szStrings[MAX_STRINGS][MAX_LENGTH+1] = { '\0' };
#pragma data_seg()
#pragma comment(linker,"/SECTION:shared,RWS")
int _stdcall DllMain(HINSTANCE hInstance, unsigned long dwReason, void *reserved)
{
	return TRUE;
}

EXPORT BOOL AddStringW(wchar_t *str)
{
	int length = WideCharToMultiByte(CP_ACP, 0, str, -1, 0, 0, 0, 0);
	char *temp = (char*)malloc(length*sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, str, -1, temp, length, 0, 0);
	BOOL bReturn = AddStringA(temp);
	free(temp);
	return bReturn;
}
EXPORT BOOL AddStringA(char *str)
{
	if (iTotal == MAX_STRINGS - 1)
		return FALSE;
	int length = strlen(str);
	if ( length== 0)
		return FALSE;
	char *temp = (char*)malloc(length+ 1);
	strcpy(temp, str);
	strupr(str);
	int i;
	for (i = iTotal; i >0; i--)
	{
		if (strcmp(str, szStrings[i - 1]) > 0)
			break;
		strcpy(szStrings[i], szStrings[i - 1]);
	}
	strcpy(szStrings[i], str);
	iTotal++;
	free(temp);
	return TRUE;
}
EXPORT BOOL DeleteStringW(wchar_t *str)
{
	int length = WideCharToMultiByte(CP_ACP, 0, str, -1, 0, 0, 0, 0);
	char *temp = (char*)malloc(length*sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, str, -1, temp, length, 0, 0);
	BOOL bReturn = DeleteStringA(temp);
	free(temp);
	return bReturn;
}
EXPORT BOOL DeleteStringA(char *str)
{
	if (iTotal == 0)
		return FALSE;
	if (strlen(str) == 0)
		return FALSE;
	int i;
	for (i = 0; i < iTotal; i++)
	{
		if (stricmp(str, szStrings[i]) == 0)
			break;
	}
	if (i >= iTotal)
		return FALSE;
	for (int j = i; j < iTotal; j++)
		strcpy(szStrings[j], szStrings[j + 1]);
	szStrings[iTotal--][0] = '\0';
	return TRUE;
}
EXPORT void GetStringW(GETSTRCB fun, void *param)
{
	for (int i = 0; i < iTotal; i++)
	{
		int length = MultiByteToWideChar(CP_ACP, 0, szStrings[i], -1, 0, 0);
		wchar_t *temp = (wchar_t*)malloc(length*sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, szStrings[i], -1, temp, length);
		int iReturn = fun(temp, param);
		free(temp);
		if (!iReturn)
			return;
	}
}
EXPORT  void GetStringA(GETSTRCB fun, void *param)
{
	for (int i = 0; i < iTotal; i++)
	{
		int iReturn = fun(szStrings[i], param);
		if (!iReturn)
			return;
	}
}


