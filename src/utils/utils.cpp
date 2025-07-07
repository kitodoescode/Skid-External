#include "utils.h"

bool CUtils::IsAddressValid(uintptr_t address) {
	if (address < 0x10000 || address > 0x7FFFFFFFFFFF)
		return false;

	MEMORY_BASIC_INFORMATION MemoryInfo;

	if (VirtualQuery((void*)address, &MemoryInfo, sizeof(MEMORY_BASIC_INFORMATION)) == 0)
		return false;

	return true;
}

std::string CUtils::ReadString(uintptr_t address) {
	const auto size = read<uintptr_t>(address + Offsets::Size); // size of the pointer

	if (size >= 16) // stored in another address
		address = read<uintptr_t>(address); // read address to get the other address

	std::string content;
	BYTE code;

	// manually read each character inside the pointer and push it to the main string
	for (std::int32_t index = 0; code = read<std::uint8_t>(address + index); index++) {
		content.push_back(code);
	}

	return content; // return string
}

std::string CUtils::GetName(uintptr_t address) {
	uintptr_t nameptr = read<uintptr_t>(address + Offsets::Name); // name ptr

	std::string name; // name container
	const auto size = read<int>(nameptr + Offsets::Size); // size of name ptr

	if (size >= 16) { // if the size is greater than 16, means this pointer holds another pointer which has the actual name
		uintptr_t nameptr2 = read<uintptr_t>(nameptr); // get the actual name ptr
		while (read<char>(nameptr2) != '\0') { // read character in nameptr2 till its null
			name += read<char>(nameptr2); // add that to the name container
			nameptr2++; // increase index so next one is read
		}
	}
	else { // name is directly accessible
		name = read<std::string>(nameptr); // read the name ptr as string
	}

	return name; // return name
}

std::string CUtils::GetClassName(uintptr_t address) {
	uintptr_t classDescriptor = read<uintptr_t>(address + Offsets::ClassDescriptor); // class desc addr
	uintptr_t className = read<uintptr_t>(classDescriptor + Offsets::ClassDescriptorToClassName); //  class desc to class name addr

	if (className) { // if class name exists
		std::string classname = ReadString(className); // read string
		return classname; // return name
	}

	return "Unknown"; // not found
}

std::vector<uintptr_t> CUtils::GetChildren(uintptr_t address) {
	std::vector<uintptr_t> children; // children container

	uintptr_t childrenPtr = read<uintptr_t>(address + Offsets::Children); // children start
	uintptr_t childrenEnd = read<uintptr_t>(childrenPtr + Offsets::ChildrenEnd); // children end

	for (auto i = read<uintptr_t>(childrenPtr); i < childrenEnd; i += Offsets::Size) { // loop through each child of parent
		children.push_back(read<uintptr_t>(i)); // push back to list
	}

	return children; // return list
}

uintptr_t CUtils::FindFirstChild(uintptr_t address, std::string name) {
	for (auto child : GetChildren(address)) { // for child in children
		if (GetName(child) == name) { // if name matches
			return child; // return child
		}
	}

	return 0; // child not found
}

uintptr_t CUtils::FindFirstChildByClass(uintptr_t address, std::string classname) {
	for (auto child : GetChildren(address)) { // for child in children
		if (GetClassName(child) == classname) { // if classname matches
			return child; // return child
		}
	}

	return 0; // child not found
}

uintptr_t CUtils::GetBaseAddr() {
	uintptr_t base = mem::find_image(); // the driver find the base
	Globals->BaseAddress = base; // store base address for further use
	return base;
}

uintptr_t CUtils::GetDataModel() {
	uintptr_t base = Globals->BaseAddress; // our stored base address
	uintptr_t fakedmptr = read<uintptr_t>(base + Offsets::FakeDataModelPointer); // adding the offset to base to get the fake datamodel pointer
	uintptr_t dmptr = read<uintptr_t>(fakedmptr + Offsets::FakeDataModelToDataModel); // this is the datamodel pointer

	Globals->DataModel = dmptr; // store datamodel pointer for further use

	// a check that can be used depending on ur case

	/*
	uintptr_t nameptr = *(uintptr_t*)(dmptr + Offsets::Name);
	std::string dmname = *(std::string*)nameptr;

	// 'Ugc' means ingame, 'LuaApp' means on homepage, so could be used for tphandler related stuff
	if (dmname == "LuaApp") {
		return 0;
	}
	*/

	return dmptr;
}

uintptr_t CUtils::GetWorkspace() {
	uintptr_t datamodel = Globals->DataModel; // our stored datamodel

	uintptr_t workspace = FindFirstChild(datamodel, "Workspace"); // ffc

	if (!workspace)
		workspace = FindFirstChildByClass(datamodel, "Workspace"); // fallback ffc incase previous one didnt work

	Globals->Workspace = workspace;

	return workspace; // return workspace
}

uintptr_t CUtils::GetPlayers() {
	uintptr_t datamodel = Globals->DataModel; // stored datamodel
	uintptr_t players = FindFirstChild(datamodel, "Players"); // ffc

	if (!players) // fallback ffc
		players = FindFirstChildByClass(datamodel, "Players");

	Globals->Players = players; // store
	return players; // return
}

uintptr_t CUtils::GetLocalPlayer() {
	uintptr_t players = Globals->Players; // stored players srvc
	uintptr_t localplayer = read<uintptr_t>(players + Offsets::LocalPlayer); // localplayer addr

	Globals->LocalPlayer = localplayer; // store
	return localplayer; // return
}

uintptr_t CUtils::GetCharacter() {
	uintptr_t localplayer = Globals->LocalPlayer; // stored localplayer
	uintptr_t character = read<uintptr_t>(localplayer + Offsets::ModelInstance); // character addr

	Globals->Character = character; // store
	return character; // return
}

uintptr_t CUtils::GetHumanoid() {
	uintptr_t character = Globals->Character; // stored character
	uintptr_t humanoid = FindFirstChild(character, "Humanoid"); // child is named "Humanoid"

	Globals->Humanoid = humanoid; // store
	return humanoid; // return
}

void CUtils::SetWalkspeed(float val) {
	uintptr_t humanoid = Globals->Humanoid; // stored humanoid
	uintptr_t walkspeed = (humanoid + Offsets::WalkSpeed); // walkspeed ptr
	uintptr_t walkspeedcheck = (humanoid + Offsets::WalkSpeedCheck); // walkspeed check (avoid getting kicked)

	// write to both ptrs
	write<float>(walkspeed, val);
	write<float>(walkspeedcheck, val);
}

void CUtils::SetJumpPower(float val) {
	uintptr_t humanoid = Globals->Humanoid; // stored humanoid
	uintptr_t jumppower = (humanoid + Offsets::JumpPower); // jumppower ptr

	// write to ptr
	write<float>(jumppower, val);
}

void CUtils::SetHealth(float val) {
	uintptr_t humanoid = Globals->Humanoid; // stored humanoid
	uintptr_t health = (humanoid + Offsets::Health); // health ptr

	// write to ptr
	write<float>(health, val);
}

std::vector<uintptr_t> CUtils::GetActiveJobs() {
	std::vector<uintptr_t> Jobs; // jobs container

	uintptr_t base = Globals->BaseAddress;
	uintptr_t Scheduler = read<uintptr_t>(base + Offsets::TaskScheduler::RawScheduler); // scheduler
	uintptr_t JobStartPtr = read<uintptr_t>(Scheduler + Offsets::TaskScheduler::JobStart); // jobs start
	uintptr_t JobEndPtr = read<uintptr_t>(Scheduler + Offsets::TaskScheduler::JobEnd); // jobs end

	for (uintptr_t i = JobStartPtr; i < JobEndPtr; i += Offsets::Size) { // for each item from job start to job end
		uintptr_t job = read<uintptr_t>(i); // read item
		Jobs.push_back(job); // add the job to list
	}

	return Jobs; // return job list
}

uintptr_t CUtils::GetJobByName(std::string name) {
	for (auto Job : GetActiveJobs()) { // for each item "Job" in the jobs list
		if (!Job) // check if job is valid
			continue;

		if (GetJobName(Job) == name) { // if job name is same as provided name
			return Job; // return the job
		}
	}

	return 0; // job not found
}

std::string CUtils::GetJobName(uintptr_t job) {
	uintptr_t nameptr = job + Offsets::TaskScheduler::Job_Name;

	int size = read<int>(nameptr + Offsets::Size); 

	if (size <= 0) // invalid
		return "Unknown";

	if (size >= 16) // the string is stored in another address whose pointer is stored here
		nameptr = read<uintptr_t>(nameptr); // read the pointer so we get to the actual address

	char jobname[128]; // buffer to store jobname

	// min(size, 127) is to ensure that no overflow occurs, 128 space in buffer - 127 characters + 1 null terminator
	read_buffer(nameptr, jobname, min(size, 127)); // reading the string
	jobname[min(size, 127)] = '\0'; // adding null terminator to ensure no extra random shit

	std::string name = std::string(jobname); // convert to a string

	if (name.empty()) // if string is empty
		return "Unknown"; // return 'Unknown'

	return name;
}