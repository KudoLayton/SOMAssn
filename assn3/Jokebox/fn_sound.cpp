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
		_debug_raise_error(__FILE__, __LINE__, "NULL 포인터에 접근",
			"_sound_play",
			"잘못된 ID로 컨텐트에 접근하였을 수 있습니다.");

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
		_debug_raise_error(__FILE__, __LINE__, "NULL 포인터에 접근",
			"_sound_pause",
			"잘못된 ID로 컨텐트에 접근하였을 수 있습니다.");

	buf->Stop();
}

void _sound_stop(SOUND sound)
{
	IDirectSoundBuffer* buf = _content_get_sound(sound);
	if (buf == NULL)
		_debug_raise_error(__FILE__, __LINE__, "NULL 포인터에 접근",
			"_sound_stop",
			"잘못된 ID로 컨텐트에 접근하였을 수 있습니다.");

	buf->Stop();
	buf->SetCurrentPosition(0);
}

int _sound_is_playing(SOUND sound)
{
	IDirectSoundBuffer* buf = _content_get_sound(sound);
	if (buf == NULL)
		_debug_raise_error(__FILE__, __LINE__, "NULL 포인터에 접근",
			"_sound_is_playing",
			"잘못된 ID로 컨텐트에 접근하였을 수 있습니다.");
	
	DWORD status;
	buf->GetStatus(&status);
	return (status == DSBSTATUS_PLAYING);
}