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