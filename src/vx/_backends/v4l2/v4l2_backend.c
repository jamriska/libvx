#include "vx/config.h"

#if defined(HAVE_V4L2)

#include "_source.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <glob.h>

#define V4L_BUFFERS_COUNT 4

typedef struct vx_source_v4l2 {

    vx_source super;

    int _fd;
    unsigned int _type;

    struct v4l2_format _format;
    struct v4l2_buffer _buffer;
    struct v4l2_control _control;
    struct v4l2_requestbuffers _requestbuffers;

    void* memAddress[V4L_BUFFERS_COUNT];

    int _bufferIndex;

    vx_frame frame;

} vx_source_v4l2;


/* helpers */
#define VX_V4L2_CAST(ptr) \
    ((struct vx_source_v4l2*)(ptr))


int vx_source_v4l2_open(vx_source* s, const char* n)
{

    struct vx_source_v4l2 *source = VX_V4L2_CAST(s);
    int i = 0;


    source->frame.frame = 0;

    /* open the video device */
    source->_fd = v4l2_open(n, O_RDWR|O_NONBLOCK, 0);

    /* another check */
    if (source->_fd < 0)
        return -1;

    // request a format
    memset(&source->_format,0,sizeof(struct v4l2_format));

    source->_format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;


    /** \todo need proper configuration */
    unsigned int requestFormat = V4L2_PIX_FMT_YUYV;

    source->_format.fmt.pix.width       = 640;
    source->_format.fmt.pix.height      = 480;
    source->_format.fmt.pix.pixelformat      = requestFormat;
    source->_format.fmt.pix.field            = V4L2_FIELD_INTERLACED;

    // check if we can capture in this format
    ioctl(source->_fd, VIDIOC_S_FMT, &source->_format);

    // post-check
    if (source->_format.fmt.pix.pixelformat != requestFormat)
    {
        printf("libv4l didn't accept 0x%x format for %s. Can't proceed.\n",requestFormat,n);
        return -1;
        //exit(EXIT_FAILURE);
    }


    // set the camera speed
    struct v4l2_streamparm streamparm;
    memset (&streamparm, 0, sizeof(struct v4l2_streamparm));
    streamparm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    streamparm.parm.capture.timeperframe.numerator = 1;
    streamparm.parm.capture.timeperframe.denominator = 15;

    ioctl(source->_fd, VIDIOC_S_PARM, &streamparm);


    memset(&source->_requestbuffers,0,sizeof(struct v4l2_requestbuffers));
    source->_requestbuffers.count = V4L_BUFFERS_COUNT;
    source->_requestbuffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    source->_requestbuffers.memory = V4L2_MEMORY_MMAP;
    if (ioctl(source->_fd, VIDIOC_REQBUFS, &source->_requestbuffers) < 0)
        return -1;

    for (i = 0; i < source->_requestbuffers.count; ++i)
    {
        memset(&source->_buffer,0,sizeof(struct v4l2_buffer));

        source->_buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        source->_buffer.memory = V4L2_MEMORY_MMAP;
        source->_buffer.index = i;

        if (-1 == ioctl (source->_fd, VIDIOC_QUERYBUF, &source->_buffer))
        {
            perror ("VIDIOC_QUERYBUF");
            //close();
            return -1;
        }

        source->memAddress[i] = mmap((void*)0,source->_buffer.length,PROT_READ|PROT_WRITE,MAP_SHARED,source->_fd, source->_buffer.m.offset);


        if (MAP_FAILED == source->memAddress[i]) {
            perror ("mmap");

            return -1;
        }

        if (ioctl(source->_fd, VIDIOC_QBUF, &source->_buffer) < 0) {

            return -1;
        }
    }

    return 0;
}

int vx_source_v4l2_close(vx_source* s)
{
    vx_source_v4l2 *source = VX_V4L2_CAST(s);

//    int i = 0;
//    for (i = 0; i < source->_requestbuffers.count; ++i)
//        v4l2_munmap(source->buffers[i].start, source->buffers[i].length);

    if (source->_fd != 0)
        source->_fd = v4l2_close(source->_fd);

    printf("%s %d\n",__FUNCTION__,__LINE__);
    return 0;
}

int vx_source_v4l2_set_state(vx_source* s,int state)
{
    vx_source_v4l2 *source = VX_V4L2_CAST(s);
    source->_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    switch (state) {
    case VX_SOURCE_STATE_RUNNING:
        return ioctl(source->_fd, VIDIOC_STREAMON, &source->_type);

    case VX_SOURCE_STATE_STOP:
        return ioctl(source->_fd, VIDIOC_STREAMOFF, &source->_type);

    }

    printf("%s %d\n",__FUNCTION__,__LINE__);
    return 0;
}

int vx_source_v4l2_get_state(vx_source* s,int* state)
{
    printf("%s %d\n",__FUNCTION__,__LINE__);
    return 0;
}



int vx_source_v4l2_fillcapabilities(int fd,vx_device_description* desc)
{
    int ret;
    struct v4l2_fmtdesc fmt;
    vx_device_capability newCapability;

    memset(&fmt, 0, sizeof(fmt));
    fmt.index = 0;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while ((ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmt)) == 0) {

        struct v4l2_frmsizeenum fsize;
        memset(&fsize, 0, sizeof(fsize));
        fsize.index = 0;
        fsize.pixel_format = fmt.pixelformat;

        /* enumerate sizes */
        while ((ret = ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &fsize)) == 0) {
            /* we only deal with discrete sizes */
            if (fsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {

                struct v4l2_frmivalenum fival;

                memset(&fival, 0, sizeof(fival));
                fival.index = 0;
                fival.pixel_format = fmt.pixelformat;
                fival.width = fsize.discrete.width;
                fival.height = fsize.discrete.height;

                while ((ret = ioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &fival)) == 0) {
                    if (fival.type == V4L2_FRMIVAL_TYPE_DISCRETE) {


                        newCapability.width = fsize.discrete.width;
                        newCapability.height = fsize.discrete.height;

                        newCapability.pixelFormat = fmt.pixelformat;

                        newCapability.speed.numerator = fival.discrete.numerator;
                        newCapability.speed.denominator = fival.discrete.denominator;

                        if (_vx_source_addcapability(&newCapability,&desc->capabilities,&desc->capabilitiesCount) < 0)
                            return -1;


                    }

                    fival.index++;
                }


            }


            fsize.index++;

        }


        fmt.index++;

    }

    if (errno != EINVAL) {
        printf("ERROR enumerating frame formats: %d\n", errno);
        return errno;
    }

}

int vx_source_v4l2_enumerate(vx_source* s)
{
    glob_t globr;
    int i = 0;


    glob("/dev/video*",0,0,&globr);

    for (i = 0;i < globr.gl_pathc;++i) {

        //        printf("Path %s\n",globr.gl_pathv[i]);

        int fd = open(globr.gl_pathv[i],O_RDWR);

        if (fd < 0)
            continue;

        struct v4l2_input device;
        memset(&device,0,sizeof(struct v4l2_input));

        while(ioctl(fd,VIDIOC_ENUMINPUT,&device) >= 0) {

            if (device.type == 0 || device.type == V4L2_INPUT_TYPE_CAMERA) {
                if (ioctl(fd,VIDIOC_S_INPUT,device.index) != 0) {


                    int newSizeDesc = s->deviceCount + 1;


                    vx_device_description* pNewDesc = 0L;

                    if (newSizeDesc == 1) {

                        pNewDesc = (vx_device_description*)malloc(sizeof(vx_device_description));

                    } else {
                        pNewDesc = (vx_device_description*)realloc(s->devices,
                                                                   newSizeDesc * (sizeof(vx_device_description)));

                    }


                    if (pNewDesc) {

                        memset(&pNewDesc[s->deviceCount],0,sizeof(struct vx_device_description));


                        struct v4l2_capability cap;
                        memset(&cap,0,sizeof(struct v4l2_capability));

                        if (ioctl(fd,VIDIOC_QUERYCAP,&cap) != 0) {
                            pNewDesc[s->deviceCount].name = strdup(globr.gl_pathv[i]);
                        } else {
                            pNewDesc[s->deviceCount].name = strdup(cap.card);
                        }

                        pNewDesc[s->deviceCount].uuid = strdup(globr.gl_pathv[i]);

                        vx_source_v4l2_fillcapabilities(fd,&pNewDesc[s->deviceCount]);

                        s->devices = pNewDesc;
                        s->deviceCount++;

                    }
                }
            }

            ++device.index;

        }

        close(fd);

    }

    globfree(&globr);

    return 0;
}

int vx_source_v4l2_update(vx_source* s,unsigned int runloop)
{

    fd_set fdsetRead,fdsetNull;
    struct timeval tv;
    int r,i;
    struct vx_source_v4l2 *source = VX_V4L2_CAST(s);

    FD_ZERO(&fdsetRead);

    FD_ZERO(&fdsetNull);

    FD_SET(source->_fd, &fdsetRead);

    /* Timeout. */
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    /* we don't have a new frame */
    if (select(source->_fd + 1, &fdsetRead, &fdsetNull, &fdsetNull, &tv) < 0)
        return 0;


    struct v4l2_buffer buffer;

    memset(&buffer,0,sizeof(struct v4l2_buffer));
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory = V4L2_MEMORY_MMAP;


    if (ioctl (source->_fd, VIDIOC_DQBUF, &buffer) < 0)
        return -1;


    struct vx_frame* frame = &source->frame;
    int bufferIndex = source->_buffer.index;

    unsigned int bytes_per_row = source->_format.fmt.pix.width * 3;

    frame->data = source->memAddress[bufferIndex];
    frame->dataSize = source->_buffer.bytesused;

    frame->stride = bytes_per_row;
    frame->width = source->_format.fmt.pix.width;
    frame->height = source->_format.fmt.pix.height;


    frame->colorModel = VX_E_COLOR_RGB24;
    frame->bpp = 24;
//    frame->tick = buffer.timestamp;

    frame->frame++;

    _vx_send_frame(s,frame);

    // check in the buffer
    ioctl(source->_fd, VIDIOC_QBUF, &source->_buffer);


    _vx_broadcast(s);


//    // try to check out the buffer
//    if (-1 == xioctl (source->_fd, VIDIOC_DQBUF, &source->_buffer))
//    {
//        switch (errno) {
//        case EAGAIN:
//            return -1;
//        case EIO:
//            /* Could ignore EIO, see spec. */
//            break;
//        default:
//            /* display the error and stop processing */
//            perror ("VIDIOC_DQBUF");
//            return -2;
//        }
//    }


    return 0;
}

void*
vx_source_v4l2_create()
{
    vx_source_v4l2* s = malloc(sizeof(vx_source_v4l2));

    memset(s,0,sizeof(struct vx_source_v4l2));


    VX_OBJECT_CAST(s)->destroy = 0;
    VX_OBJECT_CAST(s)->id = 0;
    VX_OBJECT_CAST(s)->refCount = 0;


    VX_SOURCE_CAST(s)->deviceCount = 0;
    VX_SOURCE_CAST(s)->devices = 0L;


    VX_SOURCE_CAST(s)->open = vx_source_v4l2_open;
    VX_SOURCE_CAST(s)->close = vx_source_v4l2_close;
    VX_SOURCE_CAST(s)->set_state = vx_source_v4l2_set_state;
    VX_SOURCE_CAST(s)->get_state = vx_source_v4l2_get_state;
    VX_SOURCE_CAST(s)->enumerate = vx_source_v4l2_enumerate;
    VX_SOURCE_CAST(s)->update = vx_source_v4l2_update;

    return s;
}

#else

void*
vx_source_v4l2_create()
{
    return 0;
}

#endif
