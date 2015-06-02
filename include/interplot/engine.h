#pragma once

#include <GL/glew.h>
#include "input.h"
#include "globaltimes.h"
#include "shadermanager.h"
#include "scene.h"
#include "renderer.h"

namespace interplot
{

struct VertexDescriptor;

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

private:
	void initializeDefaultShaders();

public:
	Input         input;
	GlobalTimes   time;
	ShaderManager shaders;
	Renderer      renderer;
	unsigned long frame;

private:
	Scene* m_pCurrentScene;
};

extern Engine engine;

}
