#pragma once

class Primitive
{
public:

	enum class MATERIAL_ID
	{
		MATERIAL_VERTEX_COLOR,
		MATERIAL_EMISSIVE,
		MATERIAL_DEFAULT,
	};

	Primitive();
	~Primitive();

private:

	friend class Mesh;

	std::string				_name;
	uint32_t				_matIdx;
	MATERIAL_ID				_materialId{MATERIAL_ID::MATERIAL_DEFAULT};
	ComPtr<ID3D11Buffer>			_positionBuffer{nullptr};
	ComPtr<ID3D11Buffer>			_albedoBuffer{nullptr};
	ComPtr<ID3D11Buffer>			_normalBuffer{nullptr};
	ComPtr<ID3D11Buffer>			_texcoordBuffer{nullptr};
	ComPtr<ID3D11Buffer>			_indexBuffer{nullptr};
	// TODO: rename 
	ComPtr<ID3D11InputLayout>		_vertexDeclaration{nullptr};

	ComPtr<ID3D11Buffer>			_streamOutputBuffer{nullptr};

	D3D_PRIMITIVE_TOPOLOGY	_topology;
	DXGI_FORMAT				_indicesFormat;
	uint32_t				_indicesCount;
};

