#include "shaderprogram.h"
#include <glm/ext.hpp>
#include <algorithm>

namespace interplot
{

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
	if( m_pVertexShader == nullptr || m_pFragmentShader == nullptr )
	{
		return false;
	}

	m_glProgram = glCreateProgram();
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

	if( !isLinked() )
	{
		return false;
	}

	extractUniformLocations();

	return true;
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

void ShaderProgram::extractUniformLocations()
{
	m_UniformLocations[ enum_cast( Uniform::ViewMatrix ) ] =
		glGetUniformLocation( m_glProgram, "mat_view" );
	m_UniformLocations[ enum_cast( Uniform::ViewNormalMatrix ) ] =
		glGetUniformLocation( m_glProgram, "mat_viewNormal" );
	m_UniformLocations[ enum_cast( Uniform::ProjectionMatrix ) ] =
		glGetUniformLocation( m_glProgram, "mat_projection" );
	m_UniformLocations[ enum_cast( Uniform::ViewProjectionMatrix ) ] =
		glGetUniformLocation( m_glProgram, "mat_viewProjection" );
	m_UniformLocations[ enum_cast( Uniform::ModelMatrix ) ] =
		glGetUniformLocation( m_glProgram, "mat_model" );
	m_UniformLocations[ enum_cast( Uniform::ModelNormalMatrix ) ] =
		glGetUniformLocation( m_glProgram, "mat_modelNormal" );
	m_UniformLocations[ enum_cast( Uniform::Time ) ] =
		glGetUniformLocation( m_glProgram, "f_time" );
}

void ShaderProgram::setUniform( Uniform type, float val )
{
	glUniform1f( m_UniformLocations[ enum_cast( type ) ], val );
}

void ShaderProgram::setUniform( Uniform type, const glm::vec2& vec )
{
	glUniform2fv(
			m_UniformLocations[ enum_cast( type ) ],
			1,
			glm::value_ptr( vec ) );
}

void ShaderProgram::setUniform( Uniform type, const glm::vec3& vec )
{
	glUniform3fv(
			m_UniformLocations[ enum_cast( type ) ],
			1,
			glm::value_ptr( vec ) );
}

void ShaderProgram::setUniform( Uniform type, const glm::vec4& vec )
{
	glUniform4fv(
			m_UniformLocations[ enum_cast( type ) ],
			1,
			glm::value_ptr( vec ) );
}

void ShaderProgram::setUniform( Uniform type, const glm::mat3& vec )
{
	glUniformMatrix3fv(
			m_UniformLocations[ enum_cast( type ) ],
			1,
			GL_FALSE,
			glm::value_ptr( vec ) );
}

void ShaderProgram::setUniform( Uniform type, const glm::mat4& vec )
{
	glUniformMatrix4fv(
			m_UniformLocations[ enum_cast( type ) ],
			1,
			GL_FALSE,
			glm::value_ptr( vec ) );
}

}
