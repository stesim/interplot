#pragma once

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstddef>
#include <tuple>

namespace interplot
{

template<typename T>
struct VertexAttribProp;

#define VERTEX_ATTRIB_TYPE_TO_GL(type,num_comp,comp_type) \
template<> \
struct VertexAttribProp<type> \
{ \
	typedef comp_type component_type; \
	static constexpr GLint  components = num_comp; \
}

VERTEX_ATTRIB_TYPE_TO_GL( glm::vec2, 2, float );
VERTEX_ATTRIB_TYPE_TO_GL( glm::vec3, 3, float );
VERTEX_ATTRIB_TYPE_TO_GL( glm::vec4, 4, float );

namespace internal
{
	template<typename T>
	void setGlVertexAttribFormat( GLuint index, GLint size, GLuint offset );

	template<>
	inline void setGlVertexAttribFormat<float>(
			GLuint index, GLint size, GLuint offset )
	{
		glVertexAttribFormat( index, size, GL_FLOAT, GL_FALSE, offset );
	}

	template<typename T1>
	void setGlVertexAttributes( GLuint index, GLuint offset )
	{
		setGlVertexAttribFormat<typename VertexAttribProp<T1>::component_type>(
				index, VertexAttribProp<T1>::components, offset );
	}

	template<typename T1, typename... T,
		typename = typename std::enable_if<( sizeof...( T ) > 0)>::type>
	void setGlVertexAttributes( GLuint index, GLuint offset )
	{
		setGlVertexAttribFormat<typename VertexAttribProp<T1>::component_type>(
				index, VertexAttribProp<T1>::components, offset );

		setGlVertexAttributes<T...>( index + 1, offset + sizeof( T1 ) );
	}
};

template<typename... T>
struct VertexLayout
{
	static constexpr std::size_t NUM_ATTRIBUTES = sizeof...( T );

	static void activate()
	{
		internal::setGlVertexAttributes<T...>( 0, 0 );
	}
};

#define VERTEX_LAYOUT(...) \
public: \
	typedef VertexLayout<__VA_ARGS__> layout


#define vertex_offset(type,member) ((const void*)offsetof(type,member))

struct Vertex
{
VERTEX_LAYOUT( glm::vec3, glm::vec4, glm::vec3 );

public:
	Vertex( float x, float y, float z,
			float r, float g, float b, float a,
			float nx, float ny, float nz )
		: pos( x, y, z ), color( r, g, b, a ), normal( nx, ny, nz )
	{
	}
	
public:
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec3 normal;
};

struct LineVertex
{
VERTEX_LAYOUT( glm::vec2 );

public:
	LineVertex( float x, float y )
		: pos( x, y )
	{
	}

public:
	glm::vec2 pos;
};

struct PositionVertex
{
VERTEX_LAYOUT( glm::vec3 );

public:
	PositionVertex( float x, float y, float z )
		: pos( x, y, z )
	{
	}

public:
	glm::vec3 pos;
};

/*
template<std::size_t Index, typename T1, typename... T>
struct TypePackEntry
{
	typedef typename TypePackEntry<Index - 1, T...>::type type;
};

template<typename T1, typename... T>
struct TypePackEntry<0, T1, T...>
{
	typedef T1 type;
};

template<typename... T>
struct GenericVertex;

template<typename T1>
struct GenericVertex<T1>
{
public:
	T1                  m_CurValue;
};

template<typename T1, typename... T>
struct GenericVertex<T1, T...>
{
public:
	T1                  m_CurValue;
	GenericVertex<T...> m_RemValues;
};

template<std::size_t Index, typename T1, typename... T,
	typename = void, typename = typename std::enable_if<( Index == 0 )>::type>
constexpr typename TypePackEntry<Index, T1, T...>::type& vertex_entry( GenericVertex<T1, T...>& v )
{
	return v.m_CurValue;
}

template<std::size_t Index, typename T1, typename... T,
	typename = typename std::enable_if<( Index > 0 )>::type>
constexpr typename TypePackEntry<Index, T1, T...>::type& vertex_entry( GenericVertex<T1, T...>& v )
{
	return vertex_entry<Index-1, T...>( v.m_RemValues );
}
*/

}
