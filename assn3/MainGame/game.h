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

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef GAME
#define GAME

#include "jokebox.h"

void initialize();
void update(GAMETIME gametime);
void draw(GAMETIME gametime);
void finalize();

#endif

#ifdef __cplusplus
};
#endif
