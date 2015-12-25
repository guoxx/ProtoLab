#pragma once

#include "DXSample.h"
#include "Mesh.h"
#include "Camera.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

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
	static const UINT FrameCount = 2;

	// Pipeline objects.
	D3D11_VIEWPORT m_viewport;
	D3D11_RECT m_scissorRect;

	std::shared_ptr<Camera> _camera;
	std::shared_ptr<Mesh> _dragonMesh;

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11Texture2D> m_renderTargets;
	ComPtr<ID3D11RenderTargetView> rtvHanble;

	ID3D11Buffer* _vbuffer;
	ID3D11Buffer* _ibuffer;
	ID3D11InputLayout* _vlayout;

	ID3D11VertexShader* _vexShader;
	ID3D11PixelShader* _pixShader;

	// Synchronization objects.
	UINT m_frameIndex;

	void LoadPipeline();
	void LoadAssets();
};
