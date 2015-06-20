#pragma once

#include "glsltypes.h"

namespace interplot
{

namespace internal
{

namespace std140
{

template<typename T>
struct special_sizeof
{
	static constexpr bool value = ( glsl_types::glsl_type_desc<T>().rows == 3 );
};

template<typename T>
struct special_copy
{
	static constexpr bool value =
		( glsl_types::glsl_type_desc<T>().rows == 3 &&
		  glsl_types::glsl_type_desc<T>().columns > 1 );
};

}

}

template<typename T>
constexpr typename std::enable_if<!internal::std140::special_sizeof<T>::value, size_t>::type std140_sizeof()
{
	return glsl_sizeof( glsl_type_id<T>() );
}

template<typename T>
constexpr typename std::enable_if<internal::std140::special_sizeof<T>::value, size_t>::type std140_sizeof()
{
	using namespace internal::glsl_types;
	return ( 4 * glsl_type_desc<T>().columns * glsl_sizeof( glsl_type_desc<T>().comp_id ) );
}

template<typename T>
inline typename std::enable_if<!internal::std140::special_copy<T>::value>::type std140_copy( void* dest, const T& src )
{
	*reinterpret_cast<T*>( dest ) = src;
}

template<typename T>
inline typename std::enable_if<internal::std140::special_copy<T>::value>::type std140_copy( void* dest, const T& src )
{
	vec4* ptr = reinterpret_cast<vec4*>( dest );

	for( size_t i = 0; i < internal::glsl_types::glsl_type_desc<T>().columns; ++i )
	{
		ptr[ i ][ 0 ] = src[ i ][ 0 ];
		ptr[ i ][ 1 ] = src[ i ][ 1 ];
		ptr[ i ][ 2 ] = src[ i ][ 2 ];
	}
}

}
