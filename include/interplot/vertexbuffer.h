#pragma once

#include <GL/glew.h>
#include "vertex.h"

namespace interplot
{

class VertexBufferBase
{
public:
	enum class Access
	{
		Read,
		Write,
		Copy
	};

	enum class Usage
	{
		Static,
		Dynamic,
		Stream
	};

public:
	VertexBufferBase();
	~VertexBufferBase();

	void allocHostMemory();

	void freeHostMemory();

	void copyToDevice( std::size_t offset, std::size_t size );

	void* data()
	{
		return m_pData;
	}

	const void* data() const
	{
		return m_pData;
	}

	std::size_t size() const
	{
		return m_Size;
	}

	void resize( std::size_t size );

	GLuint getID() const
	{
		return m_glVbo;
	}

private:
	inline static GLenum toGlUsage( Access access, Usage usage );

	void resizeDevice();

private:
	std::size_t m_Size;
	std::size_t m_DeviceSize;
	char*       m_pData;
	GLuint      m_glVbo;
	Access      m_Access;
	Usage       m_Usage;
	GLenum      m_glUsage;
};

template<typename T>
class VertexBuffer
{
public:
	typedef T VertexType;

public:
	VertexBuffer()
	{
	}
	VertexBuffer( std::size_t size )
	{
		resize( size );
	}

	void allocHostMemory()
	{
		m_BaseBuffer.allocHostMemory();
	}

	void freeHostMemory()
	{
		m_BaseBuffer.allocHostMemory();
	}

	void copyToDevice()
	{
		m_BaseBuffer.copyToDevice( 0, m_BaseBuffer.size() );
	}

	void copyToDevice( std::size_t offset, std::size_t size )
	{
		m_BaseBuffer.copyToDevice( offset * sizeof( T ), size * sizeof( T ) );
	}

	std::size_t size() const
	{
		return m_BaseBuffer.size();
	}

	void resize( std::size_t size )
	{
		m_BaseBuffer.resize( size * sizeof( T ) );
	}

	T& operator[]( std::size_t index )
	{
		return reinterpret_cast<T*>( m_BaseBuffer.data() )[ index ];
	}

	const T& operator[]( std::size_t index ) const
	{
		return reinterpret_cast<T*>( m_BaseBuffer.data() )[ index ];
	}

	void bind() const
	{
		for( std::size_t i = 0; i < VertexType::layout::NUM_ATTRIBUTES; ++i )
		{
			glVertexAttribBinding( i, 0 );
		}
		glBindVertexBuffer( 0, m_BaseBuffer.getID(), 0, sizeof( VertexType ) );
	}

private:
	VertexBufferBase m_BaseBuffer;
};

}
