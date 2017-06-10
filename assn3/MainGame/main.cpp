/*----------------------------------------------------------*
 *															*
 *				   Jokebox Framework v4.05					*
 *															*
 * 		   이광무에 의해 작성된 Jokebox Framework는			*
 *		크리에이티브 커먼즈 저작자표시-동일조건변경허락		*
 * 		3.0 Unported 라이선스에 따라 이용할 수 있습니다.	*
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
	// 윈도우 생성부, http://www.zetcode.com/gui/winapi/window/
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

	//엔진 세팅 후 시작
	engine_set_init_func(initialize);
	engine_set_update_func(update);
	engine_set_draw_func(draw);
	engine_set_hwnd(hwnd);

	engine_run();

	while(GetMessage(&msg, NULL, 0, 0))
		DispatchMessage(&msg);		//윈도우 명령

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