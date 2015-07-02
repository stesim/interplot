#pragma once

#include "vertexbuffer.h"
#include "shaderprogram.h"
#include "uniformbuffer.h"
#include "gpufunctionsource.h"

namespace interplot
{

class SurfaceVisual
{
public:
	SurfaceVisual();
	~SurfaceVisual();

	void initialize();

	void finalize();

	void update();

	void render();

	inline const uvec2& getNumPoints() const
	{
		return m_vecNumPoints;
	}
	void setNumPoints( const uvec2& points );

	const GpuFunctionSource& getFunction() const
	{
		return m_Function;
	}
	void setFunction( const GpuFunctionSource& func );

	inline vec2 getParamStart() const
	{
		return m_vecParamStart;
	}
	void setParamStart( const vec2& value );

	inline vec2 getParamEnd() const
	{
		return m_vecParamEnd;
	}
	void setParamEnd( const vec2& value );

	inline ShaderProgram* getShader()
	{
		return m_pShaderProgram;
	}

private:
	static constexpr size_t      MAX_POINTS_PER_INSTANCE_DIM = 64;

	VertexBuffer<LineVertex>     m_VertexBuffer;

	Shader*                      m_pVertexShader;
	Shader*                      m_pTessControlShader;
	Shader*                      m_pTessEvalShader;
	Shader*                      m_pFragmentShader;

	ShaderProgram*               m_pShaderProgram;
	ShaderProgram::CustomUniform m_uniInstances;
	ShaderProgram::CustomUniform m_uniParamStart;
	ShaderProgram::CustomUniform m_uniParamEnd;
	ShaderProgram::CustomUniform m_uniPointsPerInstance;

	vec2   m_vecParamStart;
	vec2   m_vecParamEnd;

	uvec2  m_vecNumPoints;
	uvec2  m_vecNumInstances;
	uvec2  m_vecPointsPerInstance;

	GpuFunctionSource m_Function;
};

}
