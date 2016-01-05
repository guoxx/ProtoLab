#pragma once

class Camera;
class Scene;
class Viewport;
class Filter2D;

class DeferredRenderer
{
public:
	DeferredRenderer();
	~DeferredRenderer();

	void beginFrame();
	void render(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport);
	void endFrame();

	void present();

private:
	std::shared_ptr<DX11RenderTarget>				_sceneRT;
	std::shared_ptr<DX11DepthStencilRenderTarget>	_sceneDepthRT;

	IDXGISwapChain*									_swapChain;	
	std::shared_ptr<DX11RenderTarget>				_backbufferRT;

	std::shared_ptr<Filter2D>						_filterIdentity;

	// constants
	const static int32_t			FRAME_COUNT = 2;
};

