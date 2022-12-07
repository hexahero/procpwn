#ifndef PROCMGMT
#define PROCMGMT

#include <Windows.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <tlhelp32.h>
#include <tchar.h>
#include <iomanip>

struct MDLINFO {

	std::wstring	modulename;
	std::wstring	exepath;
	DWORD			procid;
	DWORD			refcountg;
	DWORD			refcountp;
	DWORD			basesize;
	UINT64			baseaddr;

};

struct PROCINFO {

	std::wstring	procname;
	DWORD			procid;
	DWORD			thrdcount;
	DWORD			parentprocid;
	LONG			priorityclassbase;
	DWORD			ispriorityclass;

	std::vector<MDLINFO> modules;

};

void print_processes_info(const std::vector<PROCINFO>& processes);
void print_modules_info(const PROCINFO& procinfo);

bool	is_running(HANDLE procHandle);
bool	kill_process(HANDLE procHandle);
DWORD	get_priority(PROCESSENTRY32& processEntry32);

bool obtain_processes_from_system(std::vector<PROCINFO>& buffer);
bool obtain_process_modules(DWORD dwProcessID, PROCINFO& procinfo);
void print_error(const TCHAR* msg);

#endif