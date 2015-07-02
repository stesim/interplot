#include "surfacevisual.h"
#include "engine.h"
#include "shaderprogram.h"
#include <string>

namespace interplot
{

constexpr size_t SurfaceVisual::MAX_POINTS_PER_INSTANCE_DIM;

SurfaceVisual::SurfaceVisual() :
	m_pVertexShader( nullptr ),
	m_pTessControlShader( nullptr ),
	m_pTessEvalShader( nullptr ),
	m_pFragmentShader( nullptr ),
	m_pShaderProgram( nullptr ),
	m_uniInstances( -1 ),
	m_uniParamStart( -1 ),
	m_uniParamEnd( -1 ),
	m_vecParamStart( 0.0f, 0.0f ),
	m_vecParamEnd( 1.0f, 1.0f ),
	m_vecNumPoints( MAX_POINTS_PER_INSTANCE_DIM, MAX_POINTS_PER_INSTANCE_DIM ),
	m_vecNumInstances( 1, 1 ),
	m_vecPointsPerInstance( m_vecNumPoints )
{
}

SurfaceVisual::~SurfaceVisual()
{
}

void SurfaceVisual::initialize()
{
	m_VertexBuffer.resize( 4 );
	m_VertexBuffer.allocHostMemory();
	m_VertexBuffer[ 0 ] = LineVertex( 0.0f, 0.0f );
	m_VertexBuffer[ 1 ] = LineVertex( 0.0f, 1.0f );
	m_VertexBuffer[ 2 ] = LineVertex( 1.0f, 1.0f );
	m_VertexBuffer[ 3 ] = LineVertex( 1.0f, 0.0f );
	m_VertexBuffer.copyToDevice();

	ShaderManager& shaders = engine.shaders;
	m_pVertexShader =
		shaders.getShader<Shader::Type::Vertex>( "line" );
	m_pTessControlShader =
		shaders.getShader<Shader::Type::TessellationControl>( "surface" );
	m_pTessEvalShader =
		Shader::fromName( Shader::Type::TessellationEvaluation, "surface" );
	m_pFragmentShader =
		shaders.getShader<Shader::Type::Fragment>( "line" );

//	setFunction( GpuFunctionSource( 2, 3,
//				"return vec3( v, 0.0 );" ) );
	setFunction( GpuFunctionSource( 2, 3,
				"float R = 2.0;\n"
				"float rmin = 1.0;\n"
				"float rmax = R;\n"
				"float s = R - v.y * ( ( rmax - rmin ) * 0.5 * ( sin( t ) + 1.0 ) + rmin );\n"
				"return vec3( 10.0 * v.x - sin( 0.5 * t ) * 2.0 * ( ( s / R - 1.0 ) * ( s / R - 1.0 ) ), s * 2.0 * sin( 1.0 * f_time + PERIODS * v.x * TWO_PI ), s * 2.0 * cos( 1.0 * f_time + PERIODS * v.x * TWO_PI ) );" ) );
}

void SurfaceVisual::finalize()
{
}

void SurfaceVisual::update()
{
}

void SurfaceVisual::render()
{
	m_pShaderProgram->setUniform(
			ShaderProgram::Uniform::ModelMatrix,
			glm::mat4( 1.0f ) );
	m_pShaderProgram->setUniform(
			ShaderProgram::Uniform::ModelNormalMatrix,
			glm::mat3( 1.0f ) );

	engine.renderer.render(
			m_VertexBuffer,
			0,
			m_VertexBuffer.numVertices(),
			m_vecNumInstances.x * m_vecNumInstances.y );
}

void SurfaceVisual::setNumPoints( const uvec2& points )
{
	m_vecNumInstances = ( points + uvec2( MAX_POINTS_PER_INSTANCE_DIM - 1 ) ) /
		                MAX_POINTS_PER_INSTANCE_DIM;

	m_vecPointsPerInstance = ( points + m_vecNumInstances - uvec2( 1 ) ) /
		                     m_vecNumInstances;

	m_vecNumPoints = m_vecNumInstances * m_vecPointsPerInstance;

	if( m_pShaderProgram != nullptr )
	{
		dbg_printf( "setNumPoints:\n%f, %f\n%f, %f\n%lu, %lu\n%lu, %lu\n",
				m_vecParamStart.x, m_vecParamStart.y,
				m_vecParamEnd.x, m_vecParamEnd.y,
				m_vecPointsPerInstance.x, m_vecPointsPerInstance.y,
				m_vecNumPoints.x, m_vecNumPoints.y );

		m_pShaderProgram->setCustomUniform( m_uniPointsPerInstance,
											m_vecPointsPerInstance );
		m_pShaderProgram->setCustomUniform( m_uniInstances, m_vecNumInstances );
	}
}

void SurfaceVisual::setFunction( const GpuFunctionSource& func )
{
	if( ( func.getDomainDim() != 2 && func.getDomainDim() != 3 ) ||
			func.getImageDim() != 3 )
	{
		return;
	}

	m_Function = func;

	std::string source =
		"vec3 fun( vec2 v )\n"
		"{\n";
	source += m_Function.getFunctionSource();
	source += "\n"
		"}\n";

	const char* sources[] = { m_pTessEvalShader->getSource(), source.c_str() };

	Shader* tesShader = Shader::fromSources(
			Shader::Type::TessellationEvaluation,
			sources,
			2 );
	tesShader->compile();

	if( m_pShaderProgram != nullptr )
	{
		ShaderProgram::destroy( m_pShaderProgram );
	}

	m_pShaderProgram = ShaderProgram::assemble(
			m_pVertexShader,
			m_pTessControlShader,
			tesShader,
			m_pFragmentShader );
	m_pShaderProgram->link();

	m_uniPointsPerInstance =
		m_pShaderProgram->getCustomUniform( "vec_pointsPerInstance" );
	m_uniInstances =
		m_pShaderProgram->getCustomUniform( "vec_instances" );
	m_uniParamStart =
		m_pShaderProgram->getCustomUniform( "vec_paramStart" );
	m_uniParamEnd =
		m_pShaderProgram->getCustomUniform( "vec_paramEnd" );

	m_pShaderProgram->setCustomUniform( m_uniPointsPerInstance,
			                            m_vecPointsPerInstance );
	m_pShaderProgram->setCustomUniform( m_uniInstances, m_vecNumInstances );

	dbg_printf( "setFunction:\n%f, %f\n%f, %f\n%lu, %lu\n",
			m_vecParamStart.x, m_vecParamStart.y,
			m_vecParamEnd.x, m_vecParamEnd.y,
			m_vecPointsPerInstance.x, m_vecPointsPerInstance.y );

	m_pShaderProgram->setCustomUniform( m_uniParamStart, m_vecParamStart );
	m_pShaderProgram->setCustomUniform( m_uniParamEnd,   m_vecParamEnd );

}

void SurfaceVisual::setParamStart( const vec2& value )
{
	m_vecParamStart = value;
	m_pShaderProgram->setCustomUniform( m_uniParamStart, m_vecParamStart );
}

void SurfaceVisual::setParamEnd( const vec2& value )
{
	m_vecParamStart = value;
	m_pShaderProgram->setCustomUniform( m_uniParamEnd, m_vecParamEnd );
}

}
