#include "stdafx.h"
#include "D3D11HelloTriangle.h"
#include <DirectXTK/Inc/Keyboard.h>
#include <DirectXTK/Inc/Mouse.h>
#include "../Objects/Scene.h"
#include "../Objects/Model.h"
#include "../Objects/Camera.h"
#include "../Mesh/Mesh.h"
#include "../Renderer/ForwardRenderer.h"
#include "../Renderer/Viewport.h"

D3D11HelloTriangle::D3D11HelloTriangle(UINT width, UINT height, std::wstring name) :
	DXSample(width, height, name)
{
}

void D3D11HelloTriangle::OnInit()
{
	LoadPipeline();
	LoadAssets();
}

// Load the rendering pipeline dependencies.
void D3D11HelloTriangle::LoadPipeline()
{
	RHI::getInst().initialize();
}

// Load the sample assets.
void D3D11HelloTriangle::LoadAssets()
{
	_viewport = std::make_shared<Viewport>();
	_viewport->setViewport(uint32_t{0}, uint32_t{0}, GetWidth(), GetHeight());

	_camera = std::make_shared<Camera>();
	_camera->setPosition(DirectX::XMVECTOR{0, 0, 3});
	//_camera->lookAt(DirectX::XMVECTOR{3, 4, 5}, DirectX::XMVECTOR{0, 0, 0}, DirectX::XMVECTOR{0, 1, 0});
	_camera->setViewParams(54, m_width * 1.0f / m_height, 0.1f, 1000.0f);

	_scene = std::make_shared<Scene>();

	{
		// cornell box
		_cornellBox = std::make_shared<Model>();
		_scene->attachModel(_cornellBox);

		auto mesh = std::make_shared<Mesh>();
		mesh->loadMeshFromFile(GetAssetFullPath(L"CornellBox-Original.obj").c_str());
		mesh->loadShadersFromFile(GetAssetFullPath(L"hello_mesh.hlsl").c_str());
		_cornellBox->setMesh(mesh);
	}

	{
		// coordiante basis
		auto mod = std::make_shared<Model>();
		_scene->attachModel(mod);

		auto mesh = std::make_shared<Mesh>();
		mesh->loadCoordinateSystemFrame();
		mod->setMesh(mesh);
	}

	_renderer = std::make_shared<ForwardRenderer>();
}

// Update frame-based values.
void D3D11HelloTriangle::OnUpdate()
{
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	if (keyboardState.IsKeyDown(DirectX::Keyboard::W))
	{
		_camera->move(_camera->getForward(), 0.01f);
	}
	else if (keyboardState.IsKeyDown(DirectX::Keyboard::S))
	{
		_camera->move(_camera->getBackward(), 0.01f);
	}
	else if (keyboardState.IsKeyDown(DirectX::Keyboard::A))
	{
		_camera->move(_camera->getRight(), 0.01f);
	}
	else if (keyboardState.IsKeyDown(DirectX::Keyboard::D))
	{
		_camera->move(_camera->getLeft(), 0.01f);
	}

	if (mouseState.leftButton)
	{
		_camera->rotatePitchYawRoll(-mouseState.y*0.1f, -mouseState.x*0.1f, 0);
	}

	_scene->update(0.0f);
}

// Render the scene.
void D3D11HelloTriangle::OnRender()
{
	_renderer->beginFrame();
	_renderer->render(_camera, _scene, _viewport);
	_renderer->endFrame();
}

void D3D11HelloTriangle::OnPresent()
{
	_renderer->present();
}


void D3D11HelloTriangle::OnDestroy()
{
	// Ensure that the GPU is no longer referencing resources that are about to be
	// cleaned up by the destructor.
	// TODO: clean up resources
}
