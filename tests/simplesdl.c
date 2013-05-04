/* include standard libs */
#include <stdlib.h>
#include <stdio.h>

/* include needed libs */
#include <SDL.h>

#include <vx/vx.h>

/* the main function */
int main(int argc, char *argv[])
{
	/* declare screen */
	SDL_Surface *screen;

	/* initialize the SDL video system */
	if( SDL_Init(SDL_INIT_VIDEO) == -1 )
	{
		/* an error occurred */
		fprintf(stderr, "Cannot set video mode!");
		exit(1);
	}

	atexit(SDL_Quit);

	/* create a window */
	if( (screen = SDL_SetVideoMode(640, 480, 0, SDL_HWSURFACE)) == NULL )
	{
		/* an error occurred */
		fprintf(stderr, "Cannot open a window!");
		exit(1);
	}

	/* set window and icon title */
	SDL_WM_SetCaption("SDL Hello World", "SDL Hello World");

	/* wait a moment */
	SDL_Delay(3000);

	/* cleanup SDL */
	SDL_Quit();

	return 0;
}
