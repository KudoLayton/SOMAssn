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

#ifndef FN_TIME
#define FN_TIME
	
typedef struct __declspec(dllexport) Gametime
{
	unsigned long elapsed;
	unsigned long total;
}GAMETIME;

unsigned long long _time_current();

//마지막 업데이트로부터 지난 시간을 ms 단위로 전달.
unsigned long _time_elapsed();

//게임 시작으로부터 지난 시간을 ms 단위로 전달.
unsigned long _time_total();

void _time_set_start();

//Update시 마지막 업데이트 시간 체크.
void _time_set_update();

#endif
