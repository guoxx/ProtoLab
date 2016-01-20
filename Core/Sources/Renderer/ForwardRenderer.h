#pragma once

class Camera;
class Scene;
class Viewport;
class Filter2D;

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
	void _renderShadowMapPass(std::shared_ptr<Scene> scene);

private:
	std::shared_ptr<DX11DepthStencilRenderTarget>	_shadowMapRT;
	std::shared_ptr<DX11RenderTarget>				_sceneRT;
	std::shared_ptr<DX11DepthStencilRenderTarget>	_sceneDepthRT;

	ComPtr<IDXGISwapChain>							_swapChain;	
	std::shared_ptr<DX11RenderTarget>				_backbufferRT;

	std::shared_ptr<Filter2D>						_filterIdentity;
};

