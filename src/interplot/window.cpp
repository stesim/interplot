#include "window.h"

namespace interplot
{

void myDebugCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam )
{
	dbg_printf(
			"Source: %d\n"
			"Type:   %d\n"
			"ID:     %d\n"
			"Severity: %d\n"
			"Length:   %d\n"
			"\n%s\n",
			source,
			type,
			id,
			severity,
			length,
			message );
}

Window::Window()
{
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
	SDL_GL_SetAttribute(
			SDL_GL_CONTEXT_PROFILE_MASK,
			SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 8 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG );

	m_pWindow = SDL_CreateWindow(
			"",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			640,
			480,
			SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE
			);

	if( m_pWindow != nullptr )
	{
		m_GlContext = SDL_GL_CreateContext( m_pWindow );

		if( m_GlContext != nullptr )
		{
			SDL_GL_SetSwapInterval( 1 );

			if( glDebugMessageCallback != nullptr )
			{
				glEnable( GL_DEBUG_OUTPUT );
				glDebugMessageCallback( myDebugCallback, nullptr );
			}
			else if( glDebugMessageCallbackARB != nullptr )
			{
				glEnable( GL_DEBUG_OUTPUT );
				glDebugMessageCallbackARB( myDebugCallback, nullptr );
			}

			glClearColor( 0.3, 0.3, 0.3, 1.0 );
		}
		else
		{
			SDL_DestroyWindow( m_pWindow );
			m_pWindow = nullptr;
		}
	}
}

Window::~Window()
{
	destroy();
}

void Window::destroy()
{
	if( m_pWindow != nullptr )
	{
		SDL_DestroyWindow( m_pWindow );
		m_pWindow = nullptr;
	}

	m_GlContext = nullptr;
}

Error Window::show()
{
	if( m_pWindow != nullptr )
	{
		SDL_ShowWindow( m_pWindow );
		return Error::None;
	}
	else
	{
		return Error::InvalidParameter;
	}
}

void Window::hide()
{
	if( m_pWindow != nullptr )
	{
		SDL_HideWindow( m_pWindow );
	}
}

bool Window::isValid()
{
	return ( m_pWindow != nullptr );
}

void Window::clear()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//glClear( GL_DEPTH_BUFFER_BIT );
}

void Window::render()
{
	SDL_GL_SwapWindow( m_pWindow );
}

}
