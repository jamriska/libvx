#include "ds_control.h"

DirectShowControl::DirectShowControl()
    : _SourceFilter(0)
	, _ProxyFilter(0)
	, _SinkFilter(0)
	, _MediaEvent(0)
	, _MediaControl(0)
	, _GraphBuilder(0)
	, _Graph(0)
    , _rendererType(kCustomSink)
    , _allowDropFrames(false)
    , _saveGraph(false)
    , _useColorSpaceFilter(true)
{
    _InitCOM();
    _InitGraphBuilder();
    _InitFilterGraph();
}

bool DirectShowControl::Open( const char *uuid )
{
    if (_InitColorSpaceFilter())
        if (_InitCaptureDevice(uuid))
            if (_InitRenderer())
                return true;

	return false;
}

bool DirectShowControl::Start()
{
	return _Run();
}

bool DirectShowControl::Stop()
{
	return _Stop();
}

DirectShowControl::~DirectShowControl()
{

    #ifndef SAFE_RELEASE
        #define SAFE_RELEASE(x) if (x) { x->Release(); x = NULL; }
    #endif


	Stop();

	SAFE_RELEASE(_MediaControl);
	SAFE_RELEASE(_MediaEvent);
	SAFE_RELEASE(_ProxyFilter);
	SAFE_RELEASE(_SinkFilter);
	SAFE_RELEASE(_SourceFilter);
	SAFE_RELEASE(_ProxyFilter);
	SAFE_RELEASE(_GraphBuilder);
	SAFE_RELEASE(_Graph);
}

void DirectShowControl::OnError( const char* error ) const
{
	fprintf(stderr,"%s\n",error);
    fflush(stderr);
}

void DirectShowControl::SetEnumerateCallback(QueryAcceptCallback *cb)
{
    if (_SinkFilter && (_rendererType == kCustomSink)) {
        static_cast<CaptureFilter*>(_SinkFilter)->SetQueryAcceptCallback(cb);
    }
}

void DirectShowControl::SetCaptureCallback(SampleCallback *cb)
{
    if (_SinkFilter && (_rendererType == kCustomSink)) {
        static_cast<CaptureFilter*>(_SinkFilter)->SetSampleCallback(cb);
    }
}

bool DirectShowControl::_InitFilterGraph()
{
	// create a filter graph
	if(S_OK == CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
		IID_IGraphBuilder, (void**)&_Graph))
		if (S_OK == _GraphBuilder->SetFiltergraph(_Graph))
			return true;

	OnError("can't initialize filtergraph");
	return false;
}

bool DirectShowControl::_InitGraphBuilder()
{
#if defined(_WIN32_WCE)
	// create a capture graph
	if(S_OK == CoCreateInstance(CLSID_CaptureGraphBuilder, NULL, CLSCTX_INPROC,
		IID_ICaptureGraphBuilder2,(void**)&_GraphBuilder))
#else
	// create a capture graph
	if(S_OK == CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC,
		IID_ICaptureGraphBuilder2,(void**)&_GraphBuilder))
#endif
		return true;

	OnError("can't initialize graph builder");
	return false;
}

bool DirectShowControl::_InitCOM()
{
    /* CoInitializeEx always succeeds */
    CoInitializeEx(NULL,COINIT_MULTITHREADED);
	return true;
}

bool DirectShowControl::_InitCaptureDevice( const char* uuid )
{
#if defined(_WIN32_WCE)
	if (S_OK == CoCreateInstance(CLSID_VideoCapture, NULL, CLSCTX_INPROC,
		IID_IBaseFilter, (void**)&_SourceFilter))
#else

    if (_FindDevice(uuid))
#endif
        if (S_OK == _Graph->AddFilter( _SourceFilter, 0 ))
            return true;

    OnError("can't initialize capture device or add to the capture graph");

	return false;
}

bool DirectShowControl::_InitRenderer()
{
	HRESULT hr (S_FALSE);

	const CLSID ___CLSID_NullRenderer =	{0xC1F400A4, 0x3F08, 0x11D3,{0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37}};

	switch(_rendererType)
	{
	case kNullSink:
		hr = CoCreateInstance(___CLSID_NullRenderer, NULL, CLSCTX_INPROC,
			IID_IBaseFilter, (void**)&_SinkFilter);
		break;
	case kVideoSink:
		hr = CoCreateInstance(CLSID_VideoRenderer, NULL, CLSCTX_INPROC,
			IID_IBaseFilter, (void**)&_SinkFilter);
		break;
	case kCustomSink:
        _SinkFilter = new CaptureFilter(0,0);
		hr = (_SinkFilter) ? S_OK : S_FALSE;
		break;
	default:
		OnError("unknown renderer type");
	}

	if (hr != S_OK) {
		OnError("error initializing renderer");
		return false;
	}

    if (S_OK == _Graph->AddFilter(_SinkFilter,L"Sink Filter"))
		return true;

	OnError("error adding renderer / sink filter");

	return false;
}

bool DirectShowControl::_InitColorSpaceFilter()
{

    if (!_useColorSpaceFilter)
        return true;

	const CLSID __CLSID_Colour = {0x1643e180, 0x90f5, 0x11ce,{0x97, 0xd5, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a}};

	if (SUCCEEDED(CoCreateInstance( __CLSID_Colour, NULL,
		CLSCTX_INPROC, IID_IBaseFilter, (void **)&_ProxyFilter)))
		if (SUCCEEDED(_Graph->AddFilter(_ProxyFilter,0L)))

			return true;

	OnError("adding a colorspace filter failed");
	return false;
}

bool DirectShowControl::_Run()
{
    HRESULT hr(S_FALSE);

    ConnectComponents();

	if(S_OK != _Graph->QueryInterface(IID_IMediaControl,(void**)&_MediaControl))
	{
		OnError("no media control");
		return false;
	}

	hr = _MediaControl->Run();
	if (FAILED(hr))
	{
		unsigned long state(0);
		_MediaControl->GetState(100,(OAFilterState*)&state);

		switch(state)
		{
		case S_OK:
			hr = _MediaControl->Run();
			break;
		case VFW_S_STATE_INTERMEDIATE:
			OnError("cannot start graph (STATE_INTERMEDIATE)");
			break;
		case VFW_S_CANT_CUE:
			OnError("cannot start graph (CANT_CUE)");
			break;
		case E_FAIL:
			OnError("cannot start graph (E_FAIL)");
			break;
		}
	}

    if (_saveGraph)
		SaveGraphFile(_Graph, L"sstt_debug.grf");

    if (hr != S_OK)
        OnError("error running the filter graph");

    return (hr == S_OK);
}

bool DirectShowControl::Pause()
{
	if (_MediaControl)
	{
		if(S_OK == _MediaControl->Pause())
			return true;
	}
	return false;
}

bool DirectShowControl::_Stop()
{
	OAFilterState fs = State_Running;

    this->Pause();

	while (_MediaControl &&
		(fs == State_Running) &&
		(_MediaControl->GetState(INFINITE,&fs) == S_OK))
	{
	}

	_MediaControl->Stop();

	return true;
}

bool DirectShowControl::_FindDevice(const char* UID)
{
    HRESULT hr(E_FAIL);

    ICreateDevEnum* pCreateDevEnum(NULL);

    hr = CoCreateInstance(CLSID_SystemDeviceEnum, 0L, CLSCTX_INPROC_SERVER,
        IID_ICreateDevEnum, (void**)&pCreateDevEnum);

    if (!pCreateDevEnum || S_OK != hr)
    {
        OnError("error creating system device enumerator");
        return false;
    }

    // enumerate video capture devices
    IEnumMoniker *pEm(0L);
    hr = pCreateDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory,
                                                &pEm, 0 );
    if( hr != S_OK )
    {
        OnError("error creating video device enumerator");
        pCreateDevEnum->Release();
        return false;
    }

    pEm->Reset();

    // go through and find a suitable video capture device
    while( 1 )
    {
        ULONG ulFetched = 0L;
        IMoniker* pM = 0L;

        if (S_OK != pEm->Next( 1, &pM, &ulFetched ))
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
        if ((UID == 0)||
            (conv.mb_str() && (0 == strcmp(conv.mb_str(),UID))))
        {
            hr = pM->BindToObject( 0, 0, IID_IBaseFilter, (void**) &_SourceFilter );
        }

        pM->Release();

        // now we released the moniker
        if (_SourceFilter) break;
    }

    pCreateDevEnum->Release();
    pEm->Release();

    return (_SourceFilter != 0);

}

bool DirectShowControl::ConnectComponents()
{

    if (S_OK != _GraphBuilder->RenderStream(
        _allowDropFrames ? &PIN_CATEGORY_PREVIEW : &PIN_CATEGORY_CAPTURE,
        &MEDIATYPE_Video,
        _SourceFilter,
        _ProxyFilter,
        _SinkFilter
        ))
    {
        // magically this doesn't mean anything
        // it only connects and negotiates the
        // connections between the elements
    }

    if(S_OK != _Graph->QueryInterface(IID_IMediaControl,(void**)&_MediaControl))
    {
        OnError("no media control");
        return false;
    }

    _MediaControl->Pause();

    return true;
}
