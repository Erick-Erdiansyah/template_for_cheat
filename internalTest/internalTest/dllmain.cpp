// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "process.h"
#include "mem.h"


DWORD WINAPI hackThread(HMODULE hModule) {
    // create console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "hey fam, can I get a bit\n";
    // get module base
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"get module");
    bool health = false, bAmmo = false, bRecoil = false; 
    // hack loop
    while (true){
    // key input
        if (GetAsyncKeyState(VK_END) &1) {
            break;
        }
        if (GetAsyncKeyState(VK_NUMPAD1) &1) {
            health = !health;
        }
        if (GetAsyncKeyState(VK_NUMPAD2) &1) {
            bAmmo = !bAmmo;
        }
        if (GetAsyncKeyState(VK_NUMPAD3) &1) {
            bRecoil = !bRecoil;
            if (bRecoil) {
                // nope
                mem::Nope((BYTE*)(moduleBase + 0x9383), 10);
            }
            else {
                // write back
                mem::patch((BYTE*)(moduleBase+0x9383), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10);
            }
        }
       // freeze
        uintptr_t* playerPtr = (uintptr_t*)(moduleBase + 0x98239);
        if (playerPtr) {
            if (health) {
                *(int*)(*playerPtr + 0xf8) = 1337;
            }
            if (bAmmo) {
                uintptr_t ammoAddr = mem::FindDMAAddy(moduleBase + 0x9393, { 0x9ab,0x821,0x81a });
                int* ammo = (int*)ammoAddr;
                *ammo = 999;
                // or
                *(int*)mem::FindDMAAddy(moduleBase + 0x9393, { 0x9ab,0x821,0x81a }) = 999;
            }
        }
        Sleep(10);
    }
    // cleanup & eject
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        // calling loaded dll library
    case DLL_PROCESS_ATTACH: {
       CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)hackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

