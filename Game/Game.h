#pragma once

#include "Camera.h"
#include "hosTile\hTMap.h"
#include "hosTile\hTRenderer.h"
#include "Player.h"
#include "StepTimer.h"

// The game instance.
namespace hosTileSample
{
	class Game final
	{
	public:
		Game(hosTile::hTRenderer* renderer);

		void Update(const DX::StepTimer& timer);

	private:
		static const float Scale;

		std::shared_ptr<hosTile::hTMap> m_map;
		std::unique_ptr<Player> m_player;
		std::unique_ptr<Camera> m_camera;
	};
}
