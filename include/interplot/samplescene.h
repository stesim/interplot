#pragma once

#include <GL/glew.h>
#include "scene.h"
#include <cstdint>
#include "input.h"
#include "vertexbuffer.h"
#include "linevisual.h"
#include "engine.h"
#include "uniformbuffer.h"
#include "surfacevisual.h"

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
		IncreasePoints,
		DecreasePoints,
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

	inline bool isBindDown( Bindings binding )
	{
		return engine.input.isKeyDown( bind( binding ) );
	}

	inline bool wasBindPressed( Bindings binding )
	{
		return engine.input.wasKeyPressed( bind( binding ) );
	}

private:
	uint8_t m_KeyBindings[ static_cast<int>( Bindings::COUNT ) ];

	UniformBuffer<mat4, mat3, mat4> m_CameraUniformBuffer;

	LineVisual m_Line;
	SurfaceVisual m_Surface;
};

}
