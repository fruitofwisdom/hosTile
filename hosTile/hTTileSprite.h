#pragma once

#include "hTSprite.h"
#include "hTTileset.h"

namespace hosTile
{
	class hTTileSprite : public hTSprite
	{
	public:
		hTTileSprite(
			const std::shared_ptr<hTTileset>& tileset, unsigned int tileNum,
			DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f });

		void Update();

		ID3D11ShaderResourceView* GetTexture() const;
		unsigned int GetNumVertices() const;
		const VertexPositionTex* GetVertices() const;

	private:
		// Update the vertices' data after the position has changed.
		void UpdateVertices();

		const std::shared_ptr<hTTileset> m_tileset;
		int m_tileNum;

		VertexPositionTex m_vertices[4];
	};
}
