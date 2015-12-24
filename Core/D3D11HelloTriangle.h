#pragma once

#include "DXSample.h"

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

	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	// Pipeline objects.
	D3D11_VIEWPORT m_viewport;
	D3D11_RECT m_scissorRect;

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11Texture2D> m_renderTargets;
	ComPtr<ID3D11RenderTargetView> rtvHanble;

	// Synchronization objects.
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	UINT64 m_fenceValue;

	void LoadPipeline();
	void LoadAssets();
};
