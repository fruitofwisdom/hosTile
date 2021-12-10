#pragma once

#include "GameObject.h"
#include "hosTile/hosTileTileSprite.h"

namespace hosTileSample
{
	class Player : public GameObject
	{
	public:
		Player(std::shared_ptr<hosTile::hosTileTileSprite> sprite);

		void Update(const DX::StepTimer& timer);

	private:
		static const float MovementSpeed;		// pixels-per-second
	};
}
