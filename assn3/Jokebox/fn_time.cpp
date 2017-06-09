/*----------------------------------------------------------*
 *															*
 *					   Jokebox v4.051						*
 *															*
 * 				�̱����� ���� �ۼ��� Jokebox��				*
 *		ũ������Ƽ�� Ŀ���� ������ǥ��-�������Ǻ������		*
 * 		3.0 Unported ���̼����� ���� �̿��� �� �ֽ��ϴ�.	*
 *															*
 *					ORANGEYELLOW, 2013.						*
 *					iss300@postech.ac.kr					*
 *															*
 *----------------------------------------------------------*/

#include "Windows.h"
#include "fn_time.h"

unsigned __int64 _game_start_time = 0;
unsigned __int64 _last_update_time = 0;

//���� �ð��� ms ������ �޾ƿ�.
unsigned __int64 _time_current()
{
	FILETIME ftime;
	unsigned __int64 tt = 0;

	GetSystemTimeAsFileTime(&ftime);
	tt |= ftime.dwHighDateTime;
	tt <<= 32;
	tt |= ftime.dwLowDateTime;
	
	return tt / 10000;
}

//������ ������Ʈ�κ��� ���� �ð��� ms ������ ����.
unsigned long _time_elapsed()
{
	return (unsigned long)(_time_current() - _last_update_time);
}

//���� �������κ��� ���� �ð��� ms ������ ����.
unsigned long _time_total()
{
	return (unsigned long)(_time_current() - _game_start_time);
}

void _time_set_start()
{
	_game_start_time = _time_current();
}

//Update�� ������ ������Ʈ �ð� üũ.
void _time_set_update()
{
	_last_update_time = _time_current();
}