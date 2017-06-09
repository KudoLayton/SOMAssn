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
//Sound 파트: http://www.rastertek.com/dx11tut14.html 대량 참조

#include "fn_content.h"
#include "fn_game.h"
#include "fn_draw.h"
#include "fn_engine.h"
#include "fn_debug.h"
#include "ContentFontCollectionLoader.h"
#include <cwchar>
#include <cstring>
#include <algorithm>
#include <wincodec.h>					// 이미지 디코딩 라이브러리
#pragma comment (lib, "windowscodecs")	// 이미지 디코딩 라이브러리
#include <new>
#include <d2d1helper.h>
#include "vorbis/codec.h"				// OggVorbis 1.3.3
#include "vorbis/vorbisfile.h"			// OggVorbis 1.3.3
//#ifdef _DEBUG
//#pragma comment (lib, "libogg_static_d")
//#pragma comment (lib, "libvorbis_static_d")
//#pragma comment (lib, "libvorbisfile_static_d")
//#else
//#pragma comment (lib, "libogg_static")
//#pragma comment (lib, "libvorbis_static")
//#pragma comment (lib, "libvorbisfile_static")
//#endif
#include <vector>

enum SoundFileFormat { NOT_SUPPORTED = -1, WAV, OGG };

// Font Content
static IDWriteFactory* dw_factory;
static IDWriteFontCollection* content_font;
static IDWriteTextFormat** font_list;
static IDWriteFontFace** fface_list;
static unsigned int font_len;

// Texture Content
static IWICImagingFactory* iwic_factory;
static ID2D1Bitmap** texture_list;
static unsigned int texture_len;

// Sound Content
static IDirectSound8* dsound8;
static IDirectSoundBuffer* primary_buf;
static IDirectSoundBuffer** sound_list;
static unsigned int sound_len;

// 컨텐트 경로
static const wchar_t* font_root = L"Content\\Font";
static const wchar_t* texture_root = L"Content\\Texture";
static const char* sound_root = "Content\\Sound";

// 내부 함수
SoundFileFormat find_sound_file_format(const char* path);
void load_wav_file(const char* path, IDirectSoundBuffer** buffer);
void load_ogg_file(const char* path, IDirectSoundBuffer** buffer);

// 보조 함수
void read_wave_header(FILE* file, WaveRIFFChunkDescriptor* riff, WaveFMTSubChunk* fmt, WaveDataSubChunk* data);
FONT regist_font(IDWriteTextFormat* font, IDWriteFontFace* fface);
TEXTURE regist_texture(ID2D1Bitmap* texture);
SOUND regist_sound(IDirectSoundBuffer* sound);

void _content_initialize()
{
	HRESULT hr;

	/* font 생성기 초기화 */
	font_list = new IDWriteTextFormat*;
	font_list[0] = NULL;
	fface_list = new IDWriteFontFace*;
	fface_list[0] = NULL;
	font_len = 1;
	content_font = NULL;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&dw_factory));
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"DirectWrite Factory 생성 실패",
			"(내부 함수)");
	
		//컨텐트 폰트 불러오기
	DWORD filetype = GetFileAttributesW(font_root);
	if (filetype != INVALID_FILE_ATTRIBUTES)
	{
		ContentFontCollectionLoader* loader = ContentFontCollectionLoader::GetLoader();
		ContentFontCollectionLoader::SetFontContentFolder(font_root);
		HRESULT hr = dw_factory->RegisterFontCollectionLoader(loader);
		
		if (!SUCCEEDED(hr))
			_debug_raise_error(__FILE__, __LINE__,
				"CustomFontLoader 등록 실패",
				"(내부 함수)");

		static UINT cid = 0xffff;
		hr = dw_factory->CreateCustomFontCollection(loader, &cid, sizeof(cid), &content_font);			//여기서 아예 못받아옴
		
		if (!SUCCEEDED(hr))
			_debug_raise_error(__FILE__, __LINE__,
				"CustomFont 정보를 불러오는데 실패",
				"(내부 함수)");

		dw_factory->UnregisterFontCollectionLoader(loader);
	}

	/* sound 생성기 초기화 */
	DSBUFFERDESC buffer;
	WAVEFORMATEX format;

	hr = DirectSoundCreate8(NULL, &dsound8, NULL);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"DirectSound8 생성 실패",
			"(내부 함수)");
	
	hr = dsound8->SetCooperativeLevel(_engine_get_hwnd(), DSSCL_PRIORITY);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"DirectSound CooperativeLevel 설정 실패",
			"(내부 함수)");
	
	buffer.dwSize = sizeof(DSBUFFERDESC);
	buffer.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	buffer.dwBufferBytes = 0;
	buffer.dwReserved = 0;
	buffer.lpwfxFormat = NULL;
	buffer.guid3DAlgorithm = GUID_NULL;

	hr = dsound8->CreateSoundBuffer(&buffer, &primary_buf, NULL);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"DirectSound 주 버퍼 생성 실패",
			"(내부 함수)");
	
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nSamplesPerSec = 44100;
	format.wBitsPerSample = 16;
	format.nChannels = 2;
	format.nBlockAlign = (format.wBitsPerSample / 8) * format.nChannels;
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
	format.cbSize = 0;

	hr = primary_buf->SetFormat(&format);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"DirectSound 주 버퍼 포맷 초기화 실패",
			"(내부 함수)");

	/* texture 생성기 초기화 */
	hr = CoCreateInstance(CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&iwic_factory);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"WinCodec Factory 생성 실패",
			"(내부 함수)");

	texture_list = new ID2D1Bitmap*;
	texture_list[0] = NULL;
	texture_len = 1;
}

IDWriteTextFormat* _content_get_font(FONT font)
{
	if (font_len <= font || font <= 0)
		return NULL;

	return font_list[font];
}

IDWriteFontFace* _content_get_font_face(FONT font)
{
	if (font_len <= font || font <= 0)
		return NULL;

	return fface_list[font];
}

ID2D1Bitmap* _content_get_texture(TEXTURE texture)
{
	if (texture_len <= texture || texture <= 0)
		return NULL;

	return texture_list[texture];
}

IDirectSoundBuffer* _content_get_sound(SOUND sound)
{
	if (sound_len <= sound || sound <= 0)
		return NULL;

	return sound_list[sound];
}

//시스템 폰트를 불러옴.
FONT _content_load_font(char* name, float weight, float pxSize)
{
	IDWriteTextFormat* font;
	wchar_t wname[300];
	int iweight;
	IDWriteFontCollection* collection;
	
	iweight = max(min((int)(weight * 10) * 100, 900), 100);

	//Content에 있는 폰트인지 여부 확인.
	MultiByteToWideChar(CP_ACP, 0, name, strlen(name) + 1, wname, 300);
	UINT32 res_idx; BOOL res = FALSE;
	if (content_font != NULL)
		content_font->FindFamilyName(wname, &res_idx, &res);

	if (res)
		collection = content_font;
	else
		collection = 0;

	dw_factory->CreateTextFormat(
		wname,
		content_font,
		(DWRITE_FONT_WEIGHT)iweight,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		pxSize * 0.88f,
		L"en-us",
		&font);
	font->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	
	IDWriteFontCollection* fcollection;
	IDWriteFontFamily* ffamily;
	IDWriteFont* ffont;
	IDWriteFontFace* fface;

	font->GetFontCollection(&fcollection);
	fcollection->GetFontFamily(0, &ffamily);		// FIXME: INDEX 0 IS PROBLEMATIC
	ffamily->GetFont(0, &ffont);
	ffont->CreateFontFace(&fface);

	return regist_font(font, fface);
}

TEXTURE _content_load_texture(char* name)
{
	HRESULT hr;
	ID2D1Bitmap* bitmap;
	IWICBitmapDecoder* decoder;
	IWICFormatConverter* converter;
	IWICBitmapFrameDecode* source;
	wchar_t path[300];
	wchar_t wcs[300];

	//name을 적절한 path(./Content/Texture/<..>)로 개조
	MultiByteToWideChar(CP_ACP, 0, name, strlen(name) + 1, wcs, 300);
	wcscpy(path, texture_root);
	wcscat(path, L"\\");
	wcscat(path, wcs);
	
	//적합한 경로인지 확인: (솔루션 폴더)/(게임 프로젝트 폴더)/Content/Texture/(name) 에서 찾음.
	wchar_t buf[300] = {0};
	DWORD filetype = GetFileAttributesW(path);
	GetFullPathName(path, 300, buf, NULL);

	if (filetype == INVALID_FILE_ATTRIBUTES)
	{
		char mbspath[300];
		wcstombs(mbspath, buf, 300);
		_debug_raise_error(__FILE__, __LINE__,
			"컨텐트를 찾을 수 없음",
			"_content_load_texture",
			std::string("파일(경로: ") + std::string(mbspath)
				+ std::string(")을 찾을 수 없습니다."));
	}

	//그림 불러오기
	hr = iwic_factory->CreateDecoderFromFilename(path, NULL, GENERIC_READ,
		WICDecodeMetadataCacheOnLoad, &decoder);
	
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"지원되지 않는 컨텐트 포맷",
			"_content_load_texture",
			"지정한 파일(" + std::string(name) + ")의 포맷이 엔진에서 지원하지 않는 포맷입니다.");

	decoder->GetFrame(0, &source);
	iwic_factory->CreateFormatConverter(&converter);
	converter->Initialize(source, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0, WICBitmapPaletteTypeMedianCut);

	_draw_get_render_target()->CreateBitmapFromWicBitmap(
		converter, NULL, &bitmap);

	if (bitmap == NULL)
		return 0;

	TEXTURE id = regist_texture(bitmap);

	decoder->Release();
	converter->Release();
	source->Release();

	return id;
}

SOUND _content_load_sound(char* name)
{
	//FILE* file; /* C4101: 'file': unreferenced local variable */
	IDirectSoundBuffer* buffer;
	char path[200];

	strcpy(path, sound_root);
	strcat(path, "\\");
	strcat(path, name);
	
	//적합한 경로인지 확인: (솔루션 폴더)/(게임 프로젝트 폴더)/Content/Sound/(name) 에서 찾음.
	char buf[300] = {0};
	DWORD filetype = GetFileAttributesA(path);
	GetFullPathNameA(path, 300, buf, NULL);
	
	if (filetype == INVALID_FILE_ATTRIBUTES)
	{
		_debug_raise_error(__FILE__, __LINE__,
			"컨텐트를 찾을 수 없음",
			"_content_load_texture",
			std::string("파일(경로: ") + std::string(buf)
				+ std::string(")을 찾을 수 없습니다."));
	}
	
	switch (find_sound_file_format(path))
	{
	case WAV:
		load_wav_file(path, &buffer);
		break;
	case OGG:
		load_ogg_file(path, &buffer);
		break;
	default:
		_debug_raise_error(__FILE__, __LINE__,
			"지원되지 않는 컨텐트 포맷",
			"_content_load_sound",

			"지정한 파일(" + std::string(name) + ")의 포맷이 엔진에서 지원하지 않는 포맷입니다.");
	}

	return regist_sound(buffer);
}

SoundFileFormat find_sound_file_format(const char* path)
{
	WaveRIFFChunkDescriptor riff;
	OggVorbis_File ogg;
	FILE* file;
	SoundFileFormat fformat = SoundFileFormat::NOT_SUPPORTED;
	int ogg_failed;
	
	file = fopen(path, "rb");

	// 검사를 위한 세팅
	fseek(file, 0, SEEK_SET);
	fread(&riff, sizeof(WaveRIFFChunkDescriptor), 1, file);
	ogg_failed = ov_fopen(path, &ogg);
	
	// 포맷 매칭
	if (riff.format[0] == 'W' && riff.format[1] == 'A'
		&& riff.format[2] == 'V' && riff.format[3] == 'E')
		fformat = SoundFileFormat::WAV;
	else if (!ogg_failed)
		fformat = SoundFileFormat::OGG;

	fclose(file);
	ov_clear(&ogg);

	return fformat;
}

void load_wav_file(const char* path, IDirectSoundBuffer** buffer)
{
	WAVEFORMATEX format;
	DSBUFFERDESC buf_desc;

	WaveRIFFChunkDescriptor riff;
	WaveFMTSubChunk fmt;
	WaveDataSubChunk data;
	unsigned char* sound_buf;
	FILE* file;

	HRESULT hr;

	file = fopen(path, "rb");

	// Wave 헤더 읽기
	read_wave_header(file, &riff, &fmt, &data);
	
	// WaveFormat 구조체 및 Buffer Description 설정
	format.wFormatTag = fmt.audioFormat;
	format.nSamplesPerSec = fmt.sampleRate;
	format.wBitsPerSample = fmt.bitsPerSample;
	format.nChannels = fmt.numChannels;
	format.nBlockAlign = fmt.blockAlign;
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
	format.cbSize = (WORD)(fmt.subChunkSize - WAVE_FMT_HEAD_DEF_SIZE);

	buf_desc.dwSize = sizeof(DSBUFFERDESC);
	buf_desc.dwFlags = DSBCAPS_CTRLVOLUME;
	buf_desc.dwBufferBytes = data.dataSize;
	buf_desc.dwReserved = 0;
	buf_desc.lpwfxFormat = &format;
	buf_desc.guid3DAlgorithm = GUID_NULL;

	// 버퍼 생성
	hr = dsound8->CreateSoundBuffer(&buf_desc, buffer, NULL);	
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"DirectSound 버퍼 생성 실패",
			"_content_load_sound",
			"컨텐트 파일(" + std::string(path) + ")이 손상되었을 수 있습니다.");

	fseek(file, 44 + format.cbSize, SEEK_SET);
	
	// 웨이브 데이터 버퍼 할당
	try {
		sound_buf = new unsigned char[data.dataSize];
	} catch (std::bad_alloc ex) {
		_debug_raise_error(__FILE__, __LINE__,
			"사운드 버퍼 생성 실패",
			"_content_load_sound",
			"사운드 파일(" + std::string(path) + ")이 너무 큽니다.");
	}

	// Wave 데이터 버퍼 채우기
	(*buffer)->Lock(0, data.dataSize, (void**)&sound_buf, &data.dataSize, NULL, NULL, 0);
	fread(sound_buf, 1, data.dataSize, file);
	(*buffer)->Unlock((void*)sound_buf, data.dataSize, NULL, NULL);

	// 버퍼 재생 위치를 0으로
	(*buffer)->SetCurrentPosition(0);

	fclose(file);
}

void load_ogg_file(const char* path, IDirectSoundBuffer** buffer)
{
	WAVEFORMATEX format;
	DSBUFFERDESC buf_desc;

	OggVorbis_File ogg_file;
	vorbis_info* ogg_info;
	int sec;
	int count;
	char tmp_buf[4096];
	std::vector<char> sound_buf_vec;
	char* sound_buf;

	IDirectSoundBuffer* sound_dsbuf;

	// HRESULT hr; /* C4101: 'hr': unreferenced local variable */

	// OGG 읽기 및 정보 가져오기
	ov_fopen(path, &ogg_file);
	ogg_info = ov_info(&ogg_file, -1);

	// Wave 포맷 설정
	format.cbSize = sizeof(format);
	format.nChannels = ogg_info->channels;
	format.wBitsPerSample = 16;
	format.nSamplesPerSec = ogg_info->rate;
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nChannels * 2;
	format.nBlockAlign = 2 * format.nChannels;
	format.wFormatTag = 1;

	buf_desc.dwSize = sizeof(buf_desc);
	buf_desc.dwFlags = 0;
	buf_desc.lpwfxFormat = &format;
	buf_desc.dwReserved = 0;
	buf_desc.dwBufferBytes = 0;			// Accumurated later

	// vector에 임시로 불러오기
	while ((count = ov_read(&ogg_file, tmp_buf, sizeof(tmp_buf), 0, 2, 1, &sec)) > 0)
		sound_buf_vec.insert(sound_buf_vec.end(), tmp_buf, tmp_buf + count);

	// 데이터 버퍼 배열로 옮기기
	buf_desc.dwBufferBytes = sound_buf_vec.size();
	sound_buf = new char[sound_buf_vec.size()];

	// 버퍼에 데이터 연결하기
	count = sound_buf_vec.size();
	dsound8->CreateSoundBuffer(&buf_desc, &sound_dsbuf, NULL);
	sound_dsbuf->Lock(0, sound_buf_vec.size(), (void**)&sound_buf, (LPDWORD)&count, NULL, NULL, 0);
	std::copy(sound_buf_vec.begin(), sound_buf_vec.end(), sound_buf);
	sound_dsbuf->Unlock(sound_buf, count, NULL, NULL);

	sound_dsbuf->SetCurrentPosition(0);

	ov_clear(&ogg_file);

	*buffer = sound_dsbuf;
}


 //보조 함수

void read_wave_header(FILE* file, WaveRIFFChunkDescriptor* riff, WaveFMTSubChunk* fmt, WaveDataSubChunk* data)
{
	//WaveRIFF chunk descriptor, WaveFMT sub chunk 읽기
	fseek(file, 0, SEEK_SET);
	fread(riff, sizeof(WaveRIFFChunkDescriptor), 1, file);
	fread(fmt, sizeof(WaveFMTSubChunk), 1, file);
	
	//sub chunk의 실제 길이와 sizeof(WaveFMTSubChunk) 차만큼 읽기 위치 당기기
	fseek(file, fmt->subChunkSize - WAVE_FMT_HEAD_DEF_SIZE, SEEK_CUR);
	fread(data, sizeof(WaveDataSubChunk), 1, file);
}

//IDWriteTextFormat을 등록하고, 등록된 인덱스를 리턴.
FONT regist_font(IDWriteTextFormat* font, IDWriteFontFace* fface)
{
	bool registed = false;
	for (unsigned int i = 1; i < font_len; i++)
	{
		//중간이 비었으면 자동 등록
		if (font_list[i] == NULL)
		{
			font_list[i] = font;
			fface_list[i] = fface;
			registed = true;
			return i;
		}
	}

	if (!registed)
	{
		IDWriteTextFormat** buf;
		IDWriteFontFace** buf_face;

		try{
			buf = new IDWriteTextFormat*[font_len + 1];
			buf_face = new IDWriteFontFace*[font_len + 1];
		} catch (std::bad_alloc ex) {
			_debug_raise_error(__FILE__, __LINE__,
				"메모리 할당 실패",
				"(내부 함수)",
				"메모리에 할당할 수 있는 것보다 많은 폰트를 로드하려 하였습니다.");
		}

		std::copy(font_list, font_list + font_len, buf);
		std::copy(fface_list, fface_list + font_len, buf_face);
		delete[] font_list;
		delete[] fface_list;
		font_list = buf;
		fface_list = buf_face;

		font_list[font_len] = font;
		fface_list[font_len] = fface;
		font_len++;
	}

	return font_len - 1;
}

TEXTURE regist_texture(ID2D1Bitmap* texture)
{
	bool registed = false;
	for (unsigned int i = 1; i < texture_len; i++)
	{
		//중간이 비었으면 자동 등록
		if (texture_list[i] == NULL)
		{
			texture_list[i] = texture;
			registed = true;
			return i;
		}
	}

	if (!registed)
	{
		ID2D1Bitmap** buf;
		
		try {
			buf = new ID2D1Bitmap*[texture_len + 1];
		} catch (std::bad_alloc ex) {
			_debug_raise_error(__FILE__, __LINE__,
				"메모리 할당 실패",
				"(내부 함수)",
				"메모리에 할당할 수 있는 것보다 많은 텍스처를 로드하려 하였습니다.");
		}

		std::copy(texture_list, texture_list + texture_len, buf);
		delete[] texture_list;
		texture_list = buf;

		texture_list[texture_len] = texture;
		texture_len++;
	}

	return texture_len - 1;
}

SOUND regist_sound(IDirectSoundBuffer* sound)
{
	bool registed = false;
	for (unsigned int i = 1; i < sound_len; i++)
	{
		//중간이 비었으면 자동 등록
		if (sound_list[i] == NULL)
		{
			sound_list[i] = sound;
			registed = true;
			return i;
		}
	}

	if (!registed)
	{
		IDirectSoundBuffer** buf;

		try {
			buf = new IDirectSoundBuffer*[sound_len + 1];
		} catch (std::bad_alloc ex) {
			_debug_raise_error(__FILE__, __LINE__,
				"메모리 할당 실패",
				"(내부 함수)",
				"메모리에 할당할 수 있는 것보다 많은 사운드를 로드하려 하였습니다.");
		}

		std::copy(sound_list, sound_list + sound_len, buf);
		delete[] sound_list;
		sound_list = buf;

		sound_list[sound_len] = sound;
		sound_len++;
	}

	return sound_len - 1;
}