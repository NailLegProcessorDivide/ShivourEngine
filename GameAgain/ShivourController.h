#pragma once

#include <SDL.h>
#include <vector>

enum ShivControlMapings {
	BUTTON_A            = 0x000001,
	BUTTON_B            = 0x000002,
	BUTTON_X            = 0x000004,
	BUTTON_Y            = 0x000008,
	BUTTON_BACK         = 0x000010,
	BUTTON_GUIDE        = 0x000020,
	BUTTON_START        = 0x000040,
	BUTTON_LEFTSTICK    = 0x000080,
	BUTTON_RIGHTSTICK   = 0x000100,
	BUTTON_LEFTSHOLDER  = 0x000200,
	BUTTON_RIGHTSHOLDER = 0x000400,
	BUTTON_DPADUP       = 0x000800,
	BUTTON_DPADDOWN     = 0x001000,
	BUTTON_DPADLEFT     = 0x002000,
	BUTTON_DPADRIGHT    = 0x004000,
};
enum ShivAxis{
	AXIS_LEFTX          = 0x008000,
	AXIS_LEFTY          = 0x010000,
	AXIS_RIGHTX         = 0x020000,
	AXIS_RIGHTY         = 0x040000,
	AXIS_TRIGGERLEFT    = 0x080000,
	AXIS_TRIGERRIGHT    = 0x100000
};


class ShivourController {
	SDL_GameController* controller;
	std::vector<int> binds;
	int numbuttons;

public:
	ShivourController();
	~ShivourController();
};