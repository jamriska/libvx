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


#ifndef CAPTURE_H_  
#define CAPTURE_H_ 1


#include "ds_helper.h"

#include "_source.h"
#include "vx/frame.h"


typedef struct vx_source_dshow vx_source_dshow;


class CapturePin : public IPin, public IMemInputPin
{

	friend class CaptureEnumMediaTypes;

	CRITICAL_SECTION _cs;


	AM_MEDIA_TYPE *_mediatypes;
	size_t _mediatype_count;

	long _refcount;

	IPin* _connected_pin;
	IBaseFilter* _filter;

	AM_MEDIA_TYPE _connected_mediatype;

	vx_source_dshow* _handle;

	IMemAllocator* _allocator;
	ALLOCATOR_PROPERTIES _allocator_properties;

	BOOL _readonly;

	vx_frame _temp;

public:

	CapturePin(vx_source_dshow* handle,IBaseFilter* capturefilter,AM_MEDIA_TYPE *mediatypes, size_t mediatypes_count);

	virtual ~CapturePin();

	/* IUnknown */
	STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();


	/* IPin */
	STDMETHODIMP Connect( IPin * pReceivePin, const AM_MEDIA_TYPE *pmt );
	STDMETHODIMP ReceiveConnection( IPin * pConnector,
		const AM_MEDIA_TYPE *pmt );
	STDMETHODIMP Disconnect();
	STDMETHODIMP ConnectedTo( IPin **pPin );
	STDMETHODIMP ConnectionMediaType( AM_MEDIA_TYPE *pmt );
	STDMETHODIMP QueryPinInfo( PIN_INFO * pInfo );
	STDMETHODIMP QueryDirection( PIN_DIRECTION * pPinDir );
	STDMETHODIMP QueryId( LPWSTR * Id );
	STDMETHODIMP QueryAccept( const AM_MEDIA_TYPE *pmt );
	STDMETHODIMP EnumMediaTypes( IEnumMediaTypes **ppEnum );
	STDMETHODIMP QueryInternalConnections( IPin* *apPin, ULONG *nPin );
	STDMETHODIMP EndOfStream( void );

	STDMETHODIMP BeginFlush( void );
	STDMETHODIMP EndFlush( void );
	STDMETHODIMP NewSegment( REFERENCE_TIME tStart, REFERENCE_TIME tStop,
		double dRate );

	/* IMemInputPin methods */
	STDMETHODIMP GetAllocator( IMemAllocator **ppAllocator );
	STDMETHODIMP NotifyAllocator(  IMemAllocator *pAllocator, BOOL bReadOnly );
	STDMETHODIMP GetAllocatorRequirements( ALLOCATOR_PROPERTIES *pProps );
	STDMETHODIMP Receive( IMediaSample *pSample );
	STDMETHODIMP ReceiveMultiple( IMediaSample **pSamples, long nSamples,
		long *nSamplesProcessed );
	STDMETHODIMP ReceiveCanBlock( void );


};

class CaptureFilter : public IBaseFilter
{
	friend class CapturePin;

	CapturePin     *_capturepin;
	IFilterGraph   *_filtergraph;
	FILTER_STATE   _state;

	IReferenceClock *_clock;

	long i_ref;

	vx_source_dshow* _handle;

public:

	CaptureFilter( vx_source_dshow* handle, AM_MEDIA_TYPE *mt, size_t mt_count );

	/* IUnknown methods */
	STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	/* IPersist method */
	STDMETHODIMP GetClassID(CLSID *pClsID);

	/* IMediaFilter methods */
	STDMETHODIMP Stop();
	STDMETHODIMP Pause();
	STDMETHODIMP Run(REFERENCE_TIME tStart);
	STDMETHODIMP GetState(DWORD dwMSecs, FILTER_STATE *State);
	STDMETHODIMP SetSyncSource(IReferenceClock *pClock);
	STDMETHODIMP GetSyncSource(IReferenceClock **pClock);

	/* IBaseFilter methods */
	STDMETHODIMP EnumPins( IEnumPins ** ppEnum );
	STDMETHODIMP FindPin( LPCWSTR Id, IPin ** ppPin );
	STDMETHODIMP QueryFilterInfo( FILTER_INFO * pInfo );
	STDMETHODIMP JoinFilterGraph( IFilterGraph * pGraph, LPCWSTR pName );
	STDMETHODIMP QueryVendorInfo( LPWSTR* pVendorInfo );

	/* IAMovieSetup */
	STDMETHODIMP Register();
	STDMETHODIMP Unregister();

	/* Custom methods */
	CapturePin *CustomGetPin();
	virtual ~CaptureFilter();

};




class CaptureEnumPins : public IEnumPins
{
	CaptureFilter  *p_filter;

	int i_position;
	long i_ref;

	vx_source_dshow* _handle;

public:
	CaptureEnumPins( vx_source_dshow* handle, CaptureFilter *_p_filter,
		CaptureEnumPins *pEnumPins );
	virtual ~CaptureEnumPins();

	// IUnknown
	STDMETHODIMP QueryInterface( REFIID riid, void **ppv );
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	// IEnumPins
	STDMETHODIMP Next( ULONG cPins, IPin ** ppPins, ULONG * pcFetched );
	STDMETHODIMP Skip( ULONG cPins );
	STDMETHODIMP Reset();
	STDMETHODIMP Clone( IEnumPins **ppEnum );
};


class CaptureEnumMediaTypes : public IEnumMediaTypes
{

	CapturePin     *_pin;
	AM_MEDIA_TYPE _connected_mediatype;

	size_t _position;
	long _refcount;

	vx_source_dshow* _handle;

public:
	CaptureEnumMediaTypes( vx_source_dshow* _handle, CapturePin *_p_pin,
		CaptureEnumMediaTypes *pEnumMediaTypes );

	virtual ~CaptureEnumMediaTypes();

	// IUnknown
	STDMETHODIMP QueryInterface( REFIID riid, void **ppv );
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	// IEnumMediaTypes
	STDMETHODIMP Next( ULONG cMediaTypes, AM_MEDIA_TYPE ** ppMediaTypes,
		ULONG * pcFetched );
	STDMETHODIMP Skip( ULONG cMediaTypes );
	STDMETHODIMP Reset();
	STDMETHODIMP Clone( IEnumMediaTypes **ppEnum );
};


#endif