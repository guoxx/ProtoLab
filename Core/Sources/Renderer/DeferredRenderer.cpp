#include "stdafx.h"
#include "DeferredRenderer.h"
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
#include "../../Shaders/Lighting_ps.h"

namespace MaterialCB
{
	namespace Lighting
	{
		#include "../../Shaders/Lighting_common.h"
	}
}

DeferredRenderer::DeferredRenderer()
{
	// TODO: hard code window size

	_swapChain = RHI::getInstance().getDevice()->createSwapChain(Win32Application::GetHwnd(), DX11Limits::FRAME_COUNT, WIN_WIDTH, WIN_HEIGHT);
	_backbufferRT = std::make_shared<DX11RenderTarget>(_swapChain);

	_gbuffer_Albedo_MatId = std::make_shared<DX11RenderTarget>(WIN_WIDTH, WIN_HEIGHT, 1, DXGI_FORMAT_R8G8B8A8_TYPELESS, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM);
	_gbuffer_F0_Roughness = std::make_shared<DX11RenderTarget>(WIN_WIDTH, WIN_HEIGHT, 1, DXGI_FORMAT_R8G8B8A8_TYPELESS, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM);
	_gbuffer_Normal = std::make_shared<DX11RenderTarget>(WIN_WIDTH, WIN_HEIGHT, 1, DXGI_FORMAT_R8G8B8A8_TYPELESS, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM);

	_sceneRT = std::make_shared<DX11RenderTarget>(WIN_WIDTH, WIN_HEIGHT, 1, DXGI_FORMAT_R8G8B8A8_TYPELESS, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM);
	_sceneDepthRT = std::make_shared<DX11DepthStencilRenderTarget>(WIN_WIDTH, WIN_HEIGHT, 1, DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D32_FLOAT);

	{
		std::shared_ptr<DX11VertexShader> vs = std::make_shared<DX11VertexShader>(g_FilterIdentity_vs, sizeof(g_FilterIdentity_vs));
		std::shared_ptr<DX11PixelShader> ps = std::make_shared<DX11PixelShader>(g_FilterIdentity_ps, sizeof(g_FilterIdentity_ps));
		_filterIdentity = std::make_shared<Filter2D>(vs, ps);
	}

	{
		std::shared_ptr<DX11VertexShader> vs = std::make_shared<DX11VertexShader>(g_FilterIdentity_vs, sizeof(g_FilterIdentity_vs));
		std::shared_ptr<DX11PixelShader> ps = std::make_shared<DX11PixelShader>(g_Lighting_ps, sizeof(g_Lighting_ps));
		_filterLighting = std::make_shared<Filter2D>(vs, ps);

		_filterLightingViewBuffer = RHI::getInstance().getDevice()->createConstantBuffer(nullptr, sizeof(MaterialCB::Lighting::View));
		_filterLightintPointLightBuffer = RHI::getInstance().getDevice()->createConstantBuffer(nullptr, sizeof(MaterialCB::Lighting::PointLightParam));
	}
}


DeferredRenderer::~DeferredRenderer()
{
}


void DeferredRenderer::beginFrame()
{
	RHI::getInstance().beginFrame();

	RHI::getInstance().getContext()->resetDefaultRenderStates(RHI::getInstance().getRenderStateSet());

	RHI::getInstance().getContext()->clear(_backbufferRT->getRenderTarget().Get(), 0, 0, 0, 0);
}

void DeferredRenderer::render(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport)
{
	std::shared_ptr<DX11GraphicContext> gfxContext = RHI::getInstance().getContext();

	gfxContext->clear(_sceneRT->getRenderTarget().Get(), 0.f, 0.f, 0.f, 0.f);
	gfxContext->clear(_sceneDepthRT->getRenderTarget().Get(), DX11GraphicContext::RHI_CLEAR_FLAG::RHI_CLEAR_DEPTH, 1.0f);

	_renderShadowMap(scene);

	_renderGBuffer(gfxContext, camera, scene, viewport);
	_lighting(gfxContext, camera, scene, viewport);

	_filterIdentity->apply(_sceneRT, _backbufferRT);

	_renderDebugDisplay(gfxContext, camera, scene, viewport);
}

void DeferredRenderer::endFrame()
{
	RHI::getInstance().endFrame();
}

void DeferredRenderer::present()
{
	_swapChain->Present(0, 0);
}

void DeferredRenderer::_renderShadowMap(std::shared_ptr<Scene> scene)
{
	auto gfxContext = RHI::getInstance().getContext();

	GPU_MARKER(gfxContext.get(), ShadowMapPass);

	// clear shadow map
	auto models = scene->getModels();
	auto pointLights = scene->getPointLights();

	for (auto light : pointLights)
	{
		PointLight* pl = dynamic_cast<PointLight*>(light.get());

		if (pl != nullptr)
		{
			// setup shadow map render target
			DX11DepthStencilRenderTarget* shadowMapRT = pl->getShadowMapRenderTarget();
			gfxContext->clear(shadowMapRT->getRenderTarget().Get(), DX11GraphicContext::RHI_CLEAR_FLAG::RHI_CLEAR_DEPTH);
			gfxContext->OMSetRenderTargets(0, nullptr, shadowMapRT->getRenderTarget().Get());

			gfxContext->RSSetViewport(0, 0, 256, 256);

			// setup proj matrix
			DirectX::XMMATRIX mViewProj[PointLight::AXIS_END];
			for (int32_t axis = PointLight::AXIS_START; axis < PointLight::AXIS_END; ++axis)
			{
				mViewProj[axis] = pl->getViewProj(static_cast<PointLight::AXIS>(axis));
			}

			for (auto model : models)
			{
				auto mesh = model->getMesh();
				mesh->drawShadowMap(model->getWorldMatrix(), mViewProj);
			}
		}
	}
}

void DeferredRenderer::_renderGBuffer(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport)
{
	GPU_MARKER(gfxContext.get(), GBuffer);

	gfxContext->RSSetViewport(viewport.get());

	ID3D11RenderTargetView* rtvs[] = {
		_gbuffer_Albedo_MatId->getRenderTarget().Get(),
		_gbuffer_F0_Roughness->getRenderTarget().Get(),
		_gbuffer_Normal->getRenderTarget().Get()
	};
	gfxContext->OMSetRenderTargets(COUNT_OF_C_ARRAY(rtvs), rtvs, _sceneDepthRT->getRenderTarget().Get());

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
}

void DeferredRenderer::_lightsTileAssignment(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport)
{
}

void DeferredRenderer::_lighting(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport)
{
	GPU_MARKER(gfxContext.get(), Lighting);

	gfxContext->RSSetViewport(viewport.get());

	ID3D11RenderTargetView* rtvs[] = { _sceneRT->getRenderTarget().Get() };
	gfxContext->OMSetRenderTargets(COUNT_OF_C_ARRAY(rtvs), rtvs, nullptr);

	auto pointLights = scene->getPointLights();
	for (auto pl : pointLights)
	{

		{
			// update view constant buffer
			DX11ResourceMapGuard viewRes{ gfxContext.get(), _filterLightingViewBuffer.Get() };
			MaterialCB::Lighting::View* dataPtr = viewRes.getPtr<std::remove_pointer_t<decltype(dataPtr)>>();
			DirectX::XMMATRIX mProjInv = camera->getInvProjectionMatrix();
			DirectX::XMMATRIX mViewInv = camera->getInvViewMatrix();
			DirectX::XMStoreFloat4(&dataPtr->g_vCameraPosition, camera->getPosition());
			DirectX::XMStoreFloat4x4(&dataPtr->g_mProjInv, DirectX::XMMatrixTranspose(mProjInv));
			DirectX::XMStoreFloat4x4(&dataPtr->g_mViewInv, DirectX::XMMatrixTranspose(mViewInv));
		}

		{
			// update material constant buffer
			DX11ResourceMapGuard pointLightParamRes{ gfxContext.get(), _filterLightintPointLightBuffer.Get() };
			MaterialCB::Lighting::PointLightParam* pointLightParamPtr = pointLightParamRes.getPtr<std::remove_pointer_t<decltype(pointLightParamPtr)>>();
			DirectX::XMFLOAT3 intensity = pl->getIntensity();
			DirectX::XMVECTOR position = pl->getPosition();
			pointLightParamPtr->g_LightIntensity = DirectX::XMFLOAT4(intensity.x, intensity.y, intensity.z, 0.0f);
			DirectX::XMStoreFloat4(&pointLightParamPtr->g_LightPosition, position);
			pointLightParamPtr->g_RadiusStart = DirectX::XMFLOAT4(pl->getRadiusStart(), 0.0f, 0.0f, 0.0f);
			pointLightParamPtr->g_RadiusEnd = DirectX::XMFLOAT4(pl->getRadiusEnd(), 0.0f, 0.0f, 0.0f);
		}

		ID3D11Buffer* cbuffers[] = { _filterLightingViewBuffer.Get(), _filterLightintPointLightBuffer.Get() };
		gfxContext->PSSetConstantBuffers(0, 2, cbuffers);
		ID3D11ShaderResourceView* gbuffers[] = {
			_gbuffer_Albedo_MatId->getTextureSRV().Get(),
			_gbuffer_F0_Roughness->getTextureSRV().Get(),
			_gbuffer_Normal->getTextureSRV().Get(),
			_sceneDepthRT->getTextureSRV().Get()
		};
		gfxContext->PSSetShaderResources(0, COUNT_OF_C_ARRAY(gbuffers), gbuffers);
		_filterLighting->apply(_sceneRT);
	}
}

void DeferredRenderer::_renderDebugDisplay(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, std::shared_ptr<Viewport> viewport)
{
	GPU_MARKER(gfxContext.get(), DebugDisplay);

	auto pointLights = scene->getPointLights();
	for (auto light : pointLights)
	{
		light->debugDraw(gfxContext, camera);
	}
}
