#include "main.hpp"



BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
	)

{
	if (fdwReason == DLL_PROCESS_ATTACH)
		if (CreateThread(NULL, 0x0, (LPTHREAD_START_ROUTINE)SamurayX::Init, (PVOID)NULL, 0x0, NULL))
			return TRUE;

	return FALSE;
}


void SX_DebugMsgBox(const char* format, ...)
{
	va_list a;

	va_start(a, format);
	int len = vprintf_s(format, a) + 1;
	va_end(a);

	char* buff = new char[len];

	memset(buff, 0x0, len);
	va_start(a, format);
	vsprintf_s(buff, len, format, a);
	va_end(a);


	MessageBoxA(NULL, buff, "Debug:", MB_OK | MB_ICONASTERISK);


	delete[] buff;

}