#pragma once

#include "DirectX\StepTimer.h"
#include "hosTile\Sample3DSceneRenderer.h"

// The game instance.
namespace hosTileSample
{
	class Game
	{
	public:
		Game(const std::shared_ptr<hosTile::Sample3DSceneRenderer> sceneRenderer);
		~Game();

		void Update(DX::StepTimer const& timer);

	private:
		// TODO: Replace with your own content renderers.
		std::shared_ptr<hosTile::Sample3DSceneRenderer> m_sceneRenderer;
	};
}
