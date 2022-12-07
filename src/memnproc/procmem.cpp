#include "procmem.h"

#include <Windows.h>
#include "../util/util.h"

HANDLE get_processes_snapshot() {

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	return processesSnapshot;
}

void print_processes() {

	HANDLE processesSnapshot = get_processes_snapshot();

	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	if (processesSnapshot == 0)
		return;

	unsigned int ctr = 0;

	
	Process32First(processesSnapshot, &processInfo);
	std::wcout << processInfo.szExeFile << "\t\t" << processInfo.th32ProcessID << '\n';

	ctr++;

	while (Process32Next(processesSnapshot, &processInfo)) {

		std::wcout << processInfo.szExeFile << "\t\t" << processInfo.th32ProcessID << '\n';
		ctr++;

	}

	std::cout << "total processes: " << ctr << '\n';

	CloseHandle(processesSnapshot);

}

DWORD get_process_id(const std::wstring& processName) {

	HANDLE processesSnapshot = get_processes_snapshot();

	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	if (processesSnapshot == 0)
		return 0;

	Process32First(processesSnapshot, &processInfo);

	if (!processName.compare(processInfo.szExeFile)) {

		CloseHandle(processesSnapshot);

		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo)) {

		if (!processName.compare(processInfo.szExeFile)) {

			CloseHandle(processesSnapshot);

			return processInfo.th32ProcessID;
		}

	}

	CloseHandle(processesSnapshot);

	return 0;
}

HANDLE get_process_handle(const std::string& processName) {

	HANDLE processHandle = OpenProcess(
		PROCESS_VM_READ				|
		PROCESS_QUERY_INFORMATION	|
		PROCESS_VM_WRITE			|
		PROCESS_VM_OPERATION		|
		PROCESS_TERMINATE,
		FALSE,
		get_process_id(str_to_wstr(processName))
	);

	if (!processHandle) {
		std::cout << "error acquiring process handle\n";
		return NULL;
	}

	return processHandle;
}

HANDLE get_process_handle_w(const std::string& windowName) {

	HWND windowHandle = FindWindowA(0, windowName.c_str());

	if (windowHandle == 0) {
		std::cout << "error acquiring window handle\n";
		return NULL;
	}

	DWORD processID;

	GetWindowThreadProcessId(windowHandle, &processID);

	HANDLE processHandle = OpenProcess(
		PROCESS_VM_READ				|
		PROCESS_QUERY_INFORMATION	|
		PROCESS_VM_WRITE			|
		PROCESS_VM_OPERATION		|
		PROCESS_TERMINATE,
		FALSE,
		processID
	);

	if (!processHandle) {
		std::cout << "error acquiring process handle\n";
		return NULL;
	}

	return processHandle;
}

bool read_process_memory(HANDLE prochandle, long long memaddr, void* buff, unsigned int buffsize) {

	if (!ReadProcessMemory(prochandle, (LPVOID)memaddr, buff, (DWORD)buffsize, NULL)) {
		std::cout << "error reading process memory\n";
		return false;
	}

	return true;
}

bool write_process_memory(HANDLE prochandle, long long memaddr, void* buff, unsigned int buffsize) {

	if (!WriteProcessMemory(prochandle, (LPVOID)memaddr, buff, (DWORD)buffsize, NULL)) {
		std::cout << "error writing process memory\n";
		return false;
	}

	return true;
}