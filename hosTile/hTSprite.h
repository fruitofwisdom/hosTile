#pragma once

#include "hosTileShaderStructures.h"

// UPDATE THIS
// An instance of a sprite which contains a texture, position, and the vertices to render it. A
// sprite may contain a number of "sub-"sprites in a sprite sheet.
namespace hosTile
{
	class hTSprite
	{
	public:
		hTSprite(DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f });
		virtual ~hTSprite() {}

		// Game engines should use this to update vertices or other per-frame operations.
		virtual void Update() {}

		// hosTileRenderer uses these to actually render the sprite.
		virtual ID3D11ShaderResourceView* GetTexture() const = 0;
		virtual const VertexPositionTex* GetVertices() const = 0;

		DirectX::XMFLOAT3 GetPosition() const;
		void SetPosition(DirectX::XMFLOAT3 position);

		void SetScale(float scale);
		void SetXFlip(bool xFlip);
		void SetYFlip(bool yFlip);

	protected:
		DirectX::XMFLOAT3 m_position;

		unsigned int m_width;
		unsigned int m_height;

		// The scale of a sprite only applies to its width and height, not depth.
		float m_scale;
		bool m_xFlip;
		bool m_yFlip;
	};
}
