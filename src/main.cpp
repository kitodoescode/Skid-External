#include <windows.h>
#include <iostream>

#include "utils/utils.h"

int main() {
	SetConsoleTitleA("skid external - kito was here :)"); // set title

	// header for decoration purpose
	printf(
		R"(
           /$$       /$$       /$$
          | $$      |__/      | $$
  /$$$$$$$| $$   /$$ /$$  /$$$$$$$
 /$$_____/| $$  /$$/| $$ /$$__  $$
|  $$$$$$ | $$$$$$/ | $$| $$  | $$
 \____  $$| $$_  $$ | $$| $$  | $$
 /$$$$$$$/| $$ \  $$| $$|  $$$$$$$
|_______/ |__/  \__/|__/ \_______/

)"
	);

	std::cout << "  ---  made by kito with <3  ---" << "\n\n"; // more decoration stuff

	std::cout << "[+] Welcome to Skid External!" << "\n\n";

	if (!mem::find_driver()) {
		std::cout << "[!] Driver isn't loaded." << "\n";
		std::cout << "\n" << "Press any key to exit . . .";
		std::cin.get();
		return 0;
	}

	if (!mem::find_process("RobloxPlayerBeta.exe")) {
		std::cout << "[!] RobloxPlayerBeta.exe not found!" << "\n";
		std::cout << "\n" << "Press any key to exit . . .";
		std::cin.get();
		return 0;
	}

	if (!Utils->GetBaseAddr()) {
		std::cout << "[!] Base Address not found!" << "\n";
		std::cout << "\n" << "Press any key to exit . . .";
		std::cin.get();
		return 0;
	}

	std::cout << "[+] Found RobloxPlayerBeta.exe @ 0x" << std::hex << Globals->BaseAddress << "\n";

	if (!Utils->GetDataModel()) {
		std::cout << "[!] DataModel not found!" << "\n";
		std::cout << "\n" << "Press any key to exit . . .";
		std::cin.get();
		return 0;
	}

	std::cout << "[+] Found DataModel [Ugc] @ 0x" << std::hex << Globals->DataModel << "\n";

	if (!Utils->GetWorkspace()) {
		std::cout << "[!] Workspace not found!" << "\n";
		std::cout << "\n" << "Press any key to exit . . .";
		std::cin.get();
		return 0;
	}

	std::cout << "[+] Found Workspace [Service] @ 0x" << std::hex << Globals->Workspace << "\n";

	if (!Utils->GetPlayers()) {
		std::cout << "[!] Players not found!" << "\n";
		std::cout << "\n" << "Press any key to exit . . .";
		std::cin.get();
		return 0;
	}

	std::cout << "[+] Found Players [Service] @ 0x" << std::hex << Globals->Players << "\n";

	if (!Utils->GetLocalPlayer()) {
		std::cout << "[!] LocalPlayer not found!" << "\n";
		std::cout << "\n" << "Press any key to exit . . .";
		std::cin.get();
		return 0;
	}

	std::cout << "[+] Found LocalPlayer [Instance] @ 0x" << std::hex << Globals->LocalPlayer << "\n";

	if (!Utils->GetCharacter()) {
		std::cout << "[!] Character not found!" << "\n";
		std::cout << "\n" << "Press any key to exit . . .";
		std::cin.get();
		return 0;
	}

	std::cout << "[+] Found Character [Instance] @ 0x" << std::hex << Globals->Character << "\n";

	if (!Utils->GetHumanoid()) {
		std::cout << "[!] Humanoid not found!" << "\n";
		std::cout << "\n" << "Press any key to exit . . .";
		std::cin.get();
		return 0;
	}

	std::cout << "[+] Found Humanoid [Instance] @ 0x" << std::hex << Globals->Humanoid << "\n";

	//Utils->SetWalkspeed(200.0f);
	//Utils->SetJumpPower(200.0f);

	{
		// job printing block
		for (uintptr_t job : Utils->GetActiveJobs()) {
			std::string name = Utils->GetJobName(job);
			std::cout << "[+] Found " << name.c_str() << " [Job] @ 0x" << std::hex << job << "\n";
		}
	}

	std::cout << "\n" << "Press any key to exit . . .";
	std::cin.get();

	return 1; // 1 == success for me
}