void Printf(HWND hwnd, TCHAR *szFormat, ...);
BITMAPFILEHEADER * DibLoadImage(HWND hwnd, TCHAR *szPath);//��ȡͼƬ����
void DibFileInitialize(HWND hwnd, OPENFILENAME &ofn);//��ʼ���򿪶Ի���ṹ
bool DibWriteImage(TCHAR *szPath, BITMAPFILEHEADER *pbmfh);
