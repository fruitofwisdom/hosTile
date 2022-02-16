#pragma once

#include <string>
#include <vector>
#include "hTSprite.h"

namespace hosTile { class hTFont; }

// A text box, which consists of a font and a string to display.
namespace hosTile
{
	class hTTextBox : public hTSprite
	{
	public:
		// TODO: Add bounds. (And justification?)
		hTTextBox(const hTFont* font, const wchar_t* text);

		// Unlike other sprites, fonts require a position to be provided each call to Render.
		void Render(hosTile::hTRenderer& renderer, DirectX::XMFLOAT3 position);

		ID3D11ShaderResourceView* GetTexture() const;
		unsigned int GetNumVertices() const;
		const VertexPositionTex* GetVertices() const;

	private:
		// Unlike other sprites, fonts require a position to be provided each call to Render.
		// Therefore, hide this simple inherited version.
		void Render(hosTile::hTRenderer& renderer) {}

		// Update the vertices' data after the position or text has changed.
		void UpdateVertices();

		const hTFont* m_font;

		std::wstring m_text;

		std::vector<VertexPositionTex> m_vertices;
	};
}
