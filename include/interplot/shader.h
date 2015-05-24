#pragma once

#include <GL/glew.h>
#include <cstddef>

namespace interplot
{

class Shader
{
friend class ShaderManager;

public:
	enum class Type
	{
		Vertex                 = GL_VERTEX_SHADER,
		TessellationControl    = GL_TESS_CONTROL_SHADER,
		TessellationEvaluation = GL_TESS_EVALUATION_SHADER,
		Geometry               = GL_GEOMETRY_SHADER,
		Fragment               = GL_FRAGMENT_SHADER
	};

public:
	Shader( const Shader& ) = delete;
	Shader( Shader&& )      = delete;

	Shader& operator=( const Shader& ) = delete;
	Shader& operator=( Shader&& )      = delete;

	static Shader*     fromName( Type type, const char* name );
	static Shader*     fromSource( Type type, const char* source );
	static Shader*     fromFile( Type type, const char* path );

	static void        destroy( Shader* shader );

	bool               compile();

	inline bool        isCompiled() const
	{
		GLint compileStatus;
		glGetShaderiv( m_glShader, GL_COMPILE_STATUS, &compileStatus );
		return ( compileStatus == GL_TRUE );
	}

	void               getCompilationLog(
			std::size_t maxLength,
			std::size_t* logLength,
			char* log ) const;

	inline GLuint      getID() const { return m_glShader; }

	inline Type        getType() const { return m_Type; }

	inline const char* getName() const { return m_pName; }

private:
	Shader();
	Shader( Type type, const char* source );
	~Shader();

	static const char* typeToExtension( Type type );

private:
	static constexpr int  NAME_SIZE           = 64;
	static constexpr char NAME_TO_PATH_MASK[] = "resources/shaders/%s.%s.glsl";

	Type   m_Type;
	GLuint m_glShader;
	char   m_pName[ NAME_SIZE ];
	bool   m_bManaged;
};

}
