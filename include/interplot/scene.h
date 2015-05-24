#pragma once

#include "camera.h"

namespace interplot
{

class Scene
{
public:
	Scene();
	virtual ~Scene();

	inline       Camera* getActiveCamera()
	{
		return m_pActiveCamera;
	}
	inline const Camera* getActiveCamera() const
	{
		return m_pActiveCamera;
	}
	void setActiveCamera( Camera* cam );

	virtual void initialize() = 0;

	virtual void update() = 0;

	virtual void render() = 0;

protected:
	Camera* m_pActiveCamera;
};

}
