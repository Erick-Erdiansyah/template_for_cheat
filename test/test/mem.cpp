#include "mem.h"
#include<vector>
#include<Windows.h>
#include<TlHelp32.h>

void mem::patchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess) {
	DWORD oldProtect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	VirtualProtectEx(hProcess, dst, size, oldProtect, &oldProtect);
}
void mem::Nope(BYTE* dst, unsigned int size, HANDLE hProcess) {
	BYTE* nopArray = new BYTE[size];
	// 0x90 is for nop instruction
	memset(nopArray, 0x90, size);

	patchEx(dst, nopArray, size, hProcess);
	delete[] nopArray;
}
