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

#pragma once
#include "fn_content.h"
#include <Windows.h>

void _sound_play(SOUND sound, int rewind, int loopEnabled);
void _sound_pause(SOUND sound);
void _sound_stop(SOUND sound);
int _sound_is_playing(SOUND sound);

/* Deprecated */
//void _sound_bgm_play(SOUND sound, int loopEnabled);
//void _sound_bgm_stop();
//int _sound_is_bgm_playing();
