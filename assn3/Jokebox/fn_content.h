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
#include <d2d1.h>
#include <dwrite.h>
#include <dsound.h>
#pragma comment (lib, "dsound")
#pragma comment (lib, "dxguid")				// dxguid.dll : DirectX SDK (March 2009)

typedef unsigned int FONT;
typedef unsigned int TEXTURE;
typedef unsigned int SOUND;

//http://www.rastertek.com/dx11tut14.html

#define WAVE_RIFF_HEAD_DEF_SIZE 12
#define WAVE_FMT_HEAD_DEF_SIZE 16
#define WAVE_DATA_HEAD_DEF_SIZE 8

struct WaveRIFFChunkDescriptor
{
	char chunkId[4];
	unsigned long chunkSize;
	char format[4];
};

struct WaveFMTSubChunk
{
	char subChunkId[4];
	unsigned long subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
};

struct WaveDataSubChunk
{
	char dataChunkId[4];
	unsigned long dataSize;
};

void _content_initialize();

FONT _content_load_font(char* name, float weight, float pxSize);
TEXTURE _content_load_texture(char* name);
SOUND _content_load_sound(char* name);

IDWriteTextFormat* _content_get_font(FONT font);
IDWriteFontFace* _content_get_font_face(FONT font);
ID2D1Bitmap* _content_get_texture(TEXTURE texture);

IDirectSoundBuffer* _content_get_sound(SOUND sound);