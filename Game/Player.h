#pragma once

#include "GameObject.h"
#include "hosTile/hTTileSprite.h"

namespace hosTileSample
{
	class Player : public GameObject
	{
	public:
		Player(std::unique_ptr<hosTile::hTTileSprite> sprite);

		void Update(const DX::StepTimer& timer);

	private:
		static const float MovementSpeed;		// pixels-per-second
	};
}
