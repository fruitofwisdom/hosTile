#pragma once

#include "GameObject.h"
#include "..\hosTile\hTTileSprite.h"

namespace Futile
{
	class Player : public GameObject
	{
	public:
		Player(std::unique_ptr<hosTile::hTTileSprite> sprite);

		void Update(const DX::StepTimer& timer);
		void Render(hosTile::hTRenderer& renderer);

	private:
		static const float MovementSpeed;		// pixels-per-second
	};
}
