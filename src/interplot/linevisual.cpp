#include "linevisual.h"
#include "engine.h"
#include "shaderprogram.h"
#include <string>

namespace interplot
{

LineVisual::LineVisual() :
	m_pVertexShader( nullptr ),
	m_pTessControlShader( nullptr ),
	m_pTessEvalShader( nullptr ),
	m_pFragmentShader( nullptr ),
	m_pShaderProgram( nullptr ),
	m_uniRadius( -1 ),
	m_uniNumPoints( -1 ),
	m_uniTubeFaces( -1 ),
	m_uniInstancesPerLine( -1 ),
	m_uniParamStart( -1 ),
	m_uniParamEnd( -1 ),
	m_uniLineDistance( -1 ),
	m_fParamStart( 0.0f ),
	m_fParamEnd( 1.0f ),
	m_uiNumPoints( MAX_POINTS_PER_INSTANCE ),
	m_uiNumInstances( 1 ),
	m_uiNumFaces( 16 ),
	m_fRadius( 0.05f ),
	m_uiNumLines( 1 ),
	m_fLineDistance( 1.0f )
{
}

LineVisual::~LineVisual()
{
}

void LineVisual::initialize()
{
	m_VertexBuffer.resize( 4 );
	m_VertexBuffer.allocHostMemory();
	m_VertexBuffer[ 0 ] = LineVertex( 0.0f, 0.0f );
	m_VertexBuffer[ 1 ] = LineVertex( 0.0f, 1.0f );
	m_VertexBuffer[ 2 ] = LineVertex( 1.0f, 1.0f );
	m_VertexBuffer[ 3 ] = LineVertex( 1.0f, 0.0f );
	m_VertexBuffer.copyToDevice();

	//m_pShaderProgram = engine.shaders.getProgram( "line" );
	ShaderManager& shaders = engine.shaders;
	m_pVertexShader =
		shaders.getShader<Shader::Type::Vertex>( "line" );
	m_pTessControlShader =
		shaders.getShader<Shader::Type::TessellationControl>( "line" );
//	m_pTessEvalShader =
//		shaders.getShader<Shader::Type::TessellationEvaluation>( "line" );
	m_pTessEvalShader =
		Shader::fromName( Shader::Type::TessellationEvaluation, "line" );
	m_pFragmentShader =
		shaders.getShader<Shader::Type::Fragment>( "line" );

	setFunction( GpuFunctionSource( 2, 3,
				"return vec3( 10.0 * x, 2.0 * 2.0 * sin( 1.0 * f_time + PERIODS * x * TWO_PI ), 2.0 * 2.0 * cos( 1.0 * f_time + PERIODS * x * TWO_PI ) ) + vec3( 0.0, 0.0, y );" ) );
}

void LineVisual::finalize()
{
}

void LineVisual::update()
{
}

void LineVisual::render()
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
			m_uiNumInstances * m_uiNumLines );
}

void LineVisual::setNumPoints( std::size_t points )
{
	m_uiNumInstances =
		( points + MAX_POINTS_PER_INSTANCE - 1 ) / MAX_POINTS_PER_INSTANCE;
	m_uiNumPoints =
		( points + m_uiNumInstances - 1 ) / m_uiNumInstances;

	m_pShaderProgram->setCustomUniform(
			m_uniInstancesPerLine, (GLuint)m_uiNumInstances );
	m_pShaderProgram->setCustomUniform(
			m_uniNumPoints, (GLuint)m_uiNumPoints );
}

void LineVisual::setLineDistance( float distance )
{
	m_fLineDistance = distance;

	m_pShaderProgram->setCustomUniform(
			m_uniLineDistance, m_fLineDistance );
}

void LineVisual::setFunction( const GpuFunctionSource& func )
{
	if( ( func.getDomainDim() != 2 && func.getDomainDim() != 3 ) ||
			func.getImageDim() != 3 )
	{
		return;
	}

	m_Function = func;

	std::string source =
		"vec3 fun( float x, float y )\n"
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

	m_uniParamStart =
		m_pShaderProgram->getCustomUniform( "f_paramStart" );
	m_uniParamEnd =
		m_pShaderProgram->getCustomUniform( "f_paramEnd" );
	m_uniRadius =
		m_pShaderProgram->getCustomUniform( "f_lineRadius" );
	m_uniNumPoints =
		m_pShaderProgram->getCustomUniform( "ui_numPoints" );
	m_uniTubeFaces =
		m_pShaderProgram->getCustomUniform( "ui_tubeFaces" );
	m_uniInstancesPerLine =
		m_pShaderProgram->getCustomUniform( "ui_instancesPerLine" );
	m_uniLineDistance =
		m_pShaderProgram->getCustomUniform( "f_lineDistance" );

	m_pShaderProgram->setCustomUniform( m_uniParamStart,   m_fParamStart );
	m_pShaderProgram->setCustomUniform( m_uniParamEnd,     m_fParamEnd );
	m_pShaderProgram->setCustomUniform( m_uniTubeFaces,
			                            (GLuint)m_uiNumFaces );
	m_pShaderProgram->setCustomUniform( m_uniRadius,       m_fRadius );
	m_pShaderProgram->setCustomUniform( m_uniLineDistance, m_fLineDistance );

	setNumPoints( m_uiNumPoints );
}

void LineVisual::setTubeFaces( std::size_t faces )
{
	m_uiNumFaces = faces;
	m_pShaderProgram->setCustomUniform(
			m_uniTubeFaces, (GLuint)m_uniTubeFaces );
}

void LineVisual::setTubeRadius( float value )
{
	m_fRadius = value;
	m_pShaderProgram->setCustomUniform( m_uniRadius, m_fRadius );
}

void LineVisual::setParamStart( float value )
{
	m_fParamStart = value;
	m_pShaderProgram->setCustomUniform( m_uniParamStart, m_fParamStart );
}

void LineVisual::setParamEnd( float value )
{
	m_fParamStart = value;
	m_pShaderProgram->setCustomUniform( m_uniParamEnd, m_fParamEnd );
}

}
