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

#include <Windows.h>
#include "fn_input.h"
#include "fn_engine.h"

#include <stdio.h>

BYTE _prev_key_state[256];
BYTE _key_state[256];

SHORT _prev_mouse_state[2];
SHORT _mouse_state[2];
POINT _mouse_pos;

//Update�ϱ� �� �� �ѹ��� ����. ���� Ű ����.
void _input_ready()
{
	GetKeyState(0);		//�̰� ����� Ű ���� ����� ���޵�. ��..?

	//Ű���� �غ�
	memcpy(_prev_key_state, _key_state, sizeof(BYTE) * 256);
	GetKeyboardState((LPBYTE)_key_state);

	//���콺 �غ�
	memcpy(_prev_mouse_state, _mouse_state, sizeof(SHORT) * 2);
	_mouse_state[MOUSE_LEFT] = GetKeyState(VK_LBUTTON);
	_mouse_state[MOUSE_RIGHT] = GetKeyState(VK_RBUTTON);
	GetCursorPos(&_mouse_pos);
	ScreenToClient(_engine_get_hwnd(), &_mouse_pos);
}

int _keybd_is_key_down(KEYS keycode)
{
	return _key_state[keycode] >> 1;
}

int _keybd_is_key_pressed(KEYS keycode)
{
	return !(_prev_key_state[keycode] >> 1) && (_key_state[keycode] >> 1);
}

int _keybd_is_key_up(KEYS keycode)
{
	return !(_key_state[keycode] >> 1);
}

int _keybd_is_key_released(KEYS keycode)
{
	return (_prev_key_state[keycode] >> 1) && !(_key_state[keycode] >> 1);
}


int _mouse_is_btn_down(MOUSEBUTTON button)
{
	return _mouse_state[button] >> 1;
}

int _mouse_is_btn_pressed(MOUSEBUTTON button)
{
	return !(_prev_mouse_state[button] >> 1) && (_mouse_state[button] >> 1);
}

int _mouse_is_btn_up(MOUSEBUTTON button)
{
	return !(_mouse_state[button] >> 1);
}

int _mouse_is_btn_released(MOUSEBUTTON button)
{
	return (_prev_mouse_state[button] >> 1) && !(_mouse_state[button] >> 1);
}

float _mouse_get_x()
{
	return (float)_mouse_pos.x / _game_get_cell_width();
}

float _mouse_get_y()
{
	return (float)_mouse_pos.y / _game_get_cell_height();
}