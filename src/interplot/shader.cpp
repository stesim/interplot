#include "shader.h"
#include "io.h"

namespace interplot
{

constexpr char Shader::NAME_TO_PATH_MASK[];

Shader::Shader()
	: m_Type( Type::Vertex ), // random default value
	m_glShader( 0 ),
	m_pName( "" ),
	m_bManaged( false )
{
}

Shader::Shader( Type type, const char* source )
	: m_Type( type ),
	m_glShader( 0 ),
	m_pName( "" ),
	m_bManaged( false )
{
	m_glShader = glCreateShader( static_cast<GLuint>( type ) );
	glShaderSource( m_glShader, 1, &source, nullptr );
}

Shader::~Shader()
{
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

Shader* Shader::fromSource( Type type, const char* source )
{
	if( source != nullptr )
	{
		return ( new Shader( type, source ) );
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
