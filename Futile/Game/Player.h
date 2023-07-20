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
		enum PlayerState
		{
			PS_Idle,
			PS_Walking
		};

		PlayerState m_playerState;

		DirectX::XMFLOAT3 m_walkingTarget;

		static const float MovementSpeed;		// pixels-per-second
		static const float NearTarget;		// distance in world space
	};
}
