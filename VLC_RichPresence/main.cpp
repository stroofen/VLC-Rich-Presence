#include "App/App.h"

#ifdef SYSTEM_WINDOWS
#include <windows.h>
#elif defined(SYSTEM_GAY)

#endif

int main() {
#if !defined(_DEBUG)
#if defined(SYSTEM_WINDOWS)
	// Windows
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#elif defined(SYSTEM_GAY)
	// Linux
#endif
#else
#if defined(SYSTEM_WINDOWS)
	// Windows
	SetConsoleTitleW(g_VersionW);
#elif defined(SYSTEM_GAY)
	// Linux

#endif
#endif

	App app;
	if (app.init()) {
		app.run();
	}

	return 0;
}