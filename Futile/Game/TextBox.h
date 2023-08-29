#pragma once

#include "..\hosTile\hTFont.h"
#include "..\hosTile\hTMap.h"
#include "..\hosTile\hTTileset.h"
#include "..\hosTile\hTTextBox.h"

namespace Futile
{
	class TextBox
	{
	public:
		TextBox(std::string tilesetFilename, std::string boxFilename, std::string fontFilename, const wchar_t* text);

		void Render();

		void SetPosition(DirectX::XMFLOAT3 position);

		unsigned int GetWidth() const;		// in pixels
		unsigned int GetHeight() const;		// in pixels

		void SetScale(float scale);

		// Are x and y contained within the text box's box?
		bool Contains(float x, float y) const;

	private:
		std::unique_ptr<hosTile::hTTileset> m_tileset;
		std::unique_ptr<hosTile::hTMap> m_box;

		std::unique_ptr<hosTile::hTFont> m_font;
		std::unique_ptr<hosTile::hTTextBox> m_text;
	};
}
