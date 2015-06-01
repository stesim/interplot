#pragma once

#include <GL/glew.h>
#include "input.h"
#include "globaltimes.h"
#include "shadermanager.h"
#include "scene.h"

namespace interplot
{

class Engine
{
public:
	Engine();
	~Engine();

	void initialize();

	void finalize();

	void update();

	void render();

	inline Scene* getScene()
	{
		return m_pCurrentScene;
	}
	void setScene( Scene* scene );

	template<typename T>
	void setVertexLayout()
	{
		typedef typename T::layout layout;

		// disable no longer required attributes
		for( GLuint i = layout::NUM_ATTRIBUTES;
				i < m_uiActiveVertexAttributes;
				++i )
		{
			glDisableVertexAttribArray( i );
		}

		// enable newly required attributes
		for( GLuint i = m_uiActiveVertexAttributes;
				i < layout::NUM_ATTRIBUTES;
				++i )
		{
			glEnableVertexAttribArray( i );
		}

		m_uiActiveVertexAttributes = layout::NUM_ATTRIBUTES;

		layout::activate();
	}

private:
	void initializeDefaultShaders();

public:
	Input         input;
	GlobalTimes   time;
	ShaderManager shaders;
	unsigned long frame;

private:
	Scene* m_pCurrentScene;

	GLuint m_uiActiveVertexAttributes;
	GLuint m_glDefaultVao;
};

extern Engine engine;

}
