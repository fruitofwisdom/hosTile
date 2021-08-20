#pragma once

#include "DirectX\StepTimer.h"
#include "hosTile\hosTileRenderer.h"

// The game instance.
namespace hosTileSample
{
	class Game
	{
	public:
		Game(const std::shared_ptr<hosTile::hosTileRenderer> renderer);
		~Game();

		void Update(DX::StepTimer const& timer);

	private:
		// TODO: Replace with your own content renderers.
		std::shared_ptr<hosTile::hosTileRenderer> m_renderer;
	};
}
