#pragma once

#include <GL/glew.h>
#include "base.h"
#include "glsltypes.h"

namespace interplot
{

/*
 * Structures overview:
 * --------------------
 *
 *   VertexDescriptor:   vertex layout description, providing information on
 *                       number of used attributes and their types
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

struct VertexDescriptor
{
	static constexpr size_t MAX_ATTRIBUTES = 16;

	size_t   attributes;
	GLSLType type[ MAX_ATTRIBUTES ];
};

template<typename... T>
struct VertexLayout
{
	static constexpr VertexDescriptor descriptor =
		VertexDescriptor{ sizeof...( T ), { glsl_type_id<T>()... } };
};

template<typename... T>
constexpr VertexDescriptor VertexLayout<T...>::descriptor;

#define VERTEX_LAYOUT(...) \
public: \
	using layout = VertexLayout<__VA_ARGS__>

struct Vertex
{
VERTEX_LAYOUT( vec3, vec4, vec3 );

public:
	Vertex( float x, float y, float z,
			float r, float g, float b, float a,
			float nx, float ny, float nz ) :
		pos( x, y, z ), color( r, g, b, a ), normal( nx, ny, nz ) { }
	
public:
	vec3 pos;
	vec4 color;
	vec3 normal;
};

struct LineVertex
{
VERTEX_LAYOUT( vec2 );

public:
	LineVertex( float x, float y ) :
		pos( x, y ) { }

public:
	vec2 pos;
};

struct PositionVertex
{
VERTEX_LAYOUT( vec3 );

public:
	PositionVertex( float x, float y, float z ) :
		pos( x, y, z ) { }

public:
	vec3 pos;
};

}
