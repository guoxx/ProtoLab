#pragma once

#include "tiny_obj_loader.h"

class Camera;
class Primitive;

class Mesh
{
public:

	Mesh();
	~Mesh();

	void draw(DirectX::CXMMATRIX mModel, const Camera* camera) const;

	void loadMeshFromFile(const wchar_t* objFileName);
	void loadShadersFromFile(const wchar_t* shaderFileName);

	void loadCoordinateSystemFrame();

private:
	void _drawDefault(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Primitive> prim) const;
	void _drawPerVertexColor(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Primitive> prim) const;

	std::shared_ptr<DX11VertexShader> _vertexShader{nullptr};
	std::shared_ptr<DX11PixelShader> _pixelShader{nullptr};
	std::vector<std::shared_ptr<Primitive>> _primitives{};
	ID3D11InputLayout* _vertexDecl{nullptr};
	// TODO: clean code
	ID3D11Buffer* _vsConstantsBufferPerVertexColor{nullptr};
	ID3D11Buffer* _psMaterielBufferPerVertexColor{nullptr};

	ID3D11Buffer* _ViewCB{nullptr};
	ID3D11Buffer* _MaterielCB{nullptr};
	ID3D11Buffer* _PointLightCB{nullptr};

	std::vector<tinyobj::shape_t> _shapes{};
	std::vector<tinyobj::material_t> _materiels{};
};

