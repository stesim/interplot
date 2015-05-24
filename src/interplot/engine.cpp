#include "engine.h"

namespace interplot
{

Engine engine;

Engine::Engine()
	: frame( 0 ),
	m_pCurrentScene( nullptr )
{
}

Engine::~Engine()
{
}

void Engine::initialize()
{
	time.initialize();
	input.initialize();

	initializeDefaultShaders();
}

void Engine::update()
{
	time.update();
	input.update();

	if( m_pCurrentScene != nullptr )
	{
		m_pCurrentScene->update();
	}
}

void Engine::render()
{
	if( m_pCurrentScene != nullptr )
	{
		m_pCurrentScene->render();
		++frame;
	}
}

void Engine::setScene( Scene* scene )
{
	m_pCurrentScene = scene;
}

void Engine::initializeDefaultShaders()
{
	Shader* defVS = Shader::fromName( Shader::Type::Vertex,   "default" );
	Shader* defFS = Shader::fromName( Shader::Type::Fragment, "default" );

	assert( shaders.add( defVS, "default" ) );
	assert( shaders.add( defFS, "default" ) );

	ShaderProgram* defShader = ShaderProgram::assemble(
			shaders.getShader<Shader::Type::Vertex>( "default" ),
			shaders.getShader<Shader::Type::Fragment>( "default" ) );

	assert( shaders.add( defShader, "default" ) );


	Shader* vs  = Shader::fromName( Shader::Type::Vertex,                 "sandbox" );
	Shader* fs  = Shader::fromName( Shader::Type::Fragment,               "sandbox" );
	Shader* tcs = Shader::fromName( Shader::Type::TessellationControl,    "sandbox" );
	Shader* tes = Shader::fromName( Shader::Type::TessellationEvaluation, "sandbox" );

	assert( shaders.add( vs,  "sandbox" ) );
	assert( shaders.add( fs,  "sandbox" ) );
	assert( shaders.add( tcs, "sandbox" ) );
	assert( shaders.add( tes, "sandbox" ) );

	ShaderProgram* prog = ShaderProgram::assemble( vs, fs, tcs, tes );

	assert( shaders.add( prog, "sandbox" ) );


	vs  = Shader::fromName( Shader::Type::Vertex,                 "line" );
	fs  = Shader::fromName( Shader::Type::Fragment,               "line" );
	tcs = Shader::fromName( Shader::Type::TessellationControl,    "line" );
	tes = Shader::fromName( Shader::Type::TessellationEvaluation, "line" );

	assert( shaders.add( vs,  "line" ) );
	assert( shaders.add( fs,  "line" ) );
	assert( shaders.add( tcs, "line" ) );
	assert( shaders.add( tes, "line" ) );

	prog = ShaderProgram::assemble( vs, fs, tcs, tes );

	assert( shaders.add( prog, "line" ) );
}

}
