#pragma once

#include "hTSprite.h"
#include "hTTileset.h"

// An hTTileSprite is a sprite that renders an individual tile in an hTTileset. It does not
// contain any image information itself other than a reference into its tileset.
namespace hosTile
{
	class hTTileSprite : public hTSprite
	{
	public:
		hTTileSprite(
			const hTTileset* tileset, unsigned int tileNum,
			DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f });

		void Render(hosTile::hTRenderer& renderer);

		ID3D11ShaderResourceView* GetTexture() const;
		unsigned int GetNumVertices() const;
		const VertexPositionTex* GetVertices() const;

	private:
		// Update the vertices' data after the position has changed.
		void UpdateVertices();

		const hTTileset* m_tileset;
		int m_tileNum;

		VertexPositionTex m_vertices[4];
	};
}
