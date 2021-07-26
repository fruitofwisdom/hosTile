#include "pch.h"
#include "hosTileSprite.h"

#include "DirectX/DDSTextureLoader11.h"
#include "DirectX/DirectXHelper.h"
#include "hosTileShaderStructures.h"
#include "Sample3DSceneRenderer.h"

using namespace DirectX;
using namespace hosTile;

hosTileSprite::hosTileSprite(const std::shared_ptr<DX::DeviceResources>& deviceResources, std::string spriteFilename, XMFLOAT3 position)
:	m_position(position)
{
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(deviceResources->GetD3DDevice(), L"Assets/NES - Final Fantasy - Castle Corneria.dds", nullptr, &m_texture)
	);

	// Load mesh vertices. Each vertex has a position and a texture coordinate.
	float width = 544.0f;
	float height = 544.0f;
	static const VertexPositionTex meshVertices[] =
	{
		{XMFLOAT3(width / 2 * -1, height / 2 * -1, 0.0f), XMFLOAT2(0.0f, 1.0f)},	// 0, bottom-left
		{XMFLOAT3(width / 2, height / 2 * -1, 0.0f), XMFLOAT2(1.0f, 1.0f)},	// 1, bottom-right
		{XMFLOAT3(width / 2, height / 2, 0.0f), XMFLOAT2(1.0f, 0.0f)},		// 2, top-right
		{XMFLOAT3(width / 2 * -1, height / 2, 0.0f), XMFLOAT2(0.0f, 0.0f)},	// 3, top-left
	};

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = meshVertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(meshVertices), D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	DX::ThrowIfFailed(
		deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&m_vertexBuffer
		)
	);
}

hosTileSprite::~hosTileSprite()
{
	m_vertexBuffer->Release();
	m_texture->Release();
}

ID3D11Buffer* hosTileSprite::GetVertexBuffer() const
{
	return m_vertexBuffer;
}

ID3D11ShaderResourceView* hosTileSprite::GetTexture() const
{
	return m_texture;
}
