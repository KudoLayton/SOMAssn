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

	// TODO: ���� ȭ�鿡 ����� ���� �ۼ�.
	
	draw_end();
}