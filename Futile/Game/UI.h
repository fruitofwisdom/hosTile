#pragma once

#include "..\hosTile\hTFont.h"
#include "..\hosTile\hTTextBox.h"

namespace Futile
{
	class UI
	{
	public:
		UI(std::string fontFilename, const wchar_t* version);

		void Update();
		void Render();

		void SetScale(float scale);

		void SetDebugText(const wchar_t* text);

	private:
		std::unique_ptr<hosTile::hTFont> m_font;

		std::unique_ptr<hosTile::hTTextBox> m_debug;
		std::unique_ptr<hosTile::hTTextBox> m_version;
	};
}
