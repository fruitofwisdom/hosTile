#pragma once

#include "DirectX\StepTimer.h"
#include "hosTile\hosTileRenderer.h"
#include "Player.h"

// The game instance.
namespace hosTileSample
{
	class Game
	{
	public:
		Game(std::shared_ptr<hosTile::hosTileRenderer> renderer);
		virtual ~Game() {}

		void Update(const DX::StepTimer& timer);

	private:
		std::unique_ptr<Player> m_player;
	};
}
