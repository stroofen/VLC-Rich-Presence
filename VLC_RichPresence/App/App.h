#ifndef APP_H
#define APP_H

#include "../Global.h"

#ifdef SYSTEM_WINDOWS
#pragma once
#endif

#include "UI/UI.h"
#include "VLCTools/VLCTools.h"
#include "Discord/Discord.h"

class App {
public:
	// Initialize everything
	bool init();

	// Main program function
	void run();

	void shutdown();

private:
	UI* pUI = nullptr;
	VLCTools* pVLCTools = nullptr;
	Discord* pDiscord = nullptr;

	bool shouldExit = false;
};

#endif