#include "interplot.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <cstdio>
#include "window.h"

namespace interplot
{

Error initialize()
{
	if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
	{
		return Error::Generic;
	}

	Window win;

	glewExperimental = GL_TRUE;
	GLenum glerr = glewInit();

	if( glerr != GLEW_OK )
	{
		printf( "GLEW initialization failed (%d).\n", glerr );
		SDL_Quit();
		return Error::Generic;
	}

	return Error::None;
}

void finalize()
{
	SDL_Quit();
}

}
