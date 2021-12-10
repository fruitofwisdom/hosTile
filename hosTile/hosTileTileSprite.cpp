#include "pch.h"
#include "hosTileTileSprite.h"

using namespace DirectX;
using namespace hosTile;

hosTileTileSprite::hosTileTileSprite(
	const std::shared_ptr<hosTileTileset>& tileset, int tileNum,
	DirectX::XMFLOAT3 position)
:	hTSprite(position),
	m_tileset(tileset),
	m_tileNum(tileNum)
{
	m_width = m_tileset->GetTileWidth();
	m_height = m_tileset->GetTileHeight();

	UpdateVertices();
}

void hosTileTileSprite::Update()
{
	UpdateVertices();
}

ID3D11ShaderResourceView* hosTileTileSprite::GetTexture() const
{
	return m_tileset->GetTexture();
}

const VertexPositionTex* hosTileTileSprite::GetVertices() const
{
	return m_vertices;
}

// Update the vertices' data after the position has changed.
void hosTileTileSprite::UpdateVertices()
{
	// Calculate UV-coordinates and apply x-flip, y-flip, and scale.
	float uvTileWidth = (float)m_width / m_tileset->GetImageWidth();
	float uvTileHeight = (float)m_height / m_tileset->GetImageHeight();
	float uvXOffset = (float)m_tileset->GetTileXOffset(m_tileNum) / m_tileset->GetImageWidth();
	float uvYOffset = (float)m_tileset->GetTileYOffset(m_tileNum) / m_tileset->GetImageHeight();
	float uvLeft = m_xFlip ? uvXOffset + uvTileWidth : uvXOffset;
	float uvRight = m_xFlip ? uvXOffset : uvXOffset + uvTileWidth;
	float uvBottom = m_yFlip ? uvYOffset : uvYOffset + uvTileHeight;
	float uvTop = m_yFlip ? uvYOffset + uvTileHeight : uvYOffset;

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
