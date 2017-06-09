/*----------------------------------------------------------*
 *															*
 *				   Jokebox Framework v4.05					*
 *															*
 * 		   �̱����� ���� �ۼ��� Jokebox Framework��			*
 *		ũ������Ƽ�� Ŀ���� ������ǥ��-�������Ǻ������		*
 * 		3.0 Unported ���̼����� ���� �̿��� �� �ֽ��ϴ�.	*
 *															*
 *					ORANGEYELLOW, 2013.						*
 *					iss300@postech.ac.kr					*
 *															*
 *----------------------------------------------------------*/

#include "jokebox.h"
#include "game.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void set_engine();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
    PWSTR pCmdLine, int nCmdShow)
{
	// ������ ������, http://www.zetcode.com/gui/winapi/window/
	MSG  msg;    
	HWND hwnd;
	WNDCLASSW wc;
	
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.lpszClassName = L"Window";
	wc.hInstance     = hInstance;
	wc.hbrBackground = CreateSolidBrush(RGB(0,0,0));
	wc.lpszMenuName  = NULL;
	wc.lpfnWndProc   = WndProc;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&wc);
	hwnd = CreateWindowW( wc.lpszClassName, L"Console",
				WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
				100, 100, 50, 50, NULL, NULL, hInstance, NULL);

	//���� ���� �� ����
	engine_set_init_func(initialize);
	engine_set_update_func(update);
	engine_set_draw_func(draw);
	engine_set_hwnd(hwnd);

	engine_run();

	while(GetMessage(&msg, NULL, 0, 0))
		DispatchMessage(&msg);		//������ ���

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, 
    WPARAM wParam, LPARAM lParam)
{
	switch(msg)  
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;      
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}