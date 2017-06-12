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
#include "traindata.h"
#include <ctime>
int t;
float minDistance;
float correctRate;
void initialize()
{
	srand((unsigned int)time(NULL));
	new SOM2D(13, 20, 3, 1000, 1000, 0.01f, 3);
	new	Data("Wine Input.asc", "Wine Desired.asc", 13, 3);
//	game_set_cell_count(5, 5);
	t = 1;
}

void update(GAMETIME gametime)
{
	SOM& som = *SOM::globalSOM;
	Data& data = *Data::globalData;
	//if (t < 20)
	minDistance = som.learnData(data, t);
	som.resetLabel();
	for (unsigned int i = 0; i < data.trainData.size(); i++) {
		som.Labeling(data.trainData[i]->input, data.trainData[i]->label);
	}
	correctRate = som.makeResultData(data);
	t++;
}

void draw(GAMETIME gametime)
{
	draw_begin();
	draw_clear(BLACK);
	SOM::globalSOM->drawMap(*Data::globalData);
	// TODO: 게임 화면에 출력할 것을 작성.
	//draw_charc(' ', 2, 2, WHITE, WHITE);
	draw_string(0, 20, "iteration: %d", t);
	draw_string(0, 21, "minDistance: %f", minDistance);
	draw_string(0, 22, "correctRate: %f", correctRate);
	draw_end();
}