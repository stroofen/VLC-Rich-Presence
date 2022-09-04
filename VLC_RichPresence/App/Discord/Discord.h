#ifndef DISCORD_H
#define DISCORD_H

#include "../../Global.h"

#ifdef SYSTEM_WINDOWS
#pragma once
#endif

#include "../Timer.h"

class VLCTools;

class Discord {
public:
	bool init();

	void run(VLCTools* pVLCTools);

	void shutdown();

private:
	Timer updateTimer{};
};

#endif