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
#include "ds_control.h"
#include "cpropertybag.h"

#include <stdlib.h>
#include <stdio.h>


typedef struct vx_source_dshow {

    vx_source super;

    DirectShowControl* dsCtrl;

} vx_source_dshow;



#define VX_DSHOW_CAST(ptr) \
    ((vx_source_dshow*)(ptr))


int vx_source_dshow_open(vx_source* s, const char* uuid,vx_device_capability* c)
{
    vx_source_dshow* dsSource = VX_DSHOW_CAST(s);
    dsSource->dsCtrl = new DirectShowControl();
    //"@device:pnp:\\\\?\\usb#vid_05ac&pid_8507&mi_00#7&38e271ba&0&0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\\global"

    return (dsSource->dsCtrl->Open(uuid)) ? 0 : -1;
}

int vx_source_dshow_close(vx_source* s)
{
    vx_source_dshow* dsSource = VX_DSHOW_CAST(s);
    if (dsSource->dsCtrl) delete dsSource->dsCtrl;
    return 0;
}

int vx_source_dshow_set_state(vx_source* s,int state)
{
    vx_source_dshow* dsSource = VX_DSHOW_CAST(s);
    switch (state) {
    case VX_SOURCE_STATE_RUNNING:
        dsSource->dsCtrl->Start();
        break;
    case VX_SOURCE_STATE_STOP:
        dsSource->dsCtrl->Stop();
        break;
    case VX_SOURCE_STATE_PAUSED:
        dsSource->dsCtrl->Pause();
        break;
    }

    return 0;
}

int vx_source_dshow_get_state(vx_source* s,int* state)
{
    printf("%s %d\n",__FUNCTION__,__LINE__);
    return 0;
}


struct vxEnumerateCallback : QueryAcceptCallback {

    vx_device_description* _desc;

    vxEnumerateCallback(vx_device_description* desc)
        : _desc(desc) {}

    virtual HRESULT operator ()(const AM_MEDIA_TYPE* pmt)
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
            else if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_YUYV))
                newCap.pixelFormat = VX_E_COLOR_YUYV;
            else if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_YUY2))
                newCap.pixelFormat = VX_E_COLOR_YUY2;
            else if (IsEqualGUID(pmt->subtype,MEDIASUBTYPE_YV12))
                newCap.pixelFormat = VX_E_COLOR_YV12;
            else
                newCap.pixelFormat = 0;

            newCap.speed.numerator = 1.f;
            newCap.speed.denominator = 10000000.f/((float)((VIDEOINFOHEADER *)pmt->pbFormat)->AvgTimePerFrame);

            if (newCap.width && newCap.height && newCap.pixelFormat)
                _vx_source_addcapability(&newCap,
                                         &_desc->capabilities,
                                         &_desc->capabilitiesCount);

        }

        return S_FALSE;
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

            vxEnumerateCallback cb(&pNewDesc[s->deviceCount-1]);

            DirectShowControl* ds = new DirectShowControl();

            ds->Open(conv.mb_str());
            ds->SetEnumerateCallback(&cb);

            ds->ConnectComponents();

			delete ds;

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


