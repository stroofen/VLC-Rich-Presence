#include "VLCTools.h"
#include "../Error.h"
#include <string>

#if defined(SYSTEM_WINDOWS)
#include <TlHelp32.h>
#include <ProcessSnapshot.h>
#include <Psapi.h>
#ifdef _DEBUG
#include <iostream>
#endif
#elif defined(SYSTEM_GAY)

#endif

// Initialize everything
bool VLCTools::init() {
	// Check for difference.
	if (scan()) {
		isRunning = true;
		if (!isRunning) {
			if (pVLC) {
				delete pVLC;
				pVLC = nullptr;
			}
		} else {
			start = start = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();;
			pVLC->runTime.reset();
		}
	}

	return true;
}

// Main program function
void VLCTools::run() {
	// We should scan for the timer
	if (scanTimer.hasElapsed(g_ScanInterval)) {
		const bool wasRunning = isRunning;
		isRunning = scan();

		if (!wasRunning && isRunning) {
			start = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			g_ForceUpdate = true;
		} else if (wasRunning && !isRunning) {
			strcpy_s(g_VLCTitle, "Shutting down...");
			g_ForceUpdate = true;
		}

		scanTimer.reset();
	}

	if (isRunning) {
		if (updateTimer.hasElapsed(g_UpdateInterval)) {
			GetWindowTextA(pVLC->hwnd, g_VLCTitle, sizeof(g_VLCTitle));
			if (strcmp(g_VLCTitle, g_DefaultVLCTitle) != 0 && g_VLCTitle[0u] != '\0') {
				std::string str(g_VLCTitle);
				const usize idx = str.find_last_of('.');
				str.erase(idx, 0xFFFFFFFF);
				strcpy_s(g_VLCTitle, str.c_str());
			} else {
				strcpy_s(g_VLCTitle, g_DefaultVLCTitle);
			}
			updateTimer.reset();
		}
	}
}

void VLCTools::shutdown() {
	clear();
}

bool VLCTools::isVLCRunning() const {
	return isRunning && pVLC;
}

i64 VLCTools::getStartTimestamp() const {
	if (isRunning && pVLC) {
		return start;
	}
	return 0;
}

// Look for VLC Media Player process.
bool VLCTools::scan() {
	bool found = false;

	/*
	"Why don't you just find VLC's WndProc using GetWindowLongPointer and replace it and use it as a hook?"
	GREAT QUESTION! As it turns out, VLC does not have a WndProc function (I found out by calling GetWindowLongPointer
	and got a nullptr as a return value). I don't know what arcane magic VideoLAN uses, but they don't have a WndProc,
	the IDA output doesn't include 80% of the strings the source code has, however-the-fuck they even managed to do that
	(either way it means that I can't find any functions to sigscan for + hook).
	These fucking people have made an impenetrable executable without even knowing it. Unironically more secure than
	VAC could ever be.
	*/
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0u);

	if (snapshot == INVALID_HANDLE_VALUE) {
		Error(L"CreateToolhelp32Snapshot() return value was INVALID_HANDLE_VALUE.").show();
		return false;
	}

	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(PROCESSENTRY32W);

	bool res = Process32FirstW(snapshot, &entry);
	if (!res) {
		Error(L"Process32FirstW() failed.").show();
		return false;
	}

	char titlebuf[256u] = "";

	while (res = Process32NextW(snapshot, &entry)) {
		if (!wcscmp(entry.szExeFile, L"vlc.exe")) {
			// Get handle to process
			if (HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, entry.th32ProcessID)) {
				// Get path of process
				wchar_t buf[MAX_PATH] = L"";
				if (GetModuleFileNameExW(handle, 0, buf, MAX_PATH)) {
					// It matches the VLC path.
					if (!wcscmp(buf, g_VLCDefaultPath)) {
						if (!pVLC) {
							// Get HWND
							HWND vlcHWND = nullptr;
							HWND curHWND = nullptr;
							do {
								curHWND = FindWindowEx(0, curHWND, 0, 0);
								u32 pid = 0u;
								GetWindowThreadProcessId(curHWND, reinterpret_cast<DWORD*>(&pid));
								if (pid == entry.th32ProcessID) {

									GetWindowText(curHWND, titlebuf, sizeof(titlebuf));
									std::string_view s(titlebuf);
									if (s.ends_with("VLC media player")) {
										vlcHWND = curHWND;
										break;
									}
								}
							} while (curHWND != nullptr);

							// If we got a HWND
							if (vlcHWND != nullptr) {
								// Create new VLCData object
								pVLC = new VLCData();
								// Set PID
								pVLC->pid = entry.th32ProcessID;
								// Set HWND
								pVLC->hwnd = vlcHWND;
								found = true;
							}
						} else {
							found = true;
						}
						CloseHandle(handle);
						break;
					}
				}
				CloseHandle(handle);
			}
		}
	}

	if (!found) {
		if (pVLC) {
			delete pVLC;
			pVLC = nullptr;
		}
	}

	if (!CloseHandle(snapshot)) {
		Error(L"CloseHandle() failed.").show();
		return false;
	}

	return found;
}

void VLCTools::clear() {
	if (pVLC) {
		delete pVLC;
		pVLC = nullptr;
	}
}