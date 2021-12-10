#pragma once

#include <string>
#include "Other/DeviceResources.h"
#include "hTSprite.h"

// An instance of a sprite which contains a texture, position, and the vertices to render it. A
// sprite may contain a number of "sub-"sprites in a sprite sheet.
namespace hosTile
{
	class hTSimpleSprite : public hTSprite
	{
	public:
		hTSimpleSprite(
			const std::shared_ptr<DX::DeviceResources>& deviceResources, std::string spriteFilename,
			DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f });

		void Update();

		ID3D11ShaderResourceView* GetTexture() const;
		const VertexPositionTex* GetVertices() const;

	private:
		// Update the vertices' data after the position has changed.
		void UpdateVertices();

		const std::string m_spriteFilename;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

		VertexPositionTex m_vertices[4];
	};
}
