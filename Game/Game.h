#pragma once

#include "Camera.h"
#include "hosTile\hTRenderer.h"
#include "Other\StepTimer.h"
#include "Player.h"

// The game instance.
namespace hosTileSample
{
	class Game final
	{
	public:
		Game(std::shared_ptr<hosTile::hTRenderer> renderer);

		void Update(const DX::StepTimer& timer);

	private:
		std::shared_ptr<Player> m_player;
		std::unique_ptr<Camera> m_camera;
	};
}
