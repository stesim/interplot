#pragma once

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstddef>
#include <tuple>
#include "base.h"

namespace interplot
{

/*
 * Structures overview:
 * --------------------
 *
 *   VertexAttribProb:   maps composite type to type and number of components
 *                       e.g. glm::vec3 -> 3 x float
 *                       (specialized through VERTEX_ATTRIB_TYPE_TO_GL)
 *
 *   VertexDescriptor:   vertex layout description, providing information on
 *                       number of used attributes, their OpenGL types and
 *                       number of components
 *
 *   ComponentType:      maps C++ type to respective OpenGL enum value
 *                       (specialized through TYPE_TO_COMPONENT_TYPE)
 *
 *   VertexLayout:       helper class for easier descriptor construction;
 *                       constructs descriptor from type parameter pack
 *
 * Using the structures:
 * ---------------------
 * 
 * Vertex structures must have a 'layout' typedef containing a VertexDescriptor
 * static member named 'descriptor'. This can be achieved by aliasing the
 * respective VertexLayout type to 'layout'
 *   
 *   public:
 *   using layout = VertexLayout<Tattr1, ..., TattrN>;
 *
 * or by using the VERTEX_LAYOUT macro
 *
 *   VERTEX_LAYOUT( Tattr1, ..., TattrN );
 *
 * inside the vertex declaration body. Alternatively, both 'layout' and
 * 'descriptor' can be set manually.
 */

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

struct VertexDescriptor
{
public:
	enum class ComponentType
	{
		Float = GL_FLOAT,
		Int = GL_INT,
		Byte = GL_BYTE,
	};

public:
	static constexpr std::size_t MAX_ATTRIBUTES = 16;

	std::size_t     attributes;
	ComponentType   type[ MAX_ATTRIBUTES ];
	std::size_t     components[ MAX_ATTRIBUTES ];
};

template<typename T>
struct ComponentType;

#define TYPE_TO_COMPONENT_TYPE(type,component_type) \
template<> \
struct ComponentType<type> \
{ \
	static constexpr VertexDescriptor::ComponentType value = \
		VertexDescriptor::ComponentType::component_type; \
}

TYPE_TO_COMPONENT_TYPE( GLfloat, Float );
TYPE_TO_COMPONENT_TYPE( GLbyte,  Byte  );
TYPE_TO_COMPONENT_TYPE( GLint,   Int   );

#undef TYPE_TO_COMPONENT_TYPE

template<typename... T>
struct VertexLayout
{
	static constexpr VertexDescriptor descriptor = VertexDescriptor{
		sizeof...( T ),
		{ ComponentType<typename VertexAttribProp<T>::component_type>::value... },
		{ VertexAttribProp<T>::components... } };
};

template<typename... T>
constexpr VertexDescriptor VertexLayout<T...>::descriptor;

#define VERTEX_LAYOUT(...) \
public: \
	using layout = VertexLayout<__VA_ARGS__>

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
