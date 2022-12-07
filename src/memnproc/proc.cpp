#include "proc.h"

//#define PROC_MODULE_DEBUG

void print_processes_info(const std::vector<PROCINFO>& processes) {

	std::cout << "\n";

	for (const auto& procinfo : processes) {

		std::wcout
			<< "----------------------------------------------------------------\n"
			<< "   PROCESS NAME:       " << procinfo.procname << '\n'
			<< "   ID:                 " << procinfo.procid << '\n'
			<< "   Thread count:       " << procinfo.thrdcount << '\n'
			<< "   Parent process ID:  " << procinfo.parentprocid << '\n'
			<< "   Priority base:      " << procinfo.priorityclassbase << '\n'
			<< "   Priority class:     " << procinfo.priorityclassbase << '\n'
			<< "----------------------------------------------------------------\n";

	}

	std::cout << "Total processes: " << processes.size() << '\n';

}

void print_modules_info(const PROCINFO& procinfo) {

	std::cout << "------------------------------------------------------------------\n";

	for (const auto& mdlinfo : procinfo.modules) {

		std::wcout << std::dec
			<< "\n"
			<< "   MODULE NAME:      "		<< mdlinfo.modulename				<< '\n'
			<< "   EXECUTABLE PATH:  "		<< mdlinfo.exepath					<< '\n'
			<< "   ID:               "		<< mdlinfo.procid					<< '\n'
			<< "   Refcount (g):     "		<< mdlinfo.refcountg				<< '\n'
			<< "   Refcount (p):     "		<< mdlinfo.refcountp				<< '\n'
			<< "   Base size:        "		<< mdlinfo.basesize					<< '\n'
			<< "   Base address:     0x"	<< std::hex << mdlinfo.baseaddr		<< '\n'
			<< "\n";

	}

	std::cout << "Total modlues: " << procinfo.modules.size() << '\n';
	std::cout << "------------------------------------------------------------------\n";

}

bool is_running(HANDLE procHandle) {

	DWORD exitCode = 0;

	GetExitCodeProcess(procHandle, &exitCode);

	return static_cast<DWORD>(exitCode) == STILL_ACTIVE;
}

bool kill_process(HANDLE procHandle) {

	bool result = TerminateProcess(procHandle, 1);

	CloseHandle(procHandle);

	return result;
}

DWORD get_priority(PROCESSENTRY32& processEntry32) {

	HANDLE	processHandle;
	DWORD	dwPriorityClass = 0;

	processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntry32.th32ProcessID);

	if (processHandle == NULL) {
	
		#ifdef PROC_MODULE_DEBUG
			print_error(TEXT("OpenProcess"));
		#endif

		return dwPriorityClass;
	}
	else {

		dwPriorityClass = GetPriorityClass(processHandle);

		if (!dwPriorityClass) {

			#ifdef PROC_MODULE_DEBUG
				print_error(TEXT("GetPriorityClass"));
			#endif

		}

		CloseHandle(processHandle);

	}

	return dwPriorityClass;
}

bool obtain_processes_from_system(std::vector<PROCINFO>& buffer) {

	buffer.clear();

	HANDLE			processSnapshotHandle;
	PROCESSENTRY32	processEntry32;

	processEntry32.dwSize = sizeof(PROCESSENTRY32);
	processSnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (processSnapshotHandle == INVALID_HANDLE_VALUE) {

		#ifdef PROC_MODULE_DEBUG
			print_error(TEXT("CreateToolhelp32Snapshot (of processes)"));
		#endif

		return(FALSE);
	}

	if (!Process32First(processSnapshotHandle, &processEntry32)) {

		#ifdef PROC_MODULE_DEBUG
			print_error(TEXT("Process32First"));
		#endif
			
		CloseHandle(processSnapshotHandle);

		return(FALSE);
	}

	do {

		buffer.push_back({

			processEntry32.szExeFile,
			processEntry32.th32ProcessID,
			processEntry32.cntThreads,
			processEntry32.th32ParentProcessID,
			processEntry32.pcPriClassBase,
			get_priority(processEntry32)

		});

	} while (Process32Next(processSnapshotHandle, &processEntry32));

	CloseHandle(processSnapshotHandle);

	std::sort(
		buffer.begin(), buffer.end(),
		[](const auto& left, const auto& right) {
			return left.procname < right.procname;
		}
	);

	return(TRUE);
}


bool obtain_process_modules(DWORD dwProcessID, PROCINFO& procinfo) {

	HANDLE			moduleSnapshotHandle = INVALID_HANDLE_VALUE;
	MODULEENTRY32	moduleEntry32;

	moduleEntry32.dwSize = sizeof(MODULEENTRY32);
	moduleSnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);

	if (moduleSnapshotHandle == INVALID_HANDLE_VALUE) {

		#ifdef PROC_MODULE_DEBUG
			print_error(TEXT("CreateToolhelp32Snapshot (of modules)"));
		#endif

		return(FALSE);
	}

	if (!Module32First(moduleSnapshotHandle, &moduleEntry32)) {

		#ifdef PROC_MODULE_DEBUG
			print_error(TEXT("Module32First"));
		#endif

		CloseHandle(moduleSnapshotHandle);

		return(FALSE);
	}

	do {

		procinfo.modules.push_back({

			moduleEntry32.szModule,
			moduleEntry32.szExePath,
			moduleEntry32.th32ProcessID,
			moduleEntry32.GlblcntUsage,
			moduleEntry32.ProccntUsage,
			moduleEntry32.modBaseSize,
			reinterpret_cast<UINT64>(moduleEntry32.modBaseAddr)

		});

	} while (Module32Next(moduleSnapshotHandle, &moduleEntry32));

	CloseHandle(moduleSnapshotHandle);

	return(TRUE);
}

void print_error(const TCHAR* msg) {

	DWORD eNum;
	TCHAR sysmsg[256];
	TCHAR* ptr;

	eNum = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		sysmsg, 256, NULL
	);

	ptr = sysmsg;

	while ((*ptr > 31) || (*ptr == 9)) ++ptr;

	do {
		*ptr-- = 0;
	} while (
		(ptr >= sysmsg) && 
		((*ptr == '.') || (*ptr < 33))
	);

	_tprintf(TEXT("WARNING: %s failed with error %d (%s)\n"), msg, eNum, sysmsg);

}