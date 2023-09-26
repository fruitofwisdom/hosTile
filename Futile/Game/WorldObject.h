#pragma once

#include "GameObject.h"
#include "../hosTile/hTAnimatedSprite.h"
#include <string>

// A WorldObject is a specialized type of GameObject that is visible in the world, such as the
// Player or an Enemy. These have animated sprites with associated regions, facing, etc.
namespace Futile
{
	class WorldObject : public GameObject
	{
	public:
		WorldObject(std::string animation, DirectX::XMFLOAT3 position, float scale = 1.0f);

		void Update(const DX::StepTimer& timer);
		void Render(hosTile::hTRenderer& renderer);

		hosTile::hTRegion GetCollision() const;
		hosTile::hTRegion GetHitBox() const;
		hosTile::hTRegion GetHurtBox() const;

	protected:
		// Move to a new position, considering collision.
		void Move(DirectX::XMFLOAT3 position);

		// Play a particular animation based on the facing angle in the range 0.0f to 360.0f.
		void PlayAnimationForDirection(
			std::string leftAnimation, std::string downAnimation, std::string rightAnimation, std::string upAnimation, bool looping = true);

		std::unique_ptr<hosTile::hTAnimatedSprite> m_sprite;

		float m_facingAngle;		// in degrees
	};
}
