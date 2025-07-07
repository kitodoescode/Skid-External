#pragma once
#include <windows.h>

namespace Offsets {
	inline constexpr uintptr_t LocalPlayer = 0x128;
	inline constexpr uintptr_t ModelInstance = 0x330;

	inline constexpr uintptr_t WalkSpeed = 0x1D8;
	inline constexpr uintptr_t WalkSpeedCheck = 0x3B0;
	inline constexpr uintptr_t JumpPower = 0x1B8;
	inline constexpr uintptr_t Health = 0x19C;

	inline constexpr uintptr_t Name = 0x78;
	inline constexpr uintptr_t Size = 0x10;
	inline constexpr uintptr_t Children = 0x80;
	inline constexpr uintptr_t ChildrenEnd = 0x8;
	inline constexpr uintptr_t ClassDescriptor = 0x18;
	inline constexpr uintptr_t ClassDescriptorToClassName = 0x8;

	inline constexpr uintptr_t FakeDataModelPointer = 0x67633D8;
	inline constexpr uintptr_t FakeDataModelToDataModel = 0x1B8;

	namespace TaskScheduler {
		inline constexpr uintptr_t RawScheduler = 0x6829508;
		inline constexpr uintptr_t Job_Name = 0x18;
		inline constexpr uintptr_t JobStart = 0x1D0;
		inline constexpr uintptr_t JobEnd = 0x1D8;
	}
}