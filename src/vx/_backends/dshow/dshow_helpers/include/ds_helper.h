/* -*-c++-*-
*
* This file is part of SSTT and licensed under the terms in LICENSE.txt
*
* SSTT - Simplified Spatial Target Tracker
* (c) Copyright 2008-2009 Hartmut Seichter, All Rights Reserved
*
* SSTT is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
*/

#ifndef _WIN_DIRECTHOW_H_
#define _WIN_DIRECTHOW_H_


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#if defined(__MINGW32__)
#include <ocidl.h>
#include <basetyps.h>
#include <oleauto.h>
#include <strmif.h>
//#include "missing_mingw440.h"

#endif

#if !defined(_WIN32_WCE) && !defined(__MINGW32__)
    #include <atlbase.h>
#endif //!defined(WINCE)

#include <dshow.h>
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__

#if !defined(_WIN32_WCE)
#include <strmif.h>
#include <qedit.h>

	#pragma comment(lib,"uuid")
	#pragma comment(lib,"strmiids")
	#pragma comment(lib,"winmm")
#else
	#pragma comment(lib,"uuid")
	#pragma comment(lib,"strmiids")
	#pragma comment(lib,"strmbase")
#endif

#include "cpropertybag.h"


//#include <atlbase.h>
#include <olectl.h>
#include <oleauto.h>


/**
 * @brief Helper to convert from wchar to UTF8 or other code pages
 */
struct WideCharacterConversion {

    char* multiByte;

    WideCharacterConversion()
        : multiByte(0)
    {
    }

    void destroy()
    {
        if (multiByte) {
            ::free(multiByte);
            multiByte = 0;
        }
    }

    ~WideCharacterConversion()
    {
        this->destroy();
    }

    const char* mb_str() const { return multiByte; }

    const char* operator()(const LPWSTR input,unsigned int encoding = CP_UTF8)
    {
        /* get length of input string */
        int inputLen = lstrlenW( input );

        /* get prospective size of output string */
        int outputLen =
            WideCharToMultiByte(encoding,0,
            input,inputLen,0,0,0,0);

        /* resize output buffer output buffer */
        char* newBuf = (char*)::realloc(multiByte,(outputLen+1) * sizeof(char));

        if (newBuf) {

            memset(newBuf,0,(outputLen+1)*sizeof(char));

            WideCharToMultiByte(encoding,0,
                input,inputLen,
                newBuf,outputLen,0,0);

            multiByte = newBuf;
        }

        return multiByte;
    }
};


#define UDSHOW_API inline

UDSHOW_API
HRESULT DisplayFilterProperties(IBaseFilter *pFilter, HWND hWnd)
{
	ISpecifyPropertyPages* pProp;
	HRESULT hr = pFilter->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pProp);
	if (SUCCEEDED(hr)) 
	{
		// Get the filter's name and IUnknown pointer.
		FILTER_INFO FilterInfo;
		pFilter->QueryFilterInfo(&FilterInfo); 

		// Show the page. 
		CAUUID caGUID;
		pProp->GetPages(&caGUID);
		OleCreatePropertyFrame(
			hWnd,                   // Parent window
			0, 0,                   // (Reserved)
			FilterInfo.achName,     // Caption for the dialog box
			1,                      // Number of objects (just the filter)
			(IUnknown **)&pFilter,  // Array of object pointers. 
			caGUID.cElems,          // Number of property pages
			caGUID.pElems,          // Array of property page CLSIDs
			0,                      // Locale identifier
			0, NULL);               // Reserved

		// Clean up.
		if(FilterInfo.pGraph != NULL) FilterInfo.pGraph->Release(); 
		CoTaskMemFree(caGUID.pElems);
	}
	return(hr);
}

UDSHOW_API
HRESULT DisplayPinProperties(IPin* pSrcPin, HWND hWnd)
{
	ISpecifyPropertyPages* pPages;

	HRESULT hr = pSrcPin->QueryInterface(IID_ISpecifyPropertyPages, (void**)&pPages);
	if (SUCCEEDED(hr))
	{
		PIN_INFO PinInfo;
		pSrcPin->QueryPinInfo(&PinInfo);

		CAUUID caGUID;
		pPages->GetPages(&caGUID);

		OleCreatePropertyFrame(
			hWnd,
			0,
			0,
			L"Property Sheet",
			1,
			(IUnknown **)&pSrcPin,
			caGUID.cElems,
			caGUID.pElems,
			0,
			0,
			NULL);
		CoTaskMemFree(caGUID.pElems);
		PinInfo.pFilter->Release();
	}
	else return(hr);

	return(S_OK);
}

UDSHOW_API
HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath) {

	const WCHAR wszStreamName[] = L"ActiveMovieGraph";

	HRESULT hr;

	IStorage *pStorage = NULL;

	// First, create a document file which will hold the GRF file

	hr = StgCreateDocfile(

		wszPath,

		STGM_CREATE | STGM_TRANSACTED | STGM_READWRITE |

		STGM_SHARE_EXCLUSIVE,

		0, &pStorage);

	if(FAILED(hr))

	{

		return hr;

	}

	// Next, create a stream to store.

	IStream *pStream;

	hr = pStorage->CreateStream(

		wszStreamName,

		STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,

		0, 0, &pStream);

	if (FAILED(hr))

	{

		pStorage->Release();

		return hr;

	}

	// The IPersistStream converts a stream into a persistent object.

	IPersistStream *pPersist = NULL;

	pGraph->QueryInterface(IID_IPersistStream,

		reinterpret_cast<void**>(&pPersist));

	hr = pPersist->Save(pStream, TRUE);

	pStream->Release();

	pPersist->Release();

	if (SUCCEEDED(hr))

	{

		hr = pStorage->Commit(STGC_DEFAULT);

	}

	pStorage->Release();

	return hr;

}



UDSHOW_API
void FindPin(IBaseFilter* baseFilter,
					PIN_DIRECTION direction,
					int pinNumber,
					IPin** destPin)
{

	IEnumPins* enumPins;

	*destPin = NULL;

	if (SUCCEEDED(baseFilter->EnumPins(&enumPins))) {
		ULONG numFound;
		IPin* tmpPin;

		while (SUCCEEDED(enumPins->Next(1, &tmpPin, &numFound))) {
			PIN_DIRECTION pinDirection;

			tmpPin->QueryDirection(&pinDirection);
			if (pinDirection == direction) {
				if (pinNumber == 0) {
					// Return the pin's interface
					*destPin = tmpPin;
					break;
				}
				pinNumber--;
			}
			tmpPin->Release();
		}
	}
}


inline char *
_ws2mbsConvert (const wchar_t * s, void *d)
{
	size_t size = wcslen (s);	//It maybe the same size as the widechar string
	if (!size && d)
		*((char *) d) = '\0';
	if (d && size)
	{
		((char *) d)[WideCharToMultiByte (CP_ACP,	// code page
			WC_COMPOSITECHECK,	// character-type options
			s,	// address of string to map
			size,	// number of chars in string
			(char *) d,	// address of mbs-character buffer
			size << 1,	// size of buffer in bytes ,        //Use system default character.
			NULL, NULL)] = L'\0';
	}
	return (char *) d;
}
// TODO: this requires sufficient stack for a copy of the string,
// which may simply not be available.
#define _ws2mbs(s) (_ws2mbsConvert((s),  _alloca( ((2+(wcslen((const wchar_t *)(s))<<1))))))


UDSHOW_API HRESULT GetCaptureDeviceWithUID(IBaseFilter** ppCap, const char* UID)
{
	HRESULT hr(E_FAIL);

#ifndef _WIN32_WCE

	ICreateDevEnum* pCreateDevEnum(NULL);

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, 0L, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void**)&pCreateDevEnum);

	if (!pCreateDevEnum || S_OK != hr)
	{
		MessageBox(NULL, _T("Error Creating a System Enumerator"), _T("uDirectShow"), MB_OK | MB_ICONEXCLAMATION);
		return hr;
	}

	// enumerate video capture devices
    IEnumMoniker *pEm(0L);

    hr = pCreateDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory, &pEm, 0 );

    if( hr != S_OK )
	{

		MessageBox(NULL,
			_T("Error creating VideoDeviceCategory enumerator"),
			_T("uDirectShow"),
			MB_OK | MB_ICONEXCLAMATION);

		pCreateDevEnum->Release();

		return hr;
	}

	pEm->Reset();

    // go through and find a suitable video capture device
	while( 1 )
	{
		ULONG ulFetched = 0L;
		IMoniker* pM = 0L;

		hr = pEm->Next( 1, &pM, &ulFetched );
		if( hr != S_OK )
			break;

		// get the property bag interface from the moniker
		IPropertyBag* pBag;
		hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );

        // bogus stuff
		if( hr != S_OK )
			continue;

		// ask for the readable name
		VARIANT var_name;
		var_name.vt = VT_BSTR;
		hr = pBag->Read( L"FriendlyName", &var_name, NULL );

        // get the UUID
		LPOLESTR disp_name;
		pM->GetDisplayName(NULL,NULL,&disp_name);

        WideCharacterConversion conv;
        conv(disp_name);

        // if the UUID is the same we can go ahead
        if (conv.mb_str() && (0 == strcmp(conv.mb_str(),UID)))
		{
			hr = pM->BindToObject( 0, 0, IID_IBaseFilter, (void**) ppCap );
			hr = S_OK;
		}

		pM->Release();

		if (*ppCap) break;

	}

	pCreateDevEnum->Release();
	pEm->Release();

#endif

	return hr;

}



#if !defined(_WIN32_WCE)
UDSHOW_API
HRESULT GetCaptureDevice(IBaseFilter **ppCap)
{
	HRESULT hr(E_FAIL);
	*ppCap = NULL;
	ICreateDevEnum* pCreateDevEnum(NULL);

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, 0L, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void**)&pCreateDevEnum);

	if (!pCreateDevEnum || S_OK != hr)
	{
		MessageBox(NULL, _T("Error Creating a System Enumerator"), _T("uDirectShow"), MB_OK | MB_ICONEXCLAMATION);
		return hr;
	}

	// enumerate video capture devices
	IEnumMoniker *pEm;
	pCreateDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory, &pEm, 0 );

	if( !pEm )
	{

		MessageBox(NULL,
			_T("Error creating VideoDeviceCategory enumerator"),
			_T("uDirectShow"),
			MB_OK | MB_ICONEXCLAMATION);

		pCreateDevEnum->Release();

		return hr;
	}

	pEm->Reset();

	// go through and find first video capture device
	while( 1 )
	{

		fprintf(stderr,"Loop\n");

		ULONG ulFetched = 0L;
		IMoniker* pM = 0L;

		hr = pEm->Next( 1, &pM, &ulFetched );
		if( hr != S_OK )
			break;

		// get the property bag interface from the moniker
		//
		IPropertyBag* pBag;
		hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );

		if( hr != S_OK )
			continue;

		// ask for the readable name
		//
		VARIANT var;
		var.vt = VT_BSTR;
		hr = pBag->Read( L"FriendlyName", &var, NULL );

		if (hr != S_OK)
			continue;


#if !defined(_WIN32_WCE) && !defined(__MINGW32__)
		
		size_t asize = SysStringLen(var.bstrVal)+1;
		char* ansistr = (char*)malloc(asize);
		::WideCharToMultiByte(CP_ACP,0,var.bstrVal,-1,ansistr,asize,NULL,NULL);

		fprintf(stderr,"Name: %s\n",ansistr);

		free(ansistr);

		//std::clog << "Found: " << name.c_str() << std::endl;
#endif


		//std::string name =  convert_from_bstr(var.bstrVal);

		//std::clog << "Found: " << name.c_str() << std::endl;

		hr = pM->BindToObject( 0, 0, IID_IBaseFilter, (void**) ppCap );

		pM->Release();

		if( *ppCap )
		{
			// std::cout << "Finished" << std::endl;
			break;
		}
	}

	pCreateDevEnum->Release();
	pEm->Release();

    fflush(stderr);

	return hr;
}

#endif


UDSHOW_API
void WINAPI uFreeMediaType( AM_MEDIA_TYPE& mt )
{
	if( mt.cbFormat != 0 )
	{
		CoTaskMemFree( (PVOID)mt.pbFormat );
		mt.cbFormat = 0;
		mt.pbFormat = NULL;
	}
	if( mt.pUnk != NULL )
	{
		mt.pUnk->Release();
		mt.pUnk = NULL;
	}
}

UDSHOW_API
HRESULT WINAPI uCopyMediaType(AM_MEDIA_TYPE *pmtTarget, const AM_MEDIA_TYPE *pmtSource)
{
    if( !pmtSource || !pmtTarget ) return S_FALSE;

   *pmtTarget = *pmtSource;

    if( pmtSource->cbFormat && pmtSource->pbFormat )
    {
        pmtTarget->pbFormat = (PBYTE)CoTaskMemAlloc( pmtSource->cbFormat );
        if( pmtTarget->pbFormat == NULL )
        {
            pmtTarget->cbFormat = 0;
            return E_OUTOFMEMORY;
        }
        else
        {
            CopyMemory( (PVOID)pmtTarget->pbFormat, (PVOID)pmtSource->pbFormat,
                        pmtTarget->cbFormat );
        }
    }
    if( pmtTarget->pUnk != NULL )
    {
        pmtTarget->pUnk->AddRef();
    }

    return S_OK;
}
//
UDSHOW_API
HRESULT WINAPI uCopyMediaType1( AM_MEDIA_TYPE *pmtTarget,
							 const AM_MEDIA_TYPE *pmtSource )
{

	if( !pmtSource || !pmtTarget ) return S_FALSE;

	*pmtTarget = *pmtSource;

	if( pmtSource->cbFormat && pmtSource->pbFormat )
	{
		pmtTarget->pbFormat = (PBYTE)CoTaskMemAlloc( pmtSource->cbFormat );
		if( pmtTarget->pbFormat == NULL )
		{
			pmtTarget->cbFormat = 0;
			return E_OUTOFMEMORY;
		}
		else
		{
			CopyMemory( (PVOID)pmtTarget->pbFormat, (PVOID)pmtSource->pbFormat,
				pmtTarget->cbFormat );
		}
	}
	if( pmtTarget->pUnk != NULL )
	{
		pmtTarget->pUnk->AddRef();
	}

	return S_OK;
}



UDSHOW_API
HRESULT GetUnconnectedPin(
						  IBaseFilter *pFilter,   // Pointer to the filter.
						  PIN_DIRECTION PinDir,   // Direction of the pin to find.
						  IPin **ppPin)           // Receives a pointer to the pin.
{
	*ppPin = 0;
	IEnumPins *pEnum = 0;
	IPin *pPin = 0;
	HRESULT hr = pFilter->EnumPins(&pEnum);
	if (FAILED(hr))
	{
		return hr;
	}
	while (pEnum->Next(1, &pPin, NULL) == S_OK)
	{
		PIN_DIRECTION ThisPinDir;
		pPin->QueryDirection(&ThisPinDir);
		if (ThisPinDir == PinDir)
		{
			IPin *pTmp = 0;
			hr = pPin->ConnectedTo(&pTmp);
			if (SUCCEEDED(hr))  // Already connected, not the pin we want.
			{
				pTmp->Release();
			}
			else  // Unconnected, this is the pin we want.
			{
				pEnum->Release();
				*ppPin = pPin;
				return S_OK;
			}
		}
		pPin->Release();
	}
	pEnum->Release();
	// Did not find a matching pin.
	return E_FAIL;
}


UDSHOW_API
HRESULT ConnectFilters(
						 IGraphBuilder *pGraph, // Filter Graph Manager.
						 IPin *pOut,            // Output pin on the upstream filter.
						 IBaseFilter *pDest)    // Downstream filter.
{
	if ((pGraph == NULL) || (pOut == NULL) || (pDest == NULL))
	{
		return E_POINTER;
	}
#ifdef debug
	PIN_DIRECTION PinDir;
	pOut->QueryDirection(&PinDir);
	_ASSERTE(PinDir == PINDIR_OUTPUT);
#endif

	// Find an input pin on the downstream filter.
	IPin *pIn = 0;
	HRESULT hr = GetUnconnectedPin(pDest, PINDIR_INPUT, &pIn);
	if (FAILED(hr))
	{
		return hr;
	}
	// Try to connect them.
	hr = pGraph->Connect(pOut, pIn);
	pIn->Release();
	return hr;
}

UDSHOW_API
HRESULT ConnectFilters_2(
						 IGraphBuilder *pGraph,
						 IBaseFilter *pSrc,
						 IBaseFilter *pDest)
{
	if ((pGraph == NULL) || (pSrc == NULL) || (pDest == NULL))
	{
		return E_POINTER;
	}

	// Find an output pin on the first filter.
	IPin *pOut = 0;
	HRESULT hr = GetUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = ConnectFilters(pGraph, pOut, pDest);
	pOut->Release();
	return hr;
}



#endif
