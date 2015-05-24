#include "line.h"
#include "engine.h"
#include "vertex.h"

namespace interplot
{

Line::Line()
	: m_pShader( nullptr )
{
}

Line::~Line()
{
}

void Line::initialize()
{
	static const LineVertex points[] = {
		LineVertex( 0.0f, 0.0f ),
		LineVertex( 0.0f, 1.0f ),
		LineVertex( 1.0f, 1.0f ),
		LineVertex( 1.0f, 1.0f ),
		LineVertex( 1.0f, 0.0f ),
		LineVertex( 0.0f, 0.0f )
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
			sizeof( Vertex ),
			vertex_offset( Vertex, pos ) );

	m_pShader = engine.shaders.getProgram( "line" );
}

void Line::render()
{
	m_pShader->use();

	// HACK
	Camera* cam = engine.getScene()->getActiveCamera();
	m_pShader->setUniform(
			ShaderProgram::Uniform::ViewMatrix,
			cam->getViewMatrix() );
	m_pShader->setUniform(
			ShaderProgram::Uniform::ViewProjectionMatrix,
			cam->getViewProjectionMatrix() );
	m_pShader->setUniform(
			ShaderProgram::Uniform::ViewNormalMatrix,
			cam->getNormalMatrix() );

	m_pShader->setUniform( ShaderProgram::Uniform::ModelMatrix,
			glm::mat4( 1.0f ) );
	m_pShader->setUniform( ShaderProgram::Uniform::ModelNormalMatrix,
			glm::mat4( 1.0f ) );

	glBindVertexArray( m_glVao );
	glDrawArrays( GL_TRIANGLES, 0, m_uiNumVertices );
}

}
