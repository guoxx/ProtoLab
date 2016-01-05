#include "stdafx.h"
#include "Scene.h"
#include "Model.h"
#include "PointLight.h"


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

std::vector<std::shared_ptr<Model>> Scene::getModels()
{
	return _models;
}

std::vector<std::shared_ptr<PointLight>> Scene::getPointLights()
{
	return _pointLights;
}

void Scene::attachModel(std::shared_ptr<Model> model)
{
	_models.push_back(model);
}

void Scene::detachModel(std::shared_ptr<Model> model)
{
	// TODO: 
}

void Scene::attachPointLight(std::shared_ptr<PointLight> pointLight)
{
	_pointLights.push_back(pointLight);
}

void Scene::detachPointLight(std::shared_ptr<PointLight> pointLight)
{
	// TODO:
}
