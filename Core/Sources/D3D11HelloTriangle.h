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

	void loadTriangle();
	void loadMesh();

	void updateCamera();

	void drawTriangle();
	void drawMesh();

	struct TransformMatrixs
	{
		XMMATRIX modelViewProjMatrix;
	};

	std::shared_ptr<Camera> _camera;

	TransformMatrixs _vsConstantsData;
	ID3D11Buffer* _vsConstantsBuffer;

	// resources for mesh
	std::shared_ptr<Mesh> _mesh;

	// resources for triangle 
	ID3D11Buffer* _triangleVertexBuffer;
	ID3D11Buffer* _triangleIndexBuffer;
	ID3D11InputLayout* _triangleVertexLayoutBuffer;

	ID3D11VertexShader* _triangleVertexShader;
	ID3D11PixelShader* _trianglePixelShader;

	void LoadPipeline();
	void LoadAssets();
};
