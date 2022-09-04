#include "UI.h"
#include "../Error.h"

#pragma region Macro

#define LIGHT_GREY ImVec4(0.88f, 0.88f, 0.88f, 1.f)

#define WORK_AREA ImGui::GetContentRegionAvail()
#define WORK_AREA_X ImGui::GetContentRegionAvail().x
#define WORK_AREA_Y ImGui::GetContentRegionAvail().y

#define SPACING ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15.f)

#pragma endregion

#pragma region ImGui

LPDIRECT3D9             g_pD3D = 0;
LPDIRECT3DDEVICE9       g_pD3DDevice = 0;
D3DPRESENT_PARAMETERS   g_D3Dpp = {};

bool createDeviceD3D(HWND hWnd);
void cleanupDeviceD3D();
void resetDevice();
LRESULT WINAPI wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static const ImVec4 clear = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static const ImVec4 titleColor = ImVec4(0.88f, 0.88f, 0.88f, 1.f);
static const D3DCOLOR dxClear = D3DCOLOR_RGBA((i32)(clear.x * clear.w * 255.0f), (i32)(clear.y * clear.w * 255.0f), (i32)(clear.z * clear.w * 255.0f), (i32)(clear.w * 255.0f));

#pragma endregion

#pragma region ImVec

// IMVEC2

ImVec2 plus(const ImVec2& base, const ImVec2& other) {
	return ImVec2{ base.x + other.x, base.y + other.y };
}

ImVec2 minus(const ImVec2& base, const ImVec2& other) {
	return ImVec2{ base.x - other.x, base.y - other.y };
}

ImVec2 mult(const ImVec2& base, const ImVec2& other) {
	return ImVec2{ base.x * other.x, base.y * other.y };
}

ImVec2 div(const ImVec2& base, const ImVec2& other) {
	return ImVec2{ base.x / other.x, base.y / other.y };
}

ImVec2 plus(const ImVec2& base, f32 other) {
	return ImVec2{ base.x + other, base.y + other };
}

ImVec2 minus(const ImVec2& base, f32 other) {
	return ImVec2{ base.x - other, base.y - other };
}

ImVec2 mult(const ImVec2& base, f32 other) {
	return ImVec2{ base.x * other, base.y * other };
}

ImVec2 div(const ImVec2& base, f32 other) {
	return ImVec2{ base.x / other, base.y / other };
}


// IMVEC4

ImVec4 div(const ImVec4& base, f32 other) {
	return ImVec4{ base.x / other, base.y / other, base.z / other, base.w / other };
}

#pragma endregion

// Initialize everything
bool UI::init() {
	i32 screenX = GetSystemMetrics(SM_CXSCREEN);
	i32 screenY = GetSystemMetrics(SM_CYSCREEN);

	// Create application window
	//ImGui_ImplWin32_EnableDpiAwareness();
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, wndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, g_VersionA, NULL };
	::RegisterClassEx(&wc);
	hwnd = ::CreateWindow(wc.lpszClassName,
		g_VersionA,
		WS_EX_TOPMOST | WS_POPUP,
		screenX / 2 - g_GUIWidth / 2,
		screenY / 2 - g_GUIHeight / 2,
		g_GUIWidth,
		g_GUIHeight,
		NULL,
		NULL,
		wc.hInstance,
		NULL);

	// Initialize Direct3D
	if (!createDeviceD3D(hwnd)) {
		cleanupDeviceD3D();
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		Error(L"createDeviceD3D() failed.\nLine: %i\nFile: %s", __LINE__, __FILE__).show();
		return false;
	}

	// Show the window
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	// Setup ImGui stuff
	ImGui::CreateContext();
	if (!ImGui_ImplWin32_Init(hwnd)) {
		Error(L"ImGui_ImplWin32_Init() failed.\nLine: %i\nFile: %s", __LINE__, __FILE__).show();
		return false;
	}
	if (!ImGui_ImplDX9_Init(g_pD3DDevice)) {
		Error(L"ImGui_ImplDX9_Init() failed.\nLine: %i\nFile: %s", __LINE__, __FILE__).show();
		return false;
	}

	// ImGui visual style
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding = ImVec2(10, 10);
		style.WindowRounding = 0.0f;
		style.FramePadding = ImVec2(5, 5);
		style.FrameRounding = 0.0f;
		style.ItemSpacing = ImVec2(5, 4);
		style.ItemInnerSpacing = ImVec2(8, 6);
		style.IndentSpacing = 25.0f;
		style.ScrollbarSize = 10.0f;
		style.ScrollbarRounding = 7.0f;
		style.GrabMinSize = 5.0f;
		style.GrabRounding = 3.0f;

		//Text
		style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f); //???

		//Background of window
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.068f, 0.068f, 0.068f, 1.f);
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.001f, 0.001f, 0.001f, 1.00f);


		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);


		style.Colors[ImGuiCol_Border] = ImVec4(0.15f, 0.15f, 0.15f, 1.f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);

		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.039f, 0.039f, 0.039f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);


		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.002f, 0.002f, 0.002f, 0.95f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.015f, 0.015f, 0.015f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.01f, 0.01f, 0.01f, 1.00f);

		//Background for drop-down menus
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);

		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.032f, 0.032f, 0.032f, 0.85f);

		//Scrollbar (Vectical)
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);

		//Checkmark (for checkbox & radiobutton)
		style.Colors[ImGuiCol_CheckMark] = ImVec4(1.f, 0.05f, 0.05f, 0.81f);

		//Slider
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);

		//Button
		style.Colors[ImGuiCol_Button] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);

		//Drop-down menu item color
		style.Colors[ImGuiCol_Header] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);

		//Separator
		//style.Colors[ImGuiCol_Separator] = ImVec4(0.180f, 0.529f, 0.125f, 0.73f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.90f, 0.90f, 0.90f, 0.73f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.207f, 0.611f, 0.145f, 0.73f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.180f, 0.529f, 0.125f, 0.73f);

		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
	}

	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig config;
	//Fonts
	pFonts[FONT_IMGUI] = io.Fonts->AddFontDefault();
	pFonts[FONT_LARGE] = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\ariblk.ttf", 34.f, &config);
	pFonts[FONT_MEDIUM] = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 20.f, &config);
	pFonts[FONT_SMALL] = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 16.f, &config);
	// Prevent "imgui.ini" file from being created
	io.IniFilename = nullptr;

	return true;
}

// Main program function
void UI::run(bool* pShouldExit) {
	preRender(pShouldExit);

	ImGui::Begin(g_VersionA, nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	{
		// Titlebar
		{
			ImGuiStyle& style = ImGui::GetStyle();
			static const auto oldChildBg = style.Colors[ImGuiCol_ChildBg];
			static const auto oldWindowPadding = style.WindowPadding;
			static const auto oldFramePadding = style.FramePadding;
			static const auto oldItemSpacing = style.ItemSpacing;
			static const auto oldItemInnerSpacing = style.ItemInnerSpacing;
			static const auto oldIndentSpacing = style.IndentSpacing;
			static const auto oldButtonColor = style.Colors[ImGuiCol_Button];
			static const auto oldButtonHoverColor = style.Colors[ImGuiCol_ButtonHovered];
			static const auto oldButtonActiveColor = style.Colors[ImGuiCol_ButtonActive];

			style.Colors[ImGuiCol_ChildBg] = style.Colors[ImGuiCol_WindowBg];
			style.WindowPadding = ImVec2(0.f, 0.f);
			style.FramePadding = ImVec2(0.f, 0.f);
			style.ItemSpacing = ImVec2(0.f, 0.f);
			style.ItemInnerSpacing = ImVec2(0.f, 0.f);
			style.IndentSpacing = 0.f;

			ImVec2 wa = WORK_AREA;
			ImGui::BeginChild("##TITLEBAR", ImVec2(wa.x * 0.85f, wa.y * 0.09f));
			{
				setFont(UI::Font::FONT_LARGE);
				text(LIGHT_GREY, TextPosition::CENTER, TextMode::CENTER, g_VersionA);
			}
			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild("##BUTTONPANEL", ImVec2(-1, wa.y * 0.09f), true);
			{
				wa = WORK_AREA;
				// Minimize button
				if (ImGui::Button("_", ImVec2(wa.x * 0.5f, -1))) {

				}
				ImGui::SameLine();

				style.Colors[ImGuiCol_Button] = ImVec4(0.875f, 0.01f, 0.001f, 1.f);
				style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.9f, 0.015f, 0.015f, 1.f);
				style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.9f, 0.005f, 0.005f, 1.f);

				// Exit button
				if (ImGui::Button("X", ImVec2(-1, -1))) {
					*pShouldExit = true;
				}
			}
			ImGui::EndChild();

			style.Colors[ImGuiCol_ChildBg] = oldChildBg;
			style.WindowPadding = oldWindowPadding;
			style.FramePadding = oldFramePadding;
			style.ItemSpacing = oldItemSpacing;
			style.ItemInnerSpacing = oldItemInnerSpacing;
			style.IndentSpacing = oldIndentSpacing;
			style.Colors[ImGuiCol_Button] = oldButtonColor;
			style.Colors[ImGuiCol_ButtonHovered] = oldButtonHoverColor;
			style.Colors[ImGuiCol_ButtonActive] = oldButtonActiveColor;
		}
		
		// Window body
		{
			ImGui::BeginChild("##MAIN_FIELD", ImVec2{ -1.f, -1.f }, true);
			{
				ImGui::BeginChild("##LEFT", ImVec2{ WORK_AREA_X * 0.55f, -1.f });
				{
					setFont(FONT_MEDIUM);
					text(themeColor, TextPosition::CENTER_X, TextMode::CENTER_X, "Presence settings");

					SPACING;
					setFont(FONT_SMALL);

					text("VLC icon");
					if (ImGui::Combo("##VLC_ICON", &g_DiscordIcon, g_DiscordIcons, static_cast<i32>(g_DiscordIconCount))) {
						g_DiscordIconString = g_DiscordIcons[g_DiscordIcon];
					}


				}
				ImGui::EndChild();

				ImGui::SameLine();
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
				ImGui::SameLine();
				
				ImGui::BeginChild("##RIGHT", ImVec2{ -1, -1.f });
				{
					setFont(FONT_MEDIUM);
					text(themeColor, TextPosition::CENTER_X, TextMode::CENTER_X, "Settings");

					SPACING;
					SPACING;
					setFont(FONT_SMALL);

					text("Scan interval");
					ImGui::SliderFloat("##SCAN_INTERVAL", &g_ScanInterval, 0.1f, 10.f, "%.1f seconds");

					SPACING;

					text("Update interval");
					ImGui::SliderFloat("##UPDATE_INTERVAL", &g_UpdateInterval, 0.1f, 10.f, "%.1f seconds");
				}
				ImGui::EndChild();
			}
			ImGui::EndChild();
		}
	}
	ImGui::End();

	postRender();
}

void UI::shutdown() {

}

void UI::preRender(bool* pShouldExit) {
	ImGuiStyle& style = ImGui::GetStyle();

	// WinAPI stuff.
	MSG msg;
	while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		if (msg.message == WM_QUIT) {
			*pShouldExit = true;
			return;
		}
	}

	// ImGui stuff.
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Setup for frame.
	ImGui::SetNextWindowPos(ImVec2{ 0.f, 0.f });
	ImGui::SetNextWindowSize(ImVec2{ static_cast<f32>(g_GUIWidth), static_cast<f32>(g_GUIHeight) });
}

void UI::postRender() {
	// ImGui stuff.
	ImGui::EndFrame();
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pD3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, dxClear, 1.0f, 0);
	if (g_pD3DDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		g_pD3DDevice->EndScene();
	}
	HRESULT result = g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && g_pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		resetDevice();
}

void UI::text(const char* fmt, ...) {
	char buf[256u] = "";
	va_list vaP;
	va_start(vaP, fmt);
	size_t len = vsnprintf(buf, sizeof(buf), fmt, vaP);
	va_end(vaP);

	ImGui::TextColored(LIGHT_GREY, buf);
}

void UI::text(const ImVec4& color, const char* fmt, ...) {
	char buf[256u] = "";
	va_list vaP;
	va_start(vaP, fmt);
	size_t len = vsnprintf(buf, sizeof(buf), fmt, vaP);
	va_end(vaP);

	ImGui::TextColored(color, buf);
}

void UI::text(const ImVec4& color, TextPosition position, TextMode mode, const char* fmt, ...) {
	char buf[256u] = "";
	va_list vaP;
	va_start(vaP, fmt);
	size_t len = vsnprintf(buf, sizeof(buf), fmt, vaP);
	va_end(vaP);

	switch (position) {
	case TextPosition::CURSOR: {
		// Nothing
		break;
	}
	case TextPosition::CENTER: {
		// Set position cursor position to be center of work space
		ImGui::SetCursorPos(div(WORK_AREA, 2.f));
		break;
	}
	case TextPosition::CENTER_X: {
		// Set position cursor X position to be center of work space
		ImGui::SetCursorPosX(WORK_AREA_X / 2.f);
		break;
	}
	case TextPosition::CENTER_Y: {
		// Set position cursor Y position to be center of work space
		ImGui::SetCursorPosY(WORK_AREA_Y / 2.f);
		break;
	}
	}

	switch (mode) {
	case TextMode::DEFAULT: {
		// Nothing
		break;
	}
	case TextMode::CENTER: {
		const ImVec2 size = ImGui::CalcTextSize(buf);
		ImGui::SetCursorPos(minus(ImGui::GetCursorPos(), div(size, 2.f)));	// lol
		break;
	}
	case TextMode::CENTER_X: {
		const ImVec2 size = ImGui::CalcTextSize(buf);
		ImGui::SetCursorPosX(ImGui::GetCursorPos().x - (size.x / 2.f));	// lol
		break;
	}
	case TextMode::CENTER_Y: {
		const ImVec2 size = ImGui::CalcTextSize(buf);
		ImGui::SetCursorPosY(ImGui::GetCursorPos().y - (size.y / 2.f));	// lol
		break;
	}
	}

	ImGui::TextColored(color, buf);
}

#pragma region ImGui

bool createDeviceD3D(HWND hWnd) {
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;

	// Create the D3DDevice
	ZeroMemory(&g_D3Dpp, sizeof(g_D3Dpp));
	g_D3Dpp.Windowed = TRUE;
	g_D3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_D3Dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
	g_D3Dpp.EnableAutoDepthStencil = TRUE;
	g_D3Dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_D3Dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
	//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_D3Dpp, &g_pD3DDevice)))
		return false;

	return true;
}

void cleanupDeviceD3D() {
	if (g_pD3DDevice) { g_pD3DDevice->Release(); g_pD3DDevice = NULL; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void resetDevice() {
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = g_pD3DDevice->Reset(&g_D3Dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pD3DDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			g_D3Dpp.BackBufferWidth = LOWORD(lParam);
			g_D3Dpp.BackBufferHeight = HIWORD(lParam);
			resetDevice();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

#pragma endregion