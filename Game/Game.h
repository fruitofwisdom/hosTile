#pragma once

#include "Camera.h"
#include "DirectX\StepTimer.h"
#include "hosTile\hosTileRenderer.h"
#include "Player.h"

// The game instance.
namespace hosTileSample
{
	class Game final
	{
	public:
		Game(std::shared_ptr<hosTile::hosTileRenderer> renderer);

		void Update(const DX::StepTimer& timer);

	private:
		std::shared_ptr<Player> m_player;
		std::unique_ptr<Camera> m_camera;
	};
}
