#pragma once
#include <string>

void _debug_raise_error(const char* err_file, const int err_line,
	std::string err_content, const char* err_fn, std::string err_add = std::string());