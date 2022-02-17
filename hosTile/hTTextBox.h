#pragma once

#include <string>
#include <vector>
#include "hTSprite.h"

namespace hosTile { class hTFont; }

// A text box, which consists of a font and a string to display. If a text box's bounding
// dimensions are provided, it will word wrap to fit. Justification is presently left-justified.
namespace hosTile
{
	class hTTextBox : public hTSprite
	{
	public:
		hTTextBox(
			const hTFont* font, const wchar_t* text,
			DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f },
			unsigned int boundingWidth = 0, unsigned int boundingHeight = 0);

		void Render(hosTile::hTRenderer& renderer);

		ID3D11ShaderResourceView* GetTexture() const;
		unsigned int GetNumVertices() const;
		const VertexPositionTex* GetVertices() const;

		const hTFont* GetFont() const;

		void SetBounds(unsigned int boundingWidth, unsigned int boundingHeight);

	private:
		int DistanceToWhitespace(int from) const;

		// Update the vertices' data after the position or text has changed.
		void UpdateVertices();

		const hTFont* m_font;
		std::wstring m_text;

		unsigned int m_boundingWidth;		// in pixels
		unsigned int m_boundingHeight;		// in pixels

		std::vector<VertexPositionTex> m_vertices;
		unsigned int m_verticesToRender;
	};
}
