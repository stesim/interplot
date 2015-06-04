#pragma once

#include "vertexbuffer.h"
#include "shaderprogram.h"

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

	void setNumPoints( std::size_t points );

	void setTubeFaces( std::size_t faces )
	{
		m_uiNumFaces = faces;
	}

	void setTubeRadius( float value )
	{
		m_fRadius = value;
	}

	void setNumLines( std::size_t lines )
	{
		m_uiNumLines = lines;
	}

	void setLineDistance( float distance );

private:
	static constexpr std::size_t MAX_POINTS_PER_INSTANCE = 64;

	VertexBuffer<LineVertex> m_VertexBuffer;

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
};

}
