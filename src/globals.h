#pragma once
#include <windows.h>

class CGlobals {
public:
	uintptr_t BaseAddress;
	uintptr_t DataModel;
	uintptr_t Workspace;
	uintptr_t Players;
	uintptr_t LocalPlayer;
	uintptr_t Character;
	uintptr_t Humanoid;

	// default values
	float walkspeed = 16;
	float jumppower = 50;
	float health = 100;
};

inline auto Globals = std::make_unique<CGlobals>();