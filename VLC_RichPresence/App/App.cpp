#include "App.h"
#include "Error.h"

bool App::init() {
	pUI = new UI();
	if (!pUI->init())
		return false;

	pVLCTools = new VLCTools();
	if (!pVLCTools->init())
		return false;

	pDiscord = new Discord();
	if (!pDiscord->init())
		return false;

	return true;
}

void App::run() {
	do {
		pUI->run(&shouldExit);
		pVLCTools->run();
		pDiscord->run(pVLCTools);
	} while (!shouldExit);
}

void App::shutdown() {
	if (pUI) {
		pUI->shutdown();
		delete pUI;
		pUI = nullptr;
	}
	
	if (pVLCTools) {
		pVLCTools->shutdown();
		delete pVLCTools;
		pVLCTools = nullptr;
	}

	if (pDiscord) {
		pDiscord->shutdown();
		delete pDiscord;
		pDiscord = nullptr;
	}
}