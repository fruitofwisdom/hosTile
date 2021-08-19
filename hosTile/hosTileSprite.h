#pragma once

#include <string>
#include "DirectX/DeviceResources.h"
#include "hosTileShaderStructures.h"

// An instance of a sprite which contains a texture, position, and the vertices to render it.
namespace hosTile
{
	class hosTileSprite
	{
	public:
		hosTileSprite(
			const std::shared_ptr<DX::DeviceResources>& deviceResources, std::wstring spriteFilename,
			DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f });
		~hosTileSprite();

		const std::wstring GetSpriteFilename() const;
		ID3D11ShaderResourceView* GetTexture() const;

		void SetPosition(DirectX::XMFLOAT3 position);
		const VertexPositionTex* GetVertices() const;

	private:
		// Update the vertices' data after the position has changed.
		void UpdateVertices();

		const std::wstring m_spriteFilename;
		ID3D11ShaderResourceView* m_texture;
		UINT m_width, m_height;

		DirectX::XMFLOAT3 m_position;
		VertexPositionTex m_vertices[4];
	};
}
