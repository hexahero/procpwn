#include "util.h"

char rawPath[MAX_PATH];

const char* concat_root_dir_path(const char* toConcat, int catStrLength) {

	GetModuleFileNameA(NULL, rawPath, MAX_PATH);

	char* pathPtr = rawPath;
	char* binDirPtr = strstr(rawPath, "bin");

	for (int i = 0; pathPtr[i] != '\0'; i++) {

		if (pathPtr[i] == '\\') pathPtr[i] = '/';
	}

	if (binDirPtr != NULL) {

		for (int i = 0; i != catStrLength; i++)
			binDirPtr[i] = toConcat[i];

	}
	else
		return "invalid_path";

	return rawPath;
}

std::wstring str_to_wstr(const std::string& strIn) {

	if (strIn.empty())
		return std::wstring();

	int size = MultiByteToWideChar(CP_UTF8, 0, &strIn[0], static_cast<int>(strIn.size()), NULL, 0);

	std::wstring wstrOut(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, &strIn[0], static_cast<int>(strIn.size()), &wstrOut[0], size);

	return wstrOut;
}

std::string wstr_to_str(const std::wstring& wstrIn) {

	if (wstrIn.empty())
		return std::string();

	int size = WideCharToMultiByte(CP_UTF8, 0, &wstrIn[0], static_cast<int>(wstrIn.size()), NULL, 0, NULL, NULL);

	std::string strOut(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstrIn[0], static_cast<int>(wstrIn.size()), &strOut[0], size, NULL, NULL);

	return strOut;
}

std::string decimal_to_string(int decval) {

	char* decvalPtr = (char*)&decval;
	std::string asciiRep;

	for (unsigned int i = 0; i != sizeof(decval); i++) {

		if (decvalPtr[i] > 31 && decvalPtr[i] < 127)
			asciiRep.push_back(decvalPtr[i]);
		else
			asciiRep += " . ";

	}

	return asciiRep;
}

std::string get_timestamp(bool date) {

	char buffer[80];
	struct tm newtime = { 0 };
	time_t now = time(NULL);

	localtime_s(&newtime, &now);

	if (date)
		strftime(buffer, 80, "[%Y-%m-%d %H:%M:%S]", &newtime);
	else
		strftime(buffer, 80, "[%H:%M:%S]", &newtime);

	return std::string(buffer);
}