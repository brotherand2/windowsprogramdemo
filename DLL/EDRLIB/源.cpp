#include<Windows.h>
#include"edrlib.h"
int _stdcall DllMain(HINSTANCE hInstance, unsigned long fdwReason, void *pvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		MessageBox(0, TEXT("Dll ������"), TEXT("DLL "), 0);
		break;
	case DLL_PROCESS_DETACH:
		MessageBox(0, TEXT("���������ͷ�DLL"), TEXT("DLL "), 0);
		break;

	case DLL_THREAD_ATTACH:
		MessageBox(0, TEXT("�½��߳�"), TEXT("DLL "), 0);
		break;

	case DLL_THREAD_DETACH:
		MessageBox(0, TEXT("�߳��˳�"), TEXT("DLL "), 0);
		break;

	default:
		break;
	}
	return true;
}
EXPORT void EdrCenterTextA(HDC hdc,const RECT &rect, char *str)
{
	int length;
	SIZE size;
	GetTextExtentPoint32A(hdc, str, lstrlenA(str), &size);
	TextOutA(hdc, (rect.right - size.cx-rect.left) / 2, (rect.bottom - size.cy-rect.top) / 2, str, lstrlenA(str));
}
EXPORT void EdrCenterTextW(HDC hdc, const RECT &rect, wchar_t *str)
{
	int length;
	SIZE size;
	GetTextExtentPoint32W(hdc, str, lstrlenW(str), &size);
	TextOutW(hdc, (rect.right - size.cx - rect.left) / 2, (rect.bottom - size.cy - rect.top) / 2, str, lstrlenW(str));
}
EXPORT void DemoA()
{
	MessageBoxA(0, "���Ƕ�̬���ӿ⺯��", "DLL", 0);
}
EXPORT void DemoW()
{
	MessageBoxW(0, L"���Ƕ�̬���ӿ⺯��", L"DLL", 0);
}