#include "dshow_backend.h"
#include "_source.h"

#include "vx/config.h"

#if defined(HAVE_DSHOW)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#ifndef WINBOOL
#define WINBOOL BOOL
#endif

#include <Qedit.h>

#include "ds_helper.h"
#include "ds_capture.h"
#include "cpropertybag.h"

#include <stdlib.h>
#include <stdio.h>

#ifndef SAFE_RELEASE
    #define SAFE_RELEASE(x) if (x) { x->Release(); x = NULL; }
#endif


typedef struct vx_source_dshow {
	vx_source super;

	IBaseFilter* _SourceFilter;
	IBaseFilter* _ProxyFilter;
	IBaseFilter* _SinkFilter;

	//#if defined(_WIN32_WCE)
	//	IBasicVideo* _BasicVideo;
	//#endif

	IMediaEvent* _MediaEvent;
	IMediaControl* _MediaControl;

	ICaptureGraphBuilder2* _GraphBuilder;
	IGraphBuilder* _Graph;

} vx_source_dshow;



#define VX_DSHOW_CAST(ptr) \
	((vx_source_dshow*)(ptr))



static int InitFilterGraph(vx_source_dshow* cap)
{
	// preset
	HRESULT hr(S_OK);

	// create a filter graph
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
		IID_IGraphBuilder, (void**)&cap->_Graph);
	if (hr != S_OK)
	{
        MessageBox(NULL, _T("Creating FilterGraph failed!"),
                   _T("SSTT Capture"),
                   MB_OK | MB_ICONEXCLAMATION);
	} else
	{
		// attach the filter graph to the capture graph
		hr = cap->_GraphBuilder->SetFiltergraph(cap->_Graph);
		if (FAILED(hr))
		{
            MessageBox(NULL, _T("Can't set the filtergraph!"),
                       _T("SSTT Capture"),
                       MB_OK | MB_ICONEXCLAMATION);
		}
	}

	return 0;

}

int InitGraphBuilder(vx_source_dshow *cap)
{
	// preset
	HRESULT hr(S_OK);

#if defined(_WIN32_WCE)
	// create a capture graph
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder, NULL, CLSCTX_INPROC,
		IID_ICaptureGraphBuilder2,(void**)&cap->_GraphBuilder);
#else
	// create a capture graph
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC,
		IID_ICaptureGraphBuilder2,(void**)&cap->_GraphBuilder);
#endif

	if (hr != S_OK)
	{
        MessageBox(NULL, _T("Creating CaptureGraph Builder failed!"),
                   _T("SSTT Capture"),
                   MB_OK | MB_ICONEXCLAMATION);
    }

	return 0;
}


int InitCOM()
{
	HRESULT res = CoInitializeEx(NULL,COINIT_MULTITHREADED);
	if (res != S_OK) {
		fprintf(stderr,"Cannot initialize COM\n");
	}
	return 0;
}

int InitCaptureDevice(vx_source_dshow* cap,const char* uuid) {


#if defined(_WIN32_WCE)
	HRESULT hr = CoCreateInstance(CLSID_VideoCapture, NULL, CLSCTX_INPROC,
		IID_IBaseFilter, (void**)&cap->_SourceFilter);
	if (hr != S_OK)
	{
		MessageBox(NULL, _T("Error initializing VideoCapture interface"), _T("SSTT Capture"), MB_OK | MB_ICONEXCLAMATION);
	}
#else
	if (uuid) {
        /* use UUID */
		if (S_OK != GetCaptureDeviceWithUID(&cap->_SourceFilter,uuid)) {
            /* if that fails try the default */
			GetCaptureDevice(&cap->_SourceFilter);
		}
	} else {
        /* get default device */
		GetCaptureDevice(&cap->_SourceFilter);
	}

#endif
	return 0;
}

int InitRenderer(vx_source_dshow* cap, int renderType = 0,bool doEnumerate = false)
{
	HRESULT hr(S_OK);

    vx_source* source = VX_SOURCE_CAST(cap);

	const CLSID ___CLSID_NullRenderer =	{0xC1F400A4, 0x3F08, 0x11D3,{0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37}};

	switch(renderType)
	{
		case 0:
			hr = CoCreateInstance(___CLSID_NullRenderer, NULL, CLSCTX_INPROC,
				IID_IBaseFilter, (void**)&(cap->_SinkFilter));
			break;
		case 1:
			hr = CoCreateInstance(CLSID_VideoRenderer, NULL, CLSCTX_INPROC,
				IID_IBaseFilter, (void**)&(cap->_SinkFilter));
			if (SUCCEEDED(hr))
			{
				printf("Video Renderer - OK");
			}
			break;

		case 2:

            cap->_SinkFilter = new CaptureFilter(source,0,0);
			break;

		case 3:
			{
				//hr = cap->_GraphBuilder->QueryInterface(IID_IBasicVideo,(LPVOID*)&cap->_BasicVideo);
				//if(SUCCEEDED(hr))
				//{
				//	std::cout << "We have a basic Video!" << std::endl;
				//}
			}
			break;

	}

	if (hr != S_OK)
	{
        MessageBox(NULL, _T("Error Initializing Renderer"),
                   _T("SSTT Capture"),
                   MB_OK | MB_ICONEXCLAMATION);
	}

	hr = cap->_Graph->AddFilter(cap->_SinkFilter,
		L"Sink Filter");

	if (hr != S_OK)
	{
		// \TODO: check reason!
		// For some reasons this sometimes fails but the video still runs
        MessageBox(NULL, _T("Error Adding Sink Filter"),
                   _T("SSTT Capture"),
                   MB_OK | MB_ICONEXCLAMATION);
	}


	return 0;
}


int PresetCaptureDevice(vx_source_dshow* cap)
{

	HRESULT hr(E_FAIL);

#if defined(_WIN32_WCE)


	DEVMGR_DEVICE_INFORMATION devInfo;
	VARIANT  camNameVar;

	CPropertyBag PropBag;
	IPersistPropertyBag* pPropertyBag;

	hr = cap->_SourceFilter->QueryInterface( IID_IPersistPropertyBag,
		(void**)&pPropertyBag );

	if (hr != S_OK)
	{
		MessageBox(NULL, _T("Error getting the property bag"), _T("SSTT Capture"), MB_OK | MB_ICONEXCLAMATION);

		return -1;

	}

	GUID guidCamera = { 0xCB998A05, 0x122C, 0x4166, 0x84, 0x6A, 0x93, 0x3E, 0x4D, 0x7E, 0x3C, 0x86 };

	devInfo.dwSize = sizeof(devInfo);

	HANDLE h = FindFirstDevice( DeviceSearchByGuid, &guidCamera, & devInfo);
	//while (FindNextDevice(h,&devInfo))
	//{
	//	std::cout << "Camera" <<  devInfo.szDeviceName << std::endl;
	//}


	//MessageBox(NULL, &devInfo.szLegacyName[0], _T("SSTT Capture"), MB_OK | MB_ICONEXCLAMATION);


	BSTR OleCamName = SysAllocString(&devInfo.szLegacyName[0]);

	VariantInit(&camNameVar);
	camNameVar.vt = VT_BSTR;
	camNameVar.bstrVal = OleCamName;

	PropBag.Write( _T("VCapName"), &camNameVar );

	pPropertyBag->Load( &PropBag, NULL);

	hr = cap->_Graph->AddFilter( cap->_SourceFilter,
		_T("Video Capture Filter") );


	if (hr != S_OK)
	{
		MessageBox(NULL, _T("Error inserting the capture filter"), _T("SSTT Capture"), MB_OK | MB_ICONEXCLAMATION);
	}


	pPropertyBag->Release();

#else

	if (cap->_SourceFilter) {

		hr = cap->_Graph->AddFilter( cap->_SourceFilter, 0 );
		if (hr != S_OK)
		{
			MessageBox(NULL, _T("Error inserting the capture filter"), _T("SSTT Capture"), MB_OK | MB_ICONEXCLAMATION);
		}
	}

#endif

	return hr;
}


int RunCapture(vx_source_dshow* cap)
{
	HRESULT hr(S_OK);

	hr = cap->_GraphBuilder->RenderStream(
		(/*cap->_allow_drop_frames*/false) ? &PIN_CATEGORY_PREVIEW : &PIN_CATEGORY_CAPTURE,
		&MEDIATYPE_Video,
		cap->_SourceFilter,
		cap->_ProxyFilter,
		cap->_SinkFilter
		);

	if (FAILED(hr))
	{
        fprintf(stderr,"%s seems to have failed\n",__FUNCTION__);
/*		MessageBox(NULL, _T("Source can not be rendered"), _T("SSTT Capture"),
			MB_OK | MB_ICONEXCLAMATION);
		*/
	}



	//#define WM_GRAPHNOTIFY WM_APP+1

	//hr = cap->_FilterGraph->QueryInterface( IID_IMediaEventEx, (void**) &cap->_MediaEvent );

	////hr = cap->_MediaEvent->SetNotifyWindow((OAHWND)hWnd,
	////	WM_GRAPHNOTIFY,0);

	hr = cap->_Graph->QueryInterface(IID_IMediaControl,(void**)&cap->_MediaControl);
	if (hr != S_OK)
	{
		MessageBox(NULL, _T("Cannot get a media control"), _T("SSTT Capture"), MB_OK | MB_ICONEXCLAMATION);
	} else
	{

		//hr = cap->_MediaControl->Pause();

		hr = cap->_MediaControl->Run();

		if (FAILED(hr))
		{

			unsigned long state(0);

			cap->_MediaControl->GetState(100,(OAFilterState*)&state);

			switch(state)
			{
				case S_OK:
					hr = cap->_MediaControl->Run();
					break;
				case VFW_S_STATE_INTERMEDIATE:
					MessageBox(NULL, _T("Cannot start graph (STATE_INTERMEDIATE)"),
						_T("SSTT Capture"), MB_OK | MB_ICONEXCLAMATION);
					break;
				case VFW_S_CANT_CUE:
					MessageBox(NULL, _T("Cannot start graph (CANT_CUE)"),
						_T("SSTT Capture"), MB_OK | MB_ICONEXCLAMATION);
					break;
				case E_FAIL:
					MessageBox(NULL, _T("Cannot start graph (E_FAIL)"),
						_T("SSTT Capture"), MB_OK | MB_ICONEXCLAMATION);
					break;
			}

			// MessageBox(NULL, _T("Cannot start graph"), _T("SSTT Capture"), MB_OK | MB_ICONEXCLAMATION);
		}
	}


//    fprintf(stdout,"%s running? (%d)\n",__FUNCTION__,hr);

//    Sleep(1000);

	//SaveGraphFile(cap->_Graph, L"sstt_debug.grf");

	return 0;
}


void
InitColorSpaceFilter(vx_source_dshow* cap)
{
	IBaseFilter* csc(NULL);

	const CLSID __CLSID_Colour = {0x1643e180, 0x90f5, 0x11ce,{0x97, 0xd5, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a}};

	if (SUCCEEDED(CoCreateInstance( __CLSID_Colour, NULL,
		CLSCTX_INPROC, IID_IBaseFilter, (void **)&csc)))
	{
		if (SUCCEEDED(cap->_Graph->AddFilter(csc,0L)))
		{
			cap->_ProxyFilter = csc;

		} else {

			MessageBox(NULL, _T("Cannot add colourspace filter to graph"),
			_T("SSTT Capture"), MB_OK | MB_ICONEXCLAMATION);

		}



	} else
	{
		MessageBox(NULL, _T("Cannot create colourspace filter"),
			_T("SSTT Capture"), MB_OK | MB_ICONEXCLAMATION);
	}

}


int vx_source_dshow_open(vx_source* s, const char* uuid,vx_device_capability* c)
{
	vx_source_dshow* dsSource = VX_DSHOW_CAST(s);


	InitGraphBuilder(dsSource);
	InitFilterGraph(dsSource);

    InitCaptureDevice(dsSource,uuid);
	PresetCaptureDevice(dsSource);

#if USE_COLORSPACEFILTER

	InitColorSpaceFilter(dsSource);

	ConnectFilters_2(_Graph,_SourceFilter,_ProxyFilter);

	InitRenderer(dsSource,2);

	ConnectFilters_2(_Graph,_ProxyFilter,_SinkFilter);
#else
	InitRenderer( dsSource, 2 );

#endif

	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

int vx_source_dshow_close(vx_source* s)
{
	vx_source_dshow* dsSource = VX_DSHOW_CAST(s);

	SAFE_RELEASE(dsSource->_MediaControl);
	SAFE_RELEASE(dsSource->_MediaEvent);
	SAFE_RELEASE(dsSource->_ProxyFilter);
	SAFE_RELEASE(dsSource->_SinkFilter);
	SAFE_RELEASE(dsSource->_SourceFilter);
	SAFE_RELEASE(dsSource->_ProxyFilter);
	SAFE_RELEASE(dsSource->_GraphBuilder);
	SAFE_RELEASE(dsSource->_Graph);

	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

int vx_source_dshow_set_state(vx_source* s,int state)
{
	vx_source_dshow* dsSource = VX_DSHOW_CAST(s);

	switch (state) {
	case VX_SOURCE_STATE_RUNNING:
		RunCapture(dsSource);
		break;
	case VX_SOURCE_STATE_STOP:
		{

			OAFilterState fs = State_Running; 

			if (dsSource->_MediaControl) dsSource->_MediaControl->Pause();

			if (dsSource->_MediaControl) {
				while (fs == State_Running && (dsSource->_MediaControl->GetState(INFINITE,&fs) == S_OK))
				{
				}
				if (dsSource->_MediaControl) dsSource->_MediaControl->Stop();
			}
		}
		break;
	}

    printf("%s %d (%d)\n",__FUNCTION__,__LINE__,state);
	return 0;
}

int vx_source_dshow_get_state(vx_source* s,int* state)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}


struct vxEnumerateCallback : EnumerateCallback {

    vx_device_description* _desc;

    vxEnumerateCallback(vx_device_description* desc)
        : _desc(desc) {}

    virtual void operator ()(const AM_MEDIA_TYPE* pmt)
    {
        if( pmt->majortype == MEDIATYPE_Video )
        {

            vx_device_capability newCap;

            newCap.width = ((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biWidth;
            newCap.height = ((VIDEOINFOHEADER *)pmt->pbFormat)->bmiHeader.biHeight;

            if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_RGB565))
                newCap.pixelFormat = VX_E_COLOR_RGB565;
            else if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_RGB555))
                newCap.pixelFormat = VX_E_COLOR_RGB555;
            else if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_RGB32))
                newCap.pixelFormat = VX_E_COLOR_RGBA;
            else if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_RGB8))
                newCap.pixelFormat = VX_E_COLOR_RGBA;
            else if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_RGB24))
                newCap.pixelFormat = VX_E_COLOR_RGB24;
            else if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_ARGB32))
                newCap.pixelFormat = VX_E_COLOR_ARGB;
            else if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_RGB4))
                newCap.pixelFormat = VX_E_COLOR_USERDEFINED;
            else
                newCap.pixelFormat = pmt->subtype.Data1;

            newCap.speed.numerator = 1.f;
            newCap.speed.denominator = 10000000.0f/((float)((VIDEOINFOHEADER *)pmt->pbFormat)->AvgTimePerFrame);

            if (newCap.width && newCap.height)
                _vx_source_addcapability(&newCap,
                                         &_desc->capabilities,
                                         &_desc->capabilitiesCount);

        }
    }
};



int vx_source_dshow_enumerate(vx_source* s)
{

#if defined(_WIN32_WCE)

	GUID guidCamera = { 0xCB998A05, 0x122C, 0x4166, 0x84, 0x6A, 0x93, 0x3E, 0x4D, 0x7E, 0x3C, 0x86 };

	HRESULT hr = S_OK;
	HANDLE handle = NULL;
	DEVMGR_DEVICE_INFORMATION di;
	WCHAR pwzName[MAX_PATH];

	//if( pwzName == NULL ) {
	//	return E_POINTER;
	//}

	di.dwSize = sizeof(di);

	handle = FindFirstDevice( DeviceSearchByGuid, &guidCamera, &di );
	if(( handle == NULL ) || ( di.hDevice == NULL ))
	{
		MessageBox(0,_T("Error no device found"),_T("SSTT Capture"), MB_OK);
		goto Cleanup;
	}

	StringCchCopy( pwzName, MAX_PATH, di.szLegacyName );

	//MessageBox(0,pwzName,_T("Device"),MB_OK);

	//std::cout << "Camera " << pwzName << std::endl;

Cleanup:
	FindClose( handle );
	return hr;

#else

    HRESULT hr						= E_FAIL;
	IBaseFilter **ppCap				= NULL;
	ICreateDevEnum *pCreateDevEnum	= NULL;
	IEnumMoniker *pEm				= NULL;


	hr = CoCreateInstance(CLSID_SystemDeviceEnum, 0L, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void**)&pCreateDevEnum);

	if (!pCreateDevEnum || S_OK != hr)
	{
		MessageBox(NULL, _T("Error Creating a System Enumerator"), _T("libVX "), MB_OK | MB_ICONEXCLAMATION);
		return hr;
	}

	// enumerate video capture devices
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
		ULONG ulFetched					= 0L;
		IMoniker* pM					= 0L;
		IPropertyBag *pBag				= NULL;

		hr = pEm->Next( 1, &pM, &ulFetched );
		if( hr != S_OK )
			break;

		// get the property bag interface from the moniker
		//
		hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );

		if( hr != S_OK )
			continue;

		// ask for the readable name
		VARIANT var_name;
		var_name.vt = VT_BSTR;
		hr = pBag->Read( L"FriendlyName", &var_name, NULL );

        LPOLESTR disp_name;
		pM->GetDisplayName(NULL,NULL,&disp_name);

        int newSizeDesc = s->deviceCount + 1;

        vx_device_description* pNewDesc =
                (vx_device_description*)realloc(s->devices,
                                                newSizeDesc * (sizeof(vx_device_description)));

        if (pNewDesc) {

            WideCharacterConversion conv;


            memset(&pNewDesc[s->deviceCount],0,sizeof(struct vx_device_description));


            pNewDesc[s->deviceCount].name = _strdup(conv(var_name.bstrVal));
            conv.destroy();
            pNewDesc[s->deviceCount].uuid = _strdup(conv(disp_name));


            s->devices = pNewDesc;
            s->deviceCount = newSizeDesc;


            vx_source_dshow* dsSource = (vx_source_dshow*)vx_source_dshow_create();

            InitGraphBuilder(dsSource);
            InitFilterGraph(dsSource);

            InitCaptureDevice(dsSource,conv.mb_str());
            PresetCaptureDevice(dsSource);
            InitRenderer(dsSource,2,true);

            CaptureFilter* cf = (CaptureFilter*)dsSource->_SinkFilter;


            vxEnumerateCallback cb(&pNewDesc[s->deviceCount-1]);

            cf->SetEnumerateCallback(&cb);

            RunCapture(dsSource);

            vx_source_dshow_close((vx_source*)dsSource);

        }

		if (hr != S_OK)
			continue;

		if (*ppCap)
		{
            //*ppCap->Release();
		}
	}

	pCreateDevEnum->Release();
	pEm->Release();


#endif

	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}


int vx_source_dshow_update(vx_source* s,unsigned int runloop)
{
    /* might be necessary depending on the application */
    if (runloop == VX_SOURCE_UPDATE_PEEK) {
        MSG msg;
        while(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {}
    }
    return 0;
}

void*
vx_source_dshow_create()
{
	HRESULT res = CoInitializeEx(NULL,COINIT_MULTITHREADED);

	vx_source_dshow* s = (vx_source_dshow*)malloc(sizeof(vx_source_dshow));

	memset(s,0,sizeof(vx_source_dshow));

    s->super.deviceCount = 0;
    s->super.devices = 0L;

	VX_SOURCE_CAST(s)->open = vx_source_dshow_open;
	VX_SOURCE_CAST(s)->close = vx_source_dshow_close;
	VX_SOURCE_CAST(s)->set_state = vx_source_dshow_set_state;
	VX_SOURCE_CAST(s)->get_state = vx_source_dshow_get_state;
    VX_SOURCE_CAST(s)->enumerate = vx_source_dshow_enumerate;
    VX_SOURCE_CAST(s)->update = vx_source_dshow_update;

	return s;
}

#else

void*
vx_source_dshow_create()
{
	return 0;
}
#endif


