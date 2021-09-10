#include <windows.h>

struct memory_ptr {
	char* moduleName;
	uintptr_t moduleOffset;
	int offsetCount;
	uintptr_t offsets[];
};

void* GetDynamicPtr(HANDLE hProc, memory_ptr* ptr) {
	uintptr_t moduleBaseAddr = (uintptr_t)GetModuleHandle(ptr->moduleName);
	uintptr_t dynPtrBaseAddr = moduleBaseAddr + ptr->moduleOffset;

	uintptr_t dynPtr = dynPtrBaseAddr;
	for(int i = 0; i < ptr->offsetCount; ++i) {
		ReadProcessMemory(hProc, (void*)dynPtr, &dynPtr, sizeof(dynPtr), 0);
		dynPtr += ptr->offsets[i];
	}

	return (void*)dynPtr;
}

DWORD WINAPI MainThread(LPVOID param) {
	while (true) {
		if (GetAsyncKeyState(VK_F6) & 0x80000) {
			MessageBoxA(NULL, "F6 pressed!", "F6 pressed!", MB_OK);
		}
		Sleep(100);
	}

	return 0;
}

bool WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		MessageBoxA(NULL, "DLL injected!", "DLL injected!", MB_OK);
		CreateThread(0, 0, MainThread, hModule, 0, 0);
	}

	return true;
}
