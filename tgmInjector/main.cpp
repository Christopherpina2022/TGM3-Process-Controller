#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include "include/processLauncher.h"
#include "include/ProcessMonitor.h"

/*DWORD GetProcessIdByName(const wchar_t* processName)
{
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return 0;

    if (Process32FirstW(snapshot, &entry))
    {
        do
        {
            if (_wcsicmp(entry.szExeFile, processName) == 0)
            {
                DWORD pid = entry.th32ProcessID;
                CloseHandle(snapshot);
                return pid;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return 0;
} */

/*
    DWORD pid = 0;
    const char* dllPath = "tgmPatch.dll";

    while (!pid) {
        //pid = GetProcessIdByName(L"TGM3 - Copyright (C) Arika Co., Ltd - 2004-2005 (Jan 30 2005 17:52:56)");
        pid = GetProcessIdByName(L"game.exe");
        Sleep(200);
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        printf("Failed to get process");
        return 1;
    }

    LPVOID remoteMemory = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMemory) {
        std::cout << "VirtualAllocEx failed\n";
        return 1;
    }

    WriteProcessMemory(hProcess, remoteMemory, dllPath, strlen(dllPath) + 1, NULL);

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, remoteMemory, 0, NULL);
    if (!hThread) {
        std::cout << "CreateRemoteThread failed\n";
        return 1;
    }

    std::cout << "Injected successfully!\n";
    CloseHandle(hThread);
    CloseHandle(hProcess);
    */

int main() {
    std::cout << "Starting process monitor... \n";
    CaptureInitialProcesses();

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
    DWORD newPid = WaitForNewProcesses();

    std::cout << "Detected new process PID: " << newPid << "\n";

    std::cin.get(); // Holds console so it doesn't auto clear

    return 0;
}