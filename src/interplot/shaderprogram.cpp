#include "shaderprogram.h"
#include <algorithm>

namespace interplot
{

constexpr char ShaderProgram::UNIFORM_NAMES[][ NAME_SIZE ];

std::unordered_map<std::string, ShaderProgram::Uniform>
	ShaderProgram::s_mapNameToUniform;

ShaderProgram::StaticInitializer ShaderProgram::s_Initializer;

ShaderProgram::StaticInitializer::StaticInitializer()
{
	for( int i = 0; i < enum_cast( Uniform::Custom ); ++i )
	{
		s_mapNameToUniform[ UNIFORM_NAMES[ i ] ] = enum_cast<Uniform>( i );
	}
}

ShaderProgram::ShaderProgram()
	: m_glProgram( 0 ),
	m_pName( "" ),
	m_bManaged( false ),
	m_pVertexShader( nullptr ),
	m_pTessControlShader( nullptr ),
	m_pTessEvalShader( nullptr ),
	m_pGeometryShader( nullptr ),
	m_pFragmentShader( nullptr )
{
	std::fill_n(
			m_UniformLocations,
			sizeof( m_UniformLocations ) / sizeof( int ),
			-1 );
}

ShaderProgram::~ShaderProgram()
{
	if( m_glProgram != 0 )
	{
		glDeleteProgram( m_glProgram );
	}
}

ShaderProgram* ShaderProgram::assemble(
		Shader* shader1,
		Shader* shader2,
		Shader* shader3,
		Shader* shader4,
		Shader* shader5,
		Shader* shader6 )
{
	ShaderProgram* prog = new ShaderProgram();
#define SP_ADD_SHADER(shader) if( shader != nullptr ) {\
	prog->addShader( *shader );\
}

	SP_ADD_SHADER( shader1 );
	SP_ADD_SHADER( shader2 );
	SP_ADD_SHADER( shader3 );
	SP_ADD_SHADER( shader4 );
	SP_ADD_SHADER( shader5 );
	SP_ADD_SHADER( shader6 );

	prog->m_glProgram = glCreateProgram();

	return prog;
}

void ShaderProgram::destroy( ShaderProgram* program )
{
	delete program;
}

void ShaderProgram::addShader( Shader& shader )
{
#ifdef DEBUG
#define SP_TYPE_CASE(type,var) case Shader::Type::type:\
	assert( var == nullptr ); var = &shader; break;
#else
#define SP_TYPE_CASE(type,var) case Shader::Type::type: var = &shader; break;
#endif
	switch( shader.getType() )
	{
		SP_TYPE_CASE( Vertex,                 m_pVertexShader      )
		SP_TYPE_CASE( TessellationControl,    m_pTessControlShader )
		SP_TYPE_CASE( TessellationEvaluation, m_pTessEvalShader    )
		SP_TYPE_CASE( Geometry,               m_pGeometryShader    )
		SP_TYPE_CASE( Fragment,               m_pFragmentShader    )
	}
#undef SP_TYPE_CASE
}

bool ShaderProgram::link()
{
	if( m_glProgram == 0 ||
			m_pVertexShader == nullptr || m_pFragmentShader == nullptr )
	{
		return false;
	}

	if( m_pVertexShader != nullptr )
	{
		glAttachShader( m_glProgram, m_pVertexShader->getID() );
	}
	if( m_pTessControlShader != nullptr )
	{
		glAttachShader( m_glProgram, m_pTessControlShader->getID() );
	}
	if( m_pTessEvalShader != nullptr )
	{
		glAttachShader( m_glProgram, m_pTessEvalShader->getID() );
	}
	if( m_pGeometryShader != nullptr )
	{
		glAttachShader( m_glProgram, m_pGeometryShader->getID() );
	}
	if( m_pFragmentShader != nullptr )
	{
		glAttachShader( m_glProgram, m_pFragmentShader->getID() );
	}
	glLinkProgram( m_glProgram );

	if( isLinked() )
	{
		extractUniformLocations();
		return true;
	}
	else
	{
		return true;
	}
}

void ShaderProgram::getLinkLog(
		std::size_t maxLength, std::size_t* logLength, char* log ) const
{
	GLsizei outLength;
	glGetProgramInfoLog( m_glProgram, maxLength, &outLength, log );

	if( logLength != nullptr )
	{
		*logLength = outLength;
	}
}

ShaderProgram::Uniform ShaderProgram::nameToUniform( const char* name )
{
	auto iter = s_mapNameToUniform.find( name );
	if( iter != s_mapNameToUniform.end() )
	{
		return iter->second;
	}
	else
	{
		return Uniform::Custom;
	}
}

const char* ShaderProgram::uniformToName( Uniform uniform )
{
	auto index = enum_cast( uniform );
	if( index < enum_cast( Uniform::Custom ) )
	{
		return UNIFORM_NAMES[ index ];
	}
	else
	{
		return nullptr;
	}
}

void ShaderProgram::extractUniformLocations()
{
	printf( "Inspecting shader %d:\n"
			"---------------------\n",
			m_glProgram );

	char nameBuf[ NAME_SIZE ];

	GLint numUniforms;
	glGetProgramiv( m_glProgram, GL_ACTIVE_UNIFORMS,   &numUniforms   );

	for( GLint i = 0; i < numUniforms; ++i )
	{
		GLint  size;
		GLenum type;
		glGetActiveUniform(
				m_glProgram,
				i,
				sizeof( nameBuf ),
				nullptr,
				&size,
				&type,
				nameBuf );

		Uniform uniformType = nameToUniform( nameBuf );
		if( uniformType != Uniform::Custom )
		{
			m_UniformLocations[ enum_cast( uniformType ) ] = i;
		}
		else
		{
			dbg_printf(
					"Custom uniform discovered (shader : %d): %s\n",
					m_glProgram, nameBuf );
		}

		printf( "Uniform %d:\n"
				"  Name: %s\n"
				"  Type: %d\n"
				"  Size: %d\n",
				i, nameBuf, type, size );
	}

	GLint numUniformBlocks;
	glGetProgramInterfaceiv(
			m_glProgram,
			GL_UNIFORM_BLOCK,
			GL_ACTIVE_RESOURCES,
			&numUniformBlocks );

	for( GLint i = 0; i < numUniformBlocks; ++i )
	{
		glGetProgramResourceName(
				m_glProgram,
				GL_UNIFORM_BLOCK,
				i,
				NAME_SIZE,
				nullptr,
				nameBuf );
	}

	printf( "---------------------\n" );
}

}
