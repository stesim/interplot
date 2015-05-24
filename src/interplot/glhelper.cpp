#include "glhelper.h"
#include <fstream>

namespace interplot
{

namespace glhelper
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

inline const char* shaderTypeToExtension( GLuint type )
{
	switch( type )
	{
		case GL_VERTEX_SHADER:
			return "vs";
		case GL_FRAGMENT_SHADER:
			return "fs";
		default:
			return nullptr;
	}
}

GLuint getShader( const char* name, GLuint type )
{
	char filename[ 256 ];
	sprintf( &filename[ 0 ], "resources/shaders/%s.%s.glsl",
			name, shaderTypeToExtension( type ) );

	char* src = readFile( filename );

	GLuint shader = glCreateShader( type );
	glShaderSource( shader, 1, &src, nullptr );
	glCompileShader( shader );

#ifdef DEBUG
	GLint compileStatus;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compileStatus );

	if( compileStatus != GL_TRUE )
	{
		GLint logLength;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );

		char* log = new char[ logLength ];

		GLsizei outLength;
		glGetShaderInfoLog( shader, logLength, &outLength, &log[ 0 ] );

		dbg_printf( "Shader compilation failed. Log:\n%s\n", log );

		delete[] log;
	}
#endif

	delete[] src;

	return shader;
}

GLuint assembleShaderProgram( const char* vsName, const char* fsName )
{
	GLuint vs = getShader( vsName, GL_VERTEX_SHADER );
	GLuint fs = getShader( fsName, GL_FRAGMENT_SHADER );

	return assembleShaderProgram( vs, fs );
}

GLuint assembleShaderProgram( GLuint vs, GLuint fs )
{
	GLuint program = glCreateProgram();
	glAttachShader( program, vs );
	glAttachShader( program, fs );
	glLinkProgram( program );

#ifdef DEBUG
	GLint linkStatus;
	glGetProgramiv( program, GL_LINK_STATUS, &linkStatus );

	if( linkStatus != GL_TRUE )
	{
		GLint logLength;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );

		char* log = new char[ logLength ];

		GLsizei outLength;
		glGetProgramInfoLog( program, logLength, &outLength, &log[ 0 ] );

		dbg_printf( "Shader linking failed. Log:\n%s\n", log );

		delete[] log;
	}
#endif

	return program;
}

}

}
