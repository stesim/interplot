#include "linevisual.h"
#include "engine.h"
#include "shaderprogram.h"

namespace interplot
{

LineVisual::LineVisual() /*:
	m_fParamStart( 0.0f ),
	m_fParamEnd( 1.0f ),
	m_uiNumPoints( MAX_POINTS_PER_INSTANCE ),
	m_uiNumInstances( 1 ),
	m_uiNumFaces( 16 ),
	m_fRadius( 0.05f ),
	m_uiNumLines( 1 ),
	m_fLineDistance( 1.0f )*/
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

	m_pShaderProgram = engine.shaders.getProgram( "line" );

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

	m_pShaderProgram->setCustomUniform( m_uniParamStart,   0.0f );
	m_pShaderProgram->setCustomUniform( m_uniParamEnd,     1.0f );
	m_pShaderProgram->setCustomUniform( m_uniTubeFaces,    16u );
	m_pShaderProgram->setCustomUniform( m_uniRadius,       0.05f );
	m_pShaderProgram->setCustomUniform( m_uniLineDistance, 5.0f );

	setNumPoints( 256 );

	m_uiNumLines = 3;
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
			m_VertexBuffer.size(),
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

}
