#pragma once

class Model;

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<std::shared_ptr<Model>> getModels()
	{
		return _models;
	}

	void attachModel(std::shared_ptr<Model> model)
	{
		_models.push_back(model);
	}

	void detachModel(std::shared_ptr<Model> model)
	{
		// TODO: 
	}

private:
	std::vector<std::shared_ptr<Model>> _models;
};

