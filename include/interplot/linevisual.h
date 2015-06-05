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

	std::size_t getNumPoints() const
	{
		return ( m_uiNumPoints * m_uiNumInstances );
	}
	void setNumPoints( std::size_t points );

	std::size_t getTubeFaces() const
	{
		return m_uiNumFaces;
	}
	void setTubeFaces( std::size_t faces )
	{
		m_uiNumFaces = faces;
	}

	float getTubeRadius() const
	{
		return m_fRadius;
	}
	void setTubeRadius( float value )
	{
		m_fRadius = value;
	}

	std::size_t getNumLines() const
	{
		return m_uiNumLines;
	}
	void setNumLines( std::size_t lines )
	{
		m_uiNumLines = lines;
	}

	float getLineDistance() const
	{
		return m_fLineDistance;
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
