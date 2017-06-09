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

#include "fn_draw.h"
#include "fn_time.h"
#include "fn_input.h"
#include "fn_game.h"
#include "fn_content.h"

#include <Windows.h>
#include <process.h>

int _engine_run();
bool _engine_reshape_window();
void _engine_loop(void* vp);

void _engine_set_init_func(void (*init_func)());
void _engine_set_draw_func(void (*draw_func)(GAMETIME));
void _engine_set_update_func(void (*update_func)(GAMETIME));
void _engine_set_hwnd(HWND hwnd);

HWND _engine_get_hwnd();