#pragma once

class Camera;
class Scene;

class ForwardRenderer
{
public:
	ForwardRenderer();
	~ForwardRenderer();

	void render(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene);
};

