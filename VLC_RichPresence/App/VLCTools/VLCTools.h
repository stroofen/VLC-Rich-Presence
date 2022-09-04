#ifndef VLCTOOLS_H
#define VLCTOOLS_H

#include "../../Global.h"

#ifdef SYSTEM_WINDOWS
#pragma once
#endif

#include "../Timer.h"

#if defined(SYSTEM_WINDOWS)
#include <windows.h>
#elif defined(SYSTEM_GAY)

#endif

struct VLCData {
#if defined(SYSTEM_WINDOWS)
	//HANDLE handle = nullptr;
	u32 pid = 0u;
	Timer runTime = {};
	HWND hwnd = nullptr;

#elif defined(SYSTEM_GAY)

#endif
};

class VLCTools {
public:
	// Initialize everything.
	bool init();

	// Main program function.
	void run();

	void shutdown();
	bool isVLCRunning() const;
	i64 getStartTimestamp() const;

private:
	// Look for VLC Media Player process.
	bool scan();

	// Clear pVLC.
	void clear();

	// Is VLC Media Player running right now?
	bool isRunning = false;
	// VLC
	VLCData* pVLC = nullptr;
	// Tracker checking how long it was since we scanned for VLC.
	Timer scanTimer = {};
	// Update timer.
	Timer updateTimer = {};

	i64 start;
};

#endif