#ifdef __cplusplus
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

EXPORT void EdrCenterTextA(HDC,const RECT &rect, char*);
EXPORT void EdrCenterTextW(HDC,const RECT&, wchar_t*);

#ifdef UNICODE
# define EdrCenterText EdrCenterTextW
#else
# define EdrCenterText EdrCenterTextA
#endif
