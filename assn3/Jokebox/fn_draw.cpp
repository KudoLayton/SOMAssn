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

#pragma warning(once: 4244)

#include "fn_draw.h"
#include "fn_game.h"
#include "fn_engine.h"
#include "fn_debug.h"
#include <exception>
#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <cwchar>
#include <stdexcept>
#include <iostream>

#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

using namespace D2D1;
using namespace std;

int _is_drawable = 0;
HWND _hwnd;
PAINTSTRUCT ps;

ID2D1Factory *d2_factory;
IDWriteFactory *dw_factory;
ID2D1HwndRenderTarget *d2_target;
ID2D1BitmapRenderTarget *d2_bmtarget;

ID2D1SolidColorBrush *d2_stdbrush;
IDWriteTextFormat *dw_textformat;

int _cw_cache;
int _ch_cache;
float _dpi_x;
float _dpi_y;

// 보조 함수
void mbctowcs(int word, wchar_t* wcs);
void color_extract_alpha(COLOR* color, float* alpha);
float cx_to_px(float val);
float cy_to_px(float val);

ID2D1HwndRenderTarget* _draw_get_render_target()
{
	return d2_target;
}

//Draw를 초기화함. 핸들을 잡고 버퍼 초기화.
int _draw_initialize(HWND hwnd)
{
	HRESULT hr;

	if (!(_hwnd = hwnd))		//창의 핸들을 가져옴
		return 0;			

	//DWrite, D2D Factory 생성
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&d2_factory);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"Direct2D Factory 생성 실패",
			"(내부 함수)");

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&dw_factory));
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"DirectWrite Factory 생성 실패",
			"(내부 함수)");
	
	//기본 폰트 포맷을 생성
	hr = dw_factory->CreateTextFormat(
		L"Dotumche",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		min(_game_get_cell_width(), _game_get_cell_height()) * 0.88f,
		L"en-us",
		&dw_textformat);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"DirectWrite 기본 폰트 재생성 실패",
			"(내부 함수)");

	//리소스 생성
	_draw_create_resource();

	//안티앨리어싱 모드 설정
#ifdef JOKEBOX_LEVEL_A
	d2_target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_FORCE_DWORD);
	d2_target->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_FORCE_DWORD);
	d2_bmtarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_FORCE_DWORD);
	d2_bmtarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_FORCE_DWORD);
#endif
#ifdef JOKEBOX_LEVEL_B
	d2_target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	d2_target->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
	d2_bmtarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	d2_bmtarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
#endif

	//셀 크기 버퍼 등록
	_ch_cache = _game_get_cell_height();
	_cw_cache = _game_get_cell_width();


	//임시 로딩이미지를 출력
	_draw_begin();
	_draw_clear(BLACK);
	_draw_end();

	return 1;
}

bool _draw_discard_resource()
{
	if (d2_target != NULL)
	{
		d2_target->Release();
		d2_target = NULL;
	}

	if (d2_bmtarget != NULL)
	{
		d2_bmtarget->Release();
		d2_bmtarget = NULL;
	}
	
	if (d2_stdbrush != NULL)
	{
		d2_stdbrush->Release();
		d2_stdbrush = NULL;
	}

	return true;
}

bool _draw_create_resource()
{
	HRESULT hr;

	D2D1_SIZE_U size = D2D1::SizeU(_game_get_cell_width() * _game_get_cell_x_count(),
		_game_get_cell_height() * _game_get_cell_y_count());
	
	//Hwnd Render타겟을 생성
	hr = d2_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(_engine_get_hwnd(), size),
		&d2_target);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__, "Direct2D RenderTarget 크기 조정 실패",
			"(내부 함수)");
	
	////dpi를 가져옴
	//d2_factory->GetDesktopDpi(&_dpi_x, &_dpi_y);

	//Bitmap Render 타겟을 생성: 여기에 먼저 그리고 dpi에 맞게 늘임
	hr = d2_target->CreateCompatibleRenderTarget(D2D1::SizeF(_game_get_cell_width() * _game_get_cell_x_count(),
		_game_get_cell_height() * _game_get_cell_y_count()),
		size, &d2_bmtarget);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"Direct2D 백버퍼 재생성 실패",
			"(내부 함수)");

	//기본 브러시를 생성
	hr = d2_bmtarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &d2_stdbrush);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"Direct2D 기본 브러쉬 재생성 실패",
			"(내부 함수)");

	//셀 크기 버퍼 등록
	_ch_cache = _game_get_cell_height();
	_cw_cache = _game_get_cell_width();

	return true;
}

bool _draw_reset_resource()
{
	bool res = true;

	res &= _draw_discard_resource();
	res &= _draw_create_resource();

	return res;
}

//화면을 draw 가능 상태로 전환.
void _draw_begin()
{
	d2_bmtarget->BeginDraw();

	if (_is_drawable)
		_debug_raise_error(__FILE__, __LINE__,
			"그리기 가능 상태에서 다시 draw_begin을 선언",
			"_draw_begin",
			"그리기를 마치고 draw_end 선언을 하지 않았을 수 있습니다");

	_is_drawable = 1;			//드로우 가능 상태로 전환.
}

void _draw_clear(COLOR color)
{
	color_extract_alpha(&color, NULL);
	d2_bmtarget->Clear(D2D1::ColorF(color));
}

void _draw_end()
{
	HRESULT hr;

	hr = d2_bmtarget->EndDraw();
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"Direct2D 그리기 종료 실패",
			"_draw_end");

	//백버퍼 bmp를 받아옴
	ID2D1Bitmap* res = NULL;
	hr = d2_bmtarget->GetBitmap(&res);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"백버퍼 로드 실패",
			"_draw_end");

	//창 다시그리기 (invalidate)
	RECT rect;
	GetClientRect(_hwnd, &rect);
	InvalidateRect(_hwnd, &rect, 0);
	
	//백버퍼 그리기
	BeginPaint(_hwnd, &ps);
	d2_target->BeginDraw();
	d2_target->SetTransform(D2D1::Matrix3x2F::Identity());
	d2_target->Clear(D2D1::ColorF(WHITE));
	
	D2D1_SIZE_F size = d2_target->GetSize();
	D2D1_RECT_F target_rect = D2D1::RectF(0, 0, d2_target->GetSize().width, d2_target->GetSize().height);
	d2_target->DrawBitmap(res, &target_rect);

	hr = d2_target->EndDraw();
	if (!SUCCEEDED(hr))
		_draw_reset_resource();

	EndPaint(_hwnd, &ps);
	ValidateRect(_hwnd, NULL);

	res->Release();

	_is_drawable = 0;			//드로우 불가능 상태로 전환.
}

// Level A

void _draw_char(int word, int x, int y, COLOR color, COLOR backcolor)
{
	if (!_is_drawable)
		_debug_raise_error(__FILE__, __LINE__,
			"그리기 불가능한 상황에서 그리기 시도",
			"_draw_char",
			"그리기를 수행하기 전에 먼저 draw_begin을 호출해야 합니다.");

	float alpha;

	// 문자 출력
	if (x < 0 || x >= _game_get_cell_x_count() ||
		y < 0 || y >= _game_get_cell_y_count())
		return;

	D2D1_RECT_F rect = D2D1::RectF(cx_to_px(x), cy_to_px(y), 
		cx_to_px(x + 1),
		cy_to_px(y + 1));
	
	color_extract_alpha(&backcolor, &alpha);
	d2_stdbrush->SetColor(D2D1::ColorF(backcolor, alpha));
	d2_bmtarget->FillRectangle(rect, d2_stdbrush);

	dw_textformat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	wchar_t wcs[5] = {0};
	mbctowcs(word, wcs);
	color_extract_alpha(&color, &alpha);
	d2_stdbrush->SetColor(D2D1::ColorF(color, alpha));
	d2_bmtarget->DrawTextW(wcs, 1, dw_textformat, rect, d2_stdbrush);
}

void _draw_string(char* string, int x, int y, COLOR color, COLOR backcolor)
{
	if (!_is_drawable)
		_debug_raise_error(__FILE__, __LINE__,
			"그리기 불가능한 상황에서 그리기 시도",
			"_draw_string",
			"그리기를 수행하기 전에 먼저 draw_begin을 호출해야 합니다.");	//드로우 가능 상태일 때만 실행 가능.
	
	wchar_t buf[2] = {0};
	float ubacka;
	float ufonta;
	wchar_t wcs[300] = {0};

	MultiByteToWideChar(CP_ACP, 0, string, strlen(string) + 1, wcs, 300);
	color_extract_alpha(&backcolor, &ubacka);
	color_extract_alpha(&color, &ufonta);

	dw_textformat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	for (int i = 0, xoff = 0, yoff = 0, len = wcslen(wcs); i < len; i++, xoff++)
	{
		if (wcs[i] == '\n')
		{
			xoff = -1;
			yoff++;
			continue;
		}

		// 문자열 출력
		D2D1_RECT_F rect = D2D1::RectF(cx_to_px(x + xoff), cy_to_px(y + yoff), 
			cx_to_px(x + xoff + 1),
			cy_to_px(y + yoff + 1));

		d2_stdbrush->SetColor(D2D1::ColorF(backcolor, ubacka));
		d2_bmtarget->FillRectangle(rect, d2_stdbrush);

		buf[0] = wcs[i];
		d2_stdbrush->SetColor(D2D1::ColorF(color, ufonta));
		d2_bmtarget->DrawTextW(buf, 1, dw_textformat, rect, d2_stdbrush);

		if (wcs[i] == '\t')
		{
			int incr = 4 - (x + xoff + 1) % 4;
			rect.left = cx_to_px(x + xoff + 1);
			rect.top = cy_to_px(y + yoff);
			rect.right = cx_to_px(x + xoff + 1 + incr);
			rect.bottom = cy_to_px(y + yoff + 1);

			d2_stdbrush->SetColor(D2D1::ColorF(backcolor, ubacka));
			d2_bmtarget->FillRectangle(rect, d2_stdbrush);

			xoff += incr;
		}
	}
}

void _draw_area(int word, int x1, int y1, int x2, int y2, COLOR color, COLOR backcolor)
{	
	if (!_is_drawable)
		_debug_raise_error(__FILE__, __LINE__,
			"그리기 불가능한 상황에서 그리기 시도",
			"_draw_area",
			"그리기를 수행하기 전에 먼저 draw_begin을 호출해야 합니다.");		//드로우 가능 상태일 때만 실행 가능.

	for (int i = x1; i < x2; i++)
		for (int j = y1; j < y2; j++)
			_draw_char(word, i, j, color, backcolor);
}

// Level B

void _draw_rect(float x, float y, float width, float height, 
	COLOR color, float stroke, float cx, float cy, float rotation)
{
	if (!_is_drawable)
		_debug_raise_error(__FILE__, __LINE__,
			"그리기 불가능한 상황에서 그리기 시도",
			"_draw_rect",
			"그리기를 수행하기 전에 먼저 draw_begin을 호출해야 합니다.");	//드로우 가능 상태일 때만 실행 가능.

	D2D1_RECT_F rect = D2D1::RectF(cx_to_px(x), cy_to_px(y),
		cx_to_px(x + width), cy_to_px(y + height));

	if (rotation)
		d2_bmtarget->SetTransform(
			D2D1::Matrix3x2F::Rotation(-rotation, D2D1::Point2F(cx_to_px(cx), cy_to_px(cy))));
	
	float alpha;
	color_extract_alpha(&color, &alpha);
	d2_stdbrush->SetColor(D2D1::ColorF(color, alpha));
	d2_bmtarget->FillRectangle(rect, d2_stdbrush);

	d2_stdbrush->SetColor(D2D1::ColorF(BLACK));
	d2_bmtarget->DrawRectangle(rect, d2_stdbrush, stroke);

	if (rotation)
		d2_bmtarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void _draw_triangle(float x, float y, float width, float height, float aoff, TRIANGLEDIR dir,
	COLOR color, float stroke, float cx, float cy, float rotation)
{	
	if (!_is_drawable)
		_debug_raise_error(__FILE__, __LINE__,
			"그리기 불가능한 상황에서 그리기 시도",
			"_draw_polygon",
			"그리기를 수행하기 전에 먼저 draw_begin을 호출해야 합니다.");		//드로우 가능 상태일 때만 실행 가능.

	//Factory->PathGeometry->GeometrySink로 삼각형 만들기
	ID2D1PathGeometry *path = NULL;
	ID2D1GeometrySink *sink = NULL;

	d2_factory->CreatePathGeometry(&path);
	path->Open(&sink);

	switch (dir)
	{
	case TDIR_UP:
		sink->BeginFigure(D2D1::Point2F(cx_to_px(x), cy_to_px(y + height)), D2D1_FIGURE_BEGIN_FILLED);
		sink->AddLine(D2D1::Point2F(cx_to_px(x + width), cy_to_px(y + height)));
		sink->AddLine(D2D1::Point2F(cx_to_px(x + width / 2 + width * aoff), cy_to_px(y)));
		break;
	case TDIR_DOWN:
		sink->BeginFigure(D2D1::Point2F(cx_to_px(x), cy_to_px(y)), D2D1_FIGURE_BEGIN_FILLED);
		sink->AddLine(D2D1::Point2F(cx_to_px(x + width), cy_to_px(y)));
		sink->AddLine(D2D1::Point2F(cx_to_px(x + width / 2 + width * aoff), cy_to_px(y + height)));
		break;
	case TDIR_LEFT:
		sink->BeginFigure(D2D1::Point2F(cx_to_px(x + width), cy_to_px(y)), D2D1_FIGURE_BEGIN_FILLED);
		sink->AddLine(D2D1::Point2F(cx_to_px(x + width), cy_to_px(y + height)));
		sink->AddLine(D2D1::Point2F(cx_to_px(x), cy_to_px(y + height / 2 + height * aoff)));
		break;
	case TDIR_RIGHT:
		sink->BeginFigure(D2D1::Point2F(cx_to_px(x), cy_to_px(y)), D2D1_FIGURE_BEGIN_FILLED);
		sink->AddLine(D2D1::Point2F(cx_to_px(x), cy_to_px(y + height)));
		sink->AddLine(D2D1::Point2F(cx_to_px(x + width), cy_to_px(y + height / 2 + height * aoff)));
		break;
	}

	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();
	
	if (rotation)
		d2_bmtarget->SetTransform(
			D2D1::Matrix3x2F::Rotation(-rotation, D2D1::Point2F(cx_to_px(cx), cy_to_px(cy))));

	//Path 그리기
	float alpha;
	color_extract_alpha(&color, &alpha);
	d2_stdbrush->SetColor(D2D1::ColorF(color, alpha));
	d2_bmtarget->FillGeometry(path, d2_stdbrush);
	d2_stdbrush->SetColor(D2D1::ColorF(BLACK));
	d2_bmtarget->DrawGeometry(path, d2_stdbrush, stroke);

	path->Release();
	sink->Release();

	if (rotation)
		d2_bmtarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void _draw_polygon(float x, float y, float vertices[], int vCount, COLOR color, float stroke, float cx, float cy, float rotation)
{
	if (!_is_drawable)
		_debug_raise_error(__FILE__, __LINE__,
			"그리기 불가능한 상황에서 그리기 시도",
			"_draw_polygon",
			"그리기를 수행하기 전에 먼저 draw_begin을 호출해야 합니다.");		//드로우 가능 상태일 때만 실행 가능.

	//Factory->PathGeometry->GeometrySink로 다각형 만들기
	ID2D1PathGeometry *path = NULL;
	ID2D1GeometrySink *sink = NULL;

	d2_factory->CreatePathGeometry(&path);
	path->Open(&sink);

	sink->BeginFigure(D2D1::Point2F(cx_to_px(x + vertices[0]),
		cy_to_px(y + vertices[1])), D2D1_FIGURE_BEGIN_FILLED);

	try {
		for (int i = 1; i < vCount; i++)
			sink->AddLine(D2D1::Point2F(cx_to_px(vertices[2 * i]),
			cy_to_px(vertices[2 * i + 1])));
	}
	catch (std::out_of_range ex) {
		_debug_raise_error(__FILE__, __LINE__,
			"배열 범위를 벗어난 접근",
			"_draw_polygon",
			"다각형 꼭지점과 꼭지점 수가 제대로 입력되지 않았을 수 있습니다");
	}
	catch (...){
		_debug_raise_error(__FILE__, __LINE__,
			"알 수 없는 예외",
			"_draw_polygon",
			"다각형 꼭지점과 꼭지점 수가 제대로 입력되지 않았을 수 있습니다");
	}

	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();
	
	if (rotation)
		d2_bmtarget->SetTransform(
			D2D1::Matrix3x2F::Rotation(-rotation, D2D1::Point2F(cx_to_px(cx), cy_to_px(cy))));

	//Path 그리기
	float alpha;
	color_extract_alpha(&color, &alpha);
	d2_stdbrush->SetColor(D2D1::ColorF(color, alpha));
	d2_bmtarget->FillGeometry(path, d2_stdbrush);
	d2_stdbrush->SetColor(D2D1::ColorF(BLACK));
	d2_bmtarget->DrawGeometry(path, d2_stdbrush, stroke);

	path->Release();
	sink->Release();

	if (rotation)
		d2_bmtarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void _draw_ellipse(float x, float y, float radX, float radY, COLOR color, float stroke, float cx, float cy, float rotation)
{
	if (!_is_drawable)
		_debug_raise_error(__FILE__, __LINE__,
			"그리기 불가능한 상황에서 그리기 시도",
			"_draw_ellipse",
			"그리기를 수행하기 전에 먼저 draw_begin을 호출해야 합니다.");		//드로우 가능 상태일 때만 실행 가능.

	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(cx_to_px(x), cy_to_px(y)), cx_to_px(radX), cy_to_px(radY));

	if (rotation)
	{
		d2_bmtarget->SetTransform(
			D2D1::Matrix3x2F::Rotation(-rotation, D2D1::Point2F(cx_to_px(cx), cy_to_px(cy))));
	}
	
	float alpha;
	color_extract_alpha(&color, &alpha);
	d2_stdbrush->SetColor(D2D1::ColorF(color, alpha));
	d2_bmtarget->FillEllipse(ellipse, d2_stdbrush);

	d2_stdbrush->SetColor(D2D1::ColorF(BLACK));
	d2_bmtarget->DrawEllipse(ellipse, d2_stdbrush, stroke);

	if (rotation)
	{
		d2_bmtarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}
}

void _draw_line(float x1, float y1, float x2, float y2, float width, COLOR color, float stroke)
{
	if (!_is_drawable)
		_debug_raise_error(__FILE__, __LINE__,
			"그리기 불가능한 상황에서 그리기 시도",
			"_draw_line",
			"그리기를 수행하기 전에 먼저 draw_begin을 호출해야 합니다.");		//드로우 가능 상태일 때만 실행 가능.

	float alpha;
	color_extract_alpha(&color, &alpha);
	
	//테두리 그리기
	if (stroke > 0)
	{
		d2_stdbrush->SetColor(D2D1::ColorF(BLACK));

		//테두리 캡 그리기
		//(원형 캡)
		//D2D1_ELLIPSE linecap = D2D1::Ellipse(D2D1::Point2F(cx_to_px(x1), cy_to_px(y1)), (stroke + width) / 2, (stroke + width) / 2);
		//d2_bmtarget->FillEllipse(linecap, d2_stdbrush);
		//linecap.point.x = cx_to_px(x2);
		//linecap.point.y = cy_to_px(y2);
		//d2_bmtarget->FillEllipse(linecap, d2_stdbrush);
		//(사각형 캡)
		float len = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
		float xoff = stroke * (x2 - x1) / len;
		float yoff = stroke * (y2 - y1) / len;

		//테두리 선 그리기
		d2_bmtarget->DrawLine(D2D1::Point2F(cx_to_px(x1) - xoff, cy_to_px(y1) - yoff), 
			D2D1::Point2F(cx_to_px(x2) + xoff, cy_to_px(y2) + yoff), d2_stdbrush, stroke * 2 + width);
	}

	//선 그리기
	d2_stdbrush->SetColor(D2D1::ColorF(color, alpha));
	d2_bmtarget->DrawLine(D2D1::Point2F(cx_to_px(x1), cy_to_px(y1)), 
		D2D1::Point2F(cx_to_px(x2), cy_to_px(y2)), d2_stdbrush, width);
}

void _draw_text(char* text, FONT font, float x, float y, COLOR color, float stroke, float cx, float cy, float rotation)
{
	if (!_is_drawable)
		_debug_raise_error(__FILE__, __LINE__,
			"그리기 불가능한 상황에서 그리기 시도",
			"_draw_text",
			"그리기를 수행하기 전에 먼저 draw_begin을 호출해야 합니다.");			//드로우 가능 상태일 때만 실행 가능.
		
	//폰트 로드
	IDWriteTextFormat* fclass;

	if (font == NULL)				//NULL이면 기본 폰트
	{
		fclass = dw_textformat;
		dw_textformat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	}
	else							//NULL이 아니면 content에서 폰트 로드
		fclass = _content_get_font(font);

	assert(fclass != NULL && "지정된 font가 NULL임");			//폰트가 NULL이면 안됨.

	//글자 그리기
	float alpha;
	wchar_t wcs[300] = {0};
	
	if (rotation)
	{
		d2_bmtarget->SetTransform(
			D2D1::Matrix3x2F::Rotation(-rotation, D2D1::Point2F(cx_to_px(cx), cy_to_px(cy))));
	}

	MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, wcs, 300);

	D2D1_RECT_F rect = D2D1::RectF(cx_to_px(x), cy_to_px(y), 
		_game_get_cell_width() * _game_get_cell_x_count(),
		_game_get_cell_height() * _game_get_cell_y_count());
	
	color_extract_alpha(&color, &alpha);
	d2_stdbrush->SetColor(D2D1::ColorF(color, alpha));
	d2_bmtarget->DrawTextW(wcs, wcslen(wcs), fclass, rect, d2_stdbrush);

	if (rotation)
		d2_bmtarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void _draw_texture(TEXTURE texture, float x, float y, float width, float height,
	float opacity, float sourceX, float sourceY, float sourceWidth, float sourceHeight,
	float cx, float cy, float rotation)
{
	if (!_is_drawable)
		_debug_raise_error(__FILE__, __LINE__,
			"그리기 불가능한 상황에서 그리기 시도",
			"_draw_texture",
			"그리기를 수행하기 전에 먼저 draw_begin을 호출해야 합니다.");	

	ID2D1Bitmap* ftexture = _content_get_texture(texture);
	float sx, sy, sw, sh;
	
	if (!ftexture)
		_debug_raise_error(__FILE__, __LINE__,
			"NULL 포인터에 접근",
			"_draw_texture",
			"잘못된 ID로 컨텐트에 접근하였을 수 있습니다.");	

	D2D1_SIZE_U size = ftexture->GetPixelSize();
	sx = sourceX * size.width;
	sy = sourceY * size.height;
	sw = sourceWidth * size.width;
	sh = sourceHeight * size.height;

	if (rotation)
	{
		d2_bmtarget->SetTransform(
			D2D1::Matrix3x2F::Rotation(-rotation, D2D1::Point2F(cx_to_px(cx), cy_to_px(cy))));
	}

	d2_bmtarget->DrawBitmap(ftexture,
		D2D1::RectF(cx_to_px(x), cy_to_px(y), cx_to_px(x + width), cy_to_px(y + height)),
		opacity,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(sx, sy, sx + sw, sy + sh));

	if (rotation)
		d2_bmtarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

// 보조 함수

void mbctowcs(int word, wchar_t* wcs)
{
	char buf[3] = {0};

	buf[0] = ((char*)&word)[1];
	buf[1] = ((char*)&word)[0];

	if (!buf[0])
	{
		buf[0] = buf[1]; buf[1] = 0;
	}

	MultiByteToWideChar(CP_ACP, 0, buf, 3, wcs, 5);
}

void color_extract_alpha(COLOR* color, float* alpha)
{
	unsigned long uc = *(unsigned int*)(color);

	if (alpha != NULL)
	{
		unsigned int ualpha = (uc & 0xFF000000u) >> 24;
		*alpha = ualpha / 255.0f;
	}
	
	*color = uc & 0x00FFFFFFu;
}

float cx_to_px(float val)
{
	return val * _cw_cache;
}

float cy_to_px(float val)
{
	return val * _ch_cache;
}