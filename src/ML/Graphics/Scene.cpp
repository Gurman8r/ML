#include <ML/Graphics/Scene.hpp>
#include <ML/Graphics/RenderTarget.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Scene::Scene()
	{
	}

	Scene::~Scene() { dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Scene::dispose() 
	{
		return false;
	}

	bool Scene::loadFromFile(const String & filename) 
	{ 
		return false;
	}

	bool Scene::saveToFile(const String & filename) const 
	{ 
		return false; 
	}

	void Scene::draw(RenderTarget & target, RenderBatch batch) const
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}