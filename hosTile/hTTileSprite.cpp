#include "pch.h"
#include "hTTileSprite.h"

#include "hTRenderer.h"

using namespace DirectX;
using namespace hosTile;

hTTileSprite::hTTileSprite(
	const hTTileset* tileset, unsigned int tileNum,
	DirectX::XMFLOAT3 position)
:	hTSprite(position),
	m_tileset(tileset)
{
	m_width = m_tileset->GetTileWidth();
	m_height = m_tileset->GetTileHeight();

	// The top three bits are for flipping flags and the tiles are 1-indexed.
	m_tileNum = (tileNum & 0x1FFFFFFF) - 1;
	m_xFlip = tileNum & 0x80000000;
	m_yFlip = tileNum & 0x40000000;

	UpdateVertices();
}

void hTTileSprite::Render(hTRenderer& renderer)
{
	UpdateVertices();
	renderer.AddSprite(this);
}

ID3D11ShaderResourceView* hTTileSprite::GetTexture() const
{
	return m_tileset->GetTexture();
}

unsigned int hTTileSprite::GetNumVertices() const
{
	return sizeof(m_vertices) / sizeof(m_vertices[0]);
}

const VertexPositionTex* hTTileSprite::GetVertices() const
{
	return m_vertices;
}

// Update the vertices' data after the position has changed.
void hTTileSprite::UpdateVertices()
{
	// Calculate UV-coordinates.
	float uvTileWidth = (float)m_width / m_tileset->GetImageWidth();
	float uvTileHeight = (float)m_height / m_tileset->GetImageHeight();
	float uvXOffset = (float)m_tileset->GetTileXOffset(m_tileNum) / m_tileset->GetImageWidth();
	float uvYOffset = (float)m_tileset->GetTileYOffset(m_tileNum) / m_tileset->GetImageHeight();
	float uvLeft = uvXOffset;
	float uvRight = uvXOffset + uvTileWidth;
	float uvBottom = uvYOffset + uvTileHeight;
	float uvTop = uvYOffset;

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

	// Apply x-flip and y-flip.
	if (m_xFlip)
	{
		SwapUVs(m_vertices[0].tex, m_vertices[1].tex);
		SwapUVs(m_vertices[2].tex, m_vertices[3].tex);
	}
	if (m_yFlip)
	{
		SwapUVs(m_vertices[0].tex, m_vertices[3].tex);
		SwapUVs(m_vertices[1].tex, m_vertices[2].tex);
	}
}
