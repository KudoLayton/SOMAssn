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

#define JOKEBOX_LEVEL_A			// TODO: 사용하고자 하는 레벨의 주석을 해제
//#define JOKEBOX_LEVEL_B

#include "jokebox.h"
#include "jokeboxhelper.h"
#include "game.h"

void initialize()
{
	// TODO: 게임이 최초로 시작되었을 때 초기화할 것을 작성.
	//		ex) 캐릭터의 처음 위치 세팅, 변수값 초기화, 텍스처/사운드 불러오기..
}

void update(GAMETIME gametime)
{
	// TODO: 게임 화면이 출력되기 전에 필요한 연산들을 작성.
	//		ex) 시간을 측정해서 캐릭터의 위치 변경..
}

void draw(GAMETIME gametime)
{
	draw_begin();
	draw_clear(WHITE);

	// TODO: 게임 화면에 출력할 것을 작성.
	
	draw_end();
}