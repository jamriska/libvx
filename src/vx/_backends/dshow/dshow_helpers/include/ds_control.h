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

#ifndef DS_CONTROL_H
#define DS_CONTROL_H

#include "ds_capture.h"


class DirectShowControl {

    IBaseFilter*			_SourceFilter;
    IBaseFilter*			_ProxyFilter;
    IBaseFilter*			_SinkFilter;

    IMediaEvent*			_MediaEvent;
    IMediaControl*			_MediaControl;

    ICaptureGraphBuilder2*	_GraphBuilder;
    IGraphBuilder*			_Graph;

    unsigned char _rendererType;
    bool _allowDropFrames;
    bool _saveGraph;
    bool _useColorSpaceFilter;

public:

    enum RendererType {
        kVideoSink,
        kCustomSink,
        kNullSink
    };

    DirectShowControl();

	virtual ~DirectShowControl();

    bool Open(const char *uuid);

    bool Start();

    bool Stop();

    bool Pause();

    void SetQueryAcceptCallback(QueryAcceptCallback* cb);

    void SetSampleCallback(SampleCallback *cb);

	virtual void OnError(const char* error) const;

    bool ConnectComponents();

protected:

    bool _InitFilterGraph();

    bool _InitGraphBuilder();

    bool _InitCOM();

    bool _InitCaptureDevice(const char* uuid);

    bool _InitRenderer();

    bool _InitColorSpaceFilter();

    bool _Run();

    bool _Stop();

    bool _FindDevice(const char *UID);

};

#endif
