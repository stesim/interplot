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

	inline const void* data()  const { return m_pData; }

	inline std::size_t size()  const { return m_Size; }

	inline GLuint      getID() const { return m_glVbo; }

	void setAccessUsage( Access access, Usage usage );

	inline Access getAccess() const { return m_Access; }
	inline Usage  getUsage()  const { return m_Usage; }

protected:
	void resize( std::size_t size );

	void* data() { return m_pData; }

	void allocHostMemory();

	void freeHostMemory();

	void copyToDevice() { copyToDevice( 0, m_Size ); }

	void copyToDevice( std::size_t offset, std::size_t size );

private:
	inline static GLenum toGlUsage( Access access, Usage usage );

	void resizeDevice();

private:
	size_t  m_Size;
	size_t  m_DeviceSize;
	char*   m_pData;
	GLuint  m_glVbo;
	Access  m_Access;
	Usage   m_Usage;
	GLenum  m_glUsage;
};

}
