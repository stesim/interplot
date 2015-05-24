#pragma once

#include <fstream>

namespace interplot
{

namespace io
{

char* readFile( const char* filename );

inline void freeResult( const char* string )
{
	delete[] string;
}

}

}
