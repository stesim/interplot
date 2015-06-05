#include "shader.h"
#include "io.h"
#include <cstring>

namespace interplot
{

constexpr char Shader::NAME_TO_PATH_MASK[];

Shader::Shader() :
	m_Type( Type::Vertex ), // random default value
	m_glShader( 0 ),
	m_pName( "" ),
	m_pSource( nullptr ),
	m_uiSourceLength( 0 ),
	m_bManaged( false )
{
}

Shader::Shader( Type type,
		        const char* const* sources,
				std::size_t numSources ) :
	m_Type( type ),
	m_glShader( 0 ),
	m_pName( "" ),
	m_pSource( nullptr ),
	m_uiSourceLength( 0 ),
	m_bManaged( false )
{
	m_glShader = glCreateShader( static_cast<GLuint>( type ) );

	glShaderSource( m_glShader, numSources, sources, nullptr );

	GLint sourceLength;
	glGetShaderiv( m_glShader, GL_SHADER_SOURCE_LENGTH, &sourceLength );

	m_pSource = new char[ sourceLength ];
	glGetShaderSource( m_glShader, sourceLength, nullptr, m_pSource );
}

Shader::~Shader()
{
	if( m_pSource != nullptr )
	{
		delete[] m_pSource;
	}
	if( m_glShader != 0 )
	{
		glDeleteShader( m_glShader );
	}
}

Shader* Shader::fromName( Type type, const char* name )
{
	static const int FILENAME_SIZE = 256;

	if( name != nullptr )
	{
		char filename[ FILENAME_SIZE ];
		sprintf( &filename[ 0 ], NAME_TO_PATH_MASK,
				name, typeToExtension( type ) );

		return fromFile( type, filename );
	}
	else
	{
		return nullptr;
	}
}

Shader* Shader::fromSources( Type type,
		                     const char* const* sources,
							 std::size_t numSources )
{
	if( sources != nullptr )
	{
		return ( new Shader( type, sources, numSources ) );
	}
	else
	{
		return nullptr;
	}
}

Shader* Shader::fromFile( Type type, const char* path )
{
	if( path != nullptr )
	{
		char* src = io::readFile( path );
		Shader* shader = fromSource( type, src );
		io::freeResult( src );

		return shader;
	}
	else
	{
		return nullptr;
	}
}

void Shader::destroy( Shader* shader )
{
	glDeleteShader( shader->m_glShader );
	delete shader;
}

bool Shader::compile()
{
	if( m_glShader != 0 )
	{
		glCompileShader( m_glShader );

#ifdef DEBUG
		return isCompiled();
#else
		return true;
#endif
	}
	return false;
}

void Shader::getCompilationLog(
		std::size_t maxLength, std::size_t* logLength, char* log ) const
{
	GLsizei outLength;
	glGetShaderInfoLog( m_glShader, maxLength, &outLength, &log[ 0 ] );

	if( logLength != nullptr )
	{
		*logLength = outLength;
	}
}

const char* Shader::typeToExtension( Type type )
{
	switch( type )
	{
		case Type::Vertex:
			return "vs";
		case Type::TessellationControl:
			return "tcs";
		case Type::TessellationEvaluation:
			return "tes";
		case Type::Geometry:
			return "gs";
		case Type::Fragment:
			return "fs";
	}
}

}
