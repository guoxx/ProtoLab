#pragma once

class Camera;
class Scene;
class Viewport;

class ForwardRenderer : public Noncopyable
{
public:
	ForwardRenderer();
	~ForwardRenderer();

	void beginFrame();
	void render(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport);
	void endFrame();

	void present();

private:
	std::shared_ptr<DX11RenderTarget>				_sceneRT;
	std::shared_ptr<DX11DepthStencilRenderTarget>	_sceneDepthRT;

	IDXGISwapChain*									_swapChain;	
	std::shared_ptr<DX11RenderTarget>				_backbufferRT;

	// constants
	const static int32_t			FRAME_COUNT = 2;
};

