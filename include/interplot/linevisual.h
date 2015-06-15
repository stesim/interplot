#pragma once

#include "vertexbuffer.h"
#include "shaderprogram.h"
#include "uniformbuffer.h"

namespace interplot
{

class LineVisual
{
public:
	LineVisual();
	~LineVisual();

	void initialize();

	void finalize();

	void update();

	void render();

	inline std::size_t getNumPoints() const
	{
		return ( m_uiNumPoints * m_uiNumInstances );
	}
	void setNumPoints( std::size_t points );

	inline std::size_t getTubeFaces() const
	{
		return m_uiNumFaces;
	}
	void setTubeFaces( std::size_t faces );

	inline float getTubeRadius() const
	{
		return m_fRadius;
	}
	void setTubeRadius( float value );

	inline std::size_t getNumLines() const
	{
		return m_uiNumLines;
	}
	inline void setNumLines( std::size_t lines )
	{
		m_uiNumLines = lines;
	}

	inline float getLineDistance() const
	{
		return m_fLineDistance;
	}
	void setLineDistance( float distance );

	void setFunction( const char* function,
	                  const char* gradient,
	                  const char* normal );

	inline void getFunction( const char*& function,
	                         const char*& gradient,
	                         const char*& normal )
	{
		function = m_pFunctionString;
		gradient = m_pGradientString;
		normal   = m_pNormalString;
	}

	inline float getParamStart() const
	{
		return m_fParamStart;
	}
	void setParamStart( float value );

	inline float getParamEnd() const
	{
		return m_fParamEnd;
	}
	void setParamEnd( float value );

	inline ShaderProgram* getShader()
	{
		return m_pShaderProgram;
	}

private:
	static constexpr std::size_t MAX_POINTS_PER_INSTANCE = 64;

	VertexBuffer<LineVertex>        m_VertexBuffer;

	Shader* m_pVertexShader;
	Shader* m_pTessControlShader;
	Shader* m_pTessEvalShader;
	Shader* m_pFragmentShader;

	ShaderProgram*               m_pShaderProgram;
	ShaderProgram::CustomUniform m_uniRadius;
	ShaderProgram::CustomUniform m_uniNumPoints;
	ShaderProgram::CustomUniform m_uniTubeFaces;
	ShaderProgram::CustomUniform m_uniInstancesPerLine;
	ShaderProgram::CustomUniform m_uniParamStart;
	ShaderProgram::CustomUniform m_uniParamEnd;
	ShaderProgram::CustomUniform m_uniLineDistance;

	float       m_fParamStart;
	float       m_fParamEnd;

	std::size_t m_uiNumPoints;
	std::size_t m_uiNumInstances;
	std::size_t m_uiNumFaces;
	float       m_fRadius;
	std::size_t m_uiNumLines;
	float       m_fLineDistance;
	char*       m_pFunctionString;
	char*       m_pGradientString;
	char*       m_pNormalString;
};

}
