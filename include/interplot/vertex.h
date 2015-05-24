#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstddef>
#include <tuple>

namespace interplot
{

#define vertex_offset(type,member) ((const void*)offsetof(type,member))

struct Vertex
{
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
public:
	PositionVertex( float x, float y, float z )
		: pos( x, y, z )
	{
	}

public:
	glm::vec3 pos;
};

}
