#include "stdafx.h"
#include "ForwardRenderer.h"
#include "Viewport.h"
#include "../Objects/Camera.h"
#include "../Objects/Scene.h"
#include "../Objects/Model.h"
// TODO: remove this
#include "../Mesh/Mesh.h"
#include "../Win32/Win32Application.h"


ForwardRenderer::ForwardRenderer()
{
	// TODO: hard code window size

	_swapChain = RHI::createSwapChain(Win32Application::GetHwnd(), FRAME_COUNT, WIN_WIDTH, WIN_HEIGHT);
	_backbufferRT = RHI::createRenderTargetViewFromSwapChain(_swapChain);

	_sceneRT = RHI::createRenderTarget(WIN_WIDTH, WIN_HEIGHT, 1, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	_sceneDepthRT = RHI::createDepthStencilRenderTarget(WIN_WIDTH, WIN_HEIGHT, 1, DXGI_FORMAT_D32_FLOAT);
}


ForwardRenderer::~ForwardRenderer()
{
	RHI::destroySwapChain(_swapChain);
	RHI::destroyRenderTarget(_backbufferRT);

	RHI::destroyRenderTarget(_sceneRT);
	RHI::destroyDepthStencilRenderTarget(_sceneDepthRT);
}


void ForwardRenderer::beginFrame()
{
	RHI::setDefaultRHIStates();	

	RHI::clear(_backbufferRT->getRenderTarget(), 0, 0, 0, 0);
}


void ForwardRenderer::render(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport)
{
	RHI::clear(_sceneRT->getRenderTarget(), 0.f, 0.f, 0.f, 0.f);
	RHI::clear(_sceneDepthRT->getRenderTarget(), RHI::RHI_CLEAR_FLAG::RHI_CLEAR_DEPTH, 1.0f);

	uint32_t x, y, w, h;
	viewport->getViewport(x, y, w, h);
	RHI::setViewport(x, y, w, h);

	ID3D11RenderTargetView* rtvs[] = {_backbufferRT->getRenderTarget()};
	RHI::_context->OMSetRenderTargets(1, rtvs, _sceneDepthRT->getRenderTarget());

	auto models = scene->getModels();
	for (auto model :models)
	{
		auto mesh = model->getMesh();
		mesh->draw(camera.get());
	}

}

void ForwardRenderer::endFrame()
{

}

void ForwardRenderer::present()
{
	_swapChain->Present(0, 0);
}
