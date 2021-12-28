#pragma once

#include "hosTile\hTSprite.h"
#include "StepTimer.h"

namespace hosTileSample
{
	class GameObject
	{
	public:
		GameObject(std::shared_ptr<hosTile::hTSprite> sprite);
		virtual ~GameObject() {}

		virtual void Update(const DX::StepTimer& timer) {}

		DirectX::XMFLOAT3 GetPosition() const;
		std::shared_ptr<hosTile::hTSprite> GetSprite() const;

	protected:
		std::shared_ptr<hosTile::hTSprite> m_sprite;
	};
}
