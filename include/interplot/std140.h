#pragma once

#include <GL/glew.h>
#include "base.h"
#include "linalg.h"

namespace interplot
{

/*
 * Shader (component) type identifiers.
 */
enum class ShaderType
{
	Float = GL_FLOAT,
	Int   = GL_INT,
	Byte  = GL_BYTE,
};

/*
 * Composite (vector/matrix) type description (number/type of components)
 */
struct CompositeTypeDesc
{
	ShaderType type;
	size_t     rows;
	size_t     cols;
};

template<typename T>
struct CompositeType;
#define COMPOSITE_TYPE_DESC( type, comp_type, num_rows, num_cols ) \
template<> \
struct CompositeType<type> \
{\
	static constexpr CompositeTypeDesc desc = CompositeTypeDesc{ comp_type, num_rows, num_cols }; \
}

COMPOSITE_TYPE_DESC( vec2, ShaderType::Float, 2, 1 );
COMPOSITE_TYPE_DESC( vec3, ShaderType::Float, 3, 1 );
COMPOSITE_TYPE_DESC( vec4, ShaderType::Float, 4, 1 );

COMPOSITE_TYPE_DESC( ivec2, ShaderType::Int, 2, 1 );
COMPOSITE_TYPE_DESC( ivec3, ShaderType::Int, 3, 1 );
COMPOSITE_TYPE_DESC( ivec4, ShaderType::Int, 4, 1 );

COMPOSITE_TYPE_DESC( mat3, ShaderType::Float, 3, 3 );
COMPOSITE_TYPE_DESC( mat4, ShaderType::Float, 4, 4 );

#undef COMPOSITE_TYPE_DESC


/*
 * Mapping between shader type identifiers and native types.
 */
template<ShaderType T>
struct ShaderToNativeType;
template<typename T>
struct NativeToShaderType;

#define MAP_SHADER_AND_NATIVE_TYPE( native, shader ) \
template<> \
struct ShaderToNativeType<ShaderType::shader> { typedef native type; }; \
template<> \
struct NativeToShaderType<native> \
{ \
	static constexpr ShaderType type = ShaderType::shader; \
}

MAP_SHADER_AND_NATIVE_TYPE( GLfloat, Float );
MAP_SHADER_AND_NATIVE_TYPE( GLint,   Int   );
MAP_SHADER_AND_NATIVE_TYPE( GLbyte,  Byte  );

#undef MAP_SHADER_AND_NATIVE_TYPE

/*
 * Shader type size and alignment.
 */
template<ShaderType T>
constexpr std::size_t sizeof_shader()
{
	return sizeof( typename ShaderToNativeType<T>::type );
}

inline std::size_t sizeof_shader( ShaderType type )
{
	switch( type )
	{
		case ShaderType::Float:
			return sizeof( ShaderToNativeType<ShaderType::Float>::type );
		case ShaderType::Int:
			return sizeof( ShaderToNativeType<ShaderType::Int>::type );
		case ShaderType::Byte:
			return sizeof( ShaderToNativeType<ShaderType::Byte>::type );
		default:
			assert( false );
			return 0;
	}
}

template<ShaderType T>
constexpr std::size_t alignof_shader()
{
	return alignof( typename ShaderToNativeType<T>::type );
}

inline std::size_t alignof_shader( ShaderType type )
{
	switch( type )
	{
		case ShaderType::Float:
			return alignof( ShaderToNativeType<ShaderType::Float>::type );
		case ShaderType::Int:
			return alignof( ShaderToNativeType<ShaderType::Int>::type );
		case ShaderType::Byte:
			return alignof( ShaderToNativeType<ShaderType::Byte>::type );
		default:
			assert( false );
			return 0;
	}
}

/*
 * std140 type sizes.
 */
template<typename T>
constexpr
typename std::enable_if<!std::is_scalar<T>::value,size_t>::type
sizeof_std140()
{
	using CT = CompositeType<T>;
	static_assert( CT::desc.rows <= 4,
			"Composite types may have no more than 4 rows." );

	return ( sizeof_shader<CT::desc.type>() * CT::desc.cols *
			( CT::desc.rows != 3 ? CT::desc.rows : 4 ) );
}

template<typename T>
constexpr
typename std::enable_if<std::is_scalar<T>::value,size_t>::type
sizeof_std140()
{
	return sizeof( T );
}

//#define SIZEOF_STD140( type, size ) \
//template<> \
//constexpr std::size_t sizeof_std140<type>() { return size; }
//
//#undef SIZEOF_STD140

template<typename T>
inline void copy_std140( void* dest, const T& src )
{
	*reinterpret_cast<T*>( dest ) = src;
}

template<>
inline void copy_std140<mat3>( void* dest, const mat3& src )
{
	vec4* ptr = reinterpret_cast<vec4*>( dest );

	ptr[ 0 ].x = src[ 0 ].x;
	ptr[ 0 ].y = src[ 0 ].y;
	ptr[ 0 ].z = src[ 0 ].z;

	ptr[ 1 ].x = src[ 1 ].x;
	ptr[ 1 ].y = src[ 1 ].y;
	ptr[ 1 ].z = src[ 1 ].z;

	ptr[ 2 ].x = src[ 2 ].x;
	ptr[ 2 ].y = src[ 2 ].y;
	ptr[ 2 ].z = src[ 2 ].z;
}

}
