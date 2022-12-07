#ifndef DATACNTRLLR
#define DATACNTRLLR

#define MAX_PROCESSES 100

extern LOGGER g_logger;

struct HKPROC {

	std::pair<HANDLE, PROCINFO>	_info;

	std::vector<std::pair<long long, int>> _scannedInts;
	std::vector<std::pair<long long, int>> _watchedInts;

};

class DATACONTROLLER {

public:
	
	DATACONTROLLER(const DATACONTROLLER&) = delete;
	DATACONTROLLER& operator = (const DATACONTROLLER&) = delete;

	DATACONTROLLER();

	// Processes
	std::vector<PROCINFO>& get_procs();

	bool	is_procs_empty() const;
	size_t	get_procs_size() const;

	std::string		get_proc_name(unsigned int procIndex) const;
	std::wstring	get_proc_name_wstr(unsigned int procIndex) const;

	unsigned int get_proc_id(unsigned int procIndex) const;
	unsigned int get_proc_pid(unsigned int procIndex) const;

	bool is_already_hooked(unsigned int procIndex) const;

	// Hooked processes
	std::vector<HKPROC>& get_hkprocs();

	bool	is_hkprocs_empty() const;
	size_t	get_hkprocs_size() const;

	HKPROC hkprocs_at(unsigned int hkProcIndex);
	HKPROC hkprocs_at();

	std::string get_hkproc_name(unsigned int hkProcIndex) const;
	std::string get_hkproc_name() const;

	std::wstring get_hkproc_name_wstr(unsigned int hkProcIndex) const;
	std::wstring get_hkproc_name_wstr() const;

	size_t hkproc_get_size_of_scanned_ints(unsigned int hkProcIndex) const;
	size_t hkproc_get_size_of_scanned_ints() const;

	size_t hkproc_get_size_of_watched_ints(unsigned int hkProcIndex) const;
	size_t hkproc_get_size_of_watched_ints() const;

	long long hkproc_get_scanned_int_address(unsigned int hkProcIndex, unsigned int scanedIntIndex) const;
	long long hkproc_get_scanned_int_address(unsigned int scanedIntIndex) const;

	int hkproc_get_scanned_int_value(unsigned int hkProcIndex, unsigned int scanedIntIndex) const;
	int hkproc_get_scanned_int_value(unsigned int scanedIntIndex) const;

	long long hkproc_get_watched_int_address(unsigned int hkProcIndex, unsigned int watchedIntIndex) const;
	long long hkproc_get_watched_int_address(unsigned int watchedIntIndex) const;

	int hkproc_get_watched_int_value(unsigned int hkProcIndex, unsigned int watchedIntIndex) const;
	int hkproc_get_watched_int_value(unsigned int watchedIntIndex) const;

	void hkproc_add_int_to_watch(unsigned int hkProcIndex, unsigned int scanedIntIndex);
	void hkproc_add_int_to_watch(unsigned int scanedIntIndex);

	void watch_all_scanned_addresses(unsigned int hkProcIndex);
	void watch_all_scanned_addresses();

	bool hook_process(const std::string& procname, unsigned int procIndex);

	HANDLE get_hkproc_handle(unsigned int hkProcIndex);
	HANDLE get_hkproc_handle();

	void check_active_procs();

	void			set_selected_process(unsigned int newSelectedProcIndex);
	unsigned int	get_selected_process() const;

private:

	unsigned int _hkProcessSelected = 0;

	std::vector<PROCINFO>	_processes;
	std::vector<HKPROC>		_hookedProcesses;

};

DATACONTROLLER::DATACONTROLLER() {

	_hookedProcesses.reserve(MAX_PROCESSES); // Pre-allocate with the max allowed number of hooked processes. This allows to avoid 
											 // read access violation when hooking a new process while memory of other process
											 // is being scanned in an another thread.

}

// Processes
std::vector<PROCINFO>& DATACONTROLLER::get_procs() {

	return _processes;
}

bool DATACONTROLLER::is_procs_empty() const {

	return _processes.empty();
}

size_t DATACONTROLLER::get_procs_size() const {

	return _processes.size();
}

std::string DATACONTROLLER::get_proc_name(unsigned int procIndex) const {

	return wstr_to_str(_processes.at(procIndex).procname);
}

std::wstring DATACONTROLLER::get_proc_name_wstr(unsigned int procIndex) const {

	return _processes.at(procIndex).procname;
}

unsigned int DATACONTROLLER::get_proc_id(unsigned int procIndex) const {

	return _processes.at(procIndex).procid;
}

unsigned int DATACONTROLLER::get_proc_pid(unsigned int procIndex) const {

	return _processes.at(procIndex).parentprocid;
}

bool DATACONTROLLER::is_already_hooked(unsigned int procIndex) const {

	if (std::any_of(_hookedProcesses.begin(), _hookedProcesses.end(), [=](const auto& hkproc) {
		return
			hkproc._info.second.procname == _processes.at(procIndex).procname &&
			hkproc._info.second.procid == _processes.at(procIndex).procid;
	}))
		return true;

	return false;
}

// Hooked processes
std::vector<HKPROC>& DATACONTROLLER::get_hkprocs() {

	return _hookedProcesses;
}

bool DATACONTROLLER::is_hkprocs_empty() const {

	return _hookedProcesses.empty();
}

size_t DATACONTROLLER::get_hkprocs_size() const {

	return _hookedProcesses.size();
}

HKPROC DATACONTROLLER::hkprocs_at(unsigned int hkProcIndex) {

	return _hookedProcesses.at(hkProcIndex);
}

HKPROC DATACONTROLLER::hkprocs_at() {

	return _hookedProcesses.at(_hkProcessSelected);
}

std::string DATACONTROLLER::get_hkproc_name(unsigned int hkProcIndex) const {

	return wstr_to_str(_hookedProcesses.at(hkProcIndex)._info.second.procname);
}

std::string DATACONTROLLER::get_hkproc_name() const {

	return wstr_to_str(_hookedProcesses.at(_hkProcessSelected)._info.second.procname);
}

std::wstring DATACONTROLLER::get_hkproc_name_wstr(unsigned int hkProcIndex) const {

	return _hookedProcesses.at(hkProcIndex)._info.second.procname;
}

std::wstring DATACONTROLLER::get_hkproc_name_wstr() const {

	return _hookedProcesses.at(_hkProcessSelected)._info.second.procname;
}

size_t DATACONTROLLER::hkproc_get_size_of_scanned_ints(unsigned int hkProcIndex) const {

	return _hookedProcesses.at(hkProcIndex)._scannedInts.size();
}

size_t DATACONTROLLER::hkproc_get_size_of_scanned_ints() const {

	return _hookedProcesses.at(_hkProcessSelected)._scannedInts.size();
}

size_t DATACONTROLLER::hkproc_get_size_of_watched_ints(unsigned int hkProcIndex) const {

	return _hookedProcesses.at(hkProcIndex)._watchedInts.size();
}

size_t DATACONTROLLER::hkproc_get_size_of_watched_ints() const {

	return _hookedProcesses.at(_hkProcessSelected)._watchedInts.size();
}

long long DATACONTROLLER::hkproc_get_scanned_int_address(unsigned int hkProcIndex, unsigned int scanedIntIndex) const {

	return _hookedProcesses.at(hkProcIndex)._scannedInts.at(scanedIntIndex).first;
}

long long DATACONTROLLER::hkproc_get_scanned_int_address(unsigned int scanedIntIndex) const {

	return _hookedProcesses.at(_hkProcessSelected)._scannedInts.at(scanedIntIndex).first;
}

int DATACONTROLLER::hkproc_get_scanned_int_value(unsigned int hkProcIndex, unsigned int scanedIntIndex) const {

	return _hookedProcesses.at(hkProcIndex)._scannedInts.at(scanedIntIndex).second;
}

int DATACONTROLLER::hkproc_get_scanned_int_value(unsigned int scanedIntIndex) const {

	return _hookedProcesses.at(_hkProcessSelected)._scannedInts.at(scanedIntIndex).second;
}

long long DATACONTROLLER::hkproc_get_watched_int_address(unsigned int hkProcIndex, unsigned int watchedIntIndex) const {

	return _hookedProcesses.at(hkProcIndex)._watchedInts.at(watchedIntIndex).first;
}

long long DATACONTROLLER::hkproc_get_watched_int_address(unsigned int watchedIntIndex) const {

	return _hookedProcesses.at(_hkProcessSelected)._watchedInts.at(watchedIntIndex).first;
}

int DATACONTROLLER::hkproc_get_watched_int_value(unsigned int hkProcIndex, unsigned int watchedIntIndex) const {

	return _hookedProcesses.at(hkProcIndex)._watchedInts.at(watchedIntIndex).second;
}

int DATACONTROLLER::hkproc_get_watched_int_value(unsigned int watchedIntIndex) const {

	return _hookedProcesses.at(_hkProcessSelected)._watchedInts.at(watchedIntIndex).second;
}

void DATACONTROLLER::hkproc_add_int_to_watch(unsigned int hkProcIndex, unsigned int scanedIntIndex) {

	std::pair<long long, int> tempPair = hkprocs_at(hkProcIndex)._scannedInts.at(scanedIntIndex);
	_hookedProcesses.at(hkProcIndex)._watchedInts.push_back(tempPair);

}

void DATACONTROLLER::hkproc_add_int_to_watch(unsigned int scanedIntIndex) {

	std::pair<long long, int> tempPair = hkprocs_at(_hkProcessSelected)._scannedInts.at(scanedIntIndex);
	_hookedProcesses.at(_hkProcessSelected)._watchedInts.push_back(tempPair);

}

void DATACONTROLLER::watch_all_scanned_addresses(unsigned int hkProcIndex) {

	for (const auto& scannedInt : _hookedProcesses.at(hkProcIndex)._scannedInts)
		_hookedProcesses.at(hkProcIndex)._watchedInts.push_back(scannedInt);

}

void DATACONTROLLER::watch_all_scanned_addresses() {

	for (const auto& scannedInt : _hookedProcesses.at(_hkProcessSelected)._scannedInts)
		_hookedProcesses.at(_hkProcessSelected)._watchedInts.push_back(scannedInt);

}

bool DATACONTROLLER::hook_process(const std::string& procname, unsigned int procIndex) {

	auto procHandle = get_process_handle(procname);

	if (procHandle != NULL) {

		_hookedProcesses.push_back({ { procHandle, _processes.at(procIndex) } });

		return true;
	}

	g_logger.add_log("error acquiring process handle", LOGFAILURE);
	std::cout << "could not hook process\n";

	return false;
}

HANDLE DATACONTROLLER::get_hkproc_handle(unsigned int hkProcIndex) {

	return _hookedProcesses.at(hkProcIndex)._info.first;
}

HANDLE DATACONTROLLER::get_hkproc_handle() {

	return _hookedProcesses.at(_hkProcessSelected)._info.first;
}

void DATACONTROLLER::check_active_procs() {

	if (!is_hkprocs_empty()) {

		unsigned int hkProcCtr = 0;
		auto hkproc = std::begin(_hookedProcesses);
		while (hkproc != std::end(_hookedProcesses)) {

			if (!is_running(hkproc->_info.first)) {

				CloseHandle(hkproc->_info.first);
				hkproc = _hookedProcesses.erase(hkproc);

				if (
					hkProcCtr == _hkProcessSelected && 
					_hkProcessSelected != 0			&& 
					_hookedProcesses.size() - 1 > hkProcCtr
				)
					++_hkProcessSelected;
				else if (_hkProcessSelected != 0 && _hookedProcesses.size() - 1 > hkProcCtr)
					++_hkProcessSelected;
				else if (_hkProcessSelected != 0)
					--_hkProcessSelected;

			}
			else {

				++hkproc;
				++hkProcCtr;

			}

		}

	}

}

void DATACONTROLLER::set_selected_process(unsigned int newSelectedProcIndex) {

	_hkProcessSelected = newSelectedProcIndex;
}

unsigned int DATACONTROLLER::get_selected_process() const {

	return _hkProcessSelected;
}

#endif