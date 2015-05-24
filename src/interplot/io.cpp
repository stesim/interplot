#include "io.h"

namespace interplot
{

namespace io
{

char* readFile( const char* filename )
{
	std::ifstream file( filename );

	file.seekg( 0, std::ios::end );
	std::size_t length = file.tellg();
	file.seekg( 0, std::ios::beg );

	char* content = new char[ length + 1 ];
	file.read( content, length );
	content[ length ] = '\0';

	return content;
}

}

}
