/*----------------------------------------------------------*
 *															*
 *				   Jokebox Framework v4.05					*
 *															*
 * 		   �̱����� ���� �ۼ��� Jokebox Framework��			*
 *		ũ������Ƽ�� Ŀ���� ������ǥ��-�������Ǻ������		*
 * 		3.0 Unported ���̼����� ���� �̿��� �� �ֽ��ϴ�.	*
 *															*
 *		! �� �ҽ������� �� �ּ��� �����ϼŵ� �����ϴ� !		*
 *															*
 *					ORANGEYELLOW, 2013.						*
 *					iss300@postech.ac.kr					*
 *															*
 *----------------------------------------------------------*/
#ifndef JOKEBOX_LEVEL_A
#define JOKEBOX_LEVEL_A			// TODO: ����ϰ��� �ϴ� ������ �ּ��� ����
//#define JOKEBOX_LEVEL_B
#endif

#include "jokebox.h"
#include "jokeboxhelper.h"
#include "game.h"
#include "som2d.h"
#include "traindata.h"
#include <ctime>
int t;
void initialize()
{
	srand((unsigned int)time(NULL));
	new SOM2D(13, 10, 3, 10, 10, 4, 0.01f);
	new	Data("Wine Input.asc", "Wine Desired.asc", 13, 3);
//	game_set_cell_count(5, 5);
	t = 1;
}

void update(GAMETIME gametime)
{
	SOM& som = *SOM::globalSOM;
	Data& data = *Data::globalData;
	som.learnData(data, t);
	som.resetLabel();
	for (unsigned int i = 0; i < data.trainData.size(); i++) {
		som.Labeling(data.trainData[i]->input, data.trainData[i]->label);
	}
	t++;
}

void draw(GAMETIME gametime)
{
	draw_begin();
	draw_clear(BLACK);
	SOM::globalSOM->drawMap();
	// TODO: ���� ȭ�鿡 ����� ���� �ۼ�.
	//draw_charc(' ', 2, 2, WHITE, WHITE);	
	draw_end();
}