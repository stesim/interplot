#pragma once

#include "base.h"
#include <string>

namespace interplot
{

class GpuFunctionSource
{
public:
	GpuFunctionSource();
	GpuFunctionSource( size_t domainDim, size_t imageDim, const char* source );
//	GpuFunctionSource( const GpuFunctionSource& ref );
//	GpuFunctionSource( GpuFunctionSource&& ref );
//	~GpuFunctionSource();

//	GpuFunctionSource& operator=( const GpuFunctionSource& rhs );
//	GpuFunctionSource& operator=( GpuFunctionSource&& rhs );

	inline const char* getFunctionSource() const
	{
		return m_strFunctionSource.c_str();
	}
	inline void setFunctionSource( const char* source )
	{
		if( source != nullptr )
		{
			m_strFunctionSource = source;
		}
		else
		{
			m_strFunctionSource.clear();
		}
	}

	inline size_t getDomainDim() const
	{
		return m_uiDomainDim;
	}
	inline void setDomainDim( size_t dim )
	{
		m_uiDomainDim = dim;
	}

	inline size_t getImageDim() const
	{
		return m_uiImageDim;
	}
	inline void setImageDim( size_t dim )
	{
		m_uiImageDim = dim;
	}

protected:
	std::string m_strFunctionSource;

	size_t m_uiDomainDim;
	size_t m_uiImageDim;
};

}
