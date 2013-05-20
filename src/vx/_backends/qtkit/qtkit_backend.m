#include "qtkit_backend.h"
#include "vx/config.h"

#if defined(HAVE_QTKIT)

#include "_source.h"
#include "vx/frame.h"

#import <QTKit/QTkit.h>

/* delegate for receiving the buffer samples - acts as a holder for the session */
@interface VXvideoController : NSObject
{
	QTCaptureSession *session;
	QTCaptureDevice *captureDevice;
	QTCaptureDeviceInput *captureDeviceInput;
	QTCaptureDecompressedVideoOutput *decompressedVideoOutput;

	vx_source* source;
	vx_frame frame;
}

@property (readwrite, retain) QTCaptureSession *session;
@property (readwrite, retain) QTCaptureDevice *captureDevice;
@property (readwrite, retain) QTCaptureDeviceInput *captureDeviceInput;
@property (readwrite, retain) QTCaptureDecompressedVideoOutput *decompressedVideoOutput;

@end
@implementation VXvideoController

@synthesize session, captureDevice, captureDeviceInput, decompressedVideoOutput;

/*
 initialization of UI elements
 */
- (id) init: (vx_source*) aSource
{
	[super init];
	source = aSource;
	self.session = nil;
	return self;
}

/*
 event that fires when memory is to be deallocated when object is killed
 */
- (void)dealloc
{

	@try {
		[session stopRunning];

		[session release];
		[captureDevice release];
		[captureDeviceInput release];
		[decompressedVideoOutput release];
	}
	@catch (NSException * e) {
		NSLog(@"%s: %@\n%@",__FUNCTION__,[e name],[e reason]);
	}
	@finally {
		//[pool release];
	}

	[super dealloc];
}

- (BOOL)open: (NSString*) configuration
{
	NSError* error;

	if ([configuration length])
		captureDevice = [QTCaptureDevice deviceWithUniqueID:configuration ];
	else
		captureDevice = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeVideo];

	if (captureDevice)
	{
		if ([captureDevice open:&error]) {

			captureDeviceInput = [[QTCaptureDeviceInput alloc] initWithDevice:captureDevice];

			if (captureDeviceInput)  {

				self.session = [[QTCaptureSession alloc] init];

				if ([self.session addInput:captureDeviceInput error:&error])
				{
					decompressedVideoOutput = [[QTCaptureDecompressedVideoOutput alloc] init];



					NSDictionary* videoOutputSettings;
					videoOutputSettings = [NSDictionary dictionaryWithObjectsAndKeys:
							[NSNumber numberWithInt:320], (id)kCVPixelBufferWidthKey,
							[NSNumber numberWithInt:240], (id)kCVPixelBufferHeightKey,
							[NSNumber numberWithUnsignedInt:kCVPixelFormatType_32ARGB], (id)kCVPixelBufferPixelFormatTypeKey,
							[NSNumber numberWithBool:YES], (id)kCVPixelBufferOpenGLCompatibilityKey,
							nil];

					[decompressedVideoOutput setPixelBufferAttributes:videoOutputSettings];

//					[decompressedVideoOutput setPixelBufferAttributes:[NSDictionary dictionaryWithObjectsAndKeys:
//							[NSNumber numberWithUnsignedInt:kCVPixelFormatType_24RGB], kCVPixelBufferPixelFormatTypeKey,
//							nil]];


					decompressedVideoOutput.delegate = self;

					if([self.session addOutput:decompressedVideoOutput error:&error]) {
						return YES;
					}
				}
			}
		}
	}

	return NO;
}

- (void)stop
{
	if (self.session) [self.session stopRunning];
}


- (void)start
{
	frame.frame = 0;
	if (self.session) [self.session startRunning];
}

- (BOOL)isRunning
{
	return (self.session) ? [self.session isRunning] : NO;
}

- (void)captureOutput:(QTCaptureOutput *)captureOutput
  didOutputVideoFrame:(CVImageBufferRef)videoFrame
  withSampleBuffer:(QTSampleBuffer *)sampleBuffer
  fromConnection:(QTCaptureConnection *)connection
{
		char fourCC[4];

	CVBufferRetain(videoFrame);

	// we need to do this first - otherwise we potentially get no baseaddress
	OSStatus err = CVPixelBufferLockBaseAddress( videoFrame, 0 );
	if( err ) {
		NSLog( @"CVPixelBufferLockBaseAddress() failed (%d)\n", err );
		return;
	}

	OSType formatType = CVPixelBufferGetPixelFormatType(videoFrame);

	frame.frame++;
	frame.data = CVPixelBufferGetBaseAddress(videoFrame);
	frame.stride = CVPixelBufferGetBytesPerRow(videoFrame);
	frame.width = CVPixelBufferGetWidth(videoFrame);
	frame.height = CVPixelBufferGetHeight(videoFrame);
	frame.dataSize = CVPixelBufferGetDataSize(videoFrame);

	switch(formatType) {
	case k24BGRPixelFormat:
		frame.colorModel = VX_E_COLOR_BGR24;
		frame.bpp = 24;
		break;
	case k24RGBPixelFormat:
		frame.colorModel = VX_E_COLOR_RGB24;
		frame.bpp = 24;
		break;
	case k32ARGBPixelFormat:
		frame.colorModel = VX_E_COLOR_ARGB;
		frame.bpp = 32;
		break;
	default:
		memcpy(fourCC,(const void*)&formatType,sizeof(OSType));
		NSLog(@"Unhandled Color Format: %d '%c%c%c%c'",formatType,
			  fourCC[0],fourCC[1],fourCC[2],fourCC[3]);

	};

	_vx_send_frame(source,&frame);

	CVBufferRelease(videoFrame);

}


@end

/* actual interface */

typedef struct vx_source_qtkit {
	vx_source				super;
	VXvideoController		*delegate;
} vx_source_qtkit;


/* helpers */
#define VX_QTKIT_CAST(ptr) \
	((vx_source_qtkit*)(ptr))


int vx_source_qtkit_enumerate(vx_source* s)
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	NSArray* devices = [QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo];

	if ([devices count] > 0)
	{
		s->deviceCount = [devices count];
		s->devices = malloc(sizeof(vx_device_description) * s->deviceCount);

		int i = 0;
		for (QTCaptureDevice *device in devices) {

			s->devices[i].name = strdup([[device localizedDisplayName] UTF8String]);
			s->devices[i].uuid = strdup([[device uniqueID] UTF8String]);
//			NSLog(@"Device %@ %@",[device localizedDisplayName],[device uniqueID]);
			i++;
		}
	}

	[pool drain];

	return 0;
}

int vx_source_qtkit_open(vx_source* s, const char* n,vx_device_capability* cap)
{
	int retCode = -1;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	VXvideoController* delegate = [[VXvideoController alloc] init:s];
	VX_QTKIT_CAST(s)->delegate = delegate;

	/* use an empty UUID to trigger default behavior */
	NSString* uuid = (n) ?  [NSString stringWithUTF8String:n] : @"";

	if ([delegate open:uuid]) {
		retCode = 0;
	} else {
		[delegate release];
	}

	[pool release];
	return retCode;
}

int vx_source_qtkit_update(vx_source* s,unsigned int runloop)
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	switch (runloop) {
	case VX_SOURCE_UPDATE_PEEK:
	{
		NSDate *loopUntil = [NSDate dateWithTimeIntervalSinceNow:0.005];
		[[NSRunLoop currentRunLoop] runMode: NSDefaultRunLoopMode
			beforeDate:loopUntil];
	}
	break;
	case VX_SOURCE_UPDATE_FULL:
		[[NSRunLoop currentRunLoop] runUntilDate:nil];
		break;
	default:
		break;
	}

	[pool release];

	return 0;
}

int vx_source_qtkit_close(vx_source* s)
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	[VX_QTKIT_CAST(s)->delegate release];

	[pool release];

	return 0;
}

int vx_source_qtkit_set_state(vx_source* s,int state)
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	switch (state) {
	case VX_SOURCE_STATE_RUNNING:
		[VX_QTKIT_CAST(s)->delegate start];
		break;
	case VX_SOURCE_STATE_STOP:
		[VX_QTKIT_CAST(s)->delegate stop];
		break;
	}
	[pool drain];
	return 0;
}

int vx_source_qtkit_get_state(vx_source* s,int* state)
{
	if (VX_QTKIT_CAST(s)->delegate) {
		if ([VX_QTKIT_CAST(s)->delegate isRunning]) {
			*state = VX_SOURCE_STATE_RUNNING;
		} else {
			*state = VX_SOURCE_STATE_STOP;
		}
	} else {
		*state = -1;
	}

//	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

void*
vx_source_qtkit_create()
{
	vx_source_qtkit* s = (vx_source_qtkit*) malloc(sizeof(struct vx_source_qtkit));

	VX_SOURCE_CAST(s)->enumerate = vx_source_qtkit_enumerate;
	VX_SOURCE_CAST(s)->open = vx_source_qtkit_open;
	VX_SOURCE_CAST(s)->close = vx_source_qtkit_close;
	VX_SOURCE_CAST(s)->set_state = vx_source_qtkit_set_state;
	VX_SOURCE_CAST(s)->get_state = vx_source_qtkit_get_state;
	VX_SOURCE_CAST(s)->update = vx_source_qtkit_update;

	return s;
}

#else
void*
vx_source_qtkit_create()
{
	return 0;
}

#endif
