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
//Sound ��Ʈ: http://www.rastertek.com/dx11tut14.html �뷮 ����

#include "fn_content.h"
#include "fn_game.h"
#include "fn_draw.h"
#include "fn_engine.h"
#include "fn_debug.h"
#include "ContentFontCollectionLoader.h"
#include <cwchar>
#include <cstring>
#include <algorithm>
#include <wincodec.h>					// �̹��� ���ڵ� ���̺귯��
#pragma comment (lib, "windowscodecs")	// �̹��� ���ڵ� ���̺귯��
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

// ����Ʈ ���
static const wchar_t* font_root = L"Content\\Font";
static const wchar_t* texture_root = L"Content\\Texture";
static const char* sound_root = "Content\\Sound";

// ���� �Լ�
SoundFileFormat find_sound_file_format(const char* path);
void load_wav_file(const char* path, IDirectSoundBuffer** buffer);
void load_ogg_file(const char* path, IDirectSoundBuffer** buffer);

// ���� �Լ�
void read_wave_header(FILE* file, WaveRIFFChunkDescriptor* riff, WaveFMTSubChunk* fmt, WaveDataSubChunk* data);
FONT regist_font(IDWriteTextFormat* font, IDWriteFontFace* fface);
TEXTURE regist_texture(ID2D1Bitmap* texture);
SOUND regist_sound(IDirectSoundBuffer* sound);

void _content_initialize()
{
	HRESULT hr;

	/* font ������ �ʱ�ȭ */
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
			"DirectWrite Factory ���� ����",
			"(���� �Լ�)");
	
		//����Ʈ ��Ʈ �ҷ�����
	DWORD filetype = GetFileAttributesW(font_root);
	if (filetype != INVALID_FILE_ATTRIBUTES)
	{
		ContentFontCollectionLoader* loader = ContentFontCollectionLoader::GetLoader();
		ContentFontCollectionLoader::SetFontContentFolder(font_root);
		HRESULT hr = dw_factory->RegisterFontCollectionLoader(loader);
		
		if (!SUCCEEDED(hr))
			_debug_raise_error(__FILE__, __LINE__,
				"CustomFontLoader ��� ����",
				"(���� �Լ�)");

		static UINT cid = 0xffff;
		hr = dw_factory->CreateCustomFontCollection(loader, &cid, sizeof(cid), &content_font);			//���⼭ �ƿ� ���޾ƿ�
		
		if (!SUCCEEDED(hr))
			_debug_raise_error(__FILE__, __LINE__,
				"CustomFont ������ �ҷ����µ� ����",
				"(���� �Լ�)");

		dw_factory->UnregisterFontCollectionLoader(loader);
	}

	/* sound ������ �ʱ�ȭ */
	DSBUFFERDESC buffer;
	WAVEFORMATEX format;

	hr = DirectSoundCreate8(NULL, &dsound8, NULL);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"DirectSound8 ���� ����",
			"(���� �Լ�)");
	
	hr = dsound8->SetCooperativeLevel(_engine_get_hwnd(), DSSCL_PRIORITY);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"DirectSound CooperativeLevel ���� ����",
			"(���� �Լ�)");
	
	buffer.dwSize = sizeof(DSBUFFERDESC);
	buffer.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	buffer.dwBufferBytes = 0;
	buffer.dwReserved = 0;
	buffer.lpwfxFormat = NULL;
	buffer.guid3DAlgorithm = GUID_NULL;

	hr = dsound8->CreateSoundBuffer(&buffer, &primary_buf, NULL);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"DirectSound �� ���� ���� ����",
			"(���� �Լ�)");
	
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
			"DirectSound �� ���� ���� �ʱ�ȭ ����",
			"(���� �Լ�)");

	/* texture ������ �ʱ�ȭ */
	hr = CoCreateInstance(CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&iwic_factory);
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"WinCodec Factory ���� ����",
			"(���� �Լ�)");

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

//�ý��� ��Ʈ�� �ҷ���.
FONT _content_load_font(char* name, float weight, float pxSize)
{
	IDWriteTextFormat* font;
	wchar_t wname[300];
	int iweight;
	IDWriteFontCollection* collection;
	
	iweight = max(min((int)(weight * 10) * 100, 900), 100);

	//Content�� �ִ� ��Ʈ���� ���� Ȯ��.
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

	//name�� ������ path(./Content/Texture/<..>)�� ����
	MultiByteToWideChar(CP_ACP, 0, name, strlen(name) + 1, wcs, 300);
	wcscpy(path, texture_root);
	wcscat(path, L"\\");
	wcscat(path, wcs);
	
	//������ ������� Ȯ��: (�ַ�� ����)/(���� ������Ʈ ����)/Content/Texture/(name) ���� ã��.
	wchar_t buf[300] = {0};
	DWORD filetype = GetFileAttributesW(path);
	GetFullPathName(path, 300, buf, NULL);

	if (filetype == INVALID_FILE_ATTRIBUTES)
	{
		char mbspath[300];
		wcstombs(mbspath, buf, 300);
		_debug_raise_error(__FILE__, __LINE__,
			"����Ʈ�� ã�� �� ����",
			"_content_load_texture",
			std::string("����(���: ") + std::string(mbspath)
				+ std::string(")�� ã�� �� �����ϴ�."));
	}

	//�׸� �ҷ�����
	hr = iwic_factory->CreateDecoderFromFilename(path, NULL, GENERIC_READ,
		WICDecodeMetadataCacheOnLoad, &decoder);
	
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"�������� �ʴ� ����Ʈ ����",
			"_content_load_texture",
			"������ ����(" + std::string(name) + ")�� ������ �������� �������� �ʴ� �����Դϴ�.");

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
	
	//������ ������� Ȯ��: (�ַ�� ����)/(���� ������Ʈ ����)/Content/Sound/(name) ���� ã��.
	char buf[300] = {0};
	DWORD filetype = GetFileAttributesA(path);
	GetFullPathNameA(path, 300, buf, NULL);
	
	if (filetype == INVALID_FILE_ATTRIBUTES)
	{
		_debug_raise_error(__FILE__, __LINE__,
			"����Ʈ�� ã�� �� ����",
			"_content_load_texture",
			std::string("����(���: ") + std::string(buf)
				+ std::string(")�� ã�� �� �����ϴ�."));
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
			"�������� �ʴ� ����Ʈ ����",
			"_content_load_sound",

			"������ ����(" + std::string(name) + ")�� ������ �������� �������� �ʴ� �����Դϴ�.");
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

	// �˻縦 ���� ����
	fseek(file, 0, SEEK_SET);
	fread(&riff, sizeof(WaveRIFFChunkDescriptor), 1, file);
	ogg_failed = ov_fopen(path, &ogg);
	
	// ���� ��Ī
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

	// Wave ��� �б�
	read_wave_header(file, &riff, &fmt, &data);
	
	// WaveFormat ����ü �� Buffer Description ����
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

	// ���� ����
	hr = dsound8->CreateSoundBuffer(&buf_desc, buffer, NULL);	
	if (!SUCCEEDED(hr))
		_debug_raise_error(__FILE__, __LINE__,
			"DirectSound ���� ���� ����",
			"_content_load_sound",
			"����Ʈ ����(" + std::string(path) + ")�� �ջ�Ǿ��� �� �ֽ��ϴ�.");

	fseek(file, 44 + format.cbSize, SEEK_SET);
	
	// ���̺� ������ ���� �Ҵ�
	try {
		sound_buf = new unsigned char[data.dataSize];
	} catch (std::bad_alloc ex) {
		_debug_raise_error(__FILE__, __LINE__,
			"���� ���� ���� ����",
			"_content_load_sound",
			"���� ����(" + std::string(path) + ")�� �ʹ� Ů�ϴ�.");
	}

	// Wave ������ ���� ä���
	(*buffer)->Lock(0, data.dataSize, (void**)&sound_buf, &data.dataSize, NULL, NULL, 0);
	fread(sound_buf, 1, data.dataSize, file);
	(*buffer)->Unlock((void*)sound_buf, data.dataSize, NULL, NULL);

	// ���� ��� ��ġ�� 0����
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

	// OGG �б� �� ���� ��������
	ov_fopen(path, &ogg_file);
	ogg_info = ov_info(&ogg_file, -1);

	// Wave ���� ����
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

	// vector�� �ӽ÷� �ҷ�����
	while ((count = ov_read(&ogg_file, tmp_buf, sizeof(tmp_buf), 0, 2, 1, &sec)) > 0)
		sound_buf_vec.insert(sound_buf_vec.end(), tmp_buf, tmp_buf + count);

	// ������ ���� �迭�� �ű��
	buf_desc.dwBufferBytes = sound_buf_vec.size();
	sound_buf = new char[sound_buf_vec.size()];

	// ���ۿ� ������ �����ϱ�
	count = sound_buf_vec.size();
	dsound8->CreateSoundBuffer(&buf_desc, &sound_dsbuf, NULL);
	sound_dsbuf->Lock(0, sound_buf_vec.size(), (void**)&sound_buf, (LPDWORD)&count, NULL, NULL, 0);
	std::copy(sound_buf_vec.begin(), sound_buf_vec.end(), sound_buf);
	sound_dsbuf->Unlock(sound_buf, count, NULL, NULL);

	sound_dsbuf->SetCurrentPosition(0);

	ov_clear(&ogg_file);

	*buffer = sound_dsbuf;
}


 //���� �Լ�

void read_wave_header(FILE* file, WaveRIFFChunkDescriptor* riff, WaveFMTSubChunk* fmt, WaveDataSubChunk* data)
{
	//WaveRIFF chunk descriptor, WaveFMT sub chunk �б�
	fseek(file, 0, SEEK_SET);
	fread(riff, sizeof(WaveRIFFChunkDescriptor), 1, file);
	fread(fmt, sizeof(WaveFMTSubChunk), 1, file);
	
	//sub chunk�� ���� ���̿� sizeof(WaveFMTSubChunk) ����ŭ �б� ��ġ ����
	fseek(file, fmt->subChunkSize - WAVE_FMT_HEAD_DEF_SIZE, SEEK_CUR);
	fread(data, sizeof(WaveDataSubChunk), 1, file);
}

//IDWriteTextFormat�� ����ϰ�, ��ϵ� �ε����� ����.
FONT regist_font(IDWriteTextFormat* font, IDWriteFontFace* fface)
{
	bool registed = false;
	for (unsigned int i = 1; i < font_len; i++)
	{
		//�߰��� ������� �ڵ� ���
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
				"�޸� �Ҵ� ����",
				"(���� �Լ�)",
				"�޸𸮿� �Ҵ��� �� �ִ� �ͺ��� ���� ��Ʈ�� �ε��Ϸ� �Ͽ����ϴ�.");
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
		//�߰��� ������� �ڵ� ���
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
				"�޸� �Ҵ� ����",
				"(���� �Լ�)",
				"�޸𸮿� �Ҵ��� �� �ִ� �ͺ��� ���� �ؽ�ó�� �ε��Ϸ� �Ͽ����ϴ�.");
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
		//�߰��� ������� �ڵ� ���
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
				"�޸� �Ҵ� ����",
				"(���� �Լ�)",
				"�޸𸮿� �Ҵ��� �� �ִ� �ͺ��� ���� ���带 �ε��Ϸ� �Ͽ����ϴ�.");
		}

		std::copy(sound_list, sound_list + sound_len, buf);
		delete[] sound_list;
		sound_list = buf;

		sound_list[sound_len] = sound;
		sound_len++;
	}

	return sound_len - 1;
}