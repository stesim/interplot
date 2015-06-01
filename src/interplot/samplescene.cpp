#include "samplescene.h"
#include "vertex.h"
#include "shaderprogram.h"
#include "globaltimes.h"
#include "camera.h"
#include "engine.h"
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>
#include <SDL2/SDL.h>

#include <type_traits>

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
}

SampleScene::~SampleScene()
{
	delete m_pActiveCamera;
}

void SampleScene::initialize()
{
	/*
	Vertex points[] = {
		// floor quad
		Vertex( -10.0f, -5.0f, -25.0f,   0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f,  0.0f ),
		Vertex( -10.0f, -5.0f,  25.0f,   0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f,  0.0f ),
		Vertex(  10.0f, -5.0f,  25.0f,   0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f,  0.0f ),
		Vertex(  10.0f, -5.0f,  25.0f,   0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f,  0.0f ),
		Vertex(  10.0f, -5.0f, -25.0f,   0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f,  0.0f ),
		Vertex( -10.0f, -5.0f, -25.0f,   0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f,  0.0f ),

		// wall triangles
		Vertex( -10.0f, -5.0f, -25.0f,   1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f,  0.0f ),
		Vertex( -10.0f,  5.0f,   0.0f,   1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f,  0.0f ),
		Vertex( -10.0f, -5.0f,  25.0f,   1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f,  0.0f ),

		Vertex( -10.0f, -5.0f,  25.0f,   1.0f, 0.0f, 0.0f, 1.0f,    0.0f, 0.0f, -1.0f ),
		Vertex(   0.0f,  5.0f,  25.0f,   1.0f, 0.0f, 0.0f, 1.0f,    0.0f, 0.0f, -1.0f ),
		Vertex(  10.0f, -5.0f,  25.0f,   1.0f, 0.0f, 0.0f, 1.0f,    0.0f, 0.0f, -1.0f ),

		Vertex(  10.0f, -5.0f,  25.0f,   1.0f, 0.0f, 0.0f, 1.0f,   -1.0f, 0.0f,  0.0f ),
		Vertex(  10.0f,  5.0f,   0.0f,   1.0f, 0.0f, 0.0f, 1.0f,   -1.0f, 0.0f,  0.0f ),
		Vertex(  10.0f, -5.0f, -25.0f,   1.0f, 0.0f, 0.0f, 1.0f,   -1.0f, 0.0f,  0.0f ),

		Vertex(  10.0f, -5.0f, -25.0f,   1.0f, 0.0f, 0.0f, 1.0f,    0.0f, 0.0f,  1.0f ),
		Vertex(   0.0f,  5.0f, -25.0f,   1.0f, 0.0f, 0.0f, 1.0f,    0.0f, 0.0f,  1.0f ),
		Vertex( -10.0f, -5.0f, -25.0f,   1.0f, 0.0f, 0.0f, 1.0f,    0.0f, 0.0f,  1.0f ),
	};

	m_uiNumVertices = sizeof( points ) / sizeof( Vertex );

	GLuint& vbo = m_glVbo;
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData(
			GL_ARRAY_BUFFER,
			sizeof( points ),
			points,
			GL_STATIC_DRAW );

	GLuint& vao = m_glVao;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );
	glEnableVertexAttribArray( 2 );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof( Vertex ),
			vertex_offset( Vertex, pos ) );
	glVertexAttribPointer( 1,
			4,
			GL_FLOAT,
			GL_FALSE,
			sizeof( Vertex ),
			vertex_offset( Vertex, color ) );
	glVertexAttribPointer( 2,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof( Vertex ),
			vertex_offset( Vertex, normal ) );
	*/

	m_VertexBuffer.resize( 4 );
	m_VertexBuffer.allocHostMemory();
	m_VertexBuffer[ 0 ] = LineVertex( 0.0f, 0.0f );
	m_VertexBuffer[ 1 ] = LineVertex( 0.0f, 1.0f );
	m_VertexBuffer[ 2 ] = LineVertex( 1.0f, 1.0f );
	m_VertexBuffer[ 3 ] = LineVertex( 1.0f, 0.0f );
	m_VertexBuffer.copyToDevice();

	/*
	LineVertex points[] = {
		// floor quad
		LineVertex(  0.0f,  0.0f ),
		LineVertex(  0.0f,  1.0f ),
		LineVertex(  1.0f,  1.0f ),
		//LineVertex(  1.0f,  1.0f ),
		LineVertex(  1.0f,  0.0f ),
		//LineVertex(  0.0f,  0.0f ),
	};

	m_uiNumVertices = sizeof( points ) / sizeof( LineVertex );

	GLuint& vbo = m_glVbo;
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData(
			GL_ARRAY_BUFFER,
			sizeof( points ),
			points,
			GL_STATIC_DRAW );

	GLuint& vao = m_glVao;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glVertexAttribPointer(
			0,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof( LineVertex ),
			vertex_offset( LineVertex, pos ) );
	*/

	m_pShaderProgram = engine.shaders.getProgram( "line" );

	//m_Line.initialize();

	m_pActiveCamera->setPosition( glm::vec3( 5.0f, 5.0f, 20.0f ) );
}

void SampleScene::update()
{
	Camera& cam = *m_pActiveCamera;
	ftime deltaTime = engine.time.realDelta;

#define IS_PRESSED(bind) ( engine.input.isKeyPressed( bind( Bindings::bind ) ) )

	static const float mouseSens = 0.5f;

	if( is_pressed( Bindings::ResetCamera ) )
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

	if( is_pressed( Bindings::MoveForward ) )
	{
		cam.translate( deltaTime * keyboardMoveSpeed * cam.getForwardVector() );
	}
	if( is_pressed( Bindings::MoveBackward ) )
	{
		cam.translate( -deltaTime * keyboardMoveSpeed * cam.getForwardVector() );
	}
	if( is_pressed( Bindings::MoveLeft ) )
	{
		cam.translate( -deltaTime * keyboardMoveSpeed * cam.getRightVector() );
	}
	if( is_pressed( Bindings::MoveRight ) )
	{
		cam.translate( deltaTime * keyboardMoveSpeed * cam.getRightVector() );
	}
	if( is_pressed( Bindings::MoveDown ) )
	{
		cam.translate( -deltaTime * keyboardMoveSpeed * cam.getUpVector() );
	}
	if( is_pressed( Bindings::MoveUp ) )
	{
		cam.translate( deltaTime * keyboardMoveSpeed * cam.getUpVector() );
	}

	static const float keyboardTurnSpeed = 1.0f;

	if( is_pressed( Bindings::TurnUp ) )
	{
		cam.rotate( glm::angleAxis(
					keyboardTurnSpeed * deltaTime,
					cam.getRightVector() ) );
	}
	if( is_pressed( Bindings::TurnDown ) )
	{
		cam.rotate( glm::angleAxis(
					-keyboardTurnSpeed * deltaTime,
					cam.getRightVector() ) );
	}
	if( is_pressed( Bindings::TurnRight ) )
	{
		cam.rotate( glm::angleAxis(
					-keyboardTurnSpeed * deltaTime,
					cam.getUpVector() ) );
	}
	if( is_pressed( Bindings::TurnLeft ) )
	{
		cam.rotate( glm::angleAxis(
					keyboardTurnSpeed * deltaTime,
					cam.getUpVector() ) );
	}
	if( is_pressed( Bindings::RollCW ) )
	{
		cam.rotate( glm::angleAxis(
					keyboardTurnSpeed * deltaTime,
					cam.getForwardVector() ) );
	}
	if( is_pressed( Bindings::RollCCW ) )
	{
		cam.rotate( glm::angleAxis(
					-keyboardTurnSpeed * deltaTime,
					cam.getForwardVector() ) );
	}
	if( is_pressed( Bindings::RenderWireframe ) )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	if( is_pressed( Bindings::RenderFill ) )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
	if( is_pressed( Bindings::StopTime ) )
	{
		engine.time.scale = 0.0f;
	}
	else if( is_pressed( Bindings::FastForward ) )
	{
		engine.time.scale = 3.0f;
	}
	else
	{
		engine.time.scale = 1.0f;
	}

#undef IS_PRESSED
}

void SampleScene::render()
{
	if( m_pActiveCamera == nullptr )
	{
		return;
	}

	m_pActiveCamera->updateMatrices();

	m_pShaderProgram->use();

	m_pShaderProgram->setUniform(
			ShaderProgram::Uniform::ViewMatrix,
			m_pActiveCamera->getViewMatrix() );
	m_pShaderProgram->setUniform(
			ShaderProgram::Uniform::ViewNormalMatrix,
			m_pActiveCamera->getNormalMatrix() );
	m_pShaderProgram->setUniform(
			ShaderProgram::Uniform::ViewProjectionMatrix,
			m_pActiveCamera->getViewProjectionMatrix() );
	m_pShaderProgram->setUniform(
			ShaderProgram::Uniform::Time,
			engine.time.total );

	m_pShaderProgram->setUniform(
			ShaderProgram::Uniform::ModelMatrix,
			glm::scale( glm::mat4( 1.0f ), glm::vec3( 1.0f ) ) );
	m_pShaderProgram->setUniform(
			ShaderProgram::Uniform::ModelNormalMatrix,
			glm::mat3( 1.0f ) );
	//glBindVertexArray( m_glVao );
	engine.setVertexLayout<decltype( m_VertexBuffer )::VertexType>();
	m_VertexBuffer.bind();
	//glDrawArrays( GL_PATCHES, 0, m_uiNumVertices );
	glDrawArrays( GL_PATCHES, 0, m_VertexBuffer.size() * sizeof( decltype( m_VertexBuffer )::VertexType ) );

	//m_Line.render();
}

bool SampleScene::is_pressed( Bindings binding )
{
	return engine.input.isKeyPressed( bind( binding ) );
}

}
