#ifndef UTIL
#define UTIL

#include <Windows.h>
#include <time.h>
#include <string>

const char* concat_root_dir_path(const char* toConcat, int catStrLength);

std::wstring	str_to_wstr(const std::string& strIn);
std::string		wstr_to_str(const std::wstring& wstrIn);
std::string		decimal_to_string(int decval);

std::string get_timestamp(bool date = false);

#endif