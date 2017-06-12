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
#define JOKEBOX_LEVEL_B
#endif

#include "jokebox.h"
#include "jokeboxhelper.h"
#include "game.h"
#include "som1d.h"
#include "traindata.h"
#include <ctime>
int t;
float minDistance;
float correctRate;
void initialize()
{
	srand((unsigned int)time(NULL));
	new SOM1D(2, 400, 1, 1000, 1000, 0.5f, 40);
	new	Data("H input.txt", "H output.txt", 2, 1);
	game_set_cell_count(50, 50);
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
	draw_clear(WHITE);
	draw_line(0, 0, 10, 0, 2, BLUE_VIOLET, 0);
	draw_line(10, 0, 10, 10, 2, BLUE_VIOLET, 0);
	draw_line(10, 10, 20, 10, 2, BLUE_VIOLET, 0);
	draw_line(20, 10, 20, 0, 2, BLUE_VIOLET, 0);
	draw_line(20, 0, 30, 0, 2, BLUE_VIOLET, 0);
	draw_line(30, 0, 30, 30, 2, BLUE_VIOLET, 0);
	draw_line(30, 30, 20, 30, 2, BLUE_VIOLET, 0);
	draw_line(20, 30, 20, 20, 2, BLUE_VIOLET, 0);
	draw_line(20, 20, 10, 20, 2, BLUE_VIOLET, 0);
	draw_line(10, 20, 10, 30, 2, BLUE_VIOLET, 0);
	draw_line(10, 30, 0, 30, 2, BLUE_VIOLET, 0);
	draw_line(0, 30, 0, 0, 2, BLUE_VIOLET, 0);
	SOM::globalSOM->drawMap(*Data::globalData);
	// TODO: 게임 화면에 출력할 것을 작성.
	//draw_charc(' ', 2, 2, WHITE, WHITE);
	draw_string(0, 45, "iteration: %d", t);
	draw_string(0, 46, "minDistance: %f", minDistance);
	draw_string(0, 47, "correctRate: %f", correctRate);
	draw_end();
}