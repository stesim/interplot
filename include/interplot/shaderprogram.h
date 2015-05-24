#pragma once

#include <GL/glew.h>
#include "base.h"
#include "shader.h"
#include <utility>
#include <glm/fwd.hpp>

#include <type_traits>

namespace interplot
{

class ShaderProgram
{
friend class ShaderManager;

public:
	enum class Uniform
	{
		ViewMatrix = 0,
		ViewNormalMatrix,
		ProjectionMatrix,
		ViewProjectionMatrix,
		ModelMatrix,
		ModelNormalMatrix,
		Time,
		Count
	};

public:
	//template<typename... T>
	//inline static ShaderProgram* assemble( T&... shaders )
	//{
	//	ShaderProgram* prog = new ShaderProgram();
	//	prog->assignShaders( std::forward<T>( shaders )... );
	//	return prog;
	//}

	inline static ShaderProgram* assemble( Shader& shader1, Shader& shader2 )
	{
		ShaderProgram* prog = new ShaderProgram();
		prog->addShader( shader1 );
		prog->addShader( shader2 );
		return prog;
	}

	static ShaderProgram* assemble(
			Shader* shader1,
			Shader* shader2,
			Shader* shader3 = nullptr,
			Shader* shader4 = nullptr,
			Shader* shader5 = nullptr,
			Shader* shader6 = nullptr );

	static void destroy( ShaderProgram* program );

	void addShader( Shader& shader );

	inline Shader* getShader( Shader::Type type )
	{
#define SP_TYPE_CASE(type,var) case Shader::Type::type: return var;
		switch( type )
		{
			SP_TYPE_CASE( Vertex,                 m_pVertexShader      )
			SP_TYPE_CASE( TessellationControl,    m_pTessControlShader )
			SP_TYPE_CASE( TessellationEvaluation, m_pTessEvalShader    )
			SP_TYPE_CASE( Geometry,               m_pGeometryShader    )
			SP_TYPE_CASE( Fragment,               m_pFragmentShader    )
		}
#undef SP_TYPE_CASE
	}

	inline const Shader* getShader( Shader::Type type ) const
	{
#define SP_TYPE_CASE(type,var) case Shader::Type::type: return var;
		switch( type )
		{
			SP_TYPE_CASE( Vertex,                 m_pVertexShader      )
			SP_TYPE_CASE( TessellationControl,    m_pTessControlShader )
			SP_TYPE_CASE( TessellationEvaluation, m_pTessEvalShader    )
			SP_TYPE_CASE( Geometry,               m_pGeometryShader    )
			SP_TYPE_CASE( Fragment,               m_pFragmentShader    )
		}
#undef SP_TYPE_CASE
	}

	bool               link();

	inline bool        isLinked() const
	{
		GLint linkStatus;
		glGetProgramiv( m_glProgram, GL_LINK_STATUS, &linkStatus );
		return ( linkStatus == GL_TRUE );
	}

	void               getLinkLog(
			std::size_t maxLength,
			std::size_t* logLength,
			char* log ) const;

	inline GLuint      getID() const { return m_glProgram; }

	inline const char* getName() const { return m_pName; }

	void setUniform( Uniform type, float val );
	void setUniform( Uniform type, const glm::vec2& vec );
	void setUniform( Uniform type, const glm::vec3& vec );
	void setUniform( Uniform type, const glm::vec4& vec );
	void setUniform( Uniform type, const glm::mat3& mat );
	void setUniform( Uniform type, const glm::mat4& mat );

	inline void use() const { glUseProgram( m_glProgram ); }

private:
	ShaderProgram();
	~ShaderProgram();

	inline void assignShaders( Shader& shader )
	{
		addShader( shader );
	}

	template<typename... T>
	inline void assignShaders( Shader& shader, T&... shaders )
	{
		addShader( shader );
		assignShaders( std::forward<T>( shaders )... );
	}

	void extractUniformLocations();

private:
	static constexpr int  NAME_SIZE           = 64;
	static constexpr int  NUM_SHADERS         = 5;

	GLuint  m_glProgram;
	char    m_pName[ NAME_SIZE ];
	bool    m_bManaged;

	Shader* m_pVertexShader;
	Shader* m_pTessControlShader;
	Shader* m_pTessEvalShader;
	Shader* m_pGeometryShader;
	Shader* m_pFragmentShader;

	int     m_UniformLocations[ enum_cast( Uniform::Count ) ];
};


}