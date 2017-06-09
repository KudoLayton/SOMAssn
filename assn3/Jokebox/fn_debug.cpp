#include "fn_debug.h"
#include <Windows.h>
#include <sstream>
#include "fn_engine.h"

using std::string;

void _debug_raise_error(const char* err_file, const int err_line,
	std::string err_content, const char* err_fn, std::string err_add)
{
	std::stringstream ss;
	ss << err_line;
	string err_line_s = ss.str();

	string err_file_s = string(err_file);
	int tok = err_file_s.find_last_of('\\');
	if (tok != string::npos)
		err_file_s = err_file_s.substr(tok + 1);

	string msg;

	msg = "게임 실행 도중에 예외가 발생하였습니다: ";
	msg += err_file_s + ", line " + err_line_s + "\n\n";
	msg += "발생 원인: " + err_content + "\n";
	msg += "추가 정보: " + string(err_fn) + "을(를) 실행 중 에러 발생.";

	if (!err_add.empty())
		msg += "\n\n" + err_add;

	MessageBoxA(_engine_get_hwnd(), msg.c_str(), "Jokebox 4.04 오류", 
		MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);

	exit(0);
}