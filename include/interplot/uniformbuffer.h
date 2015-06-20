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

namespace uniform_buffer
{

template<size_t Index, typename... T>
inline constexpr size_t member_offset()
{
	return utils::SumConstrained<Index, std140_sizeof<T>()...>::value;
}

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

	template<size_t Index>
	const ParamType<Index>& get() const
	{
		return *reinterpret_cast<ParamType<Index>*>(
				reinterpret_cast<char*>( data() ) +
				internal::uniform_buffer::member_offset<Index, T...>() );
	}

	template<size_t Index>
	void set( const ParamType<Index>& value )
	{
		std140_copy( reinterpret_cast<char*>( data() ) +
				internal::uniform_buffer::member_offset<Index, T...>(), value );
	}

	void copyToDevice()
	{
		BufferBase::copyToDevice();
	}

private:
	static constexpr size_t SIZE = utils::Sum<std140_sizeof<T>()...>::value;
};

}
