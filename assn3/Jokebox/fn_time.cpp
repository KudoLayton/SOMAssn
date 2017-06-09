/*----------------------------------------------------------*
 *															*
 *					   Jokebox v4.051						*
 *															*
 * 				이광무에 의해 작성된 Jokebox는				*
 *		크리에이티브 커먼즈 저작자표시-동일조건변경허락		*
 * 		3.0 Unported 라이선스에 따라 이용할 수 있습니다.	*
 *															*
 *					ORANGEYELLOW, 2013.						*
 *					iss300@postech.ac.kr					*
 *															*
 *----------------------------------------------------------*/

#include "Windows.h"
#include "fn_time.h"

unsigned __int64 _game_start_time = 0;
unsigned __int64 _last_update_time = 0;

//현재 시간을 ms 단위로 받아옴.
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

//마지막 업데이트로부터 지난 시간을 ms 단위로 전달.
unsigned long _time_elapsed()
{
	return (unsigned long)(_time_current() - _last_update_time);
}

//게임 시작으로부터 지난 시간을 ms 단위로 전달.
unsigned long _time_total()
{
	return (unsigned long)(_time_current() - _game_start_time);
}

void _time_set_start()
{
	_game_start_time = _time_current();
}

//Update시 마지막 업데이트 시간 체크.
void _time_set_update()
{
	_last_update_time = _time_current();
}