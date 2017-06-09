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

#include "fn_sound.h"
#include "fn_content.h"
#include "fn_debug.h"
#include <dsound.h>
#include <cassert>
#pragma comment (lib, "dsound")

void _sound_play(SOUND sound, int rewind, int loopEnabled)
{
	IDirectSoundBuffer* buf = _content_get_sound(sound);
	if (buf == NULL)
		_debug_raise_error(__FILE__, __LINE__, "NULL �����Ϳ� ����",
			"_sound_play",
			"�߸��� ID�� ����Ʈ�� �����Ͽ��� �� �ֽ��ϴ�.");

	if (rewind)
		buf->SetCurrentPosition(0);

	buf->SetVolume(DSBVOLUME_MAX);

	if (loopEnabled)
		buf->Play(0, 0, DSBPLAY_LOOPING);
	else
		buf->Play(0, 0, 0);
}

void _sound_pause(SOUND sound)
{
	IDirectSoundBuffer* buf = _content_get_sound(sound);
	if (buf == NULL)
		_debug_raise_error(__FILE__, __LINE__, "NULL �����Ϳ� ����",
			"_sound_pause",
			"�߸��� ID�� ����Ʈ�� �����Ͽ��� �� �ֽ��ϴ�.");

	buf->Stop();
}

void _sound_stop(SOUND sound)
{
	IDirectSoundBuffer* buf = _content_get_sound(sound);
	if (buf == NULL)
		_debug_raise_error(__FILE__, __LINE__, "NULL �����Ϳ� ����",
			"_sound_stop",
			"�߸��� ID�� ����Ʈ�� �����Ͽ��� �� �ֽ��ϴ�.");

	buf->Stop();
	buf->SetCurrentPosition(0);
}

int _sound_is_playing(SOUND sound)
{
	IDirectSoundBuffer* buf = _content_get_sound(sound);
	if (buf == NULL)
		_debug_raise_error(__FILE__, __LINE__, "NULL �����Ϳ� ����",
			"_sound_is_playing",
			"�߸��� ID�� ����Ʈ�� �����Ͽ��� �� �ֽ��ϴ�.");
	
	DWORD status;
	buf->GetStatus(&status);
	return (status == DSBSTATUS_PLAYING);
}