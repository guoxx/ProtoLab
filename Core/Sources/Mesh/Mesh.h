#pragma once

#include "../Objects/Actor.h"
#include "tiny_obj_loader.h"

class Camera;
class Primitive;

// TODO: should not inherit from Actor
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
		DirectX::XMMATRIX modelViewProjMatrix;
	};

	struct MaterielProp
	{
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT4 specular;
		DirectX::XMFLOAT4 transmittance;
		DirectX::XMFLOAT4 emission;
		float shininess;
		float ior;      // index of refraction
		float dissolve; // 1 == opaque; 0 == fully transparent
		// illumination model (see http://www.fileformat.info/format/material/)
		float illum;
	};

	ID3D11VertexShader* _vertexShader{nullptr};
	ID3D11PixelShader* _pixelShader{nullptr};
	ID3D11InputLayout* _vertexDecl{nullptr};
	ID3D11Buffer* _vsConstantsBuffer{nullptr};
	ID3D11Buffer* _psMaterielBuffer{nullptr};
	std::vector<std::shared_ptr<Primitive>> _primitives{};

	std::vector<tinyobj::shape_t> _shapes{};
	std::vector<tinyobj::material_t> _materiels{};
};

