#pragma once

#include <GL/glew.h>
#include "base.h"
#include "vertexbuffer.h"

namespace interplot
{

class ShaderProgram;

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
	void render( const VertexBuffer<T>& buffer )
	{
		render( buffer, 0, buffer.size() );
	}

	template<typename T>
	void render(
			const VertexBuffer<T>& buffer,
			std::size_t offset,
			std::size_t count )
	{
		assert( offset + count <= buffer.size() );

		setVertexLayout( &buffer.getVertexDescriptor() );
		
		for( std::size_t i = 0; i < m_pVertexDescriptor->attributes; ++i )
		{
			glVertexAttribBinding( i, 0 );
		}

		glBindVertexBuffer( 0, buffer.getID(), 0, sizeof( T ) );

		glDrawArrays( GL_PATCHES, offset, count );
	}

	void setShader( ShaderProgram* shader );

	void setVertexLayout( const VertexDescriptor* desc );

private:
	ShaderProgram*           m_pShader;
	const VertexDescriptor*  m_pVertexDescriptor;

	GLuint m_glDefaultVao;
};

}
