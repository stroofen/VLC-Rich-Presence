#include "Discord.h"
#include "DiscordAPI/include/discord_register.h"
#include "DiscordAPI/include/discord_rpc.h"
#include "../VLCTools/VLCTools.h"

bool Discord::init() {
	DiscordEventHandlers handlers = {};
	Discord_Initialize(g_DiscordAppID, &handlers, 0, "");
	return true;
}

void Discord::run(VLCTools* pVLCTools) {
	if (pVLCTools->isVLCRunning()) {
		if (updateTimer.hasElapsed(g_DiscordUpdateInterval) || g_ForceUpdate) {

			DiscordRichPresence custom = {};
			custom.largeImageKey = g_DiscordIconString;
			custom.largeImageText = g_VersionA;
			custom.details = g_VLCTitle;
			if (const i64 timestamp = pVLCTools->getStartTimestamp()) {
				custom.startTimestamp = timestamp;
				custom.endTimestamp = 0;
			}
			custom.instance = 1;
			Discord_UpdatePresence(&custom);

			updateTimer.reset();
		}
	} else {
		Discord_ClearPresence();
	}
}

void Discord::shutdown() {
	Discord_ClearPresence();
	Discord_Shutdown();
}