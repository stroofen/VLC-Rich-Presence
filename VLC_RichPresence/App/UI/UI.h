#ifndef UI_H
#define UI_H

#include "../../Global.h"

#ifdef SYSTEM_WINDOWS
#pragma once
#endif

#include <d3d9.h>
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

enum class TextPosition {
	CURSOR,
	CENTER,
	CENTER_X,
	CENTER_Y
};

enum class TextMode {
	DEFAULT,
	CENTER,
	CENTER_X,
	CENTER_Y
};

class UI {
public:
	// Initialize everything
	bool init();

	// Main program function
	void run(bool* pShouldExit);

	void shutdown();

private:
	void preRender(bool* pShouldExit);
	void postRender();

	enum Font {
		FONT_IMGUI,		// Default ImGui font
		FONT_LARGE,		// Large font
		FONT_MEDIUM,	// Medium font
		FONT_SMALL,		// Normal font
		FONT_MAX		// End of enum
	};

	void text(const char* fmt, ...);
	void text(const ImVec4& color, const char* fmt, ...);
	void text(const ImVec4& color, TextPosition position, TextMode mode, const char* fmt, ...);

	__inline void setFont(Font font) {
		ImGui::SetCurrentFont(pFonts[static_cast<u32>(font)]);
	}

	ImVec4 backgroundColor = ImVec4(0.068f, 0.068f, 0.068f, 1.f);
	ImVec4 themeColor = ImVec4(RGBA(252, 165, 42, 255));


	HWND hwnd = nullptr;
	ImFont* pFonts[FONT_MAX];
};

extern LPDIRECT3D9             g_pD3D;
extern LPDIRECT3DDEVICE9       g_pD3DDevice;
extern D3DPRESENT_PARAMETERS   g_D3Dpp;

#endif