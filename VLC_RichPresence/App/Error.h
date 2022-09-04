#ifndef ERROR_H
#define ERROR_H

#include "../Global.h"

#ifdef OS_WINDOWS
#pragma once
#endif

enum class ErrorType {
	GENERIC
};

class Error {
public:
	Error(const wchar_t* fmt, ...);
	void show();

private:
	const wchar_t* title = g_VersionW;
	wchar_t text[g_MaxErrorTextLength] = L"";
};

#endif