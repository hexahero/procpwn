#ifndef LGGR
#define LGGR

#include <vector>
#include <mutex>
#include <string>
#include <fstream>

#define LOGTYPE		ImVec4

#define LOGSUCCESS	{ 0.0f, 0.8f, 0.0f, 1.0f }
#define LOGFAILURE	{ 0.8f, 0.0f, 0.0f, 1.0f }
#define LOGWARNING	{ 0.8f, 0.8f, 0.0f, 1.0f }
#define LOGINFO		{ 1.0f, 1.0f, 1.0f, 1.0f }

class LOGGER {

public:

	LOGGER(const LOGGER&) = delete;
	LOGGER& operator = (const LOGGER&) = delete;

	LOGGER() = default;

	void add_log(const std::string& log, LOGTYPE logtype);
	std::vector<std::pair<std::string, LOGTYPE>>& get_logs();
	void dump_logs();

private:

	std::mutex mtx;
	std::vector<std::pair<std::string, LOGTYPE>> logs;

};

void LOGGER::add_log(const std::string& log, LOGTYPE logtype) {

	mtx.lock();
	logs.push_back({ get_timestamp() + ' ' + log, logtype});
	mtx.unlock();

}

std::vector<std::pair<std::string, LOGTYPE>>& LOGGER::get_logs() {

	return logs;
}

void LOGGER::dump_logs() {

	std::ofstream file;
	file.open("procpwn_log_dump.txt");

	for (const auto& log : logs)
		file << log.first + '\n';

	file.close();

	add_log("logs dumped", LOGINFO);

}

#endif 