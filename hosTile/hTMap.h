#pragma once

#include "hTSprite.h"
#include "Other/json_fwd.hpp"
#include <string>

namespace hosTile { class hTTileset; }

// A map, which consists of a tileset and json description of the map, potentially consisting of
// multiple layers of tiles.
namespace hosTile
{
	struct hTMapLayer
	{
	public:
		std::vector<unsigned int> m_data;		// gid per tile
		std::vector<VertexPositionTex> m_vertices;
	};

	class hTMap : public hTSprite
	{
	public:
		hTMap(
			const hTTileset& tileset, const nlohmann::json& mapJson,
			DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f });

		void Render(hosTile::hTRenderer& renderer);

		ID3D11ShaderResourceView* GetTexture() const;
		unsigned int GetNumVertices() const;
		const VertexPositionTex* GetVertices() const;

		// Fill out vertex data from each layer.
		void FillVertices(VertexPositionTex* buffer) const;

	private:
		// Update the vertices' data after the position has changed.
		void UpdateVertices();

		const hTTileset* m_tileset;

		unsigned int m_mapWidth;		// in tiles
		unsigned int m_mapHeight;		// in tiles

		std::vector<hTMapLayer> m_layers;

		// Shrink the UVs a bit to prevent seams from showing.
		static const float UVSeamSlack;
	};
}
