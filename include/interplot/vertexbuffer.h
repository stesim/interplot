#pragma once

#include "bufferbase.h"
#include "vertex.h"

namespace interplot
{

template<typename T>
class VertexBuffer : public BufferBase
{
public:
	typedef T VertexType;

public:
	VertexBuffer() :
		m_uiNumVertices( 0 )
	{
	}
	VertexBuffer( std::size_t size ) :
		m_uiNumVertices( 0 )
	{
		resize( size );
	}

	void allocHostMemory()
	{
		BufferBase::allocHostMemory();
	}

	void freeHostMemory()
	{
		freeHostMemory();
	}

	void copyToDevice()
	{
		BufferBase::copyToDevice();
	}

	void copyToDevice( std::size_t offset, std::size_t size )
	{
		BufferBase::copyToDevice( offset * sizeof( T ), size * sizeof( T ) );
	}

	inline std::size_t numVertices() const
	{
		return m_uiNumVertices;
	}

	void resize( std::size_t size )
	{
		m_uiNumVertices = size;

		BufferBase::resize( size * sizeof( T ) );
	}

	T& operator[]( std::size_t index )
	{
		return reinterpret_cast<T*>( BufferBase::data() )[ index ];
	}

	const T& operator[]( std::size_t index ) const
	{
		return reinterpret_cast<T*>( BufferBase::data() )[ index ];
	}

	static const VertexDescriptor& getVertexDescriptor()
	{
		return VertexType::layout::descriptor;
	}

private:
	size_t m_uiNumVertices;
};

}
