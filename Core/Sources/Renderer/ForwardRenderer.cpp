#include "stdafx.h"
#include "ForwardRenderer.h"
#include "Viewport.h"
#include "Filter2D.h"
#include "../Objects/Camera.h"
#include "../Objects/Scene.h"
#include "../Objects/Model.h"
// TODO: remove this
#include "../Mesh/Mesh.h"
#include "../Win32/Win32Application.h"

#include "../../Shaders/FilterIdentity_vs.h"
#include "../../Shaders/FilterIdentity_ps.h"


ForwardRenderer::ForwardRenderer()
{
	// TODO: hard code window size

	_swapChain = RHI::getInst().createSwapChain(Win32Application::GetHwnd(), FRAME_COUNT, WIN_WIDTH, WIN_HEIGHT);
	_backbufferRT = std::make_shared<DX11RenderTarget>(_swapChain);

	_sceneRT = std::make_shared<DX11RenderTarget>(WIN_WIDTH, WIN_HEIGHT, 1, DXGI_FORMAT_R8G8B8A8_TYPELESS, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	_sceneDepthRT = std::make_shared<DX11DepthStencilRenderTarget>(WIN_WIDTH, WIN_HEIGHT, 1, DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D32_FLOAT);

	std::shared_ptr<DX11VertexShader> vs = std::make_shared<DX11VertexShader>(g_FilterIdentity_vs, sizeof(g_FilterIdentity_vs));
	std::shared_ptr<DX11PixelShader> ps = std::make_shared<DX11PixelShader>(g_FilterIdentity_ps, sizeof(g_FilterIdentity_ps));
	_filterIdentity = std::make_shared<Filter2D>(vs, ps);
}


ForwardRenderer::~ForwardRenderer()
{
	RHI::getInst().destroySwapChain(_swapChain);
}


void ForwardRenderer::beginFrame()
{
	RHI::getInst().setDefaultRHIStates();	

	RHI::getInst().getContext()->clear(_backbufferRT->getRenderTarget(), 0, 0, 0, 0);
}


void ForwardRenderer::render(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport)
{
	std::shared_ptr<DX11GraphicContext> gfxContext = RHI::getInst().getContext();

	gfxContext->clear(_sceneRT->getRenderTarget(), 0.f, 0.f, 0.f, 0.f);
	gfxContext->clear(_sceneDepthRT->getRenderTarget(), DX11GraphicContext::RHI_CLEAR_FLAG::RHI_CLEAR_DEPTH, 1.0f);

	uint32_t x, y, w, h;
	viewport->getViewport(x, y, w, h);
	gfxContext->RSSetViewport(x, y, w, h);

	ID3D11RenderTargetView* rtvs[] = {_sceneRT->getRenderTarget()};
	gfxContext->OMSetRenderTargets(1, rtvs, _sceneDepthRT->getRenderTarget());

	auto models = scene->getModels();
	auto pointLights = scene->getPointLights(); 
	for (auto model : models)
	{
		auto mesh = model->getMesh();
		for (auto pl : pointLights)
		{
			mesh->draw(model->getWorldMatrix(), camera.get(), pl);
		}
	}

	_filterIdentity->apply(_sceneRT, _backbufferRT);
}

void ForwardRenderer::endFrame()
{
	RHI::getInst().submit();
}

void ForwardRenderer::present()
{
	_swapChain->Present(0, 0);
}
