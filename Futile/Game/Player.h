#pragma once

#include "GameObject.h"
#include "..\hosTile\hTAnimatedSprite.h"

namespace Futile
{
	class Player : public GameObject
	{
	public:
		Player();

		void Update(const DX::StepTimer& timer);
		void Render(hosTile::hTRenderer& renderer);

	private:
		// Play a particular animation based on an atan2 angle in the range 0.0f to 360.0f.
		void PlayAnimationForDirection(
			float angle,
			std::string leftAnimation, std::string downAnimation, std::string rightAnimation, std::string upAnimation);

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
