#include <windows.h>
#include <iostream>
#include "include/injector.h"

bool InjectDLL(DWORD pid, const char* dllPath) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess) {
		std::cout << "ERROR: Inject DLL failed: could not open process. \n";
		return false;
	}

	LPVOID remoteMemory = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!remoteMemory) {
		std::cout << "Error: Inject DLL failed: could not allocate remote memory. \n";
		CloseHandle(hProcess);
		return false;
	}

	WriteProcessMemory(hProcess, remoteMemory, dllPath, strlen(dllPath) + 1, NULL);

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, remoteMemory, 0, NULL);
	if (!hThread) {
		std::cout << "Error: Inject DLL failed: could not create remote thread. \n";
		CloseHandle(hProcess);
		return false;
	}

	std::cout << "Injected DLL successfully.";

	CloseHandle(hThread);
	CloseHandle(hProcess);

	return true;
}