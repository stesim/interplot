#pragma once

#include "bufferbase.h"
#include "templateutils.h"
#include "std140.h"
#include <type_traits>
#include <algorithm>

namespace interplot
{

namespace internal
{

template<size_t Index, typename... T>
constexpr size_t ub_member_offset()
{
	return utils::SumConstrained<Index, sizeof_std140<T>()...>::value;
}

}

template<typename... T>
class UniformBuffer : public BufferBase
{
private:
	template<size_t Index>
	using ParamType = typename utils::TypeAtPackIndex<Index, T...>::type;

public:
	UniformBuffer()
	{
		resize( SIZE );
		
		allocHostMemory();

		setAccessUsage( Access::Write, Usage::Dynamic );
	}
	~UniformBuffer()
	{
	}

//	template<size_t Index>
//	ParamType<Index>& member()
//	{
//		return *reinterpret_cast<ParamType<Index>*>(
//				reinterpret_cast<char*>( data() ) +
//				internal::ub_member_offset<Index, T...>() );
//	}

	template<size_t Index>
	const ParamType<Index>& get() const
	{
		return *reinterpret_cast<ParamType<Index>*>(
				reinterpret_cast<char*>( data() ) +
				internal::ub_member_offset<Index, T...>() );
	}

	template<size_t Index>
	void set( const ParamType<Index>& value )
	{
//		*reinterpret_cast<ParamType<Index>*>(
//				reinterpret_cast<char*>( data() ) +
//				internal::ub_member_offset<Index, T...>() ) = value;

		copy_std140( reinterpret_cast<char*>( data() )
				+ internal::ub_member_offset<Index, T...>(), value );
	}

	void copyToDevice()
	{
		BufferBase::copyToDevice();
	}

private:
	//static constexpr size_t ALIGNMENT = utils::Max<alignof( T )...>::value;
	static constexpr size_t SIZE = utils::Sum<sizeof_std140<T>()...>::value;
};

}
