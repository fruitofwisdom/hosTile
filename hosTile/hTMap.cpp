#include "pch.h"
#include "hTMap.h"

#include "hTRenderer.h"
#include "hTTileset.h"
#include "Other/json.hpp"

using namespace DirectX;
using namespace hosTile;

const float hTMap::UVSeamSlack = 0.00005f;

hTMap::hTMap(
	const hTTileset* tileset, const nlohmann::json& mapJson,
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

void hTMap::Render(hTRenderer& renderer)
{
	UpdateVertices();
	renderer.AddSprite(this);
}

ID3D11ShaderResourceView* hTMap::GetTexture() const
{
	return m_tileset->GetTexture();
}

unsigned int hTMap::GetNumVertices() const
{
	return (unsigned int)m_mapData.size() * 4;
}

const VertexPositionTex* hTMap::GetVertices() const
{
	return &m_vertices[0];
}

void hTMap::UpdateVertices()
{
	for (unsigned int y = 0; y < m_mapHeight; ++y)
	{
		for (unsigned int x = 0; x < m_mapWidth; ++x)
		{
			unsigned int gid = m_mapData[y * m_mapWidth + x];

			// The top three bits are for flipping flags and the tiles are 1-indexed.
			unsigned int tileNum = (gid & 0x1FFFFFFF) - 1;
			float tileXPosition = x * m_tileset->GetTileWidth()
				+ m_tileset->GetTileWidth() / 2.0f
				- (m_mapWidth * m_tileset->GetTileWidth() / 2.0f);
			float tileYPosition = y * m_tileset->GetTileHeight() * -1.0f
				- m_tileset->GetTileHeight() / 2.0f
				+ (m_mapHeight * m_tileset->GetTileHeight() / 2.0f);
			bool tileXFlip = gid & 0x80000000;
			bool tileYFlip = gid & 0x40000000;
			bool tileDiagonalFlip = gid & 0x20000000;

			// Calculate UV-coordinates.
			float uvTileWidth = (float)m_tileset->GetTileWidth() / m_tileset->GetImageWidth();
			float uvTileHeight = (float)m_tileset->GetTileHeight() / m_tileset->GetImageHeight();
			float uvXOffset = (float)m_tileset->GetTileXOffset(tileNum) / m_tileset->GetImageWidth();
			float uvYOffset = (float)m_tileset->GetTileYOffset(tileNum) / m_tileset->GetImageHeight();
			// Bring in the UV borders an imperceptible amount to prevent seams from showing.
			float uvLeft = uvXOffset + UVSeamSlack;
			float uvRight = uvXOffset + uvTileWidth - UVSeamSlack;
			float uvBottom = uvYOffset + uvTileHeight - UVSeamSlack;
			float uvTop = uvYOffset + UVSeamSlack;

			unsigned int vertexOffset = (y * m_mapWidth + x) * 4;
			m_vertices[vertexOffset] =		// 0, bottom-left
			{
				XMFLOAT3(
					(m_tileset->GetTileWidth() / 2.0f * -1.0f + tileXPosition)
						* (m_xFlip ? -1.0f : 1.0f) * m_scale + m_position.x,
					(m_tileset->GetTileHeight() / 2.0f * -1.0f + tileYPosition)
						* (m_yFlip ? -1.0f : 1.0f) * m_scale + m_position.y,
					m_position.z
					),
				XMFLOAT2(uvLeft, uvBottom)
			};
			m_vertices[vertexOffset + 1] =		// 1, bottom-right
			{
				XMFLOAT3(
					(m_tileset->GetTileWidth() / 2.0f + tileXPosition)
						* (m_xFlip ? -1.0f : 1.0f) * m_scale + m_position.x,
					(m_tileset->GetTileHeight() / 2.0f * -1.0f + tileYPosition)
						* (m_yFlip ? -1.0f : 1.0f) * m_scale + m_position.y,
					m_position.z
					),
				XMFLOAT2(uvRight, uvBottom)
			};
			m_vertices[vertexOffset + 2] =		// 2, top-right
			{
				XMFLOAT3(
					(m_tileset->GetTileWidth() / 2.0f + tileXPosition)
						* (m_xFlip ? -1.0f : 1.0f) * m_scale + m_position.x,
					(m_tileset->GetTileHeight() / 2.0f + tileYPosition)
						* (m_yFlip ? -1.0f : 1.0f) * m_scale + m_position.y,
					m_position.z
					),
				XMFLOAT2(uvRight, uvTop)
			};
			m_vertices[vertexOffset + 3] =		// 3, top-left
			{
				XMFLOAT3(
					(m_tileset->GetTileWidth() / 2.0f * -1.0f + tileXPosition)
						* (m_xFlip ? -1.0f : 1.0f) * m_scale + m_position.x,
					(m_tileset->GetTileHeight() / 2.0f + tileYPosition)
						* (m_yFlip ? -1.0f : 1.0f) * m_scale + m_position.y,
					m_position.z
					),
				XMFLOAT2(uvLeft, uvTop)
			};

			// Apply diagonal-flip, x-flip, and y-flip.
			if (tileDiagonalFlip)
			{
				SwapUVs(m_vertices[vertexOffset].tex, m_vertices[vertexOffset + 2].tex);
			}
			if (tileXFlip)
			{
				SwapUVs(m_vertices[vertexOffset].tex, m_vertices[vertexOffset + 1].tex);
				SwapUVs(m_vertices[vertexOffset + 2].tex, m_vertices[vertexOffset + 3].tex);
			}
			if (tileYFlip)
			{
				SwapUVs(m_vertices[vertexOffset].tex, m_vertices[vertexOffset + 3].tex);
				SwapUVs(m_vertices[vertexOffset + 1].tex, m_vertices[vertexOffset + 2].tex);
			}
		}
	}
}
