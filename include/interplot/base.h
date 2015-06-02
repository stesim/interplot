#pragma once

#include <cstdio>
#include <cassert>

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>

#include "enum.h"              // useful enum class functionality

namespace interplot
{

const glm::vec3 vec_x = glm::vec3( 1.0f, 0.0f, 0.0f );
const glm::vec3 vec_y = glm::vec3( 0.0f, 1.0f, 0.0f );
const glm::vec3 vec_z = glm::vec3( 0.0f, 0.0f, 1.0f );

constexpr float DEG_TO_RAD = 3.141592f / 180.0f;

constexpr float operator"" _deg( long double deg )
{
	return deg * DEG_TO_RAD;
}

constexpr float operator"" _deg( unsigned long long int deg )
{
	return deg * DEG_TO_RAD;
}

enum class Error
{
	None = 0,
	Generic = 1,
	InvalidParameter
};

template<typename... T>
inline void dbg_printf( const char* mask, T... args )
{
#ifdef DEBUG
	printf( "--------------------- DEBUG: ---------------------\n" );
	printf( mask, args... );
	printf( "--------------------------------------------------\n" );
#endif
}

}
