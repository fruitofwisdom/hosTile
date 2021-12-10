#pragma once

#include "hosTileTileset.h"
#include "htSprite.h"

namespace hosTile
{
	class hosTileTileSprite : public hTSprite
	{
	public:
		hosTileTileSprite(
			const std::shared_ptr<hosTileTileset>& tileset, int tileNum,
			DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f });

		void Update();

		ID3D11ShaderResourceView* GetTexture() const;
		const VertexPositionTex* GetVertices() const;

	private:
		// Update the vertices' data after the position has changed.
		void UpdateVertices();

		const std::shared_ptr<hosTileTileset> m_tileset;
		int m_tileNum;

		VertexPositionTex m_vertices[4];
	};
}
