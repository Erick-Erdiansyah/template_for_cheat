#pragma once
#include<vector>
#include<Windows.h>
#include<TlHelp32.h>

namespace mem {
	void patchEx(BYTE* dstt, BYTE* src, unsigned int size, HANDLE hProcess);
	void NopeEx(BYTE* dst, unsigned int size, HANDLE hProcess);
	uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);

	void patch(BYTE* dstt, BYTE* src, unsigned int size);
	void Nope(BYTE* dst, unsigned int size);
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
}
