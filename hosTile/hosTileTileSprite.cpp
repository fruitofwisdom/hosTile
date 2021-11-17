#include "pch.h"
#include "hosTileTileSprite.h"

using namespace DirectX;
using namespace hosTile;

hosTileTileSprite::hosTileTileSprite(
	const std::shared_ptr<hosTileTileset>& tileset, int tileNum,
	DirectX::XMFLOAT3 position)
:	m_tileset(tileset),
	m_tileNum(tileNum),
	m_position(position)
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
	// TODO: Calculate UV-coordinates for tiles, etc.
	/*
	float uvWidth = (1.0f / (float)m_tileset->NumTiles());
	float uvOffset = uvWidth * m_tileNum;
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
	*/

	m_vertices[0] =		// 0, bottom-left
	{
		XMFLOAT3(-8.0f, -8.0f, m_position.z),
		XMFLOAT2(0.0f, 1.0f)
	};
	m_vertices[1] =		// 1, bottom-right
	{
		XMFLOAT3(8.0f, -8.0f, m_position.z),
		XMFLOAT2(1.0f, 1.0f)
	};
	m_vertices[2] =		// 2, top-right
	{
		XMFLOAT3(8.0f, 8.0f, m_position.z),
		XMFLOAT2(1.0f, 0.0f)
	};
	m_vertices[3] =		// 3, top-left
	{
		XMFLOAT3(-8.0f, 8.0f, m_position.z),
		XMFLOAT2(0.0f, 0.0f)
	};
}
