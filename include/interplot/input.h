#pragma once

#include "base.h"
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <glm/vec2.hpp>

namespace interplot
{

typedef Uint8  ScanCode;

typedef Uint32 MouseButton;

class Input
{
public:
	Input();
	~Input();

	void initialize();

	void finalize();

	void update();


	inline bool isKeyDown( ScanCode key ) const
	{
		return m_pKeyStates[ key ];
	}

	inline bool isKeyUp( ScanCode key ) const
	{
		return !isKeyDown( key );
	}

	inline bool wasKeyPressed( ScanCode key ) const
	{
		return ( m_pKeyStates[ key ] && !m_pOldKeyStates[ key ] );
	}

	inline bool wasKeyReleased( ScanCode key ) const
	{
		return ( !m_pKeyStates[ key ] && m_pOldKeyStates[ key ] );
	}


	inline const glm::ivec2& getMousePosition() const
	{
		return m_vecMousePos;
	}

	inline const glm::ivec2& getRelativeMouseMovement() const
	{
		return m_vecMouseRel;
	}

	inline bool              isMouseButtonPressed( MouseButton button ) const
	{
		return ( m_uiMouseButtonState & SDL_BUTTON( button ) );
	}

	inline bool              isMouseRelative() const
	{
		return ( SDL_GetRelativeMouseMode() == SDL_TRUE );
	}
	
	void              setMouseRelative( bool state );

private:
	const ScanCode*   m_pKeyStateSource;
	ScanCode*         m_pKeyStates;
	ScanCode*         m_pOldKeyStates;
	std::size_t       m_uiKeyStateSize;
	MouseButton       m_uiMouseButtonState;
	glm::ivec2        m_vecMousePos;
	glm::ivec2        m_vecMouseRel;
};

}
