#include <GL/glew.h>
#include "interplot.h"
#include "window.h"
#include "glhelper.h"
#include "camera.h"
#include "input.h"
#include "engine.h"
#include <SDL2/SDL.h>
#include <cstdio>
#include <cmath>
#include "samplescene.h"
#include "globaltimes.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace interplot;

int main( int argc, char* argv[] )
{
	if( interplot::initialize() != Error::None )
	{
		printf( "Interplot intialization failed.\n" );
		return 1;
	}

	Window win;

	if( !win.isValid() )
	{
		printf( "Window creation failed.\n" );
		return 1;
	}

	GLint maxPatchVertices;
	glGetIntegerv( GL_MAX_PATCH_VERTICES, &maxPatchVertices );
	GLint maxTessLevel;
	glGetIntegerv( GL_MAX_TESS_GEN_LEVEL, &maxTessLevel );

	dbg_printf(
			"Renderer:               %s\n"
			"OpenGL version:         %s\n"
			"Max patch vertices:     %d\n"
			"Max tessellation level: %d\n",
			glGetString( GL_RENDERER ),
			glGetString( GL_VERSION ),
			maxPatchVertices,
			maxTessLevel );

	engine.initialize();

	SampleScene scene;
	scene.initialize();
	engine.setScene( &scene );

	glEnable( GL_DEPTH_TEST );
	glPatchParameteri( GL_PATCH_VERTICES, 4 );

	win.show();

	bool quit = false;
	SDL_Event event;

	while( !quit )
	{
		while( SDL_PollEvent( &event ) != 0 )
		{
			switch( event.type )
			{
			case SDL_QUIT:
			{
				quit = true;
				break;
			}
			case SDL_WINDOWEVENT:
			{
				switch( event.window.event )
				{
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				{
					int width  = event.window.data1;
					int height = event.window.data2;

					glViewport( 0, 0, width, height );

					// TODO: decent resolution control
					scene.getActiveCamera()->setResolution(
							glm::ivec2( width, height ) );
					break;
				}
				}
			}
			}

			if( quit )
			{
				break;
			}
		}

		engine.update();

		win.clear();
		engine.render();
		win.render();
	}

	win.destroy();

	interplot::finalize();

	return 0;
}
