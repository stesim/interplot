#pragma once

#include <GL/glew.h>
#include "scene.h"
#include <cstdint>
#include "input.h"
#include "vertexbuffer.h"
#include "linevisual.h"

namespace interplot
{

class Shader;
class ShaderProgram;

class SampleScene : public Scene
{
private:
	enum class Bindings
	{
		ResetCamera = 0,
		MoveForward,
		MoveBackward,
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		TurnLeft,
		TurnRight,
		TurnUp,
		TurnDown,
		RollCW,
		RollCCW,
		RenderWireframe,
		RenderFill,
		FastForward,
		StopTime,
		COUNT
	};

public:
	SampleScene();
	virtual ~SampleScene();

	virtual void initialize() override;

	virtual void update() override;

	virtual void render() override;

private:
	inline uint8_t& bind( Bindings bind )
	{
		return m_KeyBindings[ static_cast<int>( bind ) ];
	}

	inline bool is_pressed( Bindings binding );

private:
	uint8_t m_KeyBindings[ static_cast<int>( Bindings::COUNT ) ];

	VertexBuffer<LineVertex> m_VertexBuffer;

	ShaderProgram* m_pShaderProgram;

	LineVisual m_Line;
};

}
