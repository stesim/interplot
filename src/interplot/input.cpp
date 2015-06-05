#include "input.h"
#include <SDL2/SDL_events.h>
#include <algorithm>

namespace interplot
{

Input::Input() :
	m_pKeyStates( nullptr ),
	m_pOldKeyStates( nullptr ),
	m_uiKeyStateSize( 0 ),
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
	int stateSize;
	m_pKeyStateSource = SDL_GetKeyboardState( &stateSize );
	
	m_pKeyStates      = new ScanCode[ stateSize ];
	m_pOldKeyStates   = new ScanCode[ stateSize ];

	m_uiKeyStateSize  = stateSize;

	std::fill_n( m_pKeyStates,    stateSize, 0 );
	std::fill_n( m_pOldKeyStates, stateSize, 0 );
}

void Input::finalize()
{
	if( m_pKeyStates != nullptr )
	{
		delete[] m_pKeyStates;
		m_pKeyStates = nullptr;
	}
	if( m_pOldKeyStates != nullptr )
	{
		delete[] m_pOldKeyStates;
		m_pOldKeyStates = nullptr;
	}
}

void Input::update()
{
	SDL_PumpEvents();

	ScanCode* swapTemp = m_pOldKeyStates;
	m_pOldKeyStates    = m_pKeyStates;
	m_pKeyStates       = swapTemp;

	memcpy( m_pKeyStates, m_pKeyStateSource, m_uiKeyStateSize );

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

void Input::setMouseRelative( bool state )
{
	SDL_SetRelativeMouseMode( state ? SDL_TRUE : SDL_FALSE );
}

}
