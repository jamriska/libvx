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

//#include <libv4l2.h>


struct buffer {
        void   *start;
        size_t length;
};

#define MAX_V4L_BUFFERS 10
#define DEFAULT_V4L_BUFFERS 4

#define CLEAR(x) memset(&(x), 0, sizeof(x))


typedef struct vx_source_v4l2 {

    vx_source super;

    int _fd;
    unsigned int _type;


//    int _flipcontrol;

    struct v4l2_format _format;
    struct v4l2_buffer _buffer;
    struct v4l2_control _control;
    struct v4l2_requestbuffers _requestbuffers;

//    v4l2_buf_type _buf_type;
    struct buffer buffers[MAX_V4L_BUFFERS + 1];
    int _bufferIndex;

    vx_frame frame;

} vx_source_v4l2;


/* helpers */
#define VX_V4L2_CAST(ptr) \
    ((vx_source_v4l2*)(ptr))



static int xioctl(int fh, int request, void *arg)
{
        int r;

        do {
                r = v4l2_ioctl(fh, request, arg);
        } while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

        if (r == -1) {
                fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
                return -1;
        }
}

int vx_source_v4l2_open(vx_source* s, const char* n)
{
    printf("%s %s\n",__FUNCTION__,n);

    /* open the video device */
    VX_V4L2_CAST(s)->_fd = v4l2_open(n, O_RDWR | O_NONBLOCK, 0);

    /* another check */
    if (VX_V4L2_CAST(s)->_fd < 0)
        return -1;

    // request a format
    CLEAR(VX_V4L2_CAST(s)->_format);

    VX_V4L2_CAST(s)->_format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;


    /** \todo need proper configuration */

    VX_V4L2_CAST(s)->_format.fmt.pix.width       = 320;
    VX_V4L2_CAST(s)->_format.fmt.pix.height      = 240;

    VX_V4L2_CAST(s)->_format.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
         VX_V4L2_CAST(s)->_format.fmt.pix.field       = V4L2_FIELD_INTERLACED;

    // check if we can capture in this format
    xioctl(VX_V4L2_CAST(s)->_fd, VIDIOC_S_FMT, &VX_V4L2_CAST(s)->_format);

    // post-check
    if (VX_V4L2_CAST(s)->_format.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB24)
    {
       printf("Libv4l didn't accept RGB24 format. Can't proceed.\n");
        return -1;
             //exit(EXIT_FAILURE);
    }


    // set the camera speed
    struct v4l2_streamparm streamparm;
    memset (&streamparm, 0, sizeof(struct v4l2_streamparm));
    streamparm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    streamparm.parm.capture.timeperframe.numerator = 1;
    streamparm.parm.capture.timeperframe.denominator = 15;

    xioctl(VX_V4L2_CAST(s)->_fd, VIDIOC_S_PARM, &streamparm);


    CLEAR(VX_V4L2_CAST(s)->_requestbuffers);
    VX_V4L2_CAST(s)->_requestbuffers.count = 4;
    VX_V4L2_CAST(s)->_requestbuffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    VX_V4L2_CAST(s)->_requestbuffers.memory = V4L2_MEMORY_MMAP;
    xioctl(VX_V4L2_CAST(s)->_fd, VIDIOC_REQBUFS, &VX_V4L2_CAST(s)->_requestbuffers);



    int n_buffers;
   for (n_buffers = 0; n_buffers < VX_V4L2_CAST(s)->_requestbuffers.count; ++n_buffers)
   {
       //struct v4l2_buffer buf;

       CLEAR (VX_V4L2_CAST(s)->_buffer);

       VX_V4L2_CAST(s)->_buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
       VX_V4L2_CAST(s)->_buffer.memory = V4L2_MEMORY_MMAP;
       VX_V4L2_CAST(s)->_buffer.index = n_buffers;

       if (-1 == xioctl (VX_V4L2_CAST(s)->_fd, VIDIOC_QUERYBUF, &VX_V4L2_CAST(s)->_buffer))
       {
           perror ("VIDIOC_QUERYBUF");
           //close();
            return -1;
       }

       //printf("%s try to MMAP buffer %d\n",__FUNCTION__,n_buffers);

       VX_V4L2_CAST(s)->buffers[n_buffers].length = VX_V4L2_CAST(s)->_buffer.length;
       VX_V4L2_CAST(s)->buffers[n_buffers].start = v4l2_mmap (NULL /* start anywhere */,
               VX_V4L2_CAST(s)->_buffer.length,
               PROT_READ | PROT_WRITE /* required */,
               MAP_SHARED /* recommended */,
               VX_V4L2_CAST(s)->_fd, VX_V4L2_CAST(s)->_buffer.m.offset);
       //printf("%s post MMAP buffer %d\n",__FUNCTION__,n_buffers);
       if (MAP_FAILED == VX_V4L2_CAST(s)->buffers[n_buffers].start) {
           perror ("mmap");

           /* free capture, and returns an error code */
            //this->close();

           return -1;
       }

    }

   int i = 0;
   for (i = 0; i < n_buffers; ++i)
   {
       CLEAR(VX_V4L2_CAST(s)->_buffer);
       VX_V4L2_CAST(s)->_buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
       VX_V4L2_CAST(s)->_buffer.memory = V4L2_MEMORY_MMAP;
       VX_V4L2_CAST(s)->_buffer.index = i;
       xioctl(VX_V4L2_CAST(s)->_fd, VIDIOC_QBUF, &VX_V4L2_CAST(s)->_buffer);
   }

    printf("%s %d\n",__FUNCTION__,__LINE__);
    return 0;
}

int vx_source_v4l2_close(vx_source* s)
{
    int i = 0;
    for (i = 0; i < VX_V4L2_CAST(s)->_requestbuffers.count; ++i)
        v4l2_munmap(VX_V4L2_CAST(s)->buffers[i].start, VX_V4L2_CAST(s)->buffers[i].length);

    if (VX_V4L2_CAST(s)->_fd != 0)
        VX_V4L2_CAST(s)->_fd = v4l2_close(VX_V4L2_CAST(s)->_fd);

    printf("%s %d\n",__FUNCTION__,__LINE__);
    return 0;
}

int vx_source_v4l2_set_state(vx_source* s,int state)
{
    VX_V4L2_CAST(s)->_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    switch (state) {
    case VX_SOURCE_STATE_RUNNING:
        return xioctl(VX_V4L2_CAST(s)->_fd, VIDIOC_STREAMON, &VX_V4L2_CAST(s)->_type);

    case VX_SOURCE_STATE_STOP:
        return xioctl(VX_V4L2_CAST(s)->_fd, VIDIOC_STREAMOFF, &VX_V4L2_CAST(s)->_type);

    }

    printf("%s %d\n",__FUNCTION__,__LINE__);
    return 0;
}

int vx_source_v4l2_get_state(vx_source* s,int* state)
{
    printf("%s %d\n",__FUNCTION__,__LINE__);
    return 0;
}

int vx_source_v4l2_enumerate(vx_source* s, vx_device_description **e, int *size)
{
    glob_t globr;
    int i = 0;

    glob("/dev/video*",0,0,&globr);

    for (i = 0;i < globr.gl_pathc;++i) {

        printf("Path %s\n",globr.gl_pathv[i]);

        int fd = open(globr.gl_pathv[i],O_RDWR);

        if (fd < 0)
            continue;

        struct v4l2_input device;
        memset(&device,0,sizeof(struct v4l2_input));

        while(ioctl(fd,VIDIOC_ENUMINPUT,&device) >= 0) {

            if (device.type == 0 || device.type == V4L2_INPUT_TYPE_CAMERA) {
                if (ioctl(fd,VIDIOC_S_INPUT,device.index) != 0) {


                    int newSizeDesc = s->deviceCount + 1;

                    vx_device_description* pNewDesc =
                            (vx_device_description*)realloc(s->devices,
                                                            newSizeDesc * (sizeof(vx_device_description)));

                    if (pNewDesc) {

                        struct v4l2_capability cap;
                        memset(&cap,0,sizeof(struct v4l2_capability));

                        if (ioctl(fd,VIDIOC_QUERYCAP,&cap) != 0) {
                            pNewDesc[s->deviceCount].name = strdup(globr.gl_pathv[i]);
                        } else {
                            pNewDesc[s->deviceCount].name = strdup(cap.card);
                        }

                        pNewDesc[s->deviceCount].uuid = strdup(globr.gl_pathv[i]);

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

    *e = s->devices;
    *size = s->deviceCount;



    printf("%s %d\n",__FUNCTION__,__LINE__);
    return 0;
}

int vx_source_v4l2_update(vx_source* s,int runloop)
{

    fd_set fds;
    struct timeval tv;
    int r,i;

    FD_ZERO(&fds);
    FD_SET(VX_V4L2_CAST(s)->_fd, &fds);

    /* Timeout. */
    tv.tv_sec = 0;
    tv.tv_usec = 100;

    r = select(VX_V4L2_CAST(s)->_fd, &fds, NULL, NULL, &tv);

    /* basically mean we don't have a new frame */
    if ( (r == -1) && errno == EINTR ) {
        fprintf(stderr,"Error Select!\n");
        return 0;
    }

    CLEAR(VX_V4L2_CAST(s)->_buffer);
    VX_V4L2_CAST(s)->_buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    VX_V4L2_CAST(s)->_buffer.memory = V4L2_MEMORY_MMAP;

    // try to check out the buffer
    if (-1 == xioctl (VX_V4L2_CAST(s)->_fd, VIDIOC_DQBUF, &VX_V4L2_CAST(s)->_buffer))
    {
        switch (errno) {
            case EAGAIN:
                return -1;
            case EIO:
                /* Could ignore EIO, see spec. */
                break;
            default:
                /* display the error and stop processing */
                perror ("VIDIOC_DQBUF");
                return -2;
            }
    }

    // calc offset
    unsigned int bytes_per_row = VX_V4L2_CAST(s)->_format.fmt.pix.width * 3;


    VX_V4L2_CAST(s)->frame.frame++;
    VX_V4L2_CAST(s)->frame.data = VX_V4L2_CAST(s)->buffers[VX_V4L2_CAST(s)->_buffer.index].start;
    VX_V4L2_CAST(s)->frame.dataSize = VX_V4L2_CAST(s)->buffers[VX_V4L2_CAST(s)->_buffer.index].length;

    VX_V4L2_CAST(s)->frame.stride = bytes_per_row;
    VX_V4L2_CAST(s)->frame.width = VX_V4L2_CAST(s)->_format.fmt.pix.width;
    VX_V4L2_CAST(s)->frame.height = VX_V4L2_CAST(s)->_format.fmt.pix.height;


    VX_V4L2_CAST(s)->frame.colorModel = 0;

    _vx_send_frame(s,&VX_V4L2_CAST(s)->frame);

    // check in the buffer
    xioctl(VX_V4L2_CAST(s)->_fd, VIDIOC_QBUF, &VX_V4L2_CAST(s)->_buffer);

    printf("%s %d\n",__FUNCTION__,__LINE__);
    return 0;
}

void*
vx_source_v4l2_create()
{
    vx_source_v4l2* s = malloc(sizeof(vx_source_v4l2));

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
