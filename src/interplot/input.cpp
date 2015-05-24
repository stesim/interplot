#include "input.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>

namespace interplot
{

Input::Input()
	: m_pKeyStates( nullptr ),
	m_uiMouseButtonState( 0 ),
	m_vecMousePos( 0, 0 ),
	m_vecMouseRel( 0, 0 )
{
}

Input::~Input()
{
}

void Input::initialize()
{
	m_pKeyStates = SDL_GetKeyboardState( nullptr );
}

void Input::update()
{
	SDL_PumpEvents();

	if( isMouseRelative() )
	{
		m_uiMouseButtonState = SDL_GetRelativeMouseState(
				&m_vecMouseRel.x, &m_vecMouseRel.y );
	}
	else
	{
		glm::ivec2 newPos;
		m_uiMouseButtonState = SDL_GetMouseState( &newPos.x, &newPos.y );

		m_vecMouseRel = newPos - m_vecMousePos;
		m_vecMousePos = newPos;
	}
}

bool Input::isKeyPressed( ScanCode key ) const
{
	return m_pKeyStates[ key ];
}

const glm::ivec2& Input::getMousePosition() const
{
	return m_vecMousePos;
}

const glm::ivec2& Input::getRelativeMouseMovement() const
{
	return m_vecMouseRel;
}

bool Input::isMouseButtonPressed( MouseButton button ) const
{
	return ( m_uiMouseButtonState & SDL_BUTTON( button ) );
}

bool Input::isMouseRelative() const
{
	return ( SDL_GetRelativeMouseMode() == SDL_TRUE );
}

void Input::setMouseRelative( bool state )
{
	SDL_SetRelativeMouseMode( state ? SDL_TRUE : SDL_FALSE );
}

}
