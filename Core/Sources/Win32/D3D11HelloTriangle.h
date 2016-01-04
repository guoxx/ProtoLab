#pragma once

#include "DXSample.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Scene;
class Camera;
class Viewport;
class DeferredRenderer;

class D3D11HelloTriangle : public DXSample
{
public:
	D3D11HelloTriangle(UINT width, UINT height, std::wstring name);

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnPresent();
	virtual void OnDestroy();

private:

	void LoadPipeline();
	void LoadAssets();

private:

	std::shared_ptr<Viewport> _viewport;
	std::shared_ptr<Camera> _camera;
	std::shared_ptr<Scene> _scene;
	std::shared_ptr<DeferredRenderer> _renderer;
};
