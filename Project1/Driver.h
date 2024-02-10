#pragma once
#include <windows.h>
#include <winternl.h>
#include <process.h>
#include <tlhelp32.h>
#include <inttypes.h>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <memory>
#include <string_view>
#include <cstdint>
#include <string>
#include <cmath>
#include <thread>
#include <cassert>
#include <xstring>
#include <dwmapi.h>
#include <vector>
#include <map>
#include <array>
#include <sstream>
#include <fstream>
#include <direct.h>

inline HANDLE DriverHandle;
inline HWND hwnd = NULL;
inline DWORD processID;

typedef struct _NULL_MEMORY
{
	void* buffer_address;
	UINT_PTR address;
	ULONGLONG size;
	ULONG pid;
	BOOLEAN write;
	BOOLEAN read;
	BOOLEAN req_base;
	void* output;
	const char* module_name;
	ULONG64 base_address;
}NULL_MEMORY;
struct HandleDisposer
{
	using pointer = HANDLE;
	void operator()(HANDLE handle) const
	{
		if (handle != NULL || handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
		}
	}
};

inline uintptr_t oBaseAddress = 0;
inline std::uint32_t oPID = 0;
using unique_handle = std::unique_ptr<HANDLE, HandleDisposer>;

template<typename ... Arg>
inline uint64_t CallHook(const Arg ... args)
{
	void* hooked_func = GetProcAddress(LoadLibrary(("win32u.dll")), ("NtGdiDdDDINetDispGetNextChunkInfo"));

	auto func = static_cast<uint64_t(_stdcall*)(Arg...)>(hooked_func);

	return func(args ...);
}

inline std::uint32_t GetPID(std::string_view process_name)
{
	PROCESSENTRY32 processentry;
	const unique_handle snapshot_handle(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL));

	if (snapshot_handle.get() == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	processentry.dwSize = sizeof(MODULEENTRY32);

	while (Process32Next(snapshot_handle.get(), &processentry) == TRUE)
	{
		if (process_name.compare(processentry.szExeFile) == NULL)
		{
			return processentry.th32ProcessID;
		}
	}
	return NULL;
}

inline static ULONG64 GetModuleBaseAddress(std::uint32_t, const char* module_name)
{
	NULL_MEMORY instructions = { 0 };
	instructions.pid = oPID;
	instructions.req_base = TRUE;
	instructions.read = FALSE;
	instructions.write = FALSE;
	instructions.module_name = module_name;
	CallHook(&instructions);

	ULONG64 base = NULL;
	base = instructions.base_address;
	return base;
}

template <typename Type>
inline Type read(unsigned long long int Address)
{
	Type response{};
	NULL_MEMORY instructions;
	instructions.pid = oPID;
	instructions.size = sizeof(Type);
	instructions.address = Address;
	instructions.read = TRUE;
	instructions.write = FALSE;
	instructions.req_base = FALSE;
	instructions.output = &response;
	CallHook(&instructions);

	return response;
}

template <typename Type>
inline Type readmem(unsigned long long int Address, int len)
{
	Type response{};
	NULL_MEMORY instructions;
	instructions.pid = oPID;
	instructions.size = len;
	instructions.address = Address;
	instructions.read = TRUE;
	instructions.write = FALSE;
	instructions.req_base = FALSE;
	instructions.output = &response;
	CallHook(&instructions);

	return response;
}

inline void writefloat(unsigned long long int Address, float stuff)
{
	NULL_MEMORY instructions;
	instructions.address = Address;
	instructions.pid = oPID;
	instructions.write = TRUE;
	instructions.read = FALSE;
	instructions.req_base = FALSE;
	instructions.buffer_address = (void*)&stuff;
	instructions.size = sizeof(float);

	CallHook(&instructions);
}

inline bool writevall(unsigned long long int Address, UINT_PTR value, SIZE_T write_size)
{
	NULL_MEMORY instructions;
	instructions.address = Address;
	instructions.pid = oPID;
	instructions.write = TRUE;
	instructions.read = FALSE;
	instructions.req_base = FALSE;
	instructions.buffer_address = (void*)value;
	instructions.size = write_size;

	CallHook(&instructions);
	return true;
}

template<typename S>
inline bool write(UINT_PTR write_address, const S& value)
{
	return writevall(write_address, (UINT_PTR)&value, sizeof(S));
}


inline void writedouble(unsigned long long int Address, double stuff)
{
	NULL_MEMORY instructions;
	instructions.address = Address;
	instructions.pid = oPID;
	instructions.write = TRUE;
	instructions.read = FALSE;
	instructions.req_base = FALSE;
	instructions.buffer_address = (void*)&stuff;
	instructions.size = sizeof(double);

	CallHook(&instructions);
}
