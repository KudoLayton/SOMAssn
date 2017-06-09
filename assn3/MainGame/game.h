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
