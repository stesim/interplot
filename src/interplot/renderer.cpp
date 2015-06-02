#include "renderer.h"
#include "shaderprogram.h"

namespace interplot
{

Renderer::Renderer() :
	m_pShader( nullptr ),
	m_pVertexDescriptor( nullptr ),
	m_glDefaultVao( 0 )
{
}

Renderer::~Renderer()
{
	finalize();
}

void Renderer::initialize()
{
	assert( m_glDefaultVao == 0 );

	glGenVertexArrays( 1, &m_glDefaultVao );
	glBindVertexArray( m_glDefaultVao );
}

void Renderer::finalize()
{
	glDeleteVertexArrays( 1, &m_glDefaultVao );
}

void Renderer::update()
{
}

void Renderer::setShader( ShaderProgram* shader )
{
	if( shader != m_pShader )
	{
		m_pShader = shader;
		glUseProgram( m_pShader != nullptr ? m_pShader->getID() : 0 );
	}
}

void Renderer::setVertexLayout( const VertexDescriptor* desc )
{
	if( desc != m_pVertexDescriptor )
	{
		GLuint oldNumAttr =
			( m_pVertexDescriptor != nullptr ?
			  m_pVertexDescriptor->attributes : 0 );

		GLuint newNumAttr = ( desc != nullptr ? desc->attributes : 0 );

		// disable no longer required attributes
		for( GLuint i = newNumAttr; i < oldNumAttr; ++i )
		{
			glDisableVertexAttribArray( i );
		}

		// enable newly required attributes
		for( GLuint i = oldNumAttr; i < newNumAttr; ++i )
		{
			glEnableVertexAttribArray( i );
		}

		GLuint offset = 0;
		for( GLuint i = 0; i < newNumAttr; ++i )
		{
			// no segfault for desc = nullptr since newNumAttr = 0
			switch( desc->type[ i ] )
			{
				case VertexDescriptor::ComponentType::Float:
					glVertexAttribFormat(
							i,
							desc->components[ i ],
							GL_FLOAT,
							GL_FALSE,
							offset );
					offset += desc->components[ i ] * sizeof( GLfloat );
					break;
				case VertexDescriptor::ComponentType::Int:
					glVertexAttribIFormat(
							i,
							desc->components[ i ],
							GL_INT,
							offset );
					offset += desc->components[ i ] * sizeof( GLint );
					break;
				case VertexDescriptor::ComponentType::Byte:
					glVertexAttribIFormat(
							i,
							desc->components[ i ],
							GL_BYTE,
							offset );
					offset += desc->components[ i ] * sizeof( GLbyte );
					break;
			}
		}

		m_pVertexDescriptor = desc;
	}
}

}
