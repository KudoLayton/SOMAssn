#include "ContentFontCollectionLoader.h"

ContentFontCollectionLoader* ContentFontCollectionLoader::_instance = new ContentFontCollectionLoader();
wchar_t* ContentFontCollectionLoader::_content_path = 0;

ContentFontCollectionLoader::ContentFontCollectionLoader() : _refCount(0) {}

ContentFontCollectionLoader* ContentFontCollectionLoader::GetLoader()
{
	return _instance;
}

void ContentFontCollectionLoader::SetFontContentFolder(const wchar_t* path)
{
	_content_path = const_cast<wchar_t*>(path);
}

//IUnknown
HRESULT STDMETHODCALLTYPE ContentFontCollectionLoader::QueryInterface(IID const& iid, void** ppvObject)
{
	if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontCollectionLoader))
	{
		*ppvObject = this;
		AddRef();
		return S_OK;
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
}
ULONG STDMETHODCALLTYPE ContentFontCollectionLoader::AddRef()
{
	return InterlockedIncrement(&_refCount);
}
ULONG STDMETHODCALLTYPE ContentFontCollectionLoader::Release()
{
	ULONG newCount = InterlockedDecrement(&_refCount);
	if (newCount == 0)
		delete this;

	return newCount;
}

//IDWriteFontCollectionLoader
HRESULT STDMETHODCALLTYPE ContentFontCollectionLoader::CreateEnumeratorFromKey(IDWriteFactory* factory,
	void const* collectionKey,
	UINT32 collectionKeySize,
	__out IDWriteFontFileEnumerator** fontFileEnumerator)
{
	*fontFileEnumerator = NULL;
		
	HRESULT hr = S_OK;

	//if (collectionKeySize % sizeof(UINT) != 0)
	//	return E_INVALIDARG;

	IDWriteFontFileEnumerator* en = new ContentFontFileEnumerator(factory, _content_path);
	en->AddRef();
	*fontFileEnumerator = en;

	return hr;
}