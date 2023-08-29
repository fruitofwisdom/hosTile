#pragma once

#include "..\hosTile\hTMap.h"
#include "..\hosTile\hTTileset.h"
#include "Player.h"

namespace Futile
{
	class Level
	{
	public:
		Level(std::string levelFilename);

		void Update(const DX::StepTimer& timer);
		void Render();

		Player& GetPlayer();

		bool IsLoaded() const;

	private:
		bool m_loaded;

		std::unique_ptr<hosTile::hTMap> m_map;
		std::unique_ptr<hosTile::hTTileset> m_tileset;

		std::unique_ptr<Player> m_player;
	};
}
