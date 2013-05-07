/*
Copyright (c) 2006-2013 Hartmut Seichter
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the VideoExtractor Project.
*/
#ifndef _LIBVX_VX_SOURCE_H_
#define _LIBVX_VX_SOURCE_H_ 1

/** @file */

#include <vx/cdecl.h>
#include <vx/sink.h>

/**
 * @brief The vx_source_state enum
 */
enum vx_source_state {
    VX_SOURCE_STATE_NULL		= 0,        /**< Do nothing */
    VX_SOURCE_STATE_READY,                  /**< Source is ready */
    VX_SOURCE_STATE_PAUSED,                 /**< Source is paused */
    VX_SOURCE_STATE_RUNNING,                /**< Source is running */
    VX_SOURCE_STATE_FINISHED,               /**< Source is finished (file) */
    VX_SOURCE_STATE_STOP,                   /**< Source is stopped */
    VX_SOURCE_STATE_USERDEFINED = 0xFF      /**< uchar padding */
};

/**
 * @brief Update methods
 */
enum vx_source_update_method {
    VX_SOURCE_UPDATE_NONE		= 0,        /**< Do nothing */
    VX_SOURCE_UPDATE_PEEK,                  /**< Only peek */
    VX_SOURCE_UPDATE_FULL       = 0xff      /**< Taking over processing */
};


/**
  * @brief description of devices
  */
typedef struct vx_source_description {
    char* name;                             /**< UTF-8 encoded display name */
    char* uuid;                             /**< Platform specific UUID */
} vx_source_description;


/**
  * Returns tag for default backend on this platform
  *
  * @return string that can be used as input for vx_source_create
  */
VX_API_CALL const char * vx_source_default();

/**
 * Allocates a video source
 *
 * @param n backed to be used
 * @return instance of the capture backend
 */
VX_API_CALL void* vx_source_create(const char *n);

/**
  * Enumerates devices available in the backend
  *
  * @param s source instance
  * @param e null terminated vector of source descriptions
  * @return 0 if successful / otherwise error code
  */
VX_API_CALL int vx_source_enumerate(vx_source *s,vx_source_description** e);

/**
  * Opens the device described on the device
  *
  * @param s instance of the source
  * @param settings - usually the uuid acquired by the enumerator
  * @return error code
  */
VX_API_CALL int vx_source_open(vx_source *s,const char* settings);

/**
  * Closes the instance - stops the capture process first.
  *
  * @param s instance
  * @return error code
  */
VX_API_CALL int vx_source_close(vx_source *s);

/**
  * Updates the internal threads. Required for most backends as they need
  * to have a trigger for their message pump. This also depends on the
  * application scenario.
  *
  * @param s instance
  * @param runloop flag determining the update method
  * @return error code
  */
VX_API_CALL int vx_source_update(vx_source* s, unsigned int runloop);

/**
  * Set the state of the internal pipeline. This call allows to start,
  * stop and pause, resume the capture process.
  *
  * @param s instance
  * @param newstate requested state of the capture process
  * @return error code
  */
VX_API_CALL int vx_source_set_state(vx_source *s,int newstate);

/**
  * Retrieve the state of the pipeline.
  *
  * @param s instance
  * @param outstate retrieves the state
  * @return error code
  */
VX_API_CALL int vx_source_get_state(vx_source *s,int* outstate);

/**
  * Push data into the backend. This is used to set specific flags in the
  * system.
  *
  * @param s instance
  * @param tag a descriptor for the flag
  * @param data pointer to data
  * @return error code
  */
VX_API_CALL int vx_source_set(vx_source *s,int tag,const void* data);

/**
  * Retrieves data from the internal settings.
  *
  * @param s instance
  * @param tag descriptor for the flag
  * @param data retrieved data
  * @return error code
  */
VX_API_CALL int vx_source_get(vx_source *s,int tag,void* data);

/**
  * Attaches a sink to the source. The sink is used to distribute the
  * incoming frames to the application.
  *
  * @param source instance of a source
  * @param sink instance of a sink
  * @return error code
  */
VX_API_CALL int vx_source_add_sink(vx_source* source, vx_sink* sink);

/**
  * Detaches a sink from the source.
  *
  * @param source instance of a source
  * @param c instance of a sink
  * @return error code
  */
VX_API_CALL int vx_source_remove_sink(vx_source* source, vx_sink* c);

/**
  * Increase the reference counter
  */
VX_API_CALL int vx_source_ref(vx_source *s);

/**
  * Decrease the reference counter
  */
VX_API_CALL int vx_source_unref(vx_source *s);


#endif
