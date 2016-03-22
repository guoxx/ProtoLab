#include "stdafx.h"
#include "D3D11HelloTriangle.h"
#include <DirectXTK/Inc/Keyboard.h>
#include <DirectXTK/Inc/Mouse.h>
#include "../Objects/Scene.h"
#include "../Objects/Model.h"
#include "../Objects/Camera.h"
#include "../Objects/PointLight.h"
#include "../Mesh/Mesh.h"
#include "../Renderer/DeferredRenderer.h"
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
	RHI::getInstance().initialize();
}

// Load the sample assets.
void D3D11HelloTriangle::LoadAssets()
{
	_viewport = std::make_shared<Viewport>();
	_viewport->setViewport(0, 0, (float)GetWidth(), (float)GetHeight());

	_camera = std::make_shared<Camera>();
	_camera->setPosition(DirectX::XMVECTOR{0, 0, 3});
	//_camera->lookAt(DirectX::XMVECTOR{3, 4, 5}, DirectX::XMVECTOR{0, 0, 0}, DirectX::XMVECTOR{0, 1, 0});
	_camera->setViewParams(54, m_width * 1.0f / m_height, 0.1f, 1000.0f);

	_scene = std::make_shared<Scene>();

	{
		// coordiante basis
		auto mod = std::make_shared<Model>();
		_scene->attachModel(mod);

		auto mesh = std::make_shared<Mesh>();
		mesh->loadCoordinateSystemFrame();
		mod->setMesh(mesh);
	}

	{
		// cornell box
		_cornellBox = std::make_shared<Model>();
		_scene->attachModel(_cornellBox);

		auto mesh = std::make_shared<Mesh>();
		mesh->loadMeshFromFile(GetAssetFullPath(L"CornellBox-Glossy.obj").c_str());
		_cornellBox->setMesh(mesh);
	}

	{
		// point light
		_pointLight = std::make_shared<PointLight>();
		_pointLight->setPosition(DirectX::XMVECTOR{0, 1, 0});
		_pointLight->setIntensity(DirectX::XMFLOAT3{3, 3, 3});
		_pointLight->setRadius(0.1f, 3.0f);
		_scene->attachPointLight(_pointLight);
	}

	_renderer = std::make_shared<DeferredRenderer>();
}

// Update frame-based values.
void D3D11HelloTriangle::OnUpdate()
{
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	float spd = 0.01f;

	bool moveLight = false;
	if (keyboardState.IsKeyDown(DirectX::Keyboard::L))
	{
		moveLight = true;
	}

	Actor* actorToMove = _camera.get();
	if (moveLight)
	{
		actorToMove = _pointLight.get();
	}

	if (keyboardState.IsKeyDown(DirectX::Keyboard::W))
	{
		actorToMove->move(actorToMove->getForward(), spd);
	}
	else if (keyboardState.IsKeyDown(DirectX::Keyboard::S))
	{
		actorToMove->move(actorToMove->getBackward(), spd);
	}
	else if (keyboardState.IsKeyDown(DirectX::Keyboard::A))
	{
		actorToMove->move(actorToMove->getRight(), spd);
	}
	else if (keyboardState.IsKeyDown(DirectX::Keyboard::D))
	{
		actorToMove->move(actorToMove->getLeft(), spd);
	}

	float mouseScale = 0.1f;
	if (mouseState.leftButton)
	{
		_camera->rotatePitchYawRoll(-mouseState.y*mouseScale, -mouseState.x*mouseScale, 0);
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
