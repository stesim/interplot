#include "samplescene.h"
#include "vertex.h"
#include "shaderprogram.h"
#include "camera.h"
#include "engine.h"
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>
#include <SDL2/SDL.h>
#include <algorithm>

namespace interplot
{

SampleScene::SampleScene()
{
	m_pActiveCamera = new Camera();

	bind( Bindings::ResetCamera )     = SDL_SCANCODE_SPACE;
	bind( Bindings::MoveForward )     = SDL_SCANCODE_W;
	bind( Bindings::MoveBackward )    = SDL_SCANCODE_S;
	bind( Bindings::MoveLeft )        = SDL_SCANCODE_A;
	bind( Bindings::MoveRight )       = SDL_SCANCODE_D;
	bind( Bindings::MoveUp )          = SDL_SCANCODE_K;
	bind( Bindings::MoveDown )        = SDL_SCANCODE_J;
	bind( Bindings::TurnLeft )        = SDL_SCANCODE_LEFT;
	bind( Bindings::TurnRight )       = SDL_SCANCODE_RIGHT;
	bind( Bindings::TurnUp )          = SDL_SCANCODE_UP;
	bind( Bindings::TurnDown )        = SDL_SCANCODE_DOWN;
	bind( Bindings::RollCW )          = SDL_SCANCODE_E;
	bind( Bindings::RollCCW )         = SDL_SCANCODE_Q;
	bind( Bindings::RenderWireframe ) = SDL_SCANCODE_R;
	bind( Bindings::RenderFill )      = SDL_SCANCODE_F;
	bind( Bindings::FastForward )     = SDL_SCANCODE_LSHIFT;
	bind( Bindings::StopTime )        = SDL_SCANCODE_LCTRL;
	bind( Bindings::IncreasePoints )  = SDL_SCANCODE_PERIOD;
	bind( Bindings::DecreasePoints )  = SDL_SCANCODE_COMMA;
}

SampleScene::~SampleScene()
{
	delete m_pActiveCamera;
}

void SampleScene::initialize()
{
	m_pActiveCamera->setPosition( glm::vec3( 5.0f, 5.0f, 20.0f ) );

	engine.renderer.bindUniformBuffer(
			ShaderProgram::UniformBlock::Camera,
			&m_CameraUniformBuffer );

	m_Line.initialize();

//	m_Line.setFunction( GpuFunctionSource( 2, 3,
//				"return vec3( x, x * x * sin( 2.0 * t ), 0.0 );" ) );
//	m_Line.setParamStart( -1.0f );
//	m_Line.setParamEnd( 1.0f );

	m_Surface.initialize();
	m_Surface.setNumPoints( uvec2( 64, 4 ) );
}

void SampleScene::update()
{
	Camera& cam = *m_pActiveCamera;
	ftime deltaTime = engine.time.realDelta;

	static const float mouseSens = 0.5f;

	if( wasBindPressed( Bindings::ResetCamera ) )
	{
		cam.setPosition( glm::vec3( 0.0f, 0.0f, 0.0f ) );
		cam.setOrientation( glm::quat( 1.0f, 0.0f, 0.0f, 0.0f ) );
	}
	else if( engine.input.isMouseButtonPressed( SDL_BUTTON_RIGHT ) )
	{
		const glm::ivec2& mouseRel = engine.input.getRelativeMouseMovement();
		if( mouseRel.x != 0 )
		{
			cam.rotate( glm::angleAxis(
						-mouseSens * mouseRel.x * deltaTime,
						cam.getUpVector() ) );
		}
		if( mouseRel.y != 0 )
		{
			cam.rotate( glm::angleAxis(
						-mouseSens * mouseRel.y * deltaTime,
						cam.getRightVector() ) );
		}
	}

	static const float keyboardMoveSpeed = 5.0f;

	if( isBindDown( Bindings::MoveForward ) )
	{
		cam.translate( deltaTime * keyboardMoveSpeed * cam.getForwardVector() );
	}
	if( isBindDown( Bindings::MoveBackward ) )
	{
		cam.translate( -deltaTime * keyboardMoveSpeed * cam.getForwardVector() );
	}
	if( isBindDown( Bindings::MoveLeft ) )
	{
		cam.translate( -deltaTime * keyboardMoveSpeed * cam.getRightVector() );
	}
	if( isBindDown( Bindings::MoveRight ) )
	{
		cam.translate( deltaTime * keyboardMoveSpeed * cam.getRightVector() );
	}
	if( isBindDown( Bindings::MoveDown ) )
	{
		cam.translate( -deltaTime * keyboardMoveSpeed * cam.getUpVector() );
	}
	if( isBindDown( Bindings::MoveUp ) )
	{
		cam.translate( deltaTime * keyboardMoveSpeed * cam.getUpVector() );
	}

	static const float keyboardTurnSpeed = 1.0f;

	if( isBindDown( Bindings::TurnUp ) )
	{
		cam.rotate( glm::angleAxis(
					keyboardTurnSpeed * deltaTime,
					cam.getRightVector() ) );
	}
	if( isBindDown( Bindings::TurnDown ) )
	{
		cam.rotate( glm::angleAxis(
					-keyboardTurnSpeed * deltaTime,
					cam.getRightVector() ) );
	}
	if( isBindDown( Bindings::TurnRight ) )
	{
		cam.rotate( glm::angleAxis(
					-keyboardTurnSpeed * deltaTime,
					cam.getUpVector() ) );
	}
	if( isBindDown( Bindings::TurnLeft ) )
	{
		cam.rotate( glm::angleAxis(
					keyboardTurnSpeed * deltaTime,
					cam.getUpVector() ) );
	}
	if( isBindDown( Bindings::RollCW ) )
	{
		cam.rotate( glm::angleAxis(
					keyboardTurnSpeed * deltaTime,
					cam.getForwardVector() ) );
	}
	if( isBindDown( Bindings::RollCCW ) )
	{
		cam.rotate( glm::angleAxis(
					-keyboardTurnSpeed * deltaTime,
					cam.getForwardVector() ) );
	}
	if( wasBindPressed( Bindings::RenderWireframe ) )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	if( wasBindPressed( Bindings::RenderFill ) )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
	if( isBindDown( Bindings::StopTime ) )
	{
		engine.time.scale = 0.0f;
	}
	else if( isBindDown( Bindings::FastForward ) )
	{
		engine.time.scale = 3.0f;
	}
	else
	{
		engine.time.scale = 1.0f;
	}

	if( wasBindPressed( Bindings::IncreasePoints ) )
	{
		m_Line.setNumPoints( std::min( m_Line.getNumPoints() * 2, 2048ul ) );
		uvec2 numPoints = m_Surface.getNumPoints();
		if( std::max( numPoints.x, numPoints.y ) < 2048 )
		{
			m_Surface.setNumPoints( 2 * numPoints );
		}
	}
	if( wasBindPressed( Bindings::DecreasePoints ) )
	{
		m_Line.setNumPoints( std::max( m_Line.getNumPoints() / 2, 2ul ) );
		uvec2 numPoints = m_Surface.getNumPoints();
		if( std::min( numPoints.x, numPoints.y ) > 2 )
		{
			m_Surface.setNumPoints( numPoints / 2 );
		}
	}

	m_Line.update();

	m_Surface.update();
}

void SampleScene::render()
{
	if( m_pActiveCamera == nullptr )
	{
		return;
	}

	m_pActiveCamera->updateMatrices();

	m_CameraUniformBuffer.set<0>( m_pActiveCamera->getViewMatrix() );
	m_CameraUniformBuffer.set<1>( m_pActiveCamera->getNormalMatrix() );
	m_CameraUniformBuffer.set<2>( m_pActiveCamera->getViewProjectionMatrix() );
	m_CameraUniformBuffer.copyToDevice();


	ShaderProgram* shader = m_Line.getShader();
	engine.renderer.setShader( shader );

	shader->setUniform(
			ShaderProgram::Uniform::Time,
			engine.time.total );

	m_Line.render();


	shader = m_Surface.getShader();
	engine.renderer.setShader( shader );

	shader->setUniform(
			ShaderProgram::Uniform::Time,
			engine.time.total );

	m_Surface.render();
}

}
