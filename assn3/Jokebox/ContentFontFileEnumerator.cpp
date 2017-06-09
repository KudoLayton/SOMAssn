#include "ContentFontFileEnumerator.h"

using std::wstring;

ContentFontFileEnumerator::ContentFontFileEnumerator(IDWriteFactory* factory, const wchar_t* contentPath)
{
	_refCount = 0;
	_dw_factory = factory;
	_dw_curFile = 0;
	_nextIdx = 0;
		
	root = wstring(contentPath);

	//폰트 찾기
	WIN32_FIND_DATA find_data;
	HANDLE handle;
	wstring search_path = root + L"\\*";

	handle = FindFirstFile(search_path.c_str(), &find_data);
	do
	{
		if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			_fonts.push_back(root + L"\\" + find_data.cFileName);
	} while (FindNextFile(handle, &find_data) != 0);
	FindClose(handle);
}

ContentFontFileEnumerator::~ContentFontFileEnumerator()
{
	if (_dw_curFile != NULL)
		_dw_curFile->Release();
}


//IUnknown
HRESULT STDMETHODCALLTYPE ContentFontFileEnumerator::QueryInterface(REFIID iid, void** ppvObject)
{
	if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontFileEnumerator))
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
ULONG STDMETHODCALLTYPE ContentFontFileEnumerator::AddRef()
{
	return InterlockedIncrement(&_refCount);
}
ULONG STDMETHODCALLTYPE ContentFontFileEnumerator::Release()
{
	ULONG newCount = InterlockedDecrement(&_refCount);
	if (newCount == 0)
		delete this;

	return newCount;
}

//IDWriteFontFileEnumerator
HRESULT STDMETHODCALLTYPE ContentFontFileEnumerator::MoveNext(BOOL* hasCurrentFile)
{
	HRESULT hr = S_OK;
	wchar_t buf[300] = {0};

	*hasCurrentFile = FALSE;
	if (_dw_curFile != NULL)
	{
		_dw_curFile->Release();
		_dw_curFile = NULL;
	}

	if (_nextIdx < _fonts.size())
	{
		wcscpy(buf, _fonts[_nextIdx].c_str());
		hr = _dw_factory->CreateFontFileReference(buf, NULL, &_dw_curFile);

		if (SUCCEEDED(hr))
		{
			*hasCurrentFile = TRUE;
			++_nextIdx;
		}
	}

	return hr;
}

HRESULT STDMETHODCALLTYPE ContentFontFileEnumerator::GetCurrentFontFile(IDWriteFontFile** fontfile)
{
	if (_dw_curFile)
	{
		*fontfile = _dw_curFile;
		_dw_curFile->AddRef();
		return S_OK;
	}
	else
		return E_FAIL;
}