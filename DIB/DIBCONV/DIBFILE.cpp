#include<Windows.h>
#include<tchar.h>
#include<stdio.h>
void Printf(HWND hwnd, TCHAR *szFormat, ...)
{
	TCHAR szBuffer[1024];
	va_list pArgList;
	va_start(pArgList, szFormat);
	_vsntprintf_s(szBuffer, 1024, szFormat, pArgList);
	va_end(pArgList);
	SendMessage(hwnd, EM_SETSEL, -2, -1);//将光标移到文本最后
	SendMessage(hwnd, EM_REPLACESEL, false, (long)szBuffer);//在文本后添加字符串
	SendMessage(hwnd, EM_SCROLLCARET, 0, 0);//滚动到光标处
}
bool DibWriteImage(TCHAR *szPath, BITMAPFILEHEADER *pbmfh)
{
	HANDLE hFile = CreateFile(szPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;
	unsigned long dwByteWrite;
	WriteFile(hFile, pbmfh, pbmfh->bfSize, &dwByteWrite, 0);
	CloseHandle(hFile);
	if (pbmfh->bfSize != dwByteWrite)
	{
		DeleteFile(szPath);
		return false;
	}
	return true;
}
BITMAPFILEHEADER *DibLoadImage(HWND hwnd, TCHAR *szPath)//读取图片内容
{
	BITMAPFILEHEADER *pbmfh;
	HANDLE hFile;
	hFile = CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	unsigned long dwFileSize, dwHighSize, dwBytesRead;
	dwFileSize = GetFileSize(hFile, &dwHighSize);
	if (dwHighSize)
	{
		CloseHandle(hFile);
		return 0;
	}
	pbmfh = (BITMAPFILEHEADER*)malloc(dwFileSize);
	if (!pbmfh)
	{
		CloseHandle(hFile);
		return 0;
	}
	bool success= ReadFile(hFile, pbmfh, dwFileSize, &dwBytesRead, 0);
	if (!success || dwFileSize != dwBytesRead || pbmfh->bfSize != dwFileSize || pbmfh->bfType != *(WORD*)TEXT("BM"))
	{
		CloseHandle(hFile);
		free(pbmfh);
		return 0;
	}
	CloseHandle(hFile);
	return pbmfh;
}
void DibFileInitialize(HWND hwnd, OPENFILENAME &ofn)//初始化打开对话框结构
{
	static TCHAR szFilter[MAX_PATH] = TEXT("Bitmap File(*.bmp)\0*.bmp\0")
		TEXT("All Files(*.*)\0*.*\0\0");
	memset(&ofn, 0, sizeof(ofn));
	ofn.hwndOwner = hwnd;
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = szFilter;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.nMaxFile = MAX_PATH;
}
