#include "stdafx.h"
#include "ForwardRenderer.h"
#include "Viewport.h"
#include "Filter2D.h"
#include "../Objects/Camera.h"
#include "../Objects/Scene.h"
#include "../Objects/Model.h"
#include "../Objects/PointLight.h"
// TODO: remove this
#include "../Mesh/Mesh.h"
#include "../Win32/Win32Application.h"

#include "../../Shaders/FilterIdentity_vs.h"
#include "../../Shaders/FilterIdentity_ps.h"


ForwardRenderer::ForwardRenderer()
{
	// TODO: hard code window size

	_swapChain = RHI::getInst().getDevice()->createSwapChain(Win32Application::GetHwnd(), DX11Limits::FRAME_COUNT, WIN_WIDTH, WIN_HEIGHT);
	_backbufferRT = std::make_shared<DX11RenderTarget>(_swapChain);

	_sceneRT = std::make_shared<DX11RenderTarget>(WIN_WIDTH, WIN_HEIGHT, 1, DXGI_FORMAT_R16G16B16A16_TYPELESS, DXGI_FORMAT_R16G16B16A16_UNORM, DXGI_FORMAT_R16G16B16A16_UNORM);
	_sceneDepthRT = std::make_shared<DX11DepthStencilRenderTarget>(WIN_WIDTH, WIN_HEIGHT, 1, DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D32_FLOAT);

	_shadowMapRT = std::make_shared<DX11DepthStencilRenderTarget>(WIN_WIDTH, WIN_HEIGHT, 1, DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D32_FLOAT);

	std::shared_ptr<DX11VertexShader> vs = std::make_shared<DX11VertexShader>(g_FilterIdentity_vs, sizeof(g_FilterIdentity_vs));
	std::shared_ptr<DX11PixelShader> ps = std::make_shared<DX11PixelShader>(g_FilterIdentity_ps, sizeof(g_FilterIdentity_ps));
	_filterIdentity = std::make_shared<Filter2D>(vs, ps);
}


ForwardRenderer::~ForwardRenderer()
{
}


void ForwardRenderer::beginFrame()
{
	RHI::getInst().getContext()->resetDefaultRenderStates(RHI::getInst().getRenderStateSet());

	RHI::getInst().getContext()->clear(_backbufferRT->getRenderTarget().Get(), 0, 0, 0, 0);
}


void ForwardRenderer::render(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport)
{
	std::shared_ptr<DX11GraphicContext> gfxContext = RHI::getInst().getContext();

	gfxContext->clear(_sceneRT->getRenderTarget().Get(), 0.f, 0.f, 0.f, 0.f);
	gfxContext->clear(_sceneDepthRT->getRenderTarget().Get(), DX11GraphicContext::RHI_CLEAR_FLAG::RHI_CLEAR_DEPTH, 1.0f);

	uint32_t x, y, w, h;
	viewport->getViewport(x, y, w, h);
	gfxContext->RSSetViewport(x, y, w, h);

	ID3D11RenderTargetView* rtvs[] = {_sceneRT->getRenderTarget().Get()};
	gfxContext->OMSetRenderTargets(1, rtvs, _sceneDepthRT->getRenderTarget().Get());

	auto models = scene->getModels();
	auto pointLights = scene->getPointLights(); 
	for (auto model : models)
	{
		auto mesh = model->getMesh();
		for (auto pl : pointLights)
		{
			mesh->draw(model->getWorldMatrix(), camera, pl);
		}
	}


	for (auto light : pointLights)
	{
		light->debugDraw(gfxContext, camera);
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

void ForwardRenderer::_renderShadowMapPass(std::shared_ptr<Scene> scene)
{
	auto gfxContext = RHI::getInst().getContext();

	// clear shadow map
	gfxContext->clear(_shadowMapRT->getRenderTarget().Get(), DX11GraphicContext::RHI_CLEAR_FLAG::RHI_CLEAR_DEPTH, 1.0);

	auto models = scene->getModels();
	auto pointLights = scene->getPointLights();

	for (auto model : models)
	{
		auto mesh = model->getMesh();
		for (auto light : pointLights)
		{
		}
	}
}
