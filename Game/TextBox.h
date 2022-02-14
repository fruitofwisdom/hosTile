#pragma once

#include "hosTile/hTMap.h"
#include "hosTile/hTRenderer.h"
#include "hosTile/hTTileset.h"

namespace hosTileSample
{
	class TextBox
	{
	public:
		TextBox(const hosTile::hTTileset& tileset, std::string mapFilename);

		void Render(hosTile::hTRenderer& renderer);

		hosTile::hTSprite* GetSprite() const;

		void SetPosition(DirectX::XMFLOAT3 position);

		unsigned int GetWidth() const;		// in pixels
		unsigned int GetHeight() const;		// in pixels

		void SetScale(float scale);

	private:
		std::unique_ptr<hosTile::hTMap> m_textBoxMap;
	};
}
