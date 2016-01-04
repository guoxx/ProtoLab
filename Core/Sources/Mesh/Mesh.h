#pragma once

#include "tiny_obj_loader.h"

class Camera;
class Primitive;

class Mesh
{
public:

	Mesh();
	~Mesh();

	void draw(const Camera* camera) const;

	void loadMeshFromFile(const wchar_t* objFileName);
	void loadShadersFromFile(const wchar_t* shaderFileName);

	void loadCoordinateSystemFrame();

private:

	ID3D11VertexShader* _vertexShader{nullptr};
	ID3D11PixelShader* _pixelShader{nullptr};
	ID3D11InputLayout* _vertexDecl{nullptr};
	ID3D11Buffer* _vsConstantsBuffer{nullptr};
	ID3D11Buffer* _psMaterielBuffer{nullptr};
	std::vector<std::shared_ptr<Primitive>> _primitives{};

	std::vector<tinyobj::shape_t> _shapes{};
	std::vector<tinyobj::material_t> _materiels{};
};

