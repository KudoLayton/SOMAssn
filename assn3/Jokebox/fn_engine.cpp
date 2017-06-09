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

#include "fn_engine.h"
#include "fn_debug.h"
#pragma comment (lib, "user32.lib")

#define DRAW_MSPF 20;

static void (*init_func)();
static void (*update_func)(GAMETIME gametime);
static void (*draw_func)(GAMETIME gametime);

static HWND _hwnd = NULL;

void _engine_loop(void* vp)
{
	int draw_cool_time = 0;

	//초기화 함수 호출
	if (init_func != NULL)
		init_func();

	while(_game_is_playing())
	{
		//Game 루프
		GAMETIME gt;

		_input_ready();		//눌린키 조사
		gt.elapsed = _time_elapsed();
		gt.total = _time_total();
		_time_set_update();

		if (update_func != NULL)
			update_func(gt);		//업데이트 함수 호출
		if (draw_func != NULL)
			draw_func(gt);			//드로우 함수 호출
	}

	exit(0);
}

bool _engine_reshape_window()
{
	RECT crect;
	RECT desktop;
	BOOL result;

	crect.left = 0;
	crect.top = 0;
	crect.right = _game_get_cell_width() * _game_get_cell_x_count();
	crect.bottom = _game_get_cell_height() * _game_get_cell_y_count();

	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&desktop, 0);
	GetWindowRect(GetDesktopWindow(), &desktop);

	result = AdjustWindowRect(&crect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);				//수정!!!
	if (!result)
		_debug_raise_error(__FILE__, __LINE__, "창 크기 조정 실패",
			"(내부 함수)");
	MoveWindow(_engine_get_hwnd(), (desktop.right - crect.right) / 2, (desktop.bottom - crect.bottom) / 2 - 20, crect.right - crect.left, crect.bottom - crect.top, FALSE);
	
	return result;
}

int _engine_run()
{
	//창의 핸들이 설정되어있지 않으면 실패를 리턴.
	if (_hwnd == NULL)
		return 0;

	//게임 초기화
	_engine_reshape_window();
	_draw_initialize(_hwnd);
	_content_initialize();

	//시간 초기화
	_time_set_start();		
	_time_set_update();

	//게임 시작
	_game_begin();

	_beginthread(_engine_loop, 0, 0);	//게임루프 스레드.

	return 1;
}

void _engine_set_init_func(void (*init)())
{
	init_func = init;	
}

void _engine_set_draw_func(void (*draw)(GAMETIME))
{
	draw_func = draw;
}

void _engine_set_update_func(void (*update)(GAMETIME))
{
	update_func = update;
}

void _engine_set_hwnd(HWND hwnd)
{
	_hwnd = hwnd;
}

HWND _engine_get_hwnd()
{
	return _hwnd;
}
