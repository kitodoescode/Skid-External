#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

#include "globals.h"
#include "driver/driver.h"
#include "offsets.h"

class CUtils {
public:
	bool IsAddressValid(uintptr_t address);
	std::string ReadString(uintptr_t address);
	std::string GetName(uintptr_t address);
	std::string GetClassName(uintptr_t address);
	std::vector<uintptr_t> GetChildren(uintptr_t address);
	uintptr_t FindFirstChild(uintptr_t address, std::string name);
	uintptr_t FindFirstChildByClass(uintptr_t address, std::string classname);

	uintptr_t GetBaseAddr();
	uintptr_t GetDataModel();
	uintptr_t GetWorkspace();
	uintptr_t GetPlayers();
	uintptr_t GetLocalPlayer();
	uintptr_t GetCharacter();
	uintptr_t GetHumanoid();

	// note: might appear as ints in roblox studio but internally are floats
	void SetWalkspeed(float val);
	void SetJumpPower(float val);
	void SetHealth(float val);

	std::vector<uintptr_t> GetActiveJobs();
	uintptr_t GetJobByName(std::string name);
	std::string GetJobName(uintptr_t job);
};

inline auto Utils = std::make_unique<CUtils>();