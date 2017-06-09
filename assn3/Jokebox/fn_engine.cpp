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

	//�ʱ�ȭ �Լ� ȣ��
	if (init_func != NULL)
		init_func();

	while(_game_is_playing())
	{
		//Game ����
		GAMETIME gt;

		_input_ready();		//����Ű ����
		gt.elapsed = _time_elapsed();
		gt.total = _time_total();
		_time_set_update();

		if (update_func != NULL)
			update_func(gt);		//������Ʈ �Լ� ȣ��
		if (draw_func != NULL)
			draw_func(gt);			//��ο� �Լ� ȣ��
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

	result = AdjustWindowRect(&crect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);				//����!!!
	if (!result)
		_debug_raise_error(__FILE__, __LINE__, "â ũ�� ���� ����",
			"(���� �Լ�)");
	MoveWindow(_engine_get_hwnd(), (desktop.right - crect.right) / 2, (desktop.bottom - crect.bottom) / 2 - 20, crect.right - crect.left, crect.bottom - crect.top, FALSE);
	
	return result;
}

int _engine_run()
{
	//â�� �ڵ��� �����Ǿ����� ������ ���и� ����.
	if (_hwnd == NULL)
		return 0;

	//���� �ʱ�ȭ
	_engine_reshape_window();
	_draw_initialize(_hwnd);
	_content_initialize();

	//�ð� �ʱ�ȭ
	_time_set_start();		
	_time_set_update();

	//���� ����
	_game_begin();

	_beginthread(_engine_loop, 0, 0);	//���ӷ��� ������.

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
