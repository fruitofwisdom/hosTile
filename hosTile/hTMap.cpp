#include "pch.h"
#include "hTMap.h"

#include "Other/json.hpp"

using namespace DirectX;
using namespace hosTile;

hTMap::hTMap(
	const std::shared_ptr<hTTileset>& tileset, const nlohmann::json& mapJson,
	DirectX::XMFLOAT3 position)
:	hTSprite(position),
	m_tileset(tileset)
{
	for (unsigned int gid : mapJson["layers"][0]["data"])
	{
		m_mapData.push_back(gid);
		m_vertices.push_back({XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)});
		m_vertices.push_back({XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)});
		m_vertices.push_back({XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)});
		m_vertices.push_back({XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)});
	}
	m_mapWidth = mapJson["layers"][0]["width"];
	m_mapHeight = mapJson["layers"][0]["height"];
	m_width = m_mapWidth * m_tileset->GetTileWidth();
	m_height = m_mapHeight * m_tileset->GetTileHeight();

	UpdateVertices();
}

void hTMap::Update()
{
	UpdateVertices();
}

ID3D11ShaderResourceView* hTMap::GetTexture() const
{
	return m_tileset->GetTexture();
}

unsigned int hTMap::GetNumVertices() const
{
	return m_mapData.size() * 4;
}

const VertexPositionTex* hTMap::GetVertices() const
{
	return &m_vertices[0];
}

void hTMap::UpdateVertices()
{
	for (int y = 0; y < m_mapHeight; ++y)
	{
		for (int x = 0; x < m_mapWidth; ++x)
		{
			unsigned int gid = m_mapData[y * m_mapWidth + x];
			// The top three bits are for flipping flags and the tiles are 1-indexed.
			gid &= 0x1FFFFFFF;
			gid--;
			float xPosition = (float)x * m_tileset->GetTileWidth()
				+ m_tileset->GetTileWidth() / 2
				- (m_mapWidth * m_tileset->GetTileWidth() / 2);
			float yPosition = (float)y * m_tileset->GetTileHeight() * -1.0f
				- m_tileset->GetTileHeight() / 2
				+ (m_mapHeight * m_tileset->GetTileHeight() / 2);

			// Calculate UV-coordinates and apply x-flip, y-flip, and scale.
			float uvTileWidth = (float)m_tileset->GetTileWidth() / m_tileset->GetImageWidth();
			float uvTileHeight = (float)m_tileset->GetTileHeight() / m_tileset->GetImageHeight();
			float uvXOffset = (float)m_tileset->GetTileXOffset(gid) / m_tileset->GetImageWidth();
			float uvYOffset = (float)m_tileset->GetTileYOffset(gid) / m_tileset->GetImageHeight();
			float uvLeft = m_xFlip ? uvXOffset + uvTileWidth : uvXOffset;
			float uvRight = m_xFlip ? uvXOffset : uvXOffset + uvTileWidth;
			float uvBottom = m_yFlip ? uvYOffset : uvYOffset + uvTileHeight;
			float uvTop = m_yFlip ? uvYOffset + uvTileHeight : uvYOffset;

			unsigned int vertexOffset = (y * m_mapWidth + x) * 4;
			m_vertices[vertexOffset] =		// 0, bottom-left
			{
				XMFLOAT3(
					(m_tileset->GetTileWidth() / 2.0f * -1.0f + xPosition) * m_scale + m_position.x,
					(m_tileset->GetTileHeight() / 2.0f * -1.0f + yPosition) * m_scale + m_position.y,
					m_position.z
					),
				XMFLOAT2(uvLeft, uvBottom)
			};
			m_vertices[vertexOffset + 1] =		// 1, bottom-right
			{
				XMFLOAT3(
					(m_tileset->GetTileWidth() / 2.0f + xPosition) * m_scale + m_position.x,
					(m_tileset->GetTileHeight() / 2.0f * -1.0f + yPosition) * m_scale + m_position.y,
					m_position.z
					),
				XMFLOAT2(uvRight, uvBottom)
			};
			m_vertices[vertexOffset + 2] =		// 2, top-right
			{
				XMFLOAT3(
					(m_tileset->GetTileWidth() / 2.0f + xPosition) * m_scale + m_position.x,
					(m_tileset->GetTileHeight() / 2.0f + yPosition) * m_scale + m_position.y,
					m_position.z
					),
				XMFLOAT2(uvRight, uvTop)
			};
			m_vertices[vertexOffset + 3] =		// 3, top-left
			{
				XMFLOAT3(
					(m_tileset->GetTileWidth() / 2.0f * -1.0f + xPosition) * m_scale + m_position.x,
					(m_tileset->GetTileHeight() / 2.0f + yPosition) * m_scale + m_position.y,
					m_position.z
					),
				XMFLOAT2(uvLeft, uvTop)
			};
		}
	}
}
