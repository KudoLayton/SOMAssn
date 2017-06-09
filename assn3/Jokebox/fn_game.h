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

#ifndef FN_GAME
#define FN_GAME

#include <Windows.h>

void _game_begin();
void _game_exit();

void _game_set_window_title(char*);
void _game_set_cell_size(int width, int height);
void _game_set_cell_count(int x, int y);
int _game_get_cell_x_count();
int _game_get_cell_y_count();
int _game_get_cell_width();
int _game_get_cell_height();
int _game_is_playing();

#endif