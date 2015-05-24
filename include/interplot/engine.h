#pragma once

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
	unsigned long frame;

private:
	Scene* m_pCurrentScene;
};

extern Engine engine;

}
