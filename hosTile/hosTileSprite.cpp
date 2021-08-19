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
:	m_spriteFilename(spriteFilename),
	m_position(position)
{
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(deviceResources->GetD3DDevice(), spriteFilename.c_str(), nullptr, &m_texture)
	);

	// Store the width and height from the texture description.
	ID3D11Resource* resource = nullptr;
	m_texture->GetResource(&resource);
	ID3D11Texture2D* texture2D = nullptr;
	DX::ThrowIfFailed(resource->QueryInterface(&texture2D));
	D3D11_TEXTURE2D_DESC textureDescription;
	texture2D->GetDesc(&textureDescription);
	m_width = textureDescription.Width;
	m_height = textureDescription.Height;
	texture2D->Release();
	resource->Release();

	UpdateVertices();
}

hosTileSprite::~hosTileSprite()
{
	m_texture->Release();
}

const wstring hosTileSprite::GetSpriteFilename() const
{
	return m_spriteFilename;
}

ID3D11ShaderResourceView* hosTileSprite::GetTexture() const
{
	return m_texture;
}

void hosTileSprite::SetPosition(XMFLOAT3 position)
{
	m_position = position;
	UpdateVertices();
}

const VertexPositionTex* hosTileSprite::GetVertices() const
{
	return m_vertices;
}

// Update the vertices' data after the position has changed.
void hosTileSprite::UpdateVertices()
{
	// 0, bottom-left
	m_vertices[0] = {
		XMFLOAT3(m_width / 2.0f * -1.0f + m_position.x, m_height / 2.0f * -1.0f + m_position.y, m_position.z),
		XMFLOAT2(0.0f, 1.0f)
	};
	// 1, bottom-right
	m_vertices[1] = {
		XMFLOAT3(m_width / 2.0f + m_position.x, m_height / 2.0f * -1.0f + m_position.y, m_position.z),
		XMFLOAT2(1.0f, 1.0f)
	};
	// 2, top-right
	m_vertices[2] = {
		XMFLOAT3(m_width / 2.0f + m_position.x, m_height / 2.0f + m_position.y, m_position.z),
		XMFLOAT2(1.0f, 0.0f)
	};
	// 3, top-left
	m_vertices[3] = {
		XMFLOAT3(m_width / 2.0f * -1.0f + m_position.x, m_height / 2.0f + m_position.y, m_position.z),
		XMFLOAT2(0.0f, 0.0f)
	};
}
