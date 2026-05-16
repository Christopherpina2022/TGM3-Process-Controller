#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include "include/processLauncher.h"
#include "include/injector.h"
#include "include/config.h"

static const char* DLL_PATH = "tgmPatch.dll";

int main(int argc, char* argv[]) {
    // launch Typex_loader.exe Game.exe
    // MENTAL NOTE: Launch this app in TGM3's directory for testing. also use true to suspend app
    std::cout << "Starting typex_loader.exe... \n";
    ProcessHandle loader = LaunchProcess(L"typex_loader.exe", L"game.exe", true);
    if (!loader.valid) {
        std::cout << "ERROR: loader has failed, please check if typex_loader is in the same directory as this executable.";
        return 1;
    }
    std::cout << "Process created. PID: " << loader.pi.dwProcessId << "\n";

    // Inject DLL file into typexLoader then continue thread
    InjectDLL(loader.pi.dwProcessId, DLL_PATH);
    ResumeThread(loader.pi.hThread);

    std::cin.get(); // Holds console so it doesn't auto clear

    return 0;
}