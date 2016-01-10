#pragma once

#include "tiny_obj_loader.h"

class Camera;
class Primitive;
class PointLight;
class Material;

class Mesh
{
public:

	Mesh();
	~Mesh();

	static std::shared_ptr<Mesh> createSphere(float diameter = 1, size_t tessellation = 16, bool rhcoords = true, bool invertn = false);

	std::shared_ptr<Material> getMaterial() const;

	void draw(DirectX::CXMMATRIX mModel, std::shared_ptr<Camera> camera, std::shared_ptr<PointLight> pointLight) const;

	void loadMeshFromFile(const wchar_t* objFileName);
	void loadCoordinateSystemFrame();

private:
	void _drawBaseMaterial(DirectX::CXMMATRIX mModel, std::shared_ptr<Camera> camera, std::shared_ptr<PointLight> pointLight, std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Primitive> prim) const;
	void _drawPerVertexColor(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Primitive> prim, DirectX::CXMMATRIX mModel, std::shared_ptr<Camera> camera) const;
	void _drawEmissive(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Primitive> prim, DirectX::CXMMATRIX mModel, std::shared_ptr<Camera> camera) const;

	std::vector<std::shared_ptr<Primitive>> _primitives{};
	std::shared_ptr<Material> _material;

	std::vector<tinyobj::shape_t> _shapes{};
	std::vector<tinyobj::material_t> _materiels{};
};

