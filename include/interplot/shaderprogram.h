#pragma once

#include <GL/glew.h>
#include "base.h"
#include "shader.h"
#include <utility>
#include <glm/fwd.hpp>
#include <glm/ext.hpp>
#include <unordered_map>
#include "glutils.h"

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
		Custom,
		Count,
	};

	enum class UniformBlock
	{
		Camera,
		Model,
		Custom
	};

	typedef GLint CustomUniform;

private:
	class StaticInitializer
	{
	public:
		StaticInitializer();
	};

public:
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
		switch( type )
		{
			case Shader::Type::Vertex:
				return m_pVertexShader;
			case Shader::Type::TessellationControl:
				return m_pTessControlShader;
			case Shader::Type::TessellationEvaluation:
				return m_pTessEvalShader;
			case Shader::Type::Geometry:
				return m_pGeometryShader;
			case Shader::Type::Fragment:
				return m_pFragmentShader;
		}
	}

	inline const Shader* getShader( Shader::Type type ) const
	{
		switch( type )
		{
			case Shader::Type::Vertex:
				return m_pVertexShader;
			case Shader::Type::TessellationControl:
				return m_pTessControlShader;
			case Shader::Type::TessellationEvaluation:
				return m_pTessEvalShader;
			case Shader::Type::Geometry:
				return m_pGeometryShader;
			case Shader::Type::Fragment:
				return m_pFragmentShader;
		}
	}

	bool               link();

	inline bool        isLinked() const
	{
		GLint linkStatus;
		glGetProgramiv( m_glProgram, GL_LINK_STATUS, &linkStatus );
		return ( linkStatus == GL_TRUE );
	}

	void               getLinkLog(
			std::size_t maxLength, std::size_t* logLength, char* log ) const;

	inline GLuint      getID() const { return m_glProgram; }

	inline const char* getName() const { return m_pName; }


	inline void setUniform( Uniform type, GLfloat val )
	{
		glProgramUniform1f(
				m_glProgram, m_UniformLocations[ enum_cast( type ) ], val );
	}
	inline void setUniform( Uniform type, const glm::vec2& vec )
	{
		glProgramUniform2fv(
				m_glProgram,
				m_UniformLocations[ enum_cast( type ) ],
				1,
				glm::value_ptr( vec ) );
	}
	inline void setUniform( Uniform type, const glm::vec3& vec )
	{
		glProgramUniform3fv(
				m_glProgram,
				m_UniformLocations[ enum_cast( type ) ],
				1,
				glm::value_ptr( vec ) );
	}
	inline void setUniform( Uniform type, const glm::vec4& vec )
	{
		glProgramUniform4fv(
				m_glProgram,
				m_UniformLocations[ enum_cast( type ) ],
				1,
				glm::value_ptr( vec ) );
	}
	inline void setUniform( Uniform type, const glm::mat3& mat )
	{
		glProgramUniformMatrix3fv(
				m_glProgram,
				m_UniformLocations[ enum_cast( type ) ],
				1,
				GL_FALSE,
				glm::value_ptr( mat ) );
	}
	inline void setUniform( Uniform type, const glm::mat4& mat )
	{
		glProgramUniformMatrix4fv(
				m_glProgram,
				m_UniformLocations[ enum_cast( type ) ],
				1,
				GL_FALSE,
				glm::value_ptr( mat ) );
	}


	inline CustomUniform getCustomUniform( const char* name )
	{
#ifdef DEBUG
		CustomUniform res = glGetUniformLocation( m_glProgram, name );
		if( res < 0 )
		{
			dbg_printf( "Shader %d variable '%s' is not active "
					"or does not exist.\n", m_glProgram, name );
		}
		return res;
#else
		return glGetUniformLocation( m_glProgram, name );
#endif
	}

	inline void setCustomUniform( CustomUniform uniform, GLfloat val )
	{
		glProgramUniform1f( m_glProgram, uniform, val );
	}
	inline void setCustomUniform( CustomUniform uniform, GLuint val )
	{
		glProgramUniform1ui( m_glProgram, uniform, val );
	}
	inline void setCustomUniform( CustomUniform uniform, const glm::vec2& vec )
	{
		glProgramUniform2fv( m_glProgram, uniform, 1, glm::value_ptr( vec ) );
	}
	inline void setCustomUniform( CustomUniform uniform, const glm::vec3& vec )
	{
		glProgramUniform3fv( m_glProgram, uniform, 1, glm::value_ptr( vec ) );
	}
	inline void setCustomUniform( CustomUniform uniform, const glm::vec4& vec )
	{
		glProgramUniform4fv( m_glProgram, uniform, 1, glm::value_ptr( vec ) );
	}
	inline void setCustomUniform( CustomUniform uniform, const glm::mat3& mat )
	{
		glProgramUniformMatrix3fv(
				m_glProgram, uniform, 1, GL_FALSE, glm::value_ptr( mat ) );
	}
	inline void setCustomUniform( CustomUniform uniform, const glm::mat4& mat )
	{
		glProgramUniformMatrix4fv(
				m_glProgram, uniform, 1, GL_FALSE, glm::value_ptr( mat ) );
	}

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

	inline static Uniform     nameToUniform( const char* name );

	inline static const char* uniformToName( Uniform uniform );

	void extractUniformLocations();

	void inspect();

private:
	static constexpr int  NAME_SIZE           = 64;
	static constexpr int  NUM_SHADERS         = 5;

	static constexpr char UNIFORM_NAMES[][ NAME_SIZE ] = {
		"mat_view",               // view matrix
		"mat_view_normal",        // view normal matrix
		"mat_projection",         // projection matrix
		"mat_view_projection",    // view-projection matrix
		"mat_model",              // model matrix
		"mat_model_normal",       // model normal matrix
		"f_time",                 // virtual time as float
	};

	static std::unordered_map<std::string, Uniform> s_mapNameToUniform;
	static StaticInitializer                        s_Initializer;

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
