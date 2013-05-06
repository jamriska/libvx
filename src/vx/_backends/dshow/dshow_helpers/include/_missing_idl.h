/**
 * udirectshow - micro layer for DirectShow programming
 *
 * (c) Copyright 2007 - 2008 Hartmut Seichter
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of Human Interface Laboratory New Zealand nor the
 *   names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef __DS_IDL_H_
#define __DS_IDL_H_ 1

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <objbase.h>

#if defined(WINCE) || defined(__MINGW32__)
#include "Ocidl.h"
#endif


#ifndef OAFilterState
typedef long OAFilterState;
#endif

#ifndef OAEVENT
typedef LONG_PTR OAEVENT;
#endif

#ifndef HRESULT
typedef long HRESULT;
#endif

#ifndef STDCALL
#define STDCALL __attribute__((__stdcall__))
#endif

#ifndef PURE
#define PURE =0
#endif

#ifndef REFERENCE_TIME
typedef LONGLONG REFERENCE_TIME;
#endif

#ifndef REFIID
typedef GUID& REFIID;
#endif

// typedef GUID CLSID;
// typedef GUID IID;


#ifndef __AMVIDEO__
// Returns the address of the BITMAPINFOHEADER from the VIDEOINFOHEADER
#define HEADER(pVideoInfo) (&(((VIDEOINFOHEADER *) (pVideoInfo))->bmiHeader))
#endif

#ifndef __QuartzTypeLib_LIBRARY_DEFINED__
typedef double REFTIME;
typedef LONG_PTR OAEVENT;
typedef LONG_PTR OAHWND;
typedef long OAFilterState;
#endif


const CLSID CLSID_FilterGraph =					{0xE436EBB3, 0x524F, 0x11CE,{0x9F, 0x53, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}};
const CLSID CLSID_SampleGrabber =				{0xC1F400A0, 0x3F08, 0x11D3,{0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37}};
const CLSID CLSID_DivX =						{0x78766964, 0x0000, 0x0010,{0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71}};
const CLSID CLSID_CaptureGraphBuilder =			{0xBF87B6E0, 0x8C27, 0x11D0,{0xB3, 0xF0, 0x00, 0xAA, 0x00, 0x37, 0x61, 0xC5}};
const CLSID CLSID_CaptureGraphBuilder2 =		{0xBF87B6E1, 0x8C27, 0x11D0,{0xB3, 0xF0, 0x00, 0xAA, 0x00, 0x37, 0x61, 0xC5}};
const CLSID CLSID_AviSplitter =					{0x1B544C20, 0xFD0B, 0x11CE,{0x8C, 0x63, 0x00, 0xAA, 0x00, 0x44, 0xB5, 0x1E}};
const CLSID CLSID_SystemDeviceEnum =			{0x62BE5D10, 0x60EB, 0x11D0,{0xBD, 0x3B, 0x00, 0xA0, 0xC9, 0x11, 0xCE, 0x86}};
const CLSID CLSID_VideoInputDeviceCategory =	{0x860BB310, 0x5D01, 0x11D0,{0xBD, 0x3B, 0x00, 0xA0, 0xC9, 0x11, 0xCE, 0x86}};
const CLSID CLSID_AVIDec =						{0xCF49D4E0, 0x1115, 0x11CE,{0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}};
const CLSID CLSID_LegacyAmFilterCategory =		{0x083863F1, 0x70DE, 0x11d0,{0xBD, 0x40, 0x00, 0xA0, 0xC9, 0x11, 0xCE, 0x86}};
const CLSID CLSID_CQzFilterClassManager =		{0x083863F1, 0x70DE, 0x11d0,{0xBD, 0x40, 0x00, 0xA0, 0xC9, 0x11, 0xCE, 0x86}};

const CLSID CLSID_NullRenderer =				{0xC1F400A4, 0x3F08, 0x11D3,{0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37}};
const CLSID CLSID_VideoRenderer =               {0x70e102b0, 0x5556, 0x11ce,{0x97, 0xc0, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a}};
const CLSID CLSID_VideoRendererDefault =        {0x6bc1cffa, 0x8fc1, 0x4261,{0xac, 0x22, 0xcf, 0xb4, 0xcc, 0x38, 0xdb, 0x50}};
const CLSID CLSID_VideoMixingRenderer9 =		{0x51B4ABF3, 0x748F, 0x4E3B,{0xA2, 0x76, 0xC8, 0x28, 0x33, 0x0E, 0x92, 0x6A}};


const CLSID CLSID_VideoCapture =				{0xf80b6e95, 0xb55a, 0x4619,{0xae, 0xc4, 0xa1, 0xe , 0xae, 0xde, 0x98, 0xc }};

const CLSID CLSID_Colour =						{0x1643e180, 0x90f5, 0x11ce,{0x97, 0xd5, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a}};
const CLSID CLSID_MemoryAllocator = 			{0x1e651cc0, 0xb199, 0x11d0,{0x82, 0x12, 0x00, 0xc0, 0x4f, 0xc3, 0x2c, 0x45}};


//const IID IID_IFilterGraph2;// = {0x36b73882, 0xc2c8, 0x11cf,{0x8b, 0x46, 0x00, 0x80, 0x5f, 0x6c, 0xef, 0x60}};
//extern "C" const IID IID_IFilterGraph;//= {0x56A8689F, 0x0AD4, 0x11CE,{0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}};


const IID IID_IAMStreamConfig =					 {0xC6E13340, 0x30AC, 0x11d0,{0xA1, 0x8C, 0x00, 0xA0, 0xC9, 0x11, 0x89, 0x56}};


const CLSID CLSID_VideoMixingRenderer = {0xB87BEB7B, 0x8D29, 0x423F,{0xAE, 0x4D, 0x65, 0x82, 0xC1, 0x01, 0x75, 0xAC}};
//const CLSID CLSID_VideoMixingRenderer9 = {0x51B4ABF3, 0x748F, 0x4E3B,{0xA2, 0x76, 0xC8, 0x28, 0x33, 0x0E, 0x92, 0x6A}};
//extern "C" const IID IID_IVMRWindowlessControl9;//= {0x8F537D09, 0xF85E, 0x4414,{0xB2, 0x3B, 0x50, 0x2E, 0x54, 0xC7, 0x99, 0x27}};
//extern "C" const IID IID_IGraphBuilder;// = {0x56A868A9, 0x0AD4, 0x11CE,{0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}};
//extern "C" const IID IID_IBaseFilter; //= {0x56A86895, 0x0AD4, 0x11CE,{0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}};
//extern "C" const IID IID_ISampleGrabber; // = {0x6B652FFF, 0x11FE, 0x4FCE,{0x92, 0xAD, 0x02, 0x66, 0xB5, 0xD7, 0xC7, 0x8F}};

//const IID IID_IVMRWindowlessControl9 =  {0x8F537D09, 0xF85E, 0x4414,{0xB2, 0x3B, 0x50, 0x2E, 0x54, 0xC7, 0x99, 0x27}};
const IID IID_IFilterGraph =            {0x56A8689F, 0x0AD4, 0x11CE,{0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}};
//const IID IID_IVMRFilterConfig9 =       {0x5A804648, 0x4F66, 0x4867,{0x9C, 0x43, 0x4F, 0x5C, 0x82, 0x2C, 0xF1, 0xB8}};
const IID IID_IGraphBuilder = 			{0x56A868A9, 0x0AD4, 0x11CE,{0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}};
const IID IID_IBaseFilter =             {0x56A86895, 0x0AD4, 0x11CE,{0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}};
const IID IID_ISampleGrabber =          {0x6B652FFF, 0x11FE, 0x4FCE,{0x92, 0xAD, 0x02, 0x66, 0xB5, 0xD7, 0xC7, 0x8F}};
const IID IID_ICaptureGraphBuilder2=	{0x93E5A4E0, 0x2D50, 0x11D2,{0xAB, 0xFA, 0x00, 0xA0, 0xC9, 0xC6, 0xE3, 0x8D}};
const IID IID_ICreateDevEnum =			{0x29840822, 0x5B84, 0x11D0,{0xBD, 0x3B, 0x00, 0xA0, 0xC9, 0x11, 0xCE, 0x86}};
const IID IID_ISampleGrabberCB =		{0x0579154A, 0x2B53, 0x4994,{0xB0, 0xD0, 0xE7, 0x73, 0x14, 0x8E, 0xFF, 0x85}};
const IID IID_IGraphConfig =			{0x03A1EB8E, 0x32BF, 0x4245,{0x85, 0x02, 0x11, 0x4D, 0x08, 0xA9, 0xCB, 0x88}};
const IID IID_IVMRWindowlessControl =   {0x0EB1088C, 0x4DCD, 0x46F0,{0x87, 0x8F, 0x39, 0xDA, 0xE8, 0x6A, 0x51, 0xB7}};
const IID IID_IEnumMediaTypes =			{0x89c31040, 0x846b, 0x11ce,{0x97, 0xd3, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a}};


const IID IID_IMemAllocator = 			{0x56a8689c, 0x0ad4, 0x11ce,{0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70}};

//extern "C" const IID IID_ISampleGrabberCB;

#if !defined(_MSC_VER) || !defined(__MINGW32__)
//const IID IID_IPropertyBag = 			{0x55272a00, 0x42cb, 0x11ce,{0x81, 0x35, 0x00, 0xaa, 0x00, 0x4b, 0xb8, 0x51}};
//const IID IID_IPersistStream = 			{0x00000109, 0x0000, 0x0000,{0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
//const IID IID_IUnknown = 						{0x00000000, 0x0000, 0x0000,{0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
#endif

#if defined(_WIN32_WCE)
//const IID IID_IPropertyBag = 			{0x55272a00, 0x42cb, 0x11ce,{0x81, 0x35, 0x00, 0xaa, 0x00, 0x4b, 0xb8, 0x51}};
#endif

const IID IID_IMediaControl=			{0x56A868B1, 0x0AD4, 0x11CE,{0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}};
const IID IID_IMediaEvent=				{0x56A868B6, 0x0AD4, 0x11CE,{0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70}};

const GUID MEDIATYPE_Video =			{0x73646976, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
const GUID MEDIATYPE_Interleaved =		{0x73766169, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
const GUID MEDIATYPE_Stream =			{0xe436eb83, 0x524f, 0x11ce, {0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70}};
const GUID MEDIASUBTYPE_PREVIEW_VIDEO = {0x2859e1da, 0xb81f, 0x4fbd, {0x94, 0x3b, 0xe2, 0x37, 0x24, 0xa1, 0xab, 0xb3}};



const GUID MEDIASUBTYPE_ARGB32		= {0x773C9AC0,0x3274,0x11D0,0xB7,0x24,0x0,0xAA,0x0,0x6C,0x1A,0x1};
const GUID MEDIASUBTYPE_Overlay		= {0xE436EB7F,0x524F,0x11CE,0x9F,0x53,0x0,0x20,0xAF,0xB,0xA7,0x70};
const GUID MEDIASUBTYPE_RGB1		= {0xE436EB78,0x524F,0x11CE,0x9F,0x53,0x0,0x20,0xAF,0xB,0xA7,0x70};
const GUID MEDIASUBTYPE_RGB24		= {0xE436EB7D,0x524F,0x11CE,0x9F,0x53,0x0,0x20,0xAF,0xB,0xA7,0x70};
const GUID MEDIASUBTYPE_RGB32		= {0xE436EB7E,0x524F,0x11CE,0x9F,0x53,0x0,0x20,0xAF,0xB,0xA7,0x70};
const GUID MEDIASUBTYPE_RGB4		= {0xE436EB79,0x524F,0x11CE,0x9F,0x53,0x0,0x20,0xAF,0xB,0xA7,0x70};
const GUID MEDIASUBTYPE_RGB555		= {0xE436EB7C,0x524F,0x11CE,0x9F,0x53,0x0,0x20,0xAF,0xB,0xA7,0x70};
const GUID MEDIASUBTYPE_RGB565		= {0xE436EB7B,0x524F,0x11CE,0x9F,0x53,0x0,0x20,0xAF,0xB,0xA7,0x70};
const GUID MEDIASUBTYPE_RGB8		= {0xE436EB7A,0x524F,0x11CE,0x9F,0x53,0x0,0x20,0xAF,0xB,0xA7,0x70};
/* Packed YUV formats */
const GUID MEDIASUBTYPE_YUYV = {0x56595559, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
const GUID MEDIASUBTYPE_Y411 = {0x31313459, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
const GUID MEDIASUBTYPE_Y211 = {0x31313259, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
const GUID MEDIASUBTYPE_YUY2 = {0x32595559, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
const GUID MEDIASUBTYPE_YVYU = {0x55595659, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
const GUID MEDIASUBTYPE_UYVY = {0x59565955, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};

/* Planar YUV formats */
const GUID MEDIASUBTYPE_YVU9 = {0x39555659, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};

const GUID MEDIASUBTYPE_YV12 = {0x32315659, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
const GUID MEDIASUBTYPE_IYUV = {0x56555949, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}; /* identical to YV12 */

const GUID MEDIASUBTYPE_Y41P = {0x50313459, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
const GUID MEDIASUBTYPE_I420 = {0x30323449, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
const GUID MEDIASUBTYPE_AYUV = {0x56555941, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};

const GUID PIN_CATEGORY_CAPTURE			= {0xfb6c4281, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba};
const GUID PIN_CATEGORY_PREVIEW			= {0xfb6c4282, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba};
const GUID PIN_CATEGORY_ANALOGVIDEOIN	= {0xfb6c4283, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba};
const GUID PIN_CATEGORY_VBI				= {0xfb6c4284, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba};
const GUID PIN_CATEGORY_VIDEOPORT		= {0xfb6c4285, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba};
const GUID PIN_CATEGORY_NABTS			= {0xfb6c4286, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba};
const GUID PIN_CATEGORY_EDS				= {0xfb6c4287, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba};
const GUID PIN_CATEGORY_TELETEXT		= {0xfb6c4288, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba};
const GUID PIN_CATEGORY_CC				= {0xfb6c4289, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba};
const GUID PIN_CATEGORY_STILL			= {0xfb6c428a, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba};
const GUID PIN_CATEGORY_TIMECODE		= {0xfb6c428b, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba};
const GUID PIN_CATEGORY_VIDEOPORT_VBI	= {0xfb6c428c, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba};

const GUID FORMAT_VideoInfo 			= {0x05589f80, 0xc356, 0x11ce, {0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a}};
const GUID FORMAT_VideoInfo2			= {0xf72a76a0, 0xeb0a, 0x11d0, {0xac, 0xe4, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba}};
const GUID FORMAT_MPEG2Video			= {0xe06d80e3, 0xdb46, 0x11cf, {0xb4, 0xd1, 0x00, 0x80, 0x5f, 0x6c, 0xbb, 0xea}};
const GUID FORMAT_MPEGVideo				= {0x05589f82, 0xc356, 0x11ce, {0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a}};

const GUID FORMAT_WaveFormatEx 			= {0x05589f81, 0xc356, 0x11ce, {0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a}};


const GUID IID_IAMCrossbar				= {0xC6E13380, 0x30AC, 0x11d0, {0xA1, 0x8C, 0x00, 0xA0, 0xC9, 0x11, 0x89, 0x56}};


const GUID IID_IPin						= {0x56a86891, 0x0ad4, 0x11ce, {0xb0,0x3a, 0x00,0x20,0xaf,0x0b,0xa7,0x70}};
const GUID IID_IMemInputPin				= {0x56a8689d, 0x0ad4, 0x11ce, {0xb0,0x3a, 0x00,0x20,0xaf,0x0b,0xa7,0x70}};
const GUID IID_IMediaFilter				= {0x56a86899, 0x0ad4, 0x11ce, {0xb0,0x3a, 0x00,0x20,0xaf,0x0b,0xa7,0x70}};
const GUID IID_IEnumPins				= {0x56a86892, 0x0ad4, 0x11ce, {0xb0,0x3a, 0x00,0x20,0xaf,0x0b,0xa7,0x70}};







/* Analog Video */
const GUID FORMAT_AnalogVideo = {0x482dde0, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};

const GUID MEDIATYPE_AnalogVideo = {0x482dde1, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xab, 0x0, 0x6e, 0xcb, 0x65}};

const GUID MEDIASUBTYPE_AnalogVideo_NTSC_M = {0x482dde2, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_PAL_B = {0x482dde5, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_PAL_D = {0x482dde6, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_PAL_G = {0x482dde7, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_PAL_H = {0x482dde8, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_PAL_I = {0x482dde9, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_PAL_M = {0x482ddea, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_PAL_N = {0x482ddeb, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_PAL_N_COMBO = {0x482ddec, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_SECAM_B = {0x482ddf0, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_SECAM_D = {0x482ddf1, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_SECAM_G = {0x482ddf2, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_SECAM_H = {0x482ddf3, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_SECAM_K = {0x482ddf4, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_SECAM_K1 = {0x482ddf5, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};
const GUID MEDIASUBTYPE_AnalogVideo_SECAM_L = {0x482ddf6, 0x7817, 0x11cf, {0x8a, 0x3, 0x0, 0xaa, 0x0, 0x6e, 0xcb, 0x65}};


const GUID LOOK_UPSTREAM_ONLY =				{ 0xac798be0, 0x98e3, 0x11d1, {0xb3, 0xf1, 0x0, 0xaa, 0x0, 0x37, 0x61, 0xc5}};
const GUID LOOK_DOWNSTREAM_ONLY =			{ 0xac798be1, 0x98e3, 0x11d1, {0xb3, 0xf1, 0x0, 0xaa, 0x0, 0x37, 0x61, 0xc5}};





#if(0)
struct __IUnknown
{
    virtual HRESULT QueryInterface(REFIID riid, void** ppvObj) = 0;
    virtual ULONG AddRef() = 0;
    virtual ULONG Release() = 0;
};
#endif

#define VFW_E_ALREADY_CONNECTED          ((HRESULT)0x80040204L)
#define VFW_E_INVALID_DIRECTION          ((HRESULT)0x80040208L)
#define VFW_E_NOT_CONNECTED              ((HRESULT)0x80040209L)
#define VFW_E_NO_ALLOCATOR               ((HRESULT)0x8004020AL)
#define VFW_E_TYPE_NOT_ACCEPTED          ((HRESULT)0x8004022AL)
#define VFW_E_NOT_FOUND                  ((HRESULT)0x80040216L)



#if 0
class IFileSinkFilter;


class IAMCopyCaptureFileProgress;

class IEnumFilters;
class IReferenceClock;
class IEnumMediaTypes;
#ifndef __IErrorLog_INTERFACE_DEFINED__
class IErrorLog;
#endif

class IBaseFilter;
class IFilterGraph;
class IPin;
class ICaptureGraphBuilder2;


typedef enum _FilterState
{
    State_Stopped,
    State_Paused,
    State_Running
} FILTER_STATE;

typedef enum _PinDirection {
    PINDIR_INPUT,
    PINDIR_OUTPUT
} PIN_DIRECTION;

typedef struct _FilterInfo {
    WCHAR        achName[128];
    IFilterGraph *pGraph;
} FILTER_INFO;

typedef struct _PinInfo {
   IBaseFilter *pFilter;
    PIN_DIRECTION dir;
    WCHAR achName[128];
} PIN_INFO;

typedef enum _AM_GRAPH_CONFIG_RECONNECT_FLAGS {
    AM_GRAPH_CONFIG_RECONNECT_DIRECTCONNECT = 0x00000001,
    AM_GRAPH_CONFIG_RECONNECT_CACHE_REMOVED_FILTERS = 0x00000002,
    AM_GRAPH_CONFIG_RECONNECT_USE_ONLY_CACHED_FILTERS = 0x00000004
} AM_GRAPH_CONFIG_RECONNECT_FLAGS;

typedef struct _AllocatorProperties {
	long cBuffers;  // count of buffers at this allocator
	long cbBuffer;  // size of each buffer, excluding any prefix

	// alignment of the buffer - buffer start will be aligned on a multiple of
	// this amount
	long cbAlign;

	// prefix amount. Each buffer is immediately preceeded by cbPrefix bytes.
	// note that GetPointer points to the beginning of the buffer proper.
	// the prefix is aligned, i.e. (GetPointer() - cbPrefix) is aligned on cbAlign.
	long cbPrefix;
} ALLOCATOR_PROPERTIES;

typedef
enum tagPhysicalConnectorType
{	PhysConn_Video_Tuner	= 1,
PhysConn_Video_Composite	= PhysConn_Video_Tuner + 1,
PhysConn_Video_SVideo	= PhysConn_Video_Composite + 1,
PhysConn_Video_RGB	= PhysConn_Video_SVideo + 1,
PhysConn_Video_YRYBY	= PhysConn_Video_RGB + 1,
PhysConn_Video_SerialDigital	= PhysConn_Video_YRYBY + 1,
PhysConn_Video_ParallelDigital	= PhysConn_Video_SerialDigital + 1,
PhysConn_Video_SCSI	= PhysConn_Video_ParallelDigital + 1,
PhysConn_Video_AUX	= PhysConn_Video_SCSI + 1,
PhysConn_Video_1394	= PhysConn_Video_AUX + 1,
PhysConn_Video_USB	= PhysConn_Video_1394 + 1,
PhysConn_Video_VideoDecoder	= PhysConn_Video_USB + 1,
PhysConn_Video_VideoEncoder	= PhysConn_Video_VideoDecoder + 1,
PhysConn_Video_SCART	= PhysConn_Video_VideoEncoder + 1,
PhysConn_Video_Black	= PhysConn_Video_SCART + 1,
PhysConn_Audio_Tuner	= 0x1000,
PhysConn_Audio_Line	= PhysConn_Audio_Tuner + 1,
PhysConn_Audio_Mic	= PhysConn_Audio_Line + 1,
PhysConn_Audio_AESDigital	= PhysConn_Audio_Mic + 1,
PhysConn_Audio_SPDIFDigital	= PhysConn_Audio_AESDigital + 1,
PhysConn_Audio_SCSI	= PhysConn_Audio_SPDIFDigital + 1,
PhysConn_Audio_AUX	= PhysConn_Audio_SCSI + 1,
PhysConn_Audio_1394	= PhysConn_Audio_AUX + 1,
PhysConn_Audio_USB	= PhysConn_Audio_1394 + 1,
PhysConn_Audio_AudioDecoder	= PhysConn_Audio_USB + 1
} 	PhysicalConnectorType;


typedef
enum tagAnalogVideoStandard
{	AnalogVideo_None	= 0,
AnalogVideo_NTSC_M	= 0x1,
AnalogVideo_NTSC_M_J	= 0x2,
AnalogVideo_NTSC_433	= 0x4,
AnalogVideo_PAL_B	= 0x10,
AnalogVideo_PAL_D	= 0x20,
AnalogVideo_PAL_G	= 0x40,
AnalogVideo_PAL_H	= 0x80,
AnalogVideo_PAL_I	= 0x100,
AnalogVideo_PAL_M	= 0x200,
AnalogVideo_PAL_N	= 0x400,
AnalogVideo_PAL_60	= 0x800,
AnalogVideo_SECAM_B	= 0x1000,
AnalogVideo_SECAM_D	= 0x2000,
AnalogVideo_SECAM_G	= 0x4000,
AnalogVideo_SECAM_H	= 0x8000,
AnalogVideo_SECAM_K	= 0x10000,
AnalogVideo_SECAM_K1	= 0x20000,
AnalogVideo_SECAM_L	= 0x40000,
AnalogVideo_SECAM_L1	= 0x80000,
AnalogVideo_PAL_N_COMBO	= 0x100000
} 	AnalogVideoStandard;


typedef struct _VIDEO_STREAM_CONFIG_CAPS
{
	GUID guid;
	ULONG VideoStandard;
	SIZE InputSize;
	SIZE MinCroppingSize;
	SIZE MaxCroppingSize;
	int CropGranularityX;
	int CropGranularityY;
	int CropAlignX;
	int CropAlignY;
	SIZE MinOutputSize;
	SIZE MaxOutputSize;
	int OutputGranularityX;
	int OutputGranularityY;
	int StretchTapsX;
	int StretchTapsY;
	int ShrinkTapsX;
	int ShrinkTapsY;
	LONGLONG MinFrameInterval;
	LONGLONG MaxFrameInterval;
	LONG MinBitsPerSecond;
	LONG MaxBitsPerSecond;
} 	VIDEO_STREAM_CONFIG_CAPS;


/*
typedef struct  _MediaType {
    GUID      majortype;
    GUID      subtype;
    BOOL      bFixedSizeSamples;
    BOOL      bTemporalCompression;
    ULONG     lSampleSize;
    GUID      formattype;
    IUnknown  *pUnk;
    ULONG     cbFormat;
    BYTE 	  *pbFormat;
} AM_MEDIA_TYPE;
*/

//#pragma pack(push,1)
typedef struct  _MediaType {
    GUID      majortype;
    GUID      subtype;
    BOOL      bFixedSizeSamples;
    BOOL      bTemporalCompression;
    ULONG     lSampleSize;
    GUID      formattype;
    IUnknown  *pUnk;
    ULONG     cbFormat;
    BYTE *pbFormat;
} AM_MEDIA_TYPE;
//#pragma pack(pop)

typedef struct tagAM_SAMPLE2_PROPERTIES {
    DWORD           cbData;
    DWORD           dwTypeSpecificFlags;
    DWORD           dwSampleFlags;
    LONG            lActual;
    REFERENCE_TIME  tStart;
    REFERENCE_TIME  tStop;
    DWORD           dwStreamId;
    AM_MEDIA_TYPE   *pMediaType;
    BYTE            *pbBuffer;
    LONG            cbBuffer;
} AM_SAMPLE2_PROPERTIES;

//typedef struct _RECT {
//	LONG left;
//	LONG top;
//	LONG right;
//	LONG bottom;
//} RECT, *PRECT;

typedef struct tagVIDEOINFOHEADER {
	RECT                rcSource;
	RECT                rcTarget;
	DWORD               dwBitRate;
	DWORD               dwBitErrorRate;
	REFERENCE_TIME      AvgTimePerFrame;
	BITMAPINFOHEADER    bmiHeader;
} VIDEOINFOHEADER;


class IAMStreamConfig : public IUnknown
{
public:
	STDMETHOD(SetFormat)(/* [in] */ AM_MEDIA_TYPE *pmt) PURE;
	STDMETHOD(GetFormat)( /* [out] */ AM_MEDIA_TYPE **ppmt) PURE;
	STDMETHOD(GetNumberOfCapabilities) ( /* [out] */ int *piCount, /* [out] */ int *piSize) PURE;
	STDMETHOD(GetStreamCaps)( /* [in] */ int iIndex,
						  /* [out] */ AM_MEDIA_TYPE **ppmt,
						  /* [out] */ BYTE *pSCC) PURE;

};



DECLARE_HANDLE(HEVENT);
DECLARE_HANDLE(HSEMAPHORE);


class IReferenceClock : public IUnknown
{
	public:


        virtual HRESULT STDMETHODCALLTYPE GetTime(
            /* [out] */ REFERENCE_TIME *pTime) = 0;

        virtual HRESULT STDMETHODCALLTYPE AdviseTime(
            /* [in] */ REFERENCE_TIME baseTime,
            /* [in] */ REFERENCE_TIME streamTime,
            /* [in] */ HEVENT hEvent,
            /* [out] */ DWORD_PTR *pdwAdviseCookie) = 0;

        virtual HRESULT STDMETHODCALLTYPE AdvisePeriodic(
            /* [in] */ REFERENCE_TIME startTime,
            /* [in] */ REFERENCE_TIME periodTime,
            /* [in] */ HSEMAPHORE hSemaphore,
            /* [out] */ DWORD_PTR *pdwAdviseCookie) = 0;

        virtual HRESULT STDMETHODCALLTYPE Unadvise(
            /* [in] */ DWORD_PTR dwAdviseCookie) = 0;
};

class IPin : public IUnknown
{
public:

    STDMETHOD(Connect)(IPin*,const AM_MEDIA_TYPE*) PURE;
	STDMETHOD(ReceiveConnection)(IPin*,const AM_MEDIA_TYPE*) PURE;
	STDMETHOD(Disconnect)() PURE;
	STDMETHOD(ConnectedTo)(IPin**) PURE;
	STDMETHOD(ConnectionMediaType)(AM_MEDIA_TYPE*) PURE;
	STDMETHOD(QueryPinInfo)(PIN_INFO*) PURE;
	STDMETHOD(QueryDirection)(PIN_DIRECTION*) PURE;
	STDMETHOD(QueryId)(LPWSTR*) PURE;
	STDMETHOD(QueryAccept)(const AM_MEDIA_TYPE*) PURE;
	STDMETHOD(EnumMediaTypes)(IEnumMediaTypes**) PURE;
	STDMETHOD(QueryInternalConnections)(IPin**,ULONG*) PURE;
	STDMETHOD(EndOfStream)() PURE;
	STDMETHOD(BeginFlush)() PURE;
	STDMETHOD(EndFlush)() PURE;
	STDMETHOD(NewSegment)(REFERENCE_TIME,REFERENCE_TIME,double) PURE;
};

class IFilterGraph : public IUnknown
{
public:
    STDMETHOD(AddFilter)(IBaseFilter *, LPCWSTR) PURE;
    STDMETHOD(RemoveFilter)(IBaseFilter *) PURE;
    STDMETHOD(EnumFilters)(IEnumFilters **) PURE;
    STDMETHOD(FindFilterByName)(LPCWSTR, IBaseFilter **) PURE;
    STDMETHOD(ConnectDirect)(IPin *, IPin *, const AM_MEDIA_TYPE *) PURE;
    STDMETHOD(Reconnect)(IPin *) PURE;
    STDMETHOD(Disconnect)(IPin *) PURE;
    STDMETHOD(SetDefaultSyncSource)() PURE;
};

class IGraphConfigCallback;
class IPinConnection;
class IGraphConfig : public IUnknown
{
public:
	STDMETHOD(Reconnect)(IPin *pOutputPin,IPin *pInputPin,
		const AM_MEDIA_TYPE *pmtFirstConnection,
		IBaseFilter *pUsingFilter, HANDLE hAbortEvent,DWORD dwFlags) PURE;
	STDMETHOD(Reconfigure)(IGraphConfigCallback *pCallback,PVOID pvContext,DWORD dwFlags, HANDLE hAbortEvent) PURE;
	STDMETHOD(AddFilterToCache)(IBaseFilter *pFilter) PURE;
	STDMETHOD(RemoveFilterFromCache)(IBaseFilter *pFilter) PURE;
	STDMETHOD(EnumCacheFilter)(IEnumFilters **pEnum) PURE;
	STDMETHOD(GetStartTime)(REFERENCE_TIME *prtStart) PURE;
	STDMETHOD(PushThroughData)(IPin *pOutputPin,IPinConnection *pConnection,HANDLE hEventAbort) PURE;
	STDMETHOD(SetFilterFlags)(IBaseFilter *pFilter,DWORD dwFlags) PURE;
	STDMETHOD(GetFilterFlags)(IBaseFilter *pFilter,DWORD *pdwFlags) PURE;
	STDMETHOD(RemoveFilterEx)(IBaseFilter *pFilter,DWORD Flags) PURE;
};


class IGraphBuilder : public IFilterGraph
{
public:
    STDMETHOD(Connect)(IPin *, IPin *) PURE;
    STDMETHOD(Render)(IPin *) PURE;
    STDMETHOD(RenderFile)(LPCWSTR, LPCWSTR) PURE;
    STDMETHOD(AddSourceFilter)(LPCWSTR, LPCWSTR, IBaseFilter **) PURE;
    STDMETHOD(SetLogFile)(DWORD_PTR) PURE;
    STDMETHOD(Abort)(void) PURE;
    STDMETHOD(ShouldOperationContinue)(void) PURE;
};

class IFilterGraph2 : public IGraphBuilder
{
public:
	STDMETHOD(AddSourceFilterForMoniker)(IMoniker*, IBindCtx*, LPCWSTR, IBaseFilter**) PURE;
	STDMETHOD(ReconnectEx)(IPin*,const AM_MEDIA_TYPE *) PURE;
	STDMETHOD(RenderEx)(IPin *, DWORD, DWORD*) PURE;
};

class IMediaFilter : public IPersist
{
public:
	STDMETHOD(Stop)() PURE;
	STDMETHOD(Pause)() PURE;
	STDMETHOD(Run)(REFERENCE_TIME) PURE;
	STDMETHOD(GetState)(DWORD,FILTER_STATE*) PURE;
	STDMETHOD(SetSyncSource)(IReferenceClock*) PURE;
	STDMETHOD(GetSyncSource)(IReferenceClock**) PURE;
};

class IEnumPins : public IUnknown
{
public:
	STDMETHOD(Next)(ULONG,IPin**,ULONG*) PURE;
	STDMETHOD(Skip)(ULONG) PURE;
	STDMETHOD(Reset)(void) PURE;
	STDMETHOD(Clone)(IEnumPins**) PURE;
};

typedef IEnumPins *PENUMPINS;


class IEnumMediaTypes : public IUnknown
{
public:
	STDMETHOD(Next)(ULONG,AM_MEDIA_TYPE**,ULONG*) PURE;
	STDMETHOD(Skip)(ULONG) PURE;
	STDMETHOD(Reset)(void) PURE;
	STDMETHOD(Clone)(IEnumMediaTypes**) PURE;
};

typedef IEnumMediaTypes *PENUMMEDIATYPES;


class IBaseFilter : public IMediaFilter
{
public:
	STDMETHOD(EnumPins)(IEnumPins**) PURE;
	STDMETHOD(FindPin)(LPCWSTR,IPin**) PURE;
	STDMETHOD(JoinFilterGraph)(IFilterGraph*,LPCWSTR) PURE;
	STDMETHOD(QueryFilterInfo)(FILTER_INFO*) PURE;
	STDMETHOD(QueryVendorInfo)(LPWSTR*) PURE;
};

struct IMediaSample : public IUnknown
{
	STDMETHOD(GetPointer)(BYTE**) PURE;
	virtual long STDMETHODCALLTYPE GetSize() PURE;
	STDMETHOD(GetTime)(REFERENCE_TIME*,REFERENCE_TIME*) PURE;
	STDMETHOD(SetTime)(REFERENCE_TIME*,REFERENCE_TIME*) PURE;
	STDMETHOD(IsSyncPoint)() PURE;
	STDMETHOD(SetSyncPoint)(BOOL) PURE;
	STDMETHOD(IsPreroll)() PURE;
	STDMETHOD(SetPreroll)(BOOL) PURE;
	virtual long STDMETHODCALLTYPE GetActualDataLength() PURE;
	STDMETHOD(SetActualDataLength)(long) PURE;
	STDMETHOD(GetMediaType)(AM_MEDIA_TYPE**) PURE;
	STDMETHOD(SetMediaType)(AM_MEDIA_TYPE*) PURE;
	STDMETHOD(IsDiscontinuity)() PURE;
	STDMETHOD(SetDiscontinuity)(BOOL) PURE;
	STDMETHOD(GetMediaTime)(LONGLONG*,LONGLONG*) PURE;
	STDMETHOD(SetMediaTime)(LONGLONG*,LONGLONG*) PURE;
};


#define AM_GBF_PREVFRAMESKIPPED 1
#define AM_GBF_NOTASYNCPOINT    2
#define AM_GBF_NOWAIT			4
#define AM_GBF_NODDSURFACELOCK  8

struct IMemAllocator : IUnknown
{
	STDMETHOD(SetProperties)(ALLOCATOR_PROPERTIES*,ALLOCATOR_PROPERTIES*) PURE;
	STDMETHOD(GetProperties)(ALLOCATOR_PROPERTIES*) PURE;
	STDMETHOD(Commit)(void) PURE;
	STDMETHOD(Decommit)(void) PURE;
	STDMETHOD(GetBuffer)(IMediaSample**,REFERENCE_TIME*,REFERENCE_TIME*,DWORD) PURE;
	STDMETHOD(ReleaseBuffer)(IMediaSample*) PURE;
};

class IEnumFilters : public IUnknown
{
public:
	STDMETHOD(Clone)(IEnumFilters**) PURE;
	STDMETHOD(Next)(ULONG cFilters,IBaseFilter **ppFilter,ULONG *pcFetched) PURE;
	STDMETHOD(Reset)(void) PURE;
	STDMETHOD(Skip)(ULONG) PURE;
};

class IMediaEvent : public IDispatch {
public:
	STDMETHOD(CancelDefaultHandling)(long lEvCode)PURE;
	STDMETHOD(FreeEventParams)( long lEventCode,LONG_PTR lParam1,LONG_PTR lParam2)PURE;
	STDMETHOD(GetEvent)( long *lEventCode,LONG_PTR *lParam1,LONG_PTR *lParam2,long msTimeout)PURE;
	STDMETHOD(GetEventHandle)(OAEVENT *)PURE;
	STDMETHOD(RestoreDefaultHandling)(long)PURE;
	STDMETHOD(WaitForCompletion)(long,long*)PURE;

};

class IMediaEventEx : public IMediaEvent {
public:
	STDMETHOD(SetNotifyWindow)(OAHWND hwnd,long lMsg,LONG_PTR lInstanceData)PURE;
	STDMETHOD(SetNotifyFlags)(long)PURE;
	STDMETHOD(GetNotifyFlags)(long*)PURE;

};

typedef IMemAllocator *PMEMALLOCATOR;

class IMemInputPin : public IUnknown {
public:

	// return the allocator interface that this input pin
	// would like the output pin to use
	STDMETHOD(GetAllocator)(IMemAllocator ** ppAllocator) PURE;
	// tell the input pin which allocator the output pin is actually
	// going to use.
	// If the readonly flag is set, then all samples from this allocator are
	// to be treated as read-only, and should be copied before being modified.
	STDMETHOD(NotifyAllocator)(IMemAllocator * pAllocator,BOOL bReadOnly) PURE;

	// this method is optional (can return E_NOTIMPL). Output pins are not obliged to call
	// this method, nor are they obliged to fulfil the request. Input pins making such a
	// request should check the allocator in NotifyAllocator to see if it meets their needs. If
	// not, the input pin is responsible for any necessary data copy.
	// Zero values will be treated as don't care: so a pin can return an alignment value
	// and leave the other values 0.
	STDMETHOD(GetAllocatorRequirements)(ALLOCATOR_PROPERTIES*) PURE;

	// here's the next block of data from the stream. AddRef it if
	// you need to hold it beyond the end of the Receive call.
	// call pSample->Release when done with it.
	//
	// This is a blocking synchronous call.  Usually no blocking
	// will occur but if a filter cannot process the sample immediately
	// it may use the caller's thread to wait until it can.
	STDMETHOD(Receive)(IMediaSample * pSample) PURE;

	// Same as Receive but with multiple samples.  Useful for
	// fragmented streams
	STDMETHOD(ReceiveMultiple)(IMediaSample **pSamples,long nSamples,long *nSamplesProcessed) PURE;

	// See if Receive might block
	// Returns S_OK if it can block, S_FALSE if it can't or some
	// failure code (assume it can in this case)
	STDMETHOD(ReceiveCanBlock)(void) PURE;
};

typedef IMemInputPin *PMEMINPUTPIN;

class ISampleGrabberCB : public IUnknown
{
public:
	STDMETHOD(SampleCB)(double,IMediaSample*) PURE;
	STDMETHOD(BufferCB)(double,BYTE*,long) PURE;
};

class ISampleGrabber : public IUnknown
{
public:
	STDMETHOD(SetOneShot)(BOOL) PURE;
	STDMETHOD(SetMediaType)(const AM_MEDIA_TYPE *) PURE;
	STDMETHOD(GetConnectedMediaType)(AM_MEDIA_TYPE*) PURE;
	STDMETHOD(SetBufferSamples)(BOOL) PURE;
	STDMETHOD(GetCurrentBuffer)(long*,long*) PURE;
	STDMETHOD(GetCurrentSample)(IMediaSample**) PURE;
	STDMETHOD(SetCallback)(ISampleGrabberCB*,long) PURE;
};


class ICreateDevEnum : public IUnknown
{
public:
	STDMETHOD(CreateClassEnumerator)(REFCLSID,IEnumMoniker**,DWORD) PURE;
};

#if !defined(_MSC_VER) && !defined(__MINGW32__)//|| defined(_WIN32_WCE) // platform SDK
struct IPropertyBag : public IUnknown
{
	STDMETHOD(Read)(LPOLESTR,VARIANT*,IErrorLog*) PURE;
	STDMETHOD(Write)(LPOLESTR,VARIANT*) PURE;
};

struct IPersistPropertyBag : public IPersist
{
	STDMETHOD(InitNew)() PURE;
	STDMETHOD(Load)(IPropertyBag*,IErrorLog*) PURE;
	STDMETHOD(Save)(IPropertyBag*,BOOL,BOOL) PURE;
};

#endif


class IAMCrossbar : public IUnknown
{
public:
	STDMETHOD(CanRoute)(long,long) PURE;
	STDMETHOD(get_CrossbarPinInfo)(BOOL,long,long *,long *) PURE;
	STDMETHOD(get_IsRoutedTo)(long,long*) PURE;
	STDMETHOD(get_PinCounts)(long*,long*) PURE;
	STDMETHOD(Route)(long,long) PURE;

};


class IMediaControl : public IDispatch
{
public:
	STDMETHOD(Run)() PURE;
	STDMETHOD(Pause)() PURE;
	STDMETHOD(Stop)() PURE;
	STDMETHOD(GetState)(LONG,OAFilterState*) PURE;
	STDMETHOD(RenderFile)(BSTR) PURE;
	STDMETHOD(AddSourceFilter)(BSTR,IDispatch**) PURE;
	STDMETHOD(StopWhenReady)() PURE;
};

//struct IMediaEvent : public IDispatch
//{
//	STDMETHOD(GetEventHandle)(OAEVENT*) PURE;
//	STDMETHOD(GetEvent)(long*,LONG_PTR,LONG_PTR,long) PURE;
//	STDMETHOD(WaitForCompletion)(long,long*) PURE;
//	STDMETHOD(CancelDefaultHandling)(long) PURE;
//	STDMETHOD(RestoreDefaultHandling)(long) PURE;
//	STDMETHOD(FreeEventParams)(long,LONG_PTR,LONG_PTR) PURE;
//};


class ICaptureGraphBuilder : public IUnknown
{
};

class ICaptureGraphBuilder2 : public IUnknown
{
public:
	STDMETHOD(SetFiltergraph)(IGraphBuilder*) PURE;
	STDMETHOD(GetFiltergraph)(IGraphBuilder**) PURE;

	STDMETHOD(SetOutputFileName)(const GUID*, LPCOLESTR, IBaseFilter**, IFileSinkFilter**) PURE;

	STDMETHOD(FindInterface)(const GUID *,const GUID *,IBaseFilter *pf,REFIID riid, void **ppint) PURE;

	STDMETHOD(RenderStream)(const GUID*,const GUID*,IUnknown*,IBaseFilter*,IBaseFilter*) PURE;
	STDMETHOD(ControlStream)(const GUID*,const GUID*,IBaseFilter*,REFERENCE_TIME*,REFERENCE_TIME*,WORD,WORD) PURE;

	STDMETHOD(AllocCapFile)(LPCOLESTR,DWORDLONG) PURE;

	STDMETHOD(CopyCaptureFile)(LPCOLESTR,LPOLESTR,int,IAMCopyCaptureFileProgress*) PURE;

	STDMETHOD(FindPin)(IUnknown*,PIN_DIRECTION,const GUID*,const GUID*,BOOL,int,IPin**) PURE;
};

class IAMCopyCaptureFileProgress : public IUnknown
{
public:
	 virtual HRESULT STDMETHODCALLTYPE Progress(int iProgress) = 0;

};

#ifndef IPersistStream
/*
struct IPersistStream : public IPersist
{
	STDMETHOD(QueryInterface)( REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(GetClassID)( LPCLSID) PURE;
	STDMETHOD(IsDirty)(THIS) PURE;
	STDMETHOD(Load)( IStream*) PURE;
	STDMETHOD(Save)( IStream*,BOOL) PURE;
	STDMETHOD(GetSizeMax)( PULARGE_INTEGER) PURE;
};
*/
#endif


#endif

#endif
