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

#include "fn_game.h"
#include "fn_engine.h"

static int _is_playing;
static int _px_width = 14;			//기본 픽셀 크기
static int _px_height = 14;
static int _px_x_count = 40;			//기본 픽셀 개수
static int _px_y_count = 30;

static bool _grid_visible = false;

void _game_begin()
{
	_is_playing = 1;
}

void _game_exit()
{
	_is_playing = 0;
}

void _game_set_window_title(char* title)
{
	SetWindowTextA(_engine_get_hwnd(), title);

	return;
}

void _game_set_cell_size(int width, int height)
{
	_px_width = width;
	_px_height = height;

	_engine_reshape_window();
	_draw_reset_resource();
}

void _game_set_cell_count(int x, int y)
{
	_px_x_count = x;
	_px_y_count = y;

	_engine_reshape_window();
	_draw_reset_resource();
}

int _game_get_cell_x_count()
{
	return _px_x_count;
}

int _game_get_cell_y_count()
{
	return _px_y_count;
}

int _game_get_cell_width()
{
	return _px_width;
}

int _game_get_cell_height()
{
	return _px_height;
}

int _game_is_playing()
{
	return _is_playing;
}