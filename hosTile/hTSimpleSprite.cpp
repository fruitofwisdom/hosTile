#include "pch.h"
#include "hTSimpleSprite.h"

#include <codecvt>
#include <locale>
#include "DDSTextureLoader.h"
#include "Other/DirectXHelper.h"

using namespace DirectX;
using namespace hosTile;
using namespace std;

hTSimpleSprite::hTSimpleSprite(
	const shared_ptr<DX::DeviceResources>& deviceResources, string spriteFilename,
	XMFLOAT3 position)
:	hTSprite(position),
	m_spriteFilename(spriteFilename)
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
	m_width = textureDescription.Width;
	m_height = textureDescription.Height;
	texture2D->Release();
	resource->Release();

	UpdateVertices();
}

void hTSimpleSprite::Update()
{
	UpdateVertices();
}

ID3D11ShaderResourceView* hTSimpleSprite::GetTexture() const
{
	return m_texture.Get();
}

const VertexPositionTex* hTSimpleSprite::GetVertices() const
{
	return m_vertices;
}

// Update the vertices' data after the position has changed.
void hTSimpleSprite::UpdateVertices()
{
	// Calculate UV-coordinates and apply x-flip, y-flip, and scale.
	float uvLeft = m_xFlip ? 1.0f : 0.0f;
	float uvRight = m_xFlip ? 0.0f : 1.0f;
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
