#ifdef __cplusplus
# define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif
#define MAX_LENGTH 63
#define MAX_STRINGS 256

typedef BOOL(CALLBACK * GETSTRCB) (TCHAR*, PVOID);

EXPORT BOOL AddStringW(wchar_t *str);
EXPORT BOOL AddStringA(char *str);
EXPORT BOOL DeleteStringW(wchar_t *str);
EXPORT BOOL DeleteStringA(char *str);
EXPORT void GetStringW(GETSTRCB fun, void *param);
EXPORT void GetStringA(GETSTRCB fun, void *param);

#ifdef UNICODE
#define AddString AddStringW
#define DeleteString DeleteStringW
#define GetString GetStringW
#else
#define AddString AddStringA
#define DeleteString DeleteStringA
#define GetString GetStringA
#endif
