#pragma once

#include "hosTile/hTRenderer.h"
#include "hosTile/hTTextBox.h"

namespace hosTileSample
{
	class UI
	{
	public:
		UI(hosTile::hTRenderer& renderer, const hosTile::hTFont* font, const wchar_t* version);

		void Update();
		void Render();

		void SetScale(float scale);

		void SetDebugText(const wchar_t* text);

	private:
		hosTile::hTRenderer* m_renderer;

		std::unique_ptr<hosTile::hTTextBox> m_debug;
		std::unique_ptr<hosTile::hTTextBox> m_version;
	};
}
