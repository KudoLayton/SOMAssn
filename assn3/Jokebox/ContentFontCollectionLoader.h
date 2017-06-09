#pragma once

#include <dwrite.h>
#include <cwchar>
#include "ContentFontFileEnumerator.h"

class ContentFontCollectionLoader : public IDWriteFontCollectionLoader
{
private:
	ULONG _refCount;
	static ContentFontCollectionLoader* _instance;
	static wchar_t* _content_path;

	ContentFontCollectionLoader();

public:
	static ContentFontCollectionLoader* GetLoader();
	static void SetFontContentFolder(const wchar_t* path);

	//IUnknown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	//IDWriteFontCollectionLoader
	virtual HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(IDWriteFactory* factory,
		void const* collectionKey,
		UINT32 collectionKeySize,
		IDWriteFontFileEnumerator** fontFileEnumerator);
};