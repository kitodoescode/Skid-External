#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>

#define code_rw CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1645, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_ba CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1646, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_get_guarded_region CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1647, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_security 0x85b3b69

typedef struct _rw {
	INT32 security;
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
	BOOLEAN write;
} rw, * prw;

typedef struct _ba {
	INT32 security;
	INT32 process_id;
	ULONGLONG* address;
} ba, * pba;

typedef struct _ga {
	INT32 security;
	ULONGLONG* address;
} ga, * pga;

namespace mem {
	extern HANDLE driver_handle;
	extern INT32 process_id;

	bool find_driver();
	void read_physical(PVOID address, PVOID buffer, DWORD size);
	void write_physical(PVOID address, PVOID buffer, DWORD size);
	uintptr_t find_image();
	uintptr_t get_guarded_region();
	INT32 find_process(LPCTSTR process_name);
}

template <typename T>
inline T read(uint64_t address) {
	T buffer{ };
	mem::read_physical((PVOID)address, &buffer, sizeof(T));
	return buffer;
}

template <typename T>
inline T write(uint64_t address, T buffer) {
	mem::write_physical((PVOID)address, &buffer, sizeof(T));
	return buffer;
}

inline bool read_buffer(uint64_t address, void* buffer, size_t size) {
	if (address == 0 || address > 0x7FFFFFFFFFFF)
		return false;

	mem::read_physical((PVOID)address, buffer, size);
	return true;
}