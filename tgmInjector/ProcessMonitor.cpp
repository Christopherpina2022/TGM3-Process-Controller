#include "include/ProcessMonitor.h"
#include "include/injector.h"
#include <TlHelp32.h>
#include <iostream>

static const wchar_t* TARGET_PROCESS = L"game.exe";
static const char* DLL_PATH = "tgmPatch.dll";

ProcessMonitor::ProcessMonitor()
{
	selfPID = GetCurrentProcessId();
}

// uses Tool Help to capture all running processes in the system
void ProcessMonitor::CaptureInitialProcesses() {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapshot == INVALID_HANDLE_VALUE) {
		std::cout << "ERROR: CaptureInitialProcesses could not write a snapshot. \n";
		return;
	}

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	if (Process32FirstW(snapshot, &entry)) {
		do {
			knownPIDs.insert(entry.th32ProcessID);
		} while (Process32NextW(snapshot, &entry));
	}

	CloseHandle(snapshot);
	std::cout << "Initial snapshot has been created. \n";
}

bool ProcessMonitor::IsNewProcess(DWORD pid) {
	if (pid == 0 || pid == selfPID) {
		return false;
	}

	return knownPIDs.find(pid) == knownPIDs.end();
}

DWORD ProcessMonitor::WaitForGame() {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	// Polls every 10 ms for our any new process that may appear and returns when game is found
	while (true) {
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshot == INVALID_HANDLE_VALUE) {
			std::cout << "ERROR: CaptureInitialProcesses could not write a snapshot. \n";
			return NULL;
		}

		if (Process32FirstW(snapshot, &entry)) {
			do {
				if (IsNewProcess(entry.th32ProcessID)) {
					knownPIDs.insert(entry.th32ProcessID);

					std::wcout << L"New process: " << entry.szExeFile << L"(PID: " << entry.th32ProcessID << L") \n";

					CloseHandle(snapshot);
					if (_wcsicmp(entry.szExeFile, TARGET_PROCESS) == 0) {
						std::cout << "Injecting DLL... \n";
						InjectDLL(entry.th32ProcessID, DLL_PATH);
						return entry.th32ProcessID;
					}
				}
			} while (Process32NextW(snapshot, &entry));
		}

		CloseHandle(snapshot);
		Sleep(10);
	}
}