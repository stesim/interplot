#pragma once

#include "base.h"

namespace interplot
{

namespace utils
{

template<size_t... I>
struct Indices
{
};

template<size_t N, size_t... I>
struct make_indices : make_indices<N - 1, N - 1, I...>
{
};

template<size_t... I>
struct make_indices<0, I...> : Indices<I...>
{
};

template<typename T, size_t N>
struct Storage
{
	T values[ N ];

	constexpr const T& operator[]( size_t index ) const
	{
		return values[ index ];
	}

	T& operator[]( size_t index )
	{
		return values[ index ];
	}
};

template<typename... T>
struct TypePack
{
};

template<size_t Size, typename T1, typename... T>
struct TypePackSubsetFront
{
	static_assert( Size <= sizeof...( T ) + 1, "Invalid subset size." );

	typedef TypePackSubsetFront<Size, T...> type;
};

template<size_t Size, typename T1, typename... T>
struct TypePackSubsetFront<Size, TypePack<T1, T...>>
{
	static_assert( Size <= sizeof...( T ) + 1, "Invalid subset size." );

	typedef TypePackSubsetFront<Size, T...> type;
};

template<typename... T>
struct TypePackSubsetFront<sizeof...( T ), T...>
{
	typedef TypePack<T...> type;
};

template<size_t Index, typename T1, typename... T>
struct TypeAtPackIndex
{
	typedef typename TypeAtPackIndex<Index - 1, T...>::type type;
};

template<typename T1, typename... T>
struct TypeAtPackIndex<0, T1, T...>
{
	typedef T1 type;
};

template<size_t Arg1, size_t... Args>
struct Max
{
	static constexpr size_t value =
		( Arg1 > Max<Args...>::value ? Arg1 : Max<Args...>::value );
};

template<size_t Arg1>
struct Max<Arg1>
{
	static constexpr size_t value = Arg1;
};

template<size_t Arg1, size_t... Args>
struct Sum
{
	static constexpr size_t value = Arg1 + Sum<Args...>::value;
};

template<size_t Arg1>
struct Sum<Arg1>
{
	static constexpr size_t value = Arg1;
};

template<size_t Size, size_t Arg1, size_t... Args>
struct SumConstrained
{
	static_assert( Size <= sizeof...( Args ) + 1, "Invalid constraint size." );

	static constexpr size_t value =
		Arg1 + SumConstrained<Size - 1, Args...>::value;
};

template<size_t... Args>
struct SumConstrained<0, Args...>
{
	static constexpr size_t value = 0;
};

}

}
