#include "shaderprogram.h"
#include <algorithm>

namespace interplot
{

constexpr char ShaderProgram::UNIFORM_NAMES      [][ NAME_SIZE ];
constexpr char ShaderProgram::UNIFORM_BLOCK_NAMES[][ NAME_SIZE ];

std::unordered_map<std::string, ShaderProgram::Uniform>
	ShaderProgram::s_mapNameToUniform;
std::unordered_map<std::string, ShaderProgram::UniformBlock>
	ShaderProgram::s_mapNameToUniformBlock;

ShaderProgram::StaticInitializer ShaderProgram::s_Initializer;

ShaderProgram::StaticInitializer::StaticInitializer()
{
	for( int i = 0; i < enum_cast( Uniform::Count ); ++i )
	{
		s_mapNameToUniform[ UNIFORM_NAMES[ i ] ] = enum_cast<Uniform>( i );
	}

	for( int i = 0; i < enum_cast( UniformBlock::Count ); ++i )
	{
		s_mapNameToUniformBlock[ UNIFORM_BLOCK_NAMES[ i ] ] =
			enum_cast<UniformBlock>( i );
	}
}

ShaderProgram::ShaderProgram()
	: m_glProgram( 0 ),
	m_pName(),
	m_bManaged( false ),
	m_pVertexShader( nullptr ),
	m_pTessControlShader( nullptr ),
	m_pTessEvalShader( nullptr ),
	m_pGeometryShader( nullptr ),
	m_pFragmentShader( nullptr )
{
	std::fill_n( m_UniformLocations, enum_cast( Uniform::Count ), -1 );

	std::fill_n(
			m_UniformBlockLocations,
			enum_cast( UniformBlock::Count ),
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

void ShaderProgram::setCustomUniformBlockBinding(
		const char* name, GLuint binding )
{
	assert( binding >= NUM_RESERVED_BLOCKS );

	GLuint index = glGetUniformBlockIndex( m_glProgram, name );
	glUniformBlockBinding( m_glProgram, index, binding );
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
	if( index < enum_cast( Uniform::Count ) )
	{
		return UNIFORM_NAMES[ index ];
	}
	else
	{
		return nullptr;
	}
}

ShaderProgram::UniformBlock ShaderProgram::nameToUniformBlock(
		const char* name )
{
	auto iter = s_mapNameToUniformBlock.find( name );
	if( iter != s_mapNameToUniformBlock.end() )
	{
		return iter->second;
	}
	else
	{
		return UniformBlock::Custom;
	}
}

const char* ShaderProgram::uniformBlockToName( UniformBlock block )
{
	auto index = enum_cast( block );
	if( index < enum_cast( UniformBlock::Count ) )
	{
		return UNIFORM_BLOCK_NAMES[ index ];
	}
	else
	{
		return nullptr;
	}
}

void ShaderProgram::extractUniformLocations()
{
//	printf( "Inspecting shader %d:\n"
//			"---------------------\n",
//			m_glProgram );

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

//		printf( "Uniform %d: %s\n"
//				"  Type: %d\n"
//				"  Size: %d\n",
//				i, nameBuf, type, size );
	}

	static const GLenum activeVariablesProp  = GL_NUM_ACTIVE_VARIABLES;
	static const GLenum blockSizeProp        = GL_BUFFER_DATA_SIZE;
	static const GLenum activeIndicesProp    = GL_ACTIVE_VARIABLES;
	static const size_t MAX_ACTIVE_VARIABLES = 16; // TODO: remove?
	static const size_t NUM_OUTPUT_PROPS     = 3;
	static const GLenum outputProps[ NUM_OUTPUT_PROPS ] = {
		GL_TYPE,
		GL_OFFSET,
		GL_MATRIX_STRIDE,
	};

	// get number of uniform blocks
	GLint numUniformBlocks;
	glGetProgramInterfaceiv(
			m_glProgram,
			GL_UNIFORM_BLOCK,
			GL_ACTIVE_RESOURCES,
			&numUniformBlocks );

	for( GLint i = 0; i < numUniformBlocks; ++i )
	{
		// get block name
		glGetProgramResourceName(
				m_glProgram,
				GL_UNIFORM_BLOCK,
				i,
				NAME_SIZE,
				nullptr,
				nameBuf );

		UniformBlock blockType = nameToUniformBlock( nameBuf );
		if( blockType != UniformBlock::Custom )
		{
			m_UniformBlockLocations[ enum_cast( blockType ) ] = i;

			glUniformBlockBinding( m_glProgram, i, enum_cast( blockType ) );
		}

		// get number of active variables
		GLint activeVariables;
		glGetProgramResourceiv(
				m_glProgram,
				GL_UNIFORM_BLOCK,
				i,
				1,
				&activeVariablesProp,
				1,
				nullptr,
				&activeVariables );

		// get required buffer size
		GLint blockSize;
		glGetProgramResourceiv(
				m_glProgram,
				GL_UNIFORM_BLOCK,
				i,
				1,
				&blockSizeProp,
				1,
				nullptr,
				&blockSize );

//		printf( "Uniform Block %d:\n"
//				"  Name:      %s\n"
//				"  Size:      %d\n"
//				"  Variables: %d\n",
//				i, nameBuf, blockSize, activeVariables );

		GLint variableIndices[ MAX_ACTIVE_VARIABLES ];
		assert( activeVariables < MAX_ACTIVE_VARIABLES );

		// get indices of active variables
		glGetProgramResourceiv(
				m_glProgram,
				GL_UNIFORM_BLOCK,
				i,
				1,
				&activeIndicesProp,
				MAX_ACTIVE_VARIABLES,
				nullptr,
				variableIndices );

		for( GLint j = 0; j < activeVariables; ++j )
		{
			// get name of variable
			glGetProgramResourceName(
					m_glProgram,
					GL_UNIFORM,
					variableIndices[ j ],
					NAME_SIZE,
					nullptr,
					nameBuf );

			GLint outputPropValues[ NUM_OUTPUT_PROPS ];
			glGetProgramResourceiv(
					m_glProgram,
					GL_UNIFORM,
					variableIndices[ j ],
					NUM_OUTPUT_PROPS,
					outputProps,
					NUM_OUTPUT_PROPS,
					nullptr,
					outputPropValues );

//			printf( "    Variable %d: %s\n"
//					"      Type:     %d\n"
//					"      Offset:   %d\n"
//					"      M-Stride: %d\n",
//					j, nameBuf,
//					outputPropValues[ 0 ],
//					outputPropValues[ 1 ],
//					outputPropValues[ 2 ] );
		}
	}

//	printf( "---------------------\n" );
}

}
