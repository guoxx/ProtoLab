#include "stdafx.h"
#include "Scene.h"
#include "Model.h"


Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::update(double delta)
{
	for (auto mod : _models)
	{
		mod->update();
	}
}
