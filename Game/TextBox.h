#pragma once

#include "hosTile/hTMap.h"
#include "hosTile/hTRenderer.h"
#include "hosTile/hTTextBox.h"

namespace hosTileSample
{
	class TextBox
	{
	public:
		TextBox(
			const hosTile::hTTileset* boxTileset, std::string boxFilename,
			const hosTile::hTFont* font, const wchar_t* text);

		void Render(hosTile::hTRenderer& renderer);

		void SetPosition(DirectX::XMFLOAT3 position);

		unsigned int GetWidth() const;		// in pixels
		unsigned int GetHeight() const;		// in pixels

		void SetScale(float scale);

	private:
		std::unique_ptr<hosTile::hTMap> m_box;
		std::unique_ptr<hosTile::hTTextBox> m_text;
	};
}