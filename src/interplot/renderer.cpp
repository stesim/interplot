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
	using namespace internal::glsl_types;

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
		// no segfault for desc = nullptr since newNumAttr = 0
		for( GLuint i = 0; i < newNumAttr; ++i )
		{
			const TypeDesc& glslDesc = glsl_type_desc( desc->type[ i ] );
			switch( glslDesc.comp_id )
			{
				case GLSLType::Float:
					glVertexAttribFormat(
							i,
							glslDesc.rows * glslDesc.columns,
							GL_FLOAT,
							GL_FALSE,
							offset );
					break;
				case GLSLType::Int:
					glVertexAttribIFormat(
							i,
							glslDesc.rows,
							GL_INT,
							offset );
					break;
				default:
					assert( false );
					break;
			}
			offset += glslDesc.size;
		}

		m_pVertexDescriptor = desc;
	}
}

}
