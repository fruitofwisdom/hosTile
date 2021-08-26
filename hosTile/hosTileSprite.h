#pragma once

#include <string>
#include "DirectX/DeviceResources.h"
#include "hosTileShaderStructures.h"

// An instance of a sprite which contains a texture, position, and the vertices to render it. A
// sprite may contain a number of "sub-"sprites in a sprite sheet.
namespace hosTile
{
	class hosTileSprite
	{
	public:
		hosTileSprite(
			const std::shared_ptr<DX::DeviceResources>& deviceResources, std::wstring spriteFilename,
			DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f },
			unsigned int currentSubSprite = 0, unsigned int numSubSprites = 1
			);
		virtual ~hosTileSprite() {}

		const std::wstring GetSpriteFilename() const;
		ID3D11ShaderResourceView* GetTexture() const;

		DirectX::XMFLOAT3 GetPosition() const;
		void SetPosition(DirectX::XMFLOAT3 position);
		const VertexPositionTex* GetVertices() const;

		void SetCurrentSubSprite(unsigned int currentSubSprite);

		void SetXFlip(bool xFlip);
		void SetYFlip(bool yFlip);

	private:
		// Update the vertices' data after the position has changed.
		void UpdateVertices();

		const std::wstring m_spriteFilename;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
		UINT m_width, m_height;

		DirectX::XMFLOAT3 m_position;
		VertexPositionTex m_vertices[4];

		// A sprite may be a "sheet" or atlas of sprites of which we render only a part.
		unsigned int m_currentSubSprite;
		unsigned int m_numSubSprites;

		bool m_xFlip;
		bool m_yFlip;
	};
}
