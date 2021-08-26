#pragma once

#include "DirectX\StepTimer.h"
#include "hosTile\hosTileSprite.h"

namespace hosTileSample
{
	class Player
	{
	public:
		Player(std::shared_ptr<hosTile::hosTileSprite> sprite);
		virtual ~Player() {}

		void Update(const DX::StepTimer& timer);

	private:
		static const float MovementSpeed;		// pixels-per-second

		std::shared_ptr<hosTile::hosTileSprite> m_sprite;
	};
}
