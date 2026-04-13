#pragma once
#include <windows.h>
#include <unordered_set>

class ProcessMonitor {
public:
	ProcessMonitor();

	void CaptureInitialProcesses();
	DWORD WaitForGame();
private:
	std::unordered_set<DWORD> knownPIDs;
	DWORD selfPID;
	bool IsNewProcess(DWORD pid);
};


