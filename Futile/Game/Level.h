#pragma once

#include "..\hosTile\hTMap.h"
#include "..\hosTile\hTTileset.h"
#include <list>
#include "Player.h"

namespace Futile
{
	class Level
	{
	public:
		Level(std::string levelFilename);

		void Update(const DX::StepTimer& timer);
		void Render();

		const Player* GetPlayer() const;

	private:
		bool m_loaded;

		std::unique_ptr<hosTile::hTTileset> m_tileset;
		std::unique_ptr<hosTile::hTMap> m_map;

		Player* m_player;
		std::list<std::unique_ptr<GameObject>> m_gameObjects;
	};
}
