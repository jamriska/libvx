

/* include standard libs */
#include <stdlib.h>
#include <stdio.h>

/* include needed libs */
#include <SDL.h>

#include <vx/vx.h>


SDL_Surface *screen;
vx_source *source;
vx_sink *sink;


void vxCaptureCallback(vx_source* source, vx_sink* sink, const vx_frame* frame, void *userdata)
{
	unsigned int mask[4];
	mask[0]=mask[1]=mask[2]=mask[3]=0;

	switch (frame->colorModel) {
    case VX_E_COLOR_BGR24:
        mask[0]=0xff0000;
        mask[1]=0x00FF00;
        mask[2]=0x0000FF;
        mask[3]=0x0;
        break;
	case VX_E_COLOR_RGB24:
		mask[0]=0x0000ff;
		mask[1]=0x00ff00;
        mask[2]=0xff0000;
		mask[3]=0x000000;
		break;
	case VX_E_COLOR_ARGB:
		mask[0]=0x0000ff00;
		mask[1]=0x00ff0000;
		mask[2]=0xff000000,
		mask[3]=0x000000ff;
		break;
	default:
    {
        char fourCC[5]; fourCC[4] = '\0';
        VX_FOURCC_TO_CHAR(frame->colorModel,fourCC);
        fprintf(stderr,"Unhandled format %s\n",fourCC);
        return;
    }
	}


//    if (frame->data == 0) return;

	SDL_Surface* videoImage =
			SDL_CreateRGBSurfaceFrom(frame->data,
									 frame->width, frame->height,
									 frame->bpp,frame->stride,
									 mask[0],mask[1],mask[2],mask[3]);

//    if (0 == SDL_LockSurface(screen)) {

        SDL_Rect dest;
        dest.x = 0;
        dest.y = 0;
        dest.w = videoImage->w;
        dest.h = videoImage->h;

        SDL_FillRect(screen, 0, SDL_MapRGBA(screen->format, 0, 0, 0, 0));

        // blit the image onto the screen
        if (0 != SDL_BlitSurface(videoImage, 0, screen, 0))
            fprintf(stderr,"Can't blit!\n");

//        SDL_UnlockSurface(screen);

//    }


    SDL_Flip(screen);


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
    vx_device_description *devices = 0L;
    int deviceCount = 0L;

	int i;

    source = vx_source_create("null");

    if (source == 0) {
        fprintf(stderr,"Error loading backend!\n");
        exit(-1);
    }

	vx_source_enumerate(source,&devices,&deviceCount);

	if (deviceCount == 0)
		exit(-1);

	for (i = 0; i < deviceCount;++i) {
		fprintf(stdout,"Name: %s - UUID:%s\n",devices[i].name,devices[i].uuid);
	}

	sink = vx_sink_create("context",VX_SINK_TYPE_DIRECT);

	vx_sink_set_frame_callback(sink,&vxCaptureCallback,0);

    if (vx_source_open(source,devices[0].uuid,0) == 0)
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
				vx_source_set_state(source,VX_SOURCE_STATE_STOP);
				goto cleanup;
			}
		}
	}

cleanup:

	vx_sink_destroy(&sink);

	return 0;
}
