#include "pch.h"
#include "hosTileSprite.h"

#include "DirectX/DDSTextureLoader11.h"
#include "DirectX/DirectXHelper.h"
#include "hosTileShaderStructures.h"
#include "Sample3DSceneRenderer.h"

using namespace DirectX;
using namespace hosTile;
using namespace std;

hosTileSprite::hosTileSprite(const shared_ptr<DX::DeviceResources>& deviceResources, wstring spriteFilename, XMFLOAT3 position)
:	m_deviceResources(deviceResources),
	m_spriteFilename(spriteFilename),
	m_position(position)
{
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), spriteFilename.c_str(), nullptr, &m_texture)
	);

	ID3D11Resource* resource = nullptr;
	m_texture->GetResource(&resource);
	ID3D11Texture2D* texture2D = nullptr;
	DX::ThrowIfFailed(resource->QueryInterface(&texture2D));
	D3D11_TEXTURE2D_DESC textureDescription;
	texture2D->GetDesc(&textureDescription);
	m_dimensions.x = static_cast<float>(textureDescription.Width);
	m_dimensions.y = static_cast<float>(textureDescription.Height);
	texture2D->Release();
	resource->Release();

	UpdateVertices();
}

hosTileSprite::~hosTileSprite()
{
	m_texture->Release();
}

void hosTileSprite::Update()
{
	UpdateVertices();
}

const wstring hosTileSprite::GetSpriteFilename() const
{
	return m_spriteFilename;
}

ID3D11ShaderResourceView* hosTileSprite::GetTexture() const
{
	return m_texture;
}

const VertexPositionTex* hosTileSprite::GetVertices() const
{
	return m_vertices;
}

void hosTileSprite::UpdateVertices()
{
	float width = m_dimensions.x;
	float height = m_dimensions.y;

	// Load the vertices. Each vertex has a position and a texture coordinate.
	m_vertices[0] = { XMFLOAT3(width / 2 * -1 + m_position.x, height / 2 * -1 + m_position.y, m_position.z),
		XMFLOAT2(0.0f, 1.0f) };		// 0, bottom-left
	m_vertices[1] = { XMFLOAT3(width / 2 + m_position.x, height / 2 * -1 + m_position.y, m_position.z),
		XMFLOAT2(1.0f, 1.0f) };		// 1, bottom-right
	m_vertices[2] = { XMFLOAT3(width / 2 + m_position.x, height / 2 + m_position.y, m_position.z),
		XMFLOAT2(1.0f, 0.0f) };		// 2, top-right
	m_vertices[3] = { XMFLOAT3(width / 2 * -1 + m_position.x, height / 2 + m_position.y, m_position.z),
		XMFLOAT2(0.0f, 0.0f) };		// 3, top-left
}
