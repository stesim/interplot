#pragma once

#include <GL/glew.h>
#include "base.h"
#include <SDL2/SDL.h>

namespace interplot
{

class Window
{
public:
	Window();
	~Window();

	void destroy();

	Error show();

	void hide();

	bool isValid();

	void clear();

	void render();

private:
	SDL_Window*     m_pWindow;
	SDL_GLContext   m_GlContext;
};

}
