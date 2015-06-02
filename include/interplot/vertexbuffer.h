#pragma once

#include "bufferbase.h"
#include "vertex.h"

namespace interplot
{

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

	static const VertexDescriptor& getVertexDescriptor()
	{
		return VertexType::layout::descriptor;
	}

	GLuint getID() const
	{
		return m_BaseBuffer.getID();
	}

private:
	BufferBase m_BaseBuffer;
};

}
