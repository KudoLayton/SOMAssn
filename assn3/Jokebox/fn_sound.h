/*----------------------------------------------------------*
 *															*
 *					   Jokebox v4.051						*
 *															*
 * 				�̱����� ���� �ۼ��� Jokebox��				*
 *		ũ������Ƽ�� Ŀ���� ������ǥ��-�������Ǻ������		*
 * 		3.0 Unported ���̼����� ���� �̿��� �� �ֽ��ϴ�.	*
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
