#include "pch.h"
#include "hosTileSprite.h"

#include <codecvt>
#include <locale>
#include "DDSTextureLoader.h"
#include "Other/DirectXHelper.h"

using namespace DirectX;
using namespace hosTile;
using namespace std;

hosTileSprite::hosTileSprite(
	const shared_ptr<DX::DeviceResources>& deviceResources, string spriteFilename,
	XMFLOAT3 position,
	unsigned int currentSubSprite, unsigned int numSubSprites)
:	m_spriteFilename(spriteFilename),
	m_position(position),
	m_currentSubSprite(currentSubSprite),
	m_numSubSprites(numSubSprites),
	m_scale(1.0f),
	m_xFlip(false),
	m_yFlip(false)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	wstring wideSpriteFilename = converter.from_bytes(spriteFilename);
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(deviceResources->GetD3DDevice(), wideSpriteFilename.c_str(), nullptr, &m_texture)
		);

	// Store the width and height from the texture description.
	ID3D11Resource* resource = nullptr;
	m_texture->GetResource(&resource);
	ID3D11Texture2D* texture2D = nullptr;
	DX::ThrowIfFailed(resource->QueryInterface(&texture2D));
	D3D11_TEXTURE2D_DESC textureDescription;
	texture2D->GetDesc(&textureDescription);
	m_width = textureDescription.Width / m_numSubSprites;
	m_height = textureDescription.Height;
	texture2D->Release();
	resource->Release();

	UpdateVertices();
}

const string hosTileSprite::GetSpriteFilename() const
{
	return m_spriteFilename;
}

ID3D11ShaderResourceView* hosTileSprite::GetTexture() const
{
	return m_texture.Get();
}

XMFLOAT3 hosTileSprite::GetPosition() const
{
	return m_position;
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

void hosTileSprite::SetCurrentSubSprite(unsigned int currentSubSprite)
{
	m_currentSubSprite = currentSubSprite;
	UpdateVertices();
}

void hosTileSprite::SetScale(float scale)
{
	m_scale = scale;
	UpdateVertices();
}

void hosTileSprite::SetXFlip(bool xFlip)
{
	m_xFlip = xFlip;
	UpdateVertices();
}

void hosTileSprite::SetYFlip(bool yFlip)
{
	m_yFlip = yFlip;
	UpdateVertices();
}

// Update the vertices' data after the position has changed.
void hosTileSprite::UpdateVertices()
{
	// Calculate UV-coordinates for sub-sprites as well as applying x- or y-flip.
	float uvWidth = (1.0f / (float)m_numSubSprites);
	float uvOffset = uvWidth * m_currentSubSprite;
	float uvLeft = m_xFlip ? uvOffset + uvWidth : uvOffset;
	float uvRight = m_xFlip ? uvOffset : uvOffset + uvWidth;
	float uvBottom = m_yFlip ? 0.0f : 1.0f;
	float uvTop = m_yFlip ? 1.0f : 0.0f;

	m_vertices[0] =		// 0, bottom-left
	{
		XMFLOAT3(
			(m_width / 2.0f * -1.0f) * m_scale + m_position.x,
			(m_height / 2.0f * -1.0f) * m_scale + m_position.y,
			m_position.z
			),
		XMFLOAT2(uvLeft, uvBottom)
	};
	m_vertices[1] =		// 1, bottom-right
	{
		XMFLOAT3(
			(m_width / 2.0f) * m_scale + m_position.x,
			(m_height / 2.0f * -1.0f) * m_scale + m_position.y,
			m_position.z
			),
		XMFLOAT2(uvRight, uvBottom)
	};
	m_vertices[2] =		// 2, top-right
	{
		XMFLOAT3(
			(m_width / 2.0f) * m_scale + m_position.x,
			(m_height / 2.0f) * m_scale + m_position.y,
			m_position.z
			),
		XMFLOAT2(uvRight, uvTop)
	};
	m_vertices[3] =		// 3, top-left
	{
		XMFLOAT3(
			(m_width / 2.0f * -1.0f) * m_scale + m_position.x,
			(m_height / 2.0f) * m_scale + m_position.y,
			m_position.z
			),
		XMFLOAT2(uvLeft, uvTop)
	};
}
