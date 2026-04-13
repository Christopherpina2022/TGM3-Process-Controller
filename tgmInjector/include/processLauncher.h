#pragma once
#include <windows.h>

struct ProcessHandle {
	PROCESS_INFORMATION pi;
	BOOL valid;
};

ProcessHandle LaunchProcess(const wchar_t* path, const wchar_t* args = nullptr, bool suspended = false);
void CloseProcessHandle(ProcessHandle& handle);