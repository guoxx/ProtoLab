#pragma once

#include "Actor.h"
#include "tiny_obj_loader.h"

class Camera;
class Primitive;

class Mesh : public Actor
{
public:
	Mesh();
	~Mesh();

	void draw(const Camera* camera) const;

	void loadMeshFromFile(const wchar_t* objFileName);
	void loadShadersFromFile(const wchar_t* shaderFileName);

private:

	struct TransformMatrixs
	{
		DirectX::XMFLOAT4X4 modelViewProjMatrix;
	};

	ID3D11VertexShader* _vertexShader{nullptr};
	ID3D11PixelShader* _pixelShader{nullptr};
	ID3D11InputLayout* _vertexDecl{nullptr};
	ID3D11Buffer* _vsConstantsBuffer{nullptr};
	TransformMatrixs _vsConstantsData{};
	std::vector<std::shared_ptr<Primitive>> _primitives{};

	std::vector<tinyobj::shape_t> _shapes{};
	std::vector<tinyobj::material_t> _materiels{};
};

