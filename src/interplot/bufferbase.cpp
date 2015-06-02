#include "bufferbase.h"

namespace interplot
{

BufferBase::BufferBase() :
	m_Size( 0 ),
	m_DeviceSize( 0 ),
	m_pData( nullptr ),
	m_glVbo( 0 ),
	m_Access( Access::Write ),
	m_Usage( Usage::Static ),
	m_glUsage( toGlUsage( m_Access, m_Usage ) )
{
	glCreateBuffers( 1, &m_glVbo );
}

BufferBase::~BufferBase()
{
	if( m_pData != nullptr )
	{
		delete[] m_pData;
	}
}

void BufferBase::allocHostMemory()
{
	if( m_Size > 0 && m_pData == nullptr )
	{
		m_pData = new char[ m_Size ];
	}
}

void BufferBase::freeHostMemory()
{
	if( m_pData != nullptr )
	{
		delete[] m_pData;
		m_pData = nullptr;
	}
}

void BufferBase::copyToDevice( std::size_t offset, std::size_t size )
{
	if( m_DeviceSize < offset + size )
	{
		resizeDevice();
	}

	glNamedBufferSubData(
			m_glVbo, offset, size, &m_pData[ offset ] );
}

void BufferBase::resize( std::size_t size )
{
	if( m_Size != size )
	{
		if( m_pData != nullptr )
		{
			delete[] m_pData;
			m_pData = new char[ size ];
		}

		m_Size = size;
	}
}

GLenum BufferBase::toGlUsage( Access access, Usage usage )
{
	switch( usage )
	{
		case Usage::Static:
			switch( access )
			{
				case Access::Read:
					return GL_STATIC_READ;
				case Access::Write:
					return GL_STATIC_DRAW;
				case Access::Copy:
					return GL_STATIC_COPY;
			}
		case Usage::Dynamic:
			switch( access )
			{
				case Access::Read:
					return GL_DYNAMIC_READ;
				case Access::Write:
					return GL_DYNAMIC_DRAW;
				case Access::Copy:
					return GL_DYNAMIC_COPY;
			}
		case Usage::Stream:
			switch( access )
			{
				case Access::Read:
					return GL_STREAM_READ;
				case Access::Write:
					return GL_STREAM_DRAW;
				case Access::Copy:
					return GL_STREAM_COPY;
			}
	}
}

void BufferBase::resizeDevice()
{
	m_DeviceSize = m_Size;
	glNamedBufferData(
			m_glVbo, m_DeviceSize, nullptr, toGlUsage( m_Access, m_Usage ) );
}

}
