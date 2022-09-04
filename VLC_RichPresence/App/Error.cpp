#include "Error.h"
#include <string>
#include <stdarg.h>

#ifdef SYSTEM_WINDOWS
#include <windows.h>
#elif defined(SYSTEM_GAY)

#endif

Error::Error(const wchar_t* fmt, ...) {
	va_list vaP;
	va_start(vaP, fmt);
	size_t len = vswprintf(text, sizeof(text), fmt, vaP);
	va_end(vaP);
}

void Error::show() {
#ifdef SYSTEM_WINDOWS
	MessageBoxW(nullptr, text, title, MB_ICONERROR);
#elif defined(SYSTEM_GAY)

#endif
}