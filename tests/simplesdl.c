/* include standard libs */
#include <stdlib.h>
#include <stdio.h>

/* include needed libs */
#include <SDL.h>

#include <vx/vx.h>


SDL_Surface *screen;
vx_source *source;
vx_sink *sink;
vx_device_description *devices;
int deviceCount;


void vxCaptureCallback(vx_source* source, vx_sink* sink, const vx_frame* frame, void *userdata)
{

	SDL_Surface* videoImage =
			SDL_CreateRGBSurfaceFrom(frame->data,
									 frame->width, frame->height,
									 24,frame->stride,
									 0,0,0,0);
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.w = videoImage->w;
	dest.h = videoImage->h;

	SDL_FillRect(screen, 0, SDL_MapRGBA(screen->format, 0, 0, 0, 0));

	// blit the image onto the screen
	if (0 == SDL_BlitSurface(videoImage, 0, screen, 0)) {
		SDL_Flip(screen);
	} else {
		fprintf(stderr,"Can't blit!\n");
	}

	SDL_FreeSurface(videoImage);

}


void sdlInit()
{
	/* initialize the SDL video system */
	if( SDL_Init(SDL_INIT_EVERYTHING) == -1 )
	{
		/* an error occurred */
		fprintf(stderr, "Cannot set video mode!");
		exit(1);
	}

	atexit(SDL_Quit);

	/* create a window */
	if((screen = SDL_SetVideoMode(640, 480, 0,  SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
	{
		/* an error occurred */
		fprintf(stderr, "Cannot open a window!");
		exit(1);
	}

	/* set window and icon title */
	SDL_WM_SetCaption("libVX SDL", "libVX SDL");

}

void vxInit()
{

	int i;

	source = vx_source_create(0);

	vx_source_enumerate(source,&devices,&deviceCount);

	if (deviceCount == 0)
		exit(-1);

	for (i = 0; i < deviceCount;++i) {
		fprintf(stdout,"Name: %s - UUID:%s\n",devices[i].name,devices[i].uuid);
	}

	sink = vx_sink_create("context",VX_SINK_TYPE_DIRECT);

	vx_sink_set_frame_callback(sink,&vxCaptureCallback,0);

	if (vx_source_open(source,devices[0].uuid) == 0)
	{
		vx_source_add_sink(source,sink);

		vx_source_set_state(source,VX_SOURCE_STATE_RUNNING);

	}

}

/* the main function */
int main(int argc, char *argv[])
{

	sdlInit();
	vxInit();

	// Poll for events, and handle the ones we care about.
	SDL_Event event;

	while (1) {

		vx_source_update(source,VX_SOURCE_UPDATE_NONE);

//		sdlRender();

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
					break;
			case SDL_KEYUP:
				// If escape is pressed, return (and thus, quit)
				if (event.key.keysym.sym == SDLK_ESCAPE)
					return 0;
				break;
			case SDL_QUIT:
				return(0);
			}
		}
	}


	vx_sink_destroy(&sink);

	return 0;
}
