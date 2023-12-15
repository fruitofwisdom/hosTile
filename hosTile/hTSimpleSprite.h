#pragma once

#include <string>
#include "Other/DeviceResources.h"
#include "hTSprite.h"

// An hTSimpleSprite is the simplest type of sprite, as its name implies. It is used to render a
// single image without any notion of tiles.
namespace hosTile
{
	class hTSimpleSprite : public hTSprite
	{
	public:
		hTSimpleSprite(
			const DX::DeviceResources* deviceResources, std::string spriteFilename,
			DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f });

		void Render(hosTile::hTRenderer& renderer);

		ID3D11ShaderResourceView* GetTexture() const;
		unsigned int GetNumVertices() const;
		const VertexPositionTex* GetVertices() const;

		// Replace the sprite with a new one.
		void SetSprite(const DX::DeviceResources* deviceResources, std::string spriteFilename);

	protected:
		// Update the vertices' data after the position has changed.
		virtual void UpdateVertices();

		std::string m_spriteFilename;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

		VertexPositionTex m_vertices[4];
	};
}
