#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include "include/processLauncher.h"
#include "include/ProcessMonitor.h"
#include "include/config.h"

int main(int argc, char* argv[]) {
    /*
    if (argc < 3)
    {
        std::cout << "Usage: app.exe <width> <height>\n";
        std::cin.get();
        return 1;
    }

    // Run config
    ResolutionConfig cfg;
    cfg.width = std::stoi(argv[1]);
    cfg.height = std::stoi(argv[2]);
    if (!WriteConfig(cfg, "C:\\temp\\tgm3_config.bin")) {
        std::cin.get();
        return 1;
    }
    */
    // Run process monitor
    ProcessMonitor monitor;
    std::cout << "Starting process monitor... \n";
    monitor.CaptureInitialProcesses();

    // launch Typex_loader.exe
    // MENTAL NOTE: Launch this app in TGM3's directory for testing. also use true to suspend app
    std::cout << "Starting typex_loader.exe... \n";
    ProcessHandle loader = LaunchProcess(L"typex_loader.exe", L"game.exe", false);
    if (!loader.valid) {
        std::cout << "ERROR: loader has failed, please check if typex_loader is in the same directory as this executable.";
        return 1;
    }
    std::cout << "Process created. PID: " << loader.pi.dwProcessId << "\n";

    // TEMP pauser for testing
    /*
    std::cout << "Press ENTER to resume the loader... \n"; 
    std::cin.get();
    std::cout << "Resuming process... \n";
    ResumeThread(loader.pi.hThread);
    */

    // Run monitor to search for game.exe
    std::cout << "starting search for any new processes that come in... \n";
    DWORD newPid = monitor.WaitForGame();

    std::cout << "Detected new process PID: " << newPid << "\n";
    std::cin.get(); // Holds console so it doesn't auto clear

    return 0;
}