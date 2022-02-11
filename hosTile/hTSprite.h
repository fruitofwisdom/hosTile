#pragma once

#include "hTShaderStructures.h"

// An hTSprite is the virtual base class of sprites used in hosTile and only contains the
// prototypes that derived sprites need to implement. These are meant to present a unified
// interface and to work seamlessly with hTRenderer.
namespace hosTile
{
	class hTSprite
	{
	public:
		hTSprite(DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f });
		virtual ~hTSprite() {}

		// Game engines should use this to update vertices or other per-frame operations.
		virtual void Update() {}

		// hTRenderer uses these to actually render the sprite.
		virtual ID3D11ShaderResourceView* GetTexture() const = 0;
		virtual unsigned int GetNumVertices() const = 0;
		virtual const VertexPositionTex* GetVertices() const = 0;

		DirectX::XMFLOAT3 GetPosition() const;
		void SetPosition(DirectX::XMFLOAT3 position);

		unsigned int GetWidth() const;		// in pixels
		unsigned int GetHeight() const;		// in pixels

		void SetScale(float scale);
		void SetXFlip(bool xFlip);
		void SetYFlip(bool yFlip);

	protected:
		// Swaps the values of two UVs. Handy for applying x-flip, y-flip, etc.
		void SwapUVs(DirectX::XMFLOAT2& uv1, DirectX::XMFLOAT2& uv2) const;

		DirectX::XMFLOAT3 m_position;

		unsigned int m_width;		// in pixels
		unsigned int m_height;		// in pixels

		float m_scale;		// scale only applies to width and height
		bool m_xFlip;
		bool m_yFlip;
	};
}
