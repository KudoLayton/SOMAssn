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

#define DLL_EXPORT
#define JOKEBOX_LEVEL_A
#define JOKEBOX_LEVEL_B
#include "jokebox.h"
#include <stdarg.h>
#include <stdio.h>

/* 프레임워크에서 정상적으로 이용할 수 있는 함수의 모음 */

//Draw
void draw_begin()
{
	_draw_begin();
}

void draw_clear(COLOR color)
{
	_draw_clear(color);
}

void draw_end()
{
	_draw_end();
}

void draw_char(int word, int x, int y)
{
	_draw_char(word, x, y, WHITE, BLACK);
}

void draw_charc(int word, int x, int y, COLOR color, COLOR backcolor)
{
	_draw_char(word, x, y, color, backcolor);
}

void draw_string(int x, int y, char* string, ...)
{
	char buf[400];
	va_list args;

	va_start(args, string);
	vsprintf_s(buf, 400, string, args);
	va_end(args);

	_draw_string(buf, x, y, WHITE, BLACK);
}

void draw_stringc(int x, int y, COLOR color, COLOR backcolor, char* string, ...)
{
	char buf[400];
	va_list args;

	va_start(args, string);
	vsprintf_s(buf, 400, string, args);
	va_end(args);

	_draw_string(buf, x, y, color, backcolor);
}

void draw_area(int word, int x1, int y1, int x2, int y2)
{
	_draw_area(word, x1, y1, x2, y2, WHITE, BLACK);
}

void draw_areac(int word, int x1, int y1, int x2, int y2, COLOR color, COLOR backcolor)
{
	_draw_area(word, x1, y1, x2, y2, color, backcolor);
}

void draw_rect(float x, float y, float width, float height, COLOR color, float stroke)
{
	_draw_rect(x, y, width, height, color, stroke, 0, 0, 0);
}

void draw_rectr(float x, float y, float width, float height, COLOR color, float stroke, float centerX, float centerY, float rotation)
{
	_draw_rect(x, y, width, height, color, stroke, centerX, centerY, rotation);
}

void draw_triangle(float x, float y, float width, float height, float aoff, TRIANGLEDIR dir, COLOR color, float stroke)
{
	_draw_triangle(x, y, width, height, aoff, dir, color, stroke, 0, 0, 0);
}

void draw_triangler(float x, float y, float width, float height, float aoff, TRIANGLEDIR dir, COLOR color, float stroke, float cx, float cy, float rotation)
{
	_draw_triangle(x, y, width, height, aoff, dir, color, stroke, cx, cy, rotation);
}

void draw_polygon(float x, float y, float vertices[], int vCount, COLOR color, float stroke)
{
	_draw_polygon(x, y, vertices, vCount, color, stroke, 0, 0, 0);
}

void draw_polygonr(float x, float y, float vertices[], int vCount, COLOR color, float stroke, float cx, float cy, float rotation)
{
	_draw_polygon(x, y, vertices, vCount, color, stroke, cx, cy, rotation);
}

void draw_ellipse(float x, float y, float radX, float radY, COLOR color, float stroke)
{
	_draw_ellipse(x, y, radX, radY, color, stroke, 0, 0, 0);
}

void draw_ellipser(float x, float y, float radX, float radY, COLOR color, float stroke, float cx, float cy, float rotation)
{
	_draw_ellipse(x, y, radX, radY, color, stroke, cx, cy, rotation);
}

void draw_line(float x1, float y1, float x2, float y2, float width, COLOR color, float stroke)
{
	_draw_line(x1, y1, x2, y2, width, color, stroke);
}

void draw_text(char* text, FONT font, float x, float y, COLOR color, float stroke)
{
	_draw_text(text, font, x, y, color, stroke, 0, 0, 0);
}

void draw_textr(char* text, FONT font, float x, float y, COLOR color, float stroke, float cx, float cy, float rotation)
{
	_draw_text(text, font, x, y, color, stroke, cx, cy, rotation);
}

void draw_texture(TEXTURE texture, float x, float y, float width, float height, float opacity)
{
	_draw_texture(texture, x, y, width, height, opacity,
		0, 0, 1, 1, 0, 0, 0);
}

void draw_texturer(TEXTURE texture, float x, float y, float width, float height, float opacity,
	float cx, float cy, float rotation)
{
	_draw_texture(texture, x, y, width, height, opacity,
		0, 0, 1, 1, cx, cy, rotation);
}

void draw_texturep(TEXTURE texture, float x, float y, float width, float height, float opacity,
	float sourceX, float sourceY, float sourceWidth, float sourceHeight)
{
	_draw_texture(texture, x, y, width, height, opacity,
		sourceX, sourceY, sourceWidth, sourceHeight, 0, 0, 0);
}

void draw_texturepr(TEXTURE texture, float x, float y, float width, float height, float opacity,
	float sourceX, float sourceY, float sourceWidth, float sourceHeight,
	float cx, float cy, float rotation)
{
	_draw_texture(texture, x, y, width, height, opacity,
		sourceX, sourceY, sourceWidth, sourceHeight,
		cx, cy, rotation);
}

//Sound
void sound_fx_play(SOUND sound)
{
	sound_play(sound, 1, 0);
}

void sound_play(SOUND sound, int rewind, int loopEnabled)
{
	_sound_play(sound, rewind, loopEnabled);
}

void sound_pause(SOUND sound)
{
	_sound_pause(sound);
}

void sound_stop(SOUND sound)
{
	_sound_stop(sound);
}

int sound_is_playing(SOUND sound)
{
	return _sound_is_playing(sound);
}

/* Deprecated */
//void sound_bgm_play(SOUND sound, int loopEnabled)
//{
//	_sound_bgm_play(sound, loopEnabled);
//}
//
//void sound_bgm_stop()
//{
//	_sound_bgm_stop();
//}
//
//int sound_is_bgm_playing()
//{
//	return _sound_is_bgm_playing();
//}

//Content

FONT content_load_font(char* name, float weight, float pxSize)
{
	return _content_load_font(name, weight, pxSize);
}

TEXTURE content_load_texture(char* name)
{
	return _content_load_texture(name);
}

SOUND content_load_sound(char* name)
{
	return _content_load_sound(name);
}


//Keyboard
int keybd_is_key_down(KEYS keycode)
{
	return _keybd_is_key_down(keycode);
}
int keybd_is_key_pressed(KEYS keycode)
{
	return _keybd_is_key_pressed(keycode);
}

int keybd_is_key_up(KEYS keycode)
{
	return _keybd_is_key_up(keycode);
}

int keybd_is_key_released(KEYS keycode)
{
	return _keybd_is_key_released(keycode);
}


//Mouse
int mouse_is_btn_down(MOUSEBUTTON button)
{
	return _mouse_is_btn_down(button);
}

int mouse_is_btn_pressed(MOUSEBUTTON button)
{
	return _mouse_is_btn_pressed(button);
}

int mouse_is_btn_up(MOUSEBUTTON button)
{
	return _mouse_is_btn_up(button);
}

int mouse_is_btn_released(MOUSEBUTTON button)
{
	return _mouse_is_btn_released(button);
}

float mouse_get_x()
{
	return _mouse_get_x();
}

float mouse_get_y()
{
	return _mouse_get_y();
}


//Game
void game_exit()
{
	_game_exit();
}

void game_set_window_title(char* title)
{
	_game_set_window_title(title);
}

void game_set_cell_size(int width, int height)
{
	_game_set_cell_size(width, height);
}

void game_set_cell_count(int x, int y)
{
	_game_set_cell_count(x, y);
}

	
int game_get_cell_x_count()
{
	return _game_get_cell_x_count();
}

int game_get_cell_y_count()
{
	return _game_get_cell_y_count();
}
	
int game_get_cell_width()
{
	return _game_get_cell_width();
}

int game_get_cell_height()
{
	return _game_get_cell_height();
}

int game_is_playing()
{
	return _game_is_playing();
}

//Engine: 엔진 구동에 필요한 함수.
void engine_set_init_func(void (*init_func)())
{
	_engine_set_init_func(init_func);
}

void engine_set_draw_func(void (*draw_func)(GAMETIME))
{
	_engine_set_draw_func(draw_func);
}

void engine_set_update_func(void (*update_func)(GAMETIME))
{
	_engine_set_update_func(update_func);
}

void engine_set_hwnd(HWND hwnd)
{
	_engine_set_hwnd(hwnd);
}

int engine_run()
{
	return _engine_run();
}