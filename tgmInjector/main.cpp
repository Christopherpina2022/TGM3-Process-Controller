#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

DWORD GetProcessIdByName(const wchar_t* processName)
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
}

int main() {
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

    return 0;
}