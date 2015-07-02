#include "gpufunctionsource.h"

namespace interplot
{

GpuFunctionSource::GpuFunctionSource() :
	GpuFunctionSource( 0, 0, nullptr )
{
}

GpuFunctionSource::GpuFunctionSource(
		size_t domainDim, size_t imageDim, const char* source ) :
	m_uiDomainDim( domainDim ),
	m_uiImageDim( imageDim )
{
	if( source != nullptr )
	{
		m_strFunctionSource = source;
	}
}

//GpuFunctionSource::GpuFunctionSource( const GpuFunctionSource& ref ) :
//	GpuFunctionSource( ref.m_uiDomainDim,
//			           ref.m_uiImageDim,
//					   ref.m_strFunctionSource.c_str() )
//{
//}
//
//GpuFunctionSource::GpuFunctionSource( GpuFunctionSource&& ref ) :
//	m_uiDomainDim( ref.m_uiDomainDim ),
//	m_uiImageDim( ref.m_uiImageDim )
//{
//	m_strFunctionSource = std::move( ref.m_strFunctionSource );
//}
//
//GpuFunctionSource& GpuFunctionSource::operator=( const GpuFunctionSource& rhs )
//{
//	m_strFunctionSource = rhs.m_strFunctionSource;
//	m_uiDomainDim = rhs.m_uiDomainDim;
//	m_uiImageDim = rhs.m_uiImageDim;
//	return *this;
//}
//
//GpuFunctionSource& GpuFunctionSource::operator=( GpuFunctionSource&& rhs )
//{
//	m_strFunctionSource = std::move( rhs.m_strFunctionSource );
//	m_uiDomainDim = rhs.m_uiDomainDim;
//	m_uiImageDim = rhs.m_uiImageDim;
//	return *this;
//}

}
