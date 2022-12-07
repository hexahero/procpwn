#ifndef PROCMEMMGMT
#define PROCMEMMGMT

#include <Windows.h>
#include <iostream>
#include <psapi.h>
#include <TlHelp32.h>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>

HANDLE	get_processes_snapshot();
void	print_processes();

DWORD	get_process_id(const std::wstring& processName);
HANDLE	get_process_handle(const std::string& processName);
HANDLE	get_process_handle_w(const std::string& windowName);

template<typename T>
int find_val_index(std::vector<char>& buffer, T valtofind);
template<typename T>
void scan_process_memory(
	HANDLE prochandle, 
	std::vector<std::pair<long long, T>>& foundvals, 
	T valtofind, 
	std::function<void()> doneCallback = nullptr
);

bool read_process_memory(HANDLE prochandle, long long memaddr, void* buff, unsigned int buffsize);
bool write_process_memory(HANDLE prochandle, long long memaddr, void* buff, unsigned int buffsize);

template<typename T>
int find_val_index(std::vector<char>& buffer, T valtofind) {

	constexpr unsigned int valToFindSize = sizeof(valtofind);
	char value[valToFindSize]{ 0 };
	char* valToFindPtr = (char*)&valtofind;

	for (int i = 0; i < valToFindSize; i++)
		value[i] = valToFindPtr[i];

	std::vector<char>::iterator iterator;
	iterator = std::search(buffer.begin(), buffer.end(), value, value + valToFindSize);

	if (iterator == buffer.end())
		return -1;

	return static_cast<int>(iterator - buffer.begin());
}

template <typename T>
void scan_process_memory(HANDLE prochandle, std::vector<std::pair<long long, T>>& foundvals, T valtofind, std::function<void()> doneCallback) {

	foundvals.clear();

	MEMORY_BASIC_INFORMATION info{ 0 };
	char* currentPageBaseAddress;

	for (
		currentPageBaseAddress = static_cast<char*>(info.BaseAddress);
		VirtualQueryEx(prochandle, currentPageBaseAddress, &info, sizeof(info)) == sizeof(info);
		currentPageBaseAddress += info.RegionSize
	) {

		if (info.State == MEM_COMMIT && info.Protect != PAGE_NOACCESS) {

			std::vector<char> buffer;
			SIZE_T bytesRead = 0;

			buffer.resize(info.RegionSize);
			ReadProcessMemory(prochandle, currentPageBaseAddress, &buffer[0], info.RegionSize, &bytesRead);
			buffer.resize(bytesRead);

			int index = find_val_index(buffer, valtofind);

			if (index >= 0)
				foundvals.push_back({ reinterpret_cast<long long>(currentPageBaseAddress + index), valtofind });

		}

	}

	if (doneCallback != nullptr)
		doneCallback();

	std::cout << "scanning finished\n";

}

#endif