#pragma once

#include "GameObject.h"

namespace hosTileSample
{
	class Player : public GameObject
	{
	public:
		Player(std::shared_ptr<hosTile::hosTileSprite> sprite);

		void Update(const DX::StepTimer& timer);

	private:
		static const float MovementSpeed;		// pixels-per-second
	};
}
