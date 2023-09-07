#include "pch.h"
#include "hTMap.h"

#include "hTRenderer.h"
#include "hTTileset.h"
#include "Other/json.hpp"

using namespace DirectX;
using namespace hosTile;
using namespace nlohmann;

const float hTMap::UVSeamSlack = 0.00005f;

hTMap::hTMap(
	const hTTileset& tileset, const json& mapJson,
	DirectX::XMFLOAT3 position)
:	hTSprite(position),
	m_tileset(&tileset)
{
	json layers = mapJson["layers"];
	for (json layer : layers)
	{
		if (layer.contains("data"))
		{
			hTMapLayer mapLayer;
			for (unsigned int gid : layer["data"])
			{
				mapLayer.m_data.push_back(gid);
				mapLayer.m_vertices.push_back({ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
				mapLayer.m_vertices.push_back({ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
				mapLayer.m_vertices.push_back({ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
				mapLayer.m_vertices.push_back({ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
			}
			m_layers.push_back(mapLayer);
		}
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
	unsigned int numVertices = 0;
	for (int i = 0; i < m_layers.size(); ++i)
	{
		numVertices += (unsigned int)m_layers[i].m_vertices.size();
	}
	return numVertices;
}

const VertexPositionTex* hTMap::GetVertices() const
{
	// hTMap doesn't keep vertex data contiguously, instead ask hTRenderer to use FillVertices.
	return nullptr;
}

void hTMap::FillVertices(VertexPositionTex* buffer) const
{
	unsigned int numVertices = 0;
	for (int i = 0; i < m_layers.size(); ++i)
	{
		memcpy(&buffer[numVertices], &m_layers[i].m_vertices[0], sizeof(VertexPositionTex) * m_layers[i].m_vertices.size());
		numVertices += (unsigned int)m_layers[i].m_vertices.size();
	}
}

void hTMap::UpdateVertices()
{
	for (int i = 0; i < m_layers.size(); ++i)
	{
		for (unsigned int y = 0; y < m_mapHeight; ++y)
		{
			for (unsigned int x = 0; x < m_mapWidth; ++x)
			{
				unsigned int gid = m_layers[i].m_data[y * m_mapWidth + x];

				// A gid of 0 means no tile.
				if (gid == 0)
				{
					continue;
				}

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
				m_layers[i].m_vertices[vertexOffset] =		// 0, bottom-left
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
				m_layers[i].m_vertices[vertexOffset + 1] =		// 1, bottom-right
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
				m_layers[i].m_vertices[vertexOffset + 2] =		// 2, top-right
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
				m_layers[i].m_vertices[vertexOffset + 3] =		// 3, top-left
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
					SwapUVs(m_layers[i].m_vertices[vertexOffset].tex, m_layers[i].m_vertices[vertexOffset + 2].tex);
				}
				if (tileXFlip)
				{
					SwapUVs(m_layers[i].m_vertices[vertexOffset].tex, m_layers[i].m_vertices[vertexOffset + 1].tex);
					SwapUVs(m_layers[i].m_vertices[vertexOffset + 2].tex, m_layers[i].m_vertices[vertexOffset + 3].tex);
				}
				if (tileYFlip)
				{
					SwapUVs(m_layers[i].m_vertices[vertexOffset].tex, m_layers[i].m_vertices[vertexOffset + 3].tex);
					SwapUVs(m_layers[i].m_vertices[vertexOffset + 1].tex, m_layers[i].m_vertices[vertexOffset + 2].tex);
				}
			}
		}
	}
}
