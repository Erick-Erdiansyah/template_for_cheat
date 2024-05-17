// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <Windows.h>
#include <process.h>
#include "process.h"
#include "mem.h"

int main(){
    //// get process id
    //DWORD procId = GetProcId(L"id");
    //// get module base address
    //uintptr_t moduleBase = GetModuleBaseAddress(procId, L"module");
    //// get handle to process
    //HANDLE hProcess = 0;
    //hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
    //// resolve base address plus relative offset"
    //uintptr_t DBAddress = moduleBase + 0x9893;

    //std::cout << "check dynamic pointer address : " << "0x" << std::hex << DBAddress << std::endl;

    ////resolve pointer chain
    //std::vector<unsigned int> pointerChainOffsets = {};
    //uintptr_t ChainAdress = FindDMAAddy(hProcess, DBAddress, pointerChainOffsets);
    //// read value from pointer
    //int ChainValue = 0;
    //ReadProcessMemory(hProcess, (BYTE*)ChainAdress, &ChainValue, sizeof(ChainValue), nullptr);

    //std::cout << "current value : " << std::dec << ChainValue << std::endl;

    //// write new value to pointer
    //int newValue = 0;
    //WriteProcessMemory(hProcess, (BYTE*)ChainAdress, &newValue, sizeof(newValue), nullptr);

    //getchar();
    //return 0;

    HANDLE hProcess = 0;

    uintptr_t moduleBase = 0, playerAddr = 0, hPaddr = 0;
    bool health = false, bAmmo = false, bRecoil = false;

    const int newValue = 6969;

    DWORD procId = GetProcId(L"idname");

    if (procId){
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
        moduleBase = GetModuleBaseAddress(procId, L"module name");
        playerAddr = moduleBase + 0x4;
        hPaddr = FindDMAAddy(hProcess, playerAddr, { 0xf8 });
    }
    else {
        std::cout << "process not found\n";
        getchar();
        return;
    }
    DWORD dwExit = 0;

    while (GetExitCodeProcess(hProcess,&dwExit) && dwExit == STILL_ACTIVE){
        if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
            health = !health;
        }
        if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
            bAmmo = !bAmmo;
            if (bAmmo)
            {
                // ff 06 = inc [esi];
                mem::patchEx((BYTE*)(moduleBase + 0x483094), (BYTE*)"\xFF\0x06", 2, hProcess);
            } else {
                // ff 0E = dec [esi];
                mem::patchEx((BYTE*)(moduleBase + 0x92293), (BYTE*)"\xFF\0x0E", 2, hProcess);
            }
        }
        if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
            bRecoil = !bRecoil;
            if (bRecoil){
                mem::Nope((BYTE*)(moduleBase + 0x8239823), 10, hProcess);
            } else {
                // recoil bytes array, the array of instruction we want to edit
                mem::patchEx((BYTE*)(moduleBase + 0x82), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2",10,hProcess);
            }
        }
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            return 0;
        }

        if (health){
            mem::patchEx((BYTE*)(hPaddr), (BYTE*)&newValue, sizeof(newValue), hProcess);
        }

        Sleep(10);
    }

    std::cout << "process not found, enter to exit\n";
    getchar();
    return 0;
}