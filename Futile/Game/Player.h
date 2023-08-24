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

		hosTile::hTRegion GetCollision() const;
		hosTile::hTRegion GetHitBox() const;
		hosTile::hTRegion GetHurtBox() const;

	private:
		// Play a particular animation based on the player's angle in the range 0.0f to 360.0f.
		void PlayAnimationForDirection(
			std::string leftAnimation, std::string downAnimation, std::string rightAnimation, std::string upAnimation, bool looping = true);

		enum PlayerState
		{
			PS_Attack,
			PS_Idle,
			PS_Walk
		};

		PlayerState m_playerState;
		float m_facingAngle;		// in degrees

		DirectX::XMFLOAT3 m_walkingTarget;

		static const float MovementSpeed;		// pixels-per-second
		static const float NearTarget;		// distance in world space
	};
}
