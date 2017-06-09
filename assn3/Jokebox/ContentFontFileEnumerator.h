#pragma once

#include <Windows.h>
#include <dwrite.h>
#include <vector>
#include <cwchar>
#include <string>
#include "ContentFontCollectionLoader.h"

using std::wstring;

class ContentFontFileEnumerator : public IDWriteFontFileEnumerator
{
private:
	ULONG _refCount;

	IDWriteFactory* _dw_factory;
	IDWriteFontFile* _dw_curFile;
	std::vector<wstring> _fonts; 
	size_t _nextIdx;

	wstring root;

public:
	ContentFontFileEnumerator(IDWriteFactory* factory, const wchar_t* contentPath);
	~ContentFontFileEnumerator();

	//IUnknown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	//IDWriteFontFileEnumerator
	virtual HRESULT STDMETHODCALLTYPE MoveNext(BOOL* hasCurrentFile);
	virtual HRESULT STDMETHODCALLTYPE GetCurrentFontFile(IDWriteFontFile** fontfile);
};