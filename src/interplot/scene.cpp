#include "scene.h"

namespace interplot
{

Scene::Scene()
	: m_pActiveCamera( nullptr )
{
}

Scene::~Scene()
{
}

void Scene::setActiveCamera( Camera* cam )
{
	m_pActiveCamera = cam;
}

}
