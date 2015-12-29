#include "stdafx.h"
#include "D3D11HelloTriangle.h"

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
	RHI::initialize(GetWidth(), GetHeight());	
}

// Load the sample assets.
void D3D11HelloTriangle::LoadAssets()
{
	_camera = std::make_shared<Camera>();

	_camera->lookAt(DirectX::XMVECTOR{3, 4, 5}, DirectX::XMVECTOR{0, 0, 0}, DirectX::XMVECTOR{0, 1, 0});
	_camera->setViewParams(90, m_width * 1.0f / m_height, 0.1f, 1000.0f);

	_vsConstantsBuffer = RHI::createConstantBuffer(&_vsConstantsData, sizeof(_vsConstantsData));

	loadTriangle();
	loadMesh();
}

// Update frame-based values.
void D3D11HelloTriangle::OnUpdate()
{
}

// Render the scene.
void D3D11HelloTriangle::OnRender()
{
	RHI::clear(RHI::getBackbufferRTV(), 1.0f, 1.0f, 1.0f, 1.0f);

	updateCamera();
	//drawTriangle();
	drawMesh();
}

void D3D11HelloTriangle::OnPresent()
{
	RHI::present();
}

void D3D11HelloTriangle::OnDestroy()
{
	// Ensure that the GPU is no longer referencing resources that are about to be
	// cleaned up by the destructor.
	// TODO: clean up resources
}

void D3D11HelloTriangle::loadTriangle()
{
	struct SimpleVertex
	{
		XMFLOAT3 position;
		XMFLOAT3 color;
		XMFLOAT2 texCoord;
	};
	
	static SimpleVertex vertexDatas[3]= {
		XMFLOAT3(-0.5, 0, 0), XMFLOAT3(1, 0, 0), XMFLOAT2(1, 0),
		XMFLOAT3( 0.5, 0, 0), XMFLOAT3(0, 1, 0), XMFLOAT2(0, 1),
		XMFLOAT3(0, 0, 0.5), XMFLOAT3(0, 0, 1), XMFLOAT2(1, 1),
	};

	_triangleVertexBuffer = RHI::createVertexBuffer(vertexDatas, sizeof(vertexDatas));

	static uint32_t indexDatas[] = {0, 2, 1};
	_triangleIndexBuffer = RHI::createIndexBuffer(indexDatas, sizeof(indexDatas));

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3)*2, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	_triangleVertexShader = RHI::createVertexShaderFromFile(GetAssetFullPath(L"hello_triangle.hlsl").c_str(),
												"VSMain",
												inputDesc,
												sizeof(inputDesc)/sizeof(inputDesc[0]),
												_triangleVertexLayoutBuffer);
	_trianglePixelShader = RHI::createPixelShaderFromFile(GetAssetFullPath(L"hello_triangle.hlsl").c_str(), "PSMain");
}

void D3D11HelloTriangle::loadMesh()
{
	_mesh = std::make_shared<Mesh>();
	_mesh->loadMeshFromFile(GetAssetFullPath(L"CornellBox-Original.obj").c_str());
	_mesh->loadShadersFromFile(GetAssetFullPath(L"hello_mesh.hlsl").c_str());
}


void D3D11HelloTriangle::updateCamera()
{
	/*
	static float roll = 0;
	//roll += 0.01f;
	static float pitch = 0;
	//pitch += 0.01f;
	static float yaw = 0;
	yaw += 0.01f;
	#define RADIANS(angle) (angle/180.0f*3.14f)

	// TODO: update constants buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	RHI::_context->Map(_vsConstantsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	TransformMatrixs* dataPtr = (TransformMatrixs*)mappedResource.pData;
	DirectX::XMMATRIX transMatrix = DirectX::XMMatrixTranslation(1, 0, 0);
	DirectX::XMMATRIX rotateMatrix = DirectX::XMMatrixRotationRollPitchYaw(RADIANS(roll), RADIANS(pitch), RADIANS(yaw));
	DirectX::XMMATRIX finalMatrix = DirectX::XMMatrixMultiply(transMatrix, rotateMatrix);
	dataPtr->modelViewProjMatrix = finalMatrix;
	RHI::_context->Unmap(_vsConstantsBuffer, 0);
	*/

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	RHI::_context->Map(_vsConstantsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	TransformMatrixs* dataPtr = (TransformMatrixs*)mappedResource.pData;
	dataPtr->modelViewProjMatrix = DirectX::XMMatrixTranspose(_camera->getViewProjectionMatrix());
	RHI::_context->Unmap(_vsConstantsBuffer, 0);
}

void D3D11HelloTriangle::drawTriangle()
{

	ID3D11Buffer* buffers[] = {_triangleVertexBuffer, _triangleVertexBuffer, _triangleVertexBuffer};
	uint32_t strides[] = {sizeof(XMFLOAT3)*2+sizeof(XMFLOAT2), sizeof(XMFLOAT3)*2+sizeof(XMFLOAT2), sizeof(XMFLOAT3)*2+sizeof(XMFLOAT2)};
	uint32_t offsets[] = {0, sizeof(XMFLOAT3), sizeof(XMFLOAT3)*2};
	ID3D11RenderTargetView* rtvs[] = {RHI::getBackbufferRTV()};

	RHI::setDefaultRHIStates();	
	RHI::setViewport(0, 0, GetWidth(), GetHeight());

	RHI::_context->IASetInputLayout(_triangleVertexLayoutBuffer);
	RHI::_context->IASetVertexBuffers(0, 3, buffers, strides, offsets);
	RHI::_context->IASetIndexBuffer(_triangleIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	RHI::_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	RHI::_context->VSSetShader(_triangleVertexShader, 0, 0);
	RHI::_context->VSSetConstantBuffers(0, 1, &_vsConstantsBuffer);
	RHI::_context->PSSetShader(_trianglePixelShader, 0, 0);
	RHI::_context->OMSetRenderTargets(1, rtvs, nullptr);
	RHI::_context->DrawIndexed(3, 0, 0);
}

void D3D11HelloTriangle::drawMesh()
{
	RHI::setDefaultRHIStates();	
	RHI::setViewport(0, 0, GetWidth(), GetHeight());

	ID3D11RenderTargetView* rtvs[] = {RHI::getBackbufferRTV()};
	RHI::_context->OMSetRenderTargets(1, rtvs, nullptr);

	_mesh->draw(_camera.get());
}
