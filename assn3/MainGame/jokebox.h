/*----------------------------------------------------------*
 *															*
 *					   Jokebox v4.05						*
 *															*
 * 				이광무에 의해 작성된 Jokebox는				*
 *		크리에이티브 커먼즈 저작자표시-동일조건변경허락		*
 * 		3.0 Unported 라이선스에 따라 이용할 수 있습니다.	*
 *															*
 *					ORANGEYELLOW, 2013.						*
 *					iss300@postech.ac.kr					*
 *															*
 *----------------------------------------------------------*/

#pragma once

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC 
#endif

#ifdef DLL_EXPORT
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

#ifdef DLL_EXPORT
#include "fn_input.h"
#include "fn_draw.h"
#include "fn_sound.h"
#include "fn_time.h"
#include "fn_game.h"
#include "fn_engine.h"
#include "fn_content.h"
#endif
#include <Windows.h>
#include <stdarg.h>

/* 프레임워크에서 정상적으로 이용할 수 있는 함수의 모음 */

//구조체, enum
#ifndef DLL_EXPORT
typedef struct __declspec(dllimport) Gametime
{
	unsigned long elapsed;
	unsigned long total;
}GAMETIME;

typedef enum __declspec(dllimport) Keys {
	KEY_ENTER	= 0x0D,	KEY_ESC		= 0x1B, KEY_SPACE	= 0x20,
	KEY_SHIFT	= 0x10, KEY_CTRL	= 0x11, KEY_ALT		= 0x12,
	KEY_RSHIFT	= 0xA0, KEY_RCTRL	= 0xA2, KEY_RALT	= 0xA4,
	KEY_LSHIFT	= 0xA1, KEY_LCTRL	= 0xA3, KEY_LALT	= 0xA5,
	KEY_LEFT	= 0x25, KEY_UP		= 0x26, KEY_RIGHT	= 0x27, KEY_DOWN	= 0x28,
	KEY_0		= 0x30, 
	KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
	KEY_A		= 0x41,
	KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I,
	KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q,
	KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
	KEY_TAB		= 0x09,
	KEY_NUMPAD0	= 0x60,
	KEY_NUMPAD1, KEY_NUMPAD2, KEY_NUMPAD3, KEY_NUMPAD4, KEY_NUMPAD5,
	KEY_NUMPAD6, KEY_NUMPAD7, KEY_NUMPAD8, KEY_NUMPAD9,
	KEY_ADD		= 0x6B,	KEY_SUB		= 0x6D, KEY_MUL		= 0x6A, KEY_DIV		= 0x6F,
	KEY_F1		= 0x70,
	KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7,
	KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12
}KEYS;

typedef enum _declspec(dllimport) MouseButton
{
	MOUSE_LEFT = 0,
	MOUSE_RIGHT = 1
}MOUSEBUTTON;

#ifndef RGB
#define RGB(r, g, b) ((r) << 16 | (g) << 8 | (b) | (0xFF) << 24)
#endif
#define RGBA(r, g, b, a) ((r) << 16 | (g) << 8 | (b) | (a) << 24)
typedef enum Colors
{
	ALICE_BLUE = 0xFFF0F8FF,
	ANTIQUE_WHITE = 0xFFFAEBD7,
	AQUA = 0xFF00FFFF,
	AQUAMARINE = 0xFF7FFFD4,
	AZURE = 0xFFF0FFFF,
	BEIGE = 0xFFF5F5DC,
	BISQUE = 0xFFFFE4C4,
	BLACK = 0xFF000000,
	BLANCHED_ALMOND = 0xFFFFEBCD,
	BLUE = 0xFF0000FF,
	BLUE_VIOLET = 0xFF8A2BE2,
	BROWN = 0xFFA52A2A,
	BURLY_WOOD = 0xFFDEB887,
	CADET_BLUE = 0xFF5F9EA0,
	CHARTREUSE = 0xFF7FFF00,
	CHOCOLATE = 0xFFD2691E,
	CORAL = 0xFFFF7F50,
	CORNFLOWER_BLUE = 0xFF6495ED,
	CORNSILK = 0xFFFFF8DC,
	CRIMSON = 0xFFDC143C,
	CYAN = 0xFF00FFFF,
	DARK_BLUE = 0xFF00008B,
	DARK_CYAN = 0xFF008B8B,
	DARK_GOLDENROD = 0xFFB8860B,
	DARK_GRAY = 0xFFA9A9A9,
	DARK_GREEN = 0xFF006400,
	DARK_KHAKI = 0xFFBDB76B,
	DARK_MAGENTA = 0xFF8B008B,
	DARK_OLIVE_GREEN = 0xFF556B2F,
	DARK_ORANGE = 0xFFFF8C00,
	DARK_ORCHID = 0xFF9932CC,
	DARK_RED = 0xFF8B0000,
	DARK_SALMON = 0xFFE9967A,
	DARK_SEA_GREEN = 0xFF8FBC8F,
	DARK_SLATE_BLUE = 0xFF483D8B,
	DARK_SLATE_GRAY = 0xFF2F4F4F,
	DARK_TURQUOISE = 0xFF00CED1,
	DARK_VIOLET = 0xFF9400D3,
	DEEP_PINK = 0xFFFF1493,
	DEEP_SKY_BLUE = 0xFF00BFFF,
	DIM_GRAY = 0xFF696969,
	DODGER_BLUE = 0xFF1E90FF,
	FIREBRICK = 0xFFB22222,
	FLORAL_WHITE = 0xFFFFFAF0,
	FOREST_GREEN = 0xFF228B22,
	FUCHSIA = 0xFFFF00FF,
	GAINSBORO = 0xFFDCDCDC,
	GHOST_WHITE = 0xFFF8F8FF,
	GOLD = 0xFFFFD700,
	GOLDENROD = 0xFFDAA520,
	GRAY = 0xFF808080,
	GREEN = 0xFF008000,
	GREEN_YELLOW = 0xFFADFF2F,
	HONEYDEW = 0xFFF0FFF0,
	HOT_PINK = 0xFFFF69B4,
	INDIAN_RED = 0xFFCD5C5C,
	INDIGO = 0xFF4B0082,
	IVORY = 0xFFFFFFF0,
	KHAKI = 0xFFF0E68C,
	LAVENDER = 0xFFE6E6FA,
	LAVENDER_BLUSH = 0xFFFFF0F5,
	LAWN_GREEN = 0xFF7CFC00,
	LEMON_CHIFFON = 0xFFFFFACD,
	LIGHT_BLUE = 0xFFADD8E6,
	LIGHT_CORAL = 0xFFF08080,
	LIGHT_CYAN = 0xFFE0FFFF,
	LIGHT_GOLDENROD_YELLOW = 0xFFFAFAD2,
	LIGHT_GREEN = 0xFF90EE90,
	LIGHT_GRAY = 0xFFD3D3D3,
	LIGHT_PINK = 0xFFFFB6C1,
	LIGHT_SALMON = 0xFFFFA07A,
	LIGHT_SEA_GREEN = 0xFF20B2AA,
	LIGHT_SKY_BLUE = 0xFF87CEFA,
	LIGHT_SLATE_GRAY = 0xFF778899,
	LIGHT_STEEL_BLUE = 0xFFB0C4DE,
	LIGHT_YELLOW = 0xFFFFFFE0,
	LIME = 0xFF00FF00,
	LIME_GREEN = 0xFF32CD32,
	LINEN = 0xFFFAF0E6,
	MAGENTA = 0xFFFF00FF,
	MAROON = 0xFF800000,
	MEDIUM_AQUAMARINE = 0xFF66CDAA,
	MEDIUM_BLUE = 0xFF0000CD,
	MEDIUM_ORCHID = 0xFFBA55D3,
	MEDIUM_PURPLE = 0xFF9370DB,
	MEDIUM_SEA_GREEN = 0xFF3CB371,
	MEDIUM_SLATE_BLUE = 0xFF7B68EE,
	MEDIUM_SPRING_GREEN = 0xFF00FA9A,
	MEDIUM_TURQUOISE = 0xFF48D1CC,
	MEDIUM_VIOLET_RED = 0xFFC71585,
	MIDNIGHT_BLUE = 0xFF191970,
	MINT_CREAM = 0xFFF5FFFA,
	MISTY_ROSE = 0xFFFFE4E1,
	MOCCASIN = 0xFFFFE4B5,
	NAVAJO_WHITE = 0xFFFFDEAD,
	NAVY = 0xFF000080,
	OLD_LACE = 0xFFFDF5E6,
	OLIVE = 0xFF808000,
	OLIVE_DRAB = 0xFF6B8E23,
	ORANGE = 0xFFFFA500,
	ORANGE_RED = 0xFFFF4500,
	ORCHID = 0xFFDA70D6,
	PALE_GOLDENROD = 0xFFEEE8AA,
	PALE_GREEN = 0xFF98FB98,
	PALE_TURQUOISE = 0xFFAFEEEE,
	PALE_VIOLET_RED = 0xFFDB7093,
	PAPAYA_WHIP = 0xFFFFEFD5,
	PEACH_PUFF = 0xFFFFDAB9,
	PERU = 0xFFCD853F,
	PINK = 0xFFFFC0CB,
	PLUM = 0xFFDDA0DD,
	POWDER_BLUE = 0xFFB0E0E6,
	PURPLE = 0xFF800080,
	RED = 0xFFFF0000,
	ROSY_BROWN = 0xFFBC8F8F,
	ROYAL_BLUE = 0xFF4169E1,
	SADDLE_BROWN = 0xFF8B4513,
	SALMON = 0xFFFA8072,
	SANDY_BROWN = 0xFFF4A460,
	SEA_GREEN = 0xFF2E8B57,
	SEA_SHELL = 0xFFFFF5EE,
	SIENNA = 0xFFA0522D,
	SILVER = 0xFFC0C0C0,
	SKY_BLUE = 0xFF87CEEB,
	SLATE_BLUE = 0xFF6A5ACD,
	SLATE_GRAY = 0xFF708090,
	SNOW = 0xFFFFFAFA,
	SPRING_GREEN = 0xFF00FF7F,
	STEEL_BLUE = 0xFF4682B4,
	TAN = 0xFFD2B48C,
	TEAL = 0xFF008080,
	THISTLE = 0xFFD8BFD8,
	TOMATO = 0xFFFF6347,
	TURQUOISE = 0xFF40E0D0,
	VIOLET = 0xFFEE82EE,
	WHEAT = 0xFFF5DEB3,
	WHITE = 0xFFFFFFFF,
	WHITE_SMOKE = 0xFFF5F5F5,
	YELLOW = 0xFFFFFF00,
	YELLOW_GREEN = 0xFF9ACD32,
	TRANS = 0x00000000
}COLORS;

typedef enum Angle
{
	ANGLE_ZERO		= 0,
	ANGLE_CCW45		= 45,
	ANGLE_CCW90		= 90,
	ANGLE_CCW135	= 135,
	ANGLE_REV		= 180,
	ANGLE_CW135		= 225,
	ANGLE_CW90		= 270,
	ANGLE_CW45		= 315,
}ANGLE;

typedef enum TriangleDir
{
	TDIR_UP			= 0,
	TDIR_RIGHT		= 1,
	TDIR_DOWN		= 2,
	TDIR_LEFT		= 3,
}TRIANGLEDIR;

typedef unsigned int COLOR;

typedef unsigned int FONT;
typedef unsigned int TEXTURE;
typedef unsigned int SOUND;
#endif


//Draw
//(공통) 그리기를 시작합니다.
EXTERNC DLLAPI void draw_begin();
//(공통) 창을 지정한 색으로 지웁니다.
EXTERNC DLLAPI void draw_clear(COLOR color);
//(공통) 그리기를 마칩니다.
EXTERNC DLLAPI void draw_end();

#ifdef JOKEBOX_LEVEL_A
//(Level A) 문자 하나를 출력합니다.
EXTERNC DLLAPI void draw_char(int word, int x, int y);
//(Level A) 문자 하나를 출력합니다.
EXTERNC DLLAPI void draw_charc(int word, int x, int y, COLOR color, COLOR backcolor);
//(Level A) 문자열을 출력합니다.
EXTERNC DLLAPI void draw_string(int x, int y, char* string, ...);
//(Level A) 문자열을 출력합니다.
EXTERNC DLLAPI void draw_stringc(int x, int y, COLOR color, COLOR backcolor, char* string, ...);
//(Level A) 지정한 사각형 영역을 채웁니다.
EXTERNC DLLAPI void draw_area(int word, int x1, int y1, int x2, int y2);
//(Level A) 지정한 사각형 영역을 채웁니다.
EXTERNC DLLAPI void draw_areac(int word, int x1, int y1, int x2, int y2, COLOR color, COLOR backcolor);
#endif
#ifdef JOKEBOX_LEVEL_B
//(Level B) 사각형을 그립니다.
EXTERNC DLLAPI void draw_rect(float x, float y, float width, float height, COLOR color, float stroke);
//(Level B) 사각형을 그립니다.
EXTERNC DLLAPI void draw_rectr(float x, float y, float width, float height, COLOR color, float stroke, float cx, float cy, float rotation);
//(Level B) 삼각형을 그립니다.
EXTERNC DLLAPI void draw_triangle(float x, float y, float width, float height, float aoff, TRIANGLEDIR dir, COLOR color, float stroke);
//(Level B) 삼각형을 그립니다.
EXTERNC DLLAPI void draw_triangler(float x, float y, float width, float height, float aoff, TRIANGLEDIR dir, COLOR color, float stroke, float cx, float cy, float rotation);
//(Level B) 지정한 꼭지점을 이어 다각형을 그립니다.
EXTERNC DLLAPI void draw_polygon(float x, float y, float vertices[], int vCount, COLOR color, float stroke);
//(Level B) 지정한 꼭지점을 이어 다각형을 그립니다.
EXTERNC DLLAPI void draw_polygonr(float x, float y, float vertices[], int vCount, COLOR color, float stroke, float cx, float cy, float rotation);
//(Level B) 타원을 그립니다.
EXTERNC DLLAPI void draw_ellipse(float x, float y, float radX, float radY, COLOR color, float stroke);
//(Level B) 타원을 그립니다.
EXTERNC DLLAPI void draw_ellipser(float x, float y, float radX, float radY, COLOR color, float stroke, float cx, float cy, float rotation);
//(Level B) 선을 그립니다.
EXTERNC DLLAPI void draw_line(float x1, float y1, float x2, float y2, float width, COLOR color, float stroke);
//(Level B) 문자열을 출력합니다. [stroke 미지원]
EXTERNC DLLAPI void draw_text(char* text, FONT font, float x, float y, COLOR color, float stroke);
//(Level B) 문자열을 출력합니다. [stroke 미지원]
EXTERNC DLLAPI void draw_textr(char* text, FONT font, float x, float y, COLOR color, float stroke, float cx, float cy, float rotation);
//(Level B) 텍스처를 그립니다.
EXTERNC DLLAPI void draw_texture(TEXTURE texture, float x, float y, float width, float height, float opacity);
//(Level B) 텍스처를 그립니다.
EXTERNC DLLAPI void draw_texturer(TEXTURE texture, float x, float y, float width, float height, float opacity,
	float cx, float cy, float rotation);
//(Level B) 텍스처를 그립니다.
EXTERNC DLLAPI void draw_texturep(TEXTURE texture, float x, float y, float width, float height, float opacity,
	float sourceX, float sourceY, float sourceWidth, float sourceHeight);
//(Level B) 텍스처를 그립니다.
EXTERNC DLLAPI void draw_texturepr(TEXTURE texture, float x, float y, float width, float height, float opacity,
	float sourceX, float sourceY, float sourceWidth, float sourceHeight,
	float cx, float cy, float rotation);
#endif

//Sound
#ifdef JOKEBOX_LEVEL_B
//[사용되지 않음: sound_play로 변경] (Level B) 사운드를 효과음으로서 재생합니다.
EXTERNC DLLAPI void sound_fx_play(SOUND sound);
//(Level B) 사운드를 재생합니다.
EXTERNC DLLAPI void sound_play(SOUND sound, int rewind, int loopEnabled);
//(Level B) 사운드를 재생합니다.
EXTERNC DLLAPI void sound_pause(SOUND sound);
//(Level B) 사운드를 재생합니다.
EXTERNC DLLAPI void sound_stop(SOUND sound);
//(Level B) 사운드를 재생합니다.
EXTERNC DLLAPI int sound_is_playing(SOUND sound);

/* Deprecated */
////(Level B) 사운드를 배경음으로서 재생합니다.
//EXTERNC DLLAPI void sound_bgm_play(SOUND sound, int loopEnabled);
////(Level B) 배경음을 멈춥니다.
//EXTERNC DLLAPI void sound_bgm_stop();
////(Level B) 배경음이 재생중인지 여부를 반환합니다.
//EXTERNC DLLAPI int sound_is_bgm_playing();
#endif

//Content
#ifdef JOKEBOX_LEVEL_B
//(Level B) 지정한 이름의 폰트를 생성합니다.
EXTERNC DLLAPI FONT content_load_font(char* name, float weight, float pxSize);
//(Level B) 그림 파일로부터 텍스처를 생성합니다.
EXTERNC DLLAPI TEXTURE content_load_texture(char* name);
//(Level B) wav 파일로부터 사운드를 생성합니다.
EXTERNC DLLAPI SOUND content_load_sound(char* name);
#endif

//Keyboard
//(공통) 지정된 키가 눌려져있는지 여부를 반환합니다.
EXTERNC DLLAPI int keybd_is_key_down(KEYS keycode);
//(공통) 지정된 키가 눌려있는 상태로 바뀌었는지 반환합니다.
EXTERNC DLLAPI int keybd_is_key_pressed(KEYS keycode);
//(공통) 지정된 키가 떼져있는지 여부를 반환합니다.
EXTERNC DLLAPI int keybd_is_key_up(KEYS keycode);
//(공통) 지정된 키가 떼져있는 상태로 바뀌었는지 반환합니다.
EXTERNC DLLAPI int keybd_is_key_released(KEYS keycode);

//Mouse
//(공통) 지정된 버튼이 눌려져있는지 여부를 반환합니다.
EXTERNC DLLAPI int mouse_is_btn_down(MOUSEBUTTON button);
//(공통) 지정된 버튼이 눌려있는 상태로 바뀌었는지 반환합니다.
EXTERNC DLLAPI int mouse_is_btn_pressed(MOUSEBUTTON button);
//(공통) 지정된 버튼이 떼져있는지 여부를 반환합니다.
EXTERNC DLLAPI int mouse_is_btn_up(MOUSEBUTTON button);
//(공통) 지정된 버튼이 떼져있는 상태로 바뀌었는지 반환합니다.
EXTERNC DLLAPI int mouse_is_btn_released(MOUSEBUTTON button);
//(공통) 마우스의 x 좌표를 셀 단위로 반환합니다.
EXTERNC DLLAPI float mouse_get_x();
//(공통) 마우스의 y 좌표를 셀 단위로 반환합니다.
EXTERNC DLLAPI float mouse_get_y();

//Game
//(공통) 게임 프로그램을 종료합니다.
EXTERNC DLLAPI void game_exit();
//(공통) 윈도우의 제목을 변경합니다.
EXTERNC DLLAPI void game_set_window_title(char* title);
//(공통) 셀의 가로/세로 크기를 변경합니다.
EXTERNC DLLAPI void game_set_cell_size(int width, int height);
//(공통) 셀의 가로/세로 개수를 변경합니다.
EXTERNC DLLAPI void game_set_cell_count(int x, int y);
//(공통) 셀의 가로(x 방향) 개수를 반환합니다.
EXTERNC DLLAPI int game_get_cell_x_count();
//(공통) 셀의 세로(y 방향) 개수를 반환합니다.
EXTERNC DLLAPI int game_get_cell_y_count();
//(공통) 셀의 가로 크기를 반환합니다.
EXTERNC DLLAPI int game_get_cell_width();
//(공통) 셀의 세로 크기를 반환합니다.
EXTERNC DLLAPI int game_get_cell_height();
//(공통) 게임 프로그램이 실행중인지 여부를 반환합니다.
EXTERNC DLLAPI int game_is_playing();

//Engine: 엔진 구동에 필요한 함수.
//(공통) 엔진에서 자동으로 호출할 초기화(initialize)함수를 등록합니다.
EXTERNC DLLAPI void engine_set_init_func(void (*init_func)());
//(공통) 엔진에서 자동으로 호출할 그리기(draw)함수를 등록합니다.
EXTERNC DLLAPI void engine_set_draw_func(void (*draw_func)(GAMETIME));
//(공통) 엔진에서 자동으로 호출할 업데이트(update)함수를 등록합니다.
EXTERNC DLLAPI void engine_set_update_func(void (*update_func)(GAMETIME));
//(공통) 게임 화면이 출력될 화면의 핸들을 설정합니다.
EXTERNC DLLAPI void engine_set_hwnd(HWND hwnd);
//(공통) 엔진을 초기화하고 게임 루프를 시작합니다.
EXTERNC DLLAPI int engine_run();
