#pragma once

#include "base.h"
#include <SDL2/SDL_keyboard.h>
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

	void update();


	bool isKeyPressed( ScanCode key ) const;


	const glm::ivec2& getMousePosition() const;

	const glm::ivec2& getRelativeMouseMovement() const;

	bool              isMouseButtonPressed( MouseButton button ) const;

	bool              isMouseRelative() const;
	
	void              setMouseRelative( bool state );

private:
	const ScanCode*   m_pKeyStates;
	MouseButton       m_uiMouseButtonState;
	glm::ivec2        m_vecMousePos;
	glm::ivec2        m_vecMouseRel;
};

}
