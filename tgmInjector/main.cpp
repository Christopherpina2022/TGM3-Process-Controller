#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include "include/processLauncher.h"
#include "include/ProcessMonitor.h"

int main() {
    ProcessMonitor monitor;
    std::cout << "Starting process monitor... \n";
    monitor.CaptureInitialProcesses();

    // MENTAL NOTE: Launch this app in TGM3's directory for testing
    std::cout << "Starting typex_loader.exe... \n";
    ProcessHandle loader = LaunchProcess(L"typex_loader.exe", L"game.exe", true);
    if (!loader.valid) {
        std::cout << "ERROR: loader has failed, please check if typex_loader is in the same directory as this executable.";
        return 1;
    }

    std::cout << "Process created. PID: " << loader.pi.dwProcessId << "\n";
    std::cout << "Press ENTER to resume the loader... \n";
    std::cin.get();

    std::cout << "Resuming process... \n";
    ResumeThread(loader.pi.hThread);

    std::cout << "starting search for any new processes that come in... \n";
    DWORD newPid = monitor.WaitForGame();

    std::cout << "Detected new process PID: " << newPid << "\n";

    std::cin.get(); // Holds console so it doesn't auto clear

    return 0;
}