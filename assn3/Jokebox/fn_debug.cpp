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

	msg = "���� ���� ���߿� ���ܰ� �߻��Ͽ����ϴ�: ";
	msg += err_file_s + ", line " + err_line_s + "\n\n";
	msg += "�߻� ����: " + err_content + "\n";
	msg += "�߰� ����: " + string(err_fn) + "��(��) ���� �� ���� �߻�.";

	if (!err_add.empty())
		msg += "\n\n" + err_add;

	MessageBoxA(_engine_get_hwnd(), msg.c_str(), "Jokebox 4.04 ����", 
		MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);

	exit(0);
}