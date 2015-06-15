#include "shadermanager.h"
#include <cstring>

namespace interplot
{

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	for( auto iter : m_mapPrograms )
	{
		ShaderProgram::destroy( iter.second );
	}
	for( int i = 0; i < NUM_SHADER_MAPS; ++i )
	{
		auto& map = m_mapShaders[ i ];
		for( auto iter : map )
		{
			Shader::destroy( iter.second );
		}
	}
}

bool ShaderManager::add( Shader* shader, const char* name )
{
	if( shader == nullptr )	{ return false; }

	strncpy( shader->m_pName, name, Shader::NAME_SIZE - 1 );

	auto& map = m_mapShaders[ shaderMapIndex( shader->getType() ) ];

#ifdef DEBUG
	if( map.count( shader->getName() ) > 0 || shader->m_bManaged )
	{
		return false;
	}
#endif

	if( !shader->isCompiled() && !shader->compile() )
	{
		char log[ 2048 ];
		shader->getCompilationLog( sizeof( log ), nullptr, log );
		printf( "%s\n", log );

		return false;
	}

	map[ shader->getName() ] = shader;
	shader->m_bManaged = true;
	return true;
}

bool ShaderManager::add( ShaderProgram* program, const char* name )
{
	if( program == nullptr ) { return false; }

	strncpy( program->m_pName, name, ShaderProgram::NAME_SIZE - 1 );

#ifdef DEBUG
	if( m_mapPrograms.count( program->getName() ) > 0 || program->m_bManaged )
	{
		return false;
	}
#endif

	if( !program->isLinked() && !program->link() )
	{
		char log[ 2048 ];
		program->getLinkLog( sizeof( log ), nullptr, log );
		printf( "%s\n", log );

		return false;
	}

	m_mapPrograms[ program->getName() ] = program;
	program->m_bManaged = true;
	return true;
}

int ShaderManager::shaderMapIndex( Shader::Type type )
{
	switch( type )
	{
	case Shader::Type::Vertex:
		return 0;
	case Shader::Type::TessellationControl:
		return 1;
	case Shader::Type::TessellationEvaluation:
		return 2;
	case Shader::Type::Geometry:
		return 3;
	case Shader::Type::Fragment:
		return 4;
	default:
		assert( false );
		return -1;
	}
}

}
