#pragma once

#include <GL/glew.h>
#include "base.h"
#include "templateutils.h"

namespace interplot
{

enum class GLSLType
{
	// float vectors / matrices
	Float = 0,
	vec2,
	vec3,
	vec4,
	mat2,
	mat2x3,
	mat2x4,
	mat3x2,
	mat3,
	mat3x4,
	mat4x2,
	mat4x3,
	mat4,

	// int vectors
	Int,
	ivec2,
	ivec3,
	ivec4,

	// uint vectors
	UInt,
	uvec2,
	uvec3,
	uvec4,

	// bool vectors
	Bool,
	bvec2,
	bvec3,
	bvec4,

	// number of types
	Count,
};

namespace internal
{

namespace glsl_types
{

/*
constexpr int glsl_type_hash( int type, int columns, int rows )
{
	return ( ( ( type & 0b111 ) << 6 ) &
			( ( columns & 0b11 ) << 3 ) &
			( ( rows & 0b11 ) << 0 ) );
}
*/

struct TypeDesc
{
	GLSLType id;
	GLSLType comp_id;
	GLenum   gl_type;
	size_t   rows;
	size_t   columns;
	size_t   size;
};

template<GLSLType T>
struct TypeIDMap;

template<GLenum T>
struct TypeGLtoTypeID;

template<typename T>
struct HostTypeToTypeID;

#define TYPE_DESC( _type_id, _host_type, _glsl_constant, _comp_id, _rows, _columns ) \
template<> \
struct TypeIDMap<_type_id> \
{ \
	typedef _host_type host_type; \
	static constexpr TypeDesc desc = { _type_id, _comp_id, _glsl_constant, _rows, _columns, sizeof( _host_type ) }; \
}; \
\
template<> \
struct TypeGLtoTypeID<_glsl_constant> \
{ \
	static constexpr GLSLType value = _type_id; \
}; \
\
template<> \
struct HostTypeToTypeID<_host_type> \
{ \
	static constexpr GLSLType value = _type_id; \
}

TYPE_DESC( GLSLType::Float,  GLfloat,   GL_FLOAT,             GLSLType::Float, 1, 1 );
TYPE_DESC( GLSLType::Int,    GLint,     GL_INT,               GLSLType::Int,   1, 1 );
TYPE_DESC( GLSLType::UInt,   GLuint,    GL_UNSIGNED_INT,      GLSLType::UInt,  1, 1 );
TYPE_DESC( GLSLType::Bool,   GLboolean, GL_BOOL,              GLSLType::Bool,  1, 1 );

TYPE_DESC( GLSLType::vec2,   vec2,      GL_FLOAT_VEC2,        GLSLType::Float, 2, 1 );
TYPE_DESC( GLSLType::vec3,   vec3,      GL_FLOAT_VEC3,        GLSLType::Float, 3, 1 );
TYPE_DESC( GLSLType::vec4,   vec4,      GL_FLOAT_VEC4,        GLSLType::Float, 4, 1 );

TYPE_DESC( GLSLType::ivec2,  ivec2,     GL_INT_VEC2,          GLSLType::Int,   2, 1 );
TYPE_DESC( GLSLType::ivec3,  ivec3,     GL_INT_VEC3,          GLSLType::Int,   3, 1 );
TYPE_DESC( GLSLType::ivec4,  ivec4,     GL_INT_VEC4,          GLSLType::Int,   4, 1 );

TYPE_DESC( GLSLType::uvec2,  uvec2,     GL_UNSIGNED_INT_VEC2, GLSLType::UInt,  2, 1 );
TYPE_DESC( GLSLType::uvec3,  uvec3,     GL_UNSIGNED_INT_VEC3, GLSLType::UInt,  3, 1 );
TYPE_DESC( GLSLType::uvec4,  uvec4,     GL_UNSIGNED_INT_VEC4, GLSLType::UInt,  4, 1 );

TYPE_DESC( GLSLType::bvec2,  bvec2,     GL_BOOL_VEC2,         GLSLType::Bool,  2, 1 );
TYPE_DESC( GLSLType::bvec3,  bvec3,     GL_BOOL_VEC3,         GLSLType::Bool,  3, 1 );
TYPE_DESC( GLSLType::bvec4,  bvec4,     GL_BOOL_VEC4,         GLSLType::Bool,  4, 1 );

TYPE_DESC( GLSLType::mat2,   mat2,      GL_FLOAT_MAT2,        GLSLType::Float, 2, 2 );
TYPE_DESC( GLSLType::mat3,   mat3,      GL_FLOAT_MAT3,        GLSLType::Float, 3, 3 );
TYPE_DESC( GLSLType::mat4,   mat4,      GL_FLOAT_MAT4,        GLSLType::Float, 4, 4 );

TYPE_DESC( GLSLType::mat2x3, mat2x3,    GL_FLOAT_MAT2x3,      GLSLType::Float, 2, 3 );
TYPE_DESC( GLSLType::mat2x4, mat2x4,    GL_FLOAT_MAT2x4,      GLSLType::Float, 2, 4 );
TYPE_DESC( GLSLType::mat3x2, mat3x2,    GL_FLOAT_MAT3x2,      GLSLType::Float, 3, 2 );
TYPE_DESC( GLSLType::mat3x4, mat3x4,    GL_FLOAT_MAT3x4,      GLSLType::Float, 3, 4 );
TYPE_DESC( GLSLType::mat4x2, mat4x2,    GL_FLOAT_MAT4x2,      GLSLType::Float, 4, 2 );
TYPE_DESC( GLSLType::mat4x3, mat4x3,    GL_FLOAT_MAT4x3,      GLSLType::Float, 4, 3 );

#undef GLSL_TYPE_DESC

template<size_t... I>
inline constexpr utils::Storage<TypeDesc, enum_cast( GLSLType::Count )>
construct_type_desc_storage( utils::Indices<I...> )
{
	return { { TypeIDMap<enum_cast<GLSLType>( I )>::desc... } };
}

inline constexpr utils::Storage<TypeDesc, enum_cast( GLSLType::Count )>
construct_type_desc_storage()
{
	return construct_type_desc_storage(
			utils::make_indices<enum_cast( GLSLType::Count )>() );
}

struct TypeStorage
{
	static constexpr utils::Storage<TypeDesc, enum_cast( GLSLType::Count )> desc
		= construct_type_desc_storage();
};

template<typename T>
inline constexpr GLenum glsl_gl_type()
{
	return HostTypeToTypeID<T>::desc.gl_type;
}

inline constexpr const TypeDesc& glsl_type_desc( GLSLType id )
{
	return TypeStorage::desc[ enum_cast( id ) ];
}

template<typename T>
inline constexpr const TypeDesc& glsl_type_desc()
{
	return glsl_type_desc( HostTypeToTypeID<T>::value );
}

}

}

template<GLenum T>
using glsl_host_type =
	typename internal::glsl_types::TypeGLtoTypeID<T>::host_type;

template<typename T>
inline constexpr GLSLType glsl_type_id()
{
	return internal::glsl_types::HostTypeToTypeID<T>::value;
}

inline constexpr size_t glsl_sizeof( GLSLType id )
{
	return internal::glsl_types::glsl_type_desc( id ).size;
}

}
