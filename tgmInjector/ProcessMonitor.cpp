#include "include/ProcessMonitor.h"
#include <TlHelp32.h>
#include <unordered_set>
#include <iostream>

static std::unordered_set<DWORD> knownPIDs;

// uses Tool Help to capture all running processes in the system
void CaptureInitialProcesses() {
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

DWORD WaitForNewProcesses() {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	// Polls every 10 ms for our any new process that may appear and returns any new process that comes in
	while (true) {
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (snapshot == INVALID_HANDLE_VALUE) {
			std::cout << "ERROR: CaptureInitialProcesses could not write a snapshot. \n";
			return NULL;
		}
		if (Process32FirstW(snapshot, &entry)) {
			do {
				if (knownPIDs.find(entry.th32ProcessID) == knownPIDs.end()) {
					knownPIDs.insert(entry.th32ProcessID);

					std::wcout << L"New process: " << entry.szExeFile << L"(PID: " << entry.th32ProcessID << L") \n";

					CloseHandle(snapshot);
					return entry.th32ProcessID;
				}
			} while (Process32NextW(snapshot, &entry));
		}

		CloseHandle(snapshot);
		Sleep(10);
	}
}