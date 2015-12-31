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

	_swapChain = RHI::GetInst().createSwapChain(Win32Application::GetHwnd(), FRAME_COUNT, WIN_WIDTH, WIN_HEIGHT);
	_backbufferRT = RHI::GetInst().createRenderTargetViewFromSwapChain(_swapChain);

	_sceneRT = RHI::GetInst().createRenderTarget(WIN_WIDTH, WIN_HEIGHT, 1, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	_sceneDepthRT = RHI::GetInst().createDepthStencilRenderTarget(WIN_WIDTH, WIN_HEIGHT, 1, DXGI_FORMAT_D32_FLOAT);
}


ForwardRenderer::~ForwardRenderer()
{
	RHI::GetInst().destroySwapChain(_swapChain);
	RHI::GetInst().destroyRenderTarget(_backbufferRT);

	RHI::GetInst().destroyRenderTarget(_sceneRT);
	RHI::GetInst().destroyDepthStencilRenderTarget(_sceneDepthRT);
}


void ForwardRenderer::beginFrame()
{
	RHI::GetInst().setDefaultRHIStates();	

	RHI::GetInst().clear(_backbufferRT->getRenderTarget(), 0, 0, 0, 0);
}


void ForwardRenderer::render(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport)
{
	RHI::GetInst().clear(_sceneRT->getRenderTarget(), 0.f, 0.f, 0.f, 0.f);
	RHI::GetInst().clear(_sceneDepthRT->getRenderTarget(), RHI::RHI_CLEAR_FLAG::RHI_CLEAR_DEPTH, 1.0f);

	uint32_t x, y, w, h;
	viewport->getViewport(x, y, w, h);
	RHI::GetInst().setViewport(x, y, w, h);

	ID3D11RenderTargetView* rtvs[] = {_backbufferRT->getRenderTarget()};
	RHI::GetInst()._context->OMSetRenderTargets(1, rtvs, _sceneDepthRT->getRenderTarget());

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
