#include "avfoundation_backend.h"

#include "_source.h"
#include "vx/frame.h"

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>
#import <AVFoundation/AVFoundation.h>

/* helpers */
#define VX_AVFOUNDATION_CAST(ptr) \
	((vx_source_avfoundation*)(ptr))

/* delegate for receiving the buffer samples - acts as a holder for the session */
@interface VXCaptureDelegate : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>
{
	vx_source* source;
	vx_frame frame;

	AVCaptureSession		*session;
	AVCaptureDeviceInput	*input;
	AVCaptureDevice			*device;
}

	@property (readwrite, retain) AVCaptureSession		*session;
	@property (readwrite, retain) AVCaptureDeviceInput	*input;
	@property (readwrite, retain) AVCaptureDevice		*device;
	@property (readwrite)	vx_source					*source;


- (void) setSource: (vx_source*) aSource;
- (void) start;
- (void) stop;


@end

@implementation VXCaptureDelegate;

	@synthesize session, input, device, source;

- (id) init: (vx_source*) aSource
{
	[super init];
	self.source = aSource;
	self.session = nil;
	return self;
}

- (void) dealloc
{
	[super dealloc];
}

- (void) start
{

	frame.frame = 0;
	[self.session startRunning];
}

- (void) stop
{
	[self.session stopRunning];
}

- (BOOL) isRunning
{
	return [self.session isRunning];
}

- (void)captureOutput: (AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
	if (self.source == 0) return;

	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);

	if (kCVReturnSuccess == CVPixelBufferLockBaseAddress(imageBuffer,0)) {
		frame.frame++;
		frame.data = CVPixelBufferGetBaseAddress(imageBuffer);
		frame.stride = CVPixelBufferGetBytesPerRow(imageBuffer);
		frame.width = CVPixelBufferGetWidth(imageBuffer);
		frame.height = CVPixelBufferGetHeight(imageBuffer);
		frame.dataSize = CVPixelBufferGetDataSize(imageBuffer);

		OSType pixFormat = CVPixelBufferGetPixelFormatType(imageBuffer);

		switch (pixFormat) {
		case kCVPixelFormatType_32ARGB:
			frame.colorModel = VX_E_COLOR_ARGB;
			frame.bpp = 32;
			break;
		case kCVPixelFormatType_32ABGR:
			frame.colorModel = VX_E_COLOR_ABGR;
			frame.bpp = 32;
			break;
		case kCVPixelFormatType_32RGBA:
			frame.colorModel = VX_E_COLOR_RGBA;
			break;
		case kCVPixelFormatType_32BGRA:
			frame.colorModel = VX_E_COLOR_BGRA;
			frame.bpp = 32;
			break;
		case kCVPixelFormatType_24RGB:
			frame.colorModel = VX_E_COLOR_RGB24;
			frame.bpp = 24;
			break;
		case kCVPixelFormatType_24BGR:
			frame.colorModel = VX_E_COLOR_BGR24;
			frame.bpp = 24;
			break;
		case kCVPixelFormatType_422YpCbCr8_yuvs:
			frame.colorModel = VX_E_COLOR_YU12;
			frame.bpp = 24;
			break;
		default:
			{
			   char formatStr[5];
			   int i;
			   for(i=0; i<4; i++ )
				   formatStr[i] = ((char*)&pixFormat)[3-i];
			   formatStr[4]=0;
				NSLog(@"Unhandled format '%s'",formatStr);
			}
		}

		_vx_send_frame(source,&frame);

		CVPixelBufferUnlockBaseAddress(imageBuffer, 0);

	}

	_vx_broadcast(source);

	[pool drain];
}


- (BOOL) open: (NSString*) configuration
{
	NSError *error;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	if ([configuration length] == 0) {
		device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
	} else {
		device = [AVCaptureDevice deviceWithUniqueID:configuration];
	}

	if (![device lockForConfiguration:&error]) {

		NSLog(@"Error: %@", [error localizedDescription]);

		[pool drain];
		return NO;
	}

	self.input = [AVCaptureDeviceInput deviceInputWithDevice:device error:&error];
	if (!self.input)
	{
		NSLog(@"Error: %@", [error localizedDescription]);
		return NO;
	}

	self.session = [[AVCaptureSession alloc] init];

	AVCaptureVideoDataOutput *captureOutput = [[AVCaptureVideoDataOutput alloc] init];
	dispatch_queue_t aQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
	[captureOutput setSampleBufferDelegate:self queue:aQueue];

	dispatch_release(aQueue);

	NSDictionary *settings = [NSDictionary dictionaryWithObject:
		 [NSNumber numberWithUnsignedInt:kCVPixelFormatType_24RGB]
		  forKey:(NSString *)kCVPixelBufferPixelFormatTypeKey ];

	[session beginConfiguration];

	[captureOutput setVideoSettings:settings];
	[session setSessionPreset:AVCaptureSessionPreset352x288];

	[session addInput:input];
	[session addOutput:captureOutput];

	[session commitConfiguration];

	[device unlockForConfiguration];


	frame.frame = 0;

	[pool drain];

	return YES;
}


@end

/* actual interface */

typedef struct vx_source_avfoundation {
	vx_source				super;
	VXCaptureDelegate		*delegate;
} vx_source_avfoundation;


int vx_source_avfoundation_enumerate(vx_source* s)
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	//AVMediaTypeVideo
	NSArray* avdevices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
	if ([avdevices count] > 0)
	{

		s->deviceCount = [avdevices count];
		s->devices = malloc(sizeof(vx_device_description) * s->deviceCount);

		int i = 0;
		for (AVCaptureDevice *avdevice in avdevices) {

			s->devices[i].name = strdup([[avdevice localizedName] UTF8String]);
			s->devices[i].uuid = strdup([[avdevice uniqueID] UTF8String]);
//			NSLog(@"Device %@ %@",[device localizedName],[device uniqueID]);
			i++;
		}
	}

	[pool drain];

	return 0;
}

int vx_source_avfoundation_open(vx_source* s, const char* n)
{
	int retCode = -1;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	VXCaptureDelegate* delegate = [[VXCaptureDelegate alloc] init:s];
	VX_AVFOUNDATION_CAST(s)->delegate = delegate;

	NSString* uuid = (n) ?  [NSString stringWithUTF8String:n] : @"";

	if ([delegate open:uuid]) {
		retCode = 0;
	} else {
		[delegate release];
	}

	[pool release];
	return retCode;
}

int vx_source_avfoundation_update(vx_source* s,unsigned int runloop)
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

int vx_source_avfoundation_close(vx_source* s)
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	[VX_AVFOUNDATION_CAST(s)->delegate release];

	[pool release];

	return 0;
}

int vx_source_avfoundation_set_state(vx_source* s,int state)
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	switch (state) {
	case VX_SOURCE_STATE_RUNNING:
		[VX_AVFOUNDATION_CAST(s)->delegate start];
		break;
	case VX_SOURCE_STATE_STOP:
		[VX_AVFOUNDATION_CAST(s)->delegate stop];
		break;
	}



	[pool drain];
	return 0;
}

int vx_source_avfoundation_get_state(vx_source* s,int* state)
{
	if (VX_AVFOUNDATION_CAST(s)->delegate) {
		if ([VX_AVFOUNDATION_CAST(s)->delegate isRunning]) {
			*state = VX_SOURCE_STATE_RUNNING;
		} else {
			*state = VX_SOURCE_STATE_STOP;
		}
	}

	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

void*
vx_source_avfoundation_create()
{
	vx_source_avfoundation* s = (vx_source_avfoundation*) malloc(sizeof(struct vx_source_avfoundation));

	VX_SOURCE_CAST(s)->enumerate = vx_source_avfoundation_enumerate;
	VX_SOURCE_CAST(s)->open = vx_source_avfoundation_open;
	VX_SOURCE_CAST(s)->close = vx_source_avfoundation_close;
	VX_SOURCE_CAST(s)->set_state = vx_source_avfoundation_set_state;
	VX_SOURCE_CAST(s)->get_state = vx_source_avfoundation_get_state;
	VX_SOURCE_CAST(s)->update = vx_source_avfoundation_update;

	return s;
}
