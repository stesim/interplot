#pragma once

#include <GL/glew.h>
#include "base.h"

namespace interplot
{

class BufferBase
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
	BufferBase();
	~BufferBase();

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

}
