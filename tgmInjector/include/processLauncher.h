#pragma once
#include <windows.h>

struct ProcessHandle {
	PROCESS_INFORMATION pi;
	BOOL valid;
};

ProcessHandle LaunchProcess(const wchar_t* path, bool suspended);
void CloseProcessHandle(ProcessHandle& handle);