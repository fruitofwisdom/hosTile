#pragma once

#include "..\hosTile\hTFont.h"
#include "..\hosTile\hTTextBox.h"

// The UI represents two lines of text in the bottom-right corner of the screen: some optional
// debug text and the version number.
namespace Futile
{
	class UI
	{
	public:
		UI(std::string fontFilename, const wchar_t* version);

		void Update();
		void Render();

		void SetDebugText(const wchar_t* text);

	private:
		std::unique_ptr<hosTile::hTFont> m_font;

		std::unique_ptr<hosTile::hTTextBox> m_debug;
		std::unique_ptr<hosTile::hTTextBox> m_version;
	};
}
