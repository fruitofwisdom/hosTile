#pragma once

#include "hosTile\hosTileSprite.h"
#include "Other\StepTimer.h"

namespace hosTileSample
{
	class GameObject
	{
	public:
		GameObject(std::shared_ptr<hosTile::hosTileSprite> sprite);
		virtual ~GameObject() {}

		virtual void Update(const DX::StepTimer& timer) {}

		DirectX::XMFLOAT3 GetPosition() const;
		std::shared_ptr<hosTile::hosTileSprite> GetSprite() const;

	protected:
		std::shared_ptr<hosTile::hosTileSprite> m_sprite;
	};
}
