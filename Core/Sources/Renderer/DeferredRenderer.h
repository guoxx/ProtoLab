#pragma once

class Camera;
class Scene;
class Viewport;
class Filter2D;

class DeferredRenderer : public Noncopyable
{
public:
	DeferredRenderer();
	~DeferredRenderer();

	void beginFrame();
	void render(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport);
	void endFrame();

	void present();

private:
	void _renderShadowMap(std::shared_ptr<Scene> scene);
	void _renderGBuffer(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport);
	void _cullLighting(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport);
	void _renderLighting(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport);
	void _renderDebugDisplay(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport);

private:
	std::shared_ptr<DX11RenderTarget>				_gbuffer_Albedo_MatId;
	std::shared_ptr<DX11RenderTarget>				_gbuffer_Normal;

	std::shared_ptr<DX11RenderTarget>				_sceneRT;
	std::shared_ptr<DX11DepthStencilRenderTarget>	_sceneDepthRT;

	ComPtr<IDXGISwapChain>							_swapChain;
	std::shared_ptr<DX11RenderTarget>				_backbufferRT;

	std::shared_ptr<Filter2D>						_filterIdentity;
};

