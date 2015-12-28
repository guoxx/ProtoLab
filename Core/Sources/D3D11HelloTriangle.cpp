#include "stdafx.h"
#include "D3D11HelloTriangle.h"
#include "DX11RHI/DX11RHI.h"

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
	_camera->lookAt(DirectX::XMVECTOR{1, 1, 1}, DirectX::XMVECTOR{0, 0, 0}, DirectX::XMVECTOR{0, 1, 0});
	_camera->setViewParams(60, m_width * 1.0f / m_height, 0.1f, 1000.0f);

	_mesh = std::make_shared<Mesh>();
	_mesh->LoadMeshFromFile(GetAssetFullPath(L"CornellBox-Original.obj").c_str(), false);

	/*
	D3D11_BUFFER_DESC meshVexBufferDesc;	
	meshVexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	meshVexBufferDesc.ByteWidth = _mesh->_rawMeshData->vertices.size() * sizeof(decltype(_mesh->_rawMeshData->vertices.data()));
	meshVexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	meshVexBufferDesc.CPUAccessFlags = 0;
	meshVexBufferDesc.MiscFlags = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA meshVexBufferInitData;
	meshVexBufferInitData.pSysMem = _mesh->_rawMeshData->vertices.data();
	meshVexBufferInitData.SysMemPitch = 0;
	meshVexBufferInitData.SysMemSlicePitch = 0;

	m_device->CreateBuffer(&meshVexBufferDesc, &meshVexBufferInitData, &_meshVexBuffer);

	D3D11_BUFFER_DESC meshIndexBufferDesc;	
	meshIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	meshIndexBufferDesc.ByteWidth = _mesh->_rawMeshData->indices.size() * sizeof(decltype(_mesh->_rawMeshData->indices.data()));
	meshIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	meshIndexBufferDesc.CPUAccessFlags = 0;
	meshIndexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA meshIdxBufferInitData;
	meshIdxBufferInitData.pSysMem = _mesh->_rawMeshData->indices.data();
	meshIdxBufferInitData.SysMemPitch = 0;
	meshIdxBufferInitData.SysMemSlicePitch = 0;
	
	m_device->CreateBuffer(&meshIndexBufferDesc, &meshIdxBufferInitData, &_meshIdxBuffer);

	D3D11_BUFFER_DESC vsConstantBufferDesc;
	vsConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vsConstantBufferDesc.ByteWidth = sizeof(TransformMatrixs);
	vsConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vsConstantBufferDesc.CPUAccessFlags = 0;
	vsConstantBufferDesc.MiscFlags = 0;

	_transformMatrixsBuffer.modelViewProjMatrix = _camera->getViewProjectionMatrix();
	D3D11_SUBRESOURCE_DATA vsConstantBufferInitData;
	vsConstantBufferInitData.pSysMem = &_transformMatrixsBuffer;
	vsConstantBufferInitData.SysMemPitch = 0;
	vsConstantBufferInitData.SysMemSlicePitch = 0;

	m_device->CreateBuffer(&vsConstantBufferDesc, &vsConstantBufferInitData, &_vsConstantBuffer);
	*/

	loadTriangle();
}

// Update frame-based values.
void D3D11HelloTriangle::OnUpdate()
{
}

// Render the scene.
void D3D11HelloTriangle::OnRender()
{
	RHI::clear(RHI::getBackbufferRTV(), 1.0f, 1.0f, 1.0f, 1.0f);

	//ID3D11Buffer* buffers[] = {_meshVexBuffer, _meshVexBuffer, _meshVexBuffer};
	//uint32_t strides[] = {sizeof(XMFLOAT3)*2 + sizeof(XMFLOAT2), sizeof(XMFLOAT3)*2 + sizeof(XMFLOAT2), sizeof(XMFLOAT3)*2 + sizeof(XMFLOAT2)};
	//uint32_t offsets[] = {0, sizeof(XMFLOAT3), sizeof(XMFLOAT3)*2};

	drawTriangle();
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
		XMFLOAT3(-0.5, -0.5, 0), XMFLOAT3(1, 0, 0), XMFLOAT2(1, 0),
		XMFLOAT3( 0.5, -0.5, 0), XMFLOAT3(0, 1, 0), XMFLOAT2(0, 1),
		XMFLOAT3(-0.5,  0.5, 0), XMFLOAT3(0, 0, 1), XMFLOAT2(1, 1),
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
	//m_context->VSSetConstantBuffers(0, 1, &_vsConstantBuffer);
	RHI::_context->PSSetShader(_trianglePixelShader, 0, 0);
	RHI::_context->OMSetRenderTargets(1, rtvs, nullptr);
	RHI::_context->DrawIndexed(3, 0, 0);
}

void D3D11HelloTriangle::drawMesh()
{
}