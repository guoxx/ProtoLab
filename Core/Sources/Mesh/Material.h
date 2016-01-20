#pragma once

namespace MaterialCB
{
	namespace PerVertexColor
	{
	#include "../../Shaders/PerVertexColor_common.h"
	}

	namespace BaseMaterial
	{
	#include "../../Shaders/BaseMaterial_common.h"
	}

	namespace EmissiveMaterial
	{
	#include "../../Shaders/EmissiveMaterial_common.h"
	}

	namespace ShadowMapMaterial
	{
	#include "../../Shaders/ShadowMapMaterial_common.h"
	}
}

class DX11GraphicContext;

class Material
{
public:
	enum VEX_INPUT_SLOT
	{
		POSITION = 0,
		BLENDWEIGHT,
		NORMAL,
		COLOR0,
		COLOR1,
		TESSFACTOR,
		PSIZE,
		BLENDINDICES,
		TEXCOORD0,
		TEXCOORD1,
		TEXCOORD2,
		TEXCOORD3,
		TEXCOORD4,
		TEXCOORD5,
		TEXCOORD6,
		TEXCOORD7,
		VEX_INPUT_SLOT_NUM,

		DIFFUSE = COLOR0,
		SPECULAR = COLOR1,
		FOGCOORD = TESSFACTOR,
		TANGENT = TEXCOORD6,
		BINORMAL = TEXCOORD7,
	};

	Material();
	virtual ~Material();

	static std::shared_ptr<Material> createMaterialBase();
	static std::shared_ptr<Material> createMaterialPerVeretxColor();
	static std::shared_ptr<Material> createMaterialEmissive();
	static std::shared_ptr<Material> createMaterialShadowMap();

	void initialize(std::shared_ptr<DX11VertexShader> vertShader, std::shared_ptr<DX11PixelShader> fragShader, const D3D11_INPUT_ELEMENT_DESC* desc, uint32_t descElemCnt);

	void createVsConstantsBuffer(const void* memPtr, uint32_t memSize, uint32_t reg);
	void createPsConstantsBuffer(const void* memPtr, uint32_t memSize, uint32_t reg);

	ComPtr<ID3D11Buffer> getVsConstantBuffer(uint32_t reg);
	ComPtr<ID3D11Buffer> getPsConstantBuffer(uint32_t reg);

	void setVertexBuffer(VEX_INPUT_SLOT slot, ID3D11Buffer* buffer, uint32_t stride, uint32_t offset);
	void unsetVertexBuffers();
	void apply(std::shared_ptr<DX11GraphicContext> gfxContext);

protected:
	class VertBufferDesc
	{
	public:
		ComPtr<ID3D11Buffer>	_buffer{nullptr};
		uint32_t				_stride{0};
		uint32_t				_offset{0};
		bool					_used{false};
	};

	class ConstantBufferDesc
	{
	public:
		ComPtr<ID3D11Buffer>	_buffer{nullptr};
		bool					_used{false};
	};

	// TODO: hard code the size for the moment
	std::array<ConstantBufferDesc, 32> _vsConstantBuffers;
	std::array<ConstantBufferDesc, 32> _psConstantBuffers;
	std::array<VertBufferDesc, VEX_INPUT_SLOT_NUM> _vertexBuffers;

	std::shared_ptr<DX11VertexShader> _vertShader;
	std::shared_ptr<DX11PixelShader> _fragShader;
	ComPtr<ID3D11InputLayout> _inputLayout;
};

