#pragma once

class Camera;
class Scene;
class Viewport;

class ForwardRenderer
{
public:
	ForwardRenderer();
	~ForwardRenderer();

	void beginFrame();
	void render(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport);
	void endFrame();

private:

	DX11RenderTarget*				_sceneRT;
	DX11DepthStencilRenderTarget*	_sceneDepthRT;
};

