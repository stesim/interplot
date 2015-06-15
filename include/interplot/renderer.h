#pragma once

#include <GL/glew.h>
#include "base.h"
#include "vertexbuffer.h"
#include "uniformbuffer.h"
#include "shaderprogram.h"

namespace interplot
{

struct VertexDescriptor;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void initialize();
	
	void finalize();

	void update();

	template<typename T>
	inline void render( const VertexBuffer<T>& buffer )
	{
		render( buffer, 0, buffer.size() );
	}

	template<typename T>
	void render(
			const VertexBuffer<T>& buffer,
			std::size_t offset,
			std::size_t count,
			std::size_t instances = 1 )
	{
		assert( offset + count <= buffer.numVertices() && instances > 0 );

		setVertexLayout( &buffer.getVertexDescriptor() );
		
		for( std::size_t i = 0; i < m_pVertexDescriptor->attributes; ++i )
		{
			glVertexAttribBinding( i, 0 );
		}

		glBindVertexBuffer( 0, buffer.getID(), 0, sizeof( T ) );

		if( instances > 1 )
		{
			glDrawArraysInstanced( GL_PATCHES, offset, count, instances );
		}
		else
		{
			glDrawArrays( GL_PATCHES, offset, count );
		}
	}

	void setShader( ShaderProgram* shader );

	void setVertexLayout( const VertexDescriptor* desc );

	template<typename... T>
	inline void bindUniformBuffer(
			ShaderProgram::UniformBlock binding,
			UniformBuffer<T...>* buffer )
	{
		bindUniformBuffer( enum_cast( binding ), buffer );
	}

	template<typename... T>
	inline void bindUniformBuffer( GLuint binding, UniformBuffer<T...>* buffer )
	{
		glBindBufferBase(
				GL_UNIFORM_BUFFER,
				binding,
				buffer != nullptr ? buffer->getID() : 0 );
	}

private:
	ShaderProgram*           m_pShader;
	const VertexDescriptor*  m_pVertexDescriptor;

	GLuint m_glDefaultVao;
};

}
