#pragma once

#include "base.h"
#include "shader.h"
#include "shaderprogram.h"
#include <unordered_map>
#include <string>

namespace interplot
{

class ShaderManager
{
private:
	template<typename K, typename V>
	using map_type = std::unordered_map<K, V>;

public:
	ShaderManager();
	~ShaderManager();

	bool add( Shader* shader, const char* name );
	bool add( ShaderProgram* program, const char* name );

	template<Shader::Type T>
	Shader* getShader( const char* name )
	{
		auto& map = m_mapShaders[ shaderMapIndex( T ) ];
#ifdef DEBUG
		auto ret = map.find( name );
		return ( ret != map.end() ? ret->second : nullptr );
#else
		return map[ name ];
#endif
	}

	ShaderProgram* getProgram( const char* name )
	{
#ifdef DEBUG
		auto ret = m_mapPrograms.find( name );
		return ( ret != m_mapPrograms.end() ? ret->second : nullptr );
#else
		return m_mapPrograms[ name ];
#endif
	}

private:
	static int shaderMapIndex( Shader::Type type );

private:
	static constexpr int NUM_SHADER_MAPS = 5;

	map_type<std::string, Shader*>        m_mapShaders[ NUM_SHADER_MAPS ];
	map_type<std::string, ShaderProgram*> m_mapPrograms;
};

}
