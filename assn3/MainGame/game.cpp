/*----------------------------------------------------------*
 *															*
 *				   Jokebox Framework v4.05					*
 *															*
 * 		   이광무에 의해 작성된 Jokebox Framework는			*
 *		크리에이티브 커먼즈 저작자표시-동일조건변경허락		*
 * 		3.0 Unported 라이선스에 따라 이용할 수 있습니다.	*
 *															*
 *		! 이 소스에서는 이 주석을 삭제하셔도 좋습니다 !		*
 *															*
 *					ORANGEYELLOW, 2013.						*
 *					iss300@postech.ac.kr					*
 *															*
 *----------------------------------------------------------*/
#ifndef JOKEBOX_LEVEL_A
#define JOKEBOX_LEVEL_A			// TODO: 사용하고자 하는 레벨의 주석을 해제
//#define JOKEBOX_LEVEL_B
#endif

#include "jokebox.h"
#include "jokeboxhelper.h"
#include "game.h"
#include "som2d.h"
#include <ctime>
void initialize()
{
	srand((unsigned int)time(NULL));
	SOM2D* som2d = new SOM2D(13, 5, 3, 5, 5, 0.01f, 3);
}

void update(GAMETIME gametime)
{
	SOM2D& som = *SOM2D::globalSOM2D;
}

void draw(GAMETIME gametime)
{
	draw_begin();
	draw_clear(WHITE);

	// TODO: 게임 화면에 출력할 것을 작성.
	
	draw_end();
}