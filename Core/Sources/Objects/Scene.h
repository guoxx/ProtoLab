#pragma once

class Model;
class PointLight;

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<std::shared_ptr<Model>> getModels();
	std::vector<std::shared_ptr<PointLight>> getPointLights();

	void attachModel(std::shared_ptr<Model> model);
	void detachModel(std::shared_ptr<Model> model);

	void attachPointLight(std::shared_ptr<PointLight> pointLight);
	void detachPointLight(std::shared_ptr<PointLight> pointLight);

	void update(double delta);

private:
	std::vector<std::shared_ptr<PointLight>> _pointLights;
	std::vector<std::shared_ptr<Model>> _models;
};

