#include "avfoundation_backend.h"

#include "_source.h"

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>
#import <AVFoundation/AVFoundation.h>


struct AVSessionControler;

typedef struct vx_source_avfoundation {
	vx_source super;

	AVCaptureSession *session;
	AVCaptureDeviceInput *input;
	AVCaptureDevice *device;

	struct AVSessionControler* controler;

} vx_source_avfoundation;

#define VX_AVFOUNDATION_CAST(ptr) \
	((vx_source_avfoundation*)(ptr))

#include <stdlib.h>
#include <stdio.h>


@interface AVSessionControler : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>
{
}


@end


@implementation AVSessionControler;

//  @synthesize session, camera, videoSize;

- (id) init
{
	[super init];
//	videoSize = CGSizeMake(-1.f, -1.f);
	return self;
}

  - (void)captureOutput:
  (AVCaptureOutput *)captureOutput
  didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
		 fromConnection:(AVCaptureConnection *)connection
  {


	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	printf("FRame");

	//	  if (parent == 0) return;


//	  CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
//	  CVPixelBufferLockBaseAddress(imageBuffer,0);

//	  uint8_t *baseAddress = (uint8_t *)CVPixelBufferGetBaseAddress(imageBuffer);
//	  size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer);
//	  size_t width = CVPixelBufferGetWidth(imageBuffer);
//	  size_t height = CVPixelBufferGetHeight(imageBuffer);

//	  IplImage local;
//	  cvInitImageHeader(&local, cvSize(width,height), IPL_DEPTH_8U, 4);
//	  cvSetData(&local, baseAddress, bytesPerRow);

//	  cvCvtColor(&local, parent->get_image(sstt_capture::IMG_BGR), CV_BGRA2BGR);

//	  parent->submit();

//	  CVPixelBufferUnlockBaseAddress(imageBuffer, 0);

	  [pool drain];
  }
@end

///

int vx_source_avfoundation_enumerate(vx_source* s,vx_source_description** e)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	//AVMediaTypeVideo
	NSArray* devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
	if ([devices count] > 0)
	{

		size_t size = [devices count] + 1;
		*e = malloc(sizeof(vx_source_description) * size);

		vx_source_description* arr = *e;

		int i = 0;
		for (AVCaptureDevice *device in devices) {

			arr[i].name = strdup([[device localizedName] UTF8String]);
			arr[i].uuid = strdup([[device uniqueID] UTF8String]);
//			NSLog(@"Device %@ %@",[device localizedName],[device uniqueID]);
			i++;
		}

		arr[i].name = 0;
		arr[i].uuid = 0;
	}

	[pool drain];

	return 0;
}

int vx_source_avfoundation_open(vx_source* s, const char* n)
{
	NSError *error;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	printf("%s %d\n",__FUNCTION__,__LINE__);

	AVCaptureDevice *device;

	if (n == 0) {
		device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
	} else {
		NSString* uuid = [NSString stringWithUTF8String:n];
		device = [AVCaptureDevice deviceWithUniqueID:uuid];
	}

	VX_AVFOUNDATION_CAST(s)->device = device;

	VX_AVFOUNDATION_CAST(s)->input =
		[AVCaptureDeviceInput deviceInputWithDevice:device error:&error];

	if (!VX_AVFOUNDATION_CAST(s)->input)
	{
		NSLog(@"Error: %@", error);
		return -1;
	}


	AVCaptureSession* session;
	session = [[AVCaptureSession alloc] init];

	[session beginConfiguration];

	AVCaptureVideoDataOutput *captureOutput = [[AVCaptureVideoDataOutput alloc] init];

	AVSessionControler* ctrl = [[AVSessionControler alloc] init];

	[captureOutput setSampleBufferDelegate:ctrl queue:dispatch_get_main_queue()];


	NSDictionary *settings =
	[NSDictionary dictionaryWithObject:
	 [NSNumber numberWithUnsignedInt:kCVPixelFormatType_32BGRA]
								forKey:(NSString *)kCVPixelBufferPixelFormatTypeKey
	 ];


	[captureOutput setVideoSettings:settings];

	[session addInput:VX_AVFOUNDATION_CAST(s)->input];
	[session addOutput:captureOutput];


	[session setSessionPreset:AVCaptureSessionPreset352x288];
	printf("%s %d opened '%s' 0x%x\n",__FUNCTION__,__LINE__,n,(long)device);

	[session commitConfiguration];

	[session startRunning];


	[pool drain];
	return 0;
}

int vx_source_avfoundation_close(vx_source* s)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

int vx_source_avfoundation_set_state(vx_source* s,int state)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}

int vx_source_avfoundation_get_state(vx_source* s,int* state)
{
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

	return s;
}
