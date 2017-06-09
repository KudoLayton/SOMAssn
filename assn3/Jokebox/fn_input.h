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

#ifndef FN_INPUT
#define FN_INPUT

typedef enum __declspec(dllexport) Keys {KEY_ENTER = 13, 
	KEY_SHIFT = 16, 
	KEY_CTRL = 17, 
	KEY_ESC = 27, 
	KEY_SPACE = 32,
	KEY_LEFT = 37, 
	KEY_UP = 38, 
	KEY_RIGHT = 39, 
	KEY_DOWN = 40,
	KEY_0 = 48, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
	KEY_A = 65,
	KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R,
	KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z
}KEYS;

typedef enum _declspec(dllexport) MouseButton
{
	MOUSE_LEFT = 0,
	MOUSE_RIGHT = 1
}MOUSEBUTTON;

void _input_ready();

int _keybd_is_key_down(KEYS keycode);
int _keybd_is_key_pressed(KEYS keycode);
int _keybd_is_key_up(KEYS keycode);
int _keybd_is_key_released(KEYS keycode);

int _mouse_is_btn_down(MOUSEBUTTON button);
int _mouse_is_btn_pressed(MOUSEBUTTON button);
int _mouse_is_btn_up(MOUSEBUTTON button);
int _mouse_is_btn_released(MOUSEBUTTON button);
float _mouse_get_x();		//�� ������ ���콺 ��ġ�� �Ѱ���.
float _mouse_get_y();

#endif