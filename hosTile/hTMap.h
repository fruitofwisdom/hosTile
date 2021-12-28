#pragma once

#include <string>
#include "hosTile/Other/json_fwd.hpp"
#include "hTSprite.h"
#include "hTTileset.h"

// A map, which consists of a tileset and json description of the map.
namespace hosTile
{
	class hTMap : public hTSprite
	{
	public:
		hTMap(
			const std::shared_ptr<hTTileset>& tileset, const nlohmann::json& mapJson,
			DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f });

		void Update();

		ID3D11ShaderResourceView* GetTexture() const;
		unsigned int GetNumVertices() const;
		const VertexPositionTex* GetVertices() const;

	private:
		// Update the vertices' data after the position has changed.
		void UpdateVertices();

		const std::shared_ptr<hTTileset> m_tileset;

		std::vector<unsigned int> m_mapData;
		unsigned int m_mapWidth;		// in tiles
		unsigned int m_mapHeight;		// in tiles

		std::vector<VertexPositionTex> m_vertices;
	};
}
