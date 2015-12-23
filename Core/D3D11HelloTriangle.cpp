#include "stdafx.h"
#include "D3D11HelloTriangle.h"

D3D11HelloTriangle::D3D11HelloTriangle(UINT width, UINT height, std::wstring name) :
	DXSample(width, height, name),
	m_frameIndex(0),
	m_viewport(),
	m_scissorRect(),
	m_rtvDescriptorSize(0)
{
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);
	m_viewport.MaxDepth = 1.0f;

	m_scissorRect.right = static_cast<LONG>(width);
	m_scissorRect.bottom = static_cast<LONG>(height);
}

void D3D11HelloTriangle::OnInit()
{
	LoadPipeline();
	LoadAssets();
}

// Load the rendering pipeline dependencies.
void D3D11HelloTriangle::LoadPipeline()
{
}

// Load the sample assets.
void D3D11HelloTriangle::LoadAssets()
{
}

// Update frame-based values.
void D3D11HelloTriangle::OnUpdate()
{
}

// Render the scene.
void D3D11HelloTriangle::OnRender()
{
}

void D3D11HelloTriangle::OnDestroy()
{
	// Ensure that the GPU is no longer referencing resources that are about to be
	// cleaned up by the destructor.

	CloseHandle(m_fenceEvent);
}

