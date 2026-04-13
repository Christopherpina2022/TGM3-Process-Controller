#include "include/processLauncher.h"
#include <iostream>

// Starts the incoming process in a suspended state
ProcessHandle LaunchProcess(const wchar_t* path, bool suspended) {
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	
	DWORD flags = suspended ? CREATE_SUSPENDED : 0;

	BOOL result = CreateProcessW(
		path,
		NULL,
		NULL,
		NULL,
		FALSE,
		flags,
		NULL,
		NULL,
		&si,
		&pi
	);

	if (!result) {
		std::cout << "Failed to launch process. \n";
		return { {}, false };
	}

	std::cout << "Process launched. \n";
	std::cout << "PID: " << pi.dwProcessId << "\n";

	return { pi, true };
}

void CloseProcessHandle(ProcessHandle& handle) {
	if (handle.valid) {
		CloseHandle(handle.pi.hThread);
		CloseHandle(handle.pi.hProcess);
		handle.valid = false;
	}
}