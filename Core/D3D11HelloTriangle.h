#pragma once

#include "DXSample.h"

using namespace DirectX;

// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;

class D3D11HelloTriangle : public DXSample
{
public:
	D3D11HelloTriangle(UINT width, UINT height, std::wstring name);

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
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
	ComPtr<IDXGISwapChain2> m_swapChain;
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11Resource> m_renderTargets[FrameCount];
	UINT m_rtvDescriptorSize;

	// App resources.
	ComPtr<ID3D11Resource> m_vertexBuffer;

	// Synchronization objects.
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	UINT64 m_fenceValue;

	void LoadPipeline();
	void LoadAssets();
};
